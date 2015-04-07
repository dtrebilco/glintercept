/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_LOG_H_
#define __INTERCEPT_LOG_H_

#include "GLInterceptConfig.h"
#include <string>
#include "FunctionData.h"
#include "FunctionTable.h"

using namespace std;

//@
//  Summary:
//    This class is a base class for logging OpenGL functions.
//  
class InterceptLog 
{
public:

  //@
  //  Summary:
  //    Constructor to set up the function table.
  //  
  //  Parameters:
  //    functionTable - The table of all functions that are known/are being logged.
  //
  InterceptLog(FunctionTable * functionTable);

  //@
  //  Summary:
  //    Destructor, closes the log.
  //  
  virtual ~InterceptLog();

  //@
  //  Summary:
  //    To perform initialization. 
  //
  //  Returns:
  //    True is returned on init success, false if not.
  //
  virtual bool Init();

  //@
  //  Summary:
  //    To get the passed function and function data in a plain string format.
  //    (ie. returned string is in the format <function_name>(<parameters>,....) )
  //  
  //  Note:
  //    The return value is not included in the string. (as it is not available)
  //
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  //    retString- The string to return the function and parameters in.
  //
  void GetFunctionString(const FunctionData *funcData,uint index, const FunctionArgs & args, string &retString);

  //@
  //  Summary:
  //    To get the passed OpenGL function return value in a plain string format.
  //
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value.
  //
  //    retString- The string to return the return value in.
  //
  void GetReturnString(const FunctionData *funcData,uint index, const FunctionRetValue & retVal, string &retString);


  //@
  //  Summary:
  //    To convert a error return value into its string representation.
  //  
  //  Parameters:
  //    errorCode - The error value to convert.
  //
  //    retString - The return string containing a string representation of the error code.
  //
  void GetErrorStringValue(uint errorCode, string &retString);

  //@
  //  Summary:
  //    To log the passed function and function data 
  //    (Before the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  inline virtual void LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args);

  //@
  //  Summary:
  //    To perform any post-call logging of a function.
  //    (After the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value (if any).
  //
  inline virtual void LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    To log that a GL error has occured to the log.
  //  
  //  Parameters:
  //    errorCode - The GL error code to log.
  //
  inline virtual void LogFunctionError(uint errorCode);

  //@
  //  Summary:
  //    To log that the current function has completed. 
  //    (ie. All data for the previous function has been processed)
  //  
  inline virtual void LogFunctionComplete();

  //@
  //  Summary:
  //    To add a raw string to the output log.
  //  
  //  Parameters:
  //    addString  - The raw string to add to the log.
  //
  inline virtual void AddLogString(const char* addString);

protected:

  const FunctionTable * functionTable;            // The function table to log functions from

  //Parameter value of all the different types of parameter values
  union ParamValue
  {
    void     * pointerValue;
    GLchar     charValue;
    GLbyte     byteValue;
    GLshort    shortValue;
    GLint      intValue;
    GLubyte    ubyteValue;
    GLushort   ushortValue;
    GLuint     uintValue;
    GLfloat    floatValue;
    GLdouble   doubleValue;
    GLsizei    sizeiValue;
    GLenum     enumValue;
    GLboolean  booleanValue;
    GLbitfield bitfieldValue;

    GLintptr   intptrValue;
    GLsizeiptr sizeiptrValue;
    GLint64    int64Value;
    GLuint64   uint64Value;
    GLsync     syncValue;
  };
  //gcc 4.4.3 dont like this for some reason
  //CASSERT(sizeof(GLint64) == (64/8), __int64_size__);
  //CASSERT(sizeof(GLuint64) == (64/8), __uint64_size__);

  //@
  //  Summary:
  //    To convert the passed data as a string.
  //  
  //  Parameters:
  //    data  - The parameter.
  //
  //    isPointer - Flag to indicate if the value is a pointer
  //
  //    paramData  - A pointer to the current parameter data being processed.
  //
  //  Returns:
  //    A string version of the passed value is returned.
  //
  string ConvertParam(const ParamValue &data, bool isPointer,const ParameterData *paramData);

  //@
  //  Summary:
  //    If the type of the parameter is custom (ie. non-standard OpenGL)
  //    calling this method will attempt to handle the type.
  //  
  //  Parameters:
  //    data  - The parameter.
  //
  //    isPointer - Flag to indicate if the value is a pointer
  //
  //    paramData  - A pointer to the current parameter data being processed.
  //
  //    retString  - If the data is handled, the data for the type is returned in the string.
  //
  //  Returns:
  //    If the parameter was converted, true is returned and the data is stored in retString.
  //    Else false is returned and retString is unchanged.
  //
  virtual bool ConvertCustomParam(const ParamValue &data, bool isPointer,const ParameterData *paramData,string &retString);

  //@
  //  Summary:
  //    To get the next value.
  //  
  //  Parameters:
  //    pType - The type of the parameter.
  //
  //    args  - The current arguments array
  //
  //    isPointer - Flag to indicate if the value is a pointer
  //
  //    value  - The return value.
  //
  //  Returns:
  //    Returns true if the value was retrieved, false if not.
  //
  bool GetNextValue(ParameterType pType, FunctionArgs &args, bool isPointer, ParamValue &value);

  //@
  //  Summary:
  //    To the next value in the passed array. Array value is also moved.
  //  
  //  Parameters:
  //    pType - The type of the parameter.
  //
  //    array  - The current array
  //
  //    isPointer - Flag to indicate if the value is a pointer
  //
  //    value  -The return value.
  //
  //  Returns:
  //    Returns true if the value was retrieved, false if not.
  //
  bool GetNextArrayValue(ParameterType pType, void **array, bool isPointer, ParamValue &value);

  //@
  //  Summary:
  //    To get the passed return value as a parameter type.
  //  
  //  Parameters:
  //    pType - The type of the return value.
  //
  //    retVal  - The value to convert.
  //
  //    isPointer - Flag to indicate if the value is a pointer
  //
  //    value  - The return value.
  //
  //  Returns:
  //    Returns true if the value was retrieved, false if not.
  //
  bool GetReturnValue(ParameterType pType, const FunctionRetValue & retVal, bool isPointer, ParamValue &value);

private:

  int glGetErrorFuncIndex;       // The index of the glGetError() function in the function table.
};


///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptLog::LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptLog::LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptLog::LogFunctionError(uint errorCode)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptLog::LogFunctionComplete()
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptLog::AddLogString(const char* addString)
{

}


#endif // !__INTERCEPT_LOG_H_
