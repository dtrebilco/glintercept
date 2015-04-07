#define GLI_INCLUDE_GL_APPLE_VERTEX_ARRAY_OBJECT

enum Main {

  //GL_VERTEX_ARRAY_BINDING_APPLE       = 0x85B5,

};

void glBindVertexArrayAPPLE(GLuint array);
void glDeleteVertexArraysAPPLE(GLsizei n, const GLuint * arrays);
void glGenVertexArraysAPPLE(GLsizei n, GLuint * arrays);
GLboolean glIsVertexArrayAPPLE(GLuint array);
