/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "ShaderDebugGLSL.h"
#include "MiscUtils.h"
#include "SubstituteShaderGLSL.h"
#include "SEContext.h"
#include "GLRenderCallCache.h"
#include <CommonErrorLog.h>

USING_ERRORLOG


///////////////////////////////////////////////////////////////////////////////
//
ShaderDebugGLSL::ShaderDebugGLSL(InterceptPluginCallbacks *callBacks, GLuint debugProgramID, const string & debugShaderSource):
gliCallBacks(callBacks),
GLV(callBacks->GetCoreGLFunctions()),

shaderUtils(callBacks),
renderCache(NULL),
frameCache(NULL),

programID(debugProgramID),
programSource(debugShaderSource)
{

}

///////////////////////////////////////////////////////////////////////////////
//
ShaderDebugGLSL::~ShaderDebugGLSL()
{
  //Shutdown the render cache
  if(renderCache)
  {
    renderCache->ReleaseReference();
    renderCache = NULL;
  }

  //Shutdown the frame cache
  if(frameCache)
  {
    frameCache->ReleaseReference();
    frameCache = NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderDebugGLSL::Init(GLRenderCallCache *newRenderCache, GLFrameBufferCache *newFrameCache)
{
  //Check the cache data
  if(newRenderCache == NULL || newFrameCache == NULL)
  {
    return false;
  }

  //If an invalid program
  if(programID == 0 || programSource.length() == 0)
  {
    return false;
  }
  
  //Init the shader utils
  if(!shaderUtils.Init())
  {
    return false;
  }

  //Return if we cannot make OpenGL calls
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return false;
  }

  //Test extension support
  float glVersion = gliCallBacks->GetGLVersion();
  if(glVersion < 2.0f)
  {
    LOGERR(("ShaderEditGLSL::Init - Need OpenGL2.0 implementation to use the GLSL shader debug"));
    return false;
  }
 
  //Test for OpenGL errors during init
  // First clear any error state
  if(gliCallBacks->GetCoreGLFunctions()->glGetError() != GL_NO_ERROR)
  {
    LOGERR(("ShaderEditGLSL::Init - Existing OpenGL error? Fix Application!"));
  }

  //Assign the render cache
  renderCache = newRenderCache;
  renderCache->AddReference();

  //Assign the frame cache
  frameCache = newFrameCache; 
  frameCache->AddReference();

  //Get the uniform data from the base program
  if(!shaderUtils.GetUniformData(programID, baseUniformData))
  {
    LOGERR(("ShaderEditGLSL::Init - Unable to get uniform data from the origional program"));
    return false;
  }

  //Get all the uniform values
  for(uint i=0; i<baseUniformData.size(); i++)
  {
    shaderUtils.GetUniformValues(programID, baseUniformData[i]);
  }

  //Test compiling the shader source

  //If a error occured during init
  if(gliCallBacks->GetCoreGLFunctions()->glGetError() != GL_NO_ERROR)
  {
    LOGERR(("ShaderEditGLSL::Init - OpenGL error on Init - (broken drivers?)\n"));
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
ShaderDebugGLSL *ShaderDebugGLSL::CreateInstance(InterceptPluginCallbacks *callBacks, 
                                                 GLuint debugProgramID, const string & debugShaderSource,
                                                 GLRenderCallCache *newRenderCache,
                                                 GLFrameBufferCache *newFrameCache)
{
  //Create the new shader edit
  ShaderDebugGLSL * newShaderDebug = new ShaderDebugGLSL(callBacks, debugProgramID, debugShaderSource);

  //Attempt to init
  if(!newShaderDebug->Init(newRenderCache, newFrameCache))
  {
    LOGERR(("ShaderDebugGLSL::CreateInstance - Unable to init new GLSL shader debugger"));
    newShaderDebug->ReleaseReference();
    return NULL;
  }

  return newShaderDebug;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderDebugGLSL::SetPixelDebug(uint xPos, uint yPos, uint pixelOverdraw, string &retLog)
{
  // If already debugging a pixel, discard the data

  // Get if the shader has already been processed
/*
  if(!fragmentShaderParser.IsValid())
  {
    // Extract the fragment shader from the program source
    string fragmentShaderSource;
    uint fragmentShaderLineStart;
    if(!ShaderUtilsGLSL::ExtractShaderString("[Fragment Shader]", programSource, fragmentShaderLineStart, fragmentShaderSource))
    {
      retLog += "Unable to extract fragment shader source string";
      return false;
    }

    // Process the shader source
    if(!fragmentShaderParser.Init(GL_FRAGMENT_SHADER, fragmentShaderSource))
    {
      retLog += "Unable to process fragment shader source string";
      return false;
    }
  }
*/

  // Check the x/y pixel locations

  // Check that a valid draw call was used

  // Check that the pixel shader can be extracted, processed and compiled

  // Extract the number of times each line is passed

  // Determine the flow control paths

  return true;
}




