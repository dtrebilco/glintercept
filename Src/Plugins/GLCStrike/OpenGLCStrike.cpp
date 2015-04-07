#include "OpenGLCStrike.h"

#include <ConfigParser.h>
#include <CommonErrorLog.h>

// Some pre-OGL3.0 defines
#define GL_MODULATE                             0x2100
#define GL_TEXTURE_ENV                          0x2300
#define GL_TEXTURE_ENV_MODE                     0x2200
#define GL_TEXTURE_ENV_COLOR                    0x2201

#define GL_PROJECTION_MATRIX                    0x0BA7


USING_ERRORLOG

//The call back ids
enum CallBackIDs
{
  CBI_None =0,
  CBI_glBegin,
  CBI_glEnd
};

//Path to the dll
extern string dllPath;


///////////////////////////////////////////////////////////////////////////////
//
OpenGLCStrike::OpenGLCStrike(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),
GLV(callBacks->GetCoreGLFunctions()),
inputUtils(callBacks->GetInputUtils()),
firstRenderCall(true),
firstUICall(true),
renderDisabled(false),
renderCallType(CTI_Unknown),

drawCharacter(true),
drawWeapon(true),
drawBackground(DBM_Normal),
drawUI(true),

oldTexEnvMode(GL_MODULATE),

characterKeyState(false),
weaponKeyState(false),
backgroundKeyState(false),
UIKeyState(false)

{
  //Init the clear color
  clearColor[0] = 1.0f;
  clearColor[1] = 1.0f;
  clearColor[2] = 1.0f;
  clearColor[3] = 1.0f;

  //Register all the callbacks
  gliCallBacks->RegisterGLFunction("glEnd");
  gliCallBacks->RegisterGLFunction("glBegin");

  //Register the user ids for the callbacks
  gliCallBacks->SetFunctionID("glEnd",  CBI_glEnd);
  gliCallBacks->SetFunctionID("glBegin",CBI_glBegin);

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
}

///////////////////////////////////////////////////////////////////////////////
//
OpenGLCStrike::~OpenGLCStrike()
{

}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLCStrike::Destroy()
{
  
  //Destroy this plugin
  delete this;
}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLCStrike::ProcessConfigData(ConfigParser *parser)
{
  //Get the enable keys
  GetKeyCodes("ToggleCharacter",parser,toggleCharacterKeys);
  GetKeyCodes("ToggleWeapon",parser,toggleWeaponKeys);
  GetKeyCodes("ToggleBackground",parser,toggleBackgroundKeys);
  GetKeyCodes("ToggleUI",parser,toggleUIKeys);

  //Get the token for the color
  const ConfigToken *colorToken = parser->GetToken("ClearColor");
  if(colorToken && colorToken->GetNumValues() == 4)
  {
    int value[4];
    colorToken->Get(value[0],0);
    colorToken->Get(value[1],1);
    colorToken->Get(value[2],2);
    colorToken->Get(value[3],3);

    clearColor[0] = float(value[0]) / 255.0f;
    clearColor[1] = float(value[1]) / 255.0f;
    clearColor[2] = float(value[2]) / 255.0f;
    clearColor[3] = float(value[3]) / 255.0f;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLCStrike::GetKeyCodes(const string &tokenName, const ConfigParser *parser, vector<uint> &keyCodes) const
{
  const ConfigToken *testToken;

  //Get the token for the string
  testToken = parser->GetToken(tokenName);
  if(testToken)
  {
    //Clear any existing key codes
    keyCodes.clear();

    //Loop for the number of values in the token
    for(uint i=0;i<testToken->GetNumValues();i++)
    {
      string value;
      testToken->Get(value,i);

      //Get the key code of the string
      uint newValue = InputUtils::GetKeyCode(value);
      if(newValue != 0)
      {
        //Add the value to the array
        keyCodes.push_back(newValue);
      }
    }       
  }

}

///////////////////////////////////////////////////////////////////////////////
//
CallTypeID OpenGLCStrike::GetCallTypeID(const FunctionArgs & args) const
{
  //Determine if perspective is used
  GLfloat testProjMatrix[16];
  GLV->glGetFloatv(GL_PROJECTION_MATRIX,&testProjMatrix[0]);

  //If the "w" is not multiplied by any z, this is probably orthographic
  // (This may be a little bit of a hack)
  if(testProjMatrix[11] == 0.0f)
  {
    //The first render call could be the target cursor
    if(firstUICall)
    {
      GLint srcBlend = 0, dstBlend = 0;

      GLV->glGetIntegerv(GL_BLEND_SRC, &srcBlend);
      GLV->glGetIntegerv(GL_BLEND_DST, &dstBlend);

      //If the blend modes are correct for the crosshair
      if(srcBlend == GL_SRC_ALPHA && dstBlend == GL_ONE_MINUS_SRC_ALPHA)
      {
        return CTI_TargetCrossHair;
      }
    }

    return CTI_UI;
  }

  //Get the glBegin render type
  GLenum renderType;
  FunctionArgs accessArgs(args);
  accessArgs.Get(renderType);

  //Determine if the arguments are GL_TRIANGLES or not
  if(renderType == GL_TRIANGLE_STRIP || renderType == GL_TRIANGLE_FAN)
  {
    GLfloat depthRange[2];
    GLV->glGetFloatv(GL_DEPTH_RANGE, depthRange);

    //The character's weapon is written in a different depth range
    if(depthRange[1] == 0.85f || depthRange[1] == 0.149997f)
    {
      return CTI_Weapon;
    }

    return CTI_Character;
  }
  else
  {
    //If blending is used, assume PFX (sparks, decals, blood etc)
    GLint blendMode;
    GLV->glGetIntegerv(GL_BLEND, &blendMode);

    if(blendMode != 0)
    {
      return CTI_PFX;
    }

    return CTI_Background;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLCStrike::GLFunctionPre(uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args )
{
  //Return if not available
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //Handle pre glBegin/glEnd blocks
  if(updateID == CBI_glBegin)
  {
    //If this is the first rendre call this frame
    if(firstRenderCall)
    {
      //Clear the background color 
      //  (must be done here as HL's frame saving code is broken and does not save before the buffer swap)
      if(drawBackground != DBM_Normal && gliCallBacks->GetGLInternalCallState())
      {
        //Clear to the requested clear color
        GLV->glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        GLV->glClear(GL_COLOR_BUFFER_BIT);
      }

      //Reset the flag
      firstRenderCall = false;
    }

    //Determine the call type
    renderCallType = GetCallTypeID(args);
    
    //Disable rendering if necessary 
    if((renderCallType == CTI_Character && !drawCharacter) ||
       (renderCallType == CTI_Weapon && !drawWeapon) ||
       (renderCallType == CTI_Background && drawBackground != DBM_Normal) ||
       (renderCallType == CTI_PFX && drawBackground != DBM_Normal) ||
       (renderCallType == CTI_UI && !drawUI))
    {
      //Set rendering as disabled
      renderDisabled = true;

      //If drawing the background in "clip mode", setup a texture environment
      if(renderCallType == CTI_Background && drawBackground == DBM_ClipMode)
      {
        //Save the old mode
        GLV->glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &oldTexEnvMode);

        //Set a mode to render the color
        GLV->glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, &clearColor[0]); 
        GLV->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
        GLV->glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB,      GL_REPLACE);
        GLV->glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA,    GL_REPLACE);
        GLV->glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB,      GL_CONSTANT);
        GLV->glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA,    GL_CONSTANT);
      }
      else
      {
        //Get the current view area
        GLV->glGetIntegerv(GL_VIEWPORT, &oldViewportArea[0]);
        
        //Set an empty view area
        GLV->glViewport(oldViewportArea[0], oldViewportArea[1], 0, 0);
      }
    }


    //If this was a UI call, set the flag that indicates the first UI call to false
    if(renderCallType == CTI_UI || renderCallType == CTI_TargetCrossHair)
    {  
      firstUICall = false;
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLCStrike::GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //Return if not available
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //Handle post glBegin/glEnd blocks
  if(updateID == CBI_glEnd)
  {
    //If rendering has been disabled
    if(renderDisabled)
    {
      //If rendering the background in clip mode
      if(renderCallType == CTI_Background && drawBackground == DBM_ClipMode)
      {
        //Restore the texture env mode. (don't worry about the combine state at HL does not use it)
        GLV->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, oldTexEnvMode);
      }
      else
      {
        //Restore the view area
        GLV->glViewport(oldViewportArea[0], oldViewportArea[1], oldViewportArea[2], oldViewportArea[3]);
      }

      //Reset the flag
      renderDisabled = false;
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLCStrike::GLRenderPre(const char *funcName, uint funcIndex, const FunctionArgs & args)
{
}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLCStrike::GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLCStrike::GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //Reset the first render call for the frame flag
  firstRenderCall = true;
  firstUICall     = true;

  //Check if the enable key state has changed
  if(inputUtils->IsAllKeyDown(toggleCharacterKeys) != characterKeyState)
  {
    //Change the enabled key state
    characterKeyState = !characterKeyState;

    //If the keys were pressed, 
    if(characterKeyState)
    {
      //Toggle the character display
      drawCharacter = !drawCharacter;
    }
  }

  //Get the weapon keys
  if(inputUtils->IsAllKeyDown(toggleWeaponKeys) != weaponKeyState)
  {
    //Change the enabled key state
    weaponKeyState = !weaponKeyState;

    //If the keys were pressed, 
    if(weaponKeyState)
    {
      //Toggle the weapon display
      drawWeapon = !drawWeapon;
    }
  }

  //Get the background keys
  if(inputUtils->IsAllKeyDown(toggleBackgroundKeys) != backgroundKeyState)
  {
    //Change the enabled key state
    backgroundKeyState = !backgroundKeyState;

    //If the keys were pressed, 
    if(backgroundKeyState)
    {
      //Toggle the background display
      drawBackground = DrawBackgroundMode((drawBackground+1) % DBM_Max);
    }
  }

  //Get the UI keys
  if(inputUtils->IsAllKeyDown(toggleUIKeys) != UIKeyState)
  {
    //Change the enabled key state
    UIKeyState = !UIKeyState;

    //If the keys were pressed, 
    if(UIKeyState)
    {
      //Toggle the character display
      drawUI = !drawUI;
    }
  }

  //Clear the screen to a constant depth
  if(drawBackground != DBM_Normal && gliCallBacks->GetGLInternalCallState())
  {
    //Get the weird "flipping" depth range used this frame
    GLfloat depthRange[2];
    GLV->glGetFloatv(GL_DEPTH_RANGE, depthRange);
    GLV->glClearDepth(depthRange[1]);

    //Clear depth (must be done as HL uses the depth buffer in half to avoid buffer clears)
    GLV->glClear(GL_DEPTH_BUFFER_BIT);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void OpenGLCStrike::OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle)
{
}

///////////////////////////////////////////////////////////////////////////////
//
void OpenGLCStrike::OnGLContextDelete(HGLRC rcHandle)
{
}






