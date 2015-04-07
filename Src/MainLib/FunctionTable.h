/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __FUNCTION_TABLE_H
#define __FUNCTION_TABLE_H

#include "GLInterceptConfig.h"
#include <vector>

#include "EnumData.h"
#include "FunctionData.h"


using namespace std;

//@
//  Summary:
//    This class stores all the curretly active function pointers in 
//    the OpenGL state.
//  
class FunctionTable  
{
public:

  //Constructor/Destructor
  FunctionTable();
  virtual ~FunctionTable();


  //@
  //  Summary:
  //    Initializes the list of known functions in this table
  //  
  //  Parameters:
  //    knownFunc - The list of known functions.
  //
  //    enumData - The list of enums referenced by the functions.
  //
  void InitKnownFunctionTable(const FunctionDataArray & knownFunc,const EnumDataArray & enumData);

  //@
  //  Summary:
  //    Adds a new function to the function table. (Duplicates are allowed - 
  //    Use Find function to check for an existing function)
  //    It is valid for this to be called before InitKnownFunctionTable.
  //  
  //  Parameters:
  //    name     - The name of the function.
  //
  //    functionWrap     - A pointer to the function wrapper.
  //
  //    initialFunction  - A pointer to the initial function that is being wrapped.
  //
  //    wrapDataFunction - A pointer to the data function that is called by the
  //                       function wrapper. 
  //
  //    isFuncBuiltIn    - Is the function a built-in internal function or a loaded extension function.
  //
  //  Returns:
  //    The index number that the function was added at is returned.
  //
  int AddFunction(const string &name, void * functionWrap, void * initialFunction, void ** wrapDataFunction, bool isFuncBuiltIn);

  //@
  //  Summary:
  //    Attempts to get the function at the specified index.
  //
  //    Note that the returned pointer is only valid while the function 
  //    table is unchanged. Adding values to the table will invalidate
  //    the pointer.
  //  
  //  Parameters:
  //    funcNum     - The index of the function to return.
  //
  //  Returns:
  //    If a function exists at that index, it is returned. Else NULL is returned.
  //
  inline const FunctionData * GetFunctionData(uint funcNum) const;

  //@
  //  Summary:
  //    Attempts to get the enum at the specified index.
  //  
  //  Parameters:
  //    enumNum     - The index of the enum to return.
  //
  //  Returns:
  //    If a enum exists at that index, it is returned. Else NULL is returned.
  //
  inline const EnumData * GetEnumData(uint enumNum) const;


  //@
  //  Summary:
  //    Attempts to find the function of the given name
  //  
  //  Parameters:
  //    name     - The name of the function to find.
  //
  //  Returns:
  //    If a function of the passed name exists, the index is returned. Else -1 is returned.
  //
  int FindFunction(const string & name) const;

  //@
  //  Summary:
  //    To set the passed flag to the function of the given name. If the 
  //    function is unknown or has not been loaded yet, a entry in the known
  //    function table is created. (This function should not be called before
  //    InitKnownFunctionTable has been called.)
  //  
  //  Parameters:
  //    name     - The name of the function to set the flag for.
  //
  //    flag     - The flag(s) to set.
  //
  void SetFunctionFlag(const string & name, uint flag);

  //@
  //  Summary:
  //    To get the number of functions registered with the function table.
  //  
  //  Returns:
  //    The number of functions registered with the function table is returned.
  //
  inline uint GetNumFunctions() const;

  //@
  //  Summary:
  //    To update the "initial" function of the specified function name. This
  //    is needed for built-in functions whose initial function pointer is 
  //    not known at creation time.
  //  
  //  Parameters:
  //    name     - The name of the function to set the function pointer on.
  //
  //    newInitFunction - The new "initial" function pointer.
  //
  void UpdateBuiltinFunction(const string & name, void * newInitFunction);

protected:

  FunctionDataArray    functionTable;             //The table of lookup funtions

  bool                 knownFunctionTableInit;    //The flag indicating if the known function table has been initialized
  FunctionDataArray    knownFunctionTable;        //The table of known functions 
                                                  //  (known but may not be used -does not have valid function pointers)
  EnumDataArray        enumDataArray;             //The array of enums referenced by the functions
};
 
///////////////////////////////////////////////////////////////////////////////
//
inline const FunctionData * FunctionTable::GetFunctionData(uint funcNum) const 
{
  //Do array check
  if(funcNum >= functionTable.size())
  {
    return NULL;
  }

  //Return the function
  return &functionTable[funcNum];
}

///////////////////////////////////////////////////////////////////////////////
//
inline const EnumData * FunctionTable::GetEnumData(uint enumNum) const 
{
  //Do array check
  if(enumNum >= enumDataArray.size())
  {
    return NULL;
  }

  //Return the enum
  return &enumDataArray[enumNum];
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint FunctionTable::GetNumFunctions() const
{
  return (uint) functionTable.size();
}

#endif // __FUNCTION_TABLE_H
