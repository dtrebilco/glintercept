/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "ShaderManagerGLSL.h"


USING_ERRORLOG


//Set the class name for the shader manager
template<>
char *ShaderManager<class ShaderDataGLSL>::className = "ShaderGLSL";

///////////////////////////////////////////////////////////////////////////////
//
ShaderDataGLSL::ShaderDataGLSL(uint glID):
ShaderType(glID),
glType(0),
userDeleted(false)
{

#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

  //Create the new uniform bug manager
  UniformBugManager * newUniManager = new UniformBugManager();
  uniformBugManager.Attach(newUniManager);

#endif  //GLI_ATI_UNIFORM_GLSL_BUG

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderDataGLSL::AttachObject(GLuint newHandle)
{
  //Attempt to find the object
  for(uint i=0;i<attachArray.size();i++)
  {
    //If already attached, just return
    if(attachArray[i] == newHandle)
    {
      return;
    }
  }

  //Add to the array
  attachArray.push_back(newHandle);
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderDataGLSL::DetachObject(GLuint handle)
{
  //Attempt to find the object
  for(uint i=0;i<attachArray.size();i++)
  {
    //If equal, remove and return
    if(attachArray[i] == handle)
    {
      attachArray.erase(attachArray.begin() + i);
      return true;
    }
  }

  //Not found
  return false;
}



///////////////////////////////////////////////////////////////////////////////
//
ShaderManagerGLSL::ShaderManagerGLSL()
{
  //Add a shader (the zero shader always exists)
  AddData(0);

}

///////////////////////////////////////////////////////////////////////////////
//
ShaderManagerGLSL::~ShaderManagerGLSL()
{
  //Remove the 0 shader
  ShaderManager<ShaderDataGLSL>::RemoveData(0);
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderManagerGLSL::RemoveData(uint glId)
{
  //Never delete 0
  if(glId != 0)
  {
    return ShaderManager<ShaderDataGLSL>::RemoveData(glId);
  }

  return true;
}





