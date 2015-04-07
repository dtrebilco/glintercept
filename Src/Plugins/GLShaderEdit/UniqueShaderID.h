/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __UNIQUE_SHADER_ID_H_
#define __UNIQUE_SHADER_ID_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include <ReferenceCount.h>
#include <AutoReferenceCount.h>

#include "SubstituteShaderGLSL.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;

//Types of shaders handled
enum USIDType
{
  USID_Undefined,           // Undefined shader type
 
  USID_GLSLVertexShader,    // GLSL vertex shader
  USID_GLSLGeometryShader,  // GLSL geometry shader
  USID_GLSLFragmentShader,  // GLSL fragment shader
  USID_GLSLProgram,         // GLSL program

  USID_ARB_VP,              // ARB vertex program
  USID_ARB_FP,              // ARB fragment program

  USID_NV_VP,               // NV vertex program 
  USID_NV_FP                // NV fragment program
};

//@
//  Summary:
//    This class stores data about a OpenGL shader type.
//    Each instance of this class is unique as multiple OpenGL contexts
//    can have the same OpenGL ids. 
//    A global array of all created shader UIDs is maintained so that 
//    searches on all shaders can be performed.
//  
class UniqueShaderID : public ReferenceCount
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //    The class object is added to the gloabl array of IDs.
  //
  UniqueShaderID(GLuint programID);

  //@
  //  Summary:
  //    To set the source of the shader. This will reset any 
  //    overrides/alternate shader sources.
  //  
  //  Parameters:
  //    newSource  - The source string of the shader.
  //
  inline void SetShaderSource(const string &newSource);

  //@
  //  Summary:
  //    To get the source of the shader.
  //  
  //  Returns:
  //    The source of the shader is returned.
  //
  inline const string & GetShaderSource() const;

  //@
  //  Summary:
  //    To set the override shader string used in this shader data.
  //  
  //  Parameters:
  //    newOverride  - The new override string.
  //
  inline void SetOverrideShaderSource(const string &newOverride);

  //@
  //  Summary:
  //    To get the source of the override shader.
  //  
  //  Returns:
  //    The source of the override shader is returned.
  //
  inline const string & GetOverrideShaderSource() const;

  //@
  //  Summary:
  //    To set the shader type of this shader ID. 
  //    (This can only be set once)
  //  
  //  Parameters:
  //    newType - The type of this shader.
  //
  void SetShaderType(USIDType newType);

  //@
  //  Summary:
  //    To get the shader type of this shader ID. 
  //  
  //  Returns:
  //    The shader type is returned.
  //
  inline USIDType GetShaderType() const;

  //@
  //  Summary:
  //    To get the unique index of the shader. 
  //    (0 is reserved for no-index)
  //  
  //  Returns:
  //    The unique index of this unique shader ID is returned.
  //
  inline uint GetUniqueIndex() const;

  //@
  //  Summary:
  //    To get the OpenGL ID of this shader. 
  //  
  //  Returns:
  //    The OpenGL ID is returned.
  //
  inline uint GetShaderOpenGLID() const;

  //@
  //  Summary:
  //    To set the last frame number that the shader was used.
  //  
  //  Parameters:
  //    frameNum - The new frame number for this shader.
  //
  inline void SetLastFrameUsed(uint frameNum);

  //@
  //  Summary:
  //    To get the last frame number that the shader was used.
  //  
  //  Returns:
  //    The last frame number the shader was used is returned.
  //
  inline uint GetLastFrameUsed() const;

  //@
  //  Summary:
  //    To set the substitute shader.
  //  
  //  Parameters:
  //    newSub  - The new substitute shader.
  //
  inline void SetSubstituteShader(SubShaderGLSLRef newSub);

  //@
  //  Summary:
  //    To get the substitute shader.
  //  
  //  Returns:
  //    The substitute shader is returned.
  //
  inline SubShaderGLSLRef GetSubstituteShader();

  //@
  //  Summary:
  //    To set if the shader is debugged or not.
  //  
  //  Parameters:
  //    flag  - The flag indicating if the shader is debugged.
  // 
  inline void SetShaderDebugged(bool flag);

  //@
  //  Summary:
  //    To get if the shader is flagged for debugging.
  //  
  //  Returns:
  //    The debug status of the shader is returned..
  //
  inline bool GetShaderDebugged() const;

  //@
  //  Summary:
  //    To find the unique shader based on the ID index.
  //  
  //  Returns:
  //    If a unique shader id exists for the index, it is returned.
  //    Else NULL is returned.
  //
  static UniqueShaderID * FindUniqueShaderID(uint uIndex);

  //@
  //  Summary:
  //    To get the list of all the unique shader IDs currently available.
  //  
  //  Returns:
  //    The shader list is returned.
  //
  static const vector<UniqueShaderID *> & GetUniqueShaderIDList();

  //@
  //  Summary:
  //    To clear all the shader debug flags.
  //  
  static void ClearAllDebugFlags();

protected:

  uint     uniqueIndex;                           // The unique index of this object

  uint     shaderGLID;                            // The OpenGL shader/program ID
  USIDType shaderType;                            // The type of OpenGL shader/program

  string   shaderSource;                          // The raw source of the shader
  string   overrideShaderSource;                  // The string of data that over-rides this shader
                                                  //  (can be empty/equal if no override is set) 
  uint     lastFrameUsed;                         // The last frame this shader was used
  bool     isShaderDebugged;                      // The flag indicating if the shader is debugged 

  SubShaderGLSLRef subShader;                     // The substitute shader (GLSL only)


  static   uint indexCounter;                     // Counter index for assigning unique index ids
  static   vector<UniqueShaderID *> loadedShaderIDArray; // Array of loaded unique shader IDs

  //@
  //  Summary:
  //    Destructor, removes itself from the global array of unique IDs.
  //  
  virtual ~UniqueShaderID();

};

//Auto reference count type def for shader ids
typedef AutoReferenceCount<UniqueShaderID> UniqueShaderIDRef;


///////////////////////////////////////////////////////////////////////////////
//
inline uint UniqueShaderID::GetUniqueIndex() const
{
  return uniqueIndex;
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint UniqueShaderID::GetShaderOpenGLID() const
{
  return shaderGLID;
}

///////////////////////////////////////////////////////////////////////////////
//
inline USIDType UniqueShaderID::GetShaderType() const
{
  return shaderType;
}


///////////////////////////////////////////////////////////////////////////////
//
inline void UniqueShaderID::SetShaderSource(const string &newSource)
{
  shaderSource = newSource;

  //Unset any override shaders
  overrideShaderSource = "";

  //Unset substitute shader
  subShader = SubShaderGLSLRef();
}

///////////////////////////////////////////////////////////////////////////////
//
inline const string & UniqueShaderID::GetShaderSource() const
{
  return shaderSource;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void UniqueShaderID::SetOverrideShaderSource(const string &newOverride)
{
  overrideShaderSource = newOverride;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const string & UniqueShaderID::GetOverrideShaderSource() const
{
  return overrideShaderSource;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void UniqueShaderID::SetLastFrameUsed(uint frameNum)
{
  lastFrameUsed = frameNum;
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint UniqueShaderID::GetLastFrameUsed() const
{
  return lastFrameUsed;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void UniqueShaderID::SetSubstituteShader(SubShaderGLSLRef newSub)
{
  subShader = newSub;
}

///////////////////////////////////////////////////////////////////////////////
//
inline SubShaderGLSLRef UniqueShaderID::GetSubstituteShader()
{
  return subShader;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void UniqueShaderID::SetShaderDebugged(bool flag)
{
  isShaderDebugged = flag;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool UniqueShaderID::GetShaderDebugged() const
{
  return isShaderDebugged;
}







#endif // __SUBSTITUTE_SHADER_GLSL_H_
