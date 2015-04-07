/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __ENUM_DATA_H__
#define __ENUM_DATA_H__
#include "GLInterceptConfig.h"
#include <string>
#include <vector>

using namespace std;

//@
//  Summary:
//    simple name/value association for enums
//  
struct EnumValue
{
  string name;                                    //The string value of this enum
  uint   value;                                   //The numeric value of this enum
};


//@
//  Summary:
//    A class holding a array of related enum values 
//    (All enums of the same type)
//  
//  DT_TODO: OPTIMIZE: Make this a binary search class
class EnumData  
{
public:

  //@
  //  Summary:
  //    Constructor
  //  
  //  Parameters:
  //    name  - The name of this enum. (ie. Main_Enum)
  //
  //    isBitMask - Flag indicating if this is a bitmask type enum.
  //
  EnumData(const string & name,bool isBitMask);
  virtual ~EnumData();

  //@
  //  Summary:
  //    Gets the name of this enum
  //  
  //  Returns:
  //    The name of this enum is returned.
  //
  const string & GetName() const;

  //@
  //  Summary:
  //    Attempts to add a new enum value. Failure will occur if the enum
  //    (string or value) already exists.
  //  
  //  Parameters:
  //    value  - The new enum to add.
  //
  //  Returns:
  //    Returns true if the passed enum was added, false if otherwise
  //
  bool AddEnumValue(const EnumValue & value);

  //@
  //  Summary:
  //    Attempts to get the string value for the passed enum value.
  //  
  //  Parameters:
  //    value  - The new enum value to search for.
  //
  //  Returns:
  //    If the passed enum value exists, the string value is returned.
  //    Else the string "Unknown" is returned.
  //
  const string & GetEnum(uint value) const;

  //@
  //  Summary:
  //    Attempts to get the enum value for the passed string.
  //  
  //  Parameters:
  //    name  - The name or string value of the enum to search for.
  //
  //  Returns:
  //    If the passed string name exists, the enum value is returned.
  //    Else -1 is returned.
  //
  uint GetEnum(const string & name) const;

  //@
  //  Summary:
  //    Attempts to get the index value for the passed enum value.
  //  
  //  Parameters:
  //    value  - The new enum value to search for.
  //
  //  Returns:
  //    If the passed enum value exists, the index value is returned.
  //    Else the -1 is returned.
  //
  int GetEnumIndex(uint value) const;

  //@
  //  Summary:
  //    Attempts to get the index value for the passed enum string.
  //  
  //  Parameters:
  //    name  - The name or string value of the enum to search for.
  //
  //  Returns:
  //    If the passed string name exists, the enum index is returned.
  //    Else -1 is returned.
  //
  int GetEnumIndex(const string & name) const;

  //@
  //  Summary:
  //    Gets a dispayable string for the passed enum value.
  //    Mask enums are returned in the format "NAME1 | NAME2"
  //  
  //  Parameters:
  //    value  - The value or mask of values to generate a display string for.
  //
  //  Returns:
  //    If the value exists in this enum data, appropiate strings are generated
  //    and returned. Else a hex string version of the passed value is returned.
  //    (ie. "0x0023")
  //
  string GetDisplayString(uint value) const;

  //@
  //  Summary:
  //    To add/merge all the passed enum data with this enum's data.
  //  
  //  Parameters:
  //    newData  - The data to be merged into this enum.
  //
  //  Returns:
  //    If all the values could be merged, true is returned.
  //    Else all the value than could be merged are added and false is returned.
  //
  bool Merge(EnumData & newData);

protected:

  bool   isBitMask;                               // Flag to indicate if this is a bit-mask enumeration
  string enumName;                                // The name of this enum
  vector<EnumValue> enumValues;                   // The enum values

};

typedef vector<EnumData> EnumDataArray;

#endif // __ENUM_DATA_H__
