#ifndef __ERROR_LOG_H_
#define __ERROR_LOG_H_

#include <stdio.h>
#include <OSDefines.h>

//@
//  Summary:
//    Simple class to log errors and messages
//  
class ErrorLog  
{
public:

  //@
  //  Summary:
  //    Constructor to set up the log file. The default loggig is to stderr.
  //  
  //  Parameters:
  //    file     - A specific file to use when logging.
  //
  //    fileName - A file to open and use for logging.
  //
  ErrorLog();
  ErrorLog(FILE *file);
  ErrorLog(const char *fileName);

  //@
  //  Summary:
  //    Destructor, closes any opened files.
  //  
  virtual ~ErrorLog();

  //@
  //  Summary:
  //    To log a error message.
  //  
  //  Parameters:
  //    message  - The message to log. (printf type format string)
  //
  //    ...      - [Optional] Parameters for the above format string.
  //
  //    args     - [Optional] Parameters for the above format string.
  //               The naming of this method is different as to not be 
  //               confused with the variable argument version.
  //
  void GLI_CDECL LogError(const char *message,...) 
                          __attribute__ ((format(printf, 2, 3)));
  void LogErrorArgs(const char *message,va_list args);  

  //@
  //  Summary:
  //    To enable/disbale the log
  //  
  //  Parameters:
  //    enable  - Flag to enable and disable the log
  //
  inline void SetLogEnabled(bool enable);

  //@
  //  Summary:
  //    To enable/disbale log flushing (default to off)
  //  
  //  Parameters:
  //    enable  - Flag to enable and disable the log flushing
  //
  inline void SetLogFlush(bool enable);

  //@
  //  Summary:
  //    To determine if a debugger is attached to this application.
  //  
  //  Returns:
  //    True is returned if a debugger exists, else false is returned.
  //
  static bool IsDebuggerActive();

  //@
  //  Summary:
  //    To issue a debugger break point. If no debugger is attached, 
  //    this function just returns.
  //  
  static void IssueDebuggerBreak();

  //@
  //  Summary:
  //    To log a message to the attached debugger. If no debugger is 
  //    attached, no message is logged.
  //  
  //  Parameters:
  //    message  - The message to log. (printf type format string)
  //
  //    ...      - [Optional] Parameters for the above format string.
  //
  //    args     - [Optional] Parameters for the above format string.
  //               The naming of this method is different as to not be 
  //               confused with the variable argument version.
  //
  static void GLI_CDECL LogDebuggerMessage(const char *message,...)
                                           __attribute__ ((format(printf, 1, 2)));
  static void LogDebuggerMessageArgs(const char *message,va_list args);

  //@
  //  Summary:
  //    To set if the out-put of the error log is mirrored to the debugger.
  //  
  //  Parameters:
  //    enable  - Flag to enable and disable the log mirroring.
  //
  void SetDebuggerLogEnabled(bool enable);


protected:

  bool   logEnabled;                              //Flag to indicate if the log is enabled or not
  FILE * logFile;                                 //The log file used to report messages on

  bool   fileOpened;                              //Flag to indicate if this class opened the file
  bool   fileFlush;                               //Flag to indicate if we flush the file buffer after each message

  bool   debuggerLogEnabled;                      //Flag to indicate if the logs contents are mirrored to an attached debugger
};

///////////////////////////////////////////////////////////////////////////////
//
inline void ErrorLog::SetLogEnabled(bool enable)
{
  logEnabled = enable;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ErrorLog::SetLogFlush(bool enable)
{
  fileFlush = enable;
}


#endif // __ERROR_LOG_H_
