/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "ShaderManagerASM.h"


USING_ERRORLOG


//Set the class name for the shader manager
template<>
char *ShaderManager<class ShaderDataASM>::className = "ShaderASM";

///////////////////////////////////////////////////////////////////////////////
//
ShaderDataASM::ShaderDataASM(uint glID):
ShaderType(glID),
glType(0)
{
}

///////////////////////////////////////////////////////////////////////////////
//
ShaderManagerASM::ShaderManagerASM()
{
  //Add a shader (the zero shader always exists)
  AddData(0);

}

///////////////////////////////////////////////////////////////////////////////
//
ShaderManagerASM::~ShaderManagerASM()
{
  //Remove the 0 shader
  ShaderManager<ShaderDataASM>::RemoveData(0);
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderManagerASM::RemoveData(uint glId)
{
  //Never delete 0
  if(glId != 0)
  {
    return ShaderManager<ShaderDataASM>::RemoveData(glId);
  }

  return true;
}





