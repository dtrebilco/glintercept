#define GLI_INCLUDE_GL_NV_COPY_BUFFER

enum Main {

  //GL_COPY_READ_BUFFER_NV        = 0x8F36,
  //GL_COPY_WRITE_BUFFER_NV       = 0x8F37,

};

void glCopyBufferSubDataNV(GLenum[Main] readTarget, GLenum[Main] writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
