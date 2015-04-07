#define GLI_INCLUDE_WGL_NV_DX_INTEROP

enum Main {

  //WGL_ACCESS_READ_ONLY_NV           = 0x00000000,
  //WGL_ACCESS_READ_WRITE_NV          = 0x00000001,
  //WGL_ACCESS_WRITE_DISCARD_NV       = 0x00000002,

};

GLboolean wglDXSetResourceShareHandleNV(void * dxObject, void* shareHandle);
void* wglDXOpenDeviceNV(void * dxDevice);
GLboolean wglDXCloseDeviceNV(void* hDevice);
void* wglDXRegisterObjectNV(void* hDevice, void * dxObject, GLuint name, GLenum[Main] type, GLenum[Main] access);
GLboolean wglDXUnregisterObjectNV(void* hDevice, void* hObject);
GLboolean wglDXObjectAccessNV(void* hObject, GLenum[Main] access);
GLboolean wglDXLockObjectsNV(void* hDevice, GLint count, void* * hObjects);
GLboolean wglDXUnlockObjectsNV(void* hDevice, GLint count, void* * hObjects);
