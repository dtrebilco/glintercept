#define GLI_INCLUDE_GL_NV_FRAMEBUFFER_MULTISAMPLE

enum Main {

  //GL_RENDERBUFFER_SAMPLES_NV                     = 0x8CAB,
  //GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_NV       = 0x8D56,
  //GL_MAX_SAMPLES_NV                              = 0x8D57,

};

void glRenderbufferStorageMultisampleNV(GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height);
