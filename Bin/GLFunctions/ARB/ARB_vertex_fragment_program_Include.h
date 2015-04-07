#define GLI_INCLUDE_ARB_VERTEX_FRAGMENT_PROGRAM






///////////////////////////////////////////////////////////////////////////////
// Main GLenum
///////////////////////////////////////////////////////////////////////////////


// Main GLenum for all standard GLenums
//  (may want to break up for speed)
enum Main {

  //This enum exists in OpenGL 1.4
  //GL_COLOR_SUM_ARB                                      =  0x8458,
  GL_VERTEX_PROGRAM_ARB                                 =  0x8620,
  //GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB                    =  0x8622,
  //GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB                       =  0x8623,
  //GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB                     =  0x8624,
  //GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB                       =  0x8625,
  //GL_CURRENT_VERTEX_ATTRIB_ARB                          =  0x8626,
  GL_PROGRAM_LENGTH_ARB                                 =  0x8627,
  GL_PROGRAM_STRING_ARB                                 =  0x8628,
  GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB                 =  0x862E,
  GL_MAX_PROGRAM_MATRICES_ARB                           =  0x862F,
  GL_CURRENT_MATRIX_STACK_DEPTH_ARB                     =  0x8640,
  GL_CURRENT_MATRIX_ARB                                 =  0x8641,
  //GL_VERTEX_PROGRAM_POINT_SIZE_ARB                      =  0x8642,
  //GL_VERTEX_PROGRAM_TWO_SIDE_ARB                        =  0x8643,
  //GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB                    =  0x8645,
  GL_PROGRAM_ERROR_POSITION_ARB                         =  0x864B,
  GL_PROGRAM_BINDING_ARB                                =  0x8677,
  //GL_MAX_VERTEX_ATTRIBS_ARB                             =  0x8869,
  //GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB                 =  0x886A,
  GL_PROGRAM_ERROR_STRING_ARB                           =  0x8874,
  GL_PROGRAM_FORMAT_ASCII_ARB                           =  0x8875,
  GL_PROGRAM_FORMAT_ARB                                 =  0x8876,
  GL_PROGRAM_INSTRUCTIONS_ARB                           =  0x88A0,
  GL_MAX_PROGRAM_INSTRUCTIONS_ARB                       =  0x88A1,
  GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB                    =  0x88A2,
  GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB                =  0x88A3,
  GL_PROGRAM_TEMPORARIES_ARB                            =  0x88A4,
  GL_MAX_PROGRAM_TEMPORARIES_ARB                        =  0x88A5,
  GL_PROGRAM_NATIVE_TEMPORARIES_ARB                     =  0x88A6,
  GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB                 =  0x88A7,
  GL_PROGRAM_PARAMETERS_ARB                             =  0x88A8,
  GL_MAX_PROGRAM_PARAMETERS_ARB                         =  0x88A9,
  GL_PROGRAM_NATIVE_PARAMETERS_ARB                      =  0x88AA,
  GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB                  =  0x88AB,
  GL_PROGRAM_ATTRIBS_ARB                                =  0x88AC,
  GL_MAX_PROGRAM_ATTRIBS_ARB                            =  0x88AD,
  GL_PROGRAM_NATIVE_ATTRIBS_ARB                         =  0x88AE,
  GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB                     =  0x88AF,
  GL_PROGRAM_ADDRESS_REGISTERS_ARB                      =  0x88B0,
  GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB                  =  0x88B1,
  GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB               =  0x88B2,
  GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB           =  0x88B3,
  GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB                   =  0x88B4,
  GL_MAX_PROGRAM_ENV_PARAMETERS_ARB                     =  0x88B5,
  GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB                    =  0x88B6,
  GL_TRANSPOSE_CURRENT_MATRIX_ARB                       =  0x88B7,
  GL_MATRIX0_ARB                                        =  0x88C0,
  GL_MATRIX1_ARB                                        =  0x88C1,
  GL_MATRIX2_ARB                                        =  0x88C2,
  GL_MATRIX3_ARB                                        =  0x88C3,
  GL_MATRIX4_ARB                                        =  0x88C4,
  GL_MATRIX5_ARB                                        =  0x88C5,
  GL_MATRIX6_ARB                                        =  0x88C6,
  GL_MATRIX7_ARB                                        =  0x88C7,
  GL_MATRIX8_ARB                                        =  0x88C8,
  GL_MATRIX9_ARB                                        =  0x88C9,
  GL_MATRIX10_ARB                                       =  0x88CA,
  GL_MATRIX11_ARB                                       =  0x88CB,
  GL_MATRIX12_ARB                                       =  0x88CC,
  GL_MATRIX13_ARB                                       =  0x88CD,
  GL_MATRIX14_ARB                                       =  0x88CE,
  GL_MATRIX15_ARB                                       =  0x88CF,
  GL_MATRIX16_ARB                                       =  0x88D0,
  GL_MATRIX17_ARB                                       =  0x88D1,
  GL_MATRIX18_ARB                                       =  0x88D2,
  GL_MATRIX19_ARB                                       =  0x88D3,
  GL_MATRIX20_ARB                                       =  0x88D4,
  GL_MATRIX21_ARB                                       =  0x88D5,
  GL_MATRIX22_ARB                                       =  0x88D6,
  GL_MATRIX23_ARB                                       =  0x88D7,
  GL_MATRIX24_ARB                                       =  0x88D8,
  GL_MATRIX25_ARB                                       =  0x88D9,
  GL_MATRIX26_ARB                                       =  0x88DA,
  GL_MATRIX27_ARB                                       =  0x88DB,
  GL_MATRIX28_ARB                                       =  0x88DC,
  GL_MATRIX29_ARB                                       =  0x88DD,
  GL_MATRIX30_ARB                                       =  0x88DE,
  GL_MATRIX31_ARB                                       =  0x88DF,


  //ARB fragment program defines
  GL_FRAGMENT_PROGRAM_ARB                               =  0x8804,
  GL_PROGRAM_ALU_INSTRUCTIONS_ARB                       =  0x8805,
  GL_PROGRAM_TEX_INSTRUCTIONS_ARB                       =  0x8806,
  GL_PROGRAM_TEX_INDIRECTIONS_ARB                       =  0x8807,
  GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB                =  0x8808,
  GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB                =  0x8809,
  GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB                =  0x880A,
  GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB                   =  0x880B,
  GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB                   =  0x880C,
  GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB                   =  0x880D,
  GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB            =  0x880E,
  GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB            =  0x880F,
  GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB            =  0x8810,
  //GL_MAX_TEXTURE_COORDS_ARB                             =  0x8871,
  //GL_MAX_TEXTURE_IMAGE_UNITS_ARB                        =  0x8872,


};

///////////////////////////////////////////////////////////////////////////////
// ARB vertex/fragment program function definitions
///////////////////////////////////////////////////////////////////////////////


void glVertexAttrib1sARB(GLuint index, GLshort x);

void glVertexAttrib1fARB(GLuint index, GLfloat x);

void glVertexAttrib1dARB(GLuint index, GLdouble x);

void glVertexAttrib2sARB(GLuint index, GLshort x, GLshort y);

void glVertexAttrib2fARB(GLuint index, GLfloat x, GLfloat y);

void glVertexAttrib2dARB(GLuint index, GLdouble x, GLdouble y);

void glVertexAttrib3sARB(GLuint index, GLshort x, GLshort y, GLshort z);

void glVertexAttrib3fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z);

void glVertexAttrib3dARB(GLuint index, GLdouble x, GLdouble y, GLdouble z);

void glVertexAttrib4sARB(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);

void glVertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glVertexAttrib4dARB(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glVertexAttrib4NubARB(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);

void glVertexAttrib1svARB(GLuint index, const GLshort v[1]);
void glVertexAttrib1fvARB(GLuint index, const GLfloat v[1]);
void glVertexAttrib1dvARB(GLuint index, const GLdouble v[1]);

void glVertexAttrib2svARB(GLuint index, const GLshort v[2]);
void glVertexAttrib2fvARB(GLuint index, const GLfloat v[2]);
void glVertexAttrib2dvARB(GLuint index, const GLdouble v[2]);

void glVertexAttrib3svARB(GLuint index, const GLshort v[3]);
void glVertexAttrib3fvARB(GLuint index, const GLfloat v[3]);
void glVertexAttrib3dvARB(GLuint index, const GLdouble v[3]);

void glVertexAttrib4bvARB(GLuint index, const GLbyte v[4]);
void glVertexAttrib4svARB(GLuint index, const GLshort v[4]);
void glVertexAttrib4ivARB(GLuint index, const GLint v[4]);
void glVertexAttrib4ubvARB(GLuint index, const GLubyte v[4]);
void glVertexAttrib4usvARB(GLuint index, const GLushort v[4]);
void glVertexAttrib4uivARB(GLuint index, const GLuint v[4]);
void glVertexAttrib4fvARB(GLuint index, const GLfloat v[4]);
void glVertexAttrib4dvARB(GLuint index, const GLdouble v[4]);
void glVertexAttrib4NbvARB(GLuint index, const GLbyte v[4]);
void glVertexAttrib4NsvARB(GLuint index, const GLshort v[4]);
void glVertexAttrib4NivARB(GLuint index, const GLint v[4]);
void glVertexAttrib4NubvARB(GLuint index, const GLubyte v[4]);
void glVertexAttrib4NusvARB(GLuint index, const GLushort v[4]);
void glVertexAttrib4NuivARB(GLuint index, const GLuint v[4]);

void glVertexAttribPointerARB(GLuint index, GLint size, GLenum[Main] type,GLboolean normalized, GLsizei stride,const void *pointer);

void glEnableVertexAttribArrayARB(GLuint index);
void glDisableVertexAttribArrayARB(GLuint index);

void glProgramStringARB(GLenum[Main] target, GLenum[Main] format, GLsizei len,const void *string);

void glBindProgramARB(GLenum[Main] target, GLshaderindex program);

void glDeleteProgramsARB(GLsizei n, const GLuint *programs);

void glGenProgramsARB(GLsizei n, GLuint *programs);

void glProgramEnvParameter4dARB(GLenum[Main] target, GLuint index,GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glProgramEnvParameter4dvARB(GLenum[Main] target, GLuint index,const GLdouble params[4]);
void glProgramEnvParameter4fARB(GLenum[Main] target, GLuint index,GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glProgramEnvParameter4fvARB(GLenum[Main] target, GLuint index,const GLfloat params[4]);
void glProgramLocalParameter4dARB(GLenum[Main] target, GLuint index,GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glProgramLocalParameter4dvARB(GLenum[Main] target, GLuint index,const GLdouble params[4]);
void glProgramLocalParameter4fARB(GLenum[Main] target, GLuint index,GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glProgramLocalParameter4fvARB(GLenum[Main] target, GLuint index,const GLfloat params[4]);

void glGetProgramEnvParameterdvARB(GLenum[Main] target, GLuint index,GLdouble *params);
void glGetProgramEnvParameterfvARB(GLenum[Main] target, GLuint index,GLfloat *params);
void glGetProgramLocalParameterdvARB(GLenum[Main] target, GLuint index,GLdouble *params);
void glGetProgramLocalParameterfvARB(GLenum[Main] target, GLuint index,GLfloat *params);

void glGetProgramivARB(GLenum[Main] target, GLenum[Main] pname, GLint *params);
void glGetProgramStringARB(GLenum[Main] target, GLenum[Main] pname, void *string);

void glGetVertexAttribdvARB(GLuint index, GLenum[Main] pname, GLdouble *params);
void glGetVertexAttribfvARB(GLuint index, GLenum[Main] pname, GLfloat *params);
void glGetVertexAttribivARB(GLuint index, GLenum[Main] pname, GLint *params);
void glGetVertexAttribPointervARB(GLuint index, GLenum[Main] pname, void **pointer);

GLboolean glIsProgramARB(GLuint program);


