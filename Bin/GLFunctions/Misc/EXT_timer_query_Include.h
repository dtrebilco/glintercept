#define GLI_INCLUDE_EXT_TIMER_QUERY


enum Main {
  
  //GL_TIME_ELAPSED_EXT              = 0x88BF,

};

void glGetQueryObjecti64vEXT (GLuint id, GLenum[Main] pname, GLint64 * params);
void glGetQueryObjectui64vEXT (GLuint id, GLenum[Main] pname, GLuint64 * params);