/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __OS_DEFINES_H_
#define __OS_DEFINES_H_

//Typedef the 32 bit type (unsigned double word)
typedef unsigned int udword;

///////////////////////////////////////////////////////////////////////////////
//Windows defines
///////////////////////////////////////////////////////////////////////////////
#ifdef GLI_BUILD_WINDOWS

//Import/export symbols
#ifdef GLINTERCEPT_EXPORTS
#define GLINTERCEPT_API __declspec(dllexport)
#else
#define GLINTERCEPT_API __declspec(dllimport)
#endif

#define HIDDEN
#define QUICK_CONSTRUCT

#define GLI_STDCALL __stdcall
#define GLI_CDECL   __cdecl
#define __attribute__(X) 

typedef unsigned int uint;
typedef signed   __int64  int64;
typedef unsigned __int64 uint64;

#define _WIN32_WINDOWS 0x0410  // Assume at least win98 support
#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers

#ifdef _MSC_VER
#pragma warning (disable:4786) // No complaints about debug symbol length
#pragma warning (disable:4996) // Allow using unsafe functions (eg. fopen)
#endif // _MSC_VER

#include <windows.h>
// Detect architecture
#ifdef _WIN64
  #define OS_ARCH_x64
//  _WIN32 is defined on both x64 and x86 ...
#elif defined(_WIN32)
  #define OS_ARCH_x86
#endif

#endif //GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//Linux defines
///////////////////////////////////////////////////////////////////////////////
#ifdef GLI_BUILD_LINUX
#include <stdint.h>

#define GLINTERCEPT_API
#define HIDDEN __attribute__((visibility("hidden")))
#define QUICK_CONSTRUCT __attribute__((init_priority(101)))

#define GLI_STDCALL 
#define GLI_CDECL   

typedef unsigned  int  uint;
typedef long long int  int64;
typedef unsigned long long uint64;

// Detect architecture
#ifdef __i386__
  #define OS_ARCH_x86
#endif
#if defined(__amd64) || defined(__x86_64) || defined(__ia64_)
  #define OS_ARCH_x64
#endif

#define HGLRC       GLXContext

#include <X11/Xlib.h>
#include <X11/Xutil.h>
typedef struct __GLXcontextRec *GLXContext;
typedef struct __GLXFBConfigRec *GLXFBConfig;
typedef XID GLXDrawable;
typedef XID GLXPixmap;
typedef XID GLXWindow;
typedef XID GLXPbuffer;
typedef XID GLXContextID;

#endif //GLI_BUILD_LINUX

#if !( defined(GLI_BUILD_LINUX) || defined(GLI_BUILD_WINDOWS) )
  #error Only Windows and Linux platforms supported.
#endif

#if !( defined(OS_ARCH_x86) || defined(OS_ARCH_x64) )
  #error Only x86 and x64 architectures supported.
#endif

#endif // __OS_DEFINES_H_
