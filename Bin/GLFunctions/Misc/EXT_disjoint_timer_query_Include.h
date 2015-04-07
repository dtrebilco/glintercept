#define GLI_INCLUDE_GL_EXT_DISJOINT_TIMER_QUERY

enum Main {

  //GL_QUERY_COUNTER_BITS_EXT           = 0x8864,
  //GL_CURRENT_QUERY_EXT                = 0x8865,
  //GL_QUERY_RESULT_EXT                 = 0x8866,
  //GL_QUERY_RESULT_AVAILABLE_EXT       = 0x8867,
  //GL_TIME_ELAPSED_EXT                 = 0x88BF,
  //GL_TIMESTAMP_EXT                    = 0x8E28,
  GL_GPU_DISJOINT_EXT                 = 0x8FBB,

};

void glGenQueriesEXT(GLsizei n, GLuint *ids);
void glDeleteQueriesEXT(GLsizei n, const GLuint *ids);
GLboolean glIsQueryEXT(GLuint id);
void glBeginQueryEXT(GLenum[Main] target, GLuint id);
void glEndQueryEXT(GLenum[Main] target);
void glQueryCounterEXT(GLuint id, GLenum[Main] target);
void glGetQueryivEXT(GLenum[Main] target, GLenum[Main] pname, GLint *params);
void glGetQueryObjectivEXT(GLuint id, GLenum[Main] pname, GLint *params);
void glGetQueryObjectuivEXT(GLuint id, GLenum[Main] pname, GLuint *params);
//void glGetQueryObjecti64vEXT(GLuint id, GLenum[Main] pname, GLint64 *params);
//void glGetQueryObjectui64vEXT(GLuint id, GLenum[Main] pname, GLuint64 *params);
