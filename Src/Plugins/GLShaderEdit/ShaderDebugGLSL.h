/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SHADER_DEBUG_GLSL_H_
#define __SHADER_DEBUG_GLSL_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include "ReferenceCount.h"
#include "ShaderUtilsGLSL.h"
#include "ShaderParserGLSL.h"

#include <string>
#include <vector>

using namespace std;


class GLDriver;
class SEContext;
class GLRenderCallCache;
class GLFrameBufferCache;

//DT_TODO: Comments, class checks

//@
//  Summary:
//    This class debugs a OpenGL GLSL shader and retrieves uniforms/variables 
//    and the flow of the GLSL program.
//  
class ShaderDebugGLSL : public ReferenceCount
{
public:

  //@
  //  Summary:
  //    To create an initialize a GLSL shader debug interface.
  //  
  //  Parameters:
  //    callBacks      - The callbacks to GLIntercept.
  //
  //    debugProgramID - The shader program that is being debugged.
  //
  //    debugShaderSource - The shader source that is being debugged.
  //
  //    newRenderCache - The render cache to use.
  //
  //    newFrameCache  - The frame cache data. 
  //
  //  Returns:
  //    If a interface could be created and init, it is returned. 
  //    Else NULL is returned.
  //
  static ShaderDebugGLSL * CreateInstance(InterceptPluginCallbacks *callBacks, 
                                          GLuint debugProgramID, 
                                          const string & debugShaderSource, 
                                          GLRenderCallCache *newRenderCache,
                                          GLFrameBufferCache *newFrameCache);

  //@
  //  Summary:
  //    To start debugging the specified pixel location
  //  
  //  Parameters:
  //    xPos    - The x position of the pixel to debug.
  //
  //    xPos    - The y position of the pixel to debug.
  //
  //    pixelOverdraw - The overdrawn pixel number to debug. 
  //
  //    retLog        - The returned log of any errors/warnings during compile.
  //
  //  Returns:
  //    If pixel started debug, true is returned. Else false is returned.
  //
  bool SetPixelDebug(uint xPos, uint yPos, uint pixelOverdraw, string &retLog);

  //@
  //  Summary:
  //    To get the uniform data that is used in the debug shader.
  //  
  //  Returns:
  //    The uniform data that is used, is returned.
  //
  inline const UniformDataArray &GetUniformData();

  //@
  //  Summary:
  //    To get the source of the program being debugged.
  //  
  //  Returns:
  //    The debug program source is returned.
  //
  inline const string & GetProgramSource();

  //@
  //  Summary:
  //    To get the frame buffer cache.
  //  
  //  Returns:
  //    The frame buffer cache is returned.
  //
  inline const GLFrameBufferCache * GetFrameBufferCache() const;

protected:

  InterceptPluginCallbacks *gliCallBacks;         // The callback interface into GLIntercept
  const GLCoreDriver       *GLV;                  // The core OpenGL driver
  ShaderUtilsGLSL           shaderUtils;          // The shader utils 
  GLRenderCallCache        *renderCache;          // The render call cache for this debug session
  GLFrameBufferCache       *frameCache;           // The frame cache of the debug render call

  ShaderParserGLSL          fragmentShaderParser; // The shader parser used for fragment programs 

  GLuint                    programID;            // The program ID that is being debugged
  string                    programSource;        // The source of the program that is being debugged

  UniformDataArray          baseUniformData;      // The base uniform data from the origional program


  //@
  //  Summary:
  //    Constructor 
  //
  //  Parameters:
  //    gliCallBacks  - The callbacks to GLIntercept.
  //
  //    debugProgramID - The shader program that is being debugged.
  //
  //    debugShaderSource - The shader source that is being debugged.
  //
  ShaderDebugGLSL(InterceptPluginCallbacks *gliCallBacks, GLuint debugProgramID, const string & debugShaderSource);

  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~ShaderDebugGLSL();

  //@
  //  Summary:
  //    To initialize constants and arrays etc.
  //  
  //  Parameters:
  //    newRenderCache - The render cache to use.
  //
  //    newFrameCache  - The frame cache to use.
  //
  //  Returns:
  //    If initialization was successful, true is returned. Else false is returned.
  //
  bool Init(GLRenderCallCache *newRenderCache, GLFrameBufferCache *newFrameCache);

private:


};


///////////////////////////////////////////////////////////////////////////////
//
inline const UniformDataArray & ShaderDebugGLSL::GetUniformData()
{
  return baseUniformData;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const string & ShaderDebugGLSL::GetProgramSource()
{
  return programSource;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const GLFrameBufferCache * ShaderDebugGLSL::GetFrameBufferCache() const
{
  return frameCache;
}


#endif // __SHADER_DEBUG_GLSL_H_
