#define GLI_INCLUDE_GL_EXT_BUFFER_STORAGE

enum Main {

  //GL_MAP_PERSISTENT_BIT_EXT                     = 0x0040,
  //GL_MAP_COHERENT_BIT_EXT                       = 0x0080,
  //GL_DYNAMIC_STORAGE_BIT_EXT                    = 0x0100,
  //GL_CLIENT_STORAGE_BIT_EXT                     = 0x0200,
  //GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT_EXT       = 0x00004000,
  //GL_BUFFER_IMMUTABLE_STORAGE_EXT               = 0x821F,
  //GL_BUFFER_STORAGE_FLAGS_EXT                   = 0x8220,

};

void glBufferStorageEXT(GLenum[Main] target, GLsizeiptr size, const void *data, GLbitfield flags);
