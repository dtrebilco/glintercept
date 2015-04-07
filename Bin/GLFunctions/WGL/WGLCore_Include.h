#define GLI_INCLUDE_WGLCORE

//Attempt to map Windows call parameters to OpenGL parameter types

GLboolean  wglCopyContext           (void * a, void *b, GLuint c);
void *     wglCreateContext         (void * a);
void *     wglCreateLayerContext    (void * a, GLint b);
GLboolean  wglDeleteContext         (void * a);
void *     wglGetCurrentContext     (void);
void *     wglGetCurrentDC          (void);
void *     wglGetDefaultProcAddress (GLubyte * a);
GLint      wglGetPixelFormat        (void * a);
void *     wglGetProcAddress        (GLasciistring * a);
GLboolean  wglMakeCurrent           (void * a, void * b);
GLboolean  wglRealizeLayerPalette   (void * a, GLint b, GLboolean c);
GLboolean  wglShareLists            (void * a, void * b);
GLboolean  wglSwapBuffers           (void * a);
GLboolean  wglSwapLayerBuffers      (void * a, GLuint b);
GLboolean  wglSwapMultipleBuffers   (GLuint a, const void * b);
GLboolean  wglUseFontBitmapsA       (void * a, GLuint b, GLuint c, GLuint d);
GLboolean  wglUseFontBitmapsW       (void * a, GLuint b, GLuint c, GLuint d);


//TODO: The below functions may need review
GLint      wglChoosePixelFormat     (void * a, /*CONST PIXELFORMATDESCRIPTOR*/ void *b);
GLboolean  wglDescribeLayerPlane    (void * a, GLint b, GLint c, GLuint d, /*LPLAYERPLANEDESCRIPTOR*/ void * e);
GLint      wglDescribePixelFormat   (void * a, GLint b, GLuint c, /*LPPIXELFORMATDESCRIPTOR*/ void * d);
GLint      wglGetLayerPaletteEntries(void * a, GLint b, GLint c, GLint d, /*COLORREF*/void *e);
GLint      wglSetLayerPaletteEntries(void * a, GLint b, GLint c, GLint d, /*CONST COLORREF*/void *e);
GLboolean  wglSetPixelFormat        (void * a, GLint b, /*CONST PIXELFORMATDESCRIPTOR*/ void *c);
GLboolean  wglUseFontOutlinesA      (void * a, GLuint b, GLuint c, GLuint d, GLfloat e, GLfloat f, GLint g, /*LPGLYPHMETRICSFLOAT*/ void * h);
GLboolean  wglUseFontOutlinesW      (void * a, GLuint b, GLuint c, GLuint d, GLfloat e, GLfloat f, GLint g, /*LPGLYPHMETRICSFLOAT*/ void * h);
