#define GLI_INCLUDE_GL_EXT_MAP_BUFFER_RANGE

enum Main {

  //GL_MAP_READ_BIT_EXT                    = 0x0001,
  //GL_MAP_WRITE_BIT_EXT                   = 0x0002,
  //GL_MAP_INVALIDATE_RANGE_BIT_EXT        = 0x0004,
  //GL_MAP_INVALIDATE_BUFFER_BIT_EXT       = 0x0008,
  //GL_MAP_FLUSH_EXPLICIT_BIT_EXT          = 0x0010,
  //GL_MAP_UNSYNCHRONIZED_BIT_EXT          = 0x0020,

};

void *glMapBufferRangeEXT(GLenum[Main] target, GLintptr offset, GLsizeiptr length, GLbitfield access);
void glFlushMappedBufferRangeEXT(GLenum[Main] target, GLintptr offset, GLsizeiptr length);
