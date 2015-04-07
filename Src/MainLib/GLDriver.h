/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __GL_DRIVER_H_
#define __GL_DRIVER_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "GLStubs.h"
#include "GLStubsWGL.h"
#include "DLLLoader.h"
#include "InterceptLog.h"
#include "ConfigData.h"
#include "GLContext.h"
#include "InterceptPluginInterface.h"

#include <InputUtils.h>
#include <string>

using namespace std;

class ExtensionFunction;
class InterceptPluginManager;


//@
//  Summary:
//    This class creates and init's the OpenGL driver. It is responsible for 
//    opening and storing function pointers to the OpenGL libraries.
//  
class GLDriver
{
public:

  //Constructor/destructor
  GLDriver();
  virtual ~GLDriver();

  //@
  //  Summary:
  //    Inits the driver with the passed dll name.
  //  
  //  Returns:
  //    True is returned on success, false if otherwise. (Cannot use this class
  //    if Init fails)
  //
  bool Init();

  //@
  //  Summary:
  //    Inits the function table ready for use. 
  //    (Needs to be static as it can be called before and object of this class is init)
  //  
  static void InitFunctionTable();

  //@
  //  Summary:
  //    Logs that a function call of the type at index is about to be made.
  //  
  //  Parameters:
  //    index  - The index of the function call (in the function table) about to be made.
  //
  //    args   - The list of arguments for this function
  //
  //  Returns:
  //    True is returned on successful logging , false if otherwise
  //
  bool LogFunctionPre(uint index, const FunctionArgs & args);

  //@
  //  Summary:
  //    Logs that a function call has just been made (post function call).
  //  
  //  Parameters:
  //    index  - The index of the function call (in the function table) that was made.
  //
  //    returnVal - The return value (if any) of the function call.
  //
  //  Returns:
  //    True is returned on successful logging , false if otherwise
  //
  bool LogFunctionPost(uint index, const FunctionRetValue & returnVal);

  //@
  //  Summary:
  //    Adds a new extension function to the tables for logging
  //  
  //  Parameters:
  //    funcName  - The name of the new function to log.
  //
  //    functionPtr - The pointer to the function to log. 
  //
  //  Returns:
  //    The pointer to be used for the function call is returned. 
  //    (If a entry cannot be made, the passed function ptr is returned.)
  //
  void * AddExtensionFunction(const string & funcName,void * functionPtr);  

  //@
  //  Summary:
  //    Adds a override function of the specified name. 
  //    If the function has already been overridden or the number of wrapper
  //    functions is exceeded, false is returned.
  //  
  //  Parameters: 
  //    funcName  - The name of the function to override.
  //
  //    overrideFunctionPtr - The function pointer of the override method.
  // 
  //    origFuncPtr - The "real" OpenGL function being overridden. Can be 
  //                  NULL if the function does not exist in OpenGL.
  //
  //  Returns:
  //    Returns true if the override was successful, false on failure.
  //
  bool AddOverrideFunction(const string & funcName, void * overrideFunctionPtr, void *origFuncPtr);

  //@
  //  Summary:
  //    Returns the cached eroor code (if any)
  //  
  //  Returns:
  //    The cached error code (if any) is returned.
  //
  inline GLenum GetCachedErrorCode();

  //@
  //  Summary:
  //    Reset the cached error code to the default no-error state.
  //    This method is usually called from the glGetError method once 
  //    the user has received an error.
  //  
  inline void ResetCachedErrorCode();

  //@
  //  Summary:
  //    To set the state if extra function calls to the GL state can be made
  //    internally. (ie. these call cannot be made between a glBegin()..glEnd())
  //    The mode is actually set by a counter (To allow multiple disables to 
  //    not over-ride each other)
  //  
  //  Parameters:
  //    mode  - Mode to enable/disable extra GL calls.
  //
  void SetInternalGLCallMode(bool mode);

  //@
  //  Summary:
  //    To get the state if extra function calls to the GL state can be made
  //    internally.
  //  
  //  Returns:
  //    The internal call mode is returned.
  //
  inline bool GetInternalGLCallMode() const;
  
  //@
  //  Summary:
  //    To set if we are in a glBegin/glEnd set of function calls.
  //  
  //  Parameters:
  //    mode  - The mode to set
  //
  void SetBeginEndState(bool mode);

  //@
  //  Summary:
  //    Returns if we are in a glBegin/glEnd segment.
  //  
  //  Returns:
  //    The state of the glBegin/glEnd mode is returned. 
  //
  inline bool GetBeginEndState() const;
 
  //@
  //  Summary:
  //    To set if we are in a glNewList/glEndList set of function calls.
  //  
  //  Parameters:
  //    mode  - The mode to set
  //
  void SetNewListState(bool mode);

  //@
  //  Summary:
  //    Returns if we are in a glNewList/glEndList segment.
  //  
  //  Returns:
  //    The state of the glNewList/glEndList mode is returned. 
  //
  inline bool GetNewListState() const;

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

  //@
  //  Summary:
  //    To get the current frame number.
  //  
  //  Returns:
  //    The current frame number is returned.
  //
  inline uint GetFrameNumber() const;

  //@
  //  Summary:
  //    To create a new structure to hold OpenGL the new OpenGL context.
  //  
  //  Parameters:
  //    rcHandle  - The new OpenGL context handle.
  //
  //  Returns:
  //    Returns true on success, false on failure. 
  //    
  bool CreateOpenGLContext(HGLRC rcHandle);

  //@
  //  Summary:
  //    To delete the structure that holds a OpenGL context.
  //  
  //  Parameters:
  //    rcHandle  - The OpenGL context handle of the data to delete.
  //
  //  Returns:
  //    Returns true on success, false on failure. 
  //    
  bool DeleteOpenGLContext(HGLRC rcHandle);

  //@
  //  Summary:
  //    To set the currently active OpenGL context. 
  //    Ensure the previous context is shut down first before the 
  //    actual OpenGL context switch occurs. (This is due to some 
  //    context data may need to be saved)
  //  
  //  Parameters:
  //    rcHandle  - The OpenGL context handle to switch to. (Can be NULL)
  //
  //  Returns:
  //    Returns true on success, false on failure. 
  //    
  bool SetOpenGLContext(HGLRC rcHandle);

  //@
  //  Summary:
  //    To share the data lists/images etc between the two OpenGL contexts.
  //    This is to be called after a successful list sharing has occured in
  //    OpenGL.
  //  
  //  Parameters:
  //    contextA  - The OpenGL context that contains the data to share.
  //
  //    contextB  - The OpenGL context to receive the context data. 
  //                (should currently be empty)
  //
  void ShareOpenGLContext(HGLRC contextA, HGLRC contextB);

  //@
  //  Summary:
  //    To get the OpenGL context structre for the passed handle. 
  //  
  //  Parameters:
  //    rcHandle  - The OpenGL context handle to retrieve data for
  //
  //  Returns:
  //    If the handle could be found, the structure is returned else
  //    NULL is returned.
  //    
  GLContext *GetOpenGLContext(HGLRC rcHandle) const;

  //@
  //  Summary:
  //    To get the OpenGL current OpenGL context structure. 
  //  
  //  Returns:
  //    If there is a current context, the structure is returned else
  //    NULL is returned.
  //    
  inline GLContext * GetCurrentContext() const;

  //@
  //  Summary:
  //    To enable the function timer to record the duration of the next OpenGL
  //    call. The value can then be returned via GetFunctionTime.
  //  
  inline void SetFunctionTimer();

  //@
  //  Summary:
  //    To get the time (in microseconds) that the previous OpenGL function
  //    took to execute. If the time logger is not enabled or ready/ zero 
  //    is returned.
  //  
  //  Returns:
  //    The amount of time for the previous OpenGL function is returned.
  //    
  inline uint GetFunctionTime() const;

  //@
  //  Summary:
  //    To get how many sub-OpenGL function calls have been made. 
  //    (ie. OpenGL calls that make OpenGL calls)
  //  
  //  Returns:
  //    The function call depth is returned.
  //    
  inline uint GetFunctionCallDepth() const;

  //@
  //  Summary:
  //    To get a pointer to the input utilities.
  //  
  //  Returns:
  //    The input utilities object is returned.
  //    
  inline const InputUtils *GetInputUtils() const;

  //@
  //  Summary:
  //    To retrieve the current mode of the logger.
  //  
  //  Returns:
  //    The current logger mode is returned.
  //    
  inline LoggerMode GetLoggerMode() const;

  //@
  //  Summary:
  //    To retrieve the current log path.
  //  
  //  Returns:
  //    The current logger path is returned.
  //    
  inline string GetLoggerPath() const;

  //@
  //  Summary:
  //    To add a raw string to the output log. 
  //  
  //  Parameters: 
  //    addString  - The string to add to the output log.
  //
  inline void AddLoggerString(const char *addString);

protected:
  bool       isInit;                              // Has Init() been called successfully
  DLLLoader  openGLLib;                           // The OpenGL library
  ConfigData configData;                          // The configuration data
  InputUtils inputUtils;                          // The input utilities

  uint       frameNumber;                         // The rendering frame number
  uint       loggingStartFrameNum;                // The frame number logging started on (if logging is enabled)
  string     currLogDir;                          // The current logging directory
  bool       loggingEnabled;                      // Flag to indicate if logging is currently enabled
  bool       threadCheckingEnabled;               // Flag to indicate if thread checking is currently enabled
  uint       functionCallDepth;                   // The OpenGL function call depth (OpenGL functions can call other OpenGL functions)

  ExtensionFunction *extensionFunction;           // The class used to register extension functions 
  InterceptLog      *interceptLog;                // The class used to log function calls
  InterceptLog      *errorDataLog;                // The class used to aid reporting glErrors (translates parameters and return codes)

  string             errorFuncString;             // The string representation of the last function (used in error reporting)

  GLContext         *glContext;                   // The current OpenGL context
  vector<GLContext *> glContextArray;             // The array of all the OpenGL contexts

  InterceptPluginManager *pluginManager;          // The plugin manager


  uint   internalCallModeCount;                   // Counter to indicate if we can make extra GL function calls internally. (non-zero is false)
  bool   glBeginEndState;                         // Flag to indicate if we are currently processing a glBegin/glEnd block
  bool   glNewListState;                          // Flag to indicate if we are currently processing a glNewList/glEndList block


  TimeDiff functionTimer;                         // The timer used to measure how much time is spent in a OpenGL function
  uint     functionTime;                          // The last calculated time value for a function in microseconds
  bool     functionTimeEnabled;                   // Flag to indicate if function timing is enabled


  //@
  //  Summary:
  //    To init the function logger.
  //  
  //  Returns:
  //    True is returned on success, false is otherwise
  //
  bool InitFunctionLog();

  //@
  //  Summary:
  //    To handle the end of frame event.
  //  
  void ProcessFrameEnd();

  //@
  //  Summary:
  //    To flag to the OpenGL error reporter the passed function and 
  //    function data (Before the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  void FunctionErrorCheckPre(const FunctionData *funcData,uint index, const FunctionArgs & args);

  //@
  //  Summary:
  //    To check for an OpenGL error (if necessary and possible) and report 
  //    any errors found.
  //  
  //  Parameters:
  //    funcData - The data of the function that was just called.
  //
  //    index    - A index to the function logged (In the function table).
  //
  void FunctionErrorCheckPost(const FunctionData *funcData,uint index);
  

};

///////////////////////////////////////////////////////////////////////////////
//
inline GLenum GLDriver::GetCachedErrorCode()
{
  if(glContext)
  {
    return glContext->GetCachedError();
  }
 
  return GL_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void GLDriver::ResetCachedErrorCode()
{
  if(glContext)
  {
    glContext->SetCachedError(GL_NO_ERROR);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLDriver::GetBeginEndState() const
{
  return glBeginEndState;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLDriver::GetNewListState() const
{
  return glNewListState;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLDriver::GetInternalGLCallMode() const
{
  //Determine if the call count is ok for internal OpenGL calls
  //  (and we have a OpenGL context)
  if(glContext && internalCallModeCount == 0 )
  {
    return true;
  }
  else
  {
    return false;
  } 
}


///////////////////////////////////////////////////////////////////////////////
//
inline uint GLDriver::GetFrameNumber() const
{
  return frameNumber;
}

///////////////////////////////////////////////////////////////////////////////
//
inline GLContext * GLDriver::GetCurrentContext() const
{
  return glContext;
}


///////////////////////////////////////////////////////////////////////////////
//
inline void GLDriver::SetFunctionTimer()
{
  functionTimeEnabled = true;
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint GLDriver::GetFunctionTime() const
{
  return functionTime;
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint GLDriver::GetFunctionCallDepth() const
{
  return functionCallDepth;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const InputUtils *GLDriver::GetInputUtils() const
{
  return &inputUtils;
}

///////////////////////////////////////////////////////////////////////////////
//
inline LoggerMode GLDriver::GetLoggerMode() const
{
  if(!interceptLog)
  {
    return LM_None;
  }

  //Return if text or XML logging
  if(configData.logXMLFormat)
  {
    return LM_XML_Logging;
  }

  return LM_Text_Logging;
}

///////////////////////////////////////////////////////////////////////////////
//
inline string GLDriver::GetLoggerPath() const
{
  return currLogDir;
}


///////////////////////////////////////////////////////////////////////////////
//
inline void GLDriver::AddLoggerString(const char *addString)
{
  //Add to the log if logging
  if(interceptLog)
  {
    interceptLog->AddLogString(addString);
  }
}




#endif // __GL_DRIVER_H_
