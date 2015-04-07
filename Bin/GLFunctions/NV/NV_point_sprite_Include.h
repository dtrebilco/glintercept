#define GLI_INCLUDE_NV_POINT_SPRITE


///////////////////////////////////////////////////////////////////////////////
// Main GLenum
///////////////////////////////////////////////////////////////////////////////

enum Main {

  //GL_POINT_SPRITE_NV                    =   0x8861,
  //GL_COORD_REPLACE_NV                   =   0x8862,
  GL_POINT_SPRITE_R_MODE_NV             =   0x8863,

};

void glPointParameteriNV(GLenum[Main] pname, GLint param);
void glPointParameterivNV(GLenum[Main] pname, const GLint *params);
