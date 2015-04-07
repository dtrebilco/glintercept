/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptData.h"
#include "FileUtils.h"
#include "GLDriver.h"



USING_ERRORLOG


///////////////////////////////////////////////////////////////////////////////
//
InterceptData::InterceptData(GLDriver *ogldriver):
driver(ogldriver),
dataSavingEnabled(false),
dataSavePath(""),
dataSavePathCreated(false)
{
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptData::~InterceptData()
{
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptData::SetDataSavingPath(const string &newPath)
{
  //Assign the new path as the current save path
  dataSavePath = newPath;

  //Flag the the directory needs creation if it is requested
  dataSavePathCreated = false;
}

///////////////////////////////////////////////////////////////////////////////
//
const string & InterceptData::GetDataSavingPath() 
{
  //If the current path has not been created, attempt to create it
  if(!dataSavePathCreated)
  {
    //Create the new data logging directory
    if(!FileUtils::CreateFullDirectory(dataSavePath))
    {
      LOGERR(("InterceptData::GetDataSavingPath - Unable to create directory %s ",dataSavePath.c_str())); 

      //If could not create the new directory, just use the current directory
      dataSavePath = "";
    }

    //Flag that the directory was (or attemped to be) created
    dataSavePathCreated = true;
  }

  return dataSavePath;
}
