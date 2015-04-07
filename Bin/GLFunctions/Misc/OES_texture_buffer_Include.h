#define GLI_INCLUDE_GL_OES_TEXTURE_BUFFER

enum Main {

  //GL_TEXTURE_BUFFER_OES                          = 0x8C2A,
  //GL_TEXTURE_BUFFER_BINDING_OES                  = 0x8C2A,
  //GL_MAX_TEXTURE_BUFFER_SIZE_OES                 = 0x8C2B,
  //GL_TEXTURE_BINDING_BUFFER_OES                  = 0x8C2C,
  //GL_TEXTURE_BUFFER_DATA_STORE_BINDING_OES       = 0x8C2D,
  //GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT_OES         = 0x919F,
  //GL_SAMPLER_BUFFER_OES                          = 0x8DC2,
  //GL_INT_SAMPLER_BUFFER_OES                      = 0x8DD0,
  //GL_UNSIGNED_INT_SAMPLER_BUFFER_OES             = 0x8DD8,
  //GL_IMAGE_BUFFER_OES                            = 0x9051,
  //GL_INT_IMAGE_BUFFER_OES                        = 0x905C,
  //GL_UNSIGNED_INT_IMAGE_BUFFER_OES               = 0x9067,
  //GL_TEXTURE_BUFFER_OFFSET_OES                   = 0x919D,
  //GL_TEXTURE_BUFFER_SIZE_OES                     = 0x919E,

};

void glTexBufferOES(GLenum[Main] target, GLenum[Main] internalformat, GLuint buffer);
void glTexBufferRangeOES(GLenum[Main] target, GLenum[Main] internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
