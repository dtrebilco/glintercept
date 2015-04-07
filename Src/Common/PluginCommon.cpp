
#include <string>

using namespace std;

//The dll event handler (if any)
DLLEventHandler * dllEventHandler = NULL;

//The error logger
LOGERRPROC errorLog = NULL;

//The path to the DLL (including trailing slash)
string dllPath;

#ifdef GLI_BUILD_WINDOWS

//Exported functions
extern "C" {
__declspec(dllexport) uint GLIAPI GetFunctionLogPluginVersion();
__declspec(dllexport) InterceptPluginInterface * GLIAPI CreateFunctionLogPlugin(const char *pluginName, InterceptPluginCallbacks * callBacks);
__declspec(dllexport) bool GLIAPI RegisterDLLEventHandler(DLLEventHandler *eventHandler);
__declspec(dllexport) bool GLIAPI UnRegisterDLLEventHandler(DLLEventHandler *eventHandler);
}

//The Instance that was passed on DllMain
HINSTANCE dllInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
//
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
  //Assign the instance
  dllInstance = (HINSTANCE)hModule;

  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    { 
      //Get the module's file name
      static char dllName[1024];
      GetModuleFileName((HMODULE)hModule,dllName,1023);
 
      //Get the path
      dllPath = dllName;
      int pathEnd = (int)dllPath.rfind('\\');
      if(pathEnd != string::npos)
      {
        //Remove the file name part
        dllPath.erase(pathEnd+1,dllPath.size()-pathEnd-1);
      }
      else
      { 
        //Just use the current working directory
        dllPath = "";
      }

      break;
    }

    //NOTE: GLIntercept does not currently support multiple threads
    case DLL_THREAD_ATTACH:
    {
      break;
    }
    case DLL_THREAD_DETACH:
    {
      break;
    }
    case DLL_PROCESS_DETACH:
    {
      //If there is an event handler, flag that the plugin is being deleted
      if(dllEventHandler)
      {
        dllEventHandler->OnDLLDelete();
      }
      break;
    }
  }

  return TRUE;
}

#endif // GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

//Support for getting the .so path
#include <prefix.h>
#include "FileUtils.h"

//Exported functions
extern "C" {
uint GLIAPI GetFunctionLogPluginVersion();
InterceptPluginInterface * GLIAPI CreateFunctionLogPlugin(const char *pluginName, InterceptPluginCallbacks * callBacks);
bool GLIAPI RegisterDLLEventHandler(DLLEventHandler *eventHandler);
bool GLIAPI UnRegisterDLLEventHandler(DLLEventHandler *eventHandler);
}

///////////////////////////////////////////////////////////////////////////////
//
void init(void) __attribute__ ((constructor));
void init(void)
{
  // Attempt to get the .so path
  const char * libPathTest = SELFPATH;
  if(libPathTest != NULL)
  {
    dllPath = libPathTest;

    //Strip the .so name    
    string::size_type pathEnd = dllPath.rfind(FileUtils::dirSeparator);
    if(pathEnd != string::npos)
    {
      //Remove the file name part
      dllPath.erase(pathEnd+1,dllPath.size()-pathEnd-1);
    }
    else
    {
      dllPath = "";
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void fini(void) __attribute__ ((destructor));
void fini(void)
{
  //On unload, flag to the plugin
  if(dllEventHandler)
  {
    dllEventHandler->OnDLLDelete();
  }
}

#endif //GLI_BUILD_LINUX

///////////////////////////////////////////////////////////////////////////////
//
uint GLIAPI GetFunctionLogPluginVersion()
{
  return __GLI_INTERCEPT_PLUGIN_VER;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIAPI RegisterDLLEventHandler(DLLEventHandler *eventHandler)
{
  //Check for an existing handler
  if(dllEventHandler)
  {
    return false;
  }

  //Assign the event handler
  dllEventHandler = eventHandler;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIAPI UnRegisterDLLEventHandler(DLLEventHandler *eventHandler)
{
  if(dllEventHandler != eventHandler)
  {
    return false;
  }

  //Unset the event handler
  dllEventHandler = NULL;
  return true;
}

