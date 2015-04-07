/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __FUNCTION_ARGS_H
#define __FUNCTION_ARGS_H

#include <OSDefines.h>
#include <MiscUtils.h>
#include "GLDefines.h"


//Custom va_args type macros
#ifdef OS_ARCH_x86
#define GLIrounded_size(TYPE)    ( ((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int) )
#define GLIva_arg(pramData,TYPE) ( *((TYPE *) ((pramData += GLIrounded_size (TYPE)) - GLIrounded_size (TYPE))) )
#endif

#ifdef OS_ARCH_x64

#define GLIva_arg(pramData,TYPE)   ( GetParam<TYPE>::get(argIdx, pramData) )

#ifdef GLI_BUILD_WINDOWS
const int RegisterParameters = 4;
#endif
#ifdef GLI_BUILD_LINUX
// Linux does not do the double up of int/float registers as Windows 64 bit does? - need a different implementation of GetParam 
//const int IntegerRegisterParameters = 6;
//const int FloatingRegisterParameters = 8;
#endif

template<class T> struct IsFloat { enum { yes = 0 }; };
template<> struct IsFloat<float> { enum { yes = 1 }; };
template<> struct IsFloat<double> { enum { yes = 1 }; };

template<class T>
struct GetParam {

  static T get(uint & a_argIndex, char *& a_argStack)
  {
    uintptr_t* stack = (uintptr_t*)(a_argStack);

    // Update the stack and the index pointers for next parameter
    T retData;
    if(a_argIndex >= RegisterParameters)
    {
      retData = *((T*)(stack+0));
      a_argStack += sizeof(uintptr_t);
    }
    else
    {
      // The first few parameters are stored in registers - so both the float and int registers are saved out together
      retData = *((T*)(stack + 1 - IsFloat<T>::yes));
      a_argStack += 2 * sizeof(uintptr_t);
    }
    a_argIndex++;

    return retData;
  }
};

#endif

//@
//  Summary:
//    This class provides a way to access the parameter arguments passed on an
//    arbitary function call. 
//
//  Note:
//    This class is similar to va_args but does not have some of the 
//    restrictions. It is very platform specific.
//
class FunctionArgs  
{
public:

  //Constructor/Destructor
  inline FunctionArgs(char * args);
  inline FunctionArgs(char * args, uint offset);  
  inline FunctionArgs(const FunctionArgs &instance);
  inline virtual ~FunctionArgs();

  //@
  //  Summary:
  //    Assignment operator.
  //  
  //  Parameters:
  //    p - The value to assign to this object.
  //
  //  Returns:
  //    This object is returned.
  //
  inline FunctionArgs &operator=(const FunctionArgs& p);

  //@
  //  Summary:
  //    To get the specified type value from the function argument list.
  //    This increments the internal counters to point at the next argument
  //    value.
  //  
  //  Parameters:
  //    value - The return value.
  //
  inline void Get(void * &value);   
  inline void Get(const void * &value);  
  inline void Get(const GLbyte *&value);
  inline void Get(const GLshort *&value);
  inline void Get(const GLint *&value);
  inline void Get(const GLubyte *&value);
  inline void Get(const GLushort *&value);
  inline void Get(const GLuint *&value);
  inline void Get(const GLfloat *&value);
  inline void Get(const GLdouble *&value);

  inline void Get(GLbyte &value);
  inline void Get(GLshort &value);
  inline void Get(GLint &value);
  inline void Get(GLubyte &value);
  inline void Get(GLushort &value);
  inline void Get(GLuint &value);
  inline void Get(GLfloat &value);
  inline void Get(GLdouble &value);
  //inline void Get(GLsizei &value);
  //inline void Get(GLenum &value);
  //inline void Get(GLboolean &value);
  //inline void Get(GLhandle &value);
  //inline void Get(GLbitfield &value);

  inline void Get(GLint64 &value);
  inline void Get(GLuint64 &value);
  inline void Get(GLsync &value);

protected:

  char * paramData;                               //The parameter data 
  uint argIdx;                                    //Index of argument that will be retrived
                                                  //this is needed only on 64b oses,
                                                  //but doesnt do much harm for others
};

///////////////////////////////////////////////////////////////////////////////
//
inline FunctionArgs::FunctionArgs(char * args)
{
  paramData = args;
  argIdx = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
inline FunctionArgs::FunctionArgs(char * args, uint offset)
{
  paramData = args + offset;  
  argIdx = 0;
}


///////////////////////////////////////////////////////////////////////////////
//
inline FunctionArgs::FunctionArgs(const FunctionArgs &instance)
{
  paramData = instance.paramData;
  argIdx = instance.argIdx;
}

///////////////////////////////////////////////////////////////////////////////
//
inline FunctionArgs::~FunctionArgs()
{

}

///////////////////////////////////////////////////////////////////////////////
//
FunctionArgs &FunctionArgs::operator=(const FunctionArgs& p)
{
  //Ensure this is not the same object
  if (this == &p)
  {
    return *this;
  }

  //Copy the data
  paramData = p.paramData;
  argIdx = p.argIdx;

  return *this;  
}


///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(void *&value)
{
  value = GLIva_arg(paramData, void *);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(const void *&value)
{
  value = GLIva_arg(paramData, const void *);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(const GLbyte *&value)
{
  value = GLIva_arg(paramData, const GLbyte *);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(const GLshort *&value)
{
  value = GLIva_arg(paramData, const GLshort *);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(const GLint *&value)
{
  value = GLIva_arg(paramData, const GLint *);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(const GLubyte *&value)
{
  value = GLIva_arg(paramData, const GLubyte *);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(const GLushort *&value)
{
  value = GLIva_arg(paramData, const GLushort *);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(const GLuint *&value)
{
  value = GLIva_arg(paramData, const GLuint *);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(const GLfloat *&value)
{
  value = GLIva_arg(paramData, const GLfloat *);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(const GLdouble *&value)
{
  value = GLIva_arg(paramData, const GLdouble *);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLbyte &value)
{
  value = GLIva_arg(paramData, GLbyte);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLshort &value)
{
  value = GLIva_arg(paramData, GLshort);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLint &value)
{
  value = GLIva_arg(paramData, GLint);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLubyte &value)
{
  value = GLIva_arg(paramData, GLubyte);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLushort &value)
{
  value = GLIva_arg(paramData, GLushort);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLuint &value)
{
  value = GLIva_arg(paramData, GLuint);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLfloat &value)
{
  value = GLIva_arg(paramData, GLfloat);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLdouble &value)
{
  value = GLIva_arg(paramData, GLdouble);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLint64 &value)
{
  value = GLIva_arg(paramData, GLint64);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLuint64 &value)
{
  value = GLIva_arg(paramData, GLuint64);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void FunctionArgs::Get(GLsync &value)
{
  value = GLIva_arg(paramData, GLsync);
}

//Undefine some macros (GLIrounded_size is not undefined)
#undef GLIva_arg

#endif // __FUNCTION_ARGS_H
