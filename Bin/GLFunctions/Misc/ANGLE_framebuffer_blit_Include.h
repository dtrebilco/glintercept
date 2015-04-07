#define GLI_INCLUDE_GL_ANGLE_FRAMEBUFFER_BLIT

enum Main {

  //GL_READ_FRAMEBUFFER_ANGLE               = 0x8CA8,
  //GL_DRAW_FRAMEBUFFER_ANGLE               = 0x8CA9,
  //GL_DRAW_FRAMEBUFFER_BINDING_ANGLE       = 0x8CA6,
  //GL_READ_FRAMEBUFFER_BINDING_ANGLE       = 0x8CAA,

};

void glBlitFramebufferANGLE(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum[Main] filter);
