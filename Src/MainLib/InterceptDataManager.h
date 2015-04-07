/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_DATA_MANAGER_H_
#define __INTERCEPT_DATA_MANAGER_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"

#include <map>
#include <string>

using namespace std;

USING_ERRORLOG

class GLDriver;

#define TEMPLATE    template <class T>
#define QUAL        InterceptDataManager<T>



//@
//  Summary:
//    Structure of data about a OpenGL data type
//  
class InterceptDataType
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //
  //  Parameters:
  //    glID  - The unique OpenGL ID of this type.
  // 
  inline InterceptDataType(uint glID);

  //@
  //  Summary:
  //    Destructor
  //  
  inline virtual ~InterceptDataType();

  //@
  //  Summary:
  //    To return true if the data is dirty. 
  //    (ie. has changed since last save)
  //  
  inline bool IsDirty() const;

  //@
  //  Summary:
  //    To return true if the data is ready to be saved. 
  //    (ie. has been initilized with some data)
  //  
  inline bool IsReady() const;

  //@
  //  Summary:
  //    To get the OpenGL data ID of this list. Ensure the data 
  //    is ready (IsReady()) before calling. 
  //  
  //  Returns:
  //    The OpenGL data ID assigned to this data type is returned.
  //
  inline uint GetGLID() const;


protected:

  uint   id;         // The OpenGL ID

  bool   ready;      // Flag to indicate if the data is ready (ie. has been uploaded)  
  bool   dirty;      // Flag to indicate if the data is dirty 

};

///////////////////////////////////////////////////////////////////////////////
//
inline InterceptDataType::InterceptDataType(uint glID):
id(glID),
ready(false),
dirty(false)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline InterceptDataType::~InterceptDataType()
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline uint InterceptDataType::GetGLID() const
{
  return id;
}


///////////////////////////////////////////////////////////////////////////////
//
inline bool InterceptDataType::IsDirty() const
{
  return dirty;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool InterceptDataType::IsReady() const
{
  return ready;
}


//@
//  Summary:
//    This class template maintains a list data and controls 
//    adding/deleting and retrival of the data.
//
//    The "T" type is the type of data to track. 
//  
template <class T>
class InterceptDataManager
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //  
	InterceptDataManager();

  //@
  //  Summary:
  //    Destructor, destroys all array data and reports any leaks.
  //  
  virtual ~InterceptDataManager();

  //@
  //  Summary:
  //    To get all the data that have been flagged as valid,ready and dirty. 
  //  
  //  Parameters:
  //    dirtyData  -  The array to return all the dirty data in.
  //
  void GetAllDirtyData(vector<T *> &dirtyData);
 
  //@
  //  Summary:
  //    To set all the data that is currently being tracked, as 
  //    dirty. (ie. needs saving)
  //  
  void SetAllDataDirty();

  //@
  //  Summary:
  //    To add a new data (OpenGL) ID to the list. The data 
  //    added needs to be set to a "ready" state before it can be used.
  //  
  //  Parameters:
  //    glID  -  The OpenGL data ID of the data to add.
  //
  void AddData(uint glID);

  //@
  //  Summary:
  //    To remove an data instance from the data list.
  //  
  //  Parameters:
  //    glID  -  The OpenGL data ID of the data to remove.
  //
  bool RemoveData(uint glID);

  //@
  //  Summary:
  //    To get the data of the passed OpenGL data ID.
  //  
  //  Parameters:
  //    glID  -  The OpenGL data ID of the data to find.
  //
  //  Returns:
  //    If the data is found, it is returned. Else NULL is returned.
  //
  T * GetData(GLuint glID);

  //@
  //  Summary:
  //    To get the data of the passed OpenGL data ID. (const version)
  //  
  //  Parameters:
  //    glID  -  The OpenGL data ID of the data to find.
  //
  //  Returns:
  //    If the data is found, it is returned. Else NULL is returned.
  //
  const T * GetData(GLuint glID) const;


protected:

  typedef map<GLuint, T, less<GLuint> > DataArrayType; // Type of the data array

  DataArrayType dataArray;                        // Array of data already loaded

  static char * className;                        // String based name of this class instance

};

///////////////////////////////////////////////////////////////////////////////
//
TEMPLATE
QUAL::InterceptDataManager()
{
}

///////////////////////////////////////////////////////////////////////////////
//
TEMPLATE
QUAL::~InterceptDataManager()
{
  //Loop for all data and log all data still active
  for(typename DataArrayType::const_iterator i=dataArray.begin(); i!=dataArray.end(); ++i)
  {
    LOGERR(("%sManager::Destructor - OpenGL id %u is still active. (%s Memory leak?)",className,i->second.GetGLID(),className));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
TEMPLATE
void QUAL::GetAllDirtyData(vector<T *> &dirtyData) 
{
  //Empty the array
  dirtyData.clear();

  //Loop for all data
  for(typename DataArrayType::iterator i=dataArray.begin(); i!=dataArray.end(); ++i)
  {
    //If the data is ready and dirty, save it
    if(i->second.IsReady() &&
       i->second.IsDirty())
    { 
      //Add the data to the dirty array
      dirtyData.push_back(&(i->second));
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
TEMPLATE
void QUAL::AddData(uint glID)
{
  //Check that the ID is not already in use
  T * currData = GetData(glID);

  //Test if there is a existing entry in the array
  if(currData)
  {
    LOGERR(("%sManager::AddData - Existing %s ID %u",className,className,glID));
  }
  else
  {
    //Add a new ID to the array
    T newData(glID);
    typename DataArrayType::value_type insertData(glID,newData);
    dataArray.insert(insertData);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
TEMPLATE
bool QUAL::RemoveData(uint glID)
{
  //Erase the element
  if(dataArray.erase(glID) == 1)
  {
    return true;
  }

  LOGERR(("%sManager::RemoveData - Attempting to delete unknown %s ID %u",className,className,glID));
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
TEMPLATE
T * QUAL::GetData(GLuint glID)
{
  //Search the array 
  typename DataArrayType::iterator find = dataArray.find(glID);
  if(find == dataArray.end())
  {
    return NULL;
  }

  return &(find->second);
}

///////////////////////////////////////////////////////////////////////////////
//
TEMPLATE
const T * QUAL::GetData(GLuint glID) const
{
  //Search the array 
  typename DataArrayType::const_iterator find = dataArray.find(glID);
  if(find == dataArray.end())
  {
    return NULL;
  }

  return &(find->second);
}

///////////////////////////////////////////////////////////////////////////////
//
TEMPLATE
void QUAL::SetAllDataDirty()
{
  //Loop for all data
  for(typename DataArrayType::iterator i=dataArray.begin(); i!=dataArray.end(); ++i)
  {
    //If the data is ready set it as dirty
    if(i->second.IsReady())
    {
      i->second.SetDirty(true); 
    }
  }

}

#undef TEMPLATE
#undef QUAL


#endif // __INTERCEPT_DATA_MANAGER_H_
