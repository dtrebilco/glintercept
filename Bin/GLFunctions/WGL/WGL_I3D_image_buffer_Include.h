#define GLI_INCLUDE_WGL_I3D_IMAGE_BUFFER

enum Main {

  //WGL_IMAGE_BUFFER_MIN_ACCESS_I3D       = 0x00000001,
  //WGL_IMAGE_BUFFER_LOCK_I3D             = 0x00000002,

};

void* wglCreateImageBufferI3D(void* hDC, GLint dwSize, GLuint uFlags);
GLboolean wglDestroyImageBufferI3D(void* hDC, void*  pAddress);
GLboolean wglAssociateImageBufferEventsI3D(void* hDC, const void** pEvent, const void* * pAddress, const GLint * pSize, GLuint count);
GLboolean wglReleaseImageBufferEventsI3D(void* hDC, const void* * pAddress, GLuint count);
