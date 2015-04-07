#define GLI_INCLUDE_GL_SGIS_POINT_PARAMETERS

enum Main {

  //GL_POINT_SIZE_MIN_SGIS                  = 0x8126,
  //GL_POINT_SIZE_MAX_SGIS                  = 0x8127,
  //GL_POINT_FADE_THRESHOLD_SIZE_SGIS       = 0x8128,
  //GL_DISTANCE_ATTENUATION_SGIS            = 0x8129,

};

void glPointParameterfSGIS(GLenum[Main] pname, GLfloat param);
void glPointParameterfvSGIS(GLenum[Main] pname, const GLfloat * params);
