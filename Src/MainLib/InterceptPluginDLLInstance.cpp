/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptPluginInstance.h"
#include "InterceptPluginDLLInstance.h"
#include "InterceptPluginManager.h"
#include "GLDriver.h"



USING_ERRORLOG

//Static array of loaded dlls
vector<InterceptPluginDLLInstance*> InterceptPluginDLLInstance::loadedDLLArray;

///////////////////////////////////////////////////////////////////////////////
//
InterceptPluginDLLInstance::InterceptPluginDLLInstance(InterceptPluginManager *newManager):
dllLoader(NULL),
manager(newManager),
DLLCreateLogPlugin(NULL),
RegisterEventHandler(NULL),
UnRegisterEventHandler(NULL)
{
  //Add to the loaded array
  loadedDLLArray.push_back(this);
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptPluginDLLInstance::~InterceptPluginDLLInstance()
{
  //Delete the DLL loader
  if(dllLoader)
  {
    //Set the loader pointer to NULL to be safe
    DLLLoader *deleteLoader = dllLoader;
    dllLoader = NULL;

    //Unregister from the dll
    if(!UnRegisterEventHandler ||
       !UnRegisterEventHandler(this))
    {
      LOGERR(("InterceptPluginDLLInstance - Error unregistering handler"));
    }

    //Delete the dll
    delete deleteLoader;

    //Reset functions
    DLLCreateLogPlugin     = NULL;
    RegisterEventHandler   = NULL;
    UnRegisterEventHandler = NULL;
  }

  //Note: There is a occasional minor problem with the loaded array 
  //      being emptied on shutdown before this destructor is called.
  //      (only on application exit)

  //Remove from the array
  for(uint i=0;i<loadedDLLArray.size();i++)
  {
    //Delete this from the array
    if(loadedDLLArray[i] == this)
    {
      loadedDLLArray.erase(loadedDLLArray.begin()+i);
      break;
    }
  }
  

}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginDLLInstance::Init(const char *dllName)
{
  //Create the plugin loader
  dllLoader = new DLLLoader;

  //Attempt to open the dll
  if(!dllLoader->Init(dllName))
  {
    LOGERR(("InterceptPluginDLLInstance::Init - Unable to open plugin %s",dllName));
    return false;
  }

  uint (GLIAPI *GetFunctionLogPluginVersion)() = NULL;

  //Attempt to get the entry points
  void **loadFunction=NULL;
  loadFunction  = (void**)&GetFunctionLogPluginVersion;
  *loadFunction = dllLoader->GetFunction("GetFunctionLogPluginVersion");

  loadFunction  = (void**)&DLLCreateLogPlugin;
  *loadFunction = dllLoader->GetFunction("CreateFunctionLogPlugin");

  loadFunction  = (void**)&RegisterEventHandler;
  *loadFunction = dllLoader->GetFunction("RegisterDLLEventHandler");

  loadFunction  = (void**)&UnRegisterEventHandler;
  *loadFunction = dllLoader->GetFunction("UnRegisterDLLEventHandler");


  //Test for valid entry points
  if(GetFunctionLogPluginVersion == NULL || DLLCreateLogPlugin     == NULL ||
     RegisterEventHandler        == NULL || UnRegisterEventHandler == NULL)
  {
    LOGERR(("InterceptPluginDLLInstance::Init - Invalid entry points in plugin %s",dllName));
    return false;
  }

  //Check the version number
  uint pluginVersion = GetFunctionLogPluginVersion();
  if(pluginVersion != __GLI_INTERCEPT_PLUGIN_VER)
  {
    LOGERR(("InterceptPluginDLLInstance::Init - Plugin %s version %x does not match version %x",dllName,pluginVersion,(uint)__GLI_INTERCEPT_PLUGIN_VER));

    //Unload the plugin
    delete dllLoader;
    dllLoader = NULL;

    DLLCreateLogPlugin     = NULL;
    RegisterEventHandler   = NULL;
    UnRegisterEventHandler = NULL;
    return false;
  }

  //Register this as the callback object
  if(!RegisterEventHandler(this))
  {
    LOGERR(("InterceptPluginDLLInstance::Init - Error registering dll callback in plugin %s",dllName));

    //Unload the plugin
    delete dllLoader;
    dllLoader = NULL;

    DLLCreateLogPlugin     = NULL;
    RegisterEventHandler   = NULL;
    UnRegisterEventHandler = NULL;
    return false;  
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptPluginInterface * InterceptPluginDLLInstance::CreateLogPlugin(const char *pluginName, InterceptPluginCallbacks * callBacks)
{
  //If the function pointer has been retrieved, get the interface
  if(DLLCreateLogPlugin)
  {
    return DLLCreateLogPlugin(pluginName,callBacks);
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginDLLInstance::OnDLLDelete()
{
  //Check that these exist a dll
  if(!dllLoader)
  {
    LOGERR(("InterceptPluginDLLInstance::OnDLLDelete - No open DLL?"));
    return; 
  }

  //Add a reference to ourselves (safety to prevent deletion)
  AddReference();

  //Reset variables
  DLLCreateLogPlugin     = NULL;
  RegisterEventHandler   = NULL;
  UnRegisterEventHandler = NULL;

  //Flag to the DLL that it is not to be unloaded manually
  dllLoader->FlagDLLUnloaded();

  //Delete the DLL
  delete dllLoader;
  dllLoader = NULL; 

  //Call the manager 
  manager->OnDLLUnload(this);

  //Release the refernce
  ReleaseReference();
}


///////////////////////////////////////////////////////////////////////////////
//
InterceptPluginDLLInstance * InterceptPluginDLLInstance::CreateDLLInstance(InterceptPluginManager *manager,const char *dllName)
{
  //Search the array
  for(uint i=0; i<loadedDLLArray.size(); i++)
  {
    //Check if the dll has been loaded
    if(loadedDLLArray[i]->dllLoader && 
       loadedDLLArray[i]->dllLoader->IsDLLNameMatch(dllName))
    {
      //If found, add a reference and return.
      loadedDLLArray[i]->AddReference();

      return loadedDLLArray[i];
    }
  }

  //Create a new instance
  InterceptPluginDLLInstance *retInstance = new InterceptPluginDLLInstance(manager);

  //Attempt to init
  if(!retInstance->Init(dllName))
  {
    retInstance->ReleaseReference();
    return NULL;
  }

  return retInstance;
}


