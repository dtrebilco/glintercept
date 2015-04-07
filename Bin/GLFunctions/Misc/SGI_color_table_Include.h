#define GLI_INCLUDE_GL_SGI_COLOR_TABLE

enum Main {

  //GL_COLOR_TABLE_SGI                               = 0x80D0,
  //GL_POST_CONVOLUTION_COLOR_TABLE_SGI              = 0x80D1,
  //GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI             = 0x80D2,
  //GL_PROXY_COLOR_TABLE_SGI                         = 0x80D3,
  //GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI        = 0x80D4,
  //GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI       = 0x80D5,
  //GL_COLOR_TABLE_SCALE_SGI                         = 0x80D6,
  //GL_COLOR_TABLE_BIAS_SGI                          = 0x80D7,
  //GL_COLOR_TABLE_FORMAT_SGI                        = 0x80D8,
  //GL_COLOR_TABLE_WIDTH_SGI                         = 0x80D9,
  //GL_COLOR_TABLE_RED_SIZE_SGI                      = 0x80DA,
  //GL_COLOR_TABLE_GREEN_SIZE_SGI                    = 0x80DB,
  //GL_COLOR_TABLE_BLUE_SIZE_SGI                     = 0x80DC,
  //GL_COLOR_TABLE_ALPHA_SIZE_SGI                    = 0x80DD,
  //GL_COLOR_TABLE_LUMINANCE_SIZE_SGI                = 0x80DE,
  //GL_COLOR_TABLE_INTENSITY_SIZE_SGI                = 0x80DF,

};

void glColorTableSGI(GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLenum[Main] format, GLenum[Main] type, const GLvoid * table);
void glColorTableParameterfvSGI(GLenum[Main] target, GLenum[Main] pname, const GLfloat * params);
void glColorTableParameterivSGI(GLenum[Main] target, GLenum[Main] pname, const GLint * params);
void glCopyColorTableSGI(GLenum[Main] target, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width);
void glGetColorTableSGI(GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLvoid * table);
void glGetColorTableParameterfvSGI(GLenum[Main] target, GLenum[Main] pname, GLfloat * params);
void glGetColorTableParameterivSGI(GLenum[Main] target, GLenum[Main] pname, GLint * params);
