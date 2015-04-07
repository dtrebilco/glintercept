/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SHADER_MANAGER_H_
#define __SHADER_MANAGER_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"
#include "InterceptDataManager.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;


//@
//  Summary:
//    Structure of data about a OpenGL shader
//  
class ShaderData : public InterceptDataType
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //
  //  Parameters:
  //    glID  - The unique OpenGL ID of this list.
  // 
  ShaderData(uint glID);

  //@
  //  Summary:
  //    To flag the shader as ready and sets that the shader can now be saved.
  //    (This call also flags the shader as dirty)
  //  
  //  Parameters:
  //    type  - The type of the shader that is now ready.
  //
  inline void SetReady(GLenum type);

  //@
  //  Summary:
  //    To set if the shader is dirty or not. This flag should be set when the 
  //    OpenGL shader changes and un-set when the shader is written out.
  //  
  //  Parameters:
  //    flag  - Flag to indicate if this OpenGL shader is now dirty or not.
  //
  inline void SetDirty(bool flag);

  //@
  //  Summary:
  //    To get the OpenGL shader type. Ensure the shader is ready
  //    (IsReady()) before calling or 0 will be returned. 
  //  
  //  Returns:
  //    The OpenGL shader type assigned is returned.
  //
  inline GLenum GetGLType() const;

  //@
  //  Summary:
  //    To get a unique shader filename (including type,shaderID and savecount)
  //    without any extension. 
  //  
  //  Parameters:
  //    retString  - The string to return the unique name in.
  //
  //  Returns:
  //    If a unique string could be generated, true is returned. 
  //    Else false is returned.
  //
  bool GetUniqueFileName(string &retString) const;

  //@
  //  Summary:
  //    To get a reference to the filename that was used to save the 
  //    shader data out previously.
  //  
  //  Returns:
  //    A reference to the filename of saved shader data is returned.
  //
  inline string & GetShaderSaveFileName();


  //@
  //  Summary:
  //    To get the shaders' source string.
  //  
  //  Returns:
  //    The string containing the shaders' source is returned.
  //
  inline string & GetShaderSource();
  inline const string & GetShaderSource() const;

private:

  GLenum glType;     // The OpenGL type of the shader

  string saveFileName; // File name that was used when the shader was last saved.
  string shaderSource; // The source string of the shader

};

//@
//  Summary:
//    This class maintains a list of shader data and controls 
//    adding/deleting and retrival of shader data.
//  
class ShaderManager : public InterceptDataManager<ShaderData>
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //  
	ShaderManager();

  //@
  //  Summary:
  //    Destructor, destroys all array data and reports shader memory leaks.
  //  
  virtual ~ShaderManager();

  //@
  //  Summary:
  //    To remove a shader from the shader list.
  //  
  //  Parameters:
  //    glId  -  The OpenGL shader ID of the shader to remove.
  //
  bool RemoveData(uint glId);

};


///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderData::SetReady(GLenum type)
{
  ready  = true;
  glType = type;

  //Set as dirty
  SetDirty(true);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderData::SetDirty(bool flag)
{
  //Chcek if the flag is different
  if(flag != dirty && ready)
  {
    //Assign the dirty flag
    dirty = flag;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
inline GLenum ShaderData::GetGLType() const
{
  //Only return the stored type if a type has been set
  if(IsReady())
  {
    return glType;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
inline string & ShaderData::GetShaderSaveFileName()
{
  return saveFileName;
}

///////////////////////////////////////////////////////////////////////////////
//
inline string & ShaderData::GetShaderSource()
{
  return shaderSource;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const string & ShaderData::GetShaderSource() const
{
  return shaderSource;
}


#endif // __SHADER_MANAGER_H_
