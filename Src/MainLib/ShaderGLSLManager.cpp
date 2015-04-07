/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "ShaderGLSLManager.h"
#include "GLDriver.h"


USING_ERRORLOG


//Set the class name for the shader manager
template<>
char *InterceptDataManager<ShaderGLSLData>::className = "ShaderGLSL";

///////////////////////////////////////////////////////////////////////////////
//
ShaderGLSLData::ShaderGLSLData(uint glID):
InterceptDataType(glID),
glType(0),
userDeleted(false),
saveFileName(""),
shaderSource(""),
shaderLog("")
{
}

///////////////////////////////////////////////////////////////////////////////
//
ShaderGLSLData::~ShaderGLSLData()
{
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderGLSLData::GetUniqueFileName(string &retString) const
{
  string bufString;

  //Set the initial shader name
  retString = "ShaderGLSL_";

  //Append the shader type
  switch(glType)
  {
    case(GL_VERTEX_SHADER):
      retString = retString + "VS_";
      break;
    case(GL_GEOMETRY_SHADER):
      retString = retString + "GS_";
      break;
    case(GL_FRAGMENT_SHADER):
      retString = retString + "FS_";
      break;
    case(GL_PROGRAM_OBJECT_ARB):
      retString = retString + "PROG_";
      break;
    default:
      StringPrintF(bufString,"0x%X_", glType);
      retString = retString + "UNKNOWN_" + bufString;
      break;
  }

  //Add the shader ID
  StringPrintF(bufString,"%04u_",id);
  retString = retString + bufString;
  
  //Add the save count
  static uint saveCount=0;
  saveCount++;
  StringPrintF(bufString,"%04u",saveCount);
  retString = retString + bufString;

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
void ShaderGLSLData::AttachObject(GLhandle newHandle)
{
  //Attempt to find the object
  for(uint i=0;i<attachArray.size();i++)
  {
    //If already attached, just return
    if(attachArray[i] == newHandle)
    {
      return;
    }
  }

  //Add to the array
  attachArray.push_back(newHandle);
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderGLSLData::DetachObject(GLhandle handle)
{
  //Attempt to find the object
  for(uint i=0;i<attachArray.size();i++)
  {
    //If equal, remove and return
    if(attachArray[i] == handle)
    {
      attachArray.erase(attachArray.begin() + i);
      return true;
    }
  }

  //Not found
  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
ShaderGLSLManager::ShaderGLSLManager()
{
  //Add a shader (the zero shader always exists)
  AddData(0);

}

///////////////////////////////////////////////////////////////////////////////
//
ShaderGLSLManager::~ShaderGLSLManager()
{
  //Remove the 0 shader
  InterceptDataManager<ShaderGLSLData>::RemoveData(0);
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderGLSLManager::RemoveData(uint glId)
{
  //Never delete 0
  if(glId != 0)
  {
    return InterceptDataManager<ShaderGLSLData>::RemoveData(glId);
  }

  return true;
}




