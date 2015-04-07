#define GLI_INCLUDE_GL_EXT_CONVOLUTION

enum Main {

  //GL_CONVOLUTION_1D_EXT                     = 0x8010,
  //GL_CONVOLUTION_2D_EXT                     = 0x8011,
  //GL_SEPARABLE_2D_EXT                       = 0x8012,
  //GL_CONVOLUTION_BORDER_MODE_EXT            = 0x8013,
  //GL_CONVOLUTION_FILTER_SCALE_EXT           = 0x8014,
  //GL_CONVOLUTION_FILTER_BIAS_EXT            = 0x8015,
  //GL_REDUCE_EXT                             = 0x8016,
  //GL_CONVOLUTION_FORMAT_EXT                 = 0x8017,
  //GL_CONVOLUTION_WIDTH_EXT                  = 0x8018,
  //GL_CONVOLUTION_HEIGHT_EXT                 = 0x8019,
  //GL_MAX_CONVOLUTION_WIDTH_EXT              = 0x801A,
  //GL_MAX_CONVOLUTION_HEIGHT_EXT             = 0x801B,
  //GL_POST_CONVOLUTION_RED_SCALE_EXT         = 0x801C,
  //GL_POST_CONVOLUTION_GREEN_SCALE_EXT       = 0x801D,
  //GL_POST_CONVOLUTION_BLUE_SCALE_EXT        = 0x801E,
  //GL_POST_CONVOLUTION_ALPHA_SCALE_EXT       = 0x801F,
  //GL_POST_CONVOLUTION_RED_BIAS_EXT          = 0x8020,
  //GL_POST_CONVOLUTION_GREEN_BIAS_EXT        = 0x8021,
  //GL_POST_CONVOLUTION_BLUE_BIAS_EXT         = 0x8022,
  //GL_POST_CONVOLUTION_ALPHA_BIAS_EXT        = 0x8023,

};

void glConvolutionFilter1DEXT(GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLenum[Main] format, GLenum[Main] type, const GLvoid * image);
void glConvolutionFilter2DEXT(GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, const GLvoid * image);
void glConvolutionParameterfEXT(GLenum[Main] target, GLenum[Main] pname, GLfloat params);
void glConvolutionParameterfvEXT(GLenum[Main] target, GLenum[Main] pname, const GLfloat * params);
void glConvolutionParameteriEXT(GLenum[Main] target, GLenum[Main] pname, GLint params);
void glConvolutionParameterivEXT(GLenum[Main] target, GLenum[Main] pname, const GLint * params);
void glCopyConvolutionFilter1DEXT(GLenum[Main] target, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width);
void glCopyConvolutionFilter2DEXT(GLenum[Main] target, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
void glGetConvolutionFilterEXT(GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLvoid * image);
void glGetConvolutionParameterfvEXT(GLenum[Main] target, GLenum[Main] pname, GLfloat * params);
void glGetConvolutionParameterivEXT(GLenum[Main] target, GLenum[Main] pname, GLint * params);
void glGetSeparableFilterEXT(GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLvoid * row, GLvoid * column, GLvoid * span);
void glSeparableFilter2DEXT(GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, const GLvoid * row, const GLvoid * column);
