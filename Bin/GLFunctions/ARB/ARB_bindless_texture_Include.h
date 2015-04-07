#define GLI_INCLUDE_GL_ARB_BINDLESS_TEXTURE

enum Main {

  GL_UNSIGNED_INT64_ARB       = 0x140F,

};

GLuint64 glGetTextureHandleARB(GLuint texture);
GLuint64 glGetTextureSamplerHandleARB(GLuint texture, GLuint sampler);
void glMakeTextureHandleResidentARB(GLuint64 handle);
void glMakeTextureHandleNonResidentARB(GLuint64 handle);
GLuint64 glGetImageHandleARB(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum[Main] format);
void glMakeImageHandleResidentARB(GLuint64 handle, GLenum[Main] access);
void glMakeImageHandleNonResidentARB(GLuint64 handle);
void glUniformHandleui64ARB(GLint location, GLuint64 value);
void glUniformHandleui64vARB(GLint location, GLsizei count, const GLuint64 *value);
void glProgramUniformHandleui64ARB(GLuint program, GLint location, GLuint64 value);
void glProgramUniformHandleui64vARB(GLuint program, GLint location, GLsizei count, const GLuint64 *values);
GLboolean glIsTextureHandleResidentARB(GLuint64 handle);
GLboolean glIsImageHandleResidentARB(GLuint64 handle);
void glVertexAttribL1ui64ARB(GLuint index, GLuint64 x);
void glVertexAttribL1ui64vARB(GLuint index, const GLuint64 *v);
void glGetVertexAttribLui64vARB(GLuint index, GLenum[Main] pname, GLuint64 *params);
