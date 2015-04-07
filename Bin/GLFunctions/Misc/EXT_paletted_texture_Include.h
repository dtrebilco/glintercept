#define GLI_INCLUDE_GL_EXT_PALETTED_TEXTURE

enum Main {

  GL_COLOR_INDEX1_EXT             = 0x80E2,
  GL_COLOR_INDEX2_EXT             = 0x80E3,
  GL_COLOR_INDEX4_EXT             = 0x80E4,
  GL_COLOR_INDEX8_EXT             = 0x80E5,
  GL_COLOR_INDEX12_EXT            = 0x80E6,
  GL_COLOR_INDEX16_EXT            = 0x80E7,
  GL_TEXTURE_INDEX_SIZE_EXT       = 0x80ED,

};

void glColorTableEXT(GLenum[Main] target, GLenum[Main] internalFormat, GLsizei width, GLenum[Main] format, GLenum[Main] type, const GLvoid * table);
void glGetColorTableEXT(GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLvoid * data);
void glGetColorTableParameterivEXT(GLenum[Main] target, GLenum[Main] pname, GLint * params);
void glGetColorTableParameterfvEXT(GLenum[Main] target, GLenum[Main] pname, GLfloat * params);
