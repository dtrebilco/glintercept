#define GLI_INCLUDE_GL_EXT_DISCARD_FRAMEBUFFER

enum Main {

  //GL_COLOR_EXT         = 0x1800,
  //GL_DEPTH_EXT         = 0x1801,
  //GL_STENCIL_EXT       = 0x1802,

};

void glDiscardFramebufferEXT(GLenum[Main] target, GLsizei numAttachments, const GLenum[Main] *attachments);
