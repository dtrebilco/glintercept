#define GLI_INCLUDE_GL_EXT_SHADER_PIXEL_LOCAL_STORAGE2

enum Main {

  GL_MAX_SHADER_COMBINED_LOCAL_STORAGE_FAST_SIZE_EXT                             = 0x9650,
  GL_MAX_SHADER_COMBINED_LOCAL_STORAGE_SIZE_EXT                                  = 0x9651,
  GL_FRAMEBUFFER_INCOMPLETE_INSUFFICIENT_SHADER_COMBINED_LOCAL_STORAGE_EXT       = 0x9652,

};

void glFramebufferPixelLocalStorageSizeEXT(GLuint target, GLsizei size);
GLsizei glGetFramebufferPixelLocalStorageSizeEXT(GLuint target);
void glClearPixelLocalStorageuiEXT(GLsizei offset, GLsizei n, const GLuint *values);
