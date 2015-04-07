#define GLI_INCLUDE_GL_EXT_X11_SYNC_OBJECT

enum Main {

  GL_SYNC_X11_FENCE_EXT       = 0x90E1,

};

GLsync glImportSyncEXT(GLenum[Main] external_sync_type, GLintptr external_sync, GLbitfield flags);
