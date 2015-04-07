#define GLI_INCLUDE_GL_EXT_TEXTURE_BUFFER

enum Main {

  //GL_TEXTURE_BUFFER_EXT                          = 0x8C2A,
  //GL_TEXTURE_BUFFER_BINDING_EXT                  = 0x8C2A,
  //GL_MAX_TEXTURE_BUFFER_SIZE_EXT                 = 0x8C2B,
  //GL_TEXTURE_BINDING_BUFFER_EXT                  = 0x8C2C,
  //GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT       = 0x8C2D,
  //GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT_EXT         = 0x919F,
  //GL_TEXTURE_BUFFER_OFFSET_EXT                   = 0x919D,
  //GL_TEXTURE_BUFFER_SIZE_EXT                     = 0x919E,

};

void glTexBufferEXT(GLenum[Main] target, GLenum[Main] internalformat, GLuint buffer);
void glTexBufferRangeEXT(GLenum[Main] target, GLenum[Main] internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
