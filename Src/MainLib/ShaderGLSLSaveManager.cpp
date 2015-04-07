/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptShaderGLSL.h"
#include "GLDriver.h"
#include "ConfigData.h"

USING_ERRORLOG


///////////////////////////////////////////////////////////////////////////////
//
ShaderGLSLSaveManager::ShaderGLSLSaveManager(const ConfigData &configData)
{
}

///////////////////////////////////////////////////////////////////////////////
//
ShaderGLSLSaveManager::~ShaderGLSLSaveManager()
{
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderGLSLSaveManager::SaveShader(const string &fileName, const ShaderGLSLData *shaderData, string &retFileName) const
{
  //Init the return values
  bool retValue = false;
  retFileName = "";

  //Save the shaders based on the different types
  switch(shaderData->GetGLType())
  {
    case(GL_VERTEX_SHADER):
    case(GL_GEOMETRY_SHADER):
    case(GL_FRAGMENT_SHADER):
      retValue = SaveFragmentVertexShaderData(fileName,shaderData,retFileName);
      break;
    case(GL_PROGRAM_OBJECT_ARB):
      retValue = SaveProgramData(fileName,shaderData,retFileName);
      break;

    //For a new type, just write it out
    default:
      LOGERR(("ShaderGLSLSaveManager::ShaderGLSLData - Unknown shader type %x",shaderData->GetGLType()));
      retValue = WriteShaderFile(fileName,shaderData->GetShaderSource().c_str(),
                                          (uint)shaderData->GetShaderSource().length(),retFileName);
  }

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderGLSLSaveManager::SaveFragmentVertexShaderData(const string &fileName, const ShaderGLSLData *shaderData,string &retFileName) const
{

  //Future formatting options?

  // Add log info 
  string writeString = shaderData->GetShaderLog() + shaderData->GetShaderSource();

  //Write the sahder to disk
  return WriteShaderFile(fileName,writeString.c_str(),
                                  (uint)writeString.length(),retFileName);
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderGLSLSaveManager::SaveProgramData(const string &fileName, const ShaderGLSLData *programData, string &retFileName) const
{
  //Future formatting options?
  
  // Add log info 
  string writeString = programData->GetShaderLog() + programData->GetShaderSource();

  return WriteShaderFile(fileName,writeString.c_str(),
                                  (uint)writeString.length(),retFileName);

}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderGLSLSaveManager::WriteShaderFile(const string &fileName,const char * shaderSrc,uint shaderLength, string &retFileName) const
{

  //Add a .txt to the end
  retFileName = fileName + ".txt";

  //Open the file
  FILE * file = fopen(retFileName.c_str(),"wt");
  if(!file)
  {
    LOGERR(("ShaderGLSLSaveManager::WriteShaderFile - Unable to open file %s",retFileName.c_str()));
    retFileName = "";
    return false;
  }

  //Write the string to the file
  if(fwrite(shaderSrc,1,shaderLength,file) != shaderLength)
  {
    LOGERR(("ShaderGLSLSaveManager::WriteShaderFile - Unable to write to file %s",retFileName.c_str()));
    fclose(file);  
    retFileName = "";
    return false;
  }

  //Close the file
  fclose(file);
  return true;
}


