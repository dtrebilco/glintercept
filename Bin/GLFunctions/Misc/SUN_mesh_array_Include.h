#define GLI_INCLUDE_GL_SUN_MESH_ARRAY

enum Main {

  GL_QUAD_MESH_SUN           = 0x8614,
  GL_TRIANGLE_MESH_SUN       = 0x8615,

};

void glDrawMeshArraysSUN(GLenum[Main] mode, GLint first, GLsizei count, GLsizei width);
