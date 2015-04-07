#define GLI_INCLUDE_GL_SGIX_REFERENCE_PLANE

enum Main {

  GL_REFERENCE_PLANE_SGIX                = 0x817D,
  GL_REFERENCE_PLANE_EQUATION_SGIX       = 0x817E,

};

void glReferencePlaneSGIX(const GLdouble * equation);
