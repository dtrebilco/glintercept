#define GLI_INCLUDE_NV_DEPTH_BUFFER_FLOAT

enum Main {

  GL_DEPTH_COMPONENT32F_NV             = 0x8DAB,
  GL_DEPTH32F_STENCIL8_NV              = 0x8DAC,
  //GL_FLOAT_32_UNSIGNED_INT_24_8_REV_NV = 0x8DAD,
  GL_DEPTH_BUFFER_FLOAT_MODE_NV        = 0x8DAF,

};

void glDepthRangedNV(GLdouble n, GLdouble f);
void glClearDepthdNV(GLdouble d);
void glDepthBoundsdNV(GLdouble zmin, GLdouble zmax);