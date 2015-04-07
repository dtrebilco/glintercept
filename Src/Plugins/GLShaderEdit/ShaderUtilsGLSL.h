/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SHADER_UTILS_GLSL_H_
#define __SHADER_UTILS_GLSL_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include <string>
#include <vector>

#include "UniformBugManager.h"

using namespace std;

class GLDriver;

//DT_TODO: Comments, class checks


//Structure to hold data about the uniforms in the program
struct UniformData
{
  UniformData();

  string name;            // The name of the uniform
  GLenum type;            // The type of the uniform
  uint   size;            // The size of the uniform (array size, not the size of the type)

  uint   numTypeElements; // The number of elements in the type 
  GLuint baseFormat;      // The base format of the type (GL_INT, GL_UNSIGNED_INT, GL_FLOAT)

#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

  bool   isATIUniformOverride; //Flag if this uniform is a ATI override uniform

#endif  //GLI_ATI_UNIFORM_GLSL_BUG

  vector<uint> indexData; // The origional indices of the uniform (can be multiple for an array)
  uint remapIndex;        // The new index of the uniform

  vector<GLint>   intUniformData;   // The float uniform data (if a float type)
  vector<GLuint>  uintUniformData;  // The uint uniform data  (if a uint type)
  vector<GLfloat> floatUniformData; // The int uniform data   (if a int type)

};
typedef vector<UniformData> UniformDataArray;


//@
//  Summary:
//    This class provides utility methods to support GLSL shader editing 
//    and debugging.
//  
class ShaderUtilsGLSL
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //
  //  Parameters:
  //    gliCallBacks  - The callbacks to GLIntercept.
  //
  ShaderUtilsGLSL(InterceptPluginCallbacks *gliCallBacks);

  //@
  //  Summary:
  //    Destructor
  //  
  virtual ~ShaderUtilsGLSL();

  //@
  //  Summary:
  //    To initialize the utils (function pointers etc.)
  //    The driver must be in a state where OpenGL calls can be made for this
  //    call to succeed.
  //    This call must occur successfully before any other calls are made.
  //
  //  Returns:
  //    True is returned if initialization was successful. Else false
  //    is returned.
  //
  bool Init();

  //@
  //  Summary:
  //    To retrieve the uniform data for the passed program ID.
  //    Note that this method gets data about the uniforms, not the uniform
  //    values. 
  //
  //  Parameters:
  //    programHandle  - The OpenGL ID of the program to get the uniform
  //                     data for. 
  //
  //    retData  - The array to return the uniform data in. 
  //
  //  Returns:
  //    Returns true on success, false on failure
  //
  bool GetUniformData(GLuint programHandle, UniformDataArray &retData) const;

  //@
  //  Summary:
  //    To load the uniform values into the passed uniform data structure.
  //
  //  Parameters:
  //    programHandle  - The OpenGL ID of the program to get the uniform
  //                     data values for. 
  //
  //    uniformData    - The uniform data to fill out the values for. 
  //
  void GetUniformValues(GLuint programHandle, UniformData &uniformData) const;

  //@
  //  Summary:
  //    To get data about a specific uniform data type.
  //
  //  Parameters:
  //    type  - The uniform data type to get info for. 
  //
  //    numElements  - The returned number of elements of the type. 
  //
  //    baseFormat  - The returned base type of the format (GL_FLOAT,GL_INT,GL_UNSIGNED_INT).
  //
  //  Returns:
  //    If the type is known, true is returned and the num elements and baseFormat
  //    is assigned valid data. Else false is returned and the data
  //    values are undefined.
  //
  static bool GetTypeData(GLenum type, uint &numElements, GLuint &baseFormat);

  //@
  //  Summary:
  //    To load the uniform data. The shader to load the uniform data in must
  //    be bound before calling these methods.
  //
  //  Parameters:
  //    loadPos  - The position to load the data. 
  //
  //    type  - The type of the data loaded. 
  //
  //    count  - The count of how many of the type exists. 
  //
  //    buffer  - The buffer containing the data to upload. 
  //
  void LoadUniformDataFloat(GLuint loadPos, GLenum type, GLuint count, const GLfloat *buffer) const; 
  void LoadUniformDataInt(GLuint loadPos, GLenum type, GLuint count, const GLint *buffer) const; 
  void LoadUniformDataUInt(GLuint loadPos, GLenum type, GLuint count, const GLuint *buffer) const;

  // Helper struct to format multiple shaders into one string
  struct ShaderStrData
  {
    ShaderStrData(GLuint a_shaderType, const string & a_shaderStr)
      : shaderType(a_shaderType)
      , shaderStr(a_shaderStr)
    {}

    GLuint shaderType; // The type of the shader
    string shaderStr;  // The shader string data
  };

  //@
  //  Summary:
  //    To append all the passed shader string data onto the passed shader string.
  //    Each shader is seperated by [] tokens - eg "[Vertex Shader]"
  //  
  //  Parameters:
  //    srcStrings - The source strings to pack.
  //
  //    retString - The return packed shader string.
  //
  static void AppendShaderStrings(const vector<ShaderStrData> & srcStrings, string & retString);

  //@
  //  Summary:
  //    To extract a sub-shader string from the passed string. Sub-shaders
  //    start with a '[' on a new line.
  //  
  //  Parameters:
  //    shaderSource - The source to extract the sub-shaders from.
  //
  //    retSources - The return shader sources extracted from the main string.
  //
  //  Returns:
  //    If shader source data could be retrieved true is returned. Else false is returned.
  //
  static bool ExtractShaderStrings(const string &shaderSource, vector<ShaderStrData> &retSources);

protected:
 
  InterceptPluginCallbacks *gliCallBacks;         // The callback interface into GLIntercept

  //Internal OpenGL function entry points
  void (GLAPIENTRY *iglGetProgramiv) (GLuint object,GLenum pname,GLint *params);
  void (GLAPIENTRY *iglGetActiveUniform) (GLuint program,GLuint GLindex, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
  GLint (GLAPIENTRY *iglGetUniformLocation) (GLuint programObj, const GLchar *name);
  void (GLAPIENTRY *iglGetUniformfv) (GLuint programObj, GLint location, GLfloat *params);
  void (GLAPIENTRY *iglGetUniformiv) (GLuint programObj, GLint location, GLint *params);
  void (GLAPIENTRY *iglGetUniformuiv) (GLuint programObj, GLint location, GLuint *params);

  void (GLAPIENTRY *iglUniform1fv) (GLint location, GLsizei count, const GLfloat *value);
  void (GLAPIENTRY *iglUniform2fv) (GLint location, GLsizei count, const GLfloat *value);
  void (GLAPIENTRY *iglUniform3fv) (GLint location, GLsizei count, const GLfloat *value);
  void (GLAPIENTRY *iglUniform4fv) (GLint location, GLsizei count, const GLfloat *value);
  void (GLAPIENTRY *iglUniformMatrix2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  void (GLAPIENTRY *iglUniformMatrix3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  void (GLAPIENTRY *iglUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  void (GLAPIENTRY *iglUniform1iv) (GLint location, GLsizei count, const GLint *value);
  void (GLAPIENTRY *iglUniform2iv) (GLint location, GLsizei count, const GLint *value);
  void (GLAPIENTRY *iglUniform3iv) (GLint location, GLsizei count, const GLint *value);
  void (GLAPIENTRY *iglUniform4iv) (GLint location, GLsizei count, const GLint *value);
  void (GLAPIENTRY *iglUniform1uiv) (GLint location, GLsizei count, const GLuint* value);
  void (GLAPIENTRY *iglUniform2uiv) (GLint location, GLsizei count, const GLuint* value);
  void (GLAPIENTRY *iglUniform3uiv) (GLint location, GLsizei count, const GLuint* value);
  void (GLAPIENTRY *iglUniform4uiv) (GLint location, GLsizei count, const GLuint* value);

  void (GLAPIENTRY *iglUniformMatrix2x3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  void (GLAPIENTRY *iglUniformMatrix2x4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  void (GLAPIENTRY *iglUniformMatrix3x2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  void (GLAPIENTRY *iglUniformMatrix3x4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  void (GLAPIENTRY *iglUniformMatrix4x2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  void (GLAPIENTRY *iglUniformMatrix4x3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

  // Find a shader token in the passed string starting at the specified index
  static bool FindShaderToken(const string &shaderSource, uint startIndex, string & retShaderToken, GLuint & retGLType);

  // Get the index of the shader type (-1 if unknown type)
  static int GetShaderTypeIndex(GLuint typeID);
};


#endif // __SHADER_UTILS_GLSL_H_
