#define GLI_INCLUDE_GL_EXT_DEBUG_LABEL

enum Main {

  //GL_PROGRAM_PIPELINE_OBJECT_EXT       = 0x8A4F,
  //GL_PROGRAM_OBJECT_EXT                = 0x8B40,
  //GL_SHADER_OBJECT_EXT                 = 0x8B48,
  //GL_BUFFER_OBJECT_EXT                 = 0x9151,
  //GL_QUERY_OBJECT_EXT                  = 0x9153,
  //GL_VERTEX_ARRAY_OBJECT_EXT           = 0x9154,

};

void glLabelObjectEXT(GLenum[Main] type, GLuint object, GLsizei length, const GLchar *label);
void glGetObjectLabelEXT(GLenum[Main] type, GLuint object, GLsizei bufSize, GLsizei *length, GLchar *label);
