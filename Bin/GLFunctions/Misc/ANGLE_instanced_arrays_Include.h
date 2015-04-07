#define GLI_INCLUDE_GL_ANGLE_INSTANCED_ARRAYS

enum Main {

  //GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE       = 0x88FE,

};

void glDrawArraysInstancedANGLE(GLenum[Main] mode, GLint first, GLsizei count, GLsizei primcount);
void glDrawElementsInstancedANGLE(GLenum[Main] mode, GLsizei count, GLenum[Main] type, const void *indices, GLsizei primcount);
void glVertexAttribDivisorANGLE(GLuint index, GLuint divisor);
