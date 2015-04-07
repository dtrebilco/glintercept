#define GLI_INCLUDE_GL_SGIS_SHARPEN_TEXTURE

enum Main {

  GL_LINEAR_SHARPEN_SGIS                    = 0x80AD,
  GL_LINEAR_SHARPEN_ALPHA_SGIS              = 0x80AE,
  GL_LINEAR_SHARPEN_COLOR_SGIS              = 0x80AF,
  GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS       = 0x80B0,

};

void glSharpenTexFuncSGIS(GLenum[Main] target, GLsizei n, const GLfloat * points);
void glGetSharpenTexFuncSGIS(GLenum[Main] target, GLfloat * points);
