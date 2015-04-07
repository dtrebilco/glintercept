/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptPluginManager.h"
#include "InterceptPluginInstance.h"
#include "InterceptPluginDLLInstance.h"
#include "GLDriver.h"
#include <string.h>


USING_ERRORLOG



///////////////////////////////////////////////////////////////////////////////
//
InterceptPluginManager::InterceptPluginManager(GLDriver *ogldriver,FunctionTable * fTable):
driver(ogldriver),
functionDataConverter(NULL),
functionTable(fTable),
isRenderCall(false)
{
  //Create the function data log
  functionDataConverter =  new InterceptLog(functionTable);
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptPluginManager::~InterceptPluginManager()
{
  //Destroy all existing plugins
  for(uint i=0;i<pluginArray.size();i++)
  {
    delete pluginArray[i].pluginInstance;
  }
  pluginArray.clear();
  deletePluginArray.clear();
  
  //Destroy the function converter
  if(functionDataConverter)
  {
    delete functionDataConverter;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginManager::LoadPlugins(const ConfigData &configData)
{
  //Create the initial function array
  UpdatePluginFunctionArray();

  //Loop for all plugins
  for(uint i=0; i<configData.pluginDataArray.size(); i++) 
  {
    //Get the dll/plugin data
    string pluginDLLName       = configData.pluginBasePath + configData.pluginDataArray[i].pluginDLLName;
    const string &pluginName   = configData.pluginDataArray[i].pluginName;
    const string &configString = configData.pluginDataArray[i].pluginConfigData;

    //Load the dll
    InterceptPluginDLLInstance * dllInstance = InterceptPluginDLLInstance::CreateDLLInstance(this,pluginDLLName.c_str());
    if(dllInstance)
    {
      //Create a new plugin instance
      InterceptPluginInstance *newPlugin = new InterceptPluginInstance(this, driver, dllInstance);

      //Add the plugin to the array (In case the plugin does things in its constructor)
      AddPluginInstance(newPlugin);

      //Attempt to load
      if(!newPlugin->Init(pluginName.c_str(),configString))
      {
        //Remove and delete the attempted plugin
        RemovePluginInstance(newPlugin);
      }
    }
  }


  //If there are no plugins, return false
  if(pluginArray.size() == 0)
  {
    return false;
  }

  return true;

}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{
  //Destroy all plugins on the "destroy list"
  for(uint i=0; i<deletePluginArray.size(); i++)
  {
    //Remove from the plugin array
    RemovePluginInstance(deletePluginArray[i]);
  }
  deletePluginArray.clear();

  //Update the function call array if necessary
  if(functionUpdates.size() < functionTable->GetNumFunctions())
  {
    //This is a slow method but should only be called during startup
    UpdatePluginFunctionArray();
  }

  //Return now if the function call depth is not 0
  if(driver->GetFunctionCallDepth() != 0)
  {
    return;
  }

  //Get if there is a valid context
  bool isValidContext = (driver->GetCurrentContext() != NULL);

  //Determine if the function is a render call
  isRenderCall = false;
  if(isValidContext &&
     driver->GetCurrentContext()->IsRenderFuncion(funcData,index,args))
  {
    isRenderCall = true;
  }


  //Get the update array and perform the required updates
  if(index < functionUpdates.size())
  {
    PluginUpdateArray &notifyArray = functionUpdates[index];

    //Loop for all registered plugins of this function 
    for(uint i=0; i<notifyArray.size(); i++)
    {
      uint updateID                           = notifyArray[i].updateID;
      InterceptPluginInstance *pluginInstance = notifyArray[i].pluginInstance;

      //Check if calls can be made outside the context
      if(isValidContext || !pluginInstance->GetContextFunctionCalls())
      {
        pluginInstance->GetInterface()->GLFunctionPre(updateID,funcData->GetName().c_str(),index,args);
      }
    }
  }

  //If it is a render function, notify all plugins
  if(isRenderCall)
  {
    //Loop for all registered plugins
    for(uint i=0; i<pluginArray.size(); i++)
    {
      pluginArray[i].pluginInstance->GetInterface()->GLRenderPre(funcData->GetName().c_str(),index,args);
    }
  }


}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{
  //Return now if the function call depth is not 0
  if(driver->GetFunctionCallDepth() != 0)
  {
    return;
  }

  //Get if there is a valid context (may have changed since "pre" call)
  bool isValidContext = (driver->GetCurrentContext() != NULL);

  //If it is a render function, notify all plugins
  //  (This is done first, as in LogFunctionPre, it is done last.) 
  if(isRenderCall)
  {
    //Loop for all registered plugins (in reverse order -see below)
    for(int i=(int)pluginArray.size()-1; i>=0; i--)
    {
      pluginArray[i].pluginInstance->GetInterface()->GLRenderPost(funcData->GetName().c_str(),index,retVal);
    }
  }

  //Get the update array for this function 
  if(index < functionUpdates.size())
  {
    PluginUpdateArray &notifyArray = functionUpdates[index];

    //Loop for all registered plugins 
    // (Notifications are done in reverse order so any OpenGL stage 
    //  changes performed in the plugins can be reset corretly)
    for(int i=(int)notifyArray.size()-1; i>=0; i--)
    {
      uint updateID                           = notifyArray[i].updateID;
      InterceptPluginInstance *pluginInstance = notifyArray[i].pluginInstance;

      //Check if calls can be made outside the context
      if(isValidContext || !pluginInstance->GetContextFunctionCalls())
      {
        pluginInstance->GetInterface()->GLFunctionPost(updateID,funcData->GetName().c_str(),index,retVal);
      }
    }
  }


}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::LogFrameEndPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{
  //Loop for all registered plugins
  for(uint i=0; i<pluginArray.size(); i++)
  {
    pluginArray[i].pluginInstance->GetInterface()->GLFrameEndPre(funcData->GetName().c_str(),index,args);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::LogFrameEndPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{
  //Loop for all registered plugins
  // (Notifications are done in reverse order so any OpenGL stage 
  //  changes performed in the plugins can be reset corretly)
  for(int i=(int)pluginArray.size()-1; i>=0; i--)
  {
    pluginArray[i].pluginInstance->GetInterface()->GLFrameEndPost(funcData->GetName().c_str(),index,retVal);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::OnGLError(const char *funcName, uint funcIndex)
{
  //Loop for all registered plugins
  for(uint i=0; i<pluginArray.size(); i++)
  {
    pluginArray[i].pluginInstance->GetInterface()->OnGLError(funcName,funcIndex);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::OnGLContextCreate(HGLRC rcHandle)
{
  //Loop for all registered plugins
  for(uint i=0; i<pluginArray.size(); i++)
  {
    pluginArray[i].pluginInstance->GetInterface()->OnGLContextCreate(rcHandle);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::OnGLContextDelete(HGLRC rcHandle)
{
  //Loop for all registered plugins
  for(uint i=0; i<pluginArray.size(); i++)
  {
    pluginArray[i].pluginInstance->GetInterface()->OnGLContextDelete(rcHandle);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle)
{
  //Loop for all registered plugins
  for(uint i=0; i<pluginArray.size(); i++)
  {
    pluginArray[i].pluginInstance->GetInterface()->OnGLContextSet(oldRCHandle,newRCHandle);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle)
{
  //Loop for all registered plugins
  for(uint i=0; i<pluginArray.size(); i++)
  {
    pluginArray[i].pluginInstance->GetInterface()->OnGLContextShareLists(srcHandle,dstHandle);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginManager::GetGLArgString(uint funcIndex, const FunctionArgs & args, uint strLength, char *retString) const
{
  if(strLength == 0)
  {
    LOGERR(("GetGLArgString - Zero string length"));
    return false;
  }

  //Look up the function
  const FunctionData *funcData = functionTable->GetFunctionData(funcIndex);
  if(!funcData)
  {
    //If the function does not exist, return an empty string
    retString[0] = '\0';
    return false;
  }

  //Get the string
  string funcString;
  functionDataConverter->GetFunctionString(funcData,funcIndex,args,funcString);

  //If the string length is greater, crop
  uint numCopyChars = (uint)funcString.length();
  if(numCopyChars >= strLength)
  {
    //Save a space for the NULL character
    numCopyChars = strLength - 1;
  }

  //Assign the string
  strncpy(retString,funcString.c_str(),numCopyChars);
  retString[numCopyChars] = '\0';

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginManager::GetGLReturnString(uint funcIndex, const FunctionRetValue & retVal, uint strLength, char *retString) const
{
  if(strLength == 0)
  {
    LOGERR(("GetGLReturnString - Zero string length"));
    return false;
  }

  //Look up the function
  const FunctionData *funcData = functionTable->GetFunctionData(funcIndex);
  if(!funcData)
  {
    //If the function does not exist, return an empty string
    retString[0] = '\0';
    return false;
  }

  //Get the string
  string convertString;
  functionDataConverter->GetReturnString(funcData,funcIndex,retVal,convertString);

  //If the string length is greater, crop
  uint numCopyChars = (uint)convertString.length();
  if(numCopyChars >= strLength)
  {
    //Save a space for the NULL character
    numCopyChars = strLength - 1;
  }

  //Assign the string
  strncpy(retString,convertString.c_str(),numCopyChars);
  retString[numCopyChars] = '\0';

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
int InterceptPluginManager::FindPluginIndex(InterceptPluginInstance *searchPlugin)
{
  //Search all elements for the plugin
  for(uint i=0;i<pluginArray.size();i++)
  {
    if(pluginArray[i].pluginInstance == searchPlugin)
    {
      return i;
    }
  }  

  return -1;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginManager::AddPluginInstance(InterceptPluginInstance *newPlugin)
{
  //Check for NULL
  if(newPlugin == NULL)
  {
    LOGERR(("AddPluginInstance - Cannot add a NULL plugin"));
    return false;
  }

  //Check that the instance does not already exist
  if(FindPluginIndex(newPlugin) >= 0)
  {
    LOGERR(("AddPluginInstance - Plugin already exists in the manager array"));
    return false;
  }

  //Add to the array
  pluginArray.push_back(PluginMember(newPlugin));

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginManager::RemovePluginInstance(InterceptPluginInstance *delPlugin)
{
  //Check that the instance does not already exist
  int pluginIndex = FindPluginIndex(delPlugin);
  if(pluginIndex < 0)
  {
    LOGERR(("RemovePluginInstance - Plugin does not exist in plugin array"));
    return false;
  }

  //Erase the plugin member
  pluginArray.erase(pluginArray.begin() + pluginIndex);

  //Remove from all the plugin update arrays
  for(uint i=0;i<functionUpdates.size();i++)
  {
    RemovePluginUpdate(i,delPlugin);
  }

  //Delete the plugin
  delete delPlugin;

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginManager::FlagPluginRemoval(InterceptPluginInstance *flagPlugin)
{
  //Check if the plugin exists
  int foundIndex = FindPluginIndex(flagPlugin);
  if(foundIndex < 0)
  {
    LOGERR(("FlagPluginRemoval - Unknown plugin")); 
    return false;
  }

  //Check if already on the removal list
  for(uint i=0;i<deletePluginArray.size();i++)
  {
    //If the plugin is already being deleted, return false now
    if(deletePluginArray[i] == flagPlugin)
    {
      return false;       
    }
  }

  //Add to the plugin removal list
  deletePluginArray.push_back(flagPlugin);

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::RegisterGLFunction(InterceptPluginInstance *plugin,const char *functionName)
{
  //Ensure the plugin is registered with this manager?
  int foundIndex = FindPluginIndex(plugin);
  if(foundIndex < 0)
  {
    LOGERR(("RegisterGLFunction - Unknown plugin")); 
    return;
  }

  //Get the plugin member data
  PluginMember &pluginMember = pluginArray[foundIndex];

  //If we have registered as "all" return now
  if(pluginMember.registerAll)
  {
    return;
  }

  //If '*' (all) functions
  if(functionName[0] == '*' && functionName[1] == '\0')
  {
    //Unregister any existing single functions
    pluginMember.registeredFunctions.clear();

    //Set the 'all' flag and return
    pluginMember.registerAll = true;

    //Loop and register with all functions
    for(uint i=0;i<functionUpdates.size();i++)
    {
      AddPluginUpdate(i,plugin);
    }

    return;
  }

  //Check if this function is currently registered
  for(uint i=0;i<pluginMember.registeredFunctions.size();i++)
  {
    //If already registered, return
    if(pluginMember.registeredFunctions[i] == string(functionName))
    {
      return;
    }
  }

  //Add to the registered list
  pluginMember.registeredFunctions.push_back(functionName);

  //Get the function index
  int functionIndex = functionTable->FindFunction(functionName);

  //Register this plugin if the function is currently known
  if(functionIndex >= 0)
  {
    AddPluginUpdate(functionIndex,plugin);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::UnRegisterGLFunction(InterceptPluginInstance *plugin,const char *functionName)
{
  //Ensure the plugin is registered with this manager
  int foundIndex = FindPluginIndex(plugin);
  if(foundIndex < 0)
  {
    LOGERR(("UnRegisterGLFunction - Unknown plugin")); 
    return;
  }

  //Get the plugin member data
  PluginMember &pluginMember = pluginArray[foundIndex];

  //If '*' (all) functions
  if(functionName[0] == '*' && functionName[1] == '\0')
  {
    //Remove from the plugin update arrays
    for(uint i=0;i<functionUpdates.size();i++)
    {
      RemovePluginUpdate(i,plugin);
    }

    //Unregister any existing single functions
    pluginMember.updateIDArray.clear();
    pluginMember.registeredFunctions.clear();
    pluginMember.registerAll = false;

    return;
  }

  //If we have registered as "all" return now
  if(pluginMember.registerAll)
  {
    LOGERR(("UnRegisterGLFunction - Cannot un-register a single function once a plugin has had 'all' functions registered"));
    return;
  }

  //Remove it from the update list
  int functionIndex = functionTable->FindFunction(functionName);
  if(functionIndex >= 0)
  {
    RemovePluginUpdate(functionIndex,plugin);
  }

  //Delete any update ids
  uint i;
  for(i=0;i<pluginMember.updateIDArray.size();i++)
  {
    //If found, delete
    if(pluginMember.updateIDArray[i].functionName == string(functionName))
    {
      pluginMember.updateIDArray.erase(pluginMember.updateIDArray.begin() + i);
      break;
    }
  }

  //Delete the name of the function to un-register
  for(i=0;i<pluginMember.registeredFunctions.size();i++)
  {
    //If found, un-register it
    if(pluginMember.registeredFunctions[i] == string(functionName))
    {
      pluginMember.registeredFunctions.erase(pluginMember.registeredFunctions.begin() + i);
      return;
    }
  }

  //If the function name was not found, log an error
  LOGERR(("UnRegisterGLFunction - Function %s was not registered with this plugin",functionName));

}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginManager::SetFunctionID(InterceptPluginInstance *plugin, const char *functionName, uint newID)
{
  //Ensure the plugin is registered with this manager
  int foundIndex = FindPluginIndex(plugin);
  if(foundIndex < 0)
  {
    LOGERR(("SetFunctionID - Unknown plugin")); 
    return false;
  }

  //Get the plugin member data
  PluginMember &pluginMember = pluginArray[foundIndex];

  //Serach for the function name.
  bool isFunctionRegistered = pluginMember.registerAll;
  for(uint i=0; i<pluginMember.registeredFunctions.size(); i++)
  {
    if(pluginMember.registeredFunctions[i] == functionName)
    {
      isFunctionRegistered = true;
      break;
    }
  }

  if(!isFunctionRegistered)
  {
    LOGERR(("SetFunctionID - Function %s is not currently registered",functionName)); 
    return false;
  }


  //If the new id is zero, remove from the list
  if(newID == 0)
  {
    //Loop for all ids
    for(uint i=0; i<pluginMember.updateIDArray.size(); i++)
    {
      //Erase the id
      if(pluginMember.updateIDArray[i].functionName == functionName)
      {
        pluginMember.updateIDArray.erase(pluginMember.updateIDArray.begin() + i);
        break;
      }
    }
  }
  else
  {
    //Loop for all ids
    bool existingUpdate = false;
    for(uint i=0; i<pluginMember.updateIDArray.size(); i++)
    {
      //If matching, update the id
      if(pluginMember.updateIDArray[i].functionName == functionName)
      {
        pluginMember.updateIDArray[i].updateID = newID;
        existingUpdate = true;
        break;
      }
    }

    //If the function name did not already exist, add it
    if(!existingUpdate)
    {
      pluginMember.updateIDArray.push_back(UpdateID(newID,functionName));
    }
  }

  //Set the plugin updateID
  int funcIndex = functionTable->FindFunction(functionName);
  if(funcIndex >= 0)
  {
    SetFunctionUpdateID(funcIndex, plugin, newID);
  }

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginManager::AddPluginUpdate(uint index, InterceptPluginInstance *addPlugin)
{
  //Check the index
  if(index >= functionUpdates.size())
  {
    return false;
  }

  //Get the array to add to
  PluginUpdateArray &addArray = functionUpdates[index];

  //Loop and check for an existing value
  for(uint i=0;i<addArray.size();i++)
  {
    if(addArray[i].pluginInstance == addPlugin)
    {
      return false;
    }
  }

  //Add the plugin
  addArray.push_back(PluginUpdate(addPlugin));

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginManager::RemovePluginUpdate(uint index, InterceptPluginInstance *delPlugin)
{
  //Check the index
  if(index >= functionUpdates.size())
  {
    return false;
  }

  //Get the array to delete from
  PluginUpdateArray &delArray = functionUpdates[index];

  //Loop and check for the value to erase
  for(uint i=0;i<delArray.size();i++)
  {
    if(delArray[i].pluginInstance == delPlugin)
    {
      delArray.erase(delArray.begin() + i);
      return true;
    }
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::UpdatePluginFunctionArray()
{
  uint oldFunctionArraySize = (uint)functionUpdates.size();

  //Loop for all the functions to add
  for(uint i=(uint)functionUpdates.size(); i<functionTable->GetNumFunctions(); i++)
  {
    //Create a new plugin update array
    PluginUpdateArray newArray;

    //Get the function name at the index
    const FunctionData * funcData = functionTable->GetFunctionData(i);
    if(!funcData)
    {
      LOGERR(("UpdatePluginFunctionArray - Invalid function index?")) ;
      return;
    }
    const string &functionName = funcData->GetName();

    //Loop for all plugins
    for(uint pIndex = 0; pIndex<pluginArray.size(); pIndex++)
    {
      //If this plugin receives all updates, just add
      if(pluginArray[pIndex].registerAll)
      {
        newArray.push_back(PluginUpdate(pluginArray[pIndex].pluginInstance));
      }
      else
      {
        //Loop for all specific function updates
        for(uint fIndex=0; fIndex<pluginArray[pIndex].registeredFunctions.size();fIndex++)
        {
          if(pluginArray[pIndex].registeredFunctions[fIndex] == functionName)
          {
            newArray.push_back(PluginUpdate(pluginArray[pIndex].pluginInstance));
            break;
          }
        }
      }
    }

    //Add the array to the function update listing
    functionUpdates.push_back(newArray); 
  }


  //Reset all the plugin update ids
  // This is a brute force setting of all function update IDs. 
  // (May be slow if there are lot of registered ids)

  //Loop for all plugins
  for(uint pIndex = 0; pIndex<pluginArray.size(); pIndex++)
  {
    //Loop for all registered names in the plugin
    for(uint uIndex = 0; uIndex<pluginArray[pIndex].updateIDArray.size(); uIndex++)
    {
      //Reset the update id
      const string & updateFuncName = pluginArray[pIndex].updateIDArray[uIndex].functionName;
      uint           updateFuncID   = pluginArray[pIndex].updateIDArray[uIndex].updateID;

      //Only set if it is one of the new functions
      int funcIndex = functionTable->FindFunction(updateFuncName);
      if(funcIndex >= 0 && funcIndex >= (int)oldFunctionArraySize) 
      {
        SetFunctionUpdateID(funcIndex, pluginArray[pIndex].pluginInstance, updateFuncID);
      }
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginManager::OnDLLUnload(InterceptPluginDLLInstance *dllInstance)
{
  //Notify all plugins
  for(uint i=0;i<pluginArray.size();i++)
  {
    pluginArray[i].pluginInstance->OnDLLUnload(dllInstance);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginManager::SetFunctionUpdateID(uint funcIndex,InterceptPluginInstance *updateInstance,uint updateFuncID)
{
  //Check the index
  if(funcIndex >= functionUpdates.size())
  {
    LOGERR(("InterceptPluginManager::SetFunctionUpdateID - Invalid function index"));
    return false;
  }

  //Get the array to update the ID for
  PluginUpdateArray &updateArray = functionUpdates[funcIndex];

  //Loop and check for the value to set
  for(uint i=0;i<updateArray.size();i++)
  {
    //Update the function update id
    if(updateArray[i].pluginInstance == updateInstance)
    {
      updateArray[i].updateID = updateFuncID;
      return true;
    }
  }

  LOGERR(("InterceptPluginManager::SetFunctionUpdateID - Plugin instance not found?"));

  return false;
}



