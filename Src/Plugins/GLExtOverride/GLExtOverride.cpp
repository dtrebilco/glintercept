// GLExtOverride.cpp : Defines the entry point for the DLL application.
//

#include "ExtensionOverride.h"

//Include the common DLL code
#include <PluginCommon.cpp>

///////////////////////////////////////////////////////////////////////////////
//
InterceptPluginInterface * GLIAPI CreateFunctionLogPlugin(const char *pluginName, InterceptPluginCallbacks * callBacks)
{
  //If no call backs, return NULL
  if(callBacks == NULL)
  {
    return NULL;
  }

  //Assign the error logger
  if(errorLog == NULL)
  {
    errorLog = callBacks->GetLogErrorFunction();
  }

  string cmpPluginName(pluginName);

  //Test for the logger plugin
  if(cmpPluginName == "ExtensionOverride")
  {
    return new ExtensionOverride(callBacks);
  }

  return NULL;
}

