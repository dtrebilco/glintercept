#define GLI_INCLUDE_GL_OES_TEXTURE_CUBE_MAP

enum Main {

  //GL_NORMAL_MAP_OES                        = 0x8511,
  //GL_REFLECTION_MAP_OES                    = 0x8512,
  //GL_TEXTURE_CUBE_MAP_OES                  = 0x8513,
  //GL_TEXTURE_BINDING_CUBE_MAP_OES          = 0x8514,
  //GL_TEXTURE_CUBE_MAP_POSITIVE_X_OES       = 0x8515,
  //GL_TEXTURE_CUBE_MAP_NEGATIVE_X_OES       = 0x8516,
  //GL_TEXTURE_CUBE_MAP_POSITIVE_Y_OES       = 0x8517,
  //GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_OES       = 0x8518,
  //GL_TEXTURE_CUBE_MAP_POSITIVE_Z_OES       = 0x8519,
  //GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_OES       = 0x851A,
  //GL_MAX_CUBE_MAP_TEXTURE_SIZE_OES         = 0x851C,
  //GL_TEXTURE_GEN_MODE_OES                  = 0x2500,
  GL_TEXTURE_GEN_STR_OES                   = 0x8D60,

};

void glTexGenfOES(GLenum[Main] coord, GLenum[Main] pname, GLfloat param);
void glTexGenfvOES(GLenum[Main] coord, GLenum[Main] pname, const GLfloat *params);
void glTexGeniOES(GLenum[Main] coord, GLenum[Main] pname, GLint param);
void glTexGenivOES(GLenum[Main] coord, GLenum[Main] pname, const GLint *params);
void glGetTexGenfvOES(GLenum[Main] coord, GLenum[Main] pname, GLfloat *params);
void glGetTexGenivOES(GLenum[Main] coord, GLenum[Main] pname, GLint *params);
