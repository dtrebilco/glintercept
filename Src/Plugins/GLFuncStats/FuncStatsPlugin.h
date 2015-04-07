#ifndef __FUNCTION_STATS_PLUGIN_H_
#define __FUNCTION_STATS_PLUGIN_H_

#include "../MainLib/InterceptPluginInterface.h"

#include <vector>
#include <string>

using namespace std;

class ConfigParser;

//@
//  Summary:
//    This class implements a plugin that displays the total number
//    of OpenGL calls made and the counts for each function.
//  
class FuncStatsPlugin : public InterceptPluginInterface
{
public:
	FuncStatsPlugin(InterceptPluginCallbacks *callBacks);
	virtual ~FuncStatsPlugin();

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
  virtual void GLIAPI GLFunctionPre (uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args );

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

protected:

  InterceptPluginCallbacks *gliCallBacks;         // The callbacks to GLIntercept

  struct FunctionCallData
  {
    FunctionCallData();

    //@
    //  Summary:
    //    Sorting functions to sort and array of elements
    //
    static inline bool SortByName(const FunctionCallData &a, const FunctionCallData &b);
    static inline bool SortByCount(const FunctionCallData &a, const FunctionCallData &b);

    string functionName;                          // The name of the function
    uint   funcCallCount;                         // The call count of the function
  };

  bool                      functionsCountSort;   // If listing the functions by the number of time the function was called
  bool                      functionsNameSort;    // If listing the functions by a name sort 

  int64                     numGLFunctionCalls;   // The number of OpenGL functions called
  int64                     numGLCallsFirstFrame; // The number of OpenGL calls made in the first frame

  vector<FunctionCallData>  functionDataArray;    // Array of function data

  //@
  //  Summary:
  //    To process the configuration data.
  //
  void ProcessConfigData(ConfigParser *parser);

  //@
  //  Summary:
  //    To get the spaces need to align the function
  //
  string GetSpaces(const string &funcName);

};


///////////////////////////////////////////////////////////////////////////////
//
inline bool FuncStatsPlugin::FunctionCallData::SortByName(const FunctionCallData &a, const FunctionCallData &b)
{
  return a.functionName < b.functionName;
}


///////////////////////////////////////////////////////////////////////////////
//
inline bool FuncStatsPlugin::FunctionCallData::SortByCount(const FunctionCallData &a, const FunctionCallData &b)
{
  return a.funcCallCount > b.funcCallCount;
}


///////////////////////////////////////////////////////////////////////////////
//
inline void FuncStatsPlugin::GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{

}


///////////////////////////////////////////////////////////////////////////////
//
inline void FuncStatsPlugin::GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args )
{
  
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FuncStatsPlugin::GLRenderPre (const char *funcName, uint funcIndex, const FunctionArgs & args)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void FuncStatsPlugin::GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void FuncStatsPlugin::OnGLError(const char *funcName, uint funcIndex)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void FuncStatsPlugin::OnGLContextCreate(HGLRC rcHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void FuncStatsPlugin::OnGLContextDelete(HGLRC rcHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void FuncStatsPlugin::OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void FuncStatsPlugin::OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle)
{

}


#endif // __FUNCTION_STATS_PLUGIN_H_
