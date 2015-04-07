/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "EnumData.h"

USING_ERRORLOG

static const string unknownValueStr("Unknown");

///////////////////////////////////////////////////////////////////////////////
//
EnumData::EnumData(const string & name,bool isBitMask):
isBitMask(isBitMask),
enumName(name)
{

}

///////////////////////////////////////////////////////////////////////////////
//
EnumData::~EnumData()
{

}

///////////////////////////////////////////////////////////////////////////////
//
const string & EnumData::GetName() const
{
  return enumName;
}

///////////////////////////////////////////////////////////////////////////////
//
int EnumData::GetEnumIndex(uint value) const
{
  //Loop and retrieve
  for(uint i=0;i<enumValues.size();i++)
  {
    //Compare the value
    if(enumValues[i].value == value)
    {
      return i;
    }
  }

  return -1;
}

///////////////////////////////////////////////////////////////////////////////
//
int EnumData::GetEnumIndex(const string & name) const
{
  //Loop and retrieve
  for(uint i=0;i<enumValues.size();i++)
  {
    //Compare the name
    if(enumValues[i].name == name)
    {
      return i;
    }
  }

  return -1;
}


///////////////////////////////////////////////////////////////////////////////
//
bool EnumData::AddEnumValue(const EnumValue & value)
{
  //Loop and check
  for(uint i=0;i<enumValues.size();i++)
  {
    //Compare the value and name
    if(enumValues[i].value == value.value ||
       enumValues[i].name  == value.name)
    {
      LOGERR(("EnumData::AddEnumValue - Value %u Name %s already exists",value.value,value.name.c_str()));
      return false;
    }
  }

  //Extra check for mask enum
  if(isBitMask)
  {
    //Loop and test all bits
    uint numBits =0;
    for(uint i=0;i<sizeof(uint)*8;i++)
    {
      uint mask = ((uint)1 << i);

      //If this bit is set, increment the counter
      if(mask & value.value)
      {
        numBits++;
      }
    }

    //If there was more than one bit, log an error
    if(numBits > 1)
    {
      LOGERR(("EnumData::AddEnumValue - Value %u Name %s already has %u mask bits (Masks can only have 1 bit)",value.value,value.name.c_str(),numBits));
      return false;
    }
  }

  //Add the value
  enumValues.push_back(value);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
const string & EnumData::GetEnum(uint value) const
{
  //Retrieve the index
  int index = GetEnumIndex(value);
  if(index != -1)
  {
    return enumValues[index].name;
  }

  return unknownValueStr;
}

///////////////////////////////////////////////////////////////////////////////
//
uint EnumData::GetEnum(const string & name) const
{
  //Loop and retrieve
  int index = GetEnumIndex(name);
  if(index != -1)
  {
    return enumValues[index].value;
  }

  return (uint)-1;
}

///////////////////////////////////////////////////////////////////////////////
//
string EnumData::GetDisplayString(uint value) const
{
  //Check for a bit mask 
  if(!isBitMask)
  {
    //Look for the value
    int index = GetEnumIndex(value);
    if(index != -1)
    {
      return enumValues[index].name;
    }
    else
    {
      //Add the value to the buffer
      string retString;
      StringPrintF(retString,"0x%04x",value);
      return retString;
    }

  }
  else
  {
    string bitMaskStr;
    bool  firstBit =true;

    //Loop and test all bits
    for(uint i=0;i<sizeof(uint)*8;i++)
    {
      uint mask = ((uint)1 << i);

      //Test if the mask value is used
      if(value & mask)
      {
        //Add a seperator
        if(!firstBit)
        {
          bitMaskStr = bitMaskStr  + " | ";
        }
        else
        {
          firstBit =false;
        }

        //Look-up the mask value
        int index = GetEnumIndex(mask);
        if(index != -1)
        {
          bitMaskStr = bitMaskStr + enumValues[index].name;
        }
        else
        {
          //Add the mask value to the buffer
          string retString;
          StringPrintF(retString,"0x%04x",mask);
          bitMaskStr = bitMaskStr + retString;
        }
      }
    }

    //Assign a zero string for no bits
    if(firstBit)
    {
      //Ensure this is no "zero mask" value
      int index = GetEnumIndex(value);
      if(index != -1)
      {
        return enumValues[index].name;
      }
      else
      {
        bitMaskStr = "0x000000";
      }
    }

    //Return the mask
    return bitMaskStr;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool EnumData::Merge(EnumData & newData)
{
  bool retValue = true;

  //Loop and merge all values
  for(uint i=0;i<newData.enumValues.size();i++)
  {
    //Add and check for a existing enum
    if(!AddEnumValue(newData.enumValues[i]))
    {
      //Flag that at least one value already exists
      retValue = false;
    }
  }

  return retValue;
}
