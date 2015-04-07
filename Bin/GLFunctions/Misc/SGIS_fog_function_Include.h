#define GLI_INCLUDE_GL_SGIS_FOG_FUNCTION

enum Main {

  GL_FOG_FUNC_SGIS                  = 0x812A,
  GL_FOG_FUNC_POINTS_SGIS           = 0x812B,
  GL_MAX_FOG_FUNC_POINTS_SGIS       = 0x812C,

};

void glFogFuncSGIS(GLsizei n, const GLfloat * points);
void glGetFogFuncSGIS(GLfloat * points);
