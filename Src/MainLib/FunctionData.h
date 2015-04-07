/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __FUNCTION_DATA_H_
#define __FUNCTION_DATA_H_
#include "GLInterceptConfig.h"
#include "ParameterData.h"
#include <string>
#include <vector>

using namespace std;


enum FunctionDataFlags
{
  FDF_LOG_IGNORE      = (1L << 0),  // Ignore this function when logging (only file logging is ignored)

  FDF_IMAGE_LOG       = (1L << 1),  // This function is used by the image logger
  FDF_SHADER_LOG      = (1L << 2),  // This function is used by the shader logger
  FDF_SHADER_GLSL_LOG = (1L << 3),  // This function is used by the GLSL shader logger
  FDF_DLIST_LOG       = (1L << 4),  // This function is used by the display list logger

  FDF_RENDER_FUNC     = (1L << 5),  // This function is a render call
  FDF_FRAME_FUNC      = (1L << 6),  // This function is a end of frame function
  FDF_OS_FUNC         = (1L << 7),  // This function is a operating system function (WGL/GLX)
  FDF_BUILT_IN        = (1L << 8),  // This function is a "built-in" function
};

//@
//  Summary:
//    This class provides simple storage of data required by a GL function
//  
class FunctionData  
{
public:

  //Constructor/ Destructor
  FunctionData(const string & a_functionName);
  virtual ~FunctionData();

  //@
  //  Summary:
  //    To get the name of the OpenGL function. 
  //  
  //  Returns:
  //    The name of the function is returned.
  //
  inline const string & GetName() const;

  //@
  //  Summary:
  //    To set the specified flags for this function. 
  //  
  //  Parameters:
  //    a_flags - The flags to set for this OpenGL function.
  //
  inline void SetFlags(uint a_flags);

  //@
  //  Summary:
  //    To get the flags for this OpenGL function. 
  //  
  //  Returns:
  //    All the flags for this OpenGL function are returned.
  //
  inline uint GetFlags() const;

  //@
  //  Summary:
  //    To get if the specified flag is set on the OpenGL function. 
  //    If multiple flags are passed, returns true if any of the flags are set.
  //  
  //  Parameters:
  //    a_flag - The flag(s) to test.
  //
  //  Returns:
  //    If any of the flags passed are set, true is returned. Else false is returned.
  //
  inline bool IsFlagSet(uint a_flag) const;


  // Should protect these members in future
  void   *wrappedFunctionPtr;                     // A pointer to the wrapper function
  void   *origionalInternalPtr;                   // The origional function that is being wrapped (The internal function may be changed at runtime)  

  void  **internalFunctionDataPtr;                // A pointer to the function data that is called by the wrapper function
                                                  //  (The wrapper must call a function based on a stored data value)

  ParameterData      returnType;                  // The return type of this function
  ParameterDataArray parameterArray;              // The parameters of this function

protected:

  string  functionName;                           // The function's name
  uint    functionFlags;                          // Flags associated with this function

};
typedef vector<FunctionData> FunctionDataArray;


inline const string & FunctionData::GetName() const
{
  return functionName;
}


inline void FunctionData::SetFlags(uint a_flags)
{
  functionFlags |= a_flags;
}


inline uint FunctionData::GetFlags() const
{
  return functionFlags;
}


inline bool FunctionData::IsFlagSet(uint a_flag) const
{
  return (functionFlags & a_flag) != 0;
}


#endif // __FUNCTION_DATA_H_
