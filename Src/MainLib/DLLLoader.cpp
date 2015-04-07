/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "DLLLoader.h"
 
///////////////////////////////////////////////////////////////////////////////
#ifdef GLI_BUILD_WINDOWS
///////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <winbase.h>

///////////////////////////////////////////////////////////////////////////////
//
DLLLoader::DLLLoader():
handle(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////
//
DLLLoader::~DLLLoader()
{
  //Free the library if it existed
  if(handle)
  {
    FreeLibrary((HMODULE)handle);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool DLLLoader::Init(const char *libName)
{
  //Open the library
  handle = (void *)LoadLibrary(libName);

  //Abort on failure
  if(!handle)
  {
    return false;
  }

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
void * DLLLoader::GetFunction(const char *functionName)
{
  return GetProcAddress((HMODULE)handle, functionName);
}



///////////////////////////////////////////////////////////////////////////////
//
string DLLLoader::GetFullDLLFileName() const
{
  //If no handle, return now
  if(handle == NULL)
  {
    return "";
  }

  char buffer[1025];

  //Append a NULL character to be safe
  buffer[1024] = '\0'; 

  //Get the string
  if(GetModuleFileName((HMODULE)handle,buffer,1024) == 0)
  {
    return "";
  }

  return string(buffer);
}

///////////////////////////////////////////////////////////////////////////////
//
bool DLLLoader::IsDLLNameMatch(const char *libName) const
{
  //If no handle, return now
  if(handle == NULL)
  {
    return false;
  }

  //Compare if the resulting handles are equal
  if(handle == GetModuleHandle(libName))
  {
    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
void DLLLoader::FlagDLLUnloaded()
{
  //Flag that the handle is now invalid
  handle = NULL;
}

#endif //GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
#ifdef GLI_BUILD_LINUX
///////////////////////////////////////////////////////////////////////////////

#include <dlfcn.h>

///////////////////////////////////////////////////////////////////////////////
//
DLLLoader::DLLLoader():
handle(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////
//
DLLLoader::~DLLLoader()
{
  //Free the library if it existed
  if(handle)
  {
    dlclose(handle);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool DLLLoader::Init(const char *libName)
{
  //Open the library
  handle = dlopen(libName, RTLD_LAZY | RTLD_LOCAL);

  //Abort on failure
  if(!handle)
  {
    return false;
  }

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//

extern "C" void * __libc_dlsym(void* handle, const char* symbol);
void * DLLLoader::GetFunction(const char *functionName)
{
  // Now that dlsym is substituted by our own version, we need to use 
  // libc private version of the function for our own purposes
  // return dlsym(handle, functionName);
  return __libc_dlsym(handle, functionName);
}



///////////////////////////////////////////////////////////////////////////////
//
string DLLLoader::GetFullDLLFileName() const
{
  //If no handle, return now
  if(handle == NULL)
  {
    return "";
  }

  char buffer[1025];

  //Append a NULL character to be safe
  buffer[1024] = '\0'; 

  // DT_TODO: Use dlsym /dladdr
  //Get the string
  //if(GetModuleFileName((HMODULE)handle,buffer,1024) == 0)
  {
    return "";
  }

  return string(buffer);
}

///////////////////////////////////////////////////////////////////////////////
//
bool DLLLoader::IsDLLNameMatch(const char *libName) const
{
  //If no handle, return now
  if(handle == NULL)
  {
    return false;
  } 
/* DT_TODO: Open lib and compare handle
  //Compare if the resulting handles are equal
  if(handle == GetModuleHandle(libName))
  {
    return true;
  }
*/
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
void DLLLoader::FlagDLLUnloaded()
{
  //Flag that the handle is now invalid
  handle = NULL;
}

#endif //GLI_BUILD_LINUX

