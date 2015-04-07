/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SHADER_GLSL_SAVE_MANAGER_H_
#define __SHADER_GLSL_SAVE_MANAGER_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;
class ConfigData;


//@
//  Summary:
//    Class to handle the saving of shaders. 
//  
class ShaderGLSLSaveManager
{
public:

  //@
  //  Summary:
  //    To set the initial saving options for the shader saver.
  //  
  //  Parameters:
  //    configData  - The configuration data to copy the saving options from.
  //
  ShaderGLSLSaveManager(const ConfigData &configData);

  //@
  //  Summary:
  //    Destructor
  //  
  virtual ~ShaderGLSLSaveManager();

  //@
  //  Summary:
  //    To save the passed shader data structure out. 
  //  
  //  Parameters:
  //    fileName      - The base file name to save the shader out as. (without extension)
  //
  //    shaderData    - The shader data to save. 
  //
  //    retFileName   - The return file name of the shader saved.
  //
  //  Returns:
  //    True is returned if the shader data was written successfully. 
  //    Otherwise false is returned.
  //
  bool SaveShader(const string &fileName, const ShaderGLSLData *shaderData, string &retFileName) const;


protected:

  //@
  //  Summary:
  //    To save the passed fragment/vertex shader data structure out. 
  //  
  //  Parameters:
  //    fileName  - The base file name to save the shader out as. (without extension)
  //
  //    shaderData  - The shader data to save. 
  //
  //    retFileName - The return file name of the shader saved.
  //
  //  Returns:
  //    True is returned if the shader data was written successfully. 
  //    Otherwise false is returned.
  //
  bool SaveFragmentVertexShaderData(const string &fileName, const ShaderGLSLData *shaderData,string &retFileName) const;

  //@
  //  Summary:
  //    To save the passed program object data structure out. 
  //  
  //  Parameters:
  //    fileName      - The base file name to save the shader out as. (without extension)
  //
  //    shaderData    - The shader data to save. 
  //
  //    retFileName   - The return file name of the shader saved.
  //
  //  Returns:
  //    True is returned if the shader data was written successfully. 
  //    Otherwise false is returned.
  //
  bool SaveProgramData(const string &fileName, const ShaderGLSLData *shaderData, string &retFileName) const;

  //@
  //  Summary:
  //    To save a shaders' source to disk.
  //  
  //  Parameters:
  //    fileName  - The base file name to save the shader out as. (without extension)
  //
  //    shaderSrc  - The raw shader source data to save. 
  //
  //    shaderLength - The length of the shader source.
  //
  //    retFileName - The return file name of the shader saved.
  //
  //  Returns:
  //    True is returned if the data was written successfully. 
  //    Otherwise false is returned.
  //
  bool WriteShaderFile(const string &fileName,const char * shaderSrc, uint shaderLength,string &retFileName) const;

};





#endif // __SHADER_GLSL_SAVE_MANAGER_H_
