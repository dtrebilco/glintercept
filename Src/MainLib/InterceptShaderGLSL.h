/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_SHADER_GLSL_H_
#define __INTERCEPT_SHADER_GLSL_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"
#include "ShaderGLSLManager.h"
#include "ShaderGLSLSaveManager.h"
#include "ReferenceCount.h"
#include "InterceptData.h"

#include <string>
#include <vector>

using namespace std;


class GLDriver;

//@
//  Summary:
//    This class intercepts OpenGL GLSL shader based calls and prepares them for
//    logging.
//  
class InterceptShaderGLSL : public InterceptData
{
public:
  
  //A structure to indicate what the GLSL request type is
  enum GLSLRequestType
  {
    GLSLRT_GLSL_Program, //Request a program
    GLSLRT_GLSL_Shader,  //Request a shader
    GLSLRT_GLSL_ARB,     //Request a ARB handle (either shader or program)
  };


  //@
  //  Summary:
  //    Constructor 
  //  
  //  Parameters:
  //    driver  - The driver used to make queries on the OpenGL state.
  //
  //    functionTable - The table of all functions that are known/are being logged.
  //
  //    configData    - The configuration options for shader interception.
  //
	InterceptShaderGLSL(GLDriver *driver,FunctionTable * functionTable,const ConfigData &configData);

  //@
  //  Summary:
  //    To log the passed function and function data 
  //    (Before the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args);

  //@
  //  Summary:
  //    To perform any post-call logging of a function.
  //    (After the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value (if any).
  //
  virtual void LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal);
  
  //@
  //  Summary:
  //    To enable/disable shader saving. Ensure shader path is set before
  //    enabling.
  //  
  //  Parameters:
  //    flag - Flag to enable/disable shader saving.
  //
  virtual void SetDataSaving(bool flag);

  //@
  //  Summary:
  //    To set the path where shaders are saved (including trailing seperator)
  //  
  //  Parameters:
  //    newPath - The new path to save shaders to.
  //
  virtual void SetDataSavingPath(const string &newPath);

  //@
  //  Summary:
  //    To save the passed shader data to disk. Shaders are only saved if 
  //    shader saving is enabled. The state of the shaders' dirty flag is
  //    re-set on successful saving. 
  //  
  //  Parameters:
  //    shaderData  - The shader data to save.
  //
  //  Returns:
  //    If saving was successful, true is returned. Else false is returned.
  //
  bool SaveShaderData(ShaderGLSLData *shaderData);

  //@
  //  Summary:
  //    To save all the currently dirty shaders to disk.
  //  
  virtual void SaveAllDirtyData();

  //@
  //  Summary:
  //    To set all the shaders that are currently being tracked, as 
  //    dirty. (ie. needs saving)
  //  
  virtual inline void SetAllDataDirty();

  //@
  //  Summary:
  //    To get the saved shader file name for the passed shader ID. If the 
  //    shader is dirty, an attempt to save the shader is made. 
  //  
  //  Parameters:
  //    id  - The shader ID of the shader to save.
  //
  //    shaderType  - The type of the shader to get. 
  //
  //    retShaderName - The filename used to save the shader.
  //
  //  Returns:
  //    If saving/retrieval was successful, true is returned. Else false is returned.
  //
  bool GetShaderFileName(GLuint id,GLSLRequestType shaderType,string &retShaderName);

  //@
  //  Summary:
  //    To get the id's of the bound GLSL program
  //  
  //  Parameters:
  //    programID  - The returned program ID of the bound program.
  //                 (returns 0 for no bound program)
  //
  void GetBoundProgram(GLuint &programID);

  //@
  //  Summary:
  //    To get the information log for the passed handle (if any).
  //  
  //  Parameters:
  //    shaderData  - The shader data to get the log data for.
  //
  //    retString   - The returned data, can be empty for no data. 
  //
  void GetLogData(const ShaderGLSLData *shaderData, string & retString);

protected:

  ShaderGLSLManager     shaderObjectManager;      // The manager of shader objects (that are attached to programs)
  ShaderGLSLManager     shaderProgramManager;     // The manager of shader programs
  ShaderGLSLSaveManager shaderSaver;              // The manager of saving shaders to disk

  bool      recordInfoLog;                        // Flag to indicate if we record info log data or not
  bool      validatePreRender;                    // Flag to indicate if we validate programs "pre" render
  bool      dumpUniformsPreRender;                // Flag to indicate if we dump uniforms for programs "pre" render

  bool      initialized;                          // Flag to indicate if we have been initialized
  bool      initFailed;                           // Flag to indicate if initialization failed

  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~InterceptShaderGLSL();

  //@
  //  Summary:
  //    To initialize constants and arrays etc.
  //  
  //  Returns:
  //    If initialization was successful, true is returned. Else false is returned.
  //
  bool Init();

  //@
  //  Summary:
  //    To determine if the driver is available and perform initialization if
  //    necessary.
  //  
  //  Returns:
  //    If the driver is available, true is returned. Else false is returned.
  //
  bool DriverAvailable();

  //@
  //  Summary:
  //    To get the action index (what action needs to be taken) for the 
  //    passed function name and index. (the index is assumed to remain the
  //    same for a given function name)
  //  
  //  Returns:
  //    If a action is valid for the index it is returned. Else -1 is returned.
  //
  int GetShaderActionIndex(const string &functionName, int funcIndex); 

  //@
  //  Summary:
  //    To update the shader log data (and save the shader) just before a 
  //    render call. If enabled, this function will validate the current 
  //    program and/or dump the uniforms of the current program.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  void UpdateLogPreRender(const FunctionData *funcData);


  //@
  //  Summary:
  //    To get the value of a uniform used in a program. 
  //    (string value representation)
  //  
  //  Parameters:
  //    progID - The ID of the program to get data for.
  //
  //    typeSize - The size of the type (number elemnets in the array)
  //               to get the data of.
  //
  //    type     - The type of the uniform that is retrieved.
  //
  //    uniformName - The name of the uniform that is being retrieved.
  //
  //    retString - The return string containing data about the uniform.
  //
  void GetUniformValue(GLuint progID,GLuint typeSize,GLenum type,const string &uniformName,string &retString);

private:

  GLenum   iObjectType;                           // Internal variable to cache data on shader creation
  GLuint   iHandle;                               // Internal data to cache a handle type across a pre/post call   
  GLuint   iHandleDetach;                         // Internal data to cache a handle type across a pre/post call   

  //Internal entry points to the OpenGL driver
  void (GLAPIENTRY *iglGetProgramiv) (GLuint object,GLenum pname,GLint *params);
  void (GLAPIENTRY *iglGetShaderiv) (GLuint object,GLenum pname,GLint *params);

  void (GLAPIENTRY *iglGetProgramInfoLog) (GLuint object, GLsizei maxLength, GLsizei *length,GLchar *infoLog); 
  void (GLAPIENTRY *iglGetShaderInfoLog) (GLuint object, GLsizei maxLength, GLsizei *length,GLchar *infoLog); 

  void (GLAPIENTRY *iglValidateProgram) (GLuint object); 

  void (GLAPIENTRY *iglGetActiveUniform) (GLuint program,GLuint GLindex, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
  GLint (GLAPIENTRY *iglGetUniformLocation) (GLuint programObj, const GLchar *name);
  void (GLAPIENTRY *iglGetUniformfv) (GLuint programObj, GLint location, GLfloat *params);
  void (GLAPIENTRY *iglGetUniformiv) (GLuint programObj, GLint location, GLint *params);
  void (GLAPIENTRY *iglGetUniformuiv) (GLuint programObj, GLint location, GLuint *params);

  //Creates a object shader/program
  void CreateObjectPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args,uint funcType);
  void CreateObjectPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal);

  //Delete object
  void DeleteObjectPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void DeleteObjectPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal,uint funcType);

  //Supply shader source
  void ShaderSourcePre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);

  //Attach/Detach object
  void AttachObjectPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void DetachObjectPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void DetachObjectPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal);

  //Compile/Link/Use
  void CompileShaderPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void CompileShaderPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal);

  void LinkProgramPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void LinkProgramPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal);

  void UseProgramPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);

  //Validate the progarm
  void ValidateProgramPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void ValidateProgramPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal);

};


///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptShaderGLSL::SetAllDataDirty()
{
  shaderObjectManager.SetAllDataDirty();
  shaderProgramManager.SetAllDataDirty();
}

#endif // __INTERCEPT_SHADER_GLSL_H_
