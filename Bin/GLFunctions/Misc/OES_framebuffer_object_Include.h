#define GLI_INCLUDE_GL_OES_FRAMEBUFFER_OBJECT

enum Main {

  //GL_NONE_OES                                               = 0,
  //GL_FRAMEBUFFER_OES                                        = 0x8D40,
  //GL_RENDERBUFFER_OES                                       = 0x8D41,
  //GL_RGBA4_OES                                              = 0x8056,
  //GL_RGB5_A1_OES                                            = 0x8057,
  //GL_RGB565_OES                                             = 0x8D62,
  //GL_DEPTH_COMPONENT16_OES                                  = 0x81A5,
  //GL_RENDERBUFFER_WIDTH_OES                                 = 0x8D42,
  //GL_RENDERBUFFER_HEIGHT_OES                                = 0x8D43,
  //GL_RENDERBUFFER_INTERNAL_FORMAT_OES                       = 0x8D44,
  //GL_RENDERBUFFER_RED_SIZE_OES                              = 0x8D50,
  //GL_RENDERBUFFER_GREEN_SIZE_OES                            = 0x8D51,
  //GL_RENDERBUFFER_BLUE_SIZE_OES                             = 0x8D52,
  //GL_RENDERBUFFER_ALPHA_SIZE_OES                            = 0x8D53,
  //GL_RENDERBUFFER_DEPTH_SIZE_OES                            = 0x8D54,
  //GL_RENDERBUFFER_STENCIL_SIZE_OES                          = 0x8D55,
  //GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES                 = 0x8CD0,
  //GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES                 = 0x8CD1,
  //GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES               = 0x8CD2,
  //GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES       = 0x8CD3,
  //GL_COLOR_ATTACHMENT0_OES                                  = 0x8CE0,
  //GL_DEPTH_ATTACHMENT_OES                                   = 0x8D00,
  //GL_STENCIL_ATTACHMENT_OES                                 = 0x8D20,
  //GL_FRAMEBUFFER_COMPLETE_OES                               = 0x8CD5,
  //GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES                  = 0x8CD6,
  //GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES          = 0x8CD7,
  //GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES                  = 0x8CD9,
  //GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES                     = 0x8CDA,
  //GL_FRAMEBUFFER_UNSUPPORTED_OES                            = 0x8CDD,
  //GL_FRAMEBUFFER_BINDING_OES                                = 0x8CA6,
  //GL_RENDERBUFFER_BINDING_OES                               = 0x8CA7,
  //GL_MAX_RENDERBUFFER_SIZE_OES                              = 0x84E8,
  //GL_INVALID_FRAMEBUFFER_OPERATION_OES                      = 0x0506,

};

GLboolean glIsRenderbufferOES(GLuint renderbuffer);
void glBindRenderbufferOES(GLenum[Main] target, GLuint renderbuffer);
void glDeleteRenderbuffersOES(GLsizei n, const GLuint *renderbuffers);
void glGenRenderbuffersOES(GLsizei n, GLuint *renderbuffers);
void glRenderbufferStorageOES(GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLsizei height);
void glGetRenderbufferParameterivOES(GLenum[Main] target, GLenum[Main] pname, GLint *params);
GLboolean glIsFramebufferOES(GLuint framebuffer);
void glBindFramebufferOES(GLenum[Main] target, GLuint framebuffer);
void glDeleteFramebuffersOES(GLsizei n, const GLuint *framebuffers);
void glGenFramebuffersOES(GLsizei n, GLuint *framebuffers);
GLenum[Main] glCheckFramebufferStatusOES(GLenum[Main] target);
void glFramebufferRenderbufferOES(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] renderbuffertarget, GLuint renderbuffer);
void glFramebufferTexture2DOES(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level);
void glGetFramebufferAttachmentParameterivOES(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] pname, GLint *params);
void glGenerateMipmapOES(GLenum[Main] target);
