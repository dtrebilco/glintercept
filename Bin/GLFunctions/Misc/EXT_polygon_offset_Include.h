#define GLI_INCLUDE_GL_EXT_POLYGON_OFFSET

enum Main {

  //GL_POLYGON_OFFSET_EXT              = 0x8037,
  //GL_POLYGON_OFFSET_FACTOR_EXT       = 0x8038,
  GL_POLYGON_OFFSET_BIAS_EXT         = 0x8039,

};

void glPolygonOffsetEXT(GLfloat factor, GLfloat bias);
