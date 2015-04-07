#ifndef __OPENGL_CSTRIKE_H_
#define __OPENGL_CSTRIKE_H_

#include "..\\..\\MainLib\\InterceptPluginInterface.h"
#include <string>
#include <vector>
#include <MiscUtils.h>

using namespace std;

class ConfigParser;

//OpenGL 1.3 defines
#define  GL_COMBINE                0x8570
#define  GL_COMBINE_RGB            0x8571
#define  GL_COMBINE_ALPHA          0x8572
#define  GL_SOURCE0_RGB            0x8580
#define  GL_SOURCE1_RGB            0x8581
#define  GL_SOURCE2_RGB            0x8582
#define  GL_SOURCE0_ALPHA          0x8588
#define  GL_SOURCE1_ALPHA          0x8589
#define  GL_SOURCE2_ALPHA          0x858A
#define  GL_OPERAND0_RGB           0x8590
#define  GL_OPERAND1_RGB           0x8591
#define  GL_OPERAND2_RGB           0x8592
#define  GL_OPERAND0_ALPHA         0x8598
#define  GL_OPERAND1_ALPHA         0x8599
#define  GL_OPERAND2_ALPHA         0x859A
#define  GL_RGB_SCALE              0x8573
#define  GL_ADD_SIGNED             0x8574
#define  GL_INTERPOLATE            0x8575
#define  GL_SUBTRACT               0x84E7
#define  GL_CONSTANT               0x8576
#define  GL_PRIMARY_COLOR          0x8577
#define  GL_PREVIOUS               0x8578
#define  GL_DOT3_RGB               0x86AE
#define  GL_DOT3_RGBA              0x86AF
#define  GL_CLAMP_TO_BORDER        0x812D

//The call type
enum CallTypeID
{
  CTI_Unknown =0,
  CTI_Character,
  CTI_Weapon,
  CTI_Background,
  CTI_PFX,
  CTI_UI,
  CTI_TargetCrossHair,
};

//The different draw modes for backgrounds/walls
enum DrawBackgroundMode
{
  DBM_Normal =0, // Render as normal
  DBM_ClipMode,  // Render using the specified color 
  DBM_CullMode,  // Don't render at all
  DBM_Max        // Max value
};


//@
//  Summary:
//    This class provides counter-strike debug functionality.
//  
class OpenGLCStrike : public InterceptPluginInterface
{
public:

  //@
  //  Summary:
  //    Constructor. Sets up call back data.
  //  
  //  Parameters:
  //    callBacks - The call back interface from GLIntercept.
  //
  OpenGLCStrike(InterceptPluginCallbacks *callBacks);

  //@
  //  Summary:
  //    Destructor. Only to be called from Destroy().
  //  
  virtual ~OpenGLCStrike();

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

  InterceptPluginCallbacks *gliCallBacks;         // The callback interface into GLIntercept
  const GLCoreDriver       *GLV;                  // The core OpenGL driver
  const InputUtils         *inputUtils;           // The input utility class

  bool         firstRenderCall;                   // Flag if this is the first render call this frame
  bool         firstUICall;                       // Flag if this is the first UI render call this frame
  bool         renderDisabled;                    // Flag if the current rendre call block is disabled

  bool               drawCharacter;               // Flag indicating if character drawing is enabled.
  bool               drawWeapon;                  // Flag indicating if weapon drawing is enabled.
  DrawBackgroundMode drawBackground;              // Flag indicating if background drawing is enabled.
  bool               drawUI;                      // Flag indicating if UI drawing is enabled.

  GLint        oldViewportArea[4];                // The old saved viewport area
  GLint        oldTexEnvMode;                     // The old texture environment mode
  GLfloat      clearColor[4];                     // The clear frame buffer color values 
  CallTypeID   renderCallType;                    // The render call type

  vector<uint>    toggleCharacterKeys;            // The key combination to toggle character display
  vector<uint>    toggleWeaponKeys;               // The key combination to toggle weapon display
  vector<uint>    toggleBackgroundKeys;           // The key combination to toggle the background display
  vector<uint>    toggleUIKeys;                   // The key combination to toggle the user interface

  bool            characterKeyState;              // The state of the character keys
  bool            weaponKeyState;                 // The state of the weapon keys
  bool            backgroundKeyState;             // The state of the background keys
  bool            UIKeyState;                     // The state of the UI keys

  //@
  //  Summary:
  //    To process the configuration data.
  //
  void ProcessConfigData(ConfigParser *fileParser);

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
  //    To get type of gemoetry that is being rendered in this glBegin call.
  //
  //  Parameters:
  //    args - The arguments to glBegin
  //
  //  Returns:
  //    A guess at the geometry type is returned.
  //
  CallTypeID GetCallTypeID(const FunctionArgs & args) const;

};


//Unused entry points

///////////////////////////////////////////////////////////////////////////////
//
inline void OpenGLCStrike::GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args )
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline void OpenGLCStrike::OnGLError(const char *funcName, uint funcIndex)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void OpenGLCStrike::OnGLContextCreate(HGLRC rcHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void OpenGLCStrike::OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle)
{

}



#endif // __OPENGL_CSTRIKE_H_