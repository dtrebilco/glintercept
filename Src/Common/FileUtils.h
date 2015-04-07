/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __FILE_UTILS_H__
#define __FILE_UTILS_H__

#include <string>
#include <vector>

using namespace std;
typedef unsigned int uint;

//@
//  Summary:
//    File utility functions that are OS specific
// 
class FileUtils
{
public:

  static const char dirSeparator;                 //The characters used as directory seperators

  //Properties that can be set on files
  enum FileProperties
  {
    FP_NORMAL   = 1 << 0L,     // Standard read/write access  
    FP_READONLY = 1 << 1L,     // File is flagged as read only
  };
 
  //@
  //  Summary:
  //    To create the specified directory (if it does not already exist)
  //    Sub-directories leading up to the directory will be created if 
  //    necessary.
  //  
  //  Parameters:
  //    directory  - The directory to be created.
  //
  //  Returns:
  //    True is returned on success, false if otherwise.
  //
  static bool CreateFullDirectory(const string &directory); 

  //@
  //  Summary:
  //    To copy a file from the source to the destination file names.
  //  
  //  Parameters:
  //    srcFile  - The source file name.
  //
  //    dstFile  - The destination file name.
  //
  //    overwrite - Falg to indicate if we over-write if the dstFile already exists.
  //
  //  Returns:
  //    True is returned on success, false if otherwise.
  //
  static bool CopyFile(const string &srcFile,const string &dstFile, bool overwrite);

  //@
  //  Summary:
  //    To set the properties of a file.
  //  
  //  Parameters:
  //    fileName  - The file name to set the properties for.
  //
  //    fileProperties  - The file properties Mask to set. 
  //                      (cannot set incompatable flags such as FP_NORMAL and FP_READONLY)
  //
  //  Returns:
  //    True is returned on success, false if otherwise.
  //
  static bool SetFileProperties(const string &fileName,uint fileProperties);
};








#endif 
