#define GLI_INCLUDE_GL_SGIX_LIST_PRIORITY

enum Main {

  GL_LIST_PRIORITY_SGIX       = 0x8182,

};

void glGetListParameterfvSGIX(GLuint list, GLenum[Main] pname, GLfloat * params);
void glGetListParameterivSGIX(GLuint list, GLenum[Main] pname, GLint * params);
void glListParameterfSGIX(GLuint list, GLenum[Main] pname, GLfloat param);
void glListParameterfvSGIX(GLuint list, GLenum[Main] pname, const GLfloat * params);
void glListParameteriSGIX(GLuint list, GLenum[Main] pname, GLint param);
void glListParameterivSGIX(GLuint list, GLenum[Main] pname, const GLint * params);
