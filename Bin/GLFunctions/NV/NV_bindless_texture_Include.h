#define GLI_INCLUDE_GL_NV_BINDLESS_TEXTURE


GLuint64 glGetTextureHandleNV(GLuint texture);
GLuint64 glGetTextureSamplerHandleNV(GLuint texture, GLuint sampler);

void glMakeTextureHandleResidentNV(GLuint64 handle);
void glMakeTextureHandleNonResidentNV(GLuint64 handle);

GLuint64 glGetImageHandleNV(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum[Main] format);

void glMakeImageHandleResidentNV(GLuint64 handle, GLenum[Main] access);
void glMakeImageHandleNonResidentNV(GLuint64 handle);

void glUniformHandleui64NV(GLint location, GLuint64 value);
void glUniformHandleui64vNV(GLint location, GLsizei count, const GLuint64 *value);
void glProgramUniformHandleui64NV(GLuint program, GLint location, GLuint64 value);
void glProgramUniformHandleui64vNV(GLuint program, GLint location, GLsizei count, const GLuint64 *values);

GLboolean glIsTextureHandleResidentNV(GLuint64 handle);
GLboolean glIsImageHandleResidentNV(GLuint64 handle);

