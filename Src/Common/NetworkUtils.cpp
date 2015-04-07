/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "NetworkUtils.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
//
void NetworkBufferSend::Add(int intValue)
{
  //Add space for an int
  sendBuffer.resize(sendBuffer.size() + sizeof(int));

  //Add the int
  *((int*)&sendBuffer[sendBuffer.size() - sizeof(int)]) = intValue;
}

///////////////////////////////////////////////////////////////////////////////
//
void NetworkBufferSend::Add(uint uintValue)
{
  //Add space for an int
  sendBuffer.resize(sendBuffer.size() + sizeof(uint));

  //Add the int
  *((uint*)&sendBuffer[sendBuffer.size() - sizeof(uint)]) = uintValue;
}

///////////////////////////////////////////////////////////////////////////////
//
void NetworkBufferSend::Add(const string &strValue)
{
  //Add the real size of the string to the buffer
  Add((uint)strValue.size());

  //Get the size of the string
  uint allocateSize = (uint)strValue.size();

  //Round up if not a multiple of 4 (so all data is stored on 4 byte offsets)
  if(allocateSize % 4 != 0)
  {
    allocateSize += 4 - (allocateSize % 4);
  }

  //Add space for the string
  uint storeOffset = (uint)sendBuffer.size();
  sendBuffer.resize(storeOffset + allocateSize);
  
  //Copy across the string data (note that no NULL terminator is written)
  memcpy(&sendBuffer[storeOffset], strValue.c_str(), strValue.size());  
}

///////////////////////////////////////////////////////////////////////////////
//
void NetworkBufferSend::Add(uint arraySize, const int *addArray)
{
  //Add the size of the array to the buffer
  Add(arraySize);
  
  //Reserve space for the array
  uint storeOffset = (uint)sendBuffer.size();
  sendBuffer.resize(storeOffset + (arraySize * sizeof(int)));

  //Copy across the data
  memcpy(&sendBuffer[storeOffset], addArray, arraySize * sizeof(int));  
}

///////////////////////////////////////////////////////////////////////////////
//
void NetworkBufferSend::Add(uint arraySize, const float *addArray)
{
  //Add the size of the array to the buffer
  Add(arraySize);
  
  //Reserve space for the array
  uint storeOffset = (uint)sendBuffer.size();
  sendBuffer.resize(storeOffset + (arraySize * sizeof(float)));

  //Copy across the data
  memcpy(&sendBuffer[storeOffset], addArray, arraySize * sizeof(float));  
}


///////////////////////////////////////////////////////////////////////////////
//
NetworkBufferReceive::NetworkBufferReceive(uint newBufferSize, const void *newBuffer):
bufferSizeLeft(newBufferSize),
bufferOffset(newBuffer)
{
}


///////////////////////////////////////////////////////////////////////////////
//
NetworkBufferReceive::~NetworkBufferReceive()
{
  //Log the number of bytes not read
  if(bufferSizeLeft > 0)
  {
    //LOGERR(("~NetworkBufferReceive - %u number of bytes unread?",bufferSizeLeft)); 
  }
}


///////////////////////////////////////////////////////////////////////////////
//
bool NetworkBufferReceive::Get(int &retValue)
{
  //Check the size remaining
  if(bufferSizeLeft < sizeof(int))
  {
    return false;
  }

  //Extract the int
  retValue = *((int*)bufferOffset);

  //Move the buffer offset
  bufferOffset = ((int*)bufferOffset) + 1;
  bufferSizeLeft -= sizeof(int);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool NetworkBufferReceive::Get(uint &retValue)
{
  //Check the size remaining
  if(bufferSizeLeft < sizeof(uint))
  {
    return false;
  }

  //Extract the uint
  retValue = *((uint*)bufferOffset);

  //Mover the buffer offset
  bufferOffset = ((uint*)bufferOffset) + 1;
  bufferSizeLeft -= sizeof(uint);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool NetworkBufferReceive::Get(string &retValue)
{
  //Get the size of the string
  uint strSize = 0;
  if(!Get(strSize))
  {
    return false;
  }

  //Get the number of bytes on the buffer (rounded to 4)
  uint allocateSize = strSize;
  if(allocateSize % 4 != 0)
  {
    allocateSize += 4 - (allocateSize % 4);
  }
  
  //Check the size remaining
  if(bufferSizeLeft < allocateSize)
  {
    return false;
  }

  //Extract the string
  retValue.assign((char*)bufferOffset, strSize);

  //Mover the buffer offset
  bufferOffset = ((char*)bufferOffset) + allocateSize;
  bufferSizeLeft -= allocateSize;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool NetworkBufferReceive::Get(vector<int> &retArray)
{
  //Get the size of the array
  uint arraySize = 0;
  if(!Get(arraySize))
  {
    return false;
  }

  //Check the size remaining
  if(bufferSizeLeft < (arraySize*sizeof(int)))
  {
    return false;
  }
  
  //Init the return data
  retArray.clear();
  retArray.reserve(arraySize);
  int * accessIntBuffer = (int*)bufferOffset;

  //Copy the data to the return array
  for(uint i=0; i<arraySize; i++)
  {
    retArray.push_back(*accessIntBuffer);
    accessIntBuffer++;
  }
  
  //Mover the buffer offset
  bufferOffset = ((int*)bufferOffset) + arraySize;
  bufferSizeLeft -= (arraySize*sizeof(int));
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool NetworkBufferReceive::Get(vector<float> &retArray)
{
  //Get the size of the array
  uint arraySize = 0;
  if(!Get(arraySize))
  {
    return false;
  }

  //Check the size remaining
  if(bufferSizeLeft < (arraySize*sizeof(float)))
  {
    return false;
  }
  
  //Init the return data
  retArray.clear();
  retArray.reserve(arraySize);
  float * accessFloatBuffer = (float*)bufferOffset;

  //Copy the data to the return array
  for(uint i=0; i<arraySize; i++)
  {
    retArray.push_back(*accessFloatBuffer);
    accessFloatBuffer++;
  }
  
  //Mover the buffer offset
  bufferOffset = ((float*)bufferOffset) + arraySize;
  bufferSizeLeft -= (arraySize*sizeof(float));
  return true;
}


