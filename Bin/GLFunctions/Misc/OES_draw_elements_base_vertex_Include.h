#define GLI_INCLUDE_GL_OES_DRAW_ELEMENTS_BASE_VERTEX

void glDrawElementsBaseVertexOES(GLenum[Main] mode, GLsizei count, GLenum[Main] type, const void *indices, GLint basevertex);
void glDrawRangeElementsBaseVertexOES(GLenum[Main] mode, GLuint start, GLuint end, GLsizei count, GLenum[Main] type, const void *indices, GLint basevertex);
void glDrawElementsInstancedBaseVertexOES(GLenum[Main] mode, GLsizei count, GLenum[Main] type, const void *indices, GLsizei instancecount, GLint basevertex);
void glMultiDrawElementsBaseVertexOES(GLenum[Main] mode, const GLsizei *count, GLenum[Main] type, const void *const*indices, GLsizei primcount, const GLint *basevertex);
