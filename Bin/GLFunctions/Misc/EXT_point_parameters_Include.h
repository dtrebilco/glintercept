#define GLI_INCLUDE_GL_EXT_POINT_PARAMETERS

enum Main {

  //GL_POINT_SIZE_MIN_EXT                  = 0x8126,
  //GL_POINT_SIZE_MAX_EXT                  = 0x8127,
  //GL_POINT_FADE_THRESHOLD_SIZE_EXT       = 0x8128,
  //GL_DISTANCE_ATTENUATION_EXT            = 0x8129,

};

void glPointParameterfEXT(GLenum[Main] pname, GLfloat param);
void glPointParameterfvEXT(GLenum[Main] pname, const GLfloat * params);
