/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SHADER_MANAGER_H_
#define __SHADER_MANAGER_H_

#include <map>
#include <string>
#include <CommonErrorLog.h>

#include "UniqueShaderID.h"

using namespace std;

USING_ERRORLOG


#define TEMPLATE    template <class T>
#define QUAL        ShaderManager<T>



//@
//  Summary:
//    Structure of data about a OpenGL shader data type
//  
class ShaderType
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //
  //  Parameters:
  //    glID  - The unique OpenGL ID of this type.
  // 
  inline ShaderType(uint glID);

  //@
  //  Summary:
  //    Destructor
  //  
  inline virtual ~ShaderType();

  //@
  //  Summary:
  //    To return true if the data is ready. 
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

  //@
  //  Summary:
  //    To get the shader unique ID.
  //  
  //  Returns:
  //    The unique ID of this shader is returned.
  //
  inline UniqueShaderIDRef GetUniqueID();
  inline const UniqueShaderIDRef GetUniqueID() const;

  //@
  //  Summary:
  //    To set the shaders' source string.
  //  
  inline void SetShaderSource(const string &newSource);

protected:

  uint   id;         // The OpenGL ID
  bool   ready;      // Flag to indicate if the data is ready (ie. has been uploaded)  

  UniqueShaderIDRef shaderUID;  // A unique ID for this shader (contexts can have the same GL ID's)

};

///////////////////////////////////////////////////////////////////////////////
//
inline ShaderType::ShaderType(uint glID):
id(glID),
ready(false)
{
  //Create a new unique ID
  UniqueShaderID * newID = new UniqueShaderID(glID);
  
  //Attach to the auto reference object
  shaderUID.Attach(newID);
}

///////////////////////////////////////////////////////////////////////////////
//
inline ShaderType::~ShaderType()
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline uint ShaderType::GetGLID() const
{
  return id;
}


///////////////////////////////////////////////////////////////////////////////
//
inline bool ShaderType::IsReady() const
{
  return ready;
}


///////////////////////////////////////////////////////////////////////////////
//
inline UniqueShaderIDRef ShaderType::GetUniqueID()
{
  return shaderUID;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const UniqueShaderIDRef ShaderType::GetUniqueID() const
{
  return shaderUID;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderType::SetShaderSource(const string &newSource)
{
  shaderUID->SetShaderSource(newSource);
}


//@
//  Summary:
//    This class template maintains a list data and controls 
//    adding/deleting and retrival of the shader data.
//
//    The "T" type is the type of data to track. 
//  
template <class T>
class ShaderManager
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //  
	ShaderManager();

  //@
  //  Summary:
  //    Destructor, destroys all array data and reports any leaks.
  //  
  virtual ~ShaderManager();

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
QUAL::ShaderManager()
{
}

///////////////////////////////////////////////////////////////////////////////
//
TEMPLATE
QUAL::~ShaderManager()
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


#undef TEMPLATE
#undef QUAL


#endif // __SHADER_MANAGER_H_
