/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "UniqueShaderID.h"
#include <CommonErrorLog.h>

USING_ERRORLOG

//Static data

//Index counter -zero is reserved "no index"
uint UniqueShaderID::indexCounter = 1;
vector<UniqueShaderID *> UniqueShaderID::loadedShaderIDArray;

///////////////////////////////////////////////////////////////////////////////
//
UniqueShaderID::UniqueShaderID(GLuint newProgramID):
uniqueIndex(indexCounter),
shaderGLID(newProgramID),
shaderType(USID_Undefined),
lastFrameUsed(0),
isShaderDebugged(false)
{
  //Increment the unique index counter
  indexCounter++;

  //Add to the loaded array
  loadedShaderIDArray.push_back(this);
}

///////////////////////////////////////////////////////////////////////////////
//
UniqueShaderID::~UniqueShaderID()
{
  //Loop and remove from the loaded shader iD array
  for(uint i=0;i<loadedShaderIDArray.size(); i++)
  {
    //If equal, remove
    if(loadedShaderIDArray[i] == this)
    {
      loadedShaderIDArray.erase(loadedShaderIDArray.begin() +i);
      break;
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void UniqueShaderID::SetShaderType(USIDType newType)
{
  //Check that the type does not change once it is set
  if(shaderType != newType &&
     shaderType != USID_Undefined)
  {
    LOGERR(("UniqueShaderID::SetShaderType - Setting a different type on the shader!"));
  }

  shaderType = newType;
}

///////////////////////////////////////////////////////////////////////////////
//
UniqueShaderID * UniqueShaderID::FindUniqueShaderID(uint uIndex)
{
  //Loop for all loaded IDs
  for(uint i=0;i<loadedShaderIDArray.size(); i++)
  {
    //If equal, return
    if(loadedShaderIDArray[i]->uniqueIndex == uIndex)
    {
      return loadedShaderIDArray[i];
    }
  }

  return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
const vector<UniqueShaderID *> & UniqueShaderID::GetUniqueShaderIDList()
{
  return loadedShaderIDArray;
}


///////////////////////////////////////////////////////////////////////////////
//
void UniqueShaderID::ClearAllDebugFlags()
{
  //Loop for all loaded IDs
  for(uint i=0;i<loadedShaderIDArray.size(); i++)
  {
    //Flag that the shader is not debugged
    loadedShaderIDArray[i]->SetShaderDebugged(false);
  }

}


