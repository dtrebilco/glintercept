/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
// GLIntercept.cpp : Defines the entry point for the DLL application.

#include "GLIntercept.h"
#include "GLDriver.h"
#include <string>
#include <FileUtils.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

//The main error log
ErrorLog *gliLog=NULL;

//The main OpenGL driver 
GLDriver glDriver QUICK_CONSTRUCT;

//The path to the dll location (including trailing seperator)
string dllPath QUICK_CONSTRUCT;

#ifdef GLI_BUILD_WINDOWS

#include <direct.h>
#include <crtdbg.h>

//The handle of the DLL
HANDLE dllHandle = NULL;

///////////////////////////////////////////////////////////////////////////////
//
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved)
{

  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    { 
      //Assign the DLL handle for later usage
      dllHandle = hModule;

      //Enable memory debugging
      //_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
      //_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR );
      _CrtSetDbgFlag ( _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF 
                     /*|_CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF*/);

      //Get the module's file name
      static char dllName[1024];
      GetModuleFileName((HMODULE)hModule,dllName,1023);
 
      //Get the path
      dllPath = dllName;
      string::size_type pathEnd = dllPath.rfind(FileUtils::dirSeparator);
      if(pathEnd != string::npos)
      {
        //Remove the file name part
        dllPath.erase(pathEnd+1,dllPath.size()-pathEnd-1);
      }
      else
      {
        //If we could not even find a back slash, just set the path as the current directory
        if(_getcwd( dllName, 1024 ) != NULL)
        {
          //Assign the path and a trailing seperator
          dllPath = dllName;
          dllPath = dllPath + FileUtils::dirSeparator;
        }
        else
        {
          dllPath = "";
        }
      }

      //Create the log
      gliLog = new ErrorLog((dllPath + "gliLog.txt").c_str());

      //Get the current time
      time_t aclock;
      time( &aclock );

      //Convert it
      struct tm *newtime = localtime( &aclock );

      //Set the error log to always flush
      gliLog->SetLogFlush(true);
      gliLog->LogError("GL Intercept Log. Version : %s    Compile Date: %s    Run on: %s",__GLI_BUILD_VER_STR,__DATE__,asctime(newtime));
      gliLog->LogError("===================================================");
      gliLog->SetDebuggerLogEnabled(true);

      break;
    }

    //NOTE: GLIntercept does not currently support multiple threads
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
    {
      break;
    }
  }
  return TRUE;
}

#endif //GLI_BUILD_WINDOWS


#ifdef GLI_BUILD_LINUX

//Support for getting the .so path
#include <prefix.h>

///////////////////////////////////////////////////////////////////////////////
//
void init(void) __attribute__ ((constructor(102)));
void init(void)
{
  //Check for the GLIntercept environment variable first
  const char * envPathTest = getenv("GLI_CONFIG_PATH");
  if(envPathTest != NULL)
  {
    dllPath = envPathTest; 
    
    //If the last character is not a directory seperator, add one
    if(dllPath.length() > 0 && 
       dllPath[dllPath.length()-1] != FileUtils::dirSeparator)
    {
      dllPath = dllPath + FileUtils::dirSeparator;   
    }     
  }

  //Assign the path to the .so
  if(dllPath.length() == 0)
  {
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
    
  //Check if a valid path was assigned
  if(dllPath.length() == 0)
  {
    //Get the path from the current working directory
    static char dllName[1024];
    if(getcwd(dllName, 1023) != NULL)
    {
      dllPath = dllName; 
      dllPath = dllPath + FileUtils::dirSeparator;     
    }
    else
    {
      ErrorLog::LogDebuggerMessage("Unable to retrieve startup path");
      dllPath = ""; 
    }
  }
  
  //Create the log
  gliLog = new ErrorLog((dllPath + "gliLog.txt").c_str());
  
  //Get the current time
  time_t aclock;
  time( &aclock );
    
  //Convert it
  struct tm *newtime = localtime( &aclock );
  
  //Set the error log to always flush
  gliLog->SetLogFlush(true);
  gliLog->LogError("GL Intercept Log. Version : %s    Compile Date: %s    Run on: %s",__GLI_BUILD_VER_STR,__DATE__,asctime(newtime));
  gliLog->LogError("===================================================");
  gliLog->SetDebuggerLogEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////
//
void fini(void) __attribute__ ((destructor));
void fini(void)
{
  //Note: The Logger is shutdown in the driver 
  
}


#endif //GLI_BUILD_LINUX




