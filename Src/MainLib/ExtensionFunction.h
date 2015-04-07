/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __EXTENSION_FUNCTION_H__
#define __EXTENSION_FUNCTION_H__

#include "GLInterceptConfig.h"
#include "FunctionTable.h"
#include "ExtensionFunctionWrapper.h"
#include <string>

using namespace std;

//@
//  Summary:
//    This class wraps and stores extension functions that have been loaded
//    via OpenGL. The functions wrapping is not currently thread safe and is
//    platform dependant.
//  
class ExtensionFunction  
{
public:

  //@
  //  Summary:
  //    Constructor
  //  
  //  Parameters:
  //    functionTable  - The function table where all extension functions 
  //                     are registered. Must be valid for the duration of 
  //                     this class.
  //
  ExtensionFunction(FunctionTable * functionTable);
  virtual ~ExtensionFunction();

  //@
  //  Summary:
  //    Adds a new function of the specified name and pointer.
  //    The passed function is wrapped and a new pointer is returned. If the
  //    number of wrapper functions is exceeded, the passed function pointer
  //    is returned.
  //  
  //  Parameters: 
  //    funcName  - The name of the function to add.
  //
  //    functionPtr - The function pointer.
  //
  //  Returns:
  //    Returns the function pointer to be used when calling the function 
  //    (instead of the passed function pointer)
  //
  void * AddFunction(const string & funcName,void * functionPtr);

  //@
  //  Summary:
  //    Adds a override function of the specified name. 
  //    If the function has already been overridden or the number of wrapper
  //    functions is exceeded, false is returned.
  //  
  //  Parameters: 
  //    funcName  - The name of the function to override.
  //
  //    overrideFunctionPtr - The function pointer of the override method.
  // 
  //    origFuncPtr - The "real" OpenGL function being overridden. Can be 
  //                  NULL if the function does not exist in OpenGL.
  //
  //  Returns:
  //    Returns true if the override was successful, false on failure.
  //
  bool AddOverrideFunction(const string & funcName, void * overrideFunctionPtr, void *origFuncPtr);

protected:

  uint            currExtensionIndex;             //The index of the next extension to be added
  FunctionTable * functionTable;                  //The current function table pointer

  ExtensionFunctionWrapper manualFuncWrap;        //The manual function wrapper (for difficult functions)

};

#endif // __EXTENSION_FUNCTION_H__
