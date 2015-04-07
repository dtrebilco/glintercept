#define GLI_INCLUDE_GL_EXT_CULL_VERTEX

enum Main {

  GL_CULL_VERTEX_EXT                       = 0x81AA,
  GL_CULL_VERTEX_EYE_POSITION_EXT          = 0x81AB,
  GL_CULL_VERTEX_OBJECT_POSITION_EXT       = 0x81AC,

};

void glCullParameterdvEXT(GLenum[Main] pname, GLdouble * params);
void glCullParameterfvEXT(GLenum[Main] pname, GLfloat * params);
