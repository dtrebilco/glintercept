#define GLI_INCLUDE_GL_NV_GPU_SHADER5

enum Main {

  GL_INT64_NV                     = 0x140E,
  //GL_UNSIGNED_INT64_NV            = 0x140F,
  GL_INT8_NV                      = 0x8FE0,
  GL_INT8_VEC2_NV                 = 0x8FE1,
  GL_INT8_VEC3_NV                 = 0x8FE2,
  GL_INT8_VEC4_NV                 = 0x8FE3,
  GL_INT16_NV                     = 0x8FE4,
  GL_INT16_VEC2_NV                = 0x8FE5,
  GL_INT16_VEC3_NV                = 0x8FE6,
  GL_INT16_VEC4_NV                = 0x8FE7,
  GL_INT64_VEC2_NV                = 0x8FE9,
  GL_INT64_VEC3_NV                = 0x8FEA,
  GL_INT64_VEC4_NV                = 0x8FEB,
  GL_UNSIGNED_INT8_NV             = 0x8FEC,
  GL_UNSIGNED_INT8_VEC2_NV        = 0x8FED,
  GL_UNSIGNED_INT8_VEC3_NV        = 0x8FEE,
  GL_UNSIGNED_INT8_VEC4_NV        = 0x8FEF,
  GL_UNSIGNED_INT16_NV            = 0x8FF0,
  GL_UNSIGNED_INT16_VEC2_NV       = 0x8FF1,
  GL_UNSIGNED_INT16_VEC3_NV       = 0x8FF2,
  GL_UNSIGNED_INT16_VEC4_NV       = 0x8FF3,
  GL_UNSIGNED_INT64_VEC2_NV       = 0x8FF5,
  GL_UNSIGNED_INT64_VEC3_NV       = 0x8FF6,
  GL_UNSIGNED_INT64_VEC4_NV       = 0x8FF7,
  GL_FLOAT16_NV                   = 0x8FF8,
  GL_FLOAT16_VEC2_NV              = 0x8FF9,
  GL_FLOAT16_VEC3_NV              = 0x8FFA,
  GL_FLOAT16_VEC4_NV              = 0x8FFB,

};

void glUniform1i64NV(GLint location, GLint64 x);
void glUniform2i64NV(GLint location, GLint64 x, GLint64 y);
void glUniform3i64NV(GLint location, GLint64 x, GLint64 y, GLint64 z);
void glUniform4i64NV(GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
void glUniform1i64vNV(GLint location, GLsizei count, const GLint64 * value);
void glUniform2i64vNV(GLint location, GLsizei count, const GLint64 * value);
void glUniform3i64vNV(GLint location, GLsizei count, const GLint64 * value);
void glUniform4i64vNV(GLint location, GLsizei count, const GLint64 * value);
void glUniform1ui64NV(GLint location, GLuint64 x);
void glUniform2ui64NV(GLint location, GLuint64 x, GLuint64 y);
void glUniform3ui64NV(GLint location, GLuint64 x, GLuint64 y, GLuint64 z);
void glUniform4ui64NV(GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
void glUniform1ui64vNV(GLint location, GLsizei count, const GLuint64 * value);
void glUniform2ui64vNV(GLint location, GLsizei count, const GLuint64 * value);
void glUniform3ui64vNV(GLint location, GLsizei count, const GLuint64 * value);
void glUniform4ui64vNV(GLint location, GLsizei count, const GLuint64 * value);
void glGetUniformi64vNV(GLuint program, GLint location, GLint64 * params);
void glProgramUniform1i64NV(GLuint program, GLint location, GLint64 x);
void glProgramUniform2i64NV(GLuint program, GLint location, GLint64 x, GLint64 y);
void glProgramUniform3i64NV(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z);
void glProgramUniform4i64NV(GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
void glProgramUniform1i64vNV(GLuint program, GLint location, GLsizei count, const GLint64 * value);
void glProgramUniform2i64vNV(GLuint program, GLint location, GLsizei count, const GLint64 * value);
void glProgramUniform3i64vNV(GLuint program, GLint location, GLsizei count, const GLint64 * value);
void glProgramUniform4i64vNV(GLuint program, GLint location, GLsizei count, const GLint64 * value);
void glProgramUniform1ui64NV(GLuint program, GLint location, GLuint64 x);
void glProgramUniform2ui64NV(GLuint program, GLint location, GLuint64 x, GLuint64 y);
void glProgramUniform3ui64NV(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z);
void glProgramUniform4ui64NV(GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
void glProgramUniform1ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
void glProgramUniform2ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
void glProgramUniform3ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
void glProgramUniform4ui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
