#define GLI_INCLUDE_GL_VERSION_4_4

enum Main {

  GL_MAX_VERTEX_ATTRIB_STRIDE                      = 0x82E5,
  GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED       = 0x8221,
  GL_TEXTURE_BUFFER_BINDING                        = 0x8C2A,
  GL_BUFFER_IMMUTABLE_STORAGE                      = 0x821F,
  GL_BUFFER_STORAGE_FLAGS                          = 0x8220,
  GL_CLEAR_TEXTURE                                 = 0x9365,
  GL_LOCATION_COMPONENT                            = 0x934A,
  GL_TRANSFORM_FEEDBACK_BUFFER_INDEX               = 0x934B,
  GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE              = 0x934C,
  GL_QUERY_BUFFER                                  = 0x9192,
  GL_QUERY_BUFFER_BINDING                          = 0x9193,
  GL_QUERY_RESULT_NO_WAIT                          = 0x9194,
  GL_MIRROR_CLAMP_TO_EDGE                          = 0x8743,

};

enum Mask_Access {

  GL_MAP_PERSISTENT_BIT                            = 0x0040,
  GL_MAP_COHERENT_BIT                              = 0x0080,
  GL_DYNAMIC_STORAGE_BIT                           = 0x0100,
  GL_CLIENT_STORAGE_BIT                            = 0x0200,

};
  

enum Mask_MemBarrierMask {

  GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT              = 0x00004000,
  GL_QUERY_BUFFER_BARRIER_BIT                      = 0x00008000,

};

void glBufferStorage(GLenum[Main] target, GLsizeiptr size, const void *data, GLbitfield[Mask_Access] flags);
void glClearTexImage(GLuint texture, GLint level, GLenum[Main] format, GLenum[Main] type, const void *data);
void glClearTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLenum[Main] type, const void *data);
void glBindBuffersBase(GLenum[Main] target, GLuint first, GLsizei count, const GLuint *buffers);
void glBindBuffersRange(GLenum[Main] target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
void glBindTextures(GLuint first, GLsizei count, const GLuint *textures);
void glBindSamplers(GLuint first, GLsizei count, const GLuint *samplers);
void glBindImageTextures(GLuint first, GLsizei count, const GLuint *textures);
void glBindVertexBuffers(GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
