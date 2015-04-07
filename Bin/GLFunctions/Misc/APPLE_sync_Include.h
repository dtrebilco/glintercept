#define GLI_INCLUDE_GL_APPLE_SYNC

enum Main {

  GL_SYNC_OBJECT_APPLE                      = 0x8A53,
  //GL_MAX_SERVER_WAIT_TIMEOUT_APPLE          = 0x9111,
  //GL_OBJECT_TYPE_APPLE                      = 0x9112,
  //GL_SYNC_CONDITION_APPLE                   = 0x9113,
  //GL_SYNC_STATUS_APPLE                      = 0x9114,
  //GL_SYNC_FLAGS_APPLE                       = 0x9115,
  //GL_SYNC_FENCE_APPLE                       = 0x9116,
  //GL_SYNC_GPU_COMMANDS_COMPLETE_APPLE       = 0x9117,
  //GL_UNSIGNALED_APPLE                       = 0x9118,
  //GL_SIGNALED_APPLE                         = 0x9119,
  //GL_ALREADY_SIGNALED_APPLE                 = 0x911A,
  //GL_TIMEOUT_EXPIRED_APPLE                  = 0x911B,
  //GL_CONDITION_SATISFIED_APPLE              = 0x911C,
  //GL_WAIT_FAILED_APPLE                      = 0x911D,
  //GL_SYNC_FLUSH_COMMANDS_BIT_APPLE          = 0x00000001,
  //GL_TIMEOUT_IGNORED_APPLE                  = 0xFFFFFFFFFFFFFFFF,

};

GLsync glFenceSyncAPPLE(GLenum[Main] condition, GLbitfield flags);
GLboolean glIsSyncAPPLE(GLsync sync);
void glDeleteSyncAPPLE(GLsync sync);
GLenum[Main] glClientWaitSyncAPPLE(GLsync sync, GLbitfield flags, GLuint64 timeout);
void glWaitSyncAPPLE(GLsync sync, GLbitfield flags, GLuint64 timeout);
void glGetInteger64vAPPLE(GLenum[Main] pname, GLint64 *params);
void glGetSyncivAPPLE(GLsync sync, GLenum[Main] pname, GLsizei bufSize, GLsizei *length, GLint *values);
