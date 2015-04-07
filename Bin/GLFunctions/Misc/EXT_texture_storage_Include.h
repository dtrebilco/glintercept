#define GLI_INCLUDE_GL_EXT_TEXTURE_STORAGE

enum Main {

  //GL_TEXTURE_IMMUTABLE_FORMAT_EXT       = 0x912F,
  //GL_RGBA32F_EXT                        = 0x8814,
  //GL_RGB32F_EXT                         = 0x8815,
  //GL_ALPHA32F_EXT                       = 0x8816,
  //GL_LUMINANCE32F_EXT                   = 0x8818,
  //GL_LUMINANCE_ALPHA32F_EXT             = 0x8819,
  //GL_ALPHA16F_EXT                       = 0x881C,
  //GL_LUMINANCE16F_EXT                   = 0x881E,
  //GL_LUMINANCE_ALPHA16F_EXT             = 0x881F,
  //GL_R32F_EXT                           = 0x822E,
  //GL_RG32F_EXT                          = 0x8230,

};

void glTexStorage1DEXT(GLenum[Main] target, GLsizei levels, GLenum[Main] internalformat, GLsizei width);
void glTexStorage2DEXT(GLenum[Main] target, GLsizei levels, GLenum[Main] internalformat, GLsizei width, GLsizei height);
void glTexStorage3DEXT(GLenum[Main] target, GLsizei levels, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth);
