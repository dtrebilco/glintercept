#define GLI_INCLUDE_GL_INTEL_MAP_TEXTURE

enum Main {

  GL_TEXTURE_MEMORY_LAYOUT_INTEL          = 0x83FF,
  //GL_LAYOUT_DEFAULT_INTEL                 = 0,
  //GL_LAYOUT_LINEAR_INTEL                  = 1,
//  GL_LAYOUT_LINEAR_CPU_CACHED_INTEL       = 2,

};

void glSyncTextureINTEL(GLuint texture);
void glUnmapTexture2DINTEL(GLuint texture, GLint level);
void *glMapTexture2DINTEL(GLuint texture, GLint level, GLbitfield access, GLint *stride, GLenum[Main] *layout);
