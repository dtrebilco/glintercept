#define GLI_INCLUDE_GL_SUN_GLOBAL_ALPHA

enum Main {

  GL_GLOBAL_ALPHA_SUN              = 0x81D9,
  GL_GLOBAL_ALPHA_FACTOR_SUN       = 0x81DA,

};

void glGlobalAlphaFactorbSUN(GLbyte factor);
void glGlobalAlphaFactorsSUN(GLshort factor);
void glGlobalAlphaFactoriSUN(GLint factor);
void glGlobalAlphaFactorfSUN(GLfloat factor);
void glGlobalAlphaFactordSUN(GLdouble factor);
void glGlobalAlphaFactorubSUN(GLubyte factor);
void glGlobalAlphaFactorusSUN(GLushort factor);
void glGlobalAlphaFactoruiSUN(GLuint factor);
