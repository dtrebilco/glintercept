#define GLI_INCLUDE_GL_EXT_BASE_INSTANCE

void glDrawArraysInstancedBaseInstanceEXT(GLenum[Main] mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
void glDrawElementsInstancedBaseInstanceEXT(GLenum[Main] mode, GLsizei count, GLenum[Main] type, const void *indices, GLsizei instancecount, GLuint baseinstance);
void glDrawElementsInstancedBaseVertexBaseInstanceEXT(GLenum[Main] mode, GLsizei count, GLenum[Main] type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
