/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INPUT_UTILS_H__
#define __INPUT_UTILS_H__

#include <string>
#include <vector>

#ifdef GLI_BUILD_LINUX
#include <X11/Xlib.h>
#endif //GLI_BUILD_LINUX

using namespace std;
typedef unsigned int uint;


//@
//  Summary:
//    Input utility functions that are OS specific
// 
class InputUtils
{
public:

  //@
  //  Summary:
  //    Constructor
  //  
  InputUtils();

#ifdef GLI_BUILD_LINUX

  //@
  //  Summary:
  //    To update the input system using the passed X-Display.
  //    (It is assumed that the passed X-Display will exist for the current 
  //     frame)
  //  
  //  Parameters:
  //    display  - The X-Display to use for input.
  //
  //  Returns:
  //    If true is returned, the update was successful. Else false is returned.
  //
  bool Update(Display *display); 

#endif //GLI_BUILD_LINUX

  //@
  //  Summary:
  //    To determine if a key is currently down or up.
  //  
  //  Parameters:
  //    keyCode  - The OS specific key code to check. (use GetKeyCode)
  //
  //  Returns:
  //    If the key is down, true is returned. If the keycode is unknown
  //    or the key is up, false is returned.
  //
  bool IsKeyDown(uint keyCode) const;

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
  bool IsAllKeyDown(const vector<uint> &keyCodes) const;

  //@
  //  Summary:
  //    To get the OS specific key code for the passed string.
  //  
  //  Parameters:
  //    keyString  - The passed string to get the keycode for. Values can be
  //                 a single letter or number or a string like "ctrl","alt",
  //                 "shift","num1","tab" etc.
  //
  //  Returns:
  //    If zero is returned, the keycode for the string could not be found. 
  //    Else, the keycode is returned.
  //
  static uint GetKeyCode(const string & keyString);

protected:


#ifdef GLI_BUILD_LINUX

  Display *xDisplay;                              //The X-Display to use for input
  char     keyState[32];                          //The current key state from the X-Display

#endif //GLI_BUILD_LINUX

};








#endif //__INPUT_UTILS_H__
