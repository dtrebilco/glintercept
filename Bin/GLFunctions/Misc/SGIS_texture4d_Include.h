#define GLI_INCLUDE_GL_SGIS_TEXTURE4D

enum Main {

  GL_PACK_SKIP_VOLUMES_SGIS         = 0x8130,
  GL_PACK_IMAGE_DEPTH_SGIS          = 0x8131,
  GL_UNPACK_SKIP_VOLUMES_SGIS       = 0x8132,
  GL_UNPACK_IMAGE_DEPTH_SGIS        = 0x8133,
  GL_TEXTURE_4D_SGIS                = 0x8134,
  GL_PROXY_TEXTURE_4D_SGIS          = 0x8135,
  GL_TEXTURE_4DSIZE_SGIS            = 0x8136,
  GL_TEXTURE_WRAP_Q_SGIS            = 0x8137,
  GL_MAX_4D_TEXTURE_SIZE_SGIS       = 0x8138,
  GL_TEXTURE_4D_BINDING_SGIS        = 0x814F,

};

void glTexImage4DSGIS(GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum[Main] format, GLenum[Main] type, const GLvoid * pixels);
void glTexSubImage4DSGIS(GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum[Main] format, GLenum[Main] type, const GLvoid * pixels);
