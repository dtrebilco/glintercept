/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __FCAM_INPUT_H_
#define __FCAM_INPUT_H_

#include <InputUtils.h>

#include <vector>

using namespace std;
typedef unsigned int uint;


//@
//  Summary:
//    This class contains methods for input and data from mouse/keyboard.
//  
class FCamInput
{
public:

  //@
  //  Summary:
  //    Constructor. 
  //  
  FCamInput(const InputUtils *inputUtils);
  virtual ~FCamInput();

  //@
  //  Summary:
  //    To determine if a key is currently down or up.
  //  
  //  Parameters:
  //    keyCode  - The OS specific key code to check. (use InputUtils::GetKeyCode)
  //
  //  Returns:
  //    If the key is down, true is returned. If the keycode is unknown
  //    or the key is up, false is returned.
  //
  bool IsKeyDown(uint keyCode);

  //@
  //  Summary:
  //    To if all the passed keys are down.
  //  
  //  Parameters:
  //    keyCodes - The array of key codes to check.
  //
  //  Returns:
  //    If all the passed keycodes are down, true is returned. 
  //    Else false is returned. (false is returned if no keys are passed)
  //
  bool IsAllKeyDown(const vector<uint> &keyCodes);

  //@
  //  Summary:
  //    To hook the mouse/keyboard. (ie attempt to gain exclusive 
  //    access to the keyboard/mouse)
  //  
  //  Parameters:
  //    hookLevel - The hook level to use 0 = no hook, 1 = weak hook.
  //
  //  Returns:
  //    If the keyboard/mouse was successfully hooked, true is returned.
  //    Else false is returned.
  //
  bool HookInput(uint hookLevel);

  //@
  //  Summary:
  //    To un-hook the mouse/keyboard.
  //  
  void UnHookInput();


protected:

  uint  hookLevel;                                // The level of keyboard hooking that is occuring
  const InputUtils *inputUtils;                   // The input utility class

};



#endif // __FCAM_INPUT_H_
