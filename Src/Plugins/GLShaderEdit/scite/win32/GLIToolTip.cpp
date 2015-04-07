
#include "GLIToolTip.h"
#include "SciTE.h"
#include "Scintilla.h"

#include <algorithm>

using namespace std;

HWND  dialogWnd   = NULL;
HWND  toolTipWnd  = NULL;
HHOOK toolTipHook = NULL;

//Callbacks
BOOL CALLBACK EnumChildProc(HWND hwndCtrl, LPARAM lParam); 
LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);


//NOTE: Most of this code comes from a MSDN article

///////////////////////////////////////////////////////////////////////////////
//
GLIToolTip::GLIToolTip(HWND newDialogWnd, HINSTANCE hInstance)
{
  if(toolTipWnd || toolTipHook || dialogWnd || !newDialogWnd)
  {
    return;
  }

  //Assign the dialog window
  dialogWnd = newDialogWnd;

  // Create a tooltip window control. 
  toolTipWnd = CreateWindowEx(0, TOOLTIPS_CLASS, (LPSTR) NULL, 
                              TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                              CW_USEDEFAULT, dialogWnd, (HMENU) NULL, hInstance, NULL); 
  if (!toolTipWnd) 
  {
    return; 
  }

  // Enumerate the child windows to register them with the tooltip control. 
  if (!EnumChildWindows(dialogWnd, EnumChildProc, 0))
  {
    return;
  }

  // Install a hook procedure to monitor the message stream for mouse 
  // messages intended for the controls in the dialog box. 
  toolTipHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, (HINSTANCE) NULL, GetCurrentThreadId()); 
  if (!toolTipHook) 
  {
    return; 
  }
}

///////////////////////////////////////////////////////////////////////////////
//
GLIToolTip::~GLIToolTip()
{
  if(toolTipHook)
  {
    UnhookWindowsHookEx(toolTipHook);
    toolTipHook = NULL;
  }

  //Destroy the tool tip window
  if(toolTipWnd)
  {
    DestroyWindow(toolTipWnd);
    toolTipWnd = NULL;
  }
}
 
///////////////////////////////////////////////////////////////////////////////
//
BOOL CALLBACK EnumChildProc(HWND hwndCtrl, LPARAM lParam) 
{ 
  TOOLINFO ti; 
  char szClass[64]; 

  // Skip static controls. 
  GetClassName(hwndCtrl, szClass, sizeof(szClass)); 
  if (lstrcmpi(szClass, "STATIC"))
  { 
    ti.cbSize = sizeof(TOOLINFO); 
    ti.uFlags = TTF_IDISHWND; 
    ti.hwnd = dialogWnd; 
    ti.uId = (UINT) hwndCtrl; 
    ti.hinst = 0; 
    ti.lpszText = LPSTR_TEXTCALLBACK; 
    SendMessage(toolTipWnd, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti); 
  } 
  return TRUE; 
} 
 
 
///////////////////////////////////////////////////////////////////////////////
//
LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
  MSG *lpmsg; 

  lpmsg = (MSG *) lParam; 
  if (nCode < 0 || !(IsChild(dialogWnd, lpmsg->hwnd))) 
  {
    return (CallNextHookEx(toolTipHook, nCode, wParam, lParam)); 
  }

  switch (lpmsg->message)
  { 
      case WM_MOUSEMOVE: 
      case WM_LBUTTONDOWN: 
      case WM_LBUTTONUP: 
      case WM_RBUTTONDOWN: 
      case WM_RBUTTONUP: 
        if (toolTipWnd != NULL)
        { 
          MSG msg; 

          msg.lParam = lpmsg->lParam; 
          msg.wParam = lpmsg->wParam; 
          msg.message = lpmsg->message; 
          msg.hwnd = lpmsg->hwnd; 
          SendMessage(toolTipWnd, TTM_RELAYEVENT, 0, (LPARAM) (LPMSG) &msg); 
        } 
        break; 
      default: 
        break; 
  } 

  return (CallNextHookEx(toolTipHook, nCode, wParam, lParam)); 
} 
 


