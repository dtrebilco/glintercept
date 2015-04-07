#define GLI_INCLUDE_ARB_SHADER_OBJECTS

enum Main {

  GL_PROGRAM_OBJECT_ARB                 =  0x8B40,

  GL_OBJECT_TYPE_ARB                    =  0x8B4E,
  //GL_OBJECT_SUBTYPE_ARB                 =  0x8B4F,
  //GL_OBJECT_DELETE_STATUS_ARB           =  0x8B80,
  //GL_OBJECT_COMPILE_STATUS_ARB          =  0x8B81,
  //GL_OBJECT_LINK_STATUS_ARB             =  0x8B82,
  //GL_OBJECT_VALIDATE_STATUS_ARB         =  0x8B83,
  //GL_OBJECT_INFO_LOG_LENGTH_ARB         =  0x8B84,
  //GL_OBJECT_ATTACHED_OBJECTS_ARB        =  0x8B85,
  //GL_OBJECT_ACTIVE_UNIFORMS_ARB         =  0x8B86,
  //GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB = 0x8B87,
  //GL_OBJECT_SHADER_SOURCE_LENGTH_ARB    =  0x8B88,

  GL_SHADER_OBJECT_ARB                  =  0x8B48,

  //GL_FLOAT                              =  0x1406,
  //GL_FLOAT_VEC2_ARB                     =  0x8B50,
  //GL_FLOAT_VEC3_ARB                     =  0x8B51,
  //GL_FLOAT_VEC4_ARB                     =  0x8B52,

  //GL_INT                                =  0x1404,
  //GL_INT_VEC2_ARB                       =  0x8B53,
  //GL_INT_VEC3_ARB                       =  0x8B54,
  //GL_INT_VEC4_ARB                       =  0x8B55,

  //GL_BOOL_ARB                           =  0x8B56,
  //GL_BOOL_VEC2_ARB                      =  0x8B57,
  //GL_BOOL_VEC3_ARB                      =  0x8B58,
  //GL_BOOL_VEC4_ARB                      =  0x8B59,

  //GL_FLOAT_MAT2_ARB                     =  0x8B5A,
  //GL_FLOAT_MAT3_ARB                     =  0x8B5B,
  //GL_FLOAT_MAT4_ARB                     =  0x8B5C,

  //GL_SAMPLER_1D_ARB                     =  0x8B5D,
  //GL_SAMPLER_2D_ARB                     =  0x8B5E,
  //GL_SAMPLER_3D_ARB                     =  0x8B5F,
  //GL_SAMPLER_CUBE_ARB                   =  0x8B60,
  //GL_SAMPLER_1D_SHADOW_ARB              =  0x8B61,
  //GL_SAMPLER_2D_SHADOW_ARB              =  0x8B62,
  //GL_SAMPLER_2D_RECT_ARB                =  0x8B63,
  //GL_SAMPLER_2D_RECT_SHADOW_ARB         =  0x8B64,

  //GL_SHADING_LANGUAGE_VERSION_ARB       =  0x8B8C,

};



void glDeleteObjectARB(GLGLSL_ARB_handle obj);

GLhandle glGetHandleARB(GLenum[Main] pname);

void glDetachObjectARB(GLGLSL_ARB_handle containerObj, GLGLSL_ARB_handle attachedObj);

GLhandle glCreateShaderObjectARB(GLenum[Main] shaderType);

void glShaderSourceARB(GLGLSL_ARB_handle shaderObj, GLsizei count, const GLchar **string,const GLint *length);

void glCompileShaderARB(GLGLSL_ARB_handle shaderObj);

GLhandle glCreateProgramObjectARB(void);

void glAttachObjectARB(GLGLSL_ARB_handle containerObj, GLGLSL_ARB_handle obj);

void glLinkProgramARB(GLGLSL_ARB_handle programObj);

void glUseProgramObjectARB(GLGLSL_ARB_handle programObj);

void glValidateProgramARB(GLGLSL_ARB_handle programObj);

void glUniform1fARB(GLint location, GLfloat v0);
void glUniform2fARB(GLint location, GLfloat v0, GLfloat v1);
void glUniform3fARB(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void glUniform4fARB(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

void glUniform1iARB(GLint location, GLint v0);
void glUniform2iARB(GLint location, GLint v0, GLint v1);
void glUniform3iARB(GLint location, GLint v0, GLint v1, GLint v2);
void glUniform4iARB(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

//NOTE: The below is not technically accurate as the user can supply an array of values,
// but only the first is logged (assuming zero is not passed into the "count" field). 
// Change these back to standard pointers for logging if this becomes a problem
void glUniform1fvARB(GLint location, GLsizei count,const GLfloat value[1]);
void glUniform2fvARB(GLint location, GLsizei count,const GLfloat value[2]);
void glUniform3fvARB(GLint location, GLsizei count,const GLfloat value[3]);
void glUniform4fvARB(GLint location, GLsizei count,const GLfloat value[4]);

void glUniform1ivARB(GLint location, GLsizei count,const GLint value[1]);
void glUniform2ivARB(GLint location, GLsizei count,const GLint value[2]);
void glUniform3ivARB(GLint location, GLsizei count,const GLint value[3]);
void glUniform4ivARB(GLint location, GLsizei count,const GLint value[4]);

void glUniformMatrix2fvARB(GLint location, GLsizei count, GLboolean transpose,const GLfloat value[4]);
void glUniformMatrix3fvARB(GLint location, GLsizei count, GLboolean transpose,const GLfloat value[9]);
void glUniformMatrix4fvARB(GLint location, GLsizei count, GLboolean transpose,const GLfloat value[16]);

void glGetObjectParameterfvARB(GLhandle obj, GLenum[Main] pname, GLfloat *params);
void glGetObjectParameterivARB(GLhandle obj, GLenum[Main] pname, GLint *params);

void glGetInfoLogARB(GLGLSL_ARB_handle obj, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

void glGetAttachedObjectsARB(GLGLSL_ARB_handle containerObj, GLsizei maxCount, GLsizei *count, GLhandle *obj);

//GLint glGetUniformLocationARB(GLGLSL_ARB_handle programObj, const GLchar *name);
GLint glGetUniformLocationARB(GLGLSL_ARB_handle programObj, const GLasciistring *name);

void glGetActiveUniformARB(GLGLSL_ARB_handle programObj, GLuint GLindex, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLchar *name);

void glGetUniformfvARB(GLGLSL_ARB_handle programObj, GLint location, GLfloat *params);
void glGetUniformivARB(GLGLSL_ARB_handle programObj, GLint location, GLint *params);

void glGetShaderSourceARB(GLGLSL_ARB_handle obj, GLsizei maxLength, GLsizei *length, GLchar *source);

