/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_PLUGIN_DLL_INSTANCE_H_
#define __INTERCEPT_PLUGIN_DLL_INSTANCE_H_

#include "GLInterceptConfig.h"
#include "InterceptPluginInterface.h"
#include "DLLLoader.h"
#include "ReferenceCount.h"

#include <string>
#include <vector>

using namespace std;



class GLDriver;
class InterceptPluginManager;


//@
//  Summary:
//    This class loads and initializes GLIntercept plugin DLL's.
//    This class also handles DLL events coming from the plugin. 
//    (ie. unexpected plugin deletion)
//  
class InterceptPluginDLLInstance : public DLLEventHandler, public ReferenceCount
{
public:

  //@
  //  Summary:
  //    To create a plugin DLL Instance.
  //  
  //  Parameters:
  //    manager  - The plugin manager.
  //
  //    dllName  - The specific dll name to load.
  // 
  //  Returns:
  //    If the plugin could be loaded, it is returned. Else NULL is returned. 
  //
  static InterceptPluginDLLInstance * CreateDLLInstance(InterceptPluginManager *manager,const char *dllName);

  //@
  //  Summary:
  //    To create a plugin object from the loaded DLL of the specified name.
  //  
  //  Parameters:
  //    pluginName  - The name of the plugin in the DLL. 
  //                  (a DLL can have multiple plugins). 
  //
  //  Returns:
  //    If the plugin object could be created, it is returned. 
  //    Else NULL is returned.
  //
  InterceptPluginInterface * CreateLogPlugin(const char *pluginName, InterceptPluginCallbacks * callBacks);

  //@
  //  Summary:
  //    To get the file name of the dll used.
  //  
  //  Returns:
  //    If the plugin is loaded the name is returned Else an empty string is
  //    returned.
  //
  inline string GetDLLFileName() const;

protected:

  DLLLoader               *dllLoader;             // The dll that the plugin data comes from
  InterceptPluginManager  *manager;               // The manager of the plugin system

  CREATEFUNCTIONLOGPLUGINPROC   DLLCreateLogPlugin;     //Create interface procedure
  REGISTERDLLEVENTHANDLERPROC   RegisterEventHandler;   //Register this as an event handler
  UNREGISTERDLLEVENTHANDLERPROC UnRegisterEventHandler; //Un-Register this as an event handler

  static vector<InterceptPluginDLLInstance*> loadedDLLArray;   //Array of loaded dll's


  //@
  //  Summary:
  //    Constructor.
  //  
  //  Parameters:
  //    manager  - The manager of this dll instance. 
  //
	InterceptPluginDLLInstance(InterceptPluginManager *manager);

  //@
  //  Summary:
  //    Destructor. Will unload any loaded plugin dlls.
  //  
  virtual ~InterceptPluginDLLInstance();


  //@
  //  Summary:
  //    To initialize the DLL plugin by loading the specified dll name.
  //  
  //  Parameters:
  //    dllName  - The specific dll name to load. 
  //
  //  Returns:
  //    If initialization is successful, true is returned. Else false is returned.
  //
  bool Init(const char *dllName);

  //@
  //  Summary:
  //    A call back (from DLLEventHandler) that gets called when the plugin
  //    DLL is deleted unexpectantly (ie. on shutdown)
  //  
  virtual void OnDLLDelete();
};


///////////////////////////////////////////////////////////////////////////////
//
inline string InterceptPluginDLLInstance::GetDLLFileName() const
{
  //Return the dll name if available
  if(dllLoader)
  {
    return dllLoader->GetFullDLLFileName();
  }
  return "";
}


#endif // __INTERCEPT_PLUGIN_DLL_INSTANCE_H_
