#define GLI_INCLUDE_GL_EXT_DRAW_ELEMENTS_BASE_VERTEX

void glDrawElementsBaseVertexEXT(GLenum[Main] mode, GLsizei count, GLenum[Main] type, const void *indices, GLint basevertex);
void glDrawRangeElementsBaseVertexEXT(GLenum[Main] mode, GLuint start, GLuint end, GLsizei count, GLenum[Main] type, const void *indices, GLint basevertex);
void glDrawElementsInstancedBaseVertexEXT(GLenum[Main] mode, GLsizei count, GLenum[Main] type, const void *indices, GLsizei instancecount, GLint basevertex);
void glMultiDrawElementsBaseVertexEXT(GLenum[Main] mode, const GLsizei *count, GLenum[Main] type, const void *const*indices, GLsizei primcount, const GLint *basevertex);
