#define GLI_INCLUDE_GL_OES_TEXTURE_3D

enum Main {

  //GL_TEXTURE_WRAP_R_OES                                  = 0x8072,
  //GL_TEXTURE_3D_OES                                      = 0x806F,
  //GL_TEXTURE_BINDING_3D_OES                              = 0x806A,
  //GL_MAX_3D_TEXTURE_SIZE_OES                             = 0x8073,
  //GL_SAMPLER_3D_OES                                      = 0x8B5F,
  //GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_OES       = 0x8CD4,

};

void glTexImage3DOES(GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glTexSubImage3DOES(GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glCopyTexSubImage3DOES(GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void glCompressedTexImage3DOES(GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
void glCompressedTexSubImage3DOES(GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLsizei imageSize, const void *data);
void glFramebufferTexture3DOES(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level, GLint zoffset);
