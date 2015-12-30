#define GLI_INCLUDE_GL_EXT_BLEND_FUNC_EXTENDED

enum Main {

  //GL_SRC1_COLOR_EXT                         = 0x88F9,
  //GL_SRC1_ALPHA_EXT                         = 0x8589,
  //GL_ONE_MINUS_SRC1_COLOR_EXT               = 0x88FA,
  //GL_ONE_MINUS_SRC1_ALPHA_EXT               = 0x88FB,
  //GL_SRC_ALPHA_SATURATE_EXT                 = 0x0308,
  //GL_LOCATION_INDEX_EXT                     = 0x930F,
  //GL_MAX_DUAL_SOURCE_DRAW_BUFFERS_EXT       = 0x88FC,

};

void glBindFragDataLocationIndexedEXT(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
//void glBindFragDataLocationEXT(GLuint program, GLuint color, const GLchar *name);
GLint glGetProgramResourceLocationIndexEXT(GLuint program, GLenum[Main] programInterface, const GLchar *name);
GLint glGetFragDataIndexEXT(GLuint program, const GLchar *name);
