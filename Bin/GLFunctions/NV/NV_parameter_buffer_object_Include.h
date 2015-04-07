#define GLI_INCLUDE_NV_PARAMETER_BUFFER_OBJECT

enum Main {

  GL_MAX_PROGRAM_PARAMETER_BUFFER_BINDINGS_NV = 0x8DA0,
  GL_MAX_PROGRAM_PARAMETER_BUFFER_SIZE_NV     = 0x8DA1,
  GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV       = 0x8DA2,
  GL_GEOMETRY_PROGRAM_PARAMETER_BUFFER_NV     = 0x8DA3,
  GL_FRAGMENT_PROGRAM_PARAMETER_BUFFER_NV     = 0x8DA4,

};

void glBindBufferRangeNV(GLenum[Main] target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glBindBufferOffsetNV(GLenum[Main] target, GLuint index, GLuint buffer, GLintptr offset);
void glBindBufferBaseNV(GLenum[Main] target, GLuint index, GLuint buffer);

void glProgramBufferParametersfvNV(GLenum[Main] target, GLuint buffer, GLuint index, GLsizei count, const GLfloat *params);
void glProgramBufferParametersIivNV(GLenum[Main] target, GLuint buffer, GLuint index, GLsizei count, const GLint *params);
void glProgramBufferParametersIuivNV(GLenum[Main] target, GLuint buffer, GLuint index, GLsizei count, const GLuint *params);

//void glGetIntegerIndexedvEXT(GLenum[Main] value, GLuint index, GLboolean *data);