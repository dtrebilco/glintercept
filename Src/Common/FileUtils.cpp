/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include <OSDefines.h>
#include "FileUtils.h"
#include <CommonErrorLog.h>


USING_ERRORLOG

#ifdef GLI_BUILD_WINDOWS

#include <winbase.h>
#include <imagehlp.h>
const char FileUtils::dirSeparator = '\\';

///////////////////////////////////////////////////////////////////////////////
//
bool FileUtils::CreateFullDirectory(const string &directory)
{
  //Perhaps do string conversion?

  //Call built in function
  // (perhaps in future remove dependency on imagehlp.h?)
  if(MakeSureDirectoryPathExists(directory.c_str()))
  {
    return true;
  }
  
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FileUtils::CopyFile(const string &srcFile,const string &dstFile, bool overwrite)
{
  //Determine if the file is to be overwritten
  BOOL failIfExists = TRUE;
  if(overwrite)
  {
    failIfExists = FALSE;
  }

  //Copy the file
  if (::CopyFile(srcFile.c_str(),dstFile.c_str(),failIfExists) == 0)
  {
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FileUtils::SetFileProperties(const string &fileName,uint fileProperties)
{
  //Check the property
  if((fileProperties & FP_NORMAL) && 
     (fileProperties != FP_NORMAL))
  {
    LOGERR(("FileUtils::SetFileProperties - If the 'Normal' property is set, other properties cannot be set"));
    return false;
  }

  //Check the flag data
  if((fileProperties | (FP_NORMAL | FP_READONLY)) != (FP_NORMAL | FP_READONLY))
  {
    LOGERR(("FileUtils::SetFileProperties - Unknown flags passed in properties"));
    return false;
  }

  DWORD osFileAttributes = 0;

  //Get the OS file attribute flags
  if(fileProperties & FP_NORMAL)
  {
    osFileAttributes |= FILE_ATTRIBUTE_NORMAL;
  }
  if(fileProperties & FP_READONLY)
  {
    osFileAttributes |= FILE_ATTRIBUTE_READONLY;
  }

  //Set the attributes
  if(SetFileAttributes(fileName.c_str(),osFileAttributes) == 0)
  {
    return false;
  }

  return true;
}

#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

#include <fcntl.h>
#include <sys/stat.h>
#define BUFFER_COPY_SIZE 1024

const char FileUtils::dirSeparator = '/';

///////////////////////////////////////////////////////////////////////////////
//
bool FileUtils::CreateFullDirectory(const string &directory)
{
  string currDir = directory;
  
  //Replace all backslashes with forward slashes
  string::size_type replacePos = currDir.find('\\');
  while(replacePos != string::npos)
  {
    currDir[replacePos] = '/';
    replacePos = currDir.find('\\'); 
  }
  
  //Remove ending / if it exists
  if(currDir.length() > 1 && 
     currDir[currDir.length()-1] == '/')
  {
    currDir.erase(currDir.length()-1, 1);
  }

  //Check if the directory exists
  struct stat dirData;
  if(stat(currDir.c_str(), &dirData) == 0)
  {
    //If a file/directory exists, check that it is a directory
    if(S_ISDIR(dirData.st_mode))
    {
      return true; 
    }
    
    return false;
  }

  //Recursive call, one less directory
  string::size_type dirFindPos = currDir.rfind('/');
  if(dirFindPos != string::npos)
  {
    string newDir;
    newDir.assign(currDir, 0, dirFindPos);
    
    //Recurse with new stripped directory
    if(!CreateFullDirectory(newDir))
    {
      return false;
    }
  }

  //Create the actual directory
  if(mkdir(currDir.c_str(), 0755) < 0) //0755 Octal constant
  {
    LOGERR(("FileUtils::CreateFullDirectory - Failed to create directory \"%s\"", currDir.c_str()));
    return false; 
  }
  
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FileUtils::CopyFile(const string &srcFilename,const string &dstFilename, bool overwrite)
{
  int srcFile;
  int dstFile;
  
  //NOTE: This does not copy the file attributes as in the the win32 version
  //Open the source file
  srcFile = open(srcFilename.c_str(), O_RDONLY);
  if(srcFile < 0)
  {
    LOGERR(("FileUtils::CopyFile - Unable to open source file %s",srcFilename.c_str())); 
    return false;
  }
  
  //Open the destination file
  if(overwrite)
  {
    dstFile = open(dstFilename.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0666);  //0666 Octal constant
  }
  else
  {
    dstFile = open(dstFilename.c_str(), O_CREAT | O_EXCL | O_TRUNC | O_WRONLY, 0666);     
  }
  if(dstFile < 0)
  {
    close(srcFile);
    LOGERR(("FileUtils::CopyFile - Unable to open destination file %s",dstFilename.c_str())); 
    return false;
  }
  
  //Copy the file data
  bool copyingFile = true;
  char buffer[BUFFER_COPY_SIZE];
  
  //Loop while there is still data to copy
  while(copyingFile)
  {
    //Read into the buffer
    ssize_t srcBytes = read(srcFile, buffer, BUFFER_COPY_SIZE);
    if(srcBytes < 0)
    {
      LOGERR(("FileUtils::CopyFile - Error reading source file"));
      close(srcFile);
      close(dstFile);
      return false; 
    }
    
    //If there is still data to copy
    if(srcBytes == 0)
    {
      copyingFile = false; 
    }
    else
    {
      //Write the buffer to the destination file
      ssize_t writeBytes = write(dstFile, buffer, srcBytes);
      if(writeBytes <= 0)
      {
        LOGERR(("FileUtils::CopyFile - Error writing file data"));
        close(srcFile);
        close(dstFile);
        return false; 
      }
    }
  }
  
  //Close the files
  close(srcFile);
  close(dstFile);
  
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FileUtils::SetFileProperties(const string &fileName,uint fileProperties)
{
  //Check the property
  if((fileProperties & FP_NORMAL) && 
     (fileProperties != FP_NORMAL))
  {
    LOGERR(("FileUtils::SetFileProperties - If the 'Normal' property is set, other properties cannot be set"));
    return false;
  }

  //Check the flag data
  if((fileProperties | (FP_NORMAL | FP_READONLY)) != (FP_NORMAL | FP_READONLY))
  {
    LOGERR(("FileUtils::SetFileProperties - Unknown flags passed in properties"));
    return false;
  }

  mode_t fileMode = 0;
  
  //Get the OS file attribute flags
  if(fileProperties & FP_NORMAL)
  {
    fileMode = 0666; //Octal constant
  }
  if(fileProperties & FP_READONLY)
  {
    fileMode = 0444; //Octal constant
  }

  //Set the attributes
  if(chmod(fileName.c_str(),fileMode) < 0)
  {
    LOGERR(("FileUtils::SetFileProperties - Unable to set file properties on %s",fileName.c_str()));
    return false;
  }
  
  return true;
}

#endif//GLI_BUILD_LINUX   

