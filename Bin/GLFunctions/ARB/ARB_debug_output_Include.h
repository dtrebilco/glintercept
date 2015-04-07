#define GLI_INCLUDE_GL_ARB_DEBUG_OUTPUT

enum Main {

  //GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB               = 0x8242,
  //GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB       = 0x8243,
  //GL_DEBUG_CALLBACK_FUNCTION_ARB                = 0x8244,
  //GL_DEBUG_CALLBACK_USER_PARAM_ARB              = 0x8245,
  //GL_DEBUG_SOURCE_API_ARB                       = 0x8246,
  //GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB             = 0x8247,
  //GL_DEBUG_SOURCE_SHADER_COMPILER_ARB           = 0x8248,
  //GL_DEBUG_SOURCE_THIRD_PARTY_ARB               = 0x8249,
  //GL_DEBUG_SOURCE_APPLICATION_ARB               = 0x824A,
  //GL_DEBUG_SOURCE_OTHER_ARB                     = 0x824B,
  //GL_DEBUG_TYPE_ERROR_ARB                       = 0x824C,
  //GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB         = 0x824D,
  //GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB          = 0x824E,
  //GL_DEBUG_TYPE_PORTABILITY_ARB                 = 0x824F,
  //GL_DEBUG_TYPE_PERFORMANCE_ARB                 = 0x8250,
  //GL_DEBUG_TYPE_OTHER_ARB                       = 0x8251,
  //GL_MAX_DEBUG_MESSAGE_LENGTH_ARB               = 0x9143,
  //GL_MAX_DEBUG_LOGGED_MESSAGES_ARB              = 0x9144,
  //GL_DEBUG_LOGGED_MESSAGES_ARB                  = 0x9145,
  //GL_DEBUG_SEVERITY_HIGH_ARB                    = 0x9146,
  //GL_DEBUG_SEVERITY_MEDIUM_ARB                  = 0x9147,
  //GL_DEBUG_SEVERITY_LOW_ARB                     = 0x9148,

};

void glDebugMessageControlARB(GLenum[Main] source, GLenum[Main] type, GLenum[Main] severity, GLsizei count, const GLuint * ids, GLboolean enabled);
void glDebugMessageInsertARB(GLenum[Main] source, GLenum[Main] type, GLuint id, GLenum[Main] severity, GLsizei length, const GLchar * buf);
//void glDebugMessageCallbackARB(GLDEBUGPROCARB callback, GLvoid* userParam);
void glDebugMessageCallbackARB(GLvoid* callback, GLvoid* userParam);
GLuint glGetDebugMessageLogARB(GLuint count, GLsizei bufsize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog);
