#define GLI_INCLUDE_GL_EXT_MULTIVIEW_DRAW_BUFFERS

enum Main {

  GL_COLOR_ATTACHMENT_EXT            = 0x90F0,
  GL_MULTIVIEW_EXT                   = 0x90F1,
  //GL_DRAW_BUFFER_EXT                 = 0x0C01,
  //GL_READ_BUFFER_EXT                 = 0x0C02,
  GL_MAX_MULTIVIEW_BUFFERS_EXT       = 0x90F2,

};

void glReadBufferIndexedEXT(GLenum[Main] src, GLint index);
void glDrawBuffersIndexedEXT(GLint n, const GLenum[Main] *location, const GLint *indices);
void glGetIntegeri_vEXT(GLenum[Main] target, GLuint index, GLint *data);
