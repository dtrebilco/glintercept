// TestPlugin.cpp : Defines the entry point for the DLL application.
//

#include "../MainLib/InterceptPluginInterface.h"
#include "LogPlugin.h"

#include <string>
#include <PluginCommon.cpp>

using namespace std;

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
  if(cmpPluginName == "LogPlugin")
  {
    return new LogPlugin(callBacks);
  }

  return NULL;
}

