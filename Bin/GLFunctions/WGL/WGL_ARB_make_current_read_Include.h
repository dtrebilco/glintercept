#define GLI_INCLUDE_WGL_ARB_MAKE_CURRENT_READ

GLboolean wglMakeContextCurrentARB(void* hDrawDC, void* hReadDC,void * hglrc);

void * wglGetCurrentReadDCARB(void);


//Add EXT version
GLboolean wglMakeContextCurrentEXT(void * hDrawDC, void * hReadDC, void * hglrc);

void * wglGetCurrentReadDCEXT(void);
