/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __COMMON_ERROR_LOG_H_
#define __COMMON_ERROR_LOG_H_

//Defines how logging is handled in GLIntercept
// This file must be defined and be in the include path so the common
// tools know how to log error messages. Other projects only need to define
// the following #defines to use the common tools. (can be empty defines for no logging)
 
#include "ErrorLog.h"
#define USING_ERRORLOG extern ErrorLog *gliLog;
#define LOGMSG(string) { if(gliLog){gliLog->LogError string;}}  
#define LOGERR(string) { if(gliLog){gliLog->LogError string;}}  

#endif // __COMMON_ERROR_LOG_H_
