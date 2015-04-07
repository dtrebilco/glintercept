#define GLI_INCLUDE_GL_APPLE_FRAMEBUFFER_MULTISAMPLE

enum Main {

  //GL_RENDERBUFFER_SAMPLES_APPLE                     = 0x8CAB,
  //GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_APPLE       = 0x8D56,
  //GL_MAX_SAMPLES_APPLE                              = 0x8D57,
  //GL_READ_FRAMEBUFFER_APPLE                         = 0x8CA8,
  //GL_DRAW_FRAMEBUFFER_APPLE                         = 0x8CA9,
  //GL_DRAW_FRAMEBUFFER_BINDING_APPLE                 = 0x8CA6,
  //GL_READ_FRAMEBUFFER_BINDING_APPLE                 = 0x8CAA,

};

void glRenderbufferStorageMultisampleAPPLE(GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height);
void glResolveMultisampleFramebufferAPPLE(void);
