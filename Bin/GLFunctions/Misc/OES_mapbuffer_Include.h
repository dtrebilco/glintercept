#define GLI_INCLUDE_GL_OES_MAPBUFFER

enum Main {

  //GL_WRITE_ONLY_OES               = 0x88B9,
  //GL_BUFFER_ACCESS_OES            = 0x88BB,
  //GL_BUFFER_MAPPED_OES            = 0x88BC,
  //GL_BUFFER_MAP_POINTER_OES       = 0x88BD,

};

void *glMapBufferOES(GLenum[Main] target, GLenum[Main] access);
GLboolean glUnmapBufferOES(GLenum[Main] target);
void glGetBufferPointervOES(GLenum[Main] target, GLenum[Main] pname, void **params);
