/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __NETWORK_UTILS_H__
#define __NETWORK_UTILS_H__

#include <string>
#include <vector>
#include "OSDefines.h"

using namespace std;
 
//@
//  Summary:
//    This class provides a buffer to pack data into for sending over a 
//    network. All data is stored at 4 byte offsets.
//  
class NetworkBufferSend
{
public:

  //@
  //  Summary:
  //    Constructor.
  //  
  //  Parameters:
  //    initialSize - The estimated size of the network buffer. 
  //                  (The final buffer can grow beyond this size)
  //
  inline NetworkBufferSend(uint initialSize);

  //@
  //  Summary:
  //    Destructor.
  //  
  inline virtual ~NetworkBufferSend();

  //@
  //  Summary:
  //    Add a value to the stored network buffer. All data is stored at 4 byte
  //    offsets.
  //  
  //  Parameters:
  //    intValue - The int value to add.
  //
  //    uintValue - The uint value to add.
  //
  //    strValue  - The string value to add. 
  //                (Dummy spaces are inserted to make 4-byte boundaries)
  //
  //    arraySize - The size of the array to add.
  //
  //    addArray  - The array to add.
  //
  void Add(int intValue);
  void Add(uint uintValue);
  void Add(const string &strValue);
  void Add(uint arraySize, const int *addArray);
  void Add(uint arraySize, const float *addArray);

  //@
  //  Summary:
  //    To get the current network buffer size.
  //  
  //  Returns:
  //    The network buffer size is returned.
  //
  inline uint GetBufferSize() const;

  //@
  //  Summary:
  //    To get the current network buffer. Note that this pointer should not
  //    be stored and any further calls to Add() will invalidate it.
  //  
  //  Returns:
  //    The network buffer size is returned.
  //
  inline const void * GetBuffer() const;

protected:

  vector<char> sendBuffer;                        // An array of bytes to send over the network

};


//@
//  Summary:
//    This class provides read support for buffers that were created with 
//    NetworkBufferSend.
//  
class NetworkBufferReceive
{
public:

  //@
  //  Summary:
  //    Constructor.
  //  
  //  Parameters:
  //    bufferSize - The size of the buffer.
  //
  //    newBuffer  - The network buffer to read from.
  //
  NetworkBufferReceive(uint bufferSize, const void *newBuffer);

  //@
  //  Summary:
  //    Destructor.
  //  
  virtual ~NetworkBufferReceive();

  //@
  //  Summary:
  //    To get a value/array from the network buffer.
  //  
  //  Parameters:
  //    retValue - The value to retrieve.
  //
  //  Returns:
  //    If the value could be retrieved, true is returned, Else false is 
  //    returned.
  //
  bool Get(int &retValue);
  bool Get(uint &retValue);
  bool Get(string &retValue);
  bool Get(vector<int> &retArray);
  bool Get(vector<float> &retArray);

  //@
  //  Summary:
  //    To get the number of bytes left for reading in the buffer.
  //  
  //  Returns:
  //    The number of bytes left are returned.
  //
  inline uint GetBufferSizeLeft() const;

protected:

  uint         bufferSizeLeft;                    // The size left in the network buffer
  const void * bufferOffset;                      // The current buffer offset 

};


///////////////////////////////////////////////////////////////////////////////
//
inline NetworkBufferSend::NetworkBufferSend(uint initialSize):
sendBuffer()
{
  sendBuffer.reserve(initialSize);
}

///////////////////////////////////////////////////////////////////////////////
//
inline NetworkBufferSend::~NetworkBufferSend()
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint NetworkBufferSend::GetBufferSize() const
{
  return (uint)sendBuffer.size();
}

///////////////////////////////////////////////////////////////////////////////
//
inline const void * NetworkBufferSend::GetBuffer() const
{
  return &sendBuffer[0];
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint NetworkBufferReceive::GetBufferSizeLeft() const
{
  return bufferSizeLeft;
}

#endif //__NETWORK_UTILS_H__
