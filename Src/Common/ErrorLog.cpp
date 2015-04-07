

#include <OSDefines.h>
#include "ErrorLog.h"
#include <stdarg.h>

#define MAX_DEBUG_BUFFER 1024

///////////////////////////////////////////////////////////////////////////////
//
ErrorLog::ErrorLog():
logEnabled(true),
logFile(stderr),
fileOpened(false),
fileFlush(false),
debuggerLogEnabled(false)
{
}

///////////////////////////////////////////////////////////////////////////////
//
ErrorLog::ErrorLog(FILE *file):
logEnabled(true),
logFile(file),
fileOpened(false),
fileFlush(false),
debuggerLogEnabled(false)
{
  //Prevent NULL file settings
  if(logFile == NULL)
  {
    logFile = stderr;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
ErrorLog::ErrorLog(const char *fileName):
logEnabled(true),
logFile(stderr),
fileOpened(false),
fileFlush(false),
debuggerLogEnabled(false)
{
  //Attempt to open the file
  logFile = fopen(fileName,"wt");

  //Prevent NULL file settings
  if(logFile == NULL)
  {
    logFile = stderr;
  }
  else
  {
    //Flag that the file was opened OK
    fileOpened = true;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
ErrorLog::~ErrorLog()
{
  //Close the file if it was opened
  if(fileOpened)
  {
    fclose(logFile);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void GLI_CDECL ErrorLog::LogError(const char *message,...)
{
  //Get the variable list
  va_list marker;
  va_start(marker,message);     
  
  //Call the argument version
  LogErrorArgs(message, marker);

  va_end( marker );              
}

///////////////////////////////////////////////////////////////////////////////
//
void ErrorLog::LogErrorArgs(const char *message,va_list args)
{
  //If the log is not writing, return
  if(!logEnabled)
  {
    return;
  }

  //Write to the log
  vfprintf(logFile,message,args);
  fprintf(logFile,"\n");

  //Flush the log if necessary
  if(fileFlush)
  {
    fflush(logFile);
  }

  //Also write the data to the debugger if we mirror the contents
  if(debuggerLogEnabled)
  {
    //Log the message to the debugger
    LogDebuggerMessageArgs(message,args);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
bool ErrorLog::IsDebuggerActive()
{
#ifdef GLI_BUILD_WINDOWS

  return (IsDebuggerPresent() == TRUE);
  
#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX  

  /* This does not work as child processes do not handle the signals
  //Get the action for the SIGTRAP
  struct sigaction queryAction;  
  if (sigaction (SIGTRAP, NULL, &queryAction) < 0)
  {
    return false;  
  }

  //Check if the signal is handled in the default way or is ignored
  if (queryAction.sa_handler == SIG_DFL ||
      queryAction.sa_handler == SIG_IGN)
  {
    return false;     
  }
  */        

  return true;

#endif //GLI_BUILD_LINUX  
  
}

///////////////////////////////////////////////////////////////////////////////
//
void ErrorLog::IssueDebuggerBreak()
{
#ifdef GLI_BUILD_WINDOWS
  
  //Only issue a break if there is an attached debugger
  if(IsDebuggerActive())
  {
    //Issue break. (could also use __asm {int 3}
    DebugBreak();
  }
  
#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

  //Only issue a break if there is an attached debugger
  if(IsDebuggerActive())
  {
    //Issue a breakpoint (call raise(SIGTRAP) or int 3 here?)
    asm volatile ( "int $0x3" ); 
  }
  
#endif //GLI_BUILD_LINUX  
  
}

///////////////////////////////////////////////////////////////////////////////
//
void GLI_CDECL ErrorLog::LogDebuggerMessage(const char *message,...)
{
  va_list marker;
  va_start(marker,message);     

  //Call the argument version
  LogDebuggerMessageArgs(message,marker);

  va_end( marker );              
}

///////////////////////////////////////////////////////////////////////////////
//
void ErrorLog::LogDebuggerMessageArgs(const char *message,va_list args)
{
  
#ifdef GLI_BUILD_WINDOWS
  
  static char buf[MAX_DEBUG_BUFFER + 10] = {"GLI | "};

  //Write the data to a buffer
  int numChar = _vsnprintf(&buf[6], MAX_DEBUG_BUFFER, message, args);

  //If the entire string could not fit, crop it  
  if(numChar < 0)
  {
    numChar = MAX_DEBUG_BUFFER;
  }

  //Offset by the starting characters
  numChar+=6;

  //Add the return characters
  buf[numChar+0] = '\r';
  buf[numChar+1] = '\n';
  buf[numChar+2] = '\0';

  //Output the debug string
  OutputDebugString(buf);

#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

  //Possibly use syslog() here?
  fprintf(stderr, "GLI | ");
  vfprintf(stderr, message, args);
  fprintf(stderr, "\n");  

#endif //GLI_BUILD_LINUX

}

///////////////////////////////////////////////////////////////////////////////
//
void ErrorLog::SetDebuggerLogEnabled(bool enable)
{
  //Only allow an enable if there is a debugger active
  if(enable && IsDebuggerActive())
  {
    debuggerLogEnabled = true;
  }
  else
  {
    debuggerLogEnabled = false;
  }
}
