/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Parts of this file may be derived from GLTrace version 2.3
  by Phil Frisbie, Jr. (phil@hawksoft.com)

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SE_CONTEXT_H_
#define __SE_CONTEXT_H_

#include "ShaderEditASM.h"
#include "ShaderEditGLSL.h"
#include "GLRenderCallCache.h"
#include "GLFrameBufferCache.h"
#include "SubstituteShaderGLSL.h"

class GLDriver;

#define GENERAL_CALLBACK_INDEX       1  //Offset for general callbacks (zero is reserved) 

#define SHADER_ASM_CALLBACK_INDEX  100  //Offset for all callbacks for ASM  shaders
#define SHADER_ASM_CALLBACK_MAX    199  //Max offset for all callbacks for ASM  shaders

#define SHADER_GLSL_CALLBACK_INDEX 200  //Offset for all callbacks for GLSL shaders
#define SHADER_GLSL_CALLBACK_MAX   299  //Max offset for all callbacks for GLSL shaders

#define RENDER_CALL_CALLBACK_INDEX 300  //Offset for all callbacks for registered render calls
#define RENDER_CALL_CALLBACK_MAX   399  //Max offset for all callbacks for registered render calls


//@
//  Summary:
//    This class holds the internal data required for a OpenGL context.
//  
class SEContext
{
public:

  //@
  //  Summary:
  //    Constructor
  //
  //  Parameters:
  //    rcHandle   - The handle to the OpenGL context.
  //
  SEContext(HGLRC rcHandle);
  virtual ~SEContext();
  
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
  void LogFunctionPre(uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args);

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
  void LogFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    To share this classes' internal data with the passed context.
  //
  //  Parameters:
  //    shareContext  - The context to share data with.
  //
  //  Returns:
  //    True is returned on successful sharing, false if otherwise.
  //
  bool ShareLists(SEContext *shareContext) const;

  //@
  //  Summary:
  //    To get the render context handle for this context.
  //  
  //  Returns:
  //    The render context handle for this context is returned.
  //
  inline HGLRC GetRCHandle() const;

#ifdef GLI_BUILD_WINDOWS

  //@
  //  Summary:
  //    To set the current device context used with this render context.
  //  
  //  Parameters:
  //    newDCHandle - The device context to set.
  //
  inline void SetDCHandle(HDC newDCHandle);

  //@
  //  Summary:
  //    To get the device context that was last used with this render context.
  //  
  //  Returns:
  //    The last used device context is returned.
  //
  inline HDC GetDCHandle() const;

#endif // GLI_BUILD_WINDOWS

  //@
  //  Summary:
  //    To determine if this context has been initialized yet.
  //  
  //  Returns:
  //    If Init has been called, true is returned. Else false is returned.
  //
  inline bool IsInit();
  
  //@
  //  Summary:
  //    To initialize the context. Must be called once the 
  //    internal OpenGL context is active
  //  
  //  Parameters:
  //    gliCallBacks - The callbacks to GLIntercept.
  //
  void Init(InterceptPluginCallbacks *gliCallBacks);

  //@
  //  Summary:
  //    To be called before a render call occurs. 
  //  
  void OnRenderPre();

  //@
  //  Summary:
  //    To be called after a render call occurs.
  //    (In the case of glBegin render call, it is expected this to only
  //     be called after glEnd)
  //  
  void OnRenderPost();

  //@
  //  Summary:
  //    To set what the bound GLSL shader is.
  //  
  //  Parameters:
  //    boundGLSLShader - The currently bound GLSL shader (if any).
  //
  inline void SetBoundGLSLShader(const UniqueShaderIDRef &boundGLSLShader);

  //@
  //  Summary:
  //    To get what the bound GLSL shader is.
  //  
  //  Returns:
  //    The currently bound GLSL shader is returned (if any).
  //
  inline const UniqueShaderIDRef & GetBoundGLSLShader();

  //@
  //  Summary:
  //    To determine if the passed unique shader ID is valid in this context.
  //  
  //  Parameters:
  //    findShader - The shader ID to search for.
  //
  //  Returns:
  //    If the shaderID is valid in this context, true is returned. Else 
  //    false is returned.
  //
  bool FindShader(const UniqueShaderID *findShader);

  //@
  //  Summary:
  //    To compile the passed shader source and to override the 
  //    passed shader ID.
  //  
  //  Parameters:
  //    findShader - The shader ID to override.
  //
  //    shaderSrc   - The shader source to compile.
  //
  //    retLog      - The returned log of any errors/warnings during compile.
  //
  //  Returns:
  //    If the shader source was compiled successfully, true is returned.
  //    Else false is returned.
  //
  bool CompileShader(const UniqueShaderID *findShader, const string &shaderSrc, string &retLog);

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

  //@
  //  Summary:
  //    To get the current render call cache.
  //  
  //  Returns:
  //    The current render call cache is returned.
  //
  inline GLRenderCallCache *GetRenderCallCache() const;

  //@
  //  Summary:
  //    To get the current frame buffer cache.
  //  
  //  Returns:
  //    The current frame buffer cache is returned.
  //
  inline GLFrameBufferCache *GetFrameBufferCache() const;


protected:

  bool    isInit;                                 // Flag indicating if this context has been initialized
  HGLRC   glRCHandle;                             // The openGL rendering context handle

#ifdef GLI_BUILD_WINDOWS  
  HDC     dcHandle;                               // The last device context used with this render context
#endif //GLI_BUILD_WINDOWS

  ShaderEditASM     *shaderEditASM;               // The class used to edit ASM shaders
  ShaderEditGLSL    *shaderEditGLSL;              // The class used to edit GLSL shaders

  GLRenderCallCache *renderCallCache;             // The render call cache (used in debugging)
  GLFrameBufferCache *frameBufferCache;           // The frame buffer cache (used in debugging)

  UniqueShaderIDRef  boundGLSLShader;             // The current GLSL shader that is bound

};


///////////////////////////////////////////////////////////////////////////////
//
inline HGLRC SEContext::GetRCHandle() const
{
  return glRCHandle;
}

#ifdef GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//
inline void SEContext::SetDCHandle(HDC newDCHandle)
{ 
  dcHandle = newDCHandle;
}

///////////////////////////////////////////////////////////////////////////////
//
inline HDC SEContext::GetDCHandle() const
{
  return dcHandle;
}

#endif // GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//
inline bool SEContext::IsInit()
{
  return isInit;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void SEContext::SetBoundGLSLShader(const UniqueShaderIDRef &newBound)
{
  boundGLSLShader = newBound;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const UniqueShaderIDRef & SEContext::GetBoundGLSLShader()
{
  return boundGLSLShader;
}

///////////////////////////////////////////////////////////////////////////////
//
inline GLRenderCallCache *SEContext::GetRenderCallCache() const
{
  return renderCallCache;
}

///////////////////////////////////////////////////////////////////////////////
//
inline GLFrameBufferCache *SEContext::GetFrameBufferCache() const
{
  return frameBufferCache;
}

#endif // __SE_CONTEXT_H_
