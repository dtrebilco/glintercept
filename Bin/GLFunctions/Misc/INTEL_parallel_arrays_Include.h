#define GLI_INCLUDE_GL_INTEL_PARALLEL_ARRAYS

enum Main {

  GL_PARALLEL_ARRAYS_INTEL                             = 0x83F4,
  GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL              = 0x83F5,
  GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL              = 0x83F6,
  GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL               = 0x83F7,
  GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL       = 0x83F8,

};

void glVertexPointervINTEL(GLint size, GLenum[Main] type, const GLvoid* * pointer);
void glNormalPointervINTEL(GLenum[Main] type, const GLvoid* * pointer);
void glColorPointervINTEL(GLint size, GLenum[Main] type, const GLvoid* * pointer);
void glTexCoordPointervINTEL(GLint size, GLenum[Main] type, const GLvoid* * pointer);
