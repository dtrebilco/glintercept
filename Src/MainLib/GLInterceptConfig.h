/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __GL_INTERCEPT_CONFIG_H_
#define __GL_INTERCEPT_CONFIG_H_


//Config switches go here
#define __GLI_DEBUG

//Build version
#define __GLI_BUILD_VER      1.32
#define __GLI_BUILD_VER_STR "1.32"

 
//Misc type defines
typedef unsigned int uint;

#include "CommonErrorLog.h"
#include "FunctionArgs.h"
#include "FunctionRetValue.h"
#include <MiscUtils.h>
#include <OSDefines.h>

CASSERT(sizeof(udword) == 4, __udword_not_32_bit__);
CASSERT(sizeof(int64) == 8, __int64_type__);
CASSERT(sizeof(uint64) == 8, __uint64_type__);

#endif // __GL_INTERCEPT_CONFIG_H_
