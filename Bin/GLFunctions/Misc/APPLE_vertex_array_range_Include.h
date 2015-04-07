#define GLI_INCLUDE_GL_APPLE_VERTEX_ARRAY_RANGE

enum Main {

  //GL_VERTEX_ARRAY_RANGE_APPLE               = 0x851D,
  //GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE        = 0x851E,
  //GL_VERTEX_ARRAY_STORAGE_HINT_APPLE        = 0x851F,
  //GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_APPLE   = 0x8520,
  //GL_VERTEX_ARRAY_RANGE_POINTER_APPLE       = 0x8521,
  GL_STORAGE_CLIENT_APPLE                   = 0x85B4,
  //GL_STORAGE_CACHED_APPLE                   = 0x85BE,
  //GL_STORAGE_SHARED_APPLE                   = 0x85BF,

};

void glVertexArrayRangeAPPLE(GLsizei length, GLvoid * pointer);
void glFlushVertexArrayRangeAPPLE(GLsizei length, GLvoid * pointer);
void glVertexArrayParameteriAPPLE(GLenum[Main] pname, GLint param);
