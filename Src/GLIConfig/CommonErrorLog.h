/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __COMMON_ERROR_LOG_H_
#define __COMMON_ERROR_LOG_H_

#include <wx/wx.h>

#define USING_ERRORLOG 
#define LOGMSG(string) { wxLogWarning string;}  
#define LOGERR(string) { wxLogError string;}  

#endif // __COMMON_ERROR_LOG_H_
