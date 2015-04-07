#define GLI_INCLUDE_EXT_DEPTH_BOUNDS_TEST

enum Main {
  GL_DEPTH_BOUNDS_TEST_EXT   =  0x8890,
  GL_DEPTH_BOUNDS_EXT        =  0x8891,
};

void glDepthBoundsEXT(GLdouble zmin, GLdouble zmax);


