#include "OpenGLFreeCamera.h"

#include <ConfigParser.h>
#include <CommonErrorLog.h>

//Minimum number of OpenGL lights that have to be supported by implementations
#define MIN_GL_LIGHT_SUPPORT 8

USING_ERRORLOG

//The call back ids
enum CallBackIDs
{
  CBI_None =0,
  CBI_glEnd
};

//Path to the dll
extern string dllPath;


///////////////////////////////////////////////////////////////////////////////
//
OpenGLFreeCamera::OpenGLFreeCamera(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),
GLV(callBacks->GetCoreGLFunctions()),
configData(),
inputSys(callBacks->GetInputUtils()),
viewCamera(callBacks),
initFlag(false),
freeCameraMode(false),
disableCullMode(false),
enableWireFrameMode(false),
renderViewMode(false),
mainContextID(NULL),

enableKeyState(false),
resetPositionKeyState(false),
resetOrientationKeyState(false),
disableCullKeyState(false),
enableWireFrameKeyState(false),
renderViewKeyState(false),

modelViewAltered(false),
isTransformMatrixIdentity(true),
positionOffset(0.0f,0.0f,0.0f,0.0f),
oldFaceCullState(false)
{
  //LOGERR(("Free Cam Plugin Created"));

  //Set initial matrix values
  oldModelView.identity();
  transformMatrix.identity();
  rotateMatrix.identity();

  //Init all key states to false
  for(uint i=0;i<CKC_Max;i++)
  {
    cameraKeyState[i] = false;
  }

  //Init the scissor area
  oldScissorArea[0] = 0;
  oldScissorArea[1] = 0;
  oldScissorArea[2] = 0;
  oldScissorArea[3] = 0;

  oldPolyFillModes[0] = 0;
  oldPolyFillModes[1] = 0;

  //Register all the callbacks
  gliCallBacks->RegisterGLFunction("glEnd");

  //Register the user ids for the callbacks
  gliCallBacks->SetFunctionID("glEnd",CBI_glEnd);

  //Parse the config file
  ConfigParser fileParser;
  if(fileParser.Parse(dllPath + "config.ini"))
  {
    configData.ReadConfigData(&fileParser);
    fileParser.LogUnusedTokens(); 
  }

  //Parse the config string
  ConfigParser stringParser;
  if(stringParser.ParseString(gliCallBacks->GetConfigString()))
  {
    configData.ReadConfigData(&stringParser);
    stringParser.LogUnusedTokens(); 
  }
}

///////////////////////////////////////////////////////////////////////////////
//
OpenGLFreeCamera::~OpenGLFreeCamera()
{

}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::Destroy()
{
  //LOGERR(("Free Cam Plugin Destroyed"));
  
  //Destroy this plugin
  delete this;
}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::GLFunctionPre(uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args )
{
  //Return if not init
  if(!initFlag)
  {
    return;
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //Return if not init
  if(!initFlag)
  {
    return;
  }

  //Handle post glBegin/glEnd blocks
  if(updateID == CBI_glEnd)
  {
    //Reset matrix mode 
    AssignModelViewMatrix(true);

    //Reset cull mode
    AssignCullFaceMode(true);

    //Reset the wireframe mode
    AssignWireFrameMode(true);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::GLRenderPre(const char *funcName, uint funcIndex, const FunctionArgs & args)
{
  //Return if not init
  if(!initFlag)
  {
    return;
  }

  //Set matrix mode 
  AssignModelViewMatrix(false);

  //Set cull mode
  AssignCullFaceMode(false);

  //Set the wireframe mode
  AssignWireFrameMode(false);

}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //Return if not init
  if(!initFlag)
  {
    return;
  }

  //Reset matrix mode 
  AssignModelViewMatrix(true);

  //Reset cull mode
  AssignCullFaceMode(true);

  //Reset the wireframe mode
  AssignWireFrameMode(true);

}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //Assign the main context ID if necessary 
  //(assuming the main context is the one with the end frame call)
  if(configData.mainContextOnly && mainContextID == NULL)
  {
    mainContextID = gliCallBacks->GetGLContextID();
  }

  //Check if the enable key state has changed
  if(inputSys.IsAllKeyDown(configData.enableKeys) != enableKeyState)
  {
    //Change the enabled key state
    enableKeyState = !enableKeyState;

    //If the keys were pressed, enable/disable the free camera mode
    if(enableKeyState)
    {
      //Set the camera mode
      SetFreeCameraMode(!freeCameraMode);

      //Reset the timer
      keyTimer.StartTimer();
    }
  }

  //Check if the disable cull keys are down 
  if(inputSys.IsAllKeyDown(configData.disableFaceCullKeys) != disableCullKeyState)
  {
    //Change the disable cull key state
    disableCullKeyState = !disableCullKeyState;

    //If the keys were pressed, change the cull mode
    if(disableCullKeyState)
    {
      disableCullMode = !disableCullMode;
    }
  }

  //Check if the enable wireframe keys are down 
  if(inputSys.IsAllKeyDown(configData.enableWireFrameKeys) != enableWireFrameKeyState)
  {
    //Change the enable wireframe key state
    enableWireFrameKeyState = !enableWireFrameKeyState;

    //If the keys were pressed, change the wireframe mode
    if(enableWireFrameKeyState)
    {
      enableWireFrameMode = !enableWireFrameMode;
    }
  }

  //Check if the render view keys are down 
  if(inputSys.IsAllKeyDown(configData.renderViewFrustumKeys) != renderViewKeyState)
  {
    //Change the render view key state
    renderViewKeyState = !renderViewKeyState;

    //If the keys were pressed, change the render mode
    if(renderViewKeyState)
    {
      renderViewMode = !renderViewMode;
    }
  }

  //If the transform matrix is not the identity, process the reset keys
  if(!isTransformMatrixIdentity)
  {
    //Process position reset
    if(inputSys.IsAllKeyDown(configData.resetPositionKeys) != resetPositionKeyState)
    {
      //Change the key state
      resetPositionKeyState = !resetPositionKeyState;

      //Reset the position
      if(resetPositionKeyState)
      {
        positionOffset  = vec4(0.0f,0.0f,0.0f,0.0f);

        //Reset the transform matrix
        transformMatrix = transpose(rotateMatrix);

        //Set if the matrix is now the identity matrix
        if(transformMatrix.isIdentity())
        {
          isTransformMatrixIdentity = true;
        }
      }
    }

    //Process orientation reset
    if(inputSys.IsAllKeyDown(configData.resetOrientationKeys) != resetOrientationKeyState)
    {
      //Change the key state
      resetOrientationKeyState = !resetOrientationKeyState;

      //Reset the orientation
      if(resetOrientationKeyState)
      {
        rotateMatrix.identity();

        //Reset the transform matrix
        transformMatrix.identity();
        transformMatrix.elem[0][3] = positionOffset.x;
        transformMatrix.elem[1][3] = positionOffset.y;
        transformMatrix.elem[2][3] = positionOffset.z;

        //Transpose the matrix for loading into OpenGL
        transformMatrix = transpose(transformMatrix);

        //Set if the matrix is now the identity matrix
        if(transformMatrix.isIdentity())
        {
          isTransformMatrixIdentity = true;
        }
      }
    }
  }

  //If in free camera mode, process the camera controls
  if(freeCameraMode)
  {
    //Get the time diff
    uint timeDiff = keyTimer.GetTimeDiff();

    //Get the elapsed time
    float elapsedTime = (float)((double)timeDiff * 1.0e-06);

    //Only process if the elapsd time is significant
    if(elapsedTime > 0.001f)
    {
      //Reset the timer for next frame
      keyTimer.StartTimer();

      ProcessCameraKeys(elapsedTime);   
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle)
{
  //If the new context is NULL or already init return now
  if(newRCHandle == NULL || initFlag )
  { 
    return;
  }

  //Initialize the view frustum visualization
  viewCamera.Init(configData);

  //Flag that initialization was successful 
  initFlag = true;
}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::OnGLContextDelete(HGLRC rcHandle)
{
  //If the main context is being deleted, reset the main context id
  if(rcHandle == mainContextID)
  {
    mainContextID = NULL;
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::SetFreeCameraMode(bool newMode)
{
  //Return if the mode is already equal
  if(newMode == freeCameraMode)
  {
    return;
  }

  //Assign the state
  freeCameraMode = newMode;

  //If enabling
  if(freeCameraMode)
  {
    //If using input lock, enable the window hooks
    if(configData.inputLock)
    {
      inputSys.HookInput(1);
    }
  }
  else
  {
    //Disable the window hooks if necessary
    if(configData.inputLock)
    {
      inputSys.UnHookInput();
    }
  }

  //Init all key states to false
  for(uint i=0;i<CKC_Max;i++)
  {
    cameraKeyState[i] = false;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::ProcessCameraKeys(float elapsedTime)
{
  vec4 translate(0.0f,0.0f,0.0f,0.0f);
  vec3 rotate(0.0f,0.0f,0.0f);

  //Assign initial movement speed
  float moveAmount = configData.moveSpeed;

  //Check if the move modifier key is down
  if(configData.moveModifierKey != 0 &&
     inputSys.IsKeyDown(configData.moveModifierKey))
  {
    moveAmount *= configData.moveModifierAmount; 
  }

  //Loop for all keycodes
  for(uint i=0;i<CKC_Max;i++)
  {
    //Test if the key is down
    if(inputSys.IsKeyDown(configData.keyMappings[i]))
    {
      //If already down, get the time elapsed
      if(cameraKeyState[i])
      {
        //Determine what key code it was 
        switch(i)
        {
          //Position move
          case(CKC_StrafeLeft):
            translate.x += moveAmount * elapsedTime;
            break;

          case(CKC_StrafeRight):
            translate.x -= moveAmount * elapsedTime;
            break;

          case(CKC_Forward):
            translate.z += moveAmount * elapsedTime;
            break;

          case(CKC_Backward):
            translate.z -= moveAmount * elapsedTime;
            break;

          //Rotation
          case(CKC_YawLeft):
            rotate.y += configData.rotateSpeed * elapsedTime;
            break;

          case(CKC_YawRight):
            rotate.y -= configData.rotateSpeed * elapsedTime;
            break;

          case(CKC_PitchUp):
            rotate.x -= configData.rotateSpeed * elapsedTime;
            break;

          case(CKC_PitchDown):
            rotate.x += configData.rotateSpeed * elapsedTime;
            break;

          case(CKC_RollLeft):
            rotate.z -= configData.rotateSpeed * elapsedTime;
            break;

          case(CKC_RollRight):
            rotate.z += configData.rotateSpeed * elapsedTime;
            break;

        }
      }
      else
      {
        //Flag that the key is now down
        cameraKeyState[i] = true;
      }
    }
    else
    {
      cameraKeyState[i] = false;
    }
  }


  //Update the transform matrix
  if(rotate.x != 0.0f    || rotate.y != 0.0f    || rotate.z != 0.0f ||
     translate.x != 0.0f || translate.y != 0.0f || translate.z != 0.0f)
  {
    //Update the global rotation
    rotateMatrix = rotateZXY(rotate.x,rotate.y,rotate.z) * rotateMatrix;

    //Update the global position
    positionOffset =  positionOffset + (transpose(rotateMatrix) *translate);

    //Calculate the new trasform matrix
    mat4 workingMatrix;
    workingMatrix.identity();

    workingMatrix.elem[0][3] = positionOffset.x;
    workingMatrix.elem[1][3] = positionOffset.y;
    workingMatrix.elem[2][3] = positionOffset.z;

    transformMatrix =  transpose(rotateMatrix * workingMatrix);

    //Flag that the matrix is no longer the identity matrix
    isTransformMatrixIdentity = false;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::AssignModelViewMatrix(bool reset)
{
  //If resetting and the matrix has not been altered, return
  if(reset && !modelViewAltered)
  {
    return;
  }

  //If not resetting and the matrix is identity or has already been altered, return
  if(!reset && (isTransformMatrixIdentity || modelViewAltered))
  {
    return;
  }

  //Return now if unable to make OpenGL calls
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //If the camera only moves on the "main" context
  if(configData.mainContextOnly &&
     gliCallBacks->GetGLContextID() != mainContextID)
  {
    return;
  }

  //If we ignore orthographic modes
  if(configData.ignoreOrthographic)
  {
    //Get the projection matrix
    GLfloat testProjMatrix[16];
    GLV->glGetFloatv(GL_PROJECTION_MATRIX,&testProjMatrix[0]);

    //If the "w" is not multiplied by any z, this is probably orthographic
    // (This may be a little bit of a hack)
    if(testProjMatrix[11] == 0.0f)
    {
      return;
    }
  }

  //Set the OpenGL state to the model view matrix mode
  GLint oldMatrixMode;
  GLV->glGetIntegerv(GL_MATRIX_MODE, &oldMatrixMode);
  if(oldMatrixMode != GL_MODELVIEW)
  {
    GLV->glMatrixMode(GL_MODELVIEW);
  }
  
  //Determine if we are resetting, or changing the matrix
  if(!reset)
  {
    //Save the current model view matrix
    GLV->glGetFloatv(GL_MODELVIEW_MATRIX,oldModelView.getPtr());

    //Load in the custom translation and rotation matrix
    GLV->glLoadMatrixf(transformMatrix.getPtr()); 

    //Retrieve and set the lighting data for the new matrix
    RetrieveLightStateData();

    //Multiply by the origional model view matrix
    GLV->glMultMatrixf(oldModelView.getPtr());

    //Adjust scissoring to be the viewport size if necessary
    if(configData.adjustScissor)
    {
      //Get the old scissor values
      GLV->glGetIntegerv(GL_SCISSOR_BOX, &oldScissorArea[0]);

      //Get the viewport area
      GLint newScissorArea[4];
      GLV->glGetIntegerv(GL_VIEWPORT, &newScissorArea[0]);

      //Apply the new scissor (even if it is disabled)
      GLV->glScissor(newScissorArea[0],newScissorArea[1],newScissorArea[2],newScissorArea[3]);
    }

    //Flag the the openGL state has been changed
    modelViewAltered = true;
  }
  else
  {
    //If rendering the view frustum is enabled
    if(renderViewMode)
    {
      //Re-load in the custom translation and rotation matrix
      GLV->glLoadMatrixf(transformMatrix.getPtr()); 
      viewCamera.RenderViewFrustum();
    }

    //Reset lighting data (may reset the matrix to identity)
    ResetLightStateData();

    //Reset the old matrix
    GLV->glLoadMatrixf(oldModelView.getPtr());
    
    //Reset scissoring if necessary
    if(configData.adjustScissor)
    {
      GLV->glScissor(oldScissorArea[0],oldScissorArea[1],oldScissorArea[2],oldScissorArea[3]);
    }

    //Flag the the openGL state has been reset
    modelViewAltered = false;
  }

  //Reset the matrix mode
  if(oldMatrixMode != GL_MODELVIEW)
  {
    GLV->glMatrixMode(oldMatrixMode);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::AssignCullFaceMode(bool reset)
{
  //If not disable cull mode or we are resetting and no reset is required
  if(!disableCullMode || 
     (reset && !oldFaceCullState) ||
     !gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //If disablingcull face mode
  if(!reset)
  {
    //Get the old cull face state
    if(GLV->glIsEnabled(GL_CULL_FACE))
    {
      //Disable if enabled
      GLV->glDisable(GL_CULL_FACE);
      oldFaceCullState = true;
    }
    else
    {
      oldFaceCullState = false;
    }
  }
  else
  {
    //Re-enable the state
    GLV->glEnable(GL_CULL_FACE);

    //Reset the state
    oldFaceCullState = false;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::AssignWireFrameMode(bool reset)
{
  //If not disable cull mode or we are resetting and no reset is required
  if(!enableWireFrameMode || 
     !gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //If enabling wire frame mode
  if(!reset)
  {
    //Get the old polygon fill modes
    GLV->glGetIntegerv(GL_POLYGON_MODE, oldPolyFillModes);

    //Set both front and back to wireframe
    GLV->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else
  {
    //Reset the poly fill modes
    GLV->glPolygonMode(GL_FRONT, oldPolyFillModes[0]);
    GLV->glPolygonMode(GL_BACK,  oldPolyFillModes[1]);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::RetrieveLightStateData()
{
  //If not enabled, return
  if(!configData.adjustGLLighting)
  {
    return;
  }

  //If not forced and OpenGL lighting is not enabled, return
  if(!configData.forceAdjustGLLighting && !GLV->glIsEnabled(GL_LIGHTING))
  {
    return;
  }

  //Clear the array
  oldLightStates.clear();

  //Loop for all OpenGL lights 
  // (could query for GL_MAX_LIGHTS but all implementations have to support at least 8)
  // (more than 8 will cause enum problems with GL_LIGHT0 + i)
  for(uint i=0; i< MIN_GL_LIGHT_SUPPORT; i++)
  {
    //If enabled or forced
    if(configData.forceAdjustGLLighting || GLV->glIsEnabled(GL_LIGHT0 + i))
    {
      LightStateData newData;
      
      //Assign the light index
      newData.lightNum = GL_LIGHT0 + i;

      //Get the light position and direction (in old eye space)
      GLV->glGetLightfv(newData.lightNum, GL_POSITION, &newData.lightPos.x);
      GLV->glGetLightfv(newData.lightNum, GL_SPOT_DIRECTION, &newData.lightDir.x);
      
      //Reset the light position and direction for the current matrix
      GLV->glLightfv(newData.lightNum, GL_POSITION, &newData.lightPos.x);
      GLV->glLightfv(newData.lightNum, GL_SPOT_DIRECTION, &newData.lightDir.x);

      //Add to the array
      oldLightStates.push_back(newData);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLFreeCamera::ResetLightStateData()
{
  //If the array is empty, return.
  if(oldLightStates.size() == 0)
  {
    return;
  }

  //Reset lighting by first reseting the view matrix
  GLV->glLoadIdentity();

  //Loop for all array elements
  for(uint i=0; i< oldLightStates.size(); i++)
  {
    //Re-set all light data
    GLV->glLightfv(oldLightStates[i].lightNum, GL_POSITION, &oldLightStates[i].lightPos.x);
    GLV->glLightfv(oldLightStates[i].lightNum, GL_SPOT_DIRECTION, &oldLightStates[i].lightDir.x);
  }

  //Clear the array
  oldLightStates.clear();
}








