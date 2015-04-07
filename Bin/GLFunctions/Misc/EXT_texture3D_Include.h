#define GLI_INCLUDE_EXT_TEXTURE3D

void glTexImage3DEXT ( GLenum[Main] target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum[Main] format, GLenum[Main] type, const GLvoid *pixels );

void glTexSubImage3DEXT (GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLenum[Main] type, const GLvoid *pixels);

void glCopyTexSubImage3DEXT( GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
