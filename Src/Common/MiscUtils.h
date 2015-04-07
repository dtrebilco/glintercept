/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __MISC_UTILS_H__
#define __MISC_UTILS_H__

#include <string>
#include <vector>
#include "OSDefines.h"

using namespace std;

//Compile time Assert macro
#define	CASSERT(x,error) typedef char __CASSERT__##error[1]; typedef char __CASSERT__##error[(x)?1:2]

//@
//  Summary:
//    To write to the passed destination string using the passed "printf"
//    formatting and arguments. This method currently only allows resulting
//    strings of 1024 characters in length.
//  
//  Parameters:
//    dstString  - The string to store the passed formatting string and 
//                 arguments in.
//    
//    format     - The "printf" style formatting string. Following arguments
//                 are specified in this string
//
void GLI_CDECL StringPrintF(string &dstString, const char *format,...)
                                             __attribute__ ((format(printf, 2, 3)));

//@
//  Summary:
//    To get the activer thread ID.
// 
//  Returns:
//    The ID of the thread that called this function is returned.
//
uintptr_t GetActiveThreadID();

//@
//  Summary:
//    Simple class to perform interval timing calculations.
//  
class TimeDiff
{
public:

  //@
  //  Summary:
  //    Constructor. Inits timing variables.
  // 
  TimeDiff();

  //@
  //  Summary:
  //    Destructor, shuts down the timer.
  // 
  virtual ~TimeDiff();

  //@
  //  Summary:
  //    Sets the initial start time of the timer. 
  // 
  void StartTimer();

  //@
  //  Summary:
  //    To get the number of microseconds that have elapsed since 
  //    that last call to StartTimer.
  // 
  //  Returns:
  //    The number of microseconds that have elapsed is returned.
  //    Zero is returned if the timer could not be init. 
  //    (or the interval was less than 1 microsecond)
  //
  uint GetTimeDiff();

private:

#ifdef GLI_BUILD_WINDOWS

  int64 frequency;                                //The frequency of the timer
  int64 startTime;                                //The start time of the timer
  
#endif //GLI_BUILD_WINDOWS  
#ifdef GLI_BUILD_LINUX

  struct timeval startTime;                       //The starting time value 
  
#endif //GLI_BUILD_LINUX  


};



#endif //__MISC_UTILS_H__
