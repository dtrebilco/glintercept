#ifndef __SHADER_EDIT_MANAGER_H_
#define __SHADER_EDIT_MANAGER_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include "ShaderDebugGLSL.h"
#include "UniqueShaderID.h"

#include <string>
#include <vector>
#include <MiscUtils.h>

using namespace std;

class ConfigParser;
class SEContext;
enum  USIDType;

//@
//  Summary:
//    This class provides a plugin implementation for GLIntercept to
//    provide shader editing of OpenGL shaders. 
//  
class ShaderEditManager : public InterceptPluginInterface
{
public:

  //@
  //  Summary:
  //    Constructor. Sets up call back data.
  //  
  //  Parameters:
  //    callBacks - The call back interface from GLIntercept.
  //
  ShaderEditManager(InterceptPluginCallbacks *callBacks);

  //@
  //  Summary:
  //    Destructor. Only to be called from Destroy().
  //  
  virtual ~ShaderEditManager();

  //@
  //  Summary:
  //    Called when an OpenGL function that has been registered 
  //    (via RegisterGLFunction) is about to be made.
  //  
  //  Parameters:
  //    updateID - The ID of the registered function. 
  //
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
  //    updateID - The ID of the registered function. 
  //
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
 virtual void GLIAPI GLRenderPre(const char *funcName, uint funcIndex, const FunctionArgs & args );

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
  virtual void GLIAPI GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

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
  virtual void GLIAPI OnGLContextCreate(HGLRC rcHandle);

  //@
  //  Summary:
  //    Called when a OpenGL context is deleted.
  //  
  //  Parameters:
  //    rcHandle - The OpenGL context that is deleted.
  //
  virtual void GLIAPI OnGLContextDelete(HGLRC rcHandle);

  //@
  //  Summary:
  //    Called when a OpenGL context is assigned (set).
  //  
  //  Parameters:
  //    oldRCHandle - The old (previous) OpenGL context.
  //
  //    newRCHandle - The new OpenGL context.
  //
  virtual void GLIAPI OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle);

  //@
  //  Summary:
  //    Called when a OpenGL context share lists.
  //  
  //  Parameters:
  //    srcHandle - The context constaining the lists.
  //
  //    dstHandle - The context to now share the lists.
  //
  virtual void GLIAPI OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle);


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
  //    To compile the passed shader source and to override the 
  //    passed shader ID.
  //  
  //  Parameters:
  //    shaderID    - The unique ID of the shader to override.
  //
  //    shaderSrc   - The shader source to compile.
  //
  //    retLog      - The returned log of any errors/warnings during compile.
  //
  //  Returns:
  //    If the shader source was compiled successfully, true is returned.
  //    Else false is returned.
  //
  bool CompileShader(uint shaderID, const string &shaderSrc, string &retLog);

  //@
  //  Summary:
  //    To set the "show" shader ID. Can be 0 for no shader ID.
  //  
  //  Parameters:
  //    shaderUID    - The unique ID of the shader to show.
  //
  //    shaderType   - The type of the shader that is shown.
  //
  inline void SetShowShader(uint shaderUID, USIDType shaderType);

  //@
  //  Summary:
  //    To set the "break-on" shader ID. Can be 0 for no shader ID.
  //  
  //  Parameters:
  //    shaderUID    - The unique ID of the shader to break-on.
  //
  //    shaderType   - The type of the shader that is break-on.
  //
  inline void SetBreakShader(uint shaderUID, USIDType shaderType);

  //@
  //  Summary:
  //    To start debugging the specified pixel location
  //  
  //  Parameters:
  //    xPos    - The x position of the pixel to debug.
  //
  //    xPos    - The y position of the pixel to debug.
  //
  //    pixelOverdraw - The overdrawn pixel number to debug. 
  //
  //    retLog        - The returned log of any errors/warnings during compile.
  //
  //  Returns:
  //    If pixel started debug, true is returned. Else false is returned.
  //
  bool SetPixelDebug(uint xPos, uint yPos, uint pixelOverdraw, string &retLog);

  //@
  //  Summary:
  //    To end the debugging shader state. This ends the debugging state set
  //    by StartShaderDebug.
  //  
  void EndShaderDebug();

protected:

  InterceptPluginCallbacks *gliCallBacks;         // The callback interface into GLIntercept
  const GLCoreDriver       *GLV;                  // The core OpenGL driver
  const InputUtils         *inputUtils;           // The input utility class

  SEContext          *currContext;                // The current OpenGL context
  vector<SEContext *> contextArray;               // The array of all the OpenGL contexts

  ShaderDebugGLSL  *shaderDebugGLSL;              // The current shader debuger (if any) 

  uint      showShaderUID;                        // The unique ID of the shader to "show" (0 if no shader)
  USIDType  showShaderType;                       // The type of the shader to show
  uint      showShaderStage;                      // The stage number the shader "show" is up to 
  TimeDiff  showShaderTimer;                      // The timer for altering the "show" shader
  bool      showShaderApplied;                    // Flag indicating if the "show" shader is currently applied

  uint      breakShaderUID;                       // The unique ID of the shader to "break-on" (0 if no shader)
  USIDType  breakShaderType;                      // The type of the shader to break-on

  vector<uint>    enableKeys;                     // The key combination to enable the shader editor

  //@
  //  Summary:
  //    To find the internal context that uses the passed handle.
  //  
  //  Parameters:
  //    handle - The context handle to search for.
  //
  //  Returns:
  //    If the context is found it is returned. Else NULL is returned.
  // 
  SEContext *FindContext(HGLRC handle);

  //@
  //  Summary:
  //    Process the passed config parser to load the configuration data.
  //  
  void ProcessConfigData(const ConfigParser *parser);

  //@
  //  Summary:
  //    Process that ta render has just been completed
  //  
  void ProcessPostRender();

  //@
  //  Summary:
  //    To switch to the debugging shader state. This function will not return
  //    until debugging is completed. (Debugging occurs via re-entrant calls on
  //    this class)
  //  
  //  Parameters:
  //    boundShader - The shader that is bound and is being debugged.
  //
  //    renderCache - The render call used in debugging the shader.
  // 
  //    frameCache  - The frame buffer data pre/post the debug render call.
  //
  //  Returns:
  //    If the debugging state could be created, true is returned. 
  //    Else false is returned.
  // 
  bool StartShaderDebug(UniqueShaderIDRef boundShader, GLRenderCallCache *renderCache, GLFrameBufferCache *frameCache);

private:

  GLint oldLogicOpEnable;                         //Flags used to cache the logic op changes
  GLint oldLogicOpMode;                           //Flags used to cache the logic op changes 

  //@
  //  Summary:
  //    To apply a visualization of the currently bound shader.
  //  
  //  Parameters:
  //    revert - If the visualization is being applied or being reverted.
  //
  //  Returns:
  //    Returns true if the change was applied.
  // 
  bool ApplyShowShader(bool revert = false);

};


//Unused entry points

///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderEditManager::GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args )
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderEditManager::OnGLError(const char *funcName, uint funcIndex)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderEditManager::SetShowShader(uint shaderUID, USIDType shaderType)
{
  //Assign the shader and reset the stage
  showShaderUID   = shaderUID;
  showShaderType  = shaderType;
  showShaderStage = 0;
  showShaderTimer.StartTimer(); 
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderEditManager::SetBreakShader(uint shaderUID, USIDType shaderType)
{
  //Assign the shader and type
  breakShaderUID   = shaderUID;
  breakShaderType  = shaderType;
}


#endif // __SHADER_EDIT_MANAGER_H_
