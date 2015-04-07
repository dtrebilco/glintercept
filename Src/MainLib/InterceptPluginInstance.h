/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_PLUGIN_INSTANCE_H_
#define __INTERCEPT_PLUGIN_INSTANCE_H_

#include "GLInterceptConfig.h"
#include "InterceptPluginInterface.h"
#include "DLLLoader.h"

#include <string>
#include <vector>

using namespace std;


class GLDriver;
class InterceptPluginManager;
class InterceptPluginDLLInstance;

//@
//  Summary:
//    This class provides an implementation of the the plugin call-back methods
//    and holds a single plugin instance.
//  
class InterceptPluginInstance : public InterceptPluginCallbacks
{
public:

  //@
  //  Summary:
  //    Constructor 
  //  
  //  Parameters:
  //    pluginManager - The managers of this plugin instance.
  //
  //    driver  - The driver used to make queries on the OpenGL state.
  //
  //    dllInstance - The instance if the dll loaded.
  //
  InterceptPluginInstance(InterceptPluginManager *pluginManager, GLDriver *driver,InterceptPluginDLLInstance *dllInstance);
  virtual ~InterceptPluginInstance();

  //@
  //  Summary:
  //    To initialize the plugin by loading the specified dll.
  //  
  //  Parameters:
  //    pluginName  - The specific plugin name to load. 
  //                  (A dll can hold multiple plugins)
  //
  //    newConfigString - The configuration string for the plugin. 
  //                      (In GLIntercept config file format)
  //
  //  Returns:
  //    If initialization is successful, true is returned. Else false is returned.
  //
  bool Init(const char *pluginName, const string &newConfigString);

  //@
  //  Summary:
  //    To get the plugin instance interface.
  //  
  //  Returns:
  //    The plugin instance interface is returned.
  //
  inline InterceptPluginInterface * GetInterface();

  //@
  //  Summary:
  //    If a plugion dll is unloaded unexpectantly, (ie during shutdown). 
  //    This method is called to handle the shutdown.
  //  
  //  Parameters:
  //    dllInstance - The DLL instance that is shutting down.
  //
  void OnDLLUnload(InterceptPluginDLLInstance *dllInstance);

  //@
  //  Summary:
  //    Get if function calls are only to be made on a valid OpenGL context.
  //  
  //  Returns:
  //    Returns true if function calls for this plugin are only to be made 
  //    on valid OpenGL contexts.
  //
  inline bool GetContextFunctionCalls();


  // Implementation of the InterceptPluginCallbacks interface

  virtual const char * GLIAPI GetConfigString();
  virtual float GLIAPI GetGLVersion();
  virtual bool  GLIAPI IsGLExtensionSupported(const char *extensionName);
  virtual void *GLIAPI GetGLFunction(const char *functionName);

  virtual void GLIAPI RegisterGLFunction(const char *functionName);
  virtual void GLIAPI UnRegisterGLFunction(const char *functionName);

  virtual bool GLIAPI SetFunctionID(const char *functionName, uint newID);

  virtual void GLIAPI SetContextFunctionCalls(bool enable);

  virtual bool GLIAPI GetGLArgString   (uint funcIndex, const FunctionArgs & args, uint strLength, char *retString);
  virtual bool GLIAPI GetGLReturnString(uint funcIndex, const FunctionRetValue & retVal, uint strLength, char *retString);

  virtual bool GLIAPI GetGLInternalCallState();

  virtual uint      GLIAPI GetFrameNumber();
  virtual uintptr_t GLIAPI GetThreadID();
  virtual HGLRC     GLIAPI GetGLContextID();

  virtual LOGERRPROC GLIAPI GetLogErrorFunction();

  virtual const GLCoreDriver * GLIAPI GetCoreGLFunctions(); 
  virtual const WGLDriver *    GLIAPI GetWGLFunctions(); 
  virtual const InputUtils *   GLIAPI GetInputUtils();

  virtual bool GLIAPI AddOverrideFunction(const char * funcName, void * overrideFunctionPtr, void *origFuncPtr);
  virtual LoggerMode GLIAPI GetLoggerMode() const;
  virtual bool GLIAPI GetLoggerPath(uint strLength, char *retString);
  virtual bool GLIAPI GetLogResourceFilename(LogResourceType type, uint typeID, uint strLength, char *retString);

  virtual void GLIAPI AddLoggerString(const char *addString);

  //@
  //  Summary:
  //    Destroys the internal plugin interface. Assigns a dummy plugin 
  //    until this class is destroyed. 
  //
  //  Returns:
  //    True is returned on successful destruction, false if not. 
  //    (ie. if already destroyed)
  //
  virtual bool GLIAPI DestroyPlugin();


protected:

  GLDriver                 *driver;               // The OpenGL driver
  InterceptPluginManager   *pluginManager;        // The manager of this plugin

  string                    configString;         // The configuration string for the plugin

  bool                      contextFunctionCalls; // Flag to indicate if function callbacks should only occur on valid OpenGL contexts
  bool                      hasRegisteredFunctions;// Flag indicating if the loaded plugin has registered functions. (ie. do not unload the DLL on interface delete)

  InterceptPluginInterface   *gliPlugin;          // The GLI plugin 
  InterceptPluginDLLInstance *dllInstance;        // The dll instance

};

///////////////////////////////////////////////////////////////////////////////
//
inline InterceptPluginInterface * InterceptPluginInstance::GetInterface()
{
  return gliPlugin;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool InterceptPluginInstance::GetContextFunctionCalls()
{
  return contextFunctionCalls;
}


#endif // __INTERCEPT_PLUGIN_INSTANCE_H_
