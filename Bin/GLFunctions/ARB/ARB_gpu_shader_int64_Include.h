#define GLI_INCLUDE_GL_ARB_GPU_SHADER_INT64

enum Main {

  GL_INT64_ARB                     = 0x140E,
  GL_INT64_VEC2_ARB                = 0x8FE9,
  GL_INT64_VEC3_ARB                = 0x8FEA,
  GL_INT64_VEC4_ARB                = 0x8FEB,
  GL_UNSIGNED_INT64_VEC2_ARB       = 0x8FF5,
  GL_UNSIGNED_INT64_VEC3_ARB       = 0x8FF6,
  GL_UNSIGNED_INT64_VEC4_ARB       = 0x8FF7,

};

void glUniform1i64ARB(GLint location, GLint64 x);
void glUniform2i64ARB(GLint location, GLint64 x, GLint64 y);
void glUniform3i64ARB(GLint location, GLint64 x, GLint64 y, GLint64 z);
void glUniform4i64ARB(GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
void glUniform1i64vARB(GLint location, GLsizei count, const GLint64 *value);
void glUniform2i64vARB(GLint location, GLsizei count, const GLint64 *value);
void glUniform3i64vARB(GLint location, GLsizei count, const GLint64 *value);
void glUniform4i64vARB(GLint location, GLsizei count, const GLint64 *value);
void glUniform1ui64ARB(GLint location, GLuint64 x);
void glUniform2ui64ARB(GLint location, GLuint64 x, GLuint64 y);
void glUniform3ui64ARB(GLint location, GLuint64 x, GLuint64 y, GLuint64 z);
void glUniform4ui64ARB(GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
void glUniform1ui64vARB(GLint location, GLsizei count, const GLuint64 *value);
void glUniform2ui64vARB(GLint location, GLsizei count, const GLuint64 *value);
void glUniform3ui64vARB(GLint location, GLsizei count, const GLuint64 *value);
void glUniform4ui64vARB(GLint location, GLsizei count, const GLuint64 *value);
void glGetUniformi64vARB(GLuint program, GLint location, GLint64 *params);
void glGetUniformui64vARB(GLuint program, GLint location, GLuint64 *params);
void glGetnUniformi64vARB(GLuint program, GLint location, GLsizei bufSize, GLint64 *params);
void glGetnUniformui64vARB(GLuint program, GLint location, GLsizei bufSize, GLuint64 *params);
void glProgramUniform1i64ARB(GLuint program, GLint location, GLint64 x);
void glProgramUniform2i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y);
void glProgramUniform3i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z);
void glProgramUniform4i64ARB(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
void glProgramUniform1i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value);
void glProgramUniform2i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value);
void glProgramUniform3i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value);
void glProgramUniform4i64vARB(GLuint program, GLint location, GLsizei count, const GLint64 *value);
void glProgramUniform1ui64ARB(GLuint program, GLint location, GLuint64 x);
void glProgramUniform2ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y);
void glProgramUniform3ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z);
void glProgramUniform4ui64ARB(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
void glProgramUniform1ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value);
void glProgramUniform2ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value);
void glProgramUniform3ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value);
void glProgramUniform4ui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *value);
