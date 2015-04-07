#ifndef __LOG_PLUGIN_H_
#define __LOG_PLUGIN_H_

#include "..\\MainLib\\InterceptPluginInterface.h"


#include <vector>

using namespace std;

class ConfigParser;


//@
//  Summary:
//    This class provides a "ping" plugin inteface that will attempt to 
//    force a refresh of the OpenGL windows if the framerate is too low.
//  
class PingPlugin : public InterceptPluginInterface
{
public:
	PingPlugin(InterceptPluginCallbacks *callBacks);
	virtual ~PingPlugin();

  //@
  //  Summary:
  //    Called when an OpenGL function that has been registered 
  //    (via RegisterGLFunction) is about to be made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  inline virtual void GLIAPI GLFunctionPre (uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args );

  //@
  //  Summary:
  //    Called when an OpenGL function that has been registered 
  //    (via RegisterGLFunction) has been made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return vlaue of the function (if any).
  //
  inline virtual void GLIAPI GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    Called when the OpenGL "frame end" call is about to be made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  inline virtual void GLIAPI GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args );

  //@
  //  Summary:
  //    Called after OpenGL "frame end" call has been made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void GLIAPI GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    Called when a OpenGL "render" call is about to be made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  inline virtual void GLIAPI GLRenderPre (const char *funcName, uint funcIndex, const FunctionArgs & args );

  //@
  //  Summary:
  //    Called when a OpenGL "render" call has been made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value of the function. (If any).
  //
  inline virtual void GLIAPI GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    Called when a OpenGL error occurs.
  //  
  //  Parameters:
  //    funcData - The data of the function that cause the error.
  //
  //    index    - A index to the function (In the function table).
  //
  inline virtual void GLIAPI OnGLError(const char *funcName, uint funcIndex);

  //@
  //  Summary:
  //    Called when a OpenGL context is created.
  //  
  //  Parameters:
  //    rcHandle - The new OpenGL context.
  //
  inline virtual void GLIAPI OnGLContextCreate(HGLRC rcHandle);

  //@
  //  Summary:
  //    Called when a OpenGL context is deleted.
  //  
  //  Parameters:
  //    rcHandle - The OpenGL context that is deleted.
  //
  inline virtual void GLIAPI OnGLContextDelete(HGLRC rcHandle);

  //@
  //  Summary:
  //    Called when a OpenGL context is assigned (set).
  //  
  //  Parameters:
  //    oldRCHandle - The old (previous) OpenGL context.
  //
  //    newRCHandle - The new OpenGL context.
  //
  inline virtual void GLIAPI OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle);

  //@
  //  Summary:
  //    Called when a OpenGL context share lists.
  //  
  //  Parameters:
  //    srcHandle - The context constaining the lists.
  //
  //    dstHandle - The context to now share the lists.
  //
  inline virtual void GLIAPI OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle);


  //@
  //  Summary:
  //    Called when this plugin is to be destroyed. The plugin should delete 
  //    itself. (Note: If a plugin needs to shutdown, always request deletion
  //    via InterceptPluginCallbacks::DestroyPlugin which will call this method
  //    on the next update. Do not destroy the plugin by other means.)
  //  
  virtual void GLIAPI Destroy();

  //@
  //  Summary:
  //    To ping the OpenGL windows if the counter has not changed since the 
  //    last call. Returns true if pinging should continue, false if otherwise.
  //
  bool PingGLWindows(uint &counterTest);


protected:

  InterceptPluginCallbacks *gliCallBacks;         // Callbacks to GLI

  const GLCoreDriver *     GL;                    // The GL driver 
  const WGLDriver *        GLW;                   // The WGL driver

  vector<HWND> updateWindows;                     // Array of windows to update

  bool  pluginShutdown;                           // Flag indicating if this plugin is being shutdown
  uint  frameCounter;                             // The frame counter

  HANDLE threadHandle;                            // Handle to the ping thread
  HANDLE mutexHandle;                             // Handle to the mutex 

  //@
  //  Summary:
  //    To process the configuration data.
  //
  void ProcessConfigData(ConfigParser *fileParser);

};


///////////////////////////////////////////////////////////////////////////////
//
inline void PingPlugin::GLFunctionPre (uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args )
{

}


///////////////////////////////////////////////////////////////////////////////
//
inline void PingPlugin::GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
 
}

///////////////////////////////////////////////////////////////////////////////
//
inline void PingPlugin::GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args )
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline void PingPlugin::GLRenderPre (const char *funcName, uint funcIndex, const FunctionArgs & args)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void PingPlugin::GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline void PingPlugin::OnGLError(const char *funcName, uint funcIndex)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void PingPlugin::OnGLContextCreate(HGLRC rcHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void PingPlugin::OnGLContextDelete(HGLRC rcHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void PingPlugin::OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void PingPlugin::OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle)
{

}


#endif // __LOG_PLUGIN_H_