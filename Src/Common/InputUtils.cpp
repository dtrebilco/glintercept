/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include <OSDefines.h>
#include "InputUtils.h"

//Structure for holding key lookups
struct KeyLookup
{
  uint keyCode;
  const char *keyStr;
};

#ifdef GLI_BUILD_WINDOWS

//Some defines (and re-defines) of some not commonly used win32 virtual keys

#define VK_EQUALS        0xBB   //A virtual key code for the EQUAL SIGN (=) key. 
#define VK_MINUS         0xBD   //A virtual key code for the EQUAL SIGN (=) key. 

#define VK_LSHIFT        0xA0
#define VK_RSHIFT        0xA1
#define VK_LCONTROL      0xA2
#define VK_RCONTROL      0xA3
#define VK_LMENU         0xA4
#define VK_RMENU         0xA5

#define VK_BACK_QUOTE    0xC0   //A virtual key code for the apostrophe (`) key. 
#define VK_BACK_SLASH    0xDC   //A virtual key code for the BACKSLASH (\) key. 

#define VK_CLOSE_BRACKET 0xDD   //A virtual key code for the CLOSE BRACKET (]) key. 
#define VK_COMMA         0xBC   //A virtual key code for the COMMA (,) key. 
#define VK_OPEN_BRACKET  0xDB   //A virtual key code for the OPEN BRACKET ([) key. 
#define VK_PERIOD        0xBE   //A virtual key code for the PERIOD (.) key. 

#define VK_QUOTE         0xDE   //A virtual key code for the QUOTATION MARK key. 
#define VK_SEMICOLON     0xBA   //A virtual key code for the SEMICOLON (;) key. 
#define VK_SLASH         0xBF   //A virtual key code for the forward slash (/) key. 

#define VK_SCROLL_LOCK   0x91   //A virtual key code for the SCROLL LOCK key. 
#define VK_NUM_LOCK      0x90   //A virtual key code for the NUM LOCK key. 

//Key code lookups for windows
KeyLookup keyLookup[] = 
{
  {VK_BACK, "backspace"},
  {VK_TAB, "tab"},
  {VK_CLEAR, "clear"},
  {VK_RETURN, "enter"},

  {VK_SHIFT, "shift"},
  {VK_CONTROL, "ctrl"},
  {VK_MENU, "alt"},      

  {VK_LSHIFT, "lshift"},
  {VK_LCONTROL, "lctrl"},
  {VK_LMENU, "lalt"},      

  {VK_RSHIFT, "rshift"},
  {VK_RCONTROL, "rctrl"},
  {VK_RMENU, "ralt"},      

  {VK_PAUSE, "pause"},
  {VK_CAPITAL, "caps"},
  {VK_CAPITAL, "capslock"},
  {VK_ESCAPE, "esc"},
  {VK_SPACE, "space"},
  {VK_PRIOR, "pageup"},
  {VK_NEXT, "pagedown"},
  {VK_END, "end"},
  {VK_HOME, "home"},
  {VK_LEFT, "left"},
  {VK_UP, "up"},
  {VK_RIGHT, "right"},
  {VK_DOWN, "down"},
  {VK_SELECT, "select"},
  {VK_EXECUTE, "execute"},
  {VK_SNAPSHOT, "print"},
  {VK_SNAPSHOT, "printscreen"},
  {VK_INSERT, "insert"},
  {VK_DELETE, "delete"},
  {VK_HELP, "help"},
  {'0', "0"},
  {'1', "1"},
  {'2', "2"},
  {'3', "3"},
  {'4', "4"},
  {'5', "5"},
  {'6', "6"},
  {'7', "7"},
  {'8', "8"},
  {'8', "*"}, //Use the "8" key for multiplication also
  {'9', "9"},
  {'A', "a"},
  {'B', "b"},
  {'C', "c"},
  {'D', "d"},
  {'E', "e"},
  {'F', "f"},
  {'G', "g"},
  {'H', "h"},
  {'I', "i"},
  {'J', "j"},
  {'K', "k"},
  {'L', "l"},
  {'M', "m"},
  {'N', "n"},
  {'O', "o"},
  {'P', "p"},
  {'Q', "q"},
  {'R', "r"},
  {'S', "s"},
  {'T', "t"},
  {'U', "u"},
  {'V', "v"},
  {'W', "w"},
  {'X', "x"},
  {'Y', "y"},
  {'Z', "z"},
  {VK_LWIN, "lwin"},
  {VK_RWIN, "rwin"},
  {VK_APPS, "apps"},
  {VK_NUMPAD0, "num0"},
  {VK_NUMPAD1, "num1"},
  {VK_NUMPAD2, "num2"},
  {VK_NUMPAD3, "num3"},
  {VK_NUMPAD4, "num4"},
  {VK_NUMPAD5, "num5"},
  {VK_NUMPAD6, "num6"},
  {VK_NUMPAD7, "num7"},
  {VK_NUMPAD8, "num8"},
  {VK_NUMPAD9, "num9"},
  {VK_MULTIPLY, "num*"},
  {VK_ADD, "num+"},
  {VK_SUBTRACT, "num-"},
  {VK_DECIMAL, "num."},
  {VK_DIVIDE, "num/"},
  {VK_F1, "f1"},
  {VK_F2, "f2"},
  {VK_F3, "f3"},
  {VK_F4, "f4"},
  {VK_F5, "f5"},
  {VK_F6, "f6"},
  {VK_F7, "f7"},
  {VK_F8, "f8"},
  {VK_F9, "f9"},
  {VK_F10, "f10"},
  {VK_F11, "f11"},
  {VK_F12, "f12"},
  {VK_F13, "f13"},
  {VK_F14, "f14"},
  {VK_F15, "f15"},
  {VK_F16, "f16"},
  {VK_F17, "f17"},
  {VK_F18, "f18"},
  {VK_F19, "f19"},
  {VK_F20, "f20"},
  {VK_F21, "f21"},
  {VK_F22, "f22"},
  {VK_F23, "f23"},
  {VK_F24, "f24"},
  {VK_NUMLOCK, "numlock"},
  {VK_SCROLL, "scrolllock"},

  {VK_BACK_QUOTE,"~"},
  {VK_BACK_SLASH,"\\"},
  {VK_BACK_SLASH,"|"},
  
  {VK_CLOSE_BRACKET,"]"},
  {VK_OPEN_BRACKET,"["},

  {VK_CLOSE_BRACKET,"}"},
  {VK_OPEN_BRACKET,"{"},

  {VK_COMMA,","},
  {VK_COMMA,"<"},

  {VK_PERIOD,"."},
  {VK_PERIOD,">"},

  {VK_EQUALS,"="},
  {VK_EQUALS,"+"},

  {VK_MINUS,"_"},
  {VK_MINUS,"-"},

  {VK_NUM_LOCK,"numlock"},
    
  {VK_QUOTE,"\""},
  {VK_QUOTE,"'"},

  {VK_SCROLL_LOCK,"scrolllock"},
  {VK_SEMICOLON,":"},
  {VK_SEMICOLON,";"},

  {VK_SLASH,"/"},
  {VK_SLASH,"?"}
};

///////////////////////////////////////////////////////////////////////////////
//
InputUtils::InputUtils()
{
  
}

///////////////////////////////////////////////////////////////////////////////
//
bool InputUtils::IsKeyDown(uint keyCode) const
{
  //Return if the high byte is true (ie key is down)
  return (GetAsyncKeyState(keyCode) & 0x8000) != 0;
}


#endif // GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h> 

//Key code lookups for linux
KeyLookup keyLookup[] = 
{
  {XK_BackSpace, "backspace"},
  {XK_Tab, "tab"},
  {XK_Clear, "clear"},
  {XK_Return, "enter"},

  // No default shift/ctrl/alt so use left keys
  {XK_Shift_L, "shift"},
  {XK_Control_L, "ctrl"},
  {XK_Alt_L, "alt"},      

  {XK_Shift_L, "lshift"},
  {XK_Control_L, "lctrl"},
  {XK_Alt_L, "lalt"},      

  {XK_Shift_R, "rshift"},
  {XK_Control_R, "rctrl"},
  {XK_Alt_R, "ralt"},      

  {XK_Pause, "pause"},
  {XK_Caps_Lock, "caps"}, 
  {XK_Caps_Lock, "capslock"},
  {XK_Escape, "esc"},
  {XK_space, "space"},
  {XK_Page_Up, "pageup"},
  {XK_Page_Down, "pagedown"},
  {XK_End, "end"},
  {XK_End, "home"},
  {XK_Left, "left"},
  {XK_Up, "up"},
  {XK_Right, "right"},
  {XK_Down, "down"},
  {XK_Select, "select"},
  {XK_Execute, "execute"},
  {XK_Print, "print"},
  {XK_Print, "printscreen"},
  {XK_Insert, "insert"},
  {XK_Delete, "delete"},
  {XK_Help, "help"},
  {'0', "0"},
  {'1', "1"},
  {'2', "2"},
  {'3', "3"},
  {'4', "4"},
  {'5', "5"},
  {'6', "6"},
  {'7', "7"},
  {'8', "8"},
  {'8', "*"}, //Use the "8" key for multiplication also
  {'9', "9"},
  {'A', "a"},
  {'B', "b"},
  {'C', "c"},
  {'D', "d"},
  {'E', "e"},
  {'F', "f"},
  {'G', "g"},
  {'H', "h"},
  {'I', "i"},
  {'J', "j"},
  {'K', "k"},
  {'L', "l"},
  {'M', "m"},
  {'N', "n"},
  {'O', "o"},
  {'P', "p"},
  {'Q', "q"},
  {'R', "r"},
  {'S', "s"},
  {'T', "t"},
  {'U', "u"},
  {'V', "v"},
  {'W', "w"},
  {'X', "x"},
  {'Y', "y"},
  {'Z', "z"},
  {XK_Super_L, "lwin"},
  {XK_Super_R, "rwin"},
  {XK_KP_0, "num0"},
  {XK_KP_1, "num1"},
  {XK_KP_2, "num2"},
  {XK_KP_3, "num3"},
  {XK_KP_4, "num4"},
  {XK_KP_5, "num5"},
  {XK_KP_6, "num6"},
  {XK_KP_7, "num7"},
  {XK_KP_8, "num8"},
  {XK_KP_9, "num9"},
  {XK_KP_Multiply, "num*"},
  {XK_KP_Add, "num+"},
  {XK_KP_Subtract, "num-"},
  {XK_KP_Decimal, "num."},
  {XK_KP_Divide, "num/"},
  {XK_F1, "f1"},
  {XK_F2, "f2"},
  {XK_F3, "f3"},
  {XK_F4, "f4"},
  {XK_F5, "f5"},
  {XK_F6, "f6"},
  {XK_F7, "f7"},
  {XK_F8, "f8"},
  {XK_F9, "f9"},
  {XK_F10, "f10"},
  {XK_F11, "f11"},
  {XK_F12, "f12"},
  {XK_F13, "f13"},
  {XK_F14, "f14"},
  {XK_F15, "f15"},
  {XK_F16, "f16"},
  {XK_F17, "f17"},
  {XK_F18, "f18"},
  {XK_F19, "f19"},
  {XK_F20, "f20"},
  {XK_F21, "f21"},
  {XK_F22, "f22"},
  {XK_F23, "f23"},
  {XK_F24, "f24"},
  {XK_Num_Lock, "numlock"},
  {XK_Scroll_Lock, "scrolllock"},

  {XK_quoteleft,"~"},
  {XK_backslash,"\\"},
  {XK_backslash,"|"},
  
  {XK_braceleft,"]"},
  {XK_braceright,"["},

  {XK_bracketleft,"}"},
  {XK_bracketright,"{"},

  {XK_comma,","},
  {XK_comma,"<"},

  {XK_period,"."},
  {XK_period,">"},

  {XK_equal,"="},
  {XK_equal,"+"},

  {XK_minus,"_"},
  {XK_minus,"-"},

  {XK_Num_Lock,"numlock"},
    
  {XK_quotedbl,"\""},
  {XK_quotedbl,"'"},

  {XK_Scroll_Lock,"scrolllock"},
  {XK_colon,":"},
  {XK_semicolon,";"},

  {XK_slash,"/"},
  {XK_question,"?"}
};

///////////////////////////////////////////////////////////////////////////////
//
InputUtils::InputUtils():
xDisplay(NULL)
{
  
}

///////////////////////////////////////////////////////////////////////////////
//
bool InputUtils::Update(Display *display)
{
  //Assign the X-Display
  xDisplay = display;  

  //Update the bit array of what keys are down  
  if(display != NULL)
  {
    XQueryKeymap(xDisplay,keyState);  
    return true;
  }
  
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InputUtils::IsKeyDown(uint keyCode) const
{
  //Return now if there is no display
  if(!xDisplay)
  {
    return false;  
  }
  
  KeyCode deviceCode = XKeysymToKeycode(xDisplay, keyCode);

  //If the device code was NULL, return now  
  if(deviceCode == NoSymbol)
  {
    return false;
  }

  return ((keyState[deviceCode >> 3] & (1 << (deviceCode & 0x07))) > 0);
}


#endif //GLI_BUILD_LINUX


///////////////////////////////////////////////////////////////////////////////
//
bool InputUtils::IsAllKeyDown(const vector<uint> &keyCodes) const
{
  //Loop for all the codes
  for(uint i=0;i<keyCodes.size();i++)
  {
    //If one of the keys is not down, return now
    if(!IsKeyDown(keyCodes[i]))
    {
      return false;
    }
  }

  //Only return true when there exists some keys to test
  if(keyCodes.size() > 0)
  {
    return true;
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
uint InputUtils::GetKeyCode(const string & keyString)
{
  //Make the string lower case
  string lookUpStr = keyString;

  //Loop for all key lookups
  for(uint i=0;i < sizeof(keyLookup) / sizeof(KeyLookup); i++)
  {
    //If the string matches, return the code
    if(keyLookup[i].keyStr == lookUpStr)
    {
      return keyLookup[i].keyCode;
    }
  }

  return 0;
}


