/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptDisplayList.h"
#include "GLDriver.h"
#include "ConfigData.h"

USING_ERRORLOG

///////////////////////////////////////////////////////////////////////////////
//
DisplayListSaveManager::DisplayListSaveManager(const ConfigData &configData)
{
}

///////////////////////////////////////////////////////////////////////////////
//
DisplayListSaveManager::~DisplayListSaveManager()
{
}


///////////////////////////////////////////////////////////////////////////////
//
bool DisplayListSaveManager::SaveDisplayList(const string &fileName, const DisplayListData *displayListData,string &retFileName) const
{
  //Init the return values
  bool retValue = false;
  retFileName = "";

  retValue = WriteDisplayListFile(fileName,displayListData->GetDisplayListSource().c_str(),
                                           (uint)displayListData->GetDisplayListSource().length(),retFileName);

  return retValue;
}


///////////////////////////////////////////////////////////////////////////////
//
bool DisplayListSaveManager::WriteDisplayListFile(const string &fileName,const char * displayListSrc,uint displayListLength, string &retFileName) const
{

  //Add a .txt to the end
  retFileName = fileName + ".txt";

  //Open the file
  FILE * file = fopen(retFileName.c_str(),"wt");
  if(!file)
  {
    LOGERR(("DisplayListSaveManager::WriteDisplayListFile - Unable to open file %s",retFileName.c_str()));
    retFileName = "";
    return false;
  }

  //Write the string to the file
  if(fwrite(displayListSrc,1,displayListLength,file) != displayListLength)
  {
    LOGERR(("DisplayListSaveManager::WriteDisplayListFile - Unable to write to file %s",retFileName.c_str()));
    fclose(file);  
    retFileName = "";
    return false;
  }

  //Close the file
  fclose(file);
  return true;
}


