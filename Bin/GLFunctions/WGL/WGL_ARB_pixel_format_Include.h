#define GLI_INCLUDE_WGL_ARB_PIXEL_FORMAT


GLboolean wglGetPixelFormatAttribivARB(void * hdc, GLint iPixelFormat, GLint iLayerPlane, GLuint nAttributes, const GLint *piAttributes, GLint *piValues);

GLboolean wglGetPixelFormatAttribfvARB(void * hdc, GLint iPixelFormat, GLint iLayerPlane, GLuint nAttributes, const GLint *piAttributes, GLfloat *pfValues);

GLboolean wglChoosePixelFormatARB(void * hdc, const GLint *piAttribIList, const GLfloat *pfAttribFList, GLuint nMaxFormats, GLint *piFormats, GLuint *nNumFormats);



//EXT version
GLboolean wglGetPixelFormatAttribivEXT(void * hdc, GLint iPixelFormat, GLint iLayerPlane, GLuint nAttributes, GLint *piAttributes, GLint *piValues);

GLboolean wglGetPixelFormatAttribfvEXT(void * hdc, GLint iPixelFormat, GLint iLayerPlane, GLuint nAttributes, GLint *piAttributes, GLfloat *pfValues);

GLboolean wglChoosePixelFormatEXT(void * hdc, const GLint *piAttribIList, const GLfloat *pfAttribFList, GLuint nMaxFormats, GLint *piFormats, GLuint *nNumFormats);
