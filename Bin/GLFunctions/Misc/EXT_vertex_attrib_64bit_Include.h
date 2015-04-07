#define GLI_INCLUDE_GL_EXT_VERTEX_ATTRIB_64BIT

enum Main {

  //GL_DOUBLE_VEC2_EXT         = 0x8FFC,
  //GL_DOUBLE_VEC3_EXT         = 0x8FFD,
  //GL_DOUBLE_VEC4_EXT         = 0x8FFE,
  //GL_DOUBLE_MAT2_EXT         = 0x8F46,
  //GL_DOUBLE_MAT3_EXT         = 0x8F47,
  //GL_DOUBLE_MAT4_EXT         = 0x8F48,
  //GL_DOUBLE_MAT2x3_EXT       = 0x8F49,
  //GL_DOUBLE_MAT2x4_EXT       = 0x8F4A,
  //GL_DOUBLE_MAT3x2_EXT       = 0x8F4B,
  //GL_DOUBLE_MAT3x4_EXT       = 0x8F4C,
  //GL_DOUBLE_MAT4x2_EXT       = 0x8F4D,
  //GL_DOUBLE_MAT4x3_EXT       = 0x8F4E,

};

void glVertexAttribL1dEXT(GLuint index, GLdouble x);
void glVertexAttribL2dEXT(GLuint index, GLdouble x, GLdouble y);
void glVertexAttribL3dEXT(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void glVertexAttribL4dEXT(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glVertexAttribL1dvEXT(GLuint index, const GLdouble * v);
void glVertexAttribL2dvEXT(GLuint index, const GLdouble * v);
void glVertexAttribL3dvEXT(GLuint index, const GLdouble * v);
void glVertexAttribL4dvEXT(GLuint index, const GLdouble * v);
void glVertexAttribLPointerEXT(GLuint index, GLint size, GLenum[Main] type, GLsizei stride, const GLvoid * pointer);
void glGetVertexAttribLdvEXT(GLuint index, GLenum[Main] pname, GLdouble * params);
void glVertexArrayVertexAttribLOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum[Main] type, GLsizei stride, GLintptr offset);
