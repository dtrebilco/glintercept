/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptLog.h"
#include "ErrorLog.h"
#include "GLDriver.h"
#include <time.h>

#include <iostream>
#include <sstream>
#include <string.h>

USING_ERRORLOG

extern GLDriver glDriver;

///////////////////////////////////////////////////////////////////////////////
//
InterceptLog::InterceptLog(FunctionTable * functionTable):
functionTable(functionTable),
glGetErrorFuncIndex(-1)
{
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptLog::Init()
{
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptLog::~InterceptLog()
{
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLog::GetFunctionString(const FunctionData *funcData,uint index, const FunctionArgs & args, string &retString)
{
  //Append the function name first
  retString = funcData->GetName() + "(";

  //Get a copy of the arguments
  FunctionArgs newArgs(args);

  //Loop for all the parameters
  for(uint i=0;i<funcData->parameterArray.size();i++)
  {
    //Get the parameter
    const ParameterData * paramData = &funcData->parameterArray[i];

    //Determine if we are processing pointers
    bool isPointer=false;
    if(paramData->pointerCount > 0 || paramData->length != -1)
    {
      isPointer=true;
    }

    //Get the value
    ParamValue value;
    if(!GetNextValue(paramData->GetGLType(),newArgs,isPointer, value))
    {
      break;
    }

    //Test if this is an array value
    if(paramData->length != -1)
    {
      bool isArrayOfPointers = false;

      //Test for an array of pointers
      if(paramData->pointerCount > 0)
      {
        isArrayOfPointers = true;
      }

      //Assign the array
      void * array =  value.pointerValue;

      //Loop and print the array
      retString += "[";
      for(uint i2=0;i2<(uint)paramData->length;i2++)
      {
        //Get the value from the array
        if(!GetNextArrayValue(paramData->GetGLType(),&array,isArrayOfPointers, value))
        {
          break;
        }
      
        //Convert and print the value
        retString += ConvertParam(value,isArrayOfPointers,paramData);
        
        //Add a comma
        if(i2 != (uint)(paramData->length - 1))
        {
          retString += ",";
        }
      }
      retString += "]";
    }
    else
    {
      //Just get the single value
      retString += ConvertParam(value,isPointer,paramData);
    }

    //Add a comma if there are more parameters
    if(i != funcData->parameterArray.size() - 1)
    {
      retString += ",";
    }
  }

  //If there are no parameters (unknown function)
  if(funcData->parameterArray.size() == 0)
  {
    retString += " ??? ";
  }

  //Close the bracket
  retString += ")";
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptLog::GetReturnString(const FunctionData *funcData,uint index, const FunctionRetValue & retVal, string &retString)
{
  //Empty the return string
  retString = "";

  //Check the function data
  if(!funcData)
  {
    LOGERR(("InterceptLog::GetReturnString - Function data for index %u is NULL",index)); 
    return;
  }

  //Get the return parameter
  const ParameterData * returnData = &funcData->returnType;

  //Determine if we are processing pointers
  bool isPointer=false;
  if(returnData->pointerCount > 0 || returnData->length != -1)
  {
    isPointer=true;
  }

  //Check the return value
  if(isPointer ||
     returnData->type != PT_void)
  {
    //Look up the data
    ParamValue value;
    if(GetReturnValue(returnData->type, retVal, isPointer, value))
    {
      retString = ConvertParam(value, isPointer, returnData);
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptLog::GetErrorStringValue(uint errorCode, string &retString)
{
  //If we do no have the index of glGetError yet, get it
  if(glGetErrorFuncIndex < 0)
  {
    //Get the index of the function
    glGetErrorFuncIndex = functionTable->FindFunction("glGetError");
  }

  //If there is a valid index
  const FunctionData  * glGetErrorFuncData = NULL;
  if(glGetErrorFuncIndex != -1)
  {
    //Get the function data
    // (Note: The returned pointer is not permanent (as future calls may add to the table)
    //  so we can only store the function index)
    glGetErrorFuncData = functionTable->GetFunctionData(glGetErrorFuncIndex);
  }

  //If the function is still not found, just log the number
  if(glGetErrorFuncData == NULL || glGetErrorFuncData->returnType.type != PT_enum)
  {
    StringPrintF(retString,"0x%04x",errorCode);
  }
  else
  {
    //Get the return parameter
    const ParameterData * returnData = &glGetErrorFuncData->returnType;

    //Get the string version
    ParamValue value;
    value.enumValue = errorCode;
    retString = ConvertParam(value, false, returnData);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptLog::GetNextValue(ParameterType pType, FunctionArgs &args, bool isPointer, ParamValue &value)
{
  //Test if we are getting a pointer
  if(isPointer)
  {
    //Get the pointer value
    args.Get(value.pointerValue);

    //Return true
    return true;
  }

  //Determine the type to return
  switch(pType)
  {
    case(PT_enum):
      args.Get(value.enumValue);
      break;
    case(PT_bitfield):
      args.Get(value.bitfieldValue);
      break;

    case(PT_void):
      break;

    case(PT_byte):
      args.Get(value.byteValue);
      break;
    case(PT_short):
      args.Get(value.shortValue);
      break;

    case(PT_int):
      args.Get(value.intValue);
      break;
    case(PT_sizei):
      args.Get(value.sizeiValue);
      break;

    case(PT_ubyte):
      args.Get(value.ubyteValue);
      break;

    case(PT_char):
      args.Get(value.charValue);
      break;

    case(PT_boolean):
      args.Get(value.booleanValue);
      break;

    case(PT_ushort):
      args.Get(value.ushortValue);
      break;

    case(PT_uint):
      args.Get(value.uintValue);
      break;
    case(PT_handle):
      args.Get(value.uintValue);
      break;

    case(PT_float):
      args.Get(value.floatValue);
      break;

    case(PT_double):
      args.Get(value.doubleValue);
      break;

    case(PT_intptr):
      args.Get(value.intptrValue);
      break;

    case(PT_sizeiptr):
      args.Get(value.sizeiptrValue);
      break;

    case(PT_int64):
      args.Get(value.int64Value);
      break;

    case(PT_uint64):
      args.Get(value.uint64Value);
      break;

    case(PT_sync):
      args.Get(value.syncValue);
      break;

    default:
      LOGERR(("InterceptLog::GetNextValue - Unhandled parameter in function of type %d",(int)pType));
      return false;
  }
 

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptLog::GetNextArrayValue(ParameterType pType, void **array, bool isPointer, ParamValue &value)
{
  //The value to increment the array by
  uint arrayInc;

  //Check for NULL arrays
  if(*array == NULL)
  {
    LOGERR(("InterceptLog::GetNextArrayValue - Passing NULL as array parameter?"));
    return false;
  }

  //Test if we are getting a pointer (is an array of pointers)
  if(isPointer)
  {
    arrayInc = sizeof(void *);
    value.pointerValue = *((void**)(*array)); 
  }
  else
  {

    //Determine the type to return
    switch(pType)
    {
      case(PT_enum):
        value.enumValue = *((GLenum*)(*array));
        arrayInc = sizeof(GLenum);
        break;

      case(PT_bitfield):
        value.bitfieldValue = *((GLbitfield*)(*array));
        arrayInc = sizeof(GLbitfield);
        break;

      case(PT_void):
        arrayInc = 0;
        break;

      case(PT_byte):
        value.byteValue = *((GLbyte*)(*array));
        arrayInc = sizeof(GLbyte);
        break;

      case(PT_short):
        value.shortValue = *((GLshort*)(*array));
        arrayInc = sizeof(GLshort);
        break;

      case(PT_int):
        value.intValue = *((GLint*)(*array));
        arrayInc = sizeof(GLint);
        break;

      case(PT_sizei):
        value.sizeiValue = *((GLsizei*)(*array));
        arrayInc = sizeof(GLsizei);
        break;

      case(PT_ubyte):
        value.ubyteValue = *((GLubyte*)(*array));
        arrayInc = sizeof(GLubyte);
        break;

      case(PT_char):
        value.charValue = *((GLchar*)(*array));
        arrayInc = sizeof(GLchar);
        break;

      case(PT_boolean):
        value.booleanValue = *((GLboolean*)(*array));
        arrayInc = sizeof(GLboolean);
        break;

      case(PT_ushort):
        value.ushortValue = *((GLushort*)(*array));
        arrayInc = sizeof(GLushort);
        break;

      case(PT_uint):
      case(PT_handle): 
        value.uintValue = *((GLuint*)(*array));
        arrayInc = sizeof(GLuint);
        break;

      case(PT_intptr):
        value.intptrValue = *((GLintptr*)(*array));
        arrayInc = sizeof(GLintptr);
        break;

      case(PT_sizeiptr):
        value.sizeiptrValue = *((GLsizeiptr*)(*array));
        arrayInc = sizeof(GLsizeiptr);
        break;

      case(PT_int64):
        value.int64Value = *((GLint64*)(*array));
        arrayInc = sizeof(GLint64);
        break;

      case(PT_uint64):
        value.uint64Value = *((GLuint64*)(*array));
        arrayInc = sizeof(GLuint64);
        break;

      case(PT_sync):
        value.syncValue = *((GLsync*)(*array));
        arrayInc = sizeof(GLsync);
        break;

      case(PT_float):
        value.floatValue = *((GLfloat*)(*array));
        arrayInc = sizeof(GLfloat);
        break;

      case(PT_double):
        value.doubleValue = *((GLdouble*)(*array));
        arrayInc = sizeof(GLdouble);
        break;

      default:
        LOGERR(("InterceptLog::GetNextArrayValue - Unhandled parameter in function of type %d",(int)pType));
        return false;
    }
  } 

  //Increment the array
  *array = ((char *)(*array) + arrayInc);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptLog::GetReturnValue(ParameterType pType, const FunctionRetValue & retVal, bool isPointer, ParamValue &value)
{
  //Test if we are getting a pointer
  if(isPointer)
  {
    //Get the pointer value
    retVal.Get(value.pointerValue);

    //Return true
    return true;
  }

  //Determine the type to return
  switch(pType)
  {
    case(PT_enum):
      retVal.Get(value.enumValue);
      break;
    case(PT_bitfield):
      retVal.Get(value.bitfieldValue);
      break;

    case(PT_void):
      break;

    case(PT_byte):
      retVal.Get(value.byteValue);
      break;
    case(PT_short):
      retVal.Get(value.shortValue);
      break;

    case(PT_int):
      retVal.Get(value.intValue);
      break;
    case(PT_sizei):
      retVal.Get(value.sizeiValue);
      break;

    case(PT_ubyte):
      retVal.Get(value.ubyteValue);
      break;
    case(PT_boolean):
      retVal.Get(value.booleanValue);
      break;

    case(PT_ushort):
      retVal.Get(value.ushortValue);
      break;

    case(PT_uint):
      retVal.Get(value.uintValue);
      break;
    case(PT_handle):
      retVal.Get(value.uintValue);
      break;

    case(PT_char):
      retVal.Get(value.charValue);
      break;

    case(PT_intptr):
      retVal.Get(value.intptrValue);
      break;

    case(PT_sizeiptr):
      retVal.Get(value.sizeiptrValue);
      break;

    case(PT_sync):
      retVal.Get(value.syncValue);
      break;

    // Float types and int64 types are not really allowed via the assembly wrappers currently - but there are some hard coded exceptions
    case(PT_float):
      retVal.Get(value.floatValue);
      break;

    case(PT_int64):
      retVal.Get(value.int64Value);
      break;

    case(PT_uint64):
      retVal.Get(value.uint64Value);
      break;

/* If these types are ever return types, will need to update the wrapper asm (esp. float types on x64)
    case(PT_double):
      retVal.Get(value.doubleValue);
      break;
*/
    default:
      LOGERR(("InterceptLog::GetReturnValue - Unhandled return value in function of type %d",(int)pType));
      return false;
  }
 

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
string InterceptLog::ConvertParam(const ParamValue &data, bool isPointer,const ParameterData *paramData)
{
  string retString;

  //If the data is a custom type, attempt to handle it
  if(paramData->IsCustomType() && 
     ConvertCustomParam(data,isPointer,paramData,retString))
  {
    return retString;
  }

  //If this is a pointer, out-put the address
  if(isPointer)
  {
    //Just get the pointer's address 
    StringPrintF(retString,"%p",data.pointerValue);
  }
  else
  {

    //Do a big switch statement
    ParameterType pType=paramData->GetGLType();
    switch(pType)
    {
      case(PT_enum):
        {
          //Get the enum data
          const EnumData * enumData=functionTable->GetEnumData(paramData->index);

          //If the index is invalid, Just print the hex values
          if(enumData ==NULL)
          {
            StringPrintF(retString,"0x%04x", data.enumValue);
          }
          else
          {
            retString = enumData->GetDisplayString(data.enumValue);
          }
        }
        break;

      case(PT_bitfield):
        {
          //Get the enum data
          const EnumData * enumData=functionTable->GetEnumData(paramData->index);

          //If the index is invalid, Just print the hex values
          if(enumData ==NULL)
          {
            StringPrintF(retString,"0x%04x", data.bitfieldValue);
          }
          else
          {
            retString = enumData->GetDisplayString(data.bitfieldValue);
          }
        }
        break;

      case(PT_boolean):
        {
          int num = data.booleanValue;

          //Check the value
          if(num == 0)
          {
            retString = "false";
          }
          else if(num == 1)
          {
            retString = "true";
          }
          else
          {
            StringPrintF(retString,"Invalid boolean %u",num);
          }
          break;
        }

      case(PT_void):
        break;

      case(PT_byte):
        {
          StringPrintF(retString,"%d",data.byteValue);
          break;
        }

      case(PT_short):
        {
          StringPrintF(retString,"%d",data.shortValue);
          break;
        }

      case(PT_int):
        {
          StringPrintF(retString,"%d",data.intValue);
          break;
        }

      case(PT_sizei):
        {
          CASSERT(sizeof(data.sizeiValue) == sizeof(int), __Update_type_printf__);
          StringPrintF(retString,"%d",data.sizeiValue);
          break;
        }

      case(PT_ubyte):
        {
          StringPrintF(retString,"%u",data.ubyteValue);
          break;
        }

      case(PT_char):
        {
          StringPrintF(retString,"%c",data.charValue);
          break;
        }

      case(PT_ushort):
        {
          StringPrintF(retString,"%u",data.ushortValue);
          break;
        }

      case(PT_uint):
      case(PT_handle):
        {
          StringPrintF(retString,"%u",data.uintValue);
          break;
        }

      case(PT_intptr):
        {
          ostringstream s1;
          s1 << data.intptrValue;
          retString = s1.str();
          break;
        }

      case(PT_sizeiptr):
        {
          ostringstream s1;
          s1 << data.sizeiptrValue;
          retString = s1.str();
          break;
        }

      case(PT_int64):
        {
          ostringstream s1;
          s1 << data.int64Value;
          retString = s1.str();
          break;
        }

      case(PT_uint64):
        {
          ostringstream s1;
          s1 << data.uint64Value;
          retString = s1.str();
          break;
        }

      case(PT_sync):
        {
          ostringstream s1;
          s1 << data.syncValue;
          retString = s1.str();
          break;
        }

      case(PT_float):
        {
          StringPrintF(retString,"%f",data.floatValue);
          break;
        }

      case(PT_double):
        {
          StringPrintF(retString,"%f",data.doubleValue);
          break;
        }

      default:
        LOGERR(("InterceptLog::ConvertParam - Unhandled parameter in function of type %d",(int)pType));
    }
  }

  return retString;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptLog::ConvertCustomParam(const ParamValue &data, bool isPointer,const ParameterData *paramData,string &retString)
{
  //Handle pointer types first
  if(isPointer)
  {
    //If the pointer is to an array of characters, get the characters
    if(paramData->type == PT_ascii_string && paramData->pointerCount == 1)
    {
      char * charArray = (char *)data.pointerValue;
     
      //If it is a NULL string, the user has an error
      if (charArray == NULL)
      {
        retString = "NULL";
      } 
      //If the string length is greater than 25 charcters, append it
      else if(strlen(charArray) > 25)
      {
        //Assign the buffer data
        retString.assign(charArray,25);
        retString = "\"" + retString + "...\"";
      }
      else
      {
        //Assign the entire character array
        retString = charArray;
        retString = "\"" + retString + "\"";
      }

      return true;
    }
  }

  /*
  //Determine the type
  switch(paramData->type)
  {



  }
  */

  return false;
}


