/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SHADER_MANAGER_ASM_H_
#define __SHADER_MANAGER_ASM_H_

#include <OSDefines.h>
#include "ShaderManager.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;

//@
//  Summary:
//    Structure of data about a OpenGL ASM shader
//  
class ShaderDataASM : public ShaderType
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //
  //  Parameters:
  //    glID  - The unique OpenGL ID of this list.
  // 
  ShaderDataASM(uint glID);

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
  //    To get the OpenGL shader type. Ensure the shader is ready
  //    (IsReady()) before calling or 0 will be returned. 
  //  
  //  Returns:
  //    The OpenGL shader type assigned is returned.
  //
  inline GLenum GetGLType() const;

private:

  GLenum glType;                                  // The OpenGL type of the shader

};

//@
//  Summary:
//    This class maintains a list of shader data and controls 
//    adding/deleting and retrival of ASM shader data.
//  
class ShaderManagerASM : public ShaderManager<ShaderDataASM>
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //  
	ShaderManagerASM();

  //@
  //  Summary:
  //    Destructor, destroys all array data and reports shader memory leaks.
  //  
  virtual ~ShaderManagerASM();

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
inline void ShaderDataASM::SetReady(GLenum type)
{
  ready  = true;
  glType = type;

}

///////////////////////////////////////////////////////////////////////////////
//
inline GLenum ShaderDataASM::GetGLType() const
{
  //Only return the stored type if a type has been set
  if(IsReady())
  {
    return glType;
  }

  return 0;
}



#endif // __SHADER_MANAGER_ASM_H_
