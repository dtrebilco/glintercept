/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __GL_CONTEXT_H_
#define __GL_CONTEXT_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "InterceptImage.h"
#include "InterceptShader.h"
#include "InterceptShaderGLSL.h"
#include "InterceptFrame.h"
#include "InterceptDisplayList.h"
#include "ConfigData.h"

#include <string>

using namespace std;

class GLDriver;

//@
//  Summary:
//    This class holds the internal data required for a OpenGL context.
//  
class GLContext
{
public:

  //@
  //  Summary:
  //    Constructor
  //
  //  Parameters:
  //    rcHandle   - The handle to the OpenGL context.
  //
  //    configData - The configuration options used when creating the context. 
  //                 (ie. what loggers to create)
  //
  //    glDriver   - The GLIntercept driver used for this context
  //
  //    functionTable - The function table of known OpenGL functions
  //
  GLContext(HGLRC rcHandle, const ConfigData &configData, GLDriver *glDriver, FunctionTable *functionTable);
  virtual ~GLContext();
  
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
  void LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args);

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
  void LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal);

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
  bool ShareLists(GLContext *shareContext) const;

  //@
  //  Summary:
  //    To activate the internal loggers with the passed directory name.
  //
  //  Parameters:
  //    logPath  - The directory to activate the internal loggers with.
  //
  void ActivateLoggers(const string &logPath);

  //@
  //  Summary:
  //    To suspend the internal loggers from logging.
  //
  void SuspendLoggers();


  //@
  //  Summary:
  //    To flag all tacked logger data as "dirty". (ie needs re-saving)
  //
  void SetLoggerDataDirty();


  //@
  //  Summary:
  //    To get the image saved filenames for a passed texture ID
  //  
  //  Parameters:
  //    texId  - The openGL texture ID to get the images for.
  //
  //    retSaveFiles - The files to be returned.
  //
  //  Returns:
  //    If the images could be retrieved, true is returned. 
  //    Else false is returned.
  //
  inline bool GetTextureFileNames(GLint texId, ImageSaveFiles &retSaveFiles);

  //@
  //  Summary:
  //    To get the shader saved filename for a passed shader ID
  //  
  //  Parameters:
  //    id  - The openGL shader (ARB program) ID to get the shader for.
  //
  //    retShaderName - The shader name to be returned.
  //
  //  Returns:
  //    If the shader file name could be retrieved, true is returned. 
  //    Else false is returned.
  //
  inline bool GetShaderFileName(GLint id,string &retShaderName);

  //@
  //  Summary:
  //    To get the GLSL shader saved filename for a passed shader ID
  //  
  //  Parameters:
  //    id  - The openGL shader (GLSL handle) ID to get the shader for.
  //
  //    shaderType - The type of the GLSL shader (either program/shader/unknown)
  //
  //    retShaderName - The shader name to be returned.
  //
  //  Returns:
  //    If the shader file name could be retrieved, true is returned. 
  //    Else false is returned.
  //
  inline bool GetShaderGLSLFileName(GLint id,InterceptShaderGLSL::GLSLRequestType shaderType,string &retShaderName);

  //@
  //  Summary:
  //    To get the filenames for frame buffer logging images.
  //  
  //  Parameters:
  //    retShaderName - The file name to be retrieved.
  //
  //  Returns:
  //    If the file names could be retrieved, true is returned. 
  //    Else false is returned.
  //
  inline bool GetFrameFileNames(FrameInterceptFileNames & retFrameFileNames);

  //@
  //  Summary:
  //    To get the display list saved filename for a passed display list ID
  //  
  //  Parameters:
  //    id  - The openGL list ID to get the list fileName for.
  //
  //    retListName - The display list name to be returned.
  //
  //  Returns:
  //    If the shader file name could be retrieved, true is returned. 
  //    Else false is returned.
  //
  inline bool GetDisplayListFileName(GLint id,string &retListName);


  //@
  //  Summary:
  //    To get an array of texture IDs that are bound. 
  //  
  //  Parameters:
  //    retArray  - The return array of texture IDs (and the stages) that are bound.
  //
  inline void GetBoundTextures(BoundTextureArray &retArray);

  //@
  //  Summary:
  //    To get from the OpenGL state what texture ID is active for the 
  //    passed texture stage and passed texture target.
  //  
  //  Parameters:
  //    oglTarget  - The OpenGL target of the texture ID to get.
  //
  //    texStage   - The texture stage to get the OpenGL target from.
  //
  //    retTexID   - The returned texture ID.
  //
  //  Returns:
  //    If the texture ID could be retrieved, true is returned and retTexID 
  //    contains the texture ID, Else false is returned.
  //
  inline bool GetBoundTexture(GLenum oglTarget,uint texStage,uint &retTexID);

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
  //    
  //    programGLSLID     - The returned program ID of the bound GLSL program.
  //                        (returns 0 for no bound program)
  //
  inline void GetBoundShaders(uint &vertexShaderID,uint &fragmentShaderID,GLuint &programGLSLID);

  //@
  //  Summary:
  //    To determine if the passed function performs a render.
  //  
  //  Parameters:
  //    funcData  - The function data pointer to test.
  //
  //    funcTableIndex - The index of the passed function data in the function
  //                     table.
  //
  //    args     - The arguments of the function.
  //
  //  Returns:
  //    If the passed function data pointer is a render function, 
  //    true is returned. Else false is returned.
  //
  inline bool IsRenderFuncion(const FunctionData *funcData, uint funcTableIndex, const FunctionArgs & args);

  //@
  //  Summary:
  //    To get cached error code for this context.
  //  
  //  Returns:
  //    The cached error code for this context is returned.
  //
  inline GLenum GetCachedError();

  //@
  //  Summary:
  //    To set the cacahed error code of this context.
  //  
  //  Parameters:
  //    newError  - The new cached error code for this context
  //
  inline void SetCachedError(GLenum newError);

  //@
  //  Summary:
  //    To get the render context handle for this context.
  //  
  //  Returns:
  //    The render context handle for this context is returned.
  //
  inline HGLRC GetRCHandle();

  //@
  //  Summary:
  //    To get the thread ID that is assigned to the context.
  //  
  //  Returns:
  //    The thread ID of the context is returned.
  //
  inline uintptr_t GetContextThreadID() const;
   
  //@
  //  Summary:
  //    To set the context as active and assign the active thread ID 
  //    that is assigned to the context.
  //  
  //  Parameters:
  //    threadID  - The new context thread id.
  //
  inline void SetContextActive(uintptr_t threadID);

  //@
  //  Summary:
  //    To set the context as in-active.
  //  
  inline void SetContextInActive();

  //@
  //  Summary:
  //    To return the OpenGL version number.
  //  
  //  Returns:
  //    A float value in the form <major num>.<minor num><release num> is
  //    returned. If the version cannot be determined, 1.0 is returned.
  //
  float GetOpenGLVersion();

  //@
  //  Summary:
  //    Determines if the passed OpenGL extension is supported.
  //  
  //  Returns:
  //    If the OpenGL extension is supported, true is returned.
  //    If the extension is not supported (or it could not be determined)
  //    false is returned.
  //
  bool IsExtensionSupported(const char *extension);

protected:
  
  GLDriver *driver;                               // The OpenGL driver

  bool  initialized;                              // Flag indicating if initialization has been done
  const GLubyte * (GLAPIENTRY *iglGetStringi)(GLenum name, GLuint index); // glGetStringi for extenstion queries on OpenGL 3.0+

  HGLRC   glRCHandle;                             // The openGL rendering context handle
  GLenum  cachedError;                            // The cached error code
  uintptr_t contextThreadID;                      // The thread ID of the thread that currently has ownership of this context

  InterceptImage      *interceptImage;            // The class used to log image calls
  InterceptShader     *interceptShader;           // The class used to log shader calls
  InterceptShaderGLSL *interceptShaderGLSL;       // The class used to log GLSL shader calls
  InterceptFrame      *interceptFrame;            // The class used to save the frame buffer

  InterceptDisplayList *interceptList;            // The class used to log display list calls

  //@
  //  Summary:
  //    Initializes the active context if necessary 
  //    and retrieves necessary OpenGL entry points.
  //    Context must be active and each context will only init once.
  //  
  void InitActiveContext();
};

///////////////////////////////////////////////////////////////////////////////
//
inline GLenum GLContext::GetCachedError()
{
  return cachedError;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void GLContext::SetCachedError(GLenum newError)
{
  cachedError = newError;
}

///////////////////////////////////////////////////////////////////////////////
//
inline HGLRC GLContext::GetRCHandle()
{
  return glRCHandle;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLContext::GetTextureFileNames(GLint texId, ImageSaveFiles &retSaveFiles)
{
  //If we have the image logger, query the logger
  if(interceptImage)
  {
    return interceptImage->GetTextureFileNames(texId,retSaveFiles);
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLContext::GetShaderFileName(GLint id,string &retShaderName)
{
  //If we have the shader logger, query the logger
  if(interceptShader)
  {
    return interceptShader->GetShaderFileName(id,retShaderName);
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLContext::GetShaderGLSLFileName(GLint id,InterceptShaderGLSL::GLSLRequestType shaderType,string &retShaderName)
{
  //If we have the GLSL shader logger, query the logger
  if(interceptShaderGLSL)
  {
    return interceptShaderGLSL->GetShaderFileName(id,shaderType,retShaderName);
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLContext::GetFrameFileNames(FrameInterceptFileNames &retFrameFileNames)
{
  // If there exists a frame logger, get the images
  if(interceptFrame)
  {
    return interceptFrame->GetFrameFileNames(retFrameFileNames);
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLContext::GetDisplayListFileName(GLint id,string &retListName)
{
  //If we have the display list logger, query the logger
  if(interceptList)
  {
    return interceptList->GetDisplayListFileName(id,retListName);
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
inline void GLContext::GetBoundTextures(BoundTextureArray &retArray)
{
  //If we have the image logger, query the logger
  if(interceptImage)
  {
    interceptImage->GetBoundTextures(retArray);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLContext::GetBoundTexture(GLenum oglTarget,uint texStage,uint &retTexID)
{
  //If we have the image logger, query the logger
  if(interceptImage)
  {
    return interceptImage->GetActiveTextureID(oglTarget,texStage,retTexID);
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void GLContext::GetBoundShaders(uint &vertexShaderID,uint &fragmentShaderID,GLuint &programGLSLID)
{
  vertexShaderID   = 0;
  fragmentShaderID = 0;
  programGLSLID    = 0;

  //If we have the shader logger, query the logger
  if(interceptShader)
  {
    interceptShader->GetBoundShaders(vertexShaderID,fragmentShaderID);
  }
  if(interceptShaderGLSL)
  {
    interceptShaderGLSL->GetBoundProgram(programGLSLID);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLContext::IsRenderFuncion(const FunctionData *funcData, uint funcTableIndex, const FunctionArgs & args)
{
  //If it is a straight render function, return now
  if(funcData->IsFlagSet(FDF_RENDER_FUNC))
  {
    return true;
  }

  //If there is a display list logger, query it to determine if this is a display list call and,
  //  if it is if a render call is performed in the list.
  if(interceptList && interceptList->IsRenderListFunction(funcData,funcTableIndex,args))
  {
    return true;
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
inline uintptr_t GLContext::GetContextThreadID() const
{
  return contextThreadID;
}
 

///////////////////////////////////////////////////////////////////////////////
//
inline void GLContext::SetContextActive(uintptr_t threadID) 
{
  contextThreadID = threadID;

  // Init the context pointers if not already init
  InitActiveContext();
}

///////////////////////////////////////////////////////////////////////////////
//
inline void GLContext::SetContextInActive() 
{
  contextThreadID = 0;
}


#endif // __GL_CONTEXT_H_
