#define GLI_INCLUDE_GL_OES_VERTEX_ARRAY_OBJECT

enum Main {

  //GL_VERTEX_ARRAY_BINDING_OES       = 0x85B5,

};

void glBindVertexArrayOES(GLuint array);
void glDeleteVertexArraysOES(GLsizei n, const GLuint *arrays);
void glGenVertexArraysOES(GLsizei n, GLuint *arrays);
GLboolean glIsVertexArrayOES(GLuint array);
