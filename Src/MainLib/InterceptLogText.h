/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_LOG_TEXT_H_
#define __INTERCEPT_LOG_TEXT_H_

#include "GLInterceptConfig.h"
#include <stdio.h>
#include <string>
#include "ErrorLog.h"
#include "FunctionData.h"
#include "FunctionTable.h"
#include "ConfigData.h"
#include "InterceptLog.h"

using namespace std;

//@
//  Summary:
//    This class provides basic text output for logging of OpenGL functions.
//  
class InterceptLogText : public InterceptLog 
{
public:

  //@
  //  Summary:
  //    Constructor to set up the log file. The default loggig is to stdout.
  //  
  //  Parameters:
  //    fileName - A file to open and use for logging.
  //
  //    functionTable - The table of all functions that are known/are being logged.
  //
  //    configData    - The configuration options for logging.
  //
  InterceptLogText(const string &fileName,FunctionTable * functionTable,const ConfigData &configData);

  //@
  //  Summary:
  //    Destructor, completes the log.
  //  
  virtual ~InterceptLogText();

  //@
  //  Summary:
  //    To initialise the log for writing. (ie. writes headers etc)
  //
  //  Returns:
  //    True is returned on init success, false if not.
  //
  virtual bool Init();

  //@
  //  Summary:
  //    To log the passed function and function data 
  //    (Before the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args);

  //@
  //  Summary:
  //    To perform any post-call logging of a function.
  //    (After the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value (if any).
  //
  virtual void LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    To log that a GL error has occured.
  //  
  //  Parameters:
  //    errorCode - The GL error code to log.
  //
  virtual void LogFunctionError(uint errorCode);

  //@
  //  Summary:
  //    To log that the current function has completed. 
  //    (ie. All data for the previous function has been processed)
  //  
  virtual void LogFunctionComplete();

  //@
  //  Summary:
  //    To add a raw string to the output log.
  //  
  //  Parameters:
  //    addString  - The raw string to add to the log.
  //
  virtual void AddLogString(const char* addString);

protected:

  string logFileName;                             // The file name used in the log
  FILE * logFile;                                 // The log file used
  bool   logFlush;                                // The log file is flushed after each "Pre" call 

  bool  shaderRenderCallLog;                      // Flag to indicate if the logger logs shader states on render calls
  bool  imageRenderCallLog;                       // Flag to indicate if the logger logs image/texture states on render calls

  bool  functionTimerEnabled;                     // Flag to indicate if function time logging is enabled
  uint  functionTimerCutOff;                      // The cutoff value in microseconds when logging function times

};



#endif // !__INTERCEPT_LOG_TEXT_H_
