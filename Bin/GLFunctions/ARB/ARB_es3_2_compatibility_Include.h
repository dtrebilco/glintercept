#define GLI_INCLUDE_GL_ARB_ES3_2_COMPATIBILITY

enum Main {

  GL_PRIMITIVE_BOUNDING_BOX_ARB                   = 0x92BE,
  GL_MULTISAMPLE_LINE_WIDTH_RANGE_ARB             = 0x9381,
  GL_MULTISAMPLE_LINE_WIDTH_GRANULARITY_ARB       = 0x9382,

};

void glPrimitiveBoundingBoxARB(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
