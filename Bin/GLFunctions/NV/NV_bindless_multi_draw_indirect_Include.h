#define GLI_INCLUDE_GL_NV_BINDLESS_MULTI_DRAW_INDIRECT

void glMultiDrawArraysIndirectBindlessNV(GLenum[Main] mode, const void *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount);
void glMultiDrawElementsIndirectBindlessNV(GLenum[Main] mode, GLenum[Main] type, const void *indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount);
