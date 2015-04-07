#define GLI_INCLUDE_GLCORE2_0


///////////////////////////////////////////////////////////////////////////////
// Masks
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Error enum
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Main enum
///////////////////////////////////////////////////////////////////////////////

// Main enum for all standard enums
//  (may want to break up for speed)
enum Main {

  //GL_BLEND_EQUATION_RGB           =  GL_BLEND_EQUATION,
  GL_VERTEX_ATTRIB_ARRAY_ENABLED      =  0x8622,
  GL_VERTEX_ATTRIB_ARRAY_SIZE         =  0x8623,
  GL_VERTEX_ATTRIB_ARRAY_STRIDE       =  0x8624,
  GL_VERTEX_ATTRIB_ARRAY_TYPE         =  0x8625,
  GL_CURRENT_VERTEX_ATTRIB            =  0x8626,
  //GL_VERTEX_PROGRAM_POINT_SIZE        =  0x8642,
  GL_VERTEX_PROGRAM_TWO_SIDE          =  0x8643,
  GL_VERTEX_ATTRIB_ARRAY_POINTER      =  0x8645,
  GL_STENCIL_BACK_FUNC                =  0x8800,
  GL_STENCIL_BACK_FAIL                =  0x8801,
  GL_STENCIL_BACK_PASS_DEPTH_FAIL     =  0x8802,
  GL_STENCIL_BACK_PASS_DEPTH_PASS     =  0x8803,
  GL_MAX_DRAW_BUFFERS                 =  0x8824,
  GL_DRAW_BUFFER0                     =  0x8825,
  GL_DRAW_BUFFER1                     =  0x8826,
  GL_DRAW_BUFFER2                     =  0x8827,
  GL_DRAW_BUFFER3                     =  0x8828,
  GL_DRAW_BUFFER4                     =  0x8829,
  GL_DRAW_BUFFER5                     =  0x882A,
  GL_DRAW_BUFFER6                     =  0x882B,
  GL_DRAW_BUFFER7                     =  0x882C,
  GL_DRAW_BUFFER8                     =  0x882D,
  GL_DRAW_BUFFER9                     =  0x882E,
  GL_DRAW_BUFFER10                    =  0x882F,
  GL_DRAW_BUFFER11                    =  0x8830,
  GL_DRAW_BUFFER12                    =  0x8831,
  GL_DRAW_BUFFER13                    =  0x8832,
  GL_DRAW_BUFFER14                    =  0x8833,
  GL_DRAW_BUFFER15                    =  0x8834,
  GL_BLEND_EQUATION_ALPHA             =  0x883D,
  GL_POINT_SPRITE                     =  0x8861,
  GL_COORD_REPLACE                    =  0x8862,
  GL_MAX_VERTEX_ATTRIBS               =  0x8869,
  GL_VERTEX_ATTRIB_ARRAY_NORMALIZED   =  0x886A,
  GL_MAX_TEXTURE_COORDS               =  0x8871,
  GL_MAX_TEXTURE_IMAGE_UNITS          =  0x8872,
  GL_FRAGMENT_SHADER                  =  0x8B30,
  GL_VERTEX_SHADER                    =  0x8B31,
  GL_MAX_FRAGMENT_UNIFORM_COMPONENTS  =  0x8B49,
  GL_MAX_VERTEX_UNIFORM_COMPONENTS    =  0x8B4A,
  //GL_MAX_VARYING_FLOATS               =  0x8B4B,
  GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS   =  0x8B4C,
  GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS =  0x8B4D,
  GL_SHADER_TYPE                      =  0x8B4F,
  GL_FLOAT_VEC2                       =  0x8B50,
  GL_FLOAT_VEC3                       =  0x8B51,
  GL_FLOAT_VEC4                       =  0x8B52,
  GL_INT_VEC2                         =  0x8B53,
  GL_INT_VEC3                         =  0x8B54,
  GL_INT_VEC4                         =  0x8B55,
  GL_BOOL                             =  0x8B56,
  GL_BOOL_VEC2                        =  0x8B57,
  GL_BOOL_VEC3                        =  0x8B58,
  GL_BOOL_VEC4                        =  0x8B59,
  GL_FLOAT_MAT2                       =  0x8B5A,
  GL_FLOAT_MAT3                       =  0x8B5B,
  GL_FLOAT_MAT4                       =  0x8B5C,
  GL_SAMPLER_1D                       =  0x8B5D,
  GL_SAMPLER_2D                       =  0x8B5E,
  GL_SAMPLER_3D                       =  0x8B5F,
  GL_SAMPLER_CUBE                     =  0x8B60,
  GL_SAMPLER_1D_SHADOW                =  0x8B61,
  GL_SAMPLER_2D_SHADOW                =  0x8B62,
  GL_DELETE_STATUS                    =  0x8B80,
  GL_COMPILE_STATUS                   =  0x8B81,
  GL_LINK_STATUS                      =  0x8B82,
  GL_VALIDATE_STATUS                  =  0x8B83,
  GL_INFO_LOG_LENGTH                  =  0x8B84,
  GL_ATTACHED_SHADERS                 =  0x8B85,
  GL_ACTIVE_UNIFORMS                  =  0x8B86,
  GL_ACTIVE_UNIFORM_MAX_LENGTH        =  0x8B87,
  GL_SHADER_SOURCE_LENGTH             =  0x8B88,
  GL_ACTIVE_ATTRIBUTES                =  0x8B89,
  GL_ACTIVE_ATTRIBUTE_MAX_LENGTH      =  0x8B8A,
  GL_FRAGMENT_SHADER_DERIVATIVE_HINT  =  0x8B8B,
  GL_SHADING_LANGUAGE_VERSION         =  0x8B8C,
  GL_CURRENT_PROGRAM                  =  0x8B8D,
  GL_POINT_SPRITE_COORD_ORIGIN        =  0x8CA0,
  GL_LOWER_LEFT                       =  0x8CA1,
  GL_UPPER_LEFT                       =  0x8CA2,
  GL_STENCIL_BACK_REF                 =  0x8CA3,
  GL_STENCIL_BACK_VALUE_MASK          =  0x8CA4,
  GL_STENCIL_BACK_WRITEMASK           =  0x8CA5,

};

///////////////////////////////////////////////////////////////////////////////
// OpenGL 2.0 functions
///////////////////////////////////////////////////////////////////////////////

void glBlendEquationSeparate (GLenum[Main] modeRGB, GLenum[Main] modeAlpha);
void glDrawBuffers (GLsizei n, const GLenum *bufs);
void glStencilOpSeparate (GLenum[Main] face, GLenum[Main] sfail, GLenum[Main] dpfail, GLenum[Main] dppass);
void glStencilFuncSeparate (GLenum[Main] face, GLenum[Main] func, GLint ref, GLuint mask);
void glStencilMaskSeparate (GLenum[Main] face, GLuint mask );

void glAttachShader (GLGLSL_program program, GLGLSL_shader shader);
//void glBindAttribLocation (GLGLSL_program program, GLuint index, const GLchar *name);
void glBindAttribLocation (GLGLSL_program program, GLuint index, const GLasciistring *name);
void glCompileShader (GLGLSL_shader shader);
GLuint glCreateProgram (void);
GLuint glCreateShader (GLenum[Main] type);
void glDeleteProgram (GLGLSL_program program);
void glDeleteShader (GLGLSL_shader shader);
void glDetachShader (GLGLSL_program program, GLGLSL_shader shader);

void glGetActiveAttrib (GLGLSL_program program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void glGetActiveUniform (GLGLSL_program program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void glGetAttachedShaders (GLGLSL_program program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
//GLint glGetAttribLocation (GLGLSL_program program, const GLchar *name);
GLint glGetAttribLocation (GLGLSL_program program, const GLasciistring *name);
void glGetProgramiv (GLGLSL_program program, GLenum[Main] pname, GLint *params);
void glGetProgramInfoLog (GLGLSL_program program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

void glGetShaderiv (GLGLSL_shader shader, GLenum[Main] pname, GLint * params);
void glGetShaderInfoLog  (GLGLSL_shader shader,  GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void glGetShaderSource (GLGLSL_shader shader, GLsizei bufSize, GLsizei *length, GLchar *source);
//GLint glGetUniformLocation (GLGLSL_program program, const GLchar *name );
GLint glGetUniformLocation (GLGLSL_program program, const GLasciistring *name);

void glGetUniformfv (GLGLSL_program program, GLint location, GLfloat *params);
void glGetUniformiv (GLGLSL_program program, GLint location, GLint *params);
void glGetVertexAttribdv (GLuint index, GLenum[Main] pname, GLdouble *params);
void glGetVertexAttribfv (GLuint index, GLenum[Main] pname, GLfloat *params);
void glGetVertexAttribiv (GLuint index, GLenum[Main] pname, GLint *params);
void glGetVertexAttribPointerv (GLuint index, GLenum[Main] pname, GLvoid **pointer);

GLboolean glIsProgram (GLGLSL_program program);
GLboolean glIsShader (GLGLSL_shader shader);

void glLinkProgram (GLGLSL_program program);
void glShaderSource (GLGLSL_shader shader, GLsizei count, const GLchar** string, const GLint *length);
void glUseProgram (GLGLSL_program program);

void glUniform1f (GLint location, GLfloat value);
void glUniform2f (GLint location, GLfloat value, GLfloat value);
void glUniform3f (GLint location, GLfloat value, GLfloat value, GLfloat value);
void glUniform4f (GLint location, GLfloat value, GLfloat value, GLfloat value, GLfloat value);
void glUniform1i (GLint location, GLint value);
void glUniform2i (GLint location, GLint value, GLint value);
void glUniform3i (GLint location, GLint value, GLint value, GLint value);
void glUniform4i (GLint location, GLint value, GLint value, GLint value, GLint value);


//NOTE: The below is not technically accurate as the user can supply an array of values,
// but only the first is logged (assuming zero is not passed into the "count" field). 
// Change these back to standard pointers for logging if this becomes a problem
void glUniform1fv (GLint location, GLsizei count, const GLfloat value[1]);
void glUniform2fv (GLint location, GLsizei count, const GLfloat value[2]);
void glUniform3fv (GLint location, GLsizei count, const GLfloat value[3]);
void glUniform4fv (GLint location, GLsizei count, const GLfloat value[4]);
void glUniform1iv (GLint location, GLsizei count, const GLint value[1]);
void glUniform2iv (GLint location, GLsizei count, const GLint value[2]);
void glUniform3iv (GLint location, GLsizei count, const GLint value[3]);
void glUniform4iv (GLint location, GLsizei count, const GLint value[4]);

void glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat value[4]);
void glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat value[9]);
void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat value[16]);

void glValidateProgram (GLGLSL_program program);

void glVertexAttrib1d  (GLuint index, GLdouble value);
void glVertexAttrib1dv (GLuint index, const GLdouble value[1]);
void glVertexAttrib1f  (GLuint index, GLfloat value);
void glVertexAttrib1fv (GLuint index, const GLfloat value[1]);
void glVertexAttrib1s  (GLuint index, GLshort value);
void glVertexAttrib1sv (GLuint index, const GLshort value[1]);

void glVertexAttrib2d  (GLuint index, GLdouble value, GLdouble value);
void glVertexAttrib2dv (GLuint index, const GLdouble value[2]);
void glVertexAttrib2f  (GLuint index, GLfloat value, GLfloat value);
void glVertexAttrib2fv (GLuint index, const GLfloat value[2]);
void glVertexAttrib2s  (GLuint index, GLshort value, GLshort value);
void glVertexAttrib2sv (GLuint index, const GLshort value[2]);

void glVertexAttrib3d  (GLuint index, GLdouble value, GLdouble value, GLdouble value);
void glVertexAttrib3dv (GLuint index, const GLdouble value[3]);
void glVertexAttrib3f  (GLuint index, GLfloat value, GLfloat value, GLfloat value);
void glVertexAttrib3fv (GLuint index, const GLfloat value[3]);
void glVertexAttrib3s  (GLuint index, GLshort value, GLshort value, GLshort value);
void glVertexAttrib3sv (GLuint index, const GLshort value[3]);

void glVertexAttrib4Nbv (GLuint index, const GLbyte value[4]);
void glVertexAttrib4Niv (GLuint index, const GLint  value[4]);
void glVertexAttrib4Nsv (GLuint index, const GLshort value[4]);
void glVertexAttrib4Nub (GLuint index, GLubyte value, GLubyte value, GLubyte value, GLubyte value);
void glVertexAttrib4Nubv (GLuint index, const GLubyte value[4]);
void glVertexAttrib4Nuiv (GLuint index, const GLuint  value[4]);
void glVertexAttrib4Nusv (GLuint index, const GLushort value[4]);
void glVertexAttrib4bv (GLuint index, const GLbyte value[4]);
void glVertexAttrib4d  (GLuint index, GLdouble value, GLdouble value, GLdouble value, GLdouble value);
void glVertexAttrib4dv (GLuint index, const GLdouble value[4]);
void glVertexAttrib4f  (GLuint index, GLfloat value, GLfloat value, GLfloat value, GLfloat value);
void glVertexAttrib4fv (GLuint index, const GLfloat value[4]);
void glVertexAttrib4iv (GLuint index, const GLint value[4]);
void glVertexAttrib4s  (GLuint index, GLshort value, GLshort value, GLshort value, GLshort value);
void glVertexAttrib4sv (GLuint index, const GLshort value[4]);
void glVertexAttrib4ubv (GLuint index, const GLubyte value[4]);
void glVertexAttrib4uiv (GLuint index, const GLuint value[4]);
void glVertexAttrib4usv (GLuint index, const GLushort value[4]);

void glVertexAttribPointer (GLuint index, GLint size, GLenum[Main] type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

void glDisableVertexAttribArray (GLuint index);
void glEnableVertexAttribArray (GLuint index);




