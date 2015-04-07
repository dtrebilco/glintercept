#define GLI_INCLUDE_GL_IMG_MULTISAMPLED_RENDER_TO_TEXTURE

enum Main {

  GL_RENDERBUFFER_SAMPLES_IMG                     = 0x9133,
  GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_IMG       = 0x9134,
  GL_MAX_SAMPLES_IMG                              = 0x9135,
  GL_TEXTURE_SAMPLES_IMG                          = 0x9136,

};

void glRenderbufferStorageMultisampleIMG(GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height);
void glFramebufferTexture2DMultisampleIMG(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level, GLsizei samples);
