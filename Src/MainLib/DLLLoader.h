/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __DLL_LOADER_H_
#define __DLL_LOADER_H_

#include "GLInterceptConfig.h"

//@
//  Summary:
//    Class for loading a library and accessing functions from it
//  
class DLLLoader  
{
public:

  //Constructor and destructor
  DLLLoader();
  virtual ~DLLLoader();

  //@
  //  Summary:
  //    Inits the class with the library of the passed name
  //  
  //  Parameters:
  //    libName  - The name of the library to open
  //
  //  Returns;
  //    True is returned on success, false is otherwise
  //
  bool Init(const char *libName);

  //@
  //  Summary:
  //    Gets the specified funtion from the library.
  //  
  //  Parameters:
  //    functionName  - The name of the function to get from the library.
  //
  //  Returns;
  //    If the function exists in the library and it could be retrieved, it is returned.
  //    Else, NULL is returned.
  //
  void * GetFunction(const char *functionName);

  //@
  //  Summary:
  //    Gets the full dll file name with path.
  //  
  //  Returns;
  //    If the DLL is loaded, the full load name of the DLL is returned.
  //    Else an empty string is returned.
  //
  string GetFullDLLFileName() const;

  //@
  //  Summary:
  //    To determine if the passed dll library name matches this loaded DLL.
  //    (This will resolve the paths for the dll name before comparing against
  //    this loaded dll. Therefore this method is better for comparasons than 
  //    using GetFullDLLFileName and comparing the result)
  //  
  //  Parameters:
  //    libName  - The dll lib name to compare against. 
  //               (Will be resolved to full path first).
  //
  //  Returns;
  //    If the passed libName corresponds to this dll, true is returned. 
  //    Else false is returned.
  //
  bool IsDLLNameMatch(const char *libName) const;

  //@
  //  Summary:
  //    To determine if the passed dll handle matches the contained handle.
  //  
  //  Parameters:
  //    testHandle - The handle to test against.
  //
  //  Returns;
  //    If the passed handle corresponds to this dll, true is returned. 
  //    Else false is returned.
  //
  inline bool IsDLLHandleMatch(void *testHandle) const;

  //@
  //  Summary:
  //    If this DLL is unloaded externally (ie by the OS), setting this flag
  //    will tell the class to not to try and unload the dll on destruction.
  //  
  void FlagDLLUnloaded();


protected:

  void * handle;                                  // The OS handle to the library
};


///////////////////////////////////////////////////////////////////////////////
//
inline bool DLLLoader::IsDLLHandleMatch(void *testHandle) const
{
  //Compare the handles
  if(handle == testHandle)
  {
    return true;
  }

  return false;
}


#endif // __DLL_LOADER_H_
