#define GLI_INCLUDE_GL_AMD_MULTI_DRAW_INDIRECT

void glMultiDrawArraysIndirectAMD(GLenum[Primitives] mode, const GLvoid * indirect, GLsizei primcount, GLsizei stride);
void glMultiDrawElementsIndirectAMD(GLenum[Primitives] mode, GLenum[Main] type, const GLvoid * indirect, GLsizei primcount, GLsizei stride);
