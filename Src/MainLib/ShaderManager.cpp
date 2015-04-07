/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptShader.h"
#include "GLDriver.h"


USING_ERRORLOG


//Set the class name for the shader manager
template<>
char *InterceptDataManager<ShaderData>::className = "Shader";

///////////////////////////////////////////////////////////////////////////////
//
ShaderData::ShaderData(uint glID):
InterceptDataType(glID),
glType(0),
saveFileName(""),
shaderSource("")
{
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderData::GetUniqueFileName(string &retString) const
{
  //Set the initial shader name
  retString = "Shader_";

  //Append the shader type
  switch(glType)
  {
    //NV vertex programs have the same token
    case(GL_VERTEX_PROGRAM_ARB):
      retString = retString + "VPARB_";
      break;
    case(GL_FRAGMENT_PROGRAM_ARB):
      retString = retString + "FPARB_";
      break;
    case(GL_FRAGMENT_PROGRAM_NV):
      retString = retString + "FPNV_";
      break;
    case(GL_VERTEX_STATE_PROGRAM_NV):
      retString = retString + "VPSTATENV_";
      break;
    default:
      retString = retString + "UNKNOWN_";
      break;
  }

  //Add the shader ID
  string bufString;
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
ShaderManager::ShaderManager()
{
  //Add a shader (the zero shader always exists)
  AddData(0);

}

///////////////////////////////////////////////////////////////////////////////
//
ShaderManager::~ShaderManager()
{
  //Remove the 0 shader
  InterceptDataManager<ShaderData>::RemoveData(0);
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderManager::RemoveData(uint glId)
{
  //Never delete 0
  if(glId != 0)
  {
    return InterceptDataManager<ShaderData>::RemoveData(glId);
  }

  return true;
}





