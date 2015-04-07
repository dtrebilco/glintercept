#define GLI_INCLUDE_APPLE_FLUSH_BUFFER_RANGE


enum Main {
  
  GL_BUFFER_SERIALIZED_MODIFY_APPLE = 0x8A12,
  GL_BUFFER_FLUSHING_UNMAP_APPLE    = 0x8A13,

};

void glBufferParameteriAPPLE(GLenum[Main] target, GLenum[Main] pname, GLint param);
void glFlushMappedBufferRangeAPPLE(GLenum[Main] target, GLintptr offset, GLsizeiptr size);
