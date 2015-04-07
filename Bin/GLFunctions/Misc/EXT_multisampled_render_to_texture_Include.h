#define GLI_INCLUDE_GL_EXT_MULTISAMPLED_RENDER_TO_TEXTURE

enum Main {

  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_SAMPLES_EXT       = 0x8D6C,

};

void glFramebufferTexture2DMultisampleEXT(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level, GLsizei samples);
