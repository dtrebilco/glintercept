#define GLI_INCLUDE_GL_OVR_MULTIVIEW

enum Main {

  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_NUM_VIEWS_OVR             = 0x9630,
  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_BASE_VIEW_INDEX_OVR       = 0x9632,
  GL_MAX_VIEWS_OVR                                            = 0x9631,

};

void glFramebufferTextureMultiviewOVR(GLenum[Main] target, GLenum[Main] attachment, GLuint texture, GLint level, GLint baseViewIndex, GLsizei numViews);
