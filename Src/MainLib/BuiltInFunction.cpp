/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/

#include "BuiltInFunction.h"
#include "GLDriver.h"
#include "FunctionTable.h"

extern FunctionTable * functionTable;

///////////////////////////////////////////////////////////////////////////////
//
BuiltInFunction::BuiltInFunction(const string & functionName,void *functionWrapper,void *initialFunc,void**funcDataPtr)
{
  //Test if the function table has been init
  if(!functionTable)
  {
    GLDriver::InitFunctionTable();
  }

  //Add a new function
  funcIndex = functionTable->AddFunction(functionName,functionWrapper,initialFunc,funcDataPtr,true);
}

///////////////////////////////////////////////////////////////////////////////
//
BuiltInFunction::~BuiltInFunction()
{

}
