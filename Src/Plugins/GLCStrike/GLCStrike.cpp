#include "OpenGLCStrike.h"

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

  //Test for the c-strike plugin
  if(cmpPluginName == "OpenGLCStrike")
  {
    return new OpenGLCStrike(callBacks);
  }

  return NULL;
}

