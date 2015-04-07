/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SHADER_EDIT_GLSL_H_
#define __SHADER_EDIT_GLSL_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include "ReferenceCount.h"
#include "ShaderManagerGLSL.h"

#include <string>
#include <vector>

using namespace std;


class GLDriver;
class SEContext;

//@
//  Summary:
//    This class intercepts OpenGL GLSL shader based calls and and manages them
//    for editing.
//  
class ShaderEditGLSL : public ReferenceCount
{
public:

  //@
  //  Summary:
  //    To create an initialize a GLSL shader editor interface.
  //  
  //  Parameters:
  //    callBacks  - The callbacks to GLIntercept.
  //
  //  Returns:
  //    If a interface could be created and init, it is returned. 
  //    Else NULL is returned.
  //
  static ShaderEditGLSL * CreateInstance(InterceptPluginCallbacks *callBacks);

  //@
  //  Summary:
  //    To log the passed function and function data 
  //    (Before the actual function is called)
  //  
  //  Parameters:
  //    userIndex - The user index value associated with this function. 
  //                This is set via SetFunctionID. Zero indicates no 
  //                 value has been set.
  //
  //    funcName - The name of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  //    currContext - The current context when making the log call
  //
  virtual void LogFunctionPre(uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args, SEContext * currContext);

  //@
  //  Summary:
  //    To perform any post-call logging of a function.
  //    (After the actual function is called)
  //  
  //  Parameters:
  //    userIndex - The user index value associated with this function. 
  //                This is set via SetFunctionID. Zero indicates no 
  //                 value has been set.
  //
  //    funcName - The name of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return vlaue of the function (if any).
  //
  //    currContext - The current context when making the log call
  //
  virtual void LogFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal, SEContext * currContext);
  
  //@
  //  Summary:
  //    To get the id's of the bound GLSL program
  //  
  //  Parameters:
  //    programID  - The returned program ID of the bound program.
  //                 (returns 0 for no bound program)
  //
  void GetBoundProgram(GLuint &programID);

  //@
  //  Summary:
  //    To determine if the passed unique shader ID exists in this 
  //    shader editor/manager.
  //  
  //  Parameters:
  //    findShader - The shader ID to search for.
  //
  //  Returns:
  //    If the shaderID is found in this editor, true is returned. Else 
  //    false is returned.
  //
  bool FindShader(const UniqueShaderID *findShader);

  //@
  //  Summary:
  //    To create a substitute shader using the passed source to replace
  //    the passed program ID.
  //  
  //  Parameters:
  //    programID - The ID of the program to replace.
  //
  //    shaderSource - The GLSL shader source to compile the new program with.
  //
  //    retLog     - The log of the compile/link of the new program.
  //
  //  Returns:
  //    If substitution is successful, true is returned. Else false is returned.
  //
  bool AddSubstituteShader(GLuint programID, const string &shaderSource, string &retLog);

  //@
  //  Summary:
  //    To compile a new shader program using the passed source.
  //  
  //  Parameters:
  //    programID    - The ID of an existing program to take attribute binding 
  //                   locations from.
  //
  //    shaderSource - The GLSL shader source to compile the new program with.
  //
  //    retProgram   - The return program ID on success.
  //
  //    retLog       - The log of the compile/link of the new program.
  //
  //  Returns:
  //    If compilation and linking is successful, true is returned. Else false
  //    is returned.
  //
  bool CompileProgram(GLuint programID, const string &shaderSource, GLuint &retProgram, string &retLog);

protected:

  ShaderManagerGLSL  shaderProgramManager;        // The manager of shaders
  ShaderManagerGLSL  shaderObjectManager;         // The manager of shaders

  InterceptPluginCallbacks *gliCallBacks;         // The callback interface into GLIntercept
  const GLCoreDriver       *GLV;                  // The core OpenGL driver

  //Structure to hold vertex attribute data
  struct AttributeData
  {
    //@
    //  Summary:
    //    Constructor
    //  
    AttributeData();

    GLint  size;                                  // The size of the vertex attribute
    GLint  type;                                  // The type of the vertex attribute
    string name;                                  // The name of the vertex attribute
    bool   isGLPrefix;                            // Flag indicating if the name has the "gl_" prefix
  };


  //@
  //  Summary:
  //    Constructor 
  //
  //  Parameters:
  //    gliCallBacks  - The callbacks to GLIntercept.
  //
	ShaderEditGLSL(InterceptPluginCallbacks *gliCallBacks);


  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~ShaderEditGLSL();

  //@
  //  Summary:
  //    To initialize constants and arrays etc.
  //  
  //  Returns:
  //    If initialization was successful, true is returned. Else false is returned.
  //
  bool Init();

  //@
  //  Summary:
  //    To determine if the driver is available and perform initialization if
  //    necessary.
  //  
  //  Returns:
  //    If the driver is available, true is returned. Else false is returned.
  //
  bool DriverAvailable() const;

  //@
  //  Summary:
  //    To create and compile a new shader of the passed type using the
  //    passed source.
  //  
  //  Parameters:
  //    shaderType - The OpenGL shader type.
  //
  //    shaderSource - The source to of the shader to compile.
  //
  //    retLog - The log of the compile. Data is appended to the passed string.
  //
  //  Returns:
  //    If a shader could be created and compiled it is returned. Else 0 is returned.
  //
  GLuint CompileShader(GLenum shaderType, const string &shaderSource, string &retLog) const;

  //@
  //  Summary:
  //    To copy all the attribute locations from one program to another.
  //    This is to ensure both programs share the same vertex attribute
  //    locations.
  //  
  //  Parameters:
  //    oldProgram - The program to get the attribute locations to copy from.
  //
  //    newProgram - The program to accept the old program locations.
  //
  void AttribLocationCopy(GLuint oldProgram, GLuint newProgram) const;  

  //@
  //  Summary:
  //    To get the vertex attribute differences .
  //  
  //  Parameters:
  //    oldProgram - The origional program to compare against.
  //
  //    newProgram - The new  program to compare against.
  //
  //    attribDiff - String to record the attribute differeneces.
  //
  void GetAttribDifferences(GLuint oldProgram, GLuint newProgram, string &attribDiff) const;

  //@
  //  Summary:
  //    To get the attribute data for the passed program ID.
  //  
  //  Parameters:
  //    programID - The program to get the attribute data for.
  //
  //    vertexAttribData - The returned attribute data.
  //
  void GetAttributeData(GLuint programID, vector<AttributeData> &vertexAttribData) const;

  //@
  //  Summary:
  //    To get the information log for the passed handle (if any).
  //  
  //  Parameters:
  //    shaderObject  - The shader data to get the log data for.
  //
  //    retString   - The returned data, can be empty for no data. 
  //
  //    isProgram   - Flag indicating if we are retrieving program
  //                  log info.
  //
  void GetLogData(GLuint shaderObject, string & retString, bool isProgram) const;

  //@
  //  Summary:
  //    To get the attribute enum in string form.
  //  
  //  Parameters:
  //    attribEnum  - The attribute to get the string for.
  //
  string GetAttributeType(GLenum attribEnum) const;


private:

  GLenum   iObjectType;                           // Internal variable to cache data on shader creation
  GLuint   iHandle;                               // Internal data to cache a handle type across a pre/post call   
  GLuint   iHandleDetach;                         // Internal data to cache a handle type across a pre/post call   

  //Internal entry points to the OpenGL driver
  void (GLAPIENTRY *iglGetProgramiv) (GLuint object,GLenum pname,GLint *params);
  void (GLAPIENTRY *iglGetShaderiv) (GLuint object,GLenum pname,GLint *params);

  void (GLAPIENTRY *iglValidateProgram) (GLuint object); 

  void (GLAPIENTRY *iglGetProgramInfoLog) (GLuint object, GLsizei maxLength, GLsizei *length,GLchar *infoLog); 
  void (GLAPIENTRY *iglGetShaderInfoLog) (GLuint object, GLsizei maxLength, GLsizei *length,GLchar *infoLog); 

  void (GLAPIENTRY *iglGetActiveUniform) (GLuint program,GLuint GLindex, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
  GLint (GLAPIENTRY *iglGetUniformLocation) (GLuint programObj, const GLchar *name);

  GLuint (GLAPIENTRY *iglCreateProgram) (void);
  GLuint (GLAPIENTRY *iglCreateShader) (GLenum shaderType);

  void (GLAPIENTRY *iglDeleteProgram) (GLuint obj);
  void (GLAPIENTRY *iglDeleteShader) (GLuint obj);

  void (GLAPIENTRY *iglShaderSource) (GLuint shaderObj, GLsizei count, const GLchar **string,const GLint *length);
  void (GLAPIENTRY *iglCompileShader) (GLuint shaderObj);
  void (GLAPIENTRY *iglLinkProgram) (GLuint programObj);
  void (GLAPIENTRY *iglAttachShader) (GLuint containerObj, GLuint obj);
  void (GLAPIENTRY *iglDetachShader) (GLuint containerObj, GLuint attachedObj);

  void  (GLAPIENTRY *iglBindAttribLocation) (GLuint programObj, GLuint index, const GLchar *name);
  void  (GLAPIENTRY *iglGetActiveAttrib) (GLuint programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
  GLint (GLAPIENTRY *iglGetAttribLocation) (GLuint programObj, const GLchar *name);

  //Creates a object shader/program
  void CreateObjectPre(uint updateID, FunctionArgs & args);
  void CreateObjectPost(const FunctionRetValue & retVal);

  //Delete object
  void DeleteObjectPre(FunctionArgs & args);
  void DeleteObjectPost(uint updateID);

  //Supply shader source
  void ShaderSourcePre(FunctionArgs & args);

  //Attach/Detach object
  void AttachObjectPre(FunctionArgs & args);
  void DetachObjectPre(FunctionArgs & args);
  void DetachObjectPost();

  //Link/Use
  void LinkProgramPre(FunctionArgs & args);
  void LinkProgramPost();
  void UseProgramPre(FunctionArgs & args, SEContext * currContext);

#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

  //Uniform bug override
  void Uniform1iPre(FunctionArgs & args, SEContext * currContext, uint updateID);

#endif  //GLI_ATI_UNIFORM_GLSL_BUG

};



#endif // __SHADER_EDIT_GLSL_H_
