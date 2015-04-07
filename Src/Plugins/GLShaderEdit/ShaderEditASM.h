/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SHADER_EDIT_ASM_H_
#define __SHADER_EDIT_ASM_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include "ReferenceCount.h"
#include "ShaderManagerASM.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;


//@
//  Summary:
//    This class intercepts OpenGL ASM shader based calls and manages them
//    for editing.
//  
class ShaderEditASM : public ReferenceCount
{
public:

  //@
  //  Summary:
  //    To create an initialize a ASM shader editor interface.
  //  
  //  Parameters:
  //    callBacks  - The callbacks to GLIntercept.
  //
  //  Returns:
  //    If a interface could be created and init, it is returned. 
  //    Else NULL is returned.
  //
  static ShaderEditASM * CreateInstance(InterceptPluginCallbacks *callBacks);

  //@
  //  Summary:
  //    To log the passed function and function data 
  //    (Before the actual function is called)
  //  
  //  Parameters:
  //    userIndex - The user index value associated with this function. 
  //                This is set via SetFunctionID. Zero indicates no 
  //                 value has been set.
  //
  //    funcName - The name of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void LogFunctionPre(uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args);

  //@
  //  Summary:
  //    To perform any post-call logging of a function.
  //    (After the actual function is called)
  //  
  //  Parameters:
  //    userIndex - The user index value associated with this function. 
  //                This is set via SetFunctionID. Zero indicates no 
  //                 value has been set.
  //
  //    funcName - The name of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return vlaue of the function (if any).
  //
  virtual void LogFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal);
  
  //@
  //  Summary:
  //    To determine if the passed unique shader ID exists in this 
  //    shader editor/manager.
  //  
  //  Parameters:
  //    findShader - The shader ID to search for.
  //
  //  Returns:
  //    If the shaderID is found in this editor, true is returned. Else 
  //    false is returned.
  //
  bool FindShader(const UniqueShaderID *findShader);

  //@
  //  Summary:
  //    To create a substitute program using the passed source to replace
  //    the passed program ID.
  //  
  //  Parameters:
  //    programID - The ID of the program to replace.
  //
  //    programSource - The program source to compile the new program with.
  //
  //    retLog     - The log of the compile of the new program.
  //
  //  Returns:
  //    If substitution is successful, true is returned. Else false is returned.
  //
  bool AddSubstituteProgram(GLuint programID, const string &programSource, string &retLog);

  //@
  //  Summary:
  //    To update the current shader's frame number to the current
  //    frame number.
  //  
  //  Parameters:
  //    newFrameNum - The frame number to update the currenly 
  //                  bound shaders to.
  //
  void SetShaderFrameNum(uint newFrameNum);

  //@
  //  Summary:
  //    To determine if the passed shader ID is active.
  //  
  //  Parameters:
  //    shaderUID    - The unique ID of the shader.
  //
  //    shaderType   - The type of the shader.
  //
  bool IsShaderActive(uint shaderUID, USIDType shaderType);

protected:

  ShaderManagerASM          shaderManager;        // The manager of shaders
  InterceptPluginCallbacks *gliCallBacks;         // The callback interface into GLIntercept
  const GLCoreDriver       *GLV;                  // The core OpenGL driver

  bool      extensionARBVertexProgram;            // Flag to indicate ARB vertex program support
  bool      extensionNVVertexProgram;             // Flag to indicate NV vertex program support
  bool      extensionARBFragmentProgram;          // Flag to indicate ARB fragment program support
  bool      extensionNVFragmentProgram;           // Flag to indicate NV fragment program support



  //@
  //  Summary:
  //    Constructor 
  //  
  //  Parameters:
  //    gliCallBacks  - The callbacks to GLIntercept.
  //
	ShaderEditASM(InterceptPluginCallbacks *gliCallBacks);

  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~ShaderEditASM();

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
  //    To determine if the driver is available.
  //  
  //  Returns:
  //    If the driver is available, true is returned. Else false is returned.
  //
  bool DriverAvailable();

  //@
  //  Summary:
  //    To get from the OpenGL state what shader ID is active for the
  //    passed shader target.
  //  
  //  Returns:
  //    The OpenGL shader ID that is active for the target is returned.
  //
  uint GetActiveShaderID(GLenum oglTarget);

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

  //@
  //  Summary:
  //    To format a compile error location from a program into a string.
  //  
  //  Parameters:
  //    programSource  - The source of the program compiled.
  //
  //    errorPos  - The position of the compile error
  //
  //    retLog    - The string to have the error data appended to.
  //
  void FormatCompileErrorLocation(const string &programSource, GLint errorPos, string &retLog);

private:

  GLsizei inumNewShaders;                         // Internal variable to cache data on shader creation
  const GLuint *inewShaderIDs;                    // Internal variable to cache data on shader creation

  GLenum  ishaderTarget;                          // Internal variable to cache data on shader upload
  uint    ishaderID;                              // Internal variable to cache data on shader upload 
  GLsizei ishaderLength;                          // Internal variable to cache data on shader upload
  const void * ishaderSrc;                        // Internal variable to cache data on shader upload  

  bool    validShaderReady;                       // Internal variable to cache data on shader upload


  void (GLAPIENTRY *iglGetProgramiv) (GLenum target, GLenum pname, GLint *params); //Active program query
  void (GLAPIENTRY *iglGenProgramsARB) (GLsizei n, GLuint *programs);
  void (GLAPIENTRY *iglBindProgramARB) (GLenum target, GLuint program);
  void (GLAPIENTRY *iglProgramStringARB) (GLenum target, GLenum format, GLsizei len, const void *string);
  void (GLAPIENTRY *iglDeleteProgramsARB) (GLsizei n, const GLuint *programs);

  void (GLAPIENTRY *iglGenProgramsNV) (GLsizei n, GLuint *ids);
  void (GLAPIENTRY *iglBindProgramNV) (GLenum target, GLuint id);
  void (GLAPIENTRY *iglLoadProgramNV) (GLenum target, GLuint id, GLsizei len, const GLubyte *program);
  void (GLAPIENTRY *iglDeleteProgramsNV) (GLsizei n, const GLuint *ids);

  void CreateARBShadersPre(FunctionArgs & args);
  void CreateARBShadersPost();

  //Binds a NV/ARB shader (May also create a shader)
  void BindARBShaderPre(FunctionArgs & args);
  
  //Delete a NV/ARB shader
  void DeleteARBShaders(FunctionArgs & args);

  //Set NV/ARB shader ready
  void SetARBShaderReadyPre(uint updateID, FunctionArgs & args);
  void SetARBShaderReadyPost(uint updateID);

};


#endif // __SHADER_EDIT_ASM_H_
