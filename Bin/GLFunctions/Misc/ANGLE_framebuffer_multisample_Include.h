#define GLI_INCLUDE_GL_ANGLE_FRAMEBUFFER_MULTISAMPLE

enum Main {

  //GL_RENDERBUFFER_SAMPLES_ANGLE                     = 0x8CAB,
  //GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_ANGLE       = 0x8D56,
  //GL_MAX_SAMPLES_ANGLE                              = 0x8D57,

};

void glRenderbufferStorageMultisampleANGLE(GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height);
