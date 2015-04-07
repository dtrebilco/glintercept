#define GLI_INCLUDE_GL_EXT_MULTI_DRAW_INDIRECT

void glMultiDrawArraysIndirectEXT(GLenum[Main] mode, const void *indirect, GLsizei drawcount, GLsizei stride);
void glMultiDrawElementsIndirectEXT(GLenum[Main] mode, GLenum[Main] type, const void *indirect, GLsizei drawcount, GLsizei stride);
