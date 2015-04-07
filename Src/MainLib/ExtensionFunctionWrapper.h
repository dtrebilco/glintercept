/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2011  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __EXTENSION_FUNCTION_WRAPPER_H__
#define __EXTENSION_FUNCTION_WRAPPER_H__

#include "GLInterceptConfig.h"
#include "FunctionTable.h"
#include <string>

using namespace std;

//@
//  Summary:
//    This class wraps and stores manual extension functions that have been loaded
//    via OpenGL. This is necessary for rare OpenGL functions that cannot use the 
//    automatic wrapping provided by extension functiuon stubs.
//  
class ExtensionFunctionWrapper  
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
  ExtensionFunctionWrapper(FunctionTable * functionTable);
  virtual ~ExtensionFunctionWrapper();

  //@
  //  Summary:
  //    Get if the specified function name is a manual override
  //  
  //  Parameters: 
  //    funcName  - The name of the function.
  //
  //  Returns:
  //    Returns true for a manual wrap function name.
  //
  static bool IsManualWrapFunction(const string & funcName);

  //@
  //  Summary:
  //    Add a manual wrapping to the specified function name
  //  
  //  Parameters: 
  //    funcName  - The name of the function to add.
  //
  //    functionPtr - The function pointer.
  //
  //  Returns:
  //    Returns the function pointer to be used when calling the function 
  //    (instead of the passed function pointer) or NULL if not a valid 
  //    wrap function name.
  //
  void * AddFunctionWrap(const string & funcName,void * functionPtr);

protected:

  FunctionTable * functionTable;                  //The current function table pointer

};

#endif // __EXTENSION_FUNCTION_WRAPPER_H__
