/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/

#include <OSDefines.h>
#include "MiscUtils.h"
#include <stdarg.h>
#include <stdio.h>

#define MAX_STRING_BUF_LENGTH 1025

#ifdef GLI_BUILD_WINDOWS 
#define vsnprintf _vsnprintf
#endif //GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//
void GLI_CDECL StringPrintF(string &dstString, const char *format,...)
{
  char charBuffer[MAX_STRING_BUF_LENGTH+1];

  //Get the pointer to the arguments
  va_list marker;
  va_start( marker, format );

  //Write the formatted string
  if(vsnprintf(charBuffer,MAX_STRING_BUF_LENGTH,format,marker) == -1)
  {
    //Append a NULL character if the string was too big
    charBuffer[MAX_STRING_BUF_LENGTH-1] = '\0';
  }
 
  //Reset the marker
  va_end( marker );

  //Assign the return string
  dstString = charBuffer;
}


#ifdef GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//
uintptr_t GetActiveThreadID()
{
  //Just dirtecly call the API version
  return GetCurrentThreadId();
}

///////////////////////////////////////////////////////////////////////////////
//
TimeDiff::TimeDiff():
frequency(1),
startTime(0)
{
  //Assign the frequency
  LARGE_INTEGER freq;
  if(QueryPerformanceFrequency(&freq) == TRUE)
  {
    //Assign the frequency of the time values
    frequency = freq.QuadPart;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
TimeDiff::~TimeDiff()
{
}

///////////////////////////////////////////////////////////////////////////////
//
void TimeDiff::StartTimer()
{
  //Get the start time
  LARGE_INTEGER sTime;
  if(QueryPerformanceCounter(&sTime) == TRUE)
  {
    //Assign the starting time
    startTime = sTime.QuadPart;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
uint TimeDiff::GetTimeDiff()
{
  //Get the current time
  LARGE_INTEGER eTime;
  if(QueryPerformanceCounter(&eTime) == TRUE)
  {
    //Subtract from the start time and divide by the frequency
    int64 diffTime = ((eTime.QuadPart - startTime)*1000000) / frequency;

    //Return in microseconds
    if(diffTime >= 0)
    {
      return (uint)(diffTime);
    }
  }

  return 0;
}

#endif //GLI_BUILD_WINDOWS  

#ifdef GLI_BUILD_LINUX

#include <sys/time.h> 

///////////////////////////////////////////////////////////////////////////////
//
uintptr_t GetActiveThreadID()
{
  //Compile time check that the p-thread handle on this platform is an int
  CASSERT(sizeof(pthread_t) == sizeof(uintptr_t),pthreadhandles_not_uintptr_t_size);
  
  return pthread_self();
}

///////////////////////////////////////////////////////////////////////////////
//
TimeDiff::TimeDiff()
{
}

///////////////////////////////////////////////////////////////////////////////
//
TimeDiff::~TimeDiff()
{
}

///////////////////////////////////////////////////////////////////////////////
//
void TimeDiff::StartTimer()
{
  //Get the starting time 
  gettimeofday(&startTime, NULL);
}

///////////////////////////////////////////////////////////////////////////////
//
uint TimeDiff::GetTimeDiff()
{
  //Get the end time 
  struct timeval endTime; 
  if(gettimeofday(&endTime, NULL) != 0)
  {
    return 0; 
  }
 
  //Calculate the difference in milliseconds
  uint64 start = (uint64)startTime.tv_usec;
  uint64 end   = ((uint64)(endTime.tv_sec-startTime.tv_sec) * 1000000) + ((uint64)endTime.tv_usec);  
  return (uint)(end - start);  
}

#endif //GLI_BUILD_LINUX
