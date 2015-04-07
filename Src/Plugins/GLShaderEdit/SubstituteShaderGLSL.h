/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SUBSTITUTE_SHADER_GLSL_H_
#define __SUBSTITUTE_SHADER_GLSL_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include <ReferenceCount.h>
#include <AutoReferenceCount.h>
#include "ShaderUtilsGLSL.h"
#include "UniformBugManager.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;


//@
//  Summary:
//    This class provides the ability to replace an existing GLSL program
//    with another program. This is accomplished by binding the substitute
//    when the program to be replaced is bound during a render call. All
//    the uniforms are then copied from the old program to the new program.
//  
class SubstituteShaderGLSL : public ReferenceCount
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //
  //  Parameters:
  //    programID  - The OpenGL ID of the new substitute program. 
  //                 (must already have been successfully linked) 
  //
  //    gliCallBacks  - The callbacks to GLIntercept.
  //
  SubstituteShaderGLSL(GLuint programID,InterceptPluginCallbacks *gliCallBacks);

  //@
  //  Summary:
  //    To initialize this substitue (with the passed old program).
  //    This will retrieve all the old uniform offsets from the old 
  //    program and map them to the uniforms in the new program.
  //
  //  Parameters:
  //    oldProgram  - The OpenGL ID of the old program. 
  //
  //    initLog  - The string listing any errors/warnings that 
  //               occured during initialization.
  //
  //  Returns:
  //    True is returned if substitution was successful. Else false
  //    is returned.
  //
  bool Init(GLuint oldProgram, string &initLog);

  //@
  //  Summary:
  //    To bind the substitute GLSL program and copy all uniform data
  //    from the old program to the new program.
  //
  void BindSubstitute();

  //@
  //  Summary:
  //    To bind the old program that this substitute shader replaces.
  //
  void UnBindSubstitute();

  //@
  //  Summary:
  //    To destroy the substitute. This will prevent further Bind calls 
  //    from having effect. This is to be called when the old program 
  //    is deleted (this must be done in the same context that has the
  //    old program and that context must be active)
  //
  void DestroyProgram();

  //@
  //  Summary:
  //    To get the substitute program ID.
  //
  //  Returns:
  //    The current substitute program ID is returned.
  //
  inline GLuint GetSubstituteProgramID() const;


#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

public:

  UniformBugManagerRef uniformBugManager;   // ATI bug uniform manager for GLSL programs

#endif  //GLI_ATI_UNIFORM_GLSL_BUG

protected:

  GLuint  programID;                              // The handle to the OpenGL substitute program
  GLuint  oldProgramID;                           // The handle to the OpenGL program we are replacing

  ShaderUtilsGLSL           shaderUtils;          // The shader utils used to retrieve uniform values
  InterceptPluginCallbacks *gliCallBacks;         // The callback interface into GLIntercept

  UniformDataArray remapUniformArray;             // Array of re-mapped uniform data

  //Internal OpenGL function entry points
  void (GLAPIENTRY *iglUseProgram) (GLuint programObj);
  void (GLAPIENTRY *iglDeleteProgram) (GLuint object);

  //@
  //  Summary:
  //    Destructor, will not destroy the substitute program as 
  //    the context it was created in may no longer be valid.
  //  
  virtual ~SubstituteShaderGLSL();

  //@
  //  Summary:
  //    To get the remap array of uniform data from the old to the new program.
  //    (The old and new program IDs must be set before this is called)
  //
  //  Parameters:
  //    initLog  - The string log to record differences in the programs. 
  //
  void GenerateUniformRemapArray(string &initLog);

  //@
  //  Summary:
  //    To copy all the re-mapped uniform data from the old program to the
  //    new program. The new program must be bound before calling.
  //  
  void UniformDataCopy();

  //@
  //  Summary:
  //    To retrieve the data from the old program and test for OpenGL errors.
  //  
  //  Parameters:
  //    remapData  - The info about the uniform data to copy. 
  //
  //    retLog     - The log of any errors detected.
  //
  //  Returns:
  //    True is returned if no errors were detected. Else false is returned.
  //
  bool TestUniformDataCopy(UniformData &remapData, string &retLog) const;

};

///////////////////////////////////////////////////////////////////////////////
//
inline GLuint SubstituteShaderGLSL::GetSubstituteProgramID() const
{
  return programID;
}

//Auto reference count type def for substitute shaders
typedef AutoReferenceCount<SubstituteShaderGLSL> SubShaderGLSLRef;



#endif // __SUBSTITUTE_SHADER_GLSL_H_
