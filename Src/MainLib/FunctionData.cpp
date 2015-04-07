/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/

#include "FunctionData.h"

#ifdef GLI_BUILD_WINDOWS
static const char * s_osStartNameTag = "wgl";
#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX
static const char * s_osStartNameTag = "glX";
#endif //GLI_BUILD_LINUX


///////////////////////////////////////////////////////////////////////////////
//
FunctionData::FunctionData(const string & a_functionName):
functionName(a_functionName),
wrappedFunctionPtr(NULL),
origionalInternalPtr(NULL),
internalFunctionDataPtr(NULL),
functionFlags(0)
{
  // Flag the function as an OS method if necessary
  if(a_functionName.find(s_osStartNameTag) == 0)
  {
    functionFlags |= FDF_OS_FUNC;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
FunctionData::~FunctionData()
{

}
