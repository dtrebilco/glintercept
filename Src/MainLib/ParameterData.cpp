/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "ParameterData.h"

//A look-up structure
struct LookupData
{
  LookupData(const string & strType,ParameterType glT):stringVersion(strType),glType(glT) {}

  string        stringVersion;                    // The string version of the type
  ParameterType glType;                           // The parameter version of the type 
};

//This array must match the enum order
LookupData ParamLookupArray[PT_Max_Params] =
{
  LookupData("GLenum",PT_enum),
  LookupData("GLboolean",PT_boolean),
  LookupData("GLbitfield",PT_bitfield),
  LookupData("GLvoid",PT_void),
  LookupData("GLbyte",PT_byte),
  LookupData("GLshort",PT_short),
  LookupData("GLint",PT_int),
  LookupData("GLubyte",PT_ubyte),
  LookupData("GLushort",PT_ushort),
  LookupData("GLuint",PT_uint),
  LookupData("GLsizei",PT_sizei),
  LookupData("GLfloat",PT_float),
  LookupData("GLdouble",PT_double),
  LookupData("GLhandle",PT_handle),
  LookupData("GLchar",PT_char),
  LookupData("GLintptr",PT_intptr),
  LookupData("GLsizeiptr",PT_sizeiptr),
  LookupData("GLint64",PT_int64),
  LookupData("GLuint64",PT_uint64),
  LookupData("GLsync",PT_sync),

  //Custom types
  LookupData("GLasciistring",PT_ubyte),
  LookupData("GLimageindex",PT_uint),
  LookupData("GLshaderindex",PT_uint),
  LookupData("GLdisplaylist",PT_uint),
  LookupData("GLGLSL_ARB_handle",PT_uint),
  LookupData("GLGLSL_program",PT_uint),
  LookupData("GLGLSL_shader",PT_uint),

  LookupData("GLfixed",PT_int),
};


///////////////////////////////////////////////////////////////////////////////
//
ParameterData::ParameterData():
pointerCount(0),
type(PT_void),
length(-1),
index(-1)
{

}

///////////////////////////////////////////////////////////////////////////////
//
ParameterData::~ParameterData()
{

}

///////////////////////////////////////////////////////////////////////////////
//
bool ParameterData::AssignType(const string & typeStr)
{
  //"void" is a special type
  if(typeStr == "void")
  {
    type = PT_void;
    return true;
  }

  //Loop for all types
  for(uint i=0;i<PT_Max_Params;i++)
  {
    //Check for a match
    if(ParamLookupArray[i].stringVersion == typeStr)
    {
      //Assign the internal type
      type = (ParameterType)i;
      return true;
    }

  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
ParameterType ParameterData::GetGLType(ParameterType getType)
{
  //Return the OpenGL type
  return ParamLookupArray[getType].glType;
}


///////////////////////////////////////////////////////////////////////////////
//
const string & ParameterData::GetTypeString(ParameterType getType)
{
  //Return a string version
  return ParamLookupArray[getType].stringVersion;
}


///////////////////////////////////////////////////////////////////////////////
//
string ParameterData::GetDisplayString() const
{
  //Get the OpenGL type
  ParameterType glType = GetGLType(type);

  //Get the type string
  string retString = GetTypeString(glType);

  //Get the pointer count
  for(uint i=0;i<pointerCount;i++)
  {
    retString = retString + "*";
  }

  //If there exists and array length, that is an additional pointer
  if(length != -1)
  {
    string bufString;
    StringPrintF(bufString,"[%d]",length);
    retString = retString + bufString;
  }

  return retString;
}
