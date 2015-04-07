/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SHADER_GLSL_MANAGER_H_
#define __SHADER_GLSL_MANAGER_H_

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
//    Structure of data about a OpenGL GLSL shader/program
//  
class ShaderGLSLData : public InterceptDataType
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //
  //  Parameters:
  //    glID  - The unique OpenGL ID of this list.
  // 
  ShaderGLSLData(uint glID);

  //@
  //  Summary:
  //    Destructor
  // 
  virtual ~ShaderGLSLData();

  //@
  //  Summary:
  //    To flag the shader as ready and sets that the shader can now be saved.
  //    (This call also flags the shader as dirty)
  //  
  inline void SetReady();

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
  //    To get the OpenGL shader type. 
  //  
  //  Returns:
  //    The OpenGL shader type assigned is returned.
  //
  inline GLenum GetGLType() const;

  //@
  //  Summary:
  //    To set the OpenGL shader type.
  //  
  //  Parameters:
  //    type  - The type of the shader. 
  //
  inline void SetGLType(GLenum type);

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

  //@
  //  Summary:
  //    To get the shaders' log string.
  //  
  //  Returns:
  //    The string containing the shaders' source is returned.
  //
  inline string & GetShaderLog();
  inline const string & GetShaderLog() const;

  //@
  //  Summary:
  //    To attach the passed handle from the array of attached objects.
  //  
  //  Parameters:
  //    handle  - The handle of the object to attach.
  //
  void AttachObject(GLhandle newHandle);

  //@
  //  Summary:
  //    To detach the passed handle from the array of attached objects.
  //  
  //  Parameters:
  //    handle  - The handle of the object to detach.
  //
  //  Returns:
  //    If the object was detached, true is returned, Else false is returned.
  //
  bool DetachObject(GLhandle handle);

  //@
  //  Summary:
  //    To get a reference to this objects' attachment array.
  //  
  //  Returns:
  //    The string containing the shaders' source is returned.
  //
  inline const vector<GLhandle> & GetAttachmentArray() const;

  //@
  //  Summary:
  //    To flag that the user has now deleted this object 
  //    (but may still be attached to a program)
  //  
  inline void SetUserDeleted();

  //@
  //  Summary:
  //    To get if a user has deleted this object. 
  //    (may still exist when attached to a program)
  //  
  //  Returns:
  //    The flag indicating if the user has deleted this object is returned.
  //
  inline bool GetUserDeleted() const;

private:

  GLenum glType;       // The OpenGL type of the shader
  bool   userDeleted;  // Flag to indicate if the user has deleted this shader


  string saveFileName; // File name that was used when the shader was last saved.
  string shaderSource; // The source string of the shader

  string shaderLog;    // The shader log file

  vector<GLhandle> attachArray;    //List of all the objects attached to this program/shader object
};

//@
//  Summary:
//    This class maintains a list of shader data and controls 
//    adding/deleting and retrival of shader data.
//  
class ShaderGLSLManager : public InterceptDataManager<ShaderGLSLData>
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //  
	ShaderGLSLManager();

  //@
  //  Summary:
  //    Destructor, destroys all array data and reports shader memory leaks.
  //  
  virtual ~ShaderGLSLManager();

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
inline void ShaderGLSLData::SetReady()
{
  ready  = true;

  //Set as dirty
  SetDirty(true);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderGLSLData::SetDirty(bool flag)
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
inline void ShaderGLSLData::SetGLType(GLenum type)
{
  glType = type;
}

///////////////////////////////////////////////////////////////////////////////
//
inline GLenum ShaderGLSLData::GetGLType() const
{
  return glType;
}

///////////////////////////////////////////////////////////////////////////////
//
inline string & ShaderGLSLData::GetShaderSaveFileName()
{
  return saveFileName;
}

///////////////////////////////////////////////////////////////////////////////
//
inline string & ShaderGLSLData::GetShaderSource()
{
  return shaderSource;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const string & ShaderGLSLData::GetShaderSource() const
{
  return shaderSource;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const vector<GLhandle> & ShaderGLSLData::GetAttachmentArray() const
{
  return attachArray;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderGLSLData::SetUserDeleted()
{
  userDeleted = true;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool ShaderGLSLData::GetUserDeleted() const
{
  return userDeleted;
}

///////////////////////////////////////////////////////////////////////////////
//
inline string & ShaderGLSLData::GetShaderLog()
{
  return shaderLog;
}


///////////////////////////////////////////////////////////////////////////////
//
inline const string & ShaderGLSLData::GetShaderLog() const
{
  return shaderLog;
}


#endif // __SHADER_GLSL_MANAGER_H_
