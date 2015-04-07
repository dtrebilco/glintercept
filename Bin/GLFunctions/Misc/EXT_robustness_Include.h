#define GLI_INCLUDE_GL_EXT_ROBUSTNESS

enum Main {

  //GL_GUILTY_CONTEXT_RESET_EXT              = 0x8253,
  //GL_INNOCENT_CONTEXT_RESET_EXT            = 0x8254,
  //GL_UNKNOWN_CONTEXT_RESET_EXT             = 0x8255,
  GL_CONTEXT_ROBUST_ACCESS_EXT             = 0x90F3,
  //GL_RESET_NOTIFICATION_STRATEGY_EXT       = 0x8256,
  //GL_LOSE_CONTEXT_ON_RESET_EXT             = 0x8252,
  //GL_NO_RESET_NOTIFICATION_EXT             = 0x8261,

};

GLenum[Main] glGetGraphicsResetStatusEXT(void);
void glReadnPixelsEXT(GLint x, GLint y, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, void *data);
void glGetnUniformfvEXT(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
void glGetnUniformivEXT(GLuint program, GLint location, GLsizei bufSize, GLint *params);
