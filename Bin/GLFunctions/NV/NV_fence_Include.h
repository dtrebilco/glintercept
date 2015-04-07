#define GLI_INCLUDE_NV_FENCE

enum Main {
  
  GL_ALL_COMPLETED_NV             = 0x84F2,
  GL_FENCE_STATUS_NV              = 0x84F3,
  GL_FENCE_CONDITION_NV           = 0x84F4,

};



void glGenFencesNV(GLsizei n, GLuint *fences);

void glDeleteFencesNV(GLsizei n, const GLuint *fences);

void glSetFenceNV(GLuint fence, GLenum[Main] condition);

GLboolean glTestFenceNV(GLuint fence);

void glFinishFenceNV(GLuint fence);

GLboolean glIsFenceNV(GLuint fence);

void glGetFenceivNV(GLuint fence, GLenum[Main] pname, GLint *params);


