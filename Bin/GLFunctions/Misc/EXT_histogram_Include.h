#define GLI_INCLUDE_GL_EXT_HISTOGRAM

enum Main {

  //GL_HISTOGRAM_EXT                      = 0x8024,
  //GL_PROXY_HISTOGRAM_EXT                = 0x8025,
  //GL_HISTOGRAM_WIDTH_EXT                = 0x8026,
  //GL_HISTOGRAM_FORMAT_EXT               = 0x8027,
  //GL_HISTOGRAM_RED_SIZE_EXT             = 0x8028,
  //GL_HISTOGRAM_GREEN_SIZE_EXT           = 0x8029,
  //GL_HISTOGRAM_BLUE_SIZE_EXT            = 0x802A,
  //GL_HISTOGRAM_ALPHA_SIZE_EXT           = 0x802B,
  //GL_HISTOGRAM_LUMINANCE_SIZE_EXT       = 0x802C,
  //GL_HISTOGRAM_SINK_EXT                 = 0x802D,
  //GL_MINMAX_EXT                         = 0x802E,
  //GL_MINMAX_FORMAT_EXT                  = 0x802F,
  //GL_MINMAX_SINK_EXT                    = 0x8030,
  //GL_TABLE_TOO_LARGE_EXT                = 0x8031,

};

void glGetHistogramEXT(GLenum[Main] target, GLboolean reset, GLenum[Main] format, GLenum[Main] type, GLvoid * values);
void glGetHistogramParameterfvEXT(GLenum[Main] target, GLenum[Main] pname, GLfloat * params);
void glGetHistogramParameterivEXT(GLenum[Main] target, GLenum[Main] pname, GLint * params);
void glGetMinmaxEXT(GLenum[Main] target, GLboolean reset, GLenum[Main] format, GLenum[Main] type, GLvoid * values);
void glGetMinmaxParameterfvEXT(GLenum[Main] target, GLenum[Main] pname, GLfloat * params);
void glGetMinmaxParameterivEXT(GLenum[Main] target, GLenum[Main] pname, GLint * params);
void glHistogramEXT(GLenum[Main] target, GLsizei width, GLenum[Main] internalformat, GLboolean sink);
void glMinmaxEXT(GLenum[Main] target, GLenum[Main] internalformat, GLboolean sink);
void glResetHistogramEXT(GLenum[Main] target);
void glResetMinmaxEXT(GLenum[Main] target);
