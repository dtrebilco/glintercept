#define GLI_INCLUDE_GL_SGIS_DETAIL_TEXTURE

enum Main {

  GL_DETAIL_TEXTURE_2D_SGIS                = 0x8095,
  GL_DETAIL_TEXTURE_2D_BINDING_SGIS        = 0x8096,
  GL_LINEAR_DETAIL_SGIS                    = 0x8097,
  GL_LINEAR_DETAIL_ALPHA_SGIS              = 0x8098,
  GL_LINEAR_DETAIL_COLOR_SGIS              = 0x8099,
  GL_DETAIL_TEXTURE_LEVEL_SGIS             = 0x809A,
  GL_DETAIL_TEXTURE_MODE_SGIS              = 0x809B,
  GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS       = 0x809C,

};

void glDetailTexFuncSGIS(GLenum[Main] target, GLsizei n, const GLfloat * points);
void glGetDetailTexFuncSGIS(GLenum[Main] target, GLfloat * points);
