/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __UNIFORM_BUG_MANAGER_H_
#define __UNIFORM_BUG_MANAGER_H_

//Define to use this hack bugfix
#define GLI_ATI_UNIFORM_GLSL_BUG

#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

#include "../../MainLib/InterceptPluginInterface.h"
#include <ReferenceCount.h>
#include <AutoReferenceCount.h>
#include <map>

using namespace std;

class GLDriver;


//@
//  Summary:
//    This class stores the values for uniform sampler data that needs to 
//    be stored manually due to an ATI bug. Remove this class in future.
//  
class UniformBugManager : public ReferenceCount
{
public:

  //@
  //  Summary:
  //    Constructor
  //
  inline UniformBugManager();

  //@
  //  Summary:
  //    To set the uniform value for a location.
  //
  //  Parameters:
  //    offset  - The uniform offset in the program. 
  //
  //    data    - The new uniform data. 
  //
  inline void SetUniformData(GLuint offset, GLint data);

  //@
  //  Summary:
  //    To get the uniform value for a location.
  //
  //  Parameters:
  //    offset  - The uniform offset in the program. 
  //
  //  Returns:
  //    The uniform value is returned. If the value does not exist, zero
  //    is returned.    
  //
  inline GLint GetUniformData(GLuint offset);

protected:

  typedef map<GLuint, GLint, less<GLuint> > UniformDataArray; // Type of the data array
  UniformDataArray uniformDataArray;                          // Array of stored uniform data

  //@
  //  Summary:
  //    Destructor
  //
  inline virtual ~UniformBugManager();

};

//Auto reference count type def for substitute shaders
typedef AutoReferenceCount<UniformBugManager> UniformBugManagerRef;


///////////////////////////////////////////////////////////////////////////////
//
inline UniformBugManager::UniformBugManager()
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline UniformBugManager::~UniformBugManager()
{

}


///////////////////////////////////////////////////////////////////////////////
//
inline void UniformBugManager::SetUniformData(GLuint offset, GLint data)
{
  //Find the existing data
  UniformDataArray::iterator find = uniformDataArray.find(offset);
  if(find == uniformDataArray.end())
  {
    //Add the new data to the array
    UniformDataArray::value_type insertData(offset,data);
    uniformDataArray.insert(insertData);

    return;
  }

  //Assign the new data to the existing data
  find->second = data;
}

///////////////////////////////////////////////////////////////////////////////
//
inline GLint UniformBugManager::GetUniformData(GLuint offset)
{
  UniformDataArray::iterator find = uniformDataArray.find(offset);
  if(find == uniformDataArray.end())
  {
    return 0;
  }

  return find->second;
}


#endif // GLI_ATI_UNIFORM_GLSL_BUG

#endif // __UNIFORM_BUG_MANAGER_H_
