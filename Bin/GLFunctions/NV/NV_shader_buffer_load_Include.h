#define GLI_INCLUDE_GL_NV_SHADER_BUFFER_LOAD

enum Main {

  GL_BUFFER_GPU_ADDRESS_NV              = 0x8F1D,
  GL_GPU_ADDRESS_NV                     = 0x8F34,
  GL_MAX_SHADER_BUFFER_ADDRESS_NV       = 0x8F35,

};

void glMakeBufferResidentNV(GLenum[Main] target, GLenum[Main] access);
void glMakeBufferNonResidentNV(GLenum[Main] target);
GLboolean glIsBufferResidentNV(GLenum[Main] target);
void glMakeNamedBufferResidentNV(GLuint buffer, GLenum[Main] access);
void glMakeNamedBufferNonResidentNV(GLuint buffer);
GLboolean glIsNamedBufferResidentNV(GLuint buffer);
void glGetBufferParameterui64vNV(GLenum[Main] target, GLenum[Main] pname, GLuint64 * params);
void glGetNamedBufferParameterui64vNV(GLuint buffer, GLenum[Main] pname, GLuint64 * params);
void glGetIntegerui64vNV(GLenum[Main] value, GLuint64 * result);
void glUniformui64NV(GLint location, GLuint64 value);
void glUniformui64vNV(GLint location, GLsizei count, const GLuint64 * value);
void glGetUniformui64vNV(GLuint program, GLint location, GLuint64 * params);
void glProgramUniformui64NV(GLuint program, GLint location, GLuint64 value);
void glProgramUniformui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64 * value);
