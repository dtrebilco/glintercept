/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_PLUGIN_INTERFACE_H_
#define __INTERCEPT_PLUGIN_INTERFACE_H_

#include "GLDefines.h"
#include "GLStubs.h"
#include "GLStubsWGL.h"
#include "FunctionArgs.h"
#include "FunctionRetValue.h"
#include <OSDefines.h>
#include <InputUtils.h>

//Calling convention for the GLIntercept interface
#define GLIAPI GLI_STDCALL 
typedef unsigned int uint;

//Version of the plugin interface
#define __GLI_INTERCEPT_PLUGIN_VER  0x102

class InterceptPluginInterface;
class InterceptPluginCallbacks;
class DLLEventHandler;

//@
//  Summary:
//  GLIntercept plugins need to implement the following methods:
//  
//  To get the __GLI_INTERCEPT_PLUGIN_VER define value: (plugin version number)
//  uint GetFunctionLogPluginVersion();
//
//  To get the Plugin interface:
//  InterceptPluginInterface * CreateFunctionLogPlugin(const char *pluginName, InterceptPluginCallbacks * callBacks);
//
//  To provide plugin status data back to GLIntercept:
//  (There should only ever be one event handler referenced)
//
//  //Set the event handler, return false if it is already set.
//  bool RegisterDLLEventHandler  (DLLEventHandler *eventHandler);
//
//  //Unset the event handler. If the passed eventHandler is not equal to the internal handler, return false;
//  bool UnRegisterDLLEventHandler(DLLEventHandler *eventHandler);
//

typedef uint (GLIAPI *GETFUNCTIONLOGPLUGINVERSIONPROC)();
typedef InterceptPluginInterface * (GLIAPI *CREATEFUNCTIONLOGPLUGINPROC)(const char *pluginName, InterceptPluginCallbacks * callBacks);

typedef bool (GLIAPI *REGISTERDLLEVENTHANDLERPROC)(DLLEventHandler *eventHandler);
typedef bool (GLIAPI *UNREGISTERDLLEVENTHANDLERPROC)(DLLEventHandler *eventHandler);

typedef void (GLI_CDECL *LOGERRPROC)(const char *errorMessage,...);
 
//@
//  Summary:
//    Plugin Interface for GLIntercept. Plugins need to implement this 
//    interface and return a object of this class in a 
//    "CreateFunctionLogPlugin" exposed method.
//  
class InterceptPluginInterface
{
public:


  //@
  //  Summary:
  //    Called when an OpenGL function that has been registered 
  //    (via RegisterGLFunction) is about to be made.
  //  
  //  Parameters:
  //    userIndex - The user index value associated with this function. 
  //                This is set via SetFunctionID. Zero indicates no 
  //                 value has been set.
  //
  //    funcName  - The name of the function logged.
  //
  //    index    - A unique index of the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void GLIAPI GLFunctionPre (uint userIndex, const char *funcName, uint funcIndex, const FunctionArgs & args ) =0;

  //@
  //  Summary:
  //    Called when an OpenGL function that has been registered 
  //    (via RegisterGLFunction) has been made.
  //  
  //  Parameters:
  //    userIndex - The user index value associated with this function. 
  //                This is set via SetFunctionID. Zero indicates no 
  //                 value has been set.
  //
  //    funcName  - The name of the function logged.
  //
  //    funcIndex - A unique of the function logged (In the function table).
  //
  //    retVal    - The return vlaue of the function (if any).
  //
  virtual void GLIAPI GLFunctionPost(uint userIndex, const char *funcName, uint funcIndex, const FunctionRetValue & retVal) =0;

  //@
  //  Summary:
  //    Called when the OpenGL "frame end" call is about to be made.
  //  
  //  Parameters:
  //    funcName  - The name of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void GLIAPI GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args ) =0;

  //@
  //  Summary:
  //    Called after OpenGL "frame end" call has been made.
  //  
  //  Parameters:
  //    funcName  - The name of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void GLIAPI GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal) =0;

  //@
  //  Summary:
  //    Called when a OpenGL "render" call is about to be made.
  //    (Note: If the display list logger is active and a glCallList calls a render directly,
  //     this method will be called for the glCallList)
  //  
  //  Parameters:
  //    funcName  - The name of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void GLIAPI GLRenderPre (const char *funcName, uint funcIndex, const FunctionArgs & args ) =0;

  //@
  //  Summary:
  //    Called when a OpenGL "render" call has been made.
  //    (Note: If the display list logger is active and a glCallList calls a render directly,
  //     this method will be called for the glCallList)
  //  
  //  Parameters:
  //    funcName  - The name of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value of the function. (If any).
  //
  virtual void GLIAPI GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal) =0;

  //@
  //  Summary:
  //    Called when a OpenGL error occurs.
  //  
  //  Parameters:
  //    funcName  - The name of the function logged.
  //
  //    index    - A index to the function (In the function table).
  //
  virtual void GLIAPI OnGLError(const char *funcName, uint funcIndex) =0;

  //@
  //  Summary:
  //    Called when a OpenGL context is created.
  //  
  //  Parameters:
  //    rcHandle - The new OpenGL context.
  //
  virtual void GLIAPI OnGLContextCreate(HGLRC rcHandle) =0;

  //@
  //  Summary:
  //    Called when a OpenGL context is deleted.
  //  
  //  Parameters:
  //    rcHandle - The OpenGL context that is deleted.
  //
  virtual void GLIAPI OnGLContextDelete(HGLRC rcHandle) =0;

  //@
  //  Summary:
  //    Called when a OpenGL context is assigned (set).
  //  
  //  Parameters:
  //    oldRCHandle - The old (previous) OpenGL context.
  //
  //    newRCHandle - The new OpenGL context.
  //
  virtual void GLIAPI OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle) =0;

  //@
  //  Summary:
  //    Called when a OpenGL context share lists.
  //  
  //  Parameters:
  //    srcHandle - The context constaining the lists.
  //
  //    dstHandle - The context to now share the lists.
  //
  virtual void GLIAPI OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle) =0;


  //@
  //  Summary:
  //    Called when this plugin is to be destroyed. The plugin should delete 
  //    itself. (Note: If a plugin needs to shutdown, always request deletion
  //    via InterceptPluginCallbacks::DestroyPlugin which will call this method.
  //    Do not destroy the plugin by other means.)
  //  
  virtual void GLIAPI Destroy() =0;

/* Future methods?
  - Activate Loggers
  - Suspend Loggers
  - Set Logger data dirty??
*/

};

//Enum describing the mode of the current logger
enum LoggerMode
{
  LM_None =0,          //The logger is not currently logging
  LM_Text_Logging,     //The logger is logging text
  LM_XML_Logging       //The logger is logging XML
};

//Enum listing the types of resources that GLIntercept saves
enum LogResourceType
{
  LRT_Texture,         // Testure resources

  LRT_ShaderASM,       // ASM shader resources (ARB/NV)

  LRT_ShaderGLSL,      // GLSL shaders
  LRT_ProgramGLSL,     // GLSL programs

  LRT_DisplayList,     // Display lists

  LRT_ColorBuffer,     // The frame color buffer
  LRT_StencilBuffer,   // The frame stencil buffer
  LRT_DepthBuffer      // The frame depth buffer
};


//@
//  Summary:
//    This class is provided to the plugin instance to allow callbacks into
//    GLIntercept. This class is not to be deleted by the plugin.
//    Functions of note:
//    RegisterGLFunction - is used to flag to GLIntercept what OpenGL 
//                         functions the plugin needs to intercept. 
//    DestroyPlugin - Is used to flag to GLIntercept that the plugin needs
//                    deletion. 
//  
class InterceptPluginCallbacks
{
public:

  //@
  //  Summary:
  //    To get the configuration string for this plugin (if any).
  //    (String is in GLIntercept config file format)
  //  
  //  Returns:
  //    The configuration string for this plugin is returned.
  //
  virtual const char * GLIAPI GetConfigString() = 0;

  //@
  //  Summary:
  //    Get the OpenGL version number in float form.
  //    (Ensure GetGLInternalCallState returns true before calling)
  //  
  //  Returns:
  //    The OpenGL version number is returned if available else 0.0 is returned.
  //
  virtual float GLIAPI GetGLVersion() =0;

  //@
  //  Summary:
  //    Get if the OpenGL extension is supported. 
  //    (Ensure GetGLInternalCallState returns true before calling)
  //  
  //  Parameters:
  //    extensionName - The OpenGL extension to check for.
  //
  //  Returns:
  //    If the OpenGL extension is supported true is returned. If the extension
  //    test could not be performed or the extension does not exist, false is
  //    returned.
  //
  virtual bool GLIAPI IsGLExtensionSupported(const char *extensionName) =0;

  //@
  //  Summary:
  //    To get the OpenGL function name pointer.
  //    (Ensure GetGLInternalCallState returns true before calling)
  //  
  //  Parameters:
  //    functionName - The OpenGL function to get the pointer for.
  //
  //  Returns:
  //    The pointer to the OpenGL function is returned if it exists. 
  //    Else NULL is returned.
  //
  virtual void * GLIAPI GetGLFunction(const char *functionName) =0;

  //@
  //  Summary:
  //    To register the passed function name to receive updates into the
  //    plugin. Specifying "*" indicates all OpenGL functions.
  //  
  //  Parameters:
  //    functionName - The OpenGL function name to receive call backs on.
  //                   (via GLFunctionPre/GLFunctionPost)
  //
  virtual void GLIAPI RegisterGLFunction(const char *functionName) =0;

  //@
  //  Summary:
  //    To un-register the passed function name to prevent updates into the
  //    plugin. Specifying "*" indicates all OpenGL function updates are
  //    to be removed.
  //  
  //  Parameters:
  //    functionName - The OpenGL function name to remove call backs on.
  //                   (via GLFunctionPre/GLFunctionPost)
  //
  virtual void GLIAPI UnRegisterGLFunction(const char *functionName) =0;

  //@
  //  Summary:
  //    To set a user id to be passed to GLFunctionPre/GLFunctionPost when
  //    the specified function call is made.
  //  
  //  Parameters:
  //    functionName - The OpenGL function name to add the id to.
  //                   (function must be previously registered with 
  //                    RegisterGLFunction)
  //
  //    newID - The ID to be passed when the above function name is called.
  //
  //  Returns:
  //    If the function id was set, true is returned. Else false is returned.
  //
  virtual bool GLIAPI SetFunctionID(const char *functionName, uint newID) =0;

  //@
  //  Summary:
  //    To flag if the registered functions (via RegisterGLFunction)
  //    are only to be called when there exists a valid OpenGL context.
  //    By default, this value is true and callbacks are only made on 
  //    valid OpenGL contexts. This flag does not affect the other 
  //    callbacks (ie. OnGLContext* etc)
  //  
  //  Parameters:
  //    enable - Flag to indicate if function callbacks are received
  //             only on valid OpenGL contexts. If true, all OpenGL calls
  //             made outside of a context (including wgl calls) will be
  //             ignored. 
  //
  virtual void GLIAPI SetContextFunctionCalls(bool enable) =0;
  
  //@
  //  Summary:
  //    To convert the passed argument list into a string. 
  //    (seperated by commas)
  //  
  //  Parameters:
  //    funcIndex - The OpenGL function index.
  //
  //    args      - The arguments from the function.
  //
  //    strLength - The maximum length of the returned string.
  //
  //    retString - The pointer to return the sting in.
  //
  //  Returns:
  //    Returns true on success, false on failure. 
  //    
  virtual bool GLIAPI GetGLArgString(uint funcIndex, const FunctionArgs & args, uint strLength, char *retString) =0;


  //@
  //  Summary:
  //    To convert the passed OpenGL return value into a string. 
  //  
  //  Parameters:
  //    funcIndex - The OpenGL function index.
  //
  //    retVal    - The return value to convert.
  //
  //    strLength - The maximum length of the returned string.
  //
  //    retString - The pointer to return the sting in.
  //
  //  Returns:
  //    Returns true on success, false on failure. 
  //    
  virtual bool GLIAPI GetGLReturnString(uint funcIndex, const FunctionRetValue & retVal, uint strLength, char *retString) =0;

  //@
  //  Summary:
  //    To get the state if extra function calls to the GL state can be made
  //    internally. This needs to be true if OpenGL calls are to be made. 
  //    (as well as version and extension checking) The call state can be false
  //    if there is no current context, between glBegin/glEnd block or 
  //    glBeginList/glEndList block.
  //  
  //  Returns:
  //    The internal call mode is returned.
  //
  virtual bool GLIAPI GetGLInternalCallState() =0;

  //@
  //  Summary:
  //    To get the current frame number. This number represents the number 
  //    of frames (ie swap buffers) that have elapsed since the start of the
  //    application.
  //  
  //  Returns:
  //    The current frame number is returned.
  //
  virtual uint GLIAPI GetFrameNumber() =0;

  //@
  //  Summary:
  //    To get the current thread id.
  //  
  //  Returns:
  //    The current thread id is returned.
  //
  virtual uintptr_t GLIAPI GetThreadID() =0;

  //@
  //  Summary:
  //    To get the current OpenGL render context id.
  //    (NULL on no context)
  //  
  //  Returns:
  //    The current render context id is returned.
  //
  virtual HGLRC GLIAPI GetGLContextID() =0;

  //@
  //  Summary:
  //    To get the error logger function. Plugins can use this function
  //    to log error messages to the GLIntercept error logger.
  //    This function is valid for the duration of the application. 
  //    (ie. it is not destroyed when this interface is destroyed)
  //  
  //  Returns:
  //    A printf style error log function is returned. 
  //
  virtual LOGERRPROC GLIAPI GetLogErrorFunction() =0;

  //@
  //  Summary:
  //    To get the listing of raw OpenGL commands that directly call the
  //    OpenGL driver (version 1.1). (by-passing GLIntercept)
  //  
  //  Returns:
  //    The structure of core OpenGL methods is returned.
  //
  virtual const GLCoreDriver * GLIAPI GetCoreGLFunctions() =0; 

  //@
  //  Summary:
  //    To get the listing of raw WGL (Windows-OpenGL) commands that directly
  //    call the OpenGL driver. (by-passing GLIntercept)
  //  
  //  Returns:
  //    The structure of WGL methods is returned.
  //
  virtual const WGLDriver * GLIAPI GetWGLFunctions() =0; 

  //@
  //  Summary:
  //    To get the input utility object from GLIntercept.
  //  
  //  Returns:
  //    The input utility object is returned.
  //
  virtual const InputUtils * GLIAPI GetInputUtils() =0;

  //@
  //  Summary:
  //    Adds a override function of the specified name. When a OpenGL function
  //    is overridden, it is called by the external application instead of the
  //    "real" OpenGL call. (Internal GLIntercept calls are unaffected)
  //    NOTE: Remember to use the OpenGL calling convention on new functions.
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
  virtual bool GLIAPI AddOverrideFunction(const char * funcName, void * overrideFunctionPtr, void *origFuncPtr) =0;

  //@
  //  Summary:
  //    To retrieve the current mode of the logger.
  //  
  //  Returns:
  //    The current logger mode is returned.
  //    
  virtual LoggerMode GLIAPI GetLoggerMode() const =0;

  //@
  //  Summary:
  //    To get the current logger path (Including trailing seperator).
  //    The path may change between OpenGL calls, so do not store the 
  //    returned string.
  //  
  //  Parameters:
  //    strLength - The maximum length of the returned string.
  //
  //    retString - The pointer to return the sting in.
  //
  //  Returns:
  //    Returns true if there is a current logger path (and it could fit in
  //    the passed string). Else false is returned.
  //    
  virtual bool GLIAPI GetLoggerPath(uint strLength, char *retString) =0;

  //@
  //  Summary:
  //    To get the filename of a GLIntercept saved resource (while logging).
  //    The returned filename is relative to the current logging path.
  //    The filename may change between OpenGL calls as resources are updated.
  //  
  //  Parameters:
  //    type   - The type of resource to get.
  //
  //    typeID - The ID of the resource to get. (texture ID/shader ID) 
  //             For color buffers it is the render target number.
  //
  //    strLength - The maximum length of the returned string.
  //
  //    retString - The pointer to return the sting in.
  //
  //  Returns:
  //    Returns true if there is a current resource filename (and it could fit in
  //    the passed string). Else false is returned.
  //    
  virtual bool GLIAPI GetLogResourceFilename(LogResourceType type, uint typeID, uint strLength, char *retString) =0;
 
  //@
  //  Summary:
  //    To add a raw string to the output log. 
  //    Use GetLoggerMode to determine the format the string should be in.
  //  
  //  Parameters: 
  //    addString  - The string to add to the output log.
  //
  virtual void GLIAPI AddLoggerString(const char *addString) =0;

  //@
  //  Summary:
  //    To flag that the plugin needs to be destroyed. During the next update,
  //    the plugin will be unloaded and Destroy will be called to destroy
  //    the plugin.
  //  
  //  Returns:
  //    True is returned if successfully flagged, Flase is returned if the 
  //    plugin is unknown or the plugin is already flagged for destruction.
  //
  virtual bool GLIAPI DestroyPlugin() =0;

protected:

  //@
  //  Summary:
  //    Protected destructor to ensure no accidential deletion.
  //  
  virtual ~InterceptPluginCallbacks(){};

};

//@
//  Summary:
//    This class passes DLL plugin events back into the main GLIntercept 
//    application.
//
class DLLEventHandler
{
public:

  //@
  //  Summary:
  //    The DLL plugin is to call this method when the DLL is being unloaded.
  //
  virtual void OnDLLDelete() =0;

protected:

  //@
  //  Summary:
  //    Protected destructor to ensure no accidential deletion.
  //  
  virtual ~DLLEventHandler() {};

};


#endif // __INTERCEPT_PLUGIN_INTERFACE_H_
