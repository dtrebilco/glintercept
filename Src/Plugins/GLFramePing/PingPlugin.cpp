
#include "PingPlugin.h"

#include <ConfigParser.h>
#include <CommonErrorLog.h>

USING_ERRORLOG

//The amount of time between fraem updates
uint sleepAmount = 250;

//If the window is "force" updated
bool forceWindowUpdate = false;

//Path to the dll
extern string dllPath;

//Thread function to "Ping" the OpenGL windows
///////////////////////////////////////////////////////////////////////////////
//
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
  //Get the ping plugin class
  PingPlugin *plugin = (PingPlugin*)lpParameter;

  //Counter to keep track of when to ping
  uint pingCounter = 0;

  //Loop until plugin shutdown
  while(plugin->PingGLWindows(pingCounter))
  {
    Sleep(sleepAmount);
  }

  //For some reason this thread can be killed on process shutdown
  // without reaching this return point (still returns 0 however)
  return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
PingPlugin::PingPlugin(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),
GL(gliCallBacks->GetCoreGLFunctions()),
GLW(gliCallBacks->GetWGLFunctions()),
pluginShutdown(false),
frameCounter(0),
threadHandle(NULL),
mutexHandle(NULL)
{
  //LOGERR(("PingPlugin Created"));
  
  //Parse the config file
  ConfigParser fileParser;
  if(fileParser.Parse(dllPath + "config.ini"))
  {
    ProcessConfigData(&fileParser);
    fileParser.LogUnusedTokens(); 
  }

  //Parse the config string
  ConfigParser stringParser;
  if(stringParser.ParseString(gliCallBacks->GetConfigString()))
  {
    ProcessConfigData(&stringParser);
    stringParser.LogUnusedTokens(); 
  }

  //Create the mutex
  mutexHandle = CreateMutex(NULL,  // pointer to security attributes
                            FALSE,  // flag for initial ownership
                            NULL);
  if(!mutexHandle)
  {
    LOGERR(("PingPlugin - Mutex creation failed"));
  }

  //Start the ping thread
  DWORD threadID;
  threadHandle = CreateThread( 
                  NULL,                        // default security attributes 
                  0,                           // use default stack size  
                  ThreadProc,                  // thread function 
                  this,                        // argument to thread function 
                  0,                           // use default creation flags 
                  &threadID);                  // returns the thread identifier 
 
  // Check the return value for success. 
  if(!threadHandle) 
  {
    LOGERR(("PingPlugin - Thread creation failed"));
  }

  //LOGERR(("Thread 0x%x ID 0x%x",threadHandle,threadID));
}

///////////////////////////////////////////////////////////////////////////////
//
PingPlugin::~PingPlugin()
{

}

///////////////////////////////////////////////////////////////////////////////
//
void PingPlugin::ProcessConfigData(ConfigParser *parser)
{
  //Get the view frustum color
  const ConfigToken *updateFPSToken = parser->GetToken("UpdateFPS");
  if(updateFPSToken)
  {
    float updateFPS;
    updateFPSToken->Get(updateFPS);

    //Check if the fraem rate is reasonable
    if(updateFPS > 0.001f)
    {
      sleepAmount = (uint)(1000.0 / updateFPS);
    }
  }

  //Get if the window is force updated
  const ConfigToken *forceWindowToken = parser->GetToken("ForceUpdate");
  if(forceWindowToken)
  {
    forceWindowToken->Get(forceWindowUpdate);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void PingPlugin::GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{

  //Lock 
  if(WaitForSingleObject(mutexHandle, INFINITE) == WAIT_FAILED)
  {
    LOGERR(("PP: Frame end : Wait failed"));
    return;
  }

  //Update the frame counter
  frameCounter++;

  //Get the window device context
  HDC  windowDC  = GLW->wglGetCurrentDC();
  if(windowDC != NULL)
  {
    //Get the current window handle
    HWND wndHandle = WindowFromDC(windowDC);

    //Add to the list
    if(wndHandle != NULL)
    {
      bool windowFound = false;
      for(uint i=0; i<updateWindows.size(); i++)
      {
        //Check for an existing window
        if(updateWindows[i] == wndHandle)
        {
          windowFound = true;
          break;
        }
      }

      //Add to the update array
      if(!windowFound)
      {
        updateWindows.push_back(wndHandle);
      }
    }
  }

  //Unlock
  ReleaseMutex(mutexHandle); 
}


///////////////////////////////////////////////////////////////////////////////
//
bool PingPlugin::PingGLWindows(uint &counterTest)
{
  //Note: Cannot call LOGERR in this method as it is call by another thread
  //Note: This method must be thread safe

  //Test if the plugin is being shutdown
  if(pluginShutdown)
  {
    return false;
  }

  vector<HWND> pingWindows;

  //Lock
  if(WaitForSingleObject(mutexHandle, INFINITE) == WAIT_FAILED)
  {
    return false;
  }

  //Test the counter to see if the frame count has updated since last
  //  test
  if(frameCounter == counterTest)
  {
    //Copy the windows to be pinged (as array may be updated while pinging occurs)
    for(uint i=0; i<updateWindows.size(); i++)
    {
      //Check if the window exists anymore
      if(IsWindow(updateWindows[i]))
      {
        pingWindows.push_back(updateWindows[i]);
      } 
    }

    //Empty the array
    updateWindows.clear();

    //Increment the counter test (as the ping should increament the frame number)
    counterTest++;
  }
  else
  {
    counterTest = frameCounter;
  }

  //Unlock
  ReleaseMutex(mutexHandle); 

  //Ping the windows
  for(uint i=0; i<pingWindows.size(); i++)
  {
    //Invalidate the window - The window will then receive a update message
    // when no other messages are pending
    ::InvalidateRect(pingWindows[i], NULL, FALSE);

    //NOTE: Calling UpdateWindow crashes some apps (not necessary in most cases?)
    if(forceWindowUpdate)
    {
      ::UpdateWindow(pingWindows[i]); 
    }
  }

  return true;
}




///////////////////////////////////////////////////////////////////////////////
//
void PingPlugin::Destroy()
{
  //Set the destroy flag
  pluginShutdown = true;

  //Wait for the thread to exit
  if(threadHandle)
  {
    //Loop while the thread is still active
    DWORD exitCode =0;
    do{
    
     //Attempt to get the exit code
     if(GetExitCodeThread(threadHandle,&exitCode))
     {
       Sleep(10);
     }
     else
     {
       exitCode = 0;
     }

    }while(exitCode == STILL_ACTIVE);

    if(!CloseHandle(threadHandle))
    {
      LOGERR(("Error closing thread handle 0x%x", threadHandle));
    }
  }

  //Close the mutex
  if(!CloseHandle(mutexHandle))
  {
    LOGERR(("Error closing Mutex 0x%x",mutexHandle));
  }

  //Destroy this plugin
  delete this;
}

