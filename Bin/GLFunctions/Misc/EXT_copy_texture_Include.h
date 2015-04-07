#define GLI_INCLUDE_GL_EXT_COPY_TEXTURE

void glCopyTexImage1DEXT(GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width, GLint border);
void glCopyTexImage2DEXT(GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void glCopyTexSubImage1DEXT(GLenum[Main] target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void glCopyTexSubImage2DEXT(GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
//void glCopyTexSubImage3DEXT(GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

