#define GLI_INCLUDE_NV_PRIMITIVE_RESTART

enum Main {

  GL_PRIMITIVE_RESTART_NV             =    0x8558,
  GL_PRIMITIVE_RESTART_INDEX_NV       =    0x8559,

};

void glPrimitiveRestartNV(void);
void glPrimitiveRestartIndexNV(GLuint index);


