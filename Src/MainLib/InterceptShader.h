/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_SHADER_H_
#define __INTERCEPT_SHADER_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"
#include "ShaderManager.h"
#include "ShaderSaveManager.h"
#include "ReferenceCount.h"
#include "InterceptData.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;

//@
//  Summary:
//    This class intercepts OpenGL shader based calls and prepares them for
//    logging.
//  
class InterceptShader : public InterceptData
{
public:

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
	InterceptShader(GLDriver *driver,FunctionTable * functionTable,const ConfigData &configData);

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
  bool SaveShaderData(ShaderData *shaderData);

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
  //    retShaderName - The filename used to save the shader.
  //
  //  Returns:
  //    If saving/retrieval was successful, true is returned. Else false is returned.
  //
  bool GetShaderFileName(GLuint id,string &retShaderName);

  //@
  //  Summary:
  //    To get the id's of the bound vertex and fragment shader
  //  
  //  Parameters:
  //    vertexShaderID  - The returned shader ID of the bound vertex shader.
  //                      (returns 0 for no bound vertex shader)
  //
  //    fragmentShaderID  - The returned shader ID of the bound fragment shader.
  //                        (returns 0 for no bound fragment shader)
  void GetBoundShaders(uint &vertexShaderID,uint &fragmentShaderID);

protected:

  ShaderManager     shaderManager;                // The manager of shaders
  ShaderSaveManager shaderSaver;                  // The manager of saving shaders to disk

  bool      initialized;                          // Flag to indicate if we have been initialized
  bool      initFailed;                           // Flag to indicate if initialization failed


  bool      extensionARBVertexProgram;            // Flag to indicate ARB vertex program support
  bool      extensionNVVertexProgram;             // Flag to indicate NV vertex program support
  bool      extensionARBFragmentProgram;          // Flag to indicate ARB fragment program support
  bool      extensionNVFragmentProgram;           // Flag to indicate NV fragment program support


  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~InterceptShader();

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
  //    To get from the OpenGL state what shader ID is active for the
  //    passed shader target.
  //  
  //  Returns:
  //    The OpenGL shader ID that is active for the target is returned.
  //
  uint GetActiveShaderID(GLenum oglTarget);

private:

  GLsizei inumNewShaders;                         // Internal variable to cache data on shader creation
  const GLuint *inewShaderIDs;                    // Internal variable to cache data on shader creation

  GLenum  ishaderTarget;                          // Internal variable to cache data on shader upload
  uint    ishaderID;                              // Internal variable to cache data on shader upload 
  GLsizei ishaderLength;                          // Internal variable to cache data on shader upload
  const void * ishaderSrc;                        // Internal variable to cache data on shader upload  

  bool    validShaderReady;                       // Internal variable to cache data on shader upload


  void (GLAPIENTRY *iglGetProgramivARB) (GLenum target, GLenum pname, GLint *params); //Active program query

  //Creates a shader
  void CreateARBShadersPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void CreateARBShadersPost(const FunctionData *funcData,uint funcTableIndex);

  //Binds a ARB shader (May also create a shader)
  void BindARBShaderPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);

  //Delete a ARB shader
  void DeleteARBShaders(const FunctionData *funcData,uint index, FunctionArgs & args);

  //Set ARB shader ready
  void SetARBShaderReadyPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args,uint funcType);
  void SetARBShaderReadyPost(const FunctionData *funcData,uint funcTableIndex);

};


///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptShader::SetAllDataDirty()
{
  shaderManager.SetAllDataDirty();
}

#endif // __INTERCEPT_SHADER_H_
