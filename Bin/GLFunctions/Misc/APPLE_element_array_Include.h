#define GLI_INCLUDE_GL_APPLE_ELEMENT_ARRAY

enum Main {

  GL_ELEMENT_ARRAY_APPLE               = 0x8A0C,
  GL_ELEMENT_ARRAY_TYPE_APPLE          = 0x8A0D,
  GL_ELEMENT_ARRAY_POINTER_APPLE       = 0x8A0E,

};

void glElementPointerAPPLE(GLenum[Main] type, const GLvoid * pointer);
void glDrawElementArrayAPPLE(GLenum[Primitives] mode, GLint first, GLsizei count);
void glDrawRangeElementArrayAPPLE(GLenum[Primitives] mode, GLuint start, GLuint end, GLint first, GLsizei count);
void glMultiDrawElementArrayAPPLE(GLenum[Primitives] mode, const GLint * first, const GLsizei * count, GLsizei primcount);
void glMultiDrawRangeElementArrayAPPLE(GLenum[Primitives] mode, GLuint start, GLuint end, const GLint * first, const GLsizei * count, GLsizei primcount);
