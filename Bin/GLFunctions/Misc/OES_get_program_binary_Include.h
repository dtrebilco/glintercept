#define GLI_INCLUDE_GL_OES_GET_PROGRAM_BINARY

enum Main {

  //GL_PROGRAM_BINARY_LENGTH_OES            = 0x8741,
  //GL_NUM_PROGRAM_BINARY_FORMATS_OES       = 0x87FE,
  //GL_PROGRAM_BINARY_FORMATS_OES           = 0x87FF,

};

void glGetProgramBinaryOES(GLuint program, GLsizei bufSize, GLsizei *length, GLenum[Main] *binaryFormat, void *binary);
void glProgramBinaryOES(GLuint program, GLenum[Main] binaryFormat, const void *binary, GLint length);
