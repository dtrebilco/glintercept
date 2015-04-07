#define GLI_INCLUDE_GL_NV_BINDLESS_MULTI_DRAW_INDIRECT_COUNT

void glMultiDrawArraysIndirectBindlessCountNV(GLenum[Main] mode, const void *indirect, GLsizei drawCount, GLsizei maxDrawCount, GLsizei stride, GLint vertexBufferCount);
void glMultiDrawElementsIndirectBindlessCountNV(GLenum[Main] mode, GLenum[Main] type, const void *indirect, GLsizei drawCount, GLsizei maxDrawCount, GLsizei stride, GLint vertexBufferCount);
