#define GLI_INCLUDE_ARB_TEXTURE_COMPRESSION



void glCompressedTexImage1DARB (GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data );

void glCompressedTexImage2DARB (GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );

void glCompressedTexImage3DARB (GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data );

void glCompressedTexSubImage1DARB (GLenum[Main] target, GLint level, GLint xoffset, GLsizei width, GLenum[Main] format, GLsizei imageSize, const GLvoid *data );

void glCompressedTexSubImage2DARB (GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum[Main] format, GLsizei imageSize, const GLvoid *data );

void glCompressedTexSubImage3DARB (GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLsizei imageSize, const GLvoid *data );

void glGetCompressedTexImageARB (GLenum[Main] target, GLint lod, GLvoid *img );





