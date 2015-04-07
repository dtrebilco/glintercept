/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef _BUILT_IN_FUNCTION_H_
#define _BUILT_IN_FUNCTION_H_

#include "GLInterceptConfig.h"
#include "FunctionParamStore.h"

#include <string>

using namespace std;

//The function pre/post for built in functions
// NOTE: This is OS/Platform specific, it assumes how stack parameters are passed
#ifdef OS_ARCH_x86
#define PRE_FUNCTION(name,argList,param) glDriver.LogFunctionPre (BuiltInFunc_##name.GetFunctionIndex(),FunctionArgs((char*)&param));
#endif // OS_ARCH_x86

#ifdef OS_ARCH_x64
#define PRE_FUNCTION(name,argList,param) FunctionParamStore localParamStore = FunctionParamStore argList; glDriver.LogFunctionPre (BuiltInFunc_##name.GetFunctionIndex(),FunctionArgs((char *)&localParamStore.m_paramStore[0]));
#endif // OS_ARCH_x64

#ifdef GLI_BUILD_LINUX
// Perhaps make use of this Linux code?
// Taking address of first parameter on gcc in debug fails in case when first parameter is of type double.
// For this type gcc makes additional copy of parameter (for some reason), and taking address of the argument returns address of
// that copy, not address of variable pushed on stack by caller - thus walking stack from that address generates garbage.
//#define PRE_FUNCTION(name,param)       glDriver.LogFunctionPre (BuiltInFunc_##name.GetFunctionIndex(),FunctionArgs( ((char*)__builtin_frame_address(0)) + 2*sizeof(uintptr_t) ));
#endif

#define POST_FUNCTION(name)            glDriver.LogFunctionPost(BuiltInFunc_##name.GetFunctionIndex(),FunctionRetValue((GLuint64)0));
#define POST_FUNCTION_RET(name,retVal) glDriver.LogFunctionPost(BuiltInFunc_##name.GetFunctionIndex(),FunctionRetValue(retVal));

#define GL_BUILTIN_WRAPPER(functionName)  static BuiltInFunction BuiltInFunc_##functionName(#functionName, (void*)functionName, (void*)GLV_Builtin.functionName, (void**)&GLV_Builtin.functionName);
#define WGL_BUILTIN_WRAPPER(functionName) static BuiltInFunction BuiltInFunc_##functionName(#functionName, (void*)functionName, (void*)GLW_Builtin.functionName, (void**)&GLW_Builtin.functionName);


#define GL_FUNCTION_WRAPPER_HEADER(functionName,argDef,argList) \
  GL_BUILTIN_WRAPPER(functionName)          \
  void GLAPIENTRY functionName argDef       \


#define GL_FUNCTION_WRAPPER_HEADER_RET(functionName,argDef,argList,retType) \
  GL_BUILTIN_WRAPPER(functionName)               \
  retType GLAPIENTRY functionName argDef         \


#define WGL_FUNCTION_WRAPPER_HEADER(functionName,argDef,argList) \
  WGL_BUILTIN_WRAPPER(functionName)         \
  void WGLAPIENTRY functionName argDef      \


#define WGL_FUNCTION_WRAPPER_HEADER_RET(functionName,argDef,argList,retType) \
  WGL_BUILTIN_WRAPPER(functionName)         \
  retType WGLAPIENTRY functionName argDef   \


#define GL_FUNCTION_WRAPPER(functionName,argDef,argList,argFirst) \
  GL_FUNCTION_WRAPPER_HEADER(functionName,argDef,argList)         \
  {                                         \
    PRE_FUNCTION(functionName,argList,argFirst);    \
    GLV_Builtin.functionName argList;       \
    POST_FUNCTION(functionName)             \
  }

#define GL_FUNCTION_WRAPPER_RET(functionName,argDef,argList,argFirst,retType) \
  GL_FUNCTION_WRAPPER_HEADER_RET(functionName,argDef,argList,retType)         \
  {                                              \
    PRE_FUNCTION(functionName,argList,argFirst);         \
    retType retValue = GLV_Builtin.functionName argList; \
    POST_FUNCTION_RET(functionName,retValue)     \
    return retValue;                             \
  }


//@
//  Summary:
//    This function acts as a place holder of a built in function's index into
//    the function table.
//  
class BuiltInFunction  
{
public:

  //@
  //  Summary:
  //    Constructor. Adds a new function to the function table.
  //  
  //  Parameters:
  //    name     - The name of the function.
  //
  //    functionWrap  - A pointer to the function wrapper.
  //
  //    initialFunc   - A pointer to the initial function being wrapped.
  //
  //    funcDataPtr   - A pointer to the data function that is called by the
  //                    function wrapper.
  //
  BuiltInFunction(const string & functionName, void *functionWrapper,void *initialFunc,void**funcDataPtr);
  virtual ~BuiltInFunction();

  //@
  //  Summary:
  //    Gets the index number of the function.
  //  
  //  Returns:
  //    The index number of the function is returned.
  //
  inline uint GetFunctionIndex() const;

protected:
  uint funcIndex;                                 //The index of this function in the function table

};

///////////////////////////////////////////////////////////////////////////////
//
inline uint BuiltInFunction::GetFunctionIndex() const
{
  return funcIndex;
}

#endif // _BUILT_IN_FUNCTION_H_
