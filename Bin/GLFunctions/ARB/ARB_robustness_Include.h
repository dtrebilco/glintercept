#define GLI_INCLUDE_GL_ARB_ROBUSTNESS

enum Main {

  //GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB       = 0x00000004,
  //GL_LOSE_CONTEXT_ON_RESET_ARB                = 0x8252,
  //GL_GUILTY_CONTEXT_RESET_ARB                 = 0x8253,
  //GL_INNOCENT_CONTEXT_RESET_ARB               = 0x8254,
  //GL_UNKNOWN_CONTEXT_RESET_ARB                = 0x8255,
  //GL_RESET_NOTIFICATION_STRATEGY_ARB          = 0x8256,
  //GL_NO_RESET_NOTIFICATION_ARB                = 0x8261,

};

GLenum glGetGraphicsResetStatusARB(void);
void glGetnMapdvARB(GLenum[Main] target, GLenum[Main] query, GLsizei bufSize, GLdouble * v);
void glGetnMapfvARB(GLenum[Main] target, GLenum[Main] query, GLsizei bufSize, GLfloat * v);
void glGetnMapivARB(GLenum[Main] target, GLenum[Main] query, GLsizei bufSize, GLint * v);
void glGetnPixelMapfvARB(GLenum[Main] map, GLsizei bufSize, GLfloat * values);
void glGetnPixelMapuivARB(GLenum[Main] map, GLsizei bufSize, GLuint * values);
void glGetnPixelMapusvARB(GLenum[Main] map, GLsizei bufSize, GLushort * values);
void glGetnPolygonStippleARB(GLsizei bufSize, GLubyte * pattern);
void glGetnColorTableARB(GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, GLvoid * table);
void glGetnConvolutionFilterARB(GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, GLvoid * image);
void glGetnSeparableFilterARB(GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLsizei rowBufSize, GLvoid * row, GLsizei columnBufSize, GLvoid * column, GLvoid * span);
void glGetnHistogramARB(GLenum[Main] target, GLboolean reset, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, GLvoid * values);
void glGetnMinmaxARB(GLenum[Main] target, GLboolean reset, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, GLvoid * values);
void glGetnTexImageARB(GLenum[Main] target, GLint level, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, GLvoid * img);
void glReadnPixelsARB(GLint x, GLint y, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, GLvoid * data);
void glGetnCompressedTexImageARB(GLenum[Main] target, GLint lod, GLsizei bufSize, GLvoid * img);
void glGetnUniformfvARB(GLuint program, GLint location, GLsizei bufSize, GLfloat * params);
void glGetnUniformivARB(GLuint program, GLint location, GLsizei bufSize, GLint * params);
void glGetnUniformuivARB(GLuint program, GLint location, GLsizei bufSize, GLuint * params);
void glGetnUniformdvARB(GLuint program, GLint location, GLsizei bufSize, GLdouble * params);
