#define GLI_INCLUDE_GL_APPLE_OBJECT_PURGEABLE

enum Main {

  GL_BUFFER_OBJECT_APPLE       = 0x85B3,
  GL_RELEASED_APPLE            = 0x8A19,
  GL_VOLATILE_APPLE            = 0x8A1A,
  GL_RETAINED_APPLE            = 0x8A1B,
  GL_UNDEFINED_APPLE           = 0x8A1C,
  GL_PURGEABLE_APPLE           = 0x8A1D,

};

GLenum glObjectPurgeableAPPLE(GLenum[Main] objectType, GLuint name, GLenum[Main] option);
GLenum glObjectUnpurgeableAPPLE(GLenum[Main] objectType, GLuint name, GLenum[Main] option);
void glGetObjectParameterivAPPLE(GLenum[Main] objectType, GLuint name, GLenum[Main] pname, GLint * params);
