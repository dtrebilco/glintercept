/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptShader.h"
#include "GLDriver.h"
#include "ConfigData.h"

USING_ERRORLOG


///////////////////////////////////////////////////////////////////////////////
//
ShaderSaveManager::ShaderSaveManager(const ConfigData &configData)
{
}

///////////////////////////////////////////////////////////////////////////////
//
ShaderSaveManager::~ShaderSaveManager()
{
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderSaveManager::SaveShader(const string &fileName, const ShaderData *shaderData,string &retFileName) const
{
  //Init the return values
  bool retValue = false;
  retFileName = "";

  //Save the shaders based on the different types
  switch(shaderData->GetGLType())
  {
    //Note: GL_VERTEX_PROGRAM_NV is the same enum
    case(GL_VERTEX_PROGRAM_ARB) :
    case(GL_FRAGMENT_PROGRAM_ARB) :
      retValue = SaveARBShader(fileName,shaderData,retFileName);
      break;

    case(GL_FRAGMENT_PROGRAM_NV) :
    case(GL_VERTEX_STATE_PROGRAM_NV):
      retValue = SaveNVShader(fileName,shaderData,retFileName);
      break;

    //For a new type, just write it out
    default:
      LOGERR(("ShaderSaveManager::SaveShader - Unknown shader type %x",shaderData->GetGLType()));
      retValue = WriteShaderFile(fileName,shaderData->GetShaderSource().c_str(),
                                          (uint)shaderData->GetShaderSource().length(),retFileName);
  }

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderSaveManager::SaveARBShader(const string &fileName, const ShaderData *shaderData,string &retFileName) const
{

  //Format the string
  //isspace

  //Save the program string
  return WriteShaderFile(fileName,shaderData->GetShaderSource().c_str(),
                                  (uint)shaderData->GetShaderSource().length(),retFileName);

}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderSaveManager::SaveNVShader(const string &fileName, const ShaderData *shaderData,string &retFileName) const
{
  //Save the program string
  return WriteShaderFile(fileName,shaderData->GetShaderSource().c_str(),
                                  (uint)shaderData->GetShaderSource().length(),retFileName);
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderSaveManager::WriteShaderFile(const string &fileName,const char * shaderSrc,uint shaderLength, string &retFileName) const
{

  //Add a .txt to the end
  retFileName = fileName + ".txt";

  //Open the file
  FILE * file = fopen(retFileName.c_str(),"wb");
  if(!file)
  {
    LOGERR(("ShaderSaveManager::WriteShaderFile - Unable to open file %s",retFileName.c_str()));
    retFileName = "";
    return false;
  }

  //Write the string to the file
  if(fwrite(shaderSrc,1,shaderLength,file) != shaderLength)
  {
    LOGERR(("ShaderSaveManager::WriteShaderFile - Unable to write to file %s",retFileName.c_str()));
    fclose(file);  
    retFileName = "";
    return false;
  }

  //Close the file
  fclose(file);
  return true;
}


