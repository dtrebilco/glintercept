#define GLI_INCLUDE_GL_EXT_MULTI_DRAW_ARRAYS

void glMultiDrawArraysEXT(GLenum[Primitives] mode, const GLint * first, const GLsizei * count, GLsizei primcount);
void glMultiDrawElementsEXT(GLenum[Primitives] mode, const GLsizei * count, GLenum[Main] type, const GLvoid **indices, GLsizei primcount);
