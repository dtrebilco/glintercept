#define GLI_INCLUDE_GL_AMD_INTERLEAVED_ELEMENTS

enum Main {

  GL_VERTEX_ELEMENT_SWIZZLE_AMD       = 0x91A4,
  GL_VERTEX_ID_SWIZZLE_AMD            = 0x91A5,

};

void glVertexAttribParameteriAMD(GLuint index, GLenum[Main] pname, GLint param);
