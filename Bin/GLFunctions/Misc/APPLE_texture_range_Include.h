#define GLI_INCLUDE_GL_APPLE_TEXTURE_RANGE

enum Main {

  GL_TEXTURE_RANGE_LENGTH_APPLE        = 0x85B7,
  GL_TEXTURE_RANGE_POINTER_APPLE       = 0x85B8,
  GL_TEXTURE_STORAGE_HINT_APPLE        = 0x85BC,
  GL_STORAGE_PRIVATE_APPLE             = 0x85BD,
  GL_STORAGE_CACHED_APPLE              = 0x85BE,
  GL_STORAGE_SHARED_APPLE              = 0x85BF,
};  

void glTextureRangeAPPLE(GLenum[Main] target, GLsizei length, const GLvoid * pointer);
void glGetTexParameterPointervAPPLE(GLenum[Main] target, GLenum[Main] pname, GLvoid* * params);
