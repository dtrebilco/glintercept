/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SHADER_MANAGER_GLSL_H_
#define __SHADER_MANAGER_GLSL_H_

#include <OSDefines.h>
#include "ShaderManager.h"
#include "UniformBugManager.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;

//@
//  Summary:
//    Structure of data about a OpenGL GLSL shader
//  
class ShaderDataGLSL : public ShaderType
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //
  //  Parameters:
  //    glID  - The unique OpenGL ID of this list.
  // 
  ShaderDataGLSL(uint glID);

  //@
  //  Summary:
  //    To flag the shader as ready and sets that the shader can now be saved.
  //    (This call also flags the shader as dirty)
  //  
  inline void SetReady();

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
  //    To set the OpenGL shader type.
  //  
  //  Parameters:
  //    type  - The type of the shader. 
  //
  inline void SetGLType(GLenum type);

  //@
  //  Summary:
  //    To attach the passed handle from the array of attached objects.
  //  
  //  Parameters:
  //    handle  - The handle of the object to attach.
  //
  void AttachObject(GLuint newHandle);

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
  bool DetachObject(GLuint handle);

  //@
  //  Summary:
  //    To get a reference to this objects' attachment array.
  //  
  //  Returns:
  //    The string containing the shaders' source is returned.
  //
  inline const vector<GLuint> & GetAttachmentArray() const;

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

  GLenum glType;                                  // The OpenGL type of the shader
  bool   userDeleted;                             // Flag to indicate if the user has deleted this shader
  
  vector<GLuint> attachArray;                     // List of all the objects attached to this program/shader object

#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

public:

  UniformBugManagerRef uniformBugManager;         // ATI bug uniform manager for GLSL programs

#endif  //GLI_ATI_UNIFORM_GLSL_BUG

};

//@
//  Summary:
//    This class maintains a list of shader data and controls 
//    adding/deleting and retrival of GLSL shader data.
//  
class ShaderManagerGLSL : public ShaderManager<ShaderDataGLSL>
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //  
	ShaderManagerGLSL();

  //@
  //  Summary:
  //    Destructor, destroys all array data and reports shader memory leaks.
  //  
  virtual ~ShaderManagerGLSL();

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
inline void ShaderDataGLSL::SetReady()
{
  ready  = true;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderDataGLSL::SetGLType(GLenum type)
{
  glType = type;

  //Set the UID type 
  switch(type)
  {
    case(GL_PROGRAM_OBJECT_ARB):
      shaderUID->SetShaderType(USID_GLSLProgram);
      break;

    case(GL_VERTEX_SHADER):
      shaderUID->SetShaderType(USID_GLSLVertexShader);
      break;

    case(GL_GEOMETRY_SHADER):
      shaderUID->SetShaderType(USID_GLSLGeometryShader);
      break;

    case(GL_FRAGMENT_SHADER):
      shaderUID->SetShaderType(USID_GLSLFragmentShader);
      break;
  }
}
 

///////////////////////////////////////////////////////////////////////////////
//
inline GLenum ShaderDataGLSL::GetGLType() const
{
  return glType;
}


///////////////////////////////////////////////////////////////////////////////
//
inline const vector<GLuint> & ShaderDataGLSL::GetAttachmentArray() const
{
  return attachArray;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ShaderDataGLSL::SetUserDeleted()
{
  userDeleted = true;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool ShaderDataGLSL::GetUserDeleted() const
{
  return userDeleted;
}



#endif // __SHADER_MANAGER_GLSL_H_
