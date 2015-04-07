#define GLI_INCLUDE_WGL_ARB_PBUFFER

enum Main {

  WGL_DRAW_TO_PBUFFER_ARB          = 0x202D,
  WGL_MAX_PBUFFER_PIXELS_ARB       = 0x202E,
  WGL_MAX_PBUFFER_WIDTH_ARB        = 0x202F,
  WGL_MAX_PBUFFER_HEIGHT_ARB       = 0x2030,
  WGL_PBUFFER_LARGEST_ARB          = 0x2033,
  WGL_PBUFFER_WIDTH_ARB            = 0x2034,
  WGL_PBUFFER_HEIGHT_ARB           = 0x2035,
  WGL_PBUFFER_LOST_ARB             = 0x2036,

};

void * wglCreatePbufferARB(void * hDC, GLint iPixelFormat, GLint iWidth, GLint iHeight, const GLint *piAttribList);

void * wglGetPbufferDCARB(void * hPbuffer);

GLint wglReleasePbufferDCARB(void * hPbuffer,void * hDC);

GLboolean wglDestroyPbufferARB(void * hPbuffer);

GLboolean wglQueryPbufferARB(void *hPbuffer, GLint iAttribute, GLint *piValue);


//EXT version
void * wglCreatePbufferEXT(void * hDC, GLint iPixelFormat, GLint iWidth, GLint iHeight, const GLint *piAttribList);

void * wglGetPbufferDCEXT(void * hPbuffer);

GLint wglReleasePbufferDCEXT(void * hPbuffer, void * hDC);

GLboolean wglDestroyPbufferEXT(void * hPbuffer);

GLboolean wglQueryPbufferEXT(void * hPbuffer, GLint iAttribute, GLint *piValue);
