#define GLI_INCLUDE_GL_EXT_PRIMITIVE_BOUNDING_BOX

enum Main {

  GL_PRIMITIVE_BOUNDING_BOX_EXT       = 0x92BE,

};

void glPrimitiveBoundingBoxEXT(GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);
