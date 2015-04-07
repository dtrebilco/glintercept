/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __PARAMETER_DATA_H__
#define __PARAMETER_DATA_H__
#include "GLInterceptConfig.h"
#include <vector>
#include <string>

using namespace std;

//@
//  Summary:
//    Table of the different types of parameters
//  
enum ParameterType
{
  //GL types
  PT_enum =0,     // unsigned int            GLenum;
  PT_boolean,     // unsigned char           GLboolean;
  PT_bitfield,    // unsigned int            GLbitfield;
  PT_void,        // void                    GLvoid;
  PT_byte,        // signed char             GLbyte;         /* 1-byte signed */
  PT_short,       // short                   GLshort;        /* 2-byte signed */
  PT_int,         // int                     GLint;          /* 4-byte signed */
  PT_ubyte,       // unsigned char           GLubyte;        /* 1-byte unsigned */
  PT_ushort,      // unsigned short          GLushort;       /* 2-byte unsigned */
  PT_uint,        // unsigned int            GLuint;         /* 4-byte unsigned */
  PT_sizei,       // int                     GLsizei;        /* 4-byte signed */
  PT_float,       // float                   GLfloat;        /* single precision float */
  PT_double,      // double                  GLdouble;       /* double precision float */

  PT_handle,      // unsigned int            GLhandle;       /* 4-byte unsigned */
  PT_char,        // unsigned char           GLchar;         /* GL character */

  PT_intptr,      // int                     GLintptr;
  PT_sizeiptr,    // int                     GLsizeiptr;

  PT_int64,       // int64                   GLint64;
  PT_uint64,      // uint64                  GLuint64;

  PT_sync,        // struct __GLsync *       GLsync;

  //Custom types
  PT_ascii_string,// unsigned char  
  PT_image_index, // unsigned int
  PT_shader_index,// unsigned int ->ARB vertex/fragment program / NV vertex/fragment program 
  
  PT_display_list,// unsigned int


  PT_GLSL_ARB_handle, // unsigned int
  PT_GLSL_program,    // unsigned int
  PT_GLSL_shader,     // unsigned int

  PT_Fixed,           // int

  PT_Max_Params,  //The maximum number of parameter types
};



//@
//  Summary:
//    A holding class for a single parameter
//  
class ParameterData  
{
public:

  //Constructor/destructor
  ParameterData();
  virtual ~ParameterData();

  //@
  //  Summary:
  //    Assigns the type of the parameter from the string passed.
  //  
  //  Parameters:
  //    type  - The type to set in this class in string format.(ie "GLenum")
  //
  //  Returns:
  //    Returns true if the passed string is a recognised type, false if otherwise
  //
  bool AssignType(const string & type);

  //@
  //  Summary:
  //    To get the OpenGL type of the passed type. (maps user types to GL types)
  //  
  //  Parameters:
  //    type  - [Optional] The type to get the OpenGL parameter type of.
  //
  //  Returns:
  //    Returns the OpenGL parameter type of the passed type.
  //
  inline ParameterType GetGLType() const;
  static ParameterType GetGLType(ParameterType type);

  //@
  //  Summary:
  //    To get if the OpenGL type is a cutsom (ie no-standard) type.
  //  
  //  Returns:
  //    Returns if the OpenGL type is a custom type.
  //
  inline bool IsCustomType() const;

  //@
  //  Summary:
  //    To get the passed parameter type in string value.
  //  
  //  Parameters:
  //    type  - The type to get the string value for.
  //
  //  Returns:
  //    Returns a string version of the passed parameter type.
  //
  static const string & GetTypeString(ParameterType type);

  //@
  //  Summary:
  //    To get a display string of the current parameter type. 
  //    (Using the OpenGL version of the type and including pointers)
  //  
  string GetDisplayString() const;


public:
  uint           pointerCount;                    //The number of levels of pointer indirection
  ParameterType  type;                            //The type of this parameter

  string         paramName;                       //The parameter name assigned to the type (This is NOT the type in string format)

  int            length;                          // Optional length of the type (used for arrays if not -1)
  int            index;                           // Optional index of the type  (used for lookups)
  string         typeName;                        // Optional type name          (used for lookups)

};

///////////////////////////////////////////////////////////////////////////////
//
inline ParameterType ParameterData::GetGLType() const
{
  //Get the GL type of "this" type 
  return GetGLType(type);
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool ParameterData::IsCustomType() const
{
  //Return true if the GL type does not match this type
  if(type != GetGLType(type))
  {
    return true;
  }

  return false;
}

typedef vector<ParameterData> ParameterDataArray;

#endif // __PARAMETER_DATA_H__
