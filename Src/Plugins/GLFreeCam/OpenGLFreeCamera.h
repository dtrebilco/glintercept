#ifndef __OPENGL_FREE_CAMERA_H_
#define __OPENGL_FREE_CAMERA_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include <string>
#include <vector>
#include <MiscUtils.h>
#include "FCConfigData.h"
#include "FCamInput.h"
#include "FCViewCam.h"

#include "Vector.h"

using namespace std;

class ConfigParser;

//@
//  Summary:
//    This class provides a "free camera" plugin inteface by altering the
//    model view matrix before rendering. Viewing the frustum and other
//    options are also provided.
//  
class OpenGLFreeCamera : public InterceptPluginInterface
{
public:

  //@
  //  Summary:
  //    Constructor. Sets up call back data.
  //  
  //  Parameters:
  //    callBacks - The call back interface from GLIntercept.
  //
  OpenGLFreeCamera(InterceptPluginCallbacks *callBacks);

  //@
  //  Summary:
  //    Destructor. Only to be called from Destroy().
  //  
  virtual ~OpenGLFreeCamera();

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
  inline virtual void GLIAPI OnGLContextCreate(HGLRC rcHandle);

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

  InterceptPluginCallbacks *gliCallBacks;         //The callback interface into GLIntercept
  const GLCoreDriver       *GLV;                  //The core OpenGL driver
  FCConfigData              configData;           //Configuration data
  FCamInput                 inputSys;             //The input system
  FCViewCam                 viewCamera;           //Visual rendering of the view frustum  

  bool         initFlag;                          //Flag indicating if initialization was successful.
  bool         freeCameraMode;                    //Flag indicating if free camera mode is enabled.
  bool         disableCullMode;                   //Flag indicating if disable cull mode is enabled.
  bool         enableWireFrameMode;               //Flag indicating if enable wireframe mode is enabled.
  bool         renderViewMode;                    //Flag indicating if render view frustum mode is enabled.
  HGLRC        mainContextID;                     //If main context only processing is enabled, this is the main context

  TimeDiff     keyTimer;                          //Timer between keypresses

  bool         enableKeyState;                    //Flag indicating if the enable camera keys are down
  bool         resetPositionKeyState;             //Flag indicating if the reset position keys are down 
  bool         resetOrientationKeyState;          //Flag indicating if the reset orientation keys are down 
  bool         disableCullKeyState;               //Flag indicating if the disable cull keys are down 
  bool         enableWireFrameKeyState;           //Flag indicating if the enable wireframe keys are down
  bool         renderViewKeyState;                //Flag indicating if the render view keys are down 

  bool         cameraKeyState[CKC_Max];           //Array of camera key states

  //@
  //  Summary:
  //    To assign the internal modelview matrix. If reset is false,
  //    the current model view is multiplied by the transpose matrix.
  //    Else the model view is reset to the cached matrix value.
  //
  //  Parameters:
  //    reset - Flag indicating if we are resetting or assigning the matrix.
  //
  void AssignModelViewMatrix(bool reset);

  //@
  //  Summary:
  //    To assign the OpenGL cull face mode. If face culling has been disabled,
  //    This will disable the OpenGL state if it is enabled. Passing the reset
  //    flag will reset to the origional state.
  //
  //  Parameters:
  //    reset - Flag indicating if we are resetting or disabling the cull state.
  //
  void AssignCullFaceMode(bool reset);

  //@
  //  Summary:
  //    To assign the OpenGL wireframe mode. If wireframe mode has been enabled,
  //    this will enable the OpenGL state. Passing the reset
  //    flag will reset to the origional state.
  //
  //  Parameters:
  //    reset - Flag indicating if we are resetting or enabling wireframe mode.
  //
  void AssignWireFrameMode(bool reset);

  //@
  //  Summary:
  //    To assign the new free camera mode.
  //
  //  Parameters:
  //    newMode - The new mode for the free camera.
  //
  void SetFreeCameraMode(bool newMode);

  //@
  //  Summary:
  //    To process the camera keys and update the camera matrix.
  //
  //  Parameters:
  //    elapsedTime - The elapsed time since the last update.
  //
  void ProcessCameraKeys(float elapsedTime);   

  //@
  //  Summary:
  //    To retrieve OpenGL light state data and store it internally.
  //    This will also re-apply the light data for the current model view matrix.
  //
  void RetrieveLightStateData();

  //@
  //  Summary:
  //    To restore the OpenGL light state data.
  //    This will reset the current matrix to the identity if lights have to be resset.
  //    The current matrix mode must be model view before calling
  //
  void ResetLightStateData();

private:
  
  //Structure to store data about OpenGL fixed function lights
  struct LightStateData
  {
    uint lightNum;   // The OpenGL light number  
    vec4 lightPos;   // The origional position of the light in eye space
    vec3 lightDir;   // The origional direction of the light in eye space
  };


  bool      modelViewAltered;                     // Flag to indicate if the OpenGL model view matrix has been altered
  mat4      oldModelView;                         // The previous OpenGL modelview matrix

  bool      isTransformMatrixIdentity;            // Flag indicating if the transform matrix is the identity matrix
  mat4      transformMatrix;                      // The matrix to multiply the OpenGL modelview matrix by

  vec4      positionOffset;                       // The current position offset of the camera
  mat4      rotateMatrix;                         // The current rotation of the camera  


  vector<LightStateData>  oldLightStates;         // Array of old OpenGL light states

  GLint     oldScissorArea[4];                    // Old scissor area before the render (if scissor changing is enabled)
  bool      oldFaceCullState;                     // Old face cull state if face culling is disabled
  GLint     oldPolyFillModes[2];                  // Old polygon fill mode values

};


//Unused entry points

///////////////////////////////////////////////////////////////////////////////
//
inline void OpenGLFreeCamera::GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args )
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline void OpenGLFreeCamera::OnGLError(const char *funcName, uint funcIndex)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void OpenGLFreeCamera::OnGLContextCreate(HGLRC rcHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void OpenGLFreeCamera::OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle)
{

}



#endif // __OPENGL_FREE_CAMERA_H_
