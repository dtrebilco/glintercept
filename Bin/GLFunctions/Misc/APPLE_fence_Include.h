#define GLI_INCLUDE_GL_APPLE_FENCE

enum Main {

  GL_DRAW_PIXELS_APPLE       = 0x8A0A,
  GL_FENCE_APPLE             = 0x8A0B,

};

void glGenFencesAPPLE(GLsizei n, GLuint * fences);
void glDeleteFencesAPPLE(GLsizei n, const GLuint * fences);
void glSetFenceAPPLE(GLuint fence);
GLboolean glIsFenceAPPLE(GLuint fence);
GLboolean glTestFenceAPPLE(GLuint fence);
void glFinishFenceAPPLE(GLuint fence);
GLboolean glTestObjectAPPLE(GLenum[Main] object, GLuint name);
void glFinishObjectAPPLE(GLenum[Main] object, GLint name);
