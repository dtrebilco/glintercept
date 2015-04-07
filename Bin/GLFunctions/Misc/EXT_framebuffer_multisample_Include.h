#define GLI_INCLUDE_EXT_FRAMEBUFFER_MULTISAMPLE


enum Main {
  
 // GL_RENDERBUFFER_SAMPLES_EXT               = 0x8CAB,
 // GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT = 0x8D56,
 // GL_MAX_SAMPLES_EXT                        = 0x8D57,

};

void glRenderbufferStorageMultisampleEXT(GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height);
