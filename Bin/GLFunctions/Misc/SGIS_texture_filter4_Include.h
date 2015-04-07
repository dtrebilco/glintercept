#define GLI_INCLUDE_GL_SGIS_TEXTURE_FILTER4

enum Main {

  GL_FILTER4_SGIS                    = 0x8146,
  GL_TEXTURE_FILTER4_SIZE_SGIS       = 0x8147,

};

void glGetTexFilterFuncSGIS(GLenum[Main] target, GLenum[Main] filter, GLfloat * weights);
void glTexFilterFuncSGIS(GLenum[Main] target, GLenum[Main] filter, GLsizei n, const GLfloat * weights);
