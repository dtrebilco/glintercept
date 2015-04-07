#define GLI_INCLUDE_GL_AMD_DEBUG_OUTPUT

enum Main {

  //GL_MAX_DEBUG_MESSAGE_LENGTH_AMD                = 0x9143,
  //GL_MAX_DEBUG_LOGGED_MESSAGES_AMD               = 0x9144,
  //GL_DEBUG_LOGGED_MESSAGES_AMD                   = 0x9145,
  //GL_DEBUG_SEVERITY_HIGH_AMD                     = 0x9146,
  //GL_DEBUG_SEVERITY_MEDIUM_AMD                   = 0x9147,
  //GL_DEBUG_SEVERITY_LOW_AMD                      = 0x9148,
  GL_DEBUG_CATEGORY_API_ERROR_AMD                = 0x9149,
  GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD            = 0x914A,
  GL_DEBUG_CATEGORY_DEPRECATION_AMD              = 0x914B,
  GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD       = 0x914C,
  GL_DEBUG_CATEGORY_PERFORMANCE_AMD              = 0x914D,
  GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD          = 0x914E,
  GL_DEBUG_CATEGORY_APPLICATION_AMD              = 0x914F,
  GL_DEBUG_CATEGORY_OTHER_AMD                    = 0x9150,

};

void glDebugMessageEnableAMD(GLenum[Main] category, GLenum[Main] severity, GLsizei count, const GLuint * ids, GLboolean enabled);
void glDebugMessageInsertAMD(GLenum[Main] category, GLenum[Main] severity, GLuint id, GLsizei length, const GLchar * buf);
//void glDebugMessageCallbackAMD(GLDEBUGPROCAMD callback, GLvoid * userParam);
void glDebugMessageCallbackAMD(GLvoid * callback, GLvoid * userParam);
GLuint glGetDebugMessageLogAMD(GLuint count, GLsizei bufsize, GLenum * categories, GLuint * severities, GLuint * ids, GLsizei * lengths, GLchar * message);
