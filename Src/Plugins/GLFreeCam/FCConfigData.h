/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __FC_CONFIG_DATA_H_
#define __FC_CONFIG_DATA_H_

#include <string>
#include <vector>
#include <ConfigParser.h>

using namespace std;

// Enums for camera movement
enum CameraKeyCodes
{
  CKC_PitchUp =0,
  CKC_PitchDown,

  CKC_YawLeft,
  CKC_YawRight,

  CKC_RollLeft,
  CKC_RollRight,

  CKC_Forward,
  CKC_Backward,

  CKC_StrafeLeft,
  CKC_StrafeRight,

  CKC_Max
};


//@
//  Summary:
//    This structure holds all the configuration data used by the GL Free Camera
//    and associated classes.
//  
class FCConfigData
{
public:

  //@
  //  Summary:
  //    Constructor. Inits all config data to default values.
  //  
  FCConfigData();

  //@
  //  Summary:
  //    To read the config data values.
  //  
  //  Parameters:
  //    parser - The parser to read the data from.
  //
  void ReadConfigData(const ConfigParser *parser);

  bool inputLock;                                 // Flag to indicate if input locking is performed when the free camera is enabled
  bool adjustGLLighting;                          // Flag to indicate if OpenGL lighting is adjusted in free camera mode
  bool forceAdjustGLLighting;                     // Flag to indicate if OpenGL lighting is force-adjusted in free camera mode
  bool adjustScissor;                             // Flag to indicate if OpenGL scissoring is adjusted to be the viewport size on render 
  bool ignoreOrthographic;                        // Flag to indicate if orthographic projection (as used in user interfaces) is to be ignored
  bool mainContextOnly;                           // Flag to indicating that the free camera updates should only apply to the "main" context

  float  moveSpeed;                               // The camera movement speed
  float  rotateSpeed;                             // The camera rotate speed 
 
  uint   moveModifierKey;                         // The key to press to adjust the move speed amount
  float  moveModifierAmount;                      // The amount to multiply by the move speed when the modifier key is pressed

  string viewFrustumColor;                        // String indicating the view frustum color

  vector<uint>    enableKeys;                     // The key combination to enable the free camera
  vector<uint>    resetPositionKeys;              // The key combination to reset the camera position
  vector<uint>    resetOrientationKeys;           // The key combination to reset the camera orientation
  vector<uint>    disableFaceCullKeys;            // The key combination to disable/enable face culling
  vector<uint>    enableWireFrameKeys;            // The key combination to disable/enable wireframe mode
  vector<uint>    renderViewFrustumKeys;          // The key combination to disable/enable view frustum rendering

  uint  keyMappings[CKC_Max];                     // The array of key mappings for camera movement
  


protected:

  //@
  //  Summary:
  //    To get the key codes for the specified token value from the parser.
  //    If the token does not exist, the key code array is not changed.
  //
  //  Parameters:
  //    tokenName - The name of the token to extract from the parser.
  //
  //    parser   - The parser to read the data from.
  //
  //    keyCodes - The array to fill out with keycodes read fro the parser.
  //
  void GetKeyCodes(const string &tokenName, const ConfigParser *parser, vector<uint> &keyCodes) const;

  //@
  //  Summary:
  //    To assign the internal key mapping value to the value in the token
  //  
  //  Parameters:
  //    cameraCode - The camera key code to retrieve.
  //
  //    tokenName  - The token name for the camera keycode.
  //
  //    cameraKeyToken - The root token for camera key codes.
  //
  void GetCameraCode(CameraKeyCodes cameraCode, string tokenName,const ConfigToken * cameraKeyToken);

};



#endif // __FC_CONFIG_DATA_H_
