#define GLI_INCLUDE_WGL_NV_DELAY_BEFORE_SWAP

//BOOL wglDelayBeforeSwapNV(HDC hDC, GLfloat seconds);
GLboolean wglDelayBeforeSwapNV(void * hDC, GLfloat seconds);