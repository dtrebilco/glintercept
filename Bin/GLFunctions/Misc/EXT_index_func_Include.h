#define GLI_INCLUDE_GL_EXT_INDEX_FUNC

enum Main {

  GL_INDEX_TEST_EXT            = 0x81B5,
  GL_INDEX_TEST_FUNC_EXT       = 0x81B6,
  GL_INDEX_TEST_REF_EXT        = 0x81B7,

};

void glIndexFuncEXT(GLenum[Main] func, GLfloat ref);
