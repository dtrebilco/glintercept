#define GLI_INCLUDE_GL_NV_VIEWPORT_ARRAY

enum Main {

  //GL_MAX_VIEWPORTS_NV                         = 0x825B,
  //GL_VIEWPORT_SUBPIXEL_BITS_NV                = 0x825C,
  //GL_VIEWPORT_BOUNDS_RANGE_NV                 = 0x825D,
  //GL_VIEWPORT_INDEX_PROVOKING_VERTEX_NV       = 0x825F,

};

void glViewportArrayvNV(GLuint first, GLsizei count, const GLfloat *v);
void glViewportIndexedfNV(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
void glViewportIndexedfvNV(GLuint index, const GLfloat *v);
void glScissorArrayvNV(GLuint first, GLsizei count, const GLint *v);
void glScissorIndexedNV(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
void glScissorIndexedvNV(GLuint index, const GLint *v);
void glDepthRangeArrayfvNV(GLuint first, GLsizei count, const GLfloat *v);
void glDepthRangeIndexedfNV(GLuint index, GLfloat n, GLfloat f);
void glGetFloati_vNV(GLenum[Main] target, GLuint index, GLfloat *data);
void glEnableiNV(GLenum[Main] target, GLuint index);
void glDisableiNV(GLenum[Main] target, GLuint index);
GLboolean glIsEnablediNV(GLenum[Main] target, GLuint index);
