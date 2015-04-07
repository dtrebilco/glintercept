/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include <OSDefines.h>
#include "FCConfigData.h"

#include <ConfigParser.h>
#include <FileUtils.h>
#include <InputUtils.h>
#include <CommonErrorLog.h>

USING_ERRORLOG


///////////////////////////////////////////////////////////////////////////////
//
FCConfigData::FCConfigData():
inputLock(false),

adjustGLLighting(false),
forceAdjustGLLighting(false),

adjustScissor(false),
ignoreOrthographic(false),
mainContextOnly(false),

moveSpeed(1.0f),
rotateSpeed(1.0f),

moveModifierKey(0),
moveModifierAmount(1.0f),

viewFrustumColor("white")  
{
  //Init the key mapping array to zero
  for(uint i=0;i<CKC_Max;i++)
  {
    keyMappings[i] =0;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void FCConfigData::ReadConfigData(const ConfigParser *parser)
{
  //Get the key codes
  GetKeyCodes("EnableKeys",parser,enableKeys);
  GetKeyCodes("ResetPositionKeys",parser,resetPositionKeys);
  GetKeyCodes("ResetOrientationKeys",parser,resetOrientationKeys);
  GetKeyCodes("DisableFaceCullKeys",parser,disableFaceCullKeys);
  GetKeyCodes("EnableWireFrameKeys",parser,enableWireFrameKeys);
  GetKeyCodes("RenderViewFrustumKeys",parser,renderViewFrustumKeys);

  //Get the view frustum color
  const ConfigToken *viewColorToken;
  viewColorToken = parser->GetToken("RenderViewFrustumColor");
  if(viewColorToken)
  {
    viewColorToken->Get(viewFrustumColor);
  }

  //Get the OpenGL lighting token data
  const ConfigToken *lightToken;
  lightToken = parser->GetToken("AdjustGLLighting");
  if(lightToken)
  {
    lightToken->Get(adjustGLLighting);
  }
  lightToken = parser->GetToken("ForceAdjustGLLighting");
  if(lightToken)
  {
    lightToken->Get(forceAdjustGLLighting);
  }

  //Get if scissoring is adjusted
  const ConfigToken *scissorToken = parser->GetToken("AdjustScissor");
  if(scissorToken)
  {
    scissorToken->Get(adjustScissor);
  }

  //Get if ortho graphic mode is ignored
  const ConfigToken *orthoToken = parser->GetToken("IgnoreOrthographic");
  if(orthoToken)
  {
    orthoToken->Get(ignoreOrthographic);
  }

  //Get if only the main context is processed
  const ConfigToken *mainContextToken = parser->GetToken("MainContextOnly");
  if(mainContextToken)
  {
    mainContextToken->Get(mainContextOnly);
  }


  //Get the camera control key tokens
  const ConfigToken *keyToken;
  keyToken = parser->GetToken("CameraKeys");
  if(keyToken)
  {
    GetCameraCode(CKC_PitchUp,  "PitchUp",keyToken);
    GetCameraCode(CKC_PitchDown,"PitchDown",keyToken);

    GetCameraCode(CKC_YawLeft, "YawLeft",keyToken);
    GetCameraCode(CKC_YawRight,"YawRight",keyToken);

    GetCameraCode(CKC_RollLeft, "RollLeft",keyToken);
    GetCameraCode(CKC_RollRight,"RollRight",keyToken);

    GetCameraCode(CKC_Forward, "Forward",keyToken);
    GetCameraCode(CKC_Backward,"Backward",keyToken);

    GetCameraCode(CKC_StrafeLeft, "StrafeLeft",keyToken);
    GetCameraCode(CKC_StrafeRight,"StrafeRight",keyToken);
  }

  //Get if the input is locked 
  const ConfigToken *testToken;
  testToken = parser->GetToken("CameraInputLock");
  if(testToken)
  {
    testToken->Get(inputLock);
  }

  //Get the camera speed keys
  testToken = parser->GetToken("CameraMoveSpeed");
  if(testToken)
  {
    testToken->Get(moveSpeed);
  }
  testToken = parser->GetToken("CameraRotateSpeed");
  if(testToken)
  {
    testToken->Get(rotateSpeed);
  }
  testToken = parser->GetToken("CameraMoveModifierValue");
  if(testToken)
  {
    testToken->Get(moveModifierAmount);
  }

  //Get the key for the move modifier
  testToken = parser->GetToken("CameraMoveModifierKey");
  if(testToken)
  {
    string value;
    testToken->Get(value);

    //Get the key code of the string
    uint newValue = InputUtils::GetKeyCode(value);
    if(newValue != 0)
    {
      moveModifierKey = newValue;
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void FCConfigData::GetKeyCodes(const string &tokenName, const ConfigParser *parser, vector<uint> &keyCodes) const
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
void FCConfigData::GetCameraCode(CameraKeyCodes cameraCode, string tokenName,const ConfigToken * cameraKeyToken)
{
  const ConfigToken *testToken;

  //Get the token for the string
  testToken = cameraKeyToken->GetChildToken(tokenName);
  if(testToken)
  {
    string value;
    testToken->Get(value);

    //Get the key code of the string
    keyMappings[cameraCode] = InputUtils::GetKeyCode(value);
  }
}
