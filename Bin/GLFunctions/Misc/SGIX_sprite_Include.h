#define GLI_INCLUDE_GL_SGIX_SPRITE

enum Main {

  GL_SPRITE_SGIX                      = 0x8148,
  GL_SPRITE_MODE_SGIX                 = 0x8149,
  GL_SPRITE_AXIS_SGIX                 = 0x814A,
  GL_SPRITE_TRANSLATION_SGIX          = 0x814B,
  GL_SPRITE_AXIAL_SGIX                = 0x814C,
  GL_SPRITE_OBJECT_ALIGNED_SGIX       = 0x814D,
  GL_SPRITE_EYE_ALIGNED_SGIX          = 0x814E,

};

void glSpriteParameterfSGIX(GLenum[Main] pname, GLfloat param);
void glSpriteParameterfvSGIX(GLenum[Main] pname, const GLfloat * params);
void glSpriteParameteriSGIX(GLenum[Main] pname, GLint param);
void glSpriteParameterivSGIX(GLenum[Main] pname, const GLint * params);
