#include "LogPlugin.h"

#include <ConfigParser.h>
#include <CommonErrorLog.h>


USING_ERRORLOG

///////////////////////////////////////////////////////////////////////////////
//
LogPlugin::LogPlugin(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks)
{
  LOGERR(("LOG Plugin Created %d",6));

  gliCallBacks->RegisterGLFunction("glBegin");
  gliCallBacks->RegisterGLFunction("*");

  
  gliCallBacks->SetContextFunctionCalls(false);
  //gliCallBacks->DestroyPlugin(); 

  ConfigParser parser;
  parser.ParseString(gliCallBacks->GetConfigString());

}

///////////////////////////////////////////////////////////////////////////////
//
LogPlugin::~LogPlugin()
{

}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::GLFunctionPre (uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args )
{
  //LOGERR(("%s",funcName));

  char buffer[1024];
  gliCallBacks->GetGLArgString(funcIndex,args,1024,buffer);
  LOGERR(("%s",buffer));

}


///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  char buffer[1024];
  gliCallBacks->GetGLReturnString(funcIndex,retVal,1024,buffer);
  LOGERR(("%s",buffer));
 
}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args )
{
  //gliCallBacks->DestroyPlugin();

  gliCallBacks->GetCoreGLFunctions()->glClear(GL_COLOR_BUFFER_BIT);
}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //LOGERR(("This should not happen"));
}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::GLRenderPre (const char *funcName, uint funcIndex, const FunctionArgs & args)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::OnGLError(const char *funcName, uint funcIndex)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::OnGLContextCreate(HGLRC rcHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::OnGLContextDelete(HGLRC rcHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void LogPlugin::Destroy()
{
  LOGERR(("LOG Plugin Destroyed"));
  
  //Don't do this:
  //gliCallBacks->DestroyPlugin();

  //Destroy this plugin
  delete this;
}
