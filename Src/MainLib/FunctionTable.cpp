/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "FunctionTable.h"

USING_ERRORLOG

///////////////////////////////////////////////////////////////////////////////
//
FunctionTable::FunctionTable():
knownFunctionTableInit(false)
{
  //Reserve a big table
  functionTable.reserve(300);
}

///////////////////////////////////////////////////////////////////////////////
//
FunctionTable::~FunctionTable()
{

}


///////////////////////////////////////////////////////////////////////////////
//
int FunctionTable::AddFunction(const string &name, void * functionWrap, void * initialFunction, void ** wrapDataFunction, bool isFuncBuiltIn)
{
  //Create a new class to add
  FunctionData newData(name);

  //Attempt to find if it is a known function (use iterators for more speed)
  if(knownFunctionTableInit)
  {
    bool found=false;
    for(uint i=0;i<knownFunctionTable.size();i++)
    {
      //Check for a name match
      if(knownFunctionTable[i].GetName() == name)
      {
        newData = knownFunctionTable[i];
        found=true;
        break;
      }
    }

    //If not found, log an error (All functions should have at least void as a paremeter)
    if(!found || newData.parameterArray.size() == 0)
    {
      LOGERR(("Diagnostic: Unknown function %s being logged.",name.c_str()));
    }
  }

  //Assign the function pointers
  newData.wrappedFunctionPtr      = functionWrap;
  newData.origionalInternalPtr    = initialFunction;
  newData.internalFunctionDataPtr = wrapDataFunction;
  
  // Set if the function is a "built-in" one
  if(isFuncBuiltIn)
  {
    newData.SetFlags(FDF_BUILT_IN);
  }

  //Add it to the table
  functionTable.push_back(newData);

  return (int)functionTable.size()-1;
}

///////////////////////////////////////////////////////////////////////////////
//
int FunctionTable::FindFunction(const string & name) const
{
  //Loop and find the function name (should use iterators this could be slow)
  for(uint i=0;i<functionTable.size();i++)
  {
    //Check for a name match
    if(functionTable[i].GetName() == name)
    {
      return i;
    }
  }

  return -1;
}

///////////////////////////////////////////////////////////////////////////////
//
void FunctionTable::InitKnownFunctionTable(const FunctionDataArray & knownFunc,const EnumDataArray & enumData)
{
  uint i,i2,i3;

  //Check that this is only called once
  if(knownFunctionTableInit)
  {
    LOGERR(("FunctionTable::InitKnownFunctionTable - Initializing a second time?. Error?"));
    return;
  }

  //There should not be any existing known functions
  if(knownFunctionTable.size() > 0 || enumDataArray.size() >0)
  {
    LOGERR(("FunctionTable::InitKnownFunctionTable - Emptying existing known functions. Error?"));
  }

  //LOGERR(("Function count = %u",knownFunc.size()));

  //Empty all arrays
  knownFunctionTable.clear();
  enumDataArray.clear();

  //Loop and add all functions
  for(i=0;i<knownFunc.size();i++)
  {
    knownFunctionTable.push_back(knownFunc[i]);
  }

  //Loop and add all enums
  for(i=0;i<enumData.size();i++)
  {
    enumDataArray.push_back(enumData[i]);
  }


  //Link up the enums with the parameters
  for(i=0;i<knownFunctionTable.size();i++)
  {
    //Loop for all parameters (plus the return type)
    for(i2=0;i2<knownFunctionTable[i].parameterArray.size()+1;i2++)
    {
      ParameterData * paramData;
      
      //Assign either a parameter or the return type
      if(i2 <knownFunctionTable[i].parameterArray.size())
      {
        paramData = &knownFunctionTable[i].parameterArray[i2];
      }
      else
      {
        paramData = &knownFunctionTable[i].returnType;
      }
      
      //Init the index
      paramData->index = -1;

      //If the parameter is a named enum, attempt to link up
      if((paramData->type == PT_enum || paramData->type == PT_bitfield) && paramData->typeName != "")
      {
        //Loop for all the enums
        for(i3=0;i3<enumData.size();i3++)
        {
           //If the enum matches, assign and break
           if(enumData[i3].GetName() == paramData->typeName)
           {
             //Assign the lookup index
             paramData->index = i3;
             break;
           }
        }

        //Log a warning if not found
        if(paramData->index == -1)
        {
          LOGERR(("FunctionTable::InitKnownFunctionTable - Enum %s not found",paramData->typeName.c_str()));
        }
      }
    }
  }

  //Flag that the known function table is now initialized
  knownFunctionTableInit = true;

  //Update the data for any current built-in functions
  //Loop for all existing functions
  for(i=0;i<functionTable.size();i++)
  {
    bool found=false;

    //Loop for all known functions
    for(i2=0;i2<knownFunctionTable.size();i2++)
    {
      //Check for a name match
      if(functionTable[i].GetName() == knownFunctionTable[i2].GetName())
      {
        //Assign the parameter and return type data
        functionTable[i].parameterArray = knownFunctionTable[i2].parameterArray;
        functionTable[i].returnType     = knownFunctionTable[i2].returnType;
        functionTable[i].SetFlags(knownFunctionTable[i2].GetFlags()); 

        found=true;
        break;
      }
    }

    //If not found, log an error (All functions should have at least void as a paremeter)
    if(!found || functionTable[i].parameterArray.size() == 0)
    {
      LOGERR(("Diagnostic: Unknown function %s being logged.",functionTable[i].GetName().c_str()));
    }
  }


}


///////////////////////////////////////////////////////////////////////////////
//
void FunctionTable::SetFunctionFlag(const string & name, uint flag)
{
  //Check that this is not called before initialization
  if(!knownFunctionTableInit)
  {
    LOGERR(("FunctionTable::SetFunctionFlag - Calling before known function table has been initialized?"));
    return;
  }

  //Set the flag in both the known and working function arrays

  //Attempt to find the flag 
  int funcIndex = FindFunction(name);

  //If a valid function was found - assign the flag
  if(funcIndex != -1)
  {
    functionTable[funcIndex].SetFlags(flag);
  }

  //Attempt to find the function in the known function table
  bool found=false;
  for(uint i=0;i<knownFunctionTable.size();i++)
  {
    //Check for a name match
    if(knownFunctionTable[i].GetName() == name)
    {
      //Assign the flags
      knownFunctionTable[i].SetFlags(flag);
      found=true;
      break;
    }
  }

  //If not found, create
  if(!found)
  {
    //If the function was valid (but not known), just add it to the table
    if(funcIndex != -1)
    {
      knownFunctionTable.push_back(functionTable[funcIndex]);
    }
    else
    {
      FunctionData newData(name);
      newData.SetFlags(flag);

      //Add a new empty function with the name and flags
      knownFunctionTable.push_back(newData);
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void FunctionTable::UpdateBuiltinFunction(const string & name, void * newInitFunction)
{
  //Find the function
  int funcIndex = FindFunction(name);

  //Assign the function pointer
  if(funcIndex != -1)
  {
    functionTable[funcIndex].origionalInternalPtr = newInitFunction;
  }
  else
  {
    LOGERR(("UpdateBuiltinFunction - Unknown built-in function? %s", name.c_str())); 
  }

}

