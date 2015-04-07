/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_PLUGIN_MANAGER_H_
#define __INTERCEPT_PLUGIN_MANAGER_H_

#include "GLInterceptConfig.h"
#include "ConfigData.h"
#include "GLDefines.h"
 

#include <string>
#include <vector>

using namespace std;


class GLDriver;
class InterceptPluginInstance;
class InterceptPluginDLLInstance;
class FunctionTable;
class InterceptLog;
class FunctionData;

//@
//  Summary:
//    This class provides a interface to register GLIntercept plugins and has
//    update methods to notify the plugins of OpenGL/GLIntercept changes. 
//    (ie. function call/ frame end/ OpenGL error)
//  
class InterceptPluginManager 
{
public:

  //@
  //  Summary:
  //    Constructor 
  //  
  //  Parameters:
  //    driver  - The driver used to make queries on the OpenGL state.
  //
	InterceptPluginManager(GLDriver *driver, FunctionTable *functionTable);
  virtual ~InterceptPluginManager();

  //@
  //  Summary:
  //    To load the plugins from config data.
  //  
  //  Parameters:
  //    configData  - The config data to load the plugins from.
  //
  //  Returns:
  //    True is returned if a least one plugin was successfully initialized.
  //    Else false is returned.
  //
  bool LoadPlugins(const ConfigData &configData);

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
  //    To log the end of frame call.
  //    (Before the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  void LogFrameEndPre(const FunctionData *funcData,uint index, const FunctionArgs & args);

  //@
  //  Summary:
  //    To perform any post-call handeling of the end of frame.
  //    (After the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value (if any).
  //
  void LogFrameEndPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    To log when a OpenGL error occurs.
  //  
  //  Parameters:
  //    funcName - The name of the function that caused the error.
  //
  //    funcIndex - The index in the function table of the function 
  //                that caused the error.
  //
  void OnGLError(const char *funcName, uint funcIndex);

  //@
  //  Summary:
  //    To log when a new OpenGL context is created.
  //  
  //  Parameters:
  //    rcHandle - The render context handle of the new context.
  //
  void OnGLContextCreate(HGLRC rcHandle);

  //@
  //  Summary:
  //    To log when a OpenGL context is deleted.
  //  
  //  Parameters:
  //    rcHandle - The render context handle that has been deleted.
  //
  void OnGLContextDelete(HGLRC rcHandle);

  //@
  //  Summary:
  //    To log when a new OpenGL context has been set.
  //  
  //  Parameters:
  //    oldRCHandle - The old OpenGL render context (can be NULL).
  //
  //    newRCHandle - The new OpenGL render context (can be NULL).
  //
  void OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle);

  //@
  //  Summary:
  //    To log when OpenGL contexts are now sharing lists
  //    (and images/shaders/VBO's etc).
  //  
  //  Parameters:
  //    srcHandle - The OpenGL render context that contains the lists 
  //                to be shared.
  //
  //    dstHandle - The new OpenGL render context that is now sharing
  //                the list data.
  //
  void OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle);

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
  bool GetGLArgString(uint funcIndex, const FunctionArgs & args, uint strLength, char *retString) const;

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
  bool GetGLReturnString(uint funcIndex, const FunctionRetValue & retVal, uint strLength, char *retString) const;

  //@
  //  Summary:
  //    To flag that the passed plugin instance is to be removed on the
  //    next update. This caching is done to prevent the plugin from being
  //    removed during array iteration.
  //  
  //  Parameters:
  //    plugin - The plugin instance to be removed.
  //
  //  Returns:
  //    Returns true on success, false on failure 
  //    (Failure can occur if the plugin is not found/already flagged for 
  //     deletion). 
  //    
  bool FlagPluginRemoval(InterceptPluginInstance *plugin);

  //@
  //  Summary:
  //    To register the passed plugin to receive updates when the passed 
  //    OpenGL function name is called.
  //  
  //  Parameters:
  //    plugin - The plugin instance to receive updates.
  //
  //    functionName - The function name to receive updates for. If equal to
  //                   "*", the plugin will receive all function updates.
  //
  void RegisterGLFunction(InterceptPluginInstance *plugin,const char *functionName);

  //@
  //  Summary:
  //    To un-register the passed plugin from receiving updates for the passed 
  //    OpenGL function name.
  //  
  //  Parameters:
  //    plugin - The plugin instance to receive updates.
  //
  //    functionName - The function name to un-register for. If equal to
  //                   "*", the plugin will unregister all updates.
  //
  void UnRegisterGLFunction(InterceptPluginInstance *plugin,const char *functionName);

  //@
  //  Summary:
  //    To set a user id to be passed to GLFunctionPre/GLFunctionPost when
  //    the specified function call is made.
  //  
  //  Parameters:
  //    plugin - The plugin instance to receive updates.
  //
  //    functionName - The OpenGL function name to add the id to.
  //                   (function must be previously registered with 
  //                    RegisterGLFunction)
  //
  //    newID - The ID to be passed when the above function name is called.
  //
  //  Returns:
  //    If the function id was set, true is returned. Else false is returned.
  //
  bool SetFunctionID(InterceptPluginInstance *plugin, const char *functionName, uint newID);

  //@
  //  Summary:
  //    If a plugion dll is unloaded unexpectantly, (ie during shutdown). 
  //    This method is called to notify all the plugins of the shutdown.
  //  
  //  Parameters:
  //    dllInstance - The DLL instance that is shutting down.
  //
  void OnDLLUnload(InterceptPluginDLLInstance *dllInstance);

/*  Future methods? 
  inline virtual void SetDataSaving(bool flag);
  inline bool GetDataSaving();
  virtual void SetDataSavingPath(const string &newPath);
  const string & GetDataSavingPath();
  virtual void SaveAllDirtyData() =0;
  virtual void SetAllDataDirty() =0;
*/


protected:

  GLDriver         *driver;                       // The OpenGL driver
  InterceptLog     *functionDataConverter;        // The logger to do string conversions
  FunctionTable    *functionTable;                // The function table used for function lookups
  
  bool              isRenderCall;                 // Flag set when a render function is called

  //Structure of the updateID / function name binding
  struct UpdateID
  {
    inline UpdateID(uint updateID,const string &functionName);
    uint   updateID;                              // The update ID to use 
    string functionName;                          // The function name for the update ID 
  };

  //Helper structure to store plugin instances
  struct PluginMember
  {
    inline PluginMember(InterceptPluginInstance *pluginInstance);
    inline virtual ~PluginMember();

    InterceptPluginInstance *pluginInstance;      // The actual plugin instance

    bool           registerAll;                   // Flag if this plugin is registered with all functions 
    vector<string> registeredFunctions;           // The list of functions this plugin is registered with (if the above is not true)

    vector<UpdateID> updateIDArray;               // Array of registered update id/functions
  };

  //Structure for plugin updates
  struct PluginUpdate
  {
    inline PluginUpdate(InterceptPluginInstance *pluginInstance);

    uint   updateID;                              // The ID to be passed when updating
    InterceptPluginInstance *pluginInstance;      // The actual plugin instance
  };

  typedef vector<PluginUpdate> PluginUpdateArray;
  typedef vector<InterceptPluginInstance *> PluginInstanceArray;


  vector<PluginMember>        pluginArray;        // The array of plugins currently managed
  vector<PluginUpdateArray>   functionUpdates;    // The array of plugin instances to call on a specific function update call

  PluginInstanceArray         deletePluginArray;  // The array of plugins waiting deletion

  //@
  //  Summary:
  //    To add a new plugin to the plugin members array.
  //  
  //  Parameters:
  //    plugin - The plugin to add.
  //
  //  Returns:
  //    Returns true on success, false on failure. 
  //
  bool AddPluginInstance(InterceptPluginInstance *newPlugin);

  //@
  //  Summary:
  //    To remove the plugin to the plugin members array and destroy the plugin.
  //    This will also remove the plugin from any update arrays.
  //  
  //  Parameters:
  //    delPlugin - The plugin to delete and remove.
  //
  //  Returns:
  //    Returns true on success, false on failure. 
  //
  bool RemovePluginInstance(InterceptPluginInstance *delPlugin);

  //@
  //  Summary:
  //    To find the passed plugin in the list of currently active plugins.
  //  
  //  Parameters:
  //    searchPlugin - The plugin to search for.
  //
  //  Returns:
  //    Returns the index of the plugin if found else -1 is returned. 
  //
  int FindPluginIndex(InterceptPluginInstance *searchPlugin);

  //@
  //  Summary:
  //    To add this plugin to the list of plugins to receive updates 
  //    for the specified function index.
  //  
  //  Parameters:
  //    index - The index of the OpenGL function that the plugin 
  //            is to receive updates for.
  //
  //    addPlugin - The plugin instance that is to be registered for
  //                the function updates.
  //
  //  Returns:
  //    Returns false if the index does not exist or the plugin is already
  //    registered. Else true is returned.
  //
  bool AddPluginUpdate(uint index, InterceptPluginInstance *addPlugin);

  //@
  //  Summary:
  //    To remove this plugin from the list of plugins that receive updates 
  //    for the specified function index.
  //  
  //  Parameters:
  //    index - The index of the OpenGL function that the plugin 
  //            is to be removed from.
  //
  //    delPlugin - The plugin instance that is to be removed from
  //                the function updates.
  //
  //  Returns:
  //    Returns false if the index does not exist or the plugin is not
  //    registered. Else true is returned.
  //
  bool RemovePluginUpdate(uint index, InterceptPluginInstance *delPlugin);

  //@
  //  Summary:
  //    To re-sync the function update arrays when new functions are registered
  //    with the function table. This will then re-scan all existing plugins to
  //    see if they should be registered to receive updates for these new
  //    functions.
  //  
  void UpdatePluginFunctionArray();

  //@
  //  Summary:
  //    To set the update ID that a plugin instance will receive when a
  //    function is called.
  //  
  //  Parameters:
  //    funcIndex - The index of the OpenGL function that the update is for.
  //
  //    pluginInstance - The plugin instance instance for the update.
  //
  //    updateFuncID - The update ID that the instance will receive when the
  //                   function is called.
  //
  //  Returns:
  //    True is returned if the update ID was successfully set. False is
  //    returned if not.
  //
  bool SetFunctionUpdateID(uint funcIndex,InterceptPluginInstance *pluginInstance,uint updateFuncID);

};

///////////////////////////////////////////////////////////////////////////////
//
inline InterceptPluginManager::UpdateID::UpdateID(uint newID,const string &fName):
updateID(newID),
functionName(fName)
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline InterceptPluginManager::PluginMember::PluginMember(InterceptPluginInstance *pInstance):
pluginInstance(pInstance),
registerAll(false)
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline InterceptPluginManager::PluginMember::~PluginMember()
{
  //Do not destroy the plugin here
}

///////////////////////////////////////////////////////////////////////////////
//
inline InterceptPluginManager::PluginUpdate::PluginUpdate(InterceptPluginInstance *pInstance):
updateID(0),
pluginInstance(pInstance)
{
}


#endif // __INTERCEPT_PLUGIN_MANAGER_H_
