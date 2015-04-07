#define GLI_INCLUDE_GL_SGIX_ASYNC

enum Main {

  GL_ASYNC_MARKER_SGIX       = 0x8329,

};

void glAsyncMarkerSGIX(GLuint marker);
GLint glFinishAsyncSGIX(GLuint * markerp);
GLint glPollAsyncSGIX(GLuint * markerp);
GLuint glGenAsyncMarkersSGIX(GLsizei range);
void glDeleteAsyncMarkersSGIX(GLuint marker, GLsizei range);
GLboolean glIsAsyncMarkerSGIX(GLuint marker);
