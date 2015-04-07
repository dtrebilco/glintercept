#define GLI_INCLUDE_GL_SGIS_PIXEL_TEXTURE

enum Main {

  GL_PIXEL_TEXTURE_SGIS                     = 0x8353,
  GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS         = 0x8354,
  GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS       = 0x8355,
  GL_PIXEL_GROUP_COLOR_SGIS                 = 0x8356,

};

void glPixelTexGenParameteriSGIS(GLenum[Main] pname, GLint param);
void glPixelTexGenParameterivSGIS(GLenum[Main] pname, const GLint * params);
void glPixelTexGenParameterfSGIS(GLenum[Main] pname, GLfloat param);
void glPixelTexGenParameterfvSGIS(GLenum[Main] pname, const GLfloat * params);
void glGetPixelTexGenParameterivSGIS(GLenum[Main] pname, GLint * params);
void glGetPixelTexGenParameterfvSGIS(GLenum[Main] pname, GLfloat * params);
