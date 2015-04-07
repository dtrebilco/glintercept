/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __FUNCTION_RET_VALUE_H
#define __FUNCTION_RET_VALUE_H

#include "GLDefines.h"
#include <MiscUtils.h>

//@
//  Summary:
//    This class provides a way to access the return value from a 
//    arbitary function call. 
//
//  Note:
//    This class is very platform specific. Ensure to test return 
//    types from built-in to extension types
//
class FunctionRetValue 
{
public:

  //Constructor
  inline FunctionRetValue(GLuint64 newIntValue, GLfloat floatValue = 0.0f);
  inline FunctionRetValue(const void *newValue, GLfloat floatValue = 0.0f);

  //@
  //  Summary:
  //    To get the specified return value.
  //  
  //  Parameters:
  //    value - The return value.
  //
  inline void Get(void *&value) const;
  inline void Get(GLuint &value) const;
  inline void Get(GLint  &value) const;
  inline void Get(GLbyte &value) const;
  inline void Get(GLshort &value) const;
  inline void Get(GLubyte &value) const;
  inline void Get(GLushort &value) const;
  inline void Get(GLsync &value) const;
  inline void Get(GLfloat &value) const;
  inline void Get(GLint64 &value) const;
  inline void Get(GLuint64 &value) const;

protected:

  GLuint64 intValue;                                 //The return value 
  GLfloat floatValue;                                //The return float value
};

///////////////////////////////////////////////////////////////////////////////
//
inline FunctionRetValue::FunctionRetValue(GLuint64 newIntValue, GLfloat newFloatValue)
{
  intValue = newIntValue;
  floatValue = newFloatValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline FunctionRetValue::FunctionRetValue(const void *newValue, GLfloat newFloatValue)
{
  CASSERT(sizeof(GLuint64) >= sizeof(void*), Pointers_and_GLuint64_mismatch);
  CASSERT(sizeof(GLuint64) == 8, Incorrect_Int_size);

  //This assumes pointers and ints have the same size
  intValue = (GLuint64)(newValue);

  floatValue = newFloatValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(void *&retValue) const
{
  retValue  = (void*)intValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(GLuint &retValue) const
{
  retValue = (GLuint) intValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(GLint &retValue) const
{
  retValue = (GLint) intValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(GLbyte &retValue) const
{
  retValue = (GLbyte) intValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(GLshort &retValue) const
{
  retValue = (GLshort) intValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(GLubyte &retValue) const
{
  retValue = (GLubyte) intValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(GLushort &retValue) const
{
  retValue = (GLushort) intValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(GLsync &retValue) const
{
  //CASSERT(sizeof(GLuint) == sizeof(GLsync), GLSync_and_uints_not_the_same_size);

  retValue = (GLsync) intValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(GLfloat &retValue) const
{
  retValue = floatValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(GLint64 &retValue) const
{
  retValue = (GLint64) intValue;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionRetValue::Get(GLuint64 &retValue) const
{
  retValue = intValue;
}


#endif // __FUNCTION_RET_VALUE_H
