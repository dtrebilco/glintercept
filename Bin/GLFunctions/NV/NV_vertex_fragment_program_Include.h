#define GLI_INCLUDE_NV_VERTEX_FRAGMENT_PROGRAM






///////////////////////////////////////////////////////////////////////////////
// Main GLenum
///////////////////////////////////////////////////////////////////////////////

// Main GLenum for all standard GLenums
//  (may want to break up for speed)
enum Main {
  
  //There are many defines that are mapped in the ARB version

  //GL_VERTEX_PROGRAM_NV                                  =  0x8620,
  GL_VERTEX_STATE_PROGRAM_NV                            =  0x8621,
  //GL_ATTRIB_ARRAY_SIZE_NV                               =  0x8623,
  //GL_ATTRIB_ARRAY_STRIDE_NV                             =  0x8624,
  //GL_ATTRIB_ARRAY_TYPE_NV                               =  0x8625,
  //GL_CURRENT_ATTRIB_NV                                  =  0x8626,
  //GL_PROGRAM_LENGTH_NV                                  =  0x8627,
  //GL_PROGRAM_STRING_NV                                  =  0x8628,
  GL_MODELVIEW_PROJECTION_NV                            =  0x8629,
  GL_IDENTITY_NV                                        =  0x862A,
  GL_INVERSE_NV                                         =  0x862B,
  GL_TRANSPOSE_NV                                       =  0x862C,
  GL_INVERSE_TRANSPOSE_NV                               =  0x862D,
  //GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV                    =  0x862E,
  //GL_MAX_TRACK_MATRICES_NV                              =  0x862F,
  GL_MATRIX0_NV                                         =  0x8630,
  GL_MATRIX1_NV                                         =  0x8631,
  GL_MATRIX2_NV                                         =  0x8632,
  GL_MATRIX3_NV                                         =  0x8633,
  GL_MATRIX4_NV                                         =  0x8634,
  GL_MATRIX5_NV                                         =  0x8635,
  GL_MATRIX6_NV                                         =  0x8636,
  GL_MATRIX7_NV                                         =  0x8637,
  //GL_CURRENT_MATRIX_STACK_DEPTH_NV                      =  0x8640,
  //GL_CURRENT_MATRIX_NV                                  =  0x8641,
  //GL_VERTEX_PROGRAM_POINT_SIZE_NV                       =  0x8642,
  //GL_VERTEX_PROGRAM_TWO_SIDE_NV                         =  0x8643,
  GL_PROGRAM_PARAMETER_NV                               =  0x8644,
  //GL_ATTRIB_ARRAY_POINTER_NV                            =  0x8645,
  GL_PROGRAM_TARGET_NV                                  =  0x8646,
  GL_PROGRAM_RESIDENT_NV                                =  0x8647,
  GL_TRACK_MATRIX_NV                                    =  0x8648,
  GL_TRACK_MATRIX_TRANSFORM_NV                          =  0x8649,
  GL_VERTEX_PROGRAM_BINDING_NV                          =  0x864A,
  //GL_PROGRAM_ERROR_POSITION_NV                          =  0x864B,
  GL_VERTEX_ATTRIB_ARRAY0_NV                            =  0x8650,
  GL_VERTEX_ATTRIB_ARRAY1_NV                            =  0x8651,
  GL_VERTEX_ATTRIB_ARRAY2_NV                            =  0x8652,
  GL_VERTEX_ATTRIB_ARRAY3_NV                            =  0x8653,
  GL_VERTEX_ATTRIB_ARRAY4_NV                            =  0x8654,
  GL_VERTEX_ATTRIB_ARRAY5_NV                            =  0x8655,
  GL_VERTEX_ATTRIB_ARRAY6_NV                            =  0x8656,
  GL_VERTEX_ATTRIB_ARRAY7_NV                            =  0x8657,
  GL_VERTEX_ATTRIB_ARRAY8_NV                            =  0x8658,
  GL_VERTEX_ATTRIB_ARRAY9_NV                            =  0x8659,
  GL_VERTEX_ATTRIB_ARRAY10_NV                           =  0x865A,
  GL_VERTEX_ATTRIB_ARRAY11_NV                           =  0x865B,
  GL_VERTEX_ATTRIB_ARRAY12_NV                           =  0x865C,
  GL_VERTEX_ATTRIB_ARRAY13_NV                           =  0x865D,
  GL_VERTEX_ATTRIB_ARRAY14_NV                           =  0x865E,
  GL_VERTEX_ATTRIB_ARRAY15_NV                           =  0x865F,
  GL_MAP1_VERTEX_ATTRIB0_4_NV                           =  0x8660,
  GL_MAP1_VERTEX_ATTRIB1_4_NV                           =  0x8661,
  GL_MAP1_VERTEX_ATTRIB2_4_NV                           =  0x8662,
  GL_MAP1_VERTEX_ATTRIB3_4_NV                           =  0x8663,
  GL_MAP1_VERTEX_ATTRIB4_4_NV                           =  0x8664,
  GL_MAP1_VERTEX_ATTRIB5_4_NV                           =  0x8665,
  GL_MAP1_VERTEX_ATTRIB6_4_NV                           =  0x8666,
  GL_MAP1_VERTEX_ATTRIB7_4_NV                           =  0x8667,
  GL_MAP1_VERTEX_ATTRIB8_4_NV                           =  0x8668,
  GL_MAP1_VERTEX_ATTRIB9_4_NV                           =  0x8669,
  GL_MAP1_VERTEX_ATTRIB10_4_NV                          =  0x866A,
  GL_MAP1_VERTEX_ATTRIB11_4_NV                          =  0x866B,
  GL_MAP1_VERTEX_ATTRIB12_4_NV                          =  0x866C,
  GL_MAP1_VERTEX_ATTRIB13_4_NV                          =  0x866D,
  GL_MAP1_VERTEX_ATTRIB14_4_NV                          =  0x866E,
  GL_MAP1_VERTEX_ATTRIB15_4_NV                          =  0x866F,
  GL_MAP2_VERTEX_ATTRIB0_4_NV                           =  0x8670,
  GL_MAP2_VERTEX_ATTRIB1_4_NV                           =  0x8671,
  GL_MAP2_VERTEX_ATTRIB2_4_NV                           =  0x8672,
  GL_MAP2_VERTEX_ATTRIB3_4_NV                           =  0x8673,
  GL_MAP2_VERTEX_ATTRIB4_4_NV                           =  0x8674,
  GL_MAP2_VERTEX_ATTRIB5_4_NV                           =  0x8675,
  GL_MAP2_VERTEX_ATTRIB6_4_NV                           =  0x8676,
  
  //Issue: This actually maps to GL_PROGRAM_BINDING_ARB
  //GL_MAP2_VERTEX_ATTRIB7_4_NV                           =  0x8677,
  GL_MAP2_VERTEX_ATTRIB8_4_NV                           =  0x8678,
  GL_MAP2_VERTEX_ATTRIB9_4_NV                           =  0x8679,
  GL_MAP2_VERTEX_ATTRIB10_4_NV                          =  0x867A,
  GL_MAP2_VERTEX_ATTRIB11_4_NV                          =  0x867B,
  GL_MAP2_VERTEX_ATTRIB12_4_NV                          =  0x867C,
  GL_MAP2_VERTEX_ATTRIB13_4_NV                          =  0x867D,
  GL_MAP2_VERTEX_ATTRIB14_4_NV                          =  0x867E,
  GL_MAP2_VERTEX_ATTRIB15_4_NV                          =  0x867F,


  //NV_fragment program 
  GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV           =  0x8868,
  GL_FRAGMENT_PROGRAM_NV                                =  0x8870,
  //GL_MAX_TEXTURE_COORDS_NV                              =  0x8871,
  //GL_MAX_TEXTURE_IMAGE_UNITS_NV                         =  0x8872,
  GL_FRAGMENT_PROGRAM_BINDING_NV                        =  0x8873,
  //GL_PROGRAM_ERROR_STRING_NV                            =  0x8874,


};

///////////////////////////////////////////////////////////////////////////////
// NV vertex/fragment program function definitions
///////////////////////////////////////////////////////////////////////////////



void glBindProgramNV(GLenum[Main] target, GLshaderindex id);
void glDeleteProgramsNV(GLsizei n, const GLuint *ids);
void glExecuteProgramNV(GLenum[Main] target, GLshaderindex id, const GLfloat *params);
void glGenProgramsNV(GLsizei n, GLuint *ids);
GLboolean glAreProgramsResidentNV(GLsizei n, const GLuint *ids,GLboolean *residences);

void glRequestResidentProgramsNV(GLsizei n, const GLuint *ids);

void glGetProgramParameterfvNV(GLenum[Main] target, GLuint index, GLenum[Main] pname, GLfloat *params);
void glGetProgramParameterdvNV(GLenum[Main] target, GLuint index, GLenum[Main] pname, GLdouble *params);

void glGetProgramivNV(GLshaderindex id, GLenum[Main] pname, GLint *params);
void glGetProgramStringNV(GLshaderindex id, GLenum[Main] pname, GLubyte *program);
void glGetTrackMatrixivNV(GLenum[Main] target, GLuint address,GLenum[Main] pname, GLint *params);
void glGetVertexAttribdvNV(GLuint index, GLenum[Main] pname, GLdouble *params);
void glGetVertexAttribfvNV(GLuint index, GLenum[Main] pname, GLfloat *params);
void glGetVertexAttribivNV(GLuint index, GLenum[Main] pname, GLint *params);
void glGetVertexAttribPointervNV(GLuint index, GLenum[Main] pname, void **pointer);
GLboolean glIsProgramNV(GLuint id);

void glLoadProgramNV(GLenum[Main] target, GLuint id, GLsizei len,const GLubyte *program);
void glProgramParameter4fNV(GLenum[Main] target, GLuint index,GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glProgramParameter4dNV(GLenum[Main] target, GLuint index,GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glProgramParameter4dvNV(GLenum[Main] target, GLuint index,const GLdouble params[4]);
void glProgramParameter4fvNV(GLenum[Main] target, GLuint index,const GLfloat params[4]);
void glProgramParameters4dvNV(GLenum[Main] target, GLuint index,GLuint num, const GLdouble *params);
void glProgramParameters4fvNV(GLenum[Main] target, GLuint index,GLuint num, const GLfloat *params);

void glTrackMatrixNV(GLenum[Main] target, GLuint address,GLenum[Main] matrix, GLenum[Main] transform);

void glVertexAttribPointerNV(GLuint index, GLint size, GLenum[Main] type, GLsizei stride,const void *pointer);

void glVertexAttrib1sNV(GLuint index, GLshort x);
void glVertexAttrib1fNV(GLuint index, GLfloat x);
void glVertexAttrib1dNV(GLuint index, GLdouble x);
void glVertexAttrib2sNV(GLuint index, GLshort x, GLshort y);
void glVertexAttrib2fNV(GLuint index, GLfloat x, GLfloat y);
void glVertexAttrib2dNV(GLuint index, GLdouble x, GLdouble y);
void glVertexAttrib3sNV(GLuint index, GLshort x, GLshort y, GLshort z);
void glVertexAttrib3fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z);
void glVertexAttrib3dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void glVertexAttrib4sNV(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
void glVertexAttrib4fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glVertexAttrib4dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glVertexAttrib4ubNV(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);

void glVertexAttrib1svNV(GLuint index, const GLshort v[1]);
void glVertexAttrib1fvNV(GLuint index, const GLfloat v[1]);
void glVertexAttrib1dvNV(GLuint index, const GLdouble v[1]);

void glVertexAttrib2svNV(GLuint index, const GLshort v[2]);
void glVertexAttrib2fvNV(GLuint index, const GLfloat v[2]);
void glVertexAttrib2dvNV(GLuint index, const GLdouble v[2]);

void glVertexAttrib3svNV(GLuint index, const GLshort v[3]);
void glVertexAttrib3fvNV(GLuint index, const GLfloat v[3]);
void glVertexAttrib3dvNV(GLuint index, const GLdouble v[3]);

void glVertexAttrib4svNV(GLuint index, const GLshort v[4]);
void glVertexAttrib4fvNV(GLuint index, const GLfloat v[4]);
void glVertexAttrib4dvNV(GLuint index, const GLdouble v[4]);
void glVertexAttrib4ubvNV(GLuint index, const GLubyte v[4]);

void glVertexAttribs1svNV(GLuint index, GLsizei n, const GLshort v[1]);
void glVertexAttribs1fvNV(GLuint index, GLsizei n, const GLfloat v[1]);
void glVertexAttribs1dvNV(GLuint index, GLsizei n, const GLdouble v[1]);

void glVertexAttribs2svNV(GLuint index, GLsizei n, const GLshort v[2]);
void glVertexAttribs2fvNV(GLuint index, GLsizei n, const GLfloat v[2]);
void glVertexAttribs2dvNV(GLuint index, GLsizei n, const GLdouble v[2]);

void glVertexAttribs3svNV(GLuint index, GLsizei n, const GLshort v[3]);
void glVertexAttribs3fvNV(GLuint index, GLsizei n, const GLfloat v[3]);
void glVertexAttribs3dvNV(GLuint index, GLsizei n, const GLdouble v[3]);

void glVertexAttribs4svNV(GLuint index, GLsizei n, const GLshort v[4]);
void glVertexAttribs4fvNV(GLuint index, GLsizei n, const GLfloat v[4]);
void glVertexAttribs4dvNV(GLuint index, GLsizei n, const GLdouble v[4]);
void glVertexAttribs4ubvNV(GLuint index, GLsizei n, const GLubyte v[4]);

//NV_fragment program
void glProgramNamedParameter4fNV(GLuint id, GLsizei len, const GLubyte *name,GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glProgramNamedParameter4dNV(GLuint id, GLsizei len, const GLubyte *name,GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glProgramNamedParameter4fvNV(GLuint id, GLsizei len, const GLubyte *name,const GLfloat v[4]);
void glProgramNamedParameter4dvNV(GLuint id, GLsizei len, const GLubyte *name,const GLdouble v[4]);
void glGetProgramNamedParameterfvNV(GLuint id, GLsizei len, const GLubyte *name,GLfloat *params);
void glGetProgramNamedParameterdvNV(GLuint id, GLsizei len, const GLubyte *name,GLdouble *params);

