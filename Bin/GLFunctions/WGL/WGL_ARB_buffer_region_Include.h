#define GLI_INCLUDE_WGL_ARB_BUFFER_REGION


enum Mask_WGLCreateBuffer {

  WGL_FRONT_COLOR_BUFFER_BIT_ARB	   =   0x00000001,
  WGL_BACK_COLOR_BUFFER_BIT_ARB	     =   0x00000002,
  WGL_DEPTH_BUFFER_BIT_ARB		       =   0x00000004,
  WGL_STENCIL_BUFFER_BIT_ARB	       =   0x00000008,
};


//Not sure about the "HANDLE" structure (will void* do?)
void * wglCreateBufferRegionARB(void * hDC, GLint iLayerPlane, GLbitfield[Mask_WGLCreateBuffer] uType);

void wglDeleteBufferRegionARB(void * hRegion);

GLboolean wglSaveBufferRegionARB(void * hRegion,GLint x, GLint y,GLint width,GLint height);

GLboolean wglRestoreBufferRegionARB(void * hRegion,GLint x,GLint y,GLint width, GLint height,GLint xSrc,GLint ySrc);


