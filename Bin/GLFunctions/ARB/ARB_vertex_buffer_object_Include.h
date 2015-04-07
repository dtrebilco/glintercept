#define GLI_INCLUDE_ARB_VERTEX_BUFFER_OBJECT


void glBindBufferARB (GLenum[Main] target, GLuint buffer);

void glDeleteBuffersARB (GLsizei n, const GLuint *buffers);

void glGenBuffersARB (GLsizei n, GLuint *buffers);

GLboolean glIsBufferARB (GLuint buffer);

void glBufferDataARB (GLenum[Main] target, GLsizeiptr size, const GLvoid *data, GLenum[Main] usage);
void glBufferSubDataARB (GLenum[Main] target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
void glGetBufferSubDataARB (GLenum[Main] target, GLintptr offset, GLsizeiptr size, GLvoid *data);

GLvoid* glMapBufferARB (GLenum[Main] target, GLenum[Main] access);

GLboolean glUnmapBufferARB (GLenum[Main] target);

void glGetBufferParameterivARB (GLenum[Main] target, GLenum[Main] pname, GLint *params);

void glGetBufferPointervARB (GLenum[Main] target, GLenum[Main] pname, GLvoid **params);
