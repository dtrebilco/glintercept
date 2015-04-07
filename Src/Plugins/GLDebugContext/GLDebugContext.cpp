// TestPlugin.cpp : Defines the entry point for the DLL application.
//

#include "../MainLib/InterceptPluginInterface.h"
#include "DebugContext.h"

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

  //Test for the debug context plugin
  if(cmpPluginName == "DebugContext")
  {
    return new DebugContext(callBacks);
  }

  return NULL;
}

