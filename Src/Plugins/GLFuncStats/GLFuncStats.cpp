#include "../MainLib/InterceptPluginInterface.h"
#include "FuncStatsPlugin.h"

#include <string>

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

  //Test for the function status plugin
  if(cmpPluginName == "FunctionStats")
  {
    return new FuncStatsPlugin(callBacks);
  }

  return NULL;
}
