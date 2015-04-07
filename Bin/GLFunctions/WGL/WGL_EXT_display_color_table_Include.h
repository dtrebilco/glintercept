#define GLI_INCLUDE_WGL_EXT_DISPLAY_COLOR_TABLE

GLboolean wglCreateDisplayColorTableEXT(GLushort id);
GLboolean wglLoadDisplayColorTableEXT(const GLushort * table, GLuint length);
GLboolean wglBindDisplayColorTableEXT(GLushort id);
void wglDestroyDisplayColorTableEXT(GLushort id);
