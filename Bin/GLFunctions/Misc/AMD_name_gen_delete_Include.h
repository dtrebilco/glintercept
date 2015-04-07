#define GLI_INCLUDE_GL_AMD_NAME_GEN_DELETE

enum Main {

  GL_DATA_BUFFER_AMD               = 0x9151,
  GL_PERFORMANCE_MONITOR_AMD       = 0x9152,
  GL_QUERY_OBJECT_AMD              = 0x9153,
  GL_VERTEX_ARRAY_OBJECT_AMD       = 0x9154,
  GL_SAMPLER_OBJECT_AMD            = 0x9155,

};

void glGenNamesAMD(GLenum[Main] identifier, GLuint num, GLuint * names);
void glDeleteNamesAMD(GLenum[Main] identifier, GLuint num, const GLuint * names);
GLboolean glIsNameAMD(GLenum[Main] identifier, GLuint name);
