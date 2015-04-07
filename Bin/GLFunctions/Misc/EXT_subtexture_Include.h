#define GLI_INCLUDE_GL_EXT_SUBTEXTURE

void glTexSubImage1DEXT(GLenum[Main] target, GLint level, GLint xoffset, GLsizei width, GLenum[Main] format, GLenum[Main] type, const GLvoid * pixels);
void glTexSubImage2DEXT(GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, const GLvoid * pixels);
