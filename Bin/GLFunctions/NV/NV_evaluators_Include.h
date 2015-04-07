#define GLI_INCLUDE_GL_NV_EVALUATORS

enum Main {

  GL_EVAL_2D_NV                            = 0x86C0,
  GL_EVAL_TRIANGULAR_2D_NV                 = 0x86C1,
  GL_MAP_TESSELLATION_NV                   = 0x86C2,
  GL_MAP_ATTRIB_U_ORDER_NV                 = 0x86C3,
  GL_MAP_ATTRIB_V_ORDER_NV                 = 0x86C4,
  GL_EVAL_FRACTIONAL_TESSELLATION_NV       = 0x86C5,
  GL_EVAL_VERTEX_ATTRIB0_NV                = 0x86C6,
  GL_EVAL_VERTEX_ATTRIB1_NV                = 0x86C7,
  GL_EVAL_VERTEX_ATTRIB2_NV                = 0x86C8,
  GL_EVAL_VERTEX_ATTRIB3_NV                = 0x86C9,
  GL_EVAL_VERTEX_ATTRIB4_NV                = 0x86CA,
  GL_EVAL_VERTEX_ATTRIB5_NV                = 0x86CB,
  GL_EVAL_VERTEX_ATTRIB6_NV                = 0x86CC,
  GL_EVAL_VERTEX_ATTRIB7_NV                = 0x86CD,
  GL_EVAL_VERTEX_ATTRIB8_NV                = 0x86CE,
  GL_EVAL_VERTEX_ATTRIB9_NV                = 0x86CF,
  GL_EVAL_VERTEX_ATTRIB10_NV               = 0x86D0,
  GL_EVAL_VERTEX_ATTRIB11_NV               = 0x86D1,
  GL_EVAL_VERTEX_ATTRIB12_NV               = 0x86D2,
  GL_EVAL_VERTEX_ATTRIB13_NV               = 0x86D3,
  GL_EVAL_VERTEX_ATTRIB14_NV               = 0x86D4,
  GL_EVAL_VERTEX_ATTRIB15_NV               = 0x86D5,
  GL_MAX_MAP_TESSELLATION_NV               = 0x86D6,
  GL_MAX_RATIONAL_EVAL_ORDER_NV            = 0x86D7,

};

void glMapControlPointsNV(GLenum[Main] target, GLuint index, GLenum[Main] type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid * points);
void glMapParameterivNV(GLenum[Main] target, GLenum[Main] pname, const GLint * params);
void glMapParameterfvNV(GLenum[Main] target, GLenum[Main] pname, const GLfloat * params);
void glGetMapControlPointsNV(GLenum[Main] target, GLuint index, GLenum[Main] type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid * points);
void glGetMapParameterivNV(GLenum[Main] target, GLenum[Main] pname, GLint * params);
void glGetMapParameterfvNV(GLenum[Main] target, GLenum[Main] pname, GLfloat * params);
void glGetMapAttribParameterivNV(GLenum[Main] target, GLuint index, GLenum[Main] pname, GLint * params);
void glGetMapAttribParameterfvNV(GLenum[Main] target, GLuint index, GLenum[Main] pname, GLfloat * params);
void glEvalMapsNV(GLenum[Main] target, GLenum[Main] mode);
