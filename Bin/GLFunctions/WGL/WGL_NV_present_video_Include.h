#define GLI_INCLUDE_WGL_NV_PRESENT_VIDEO

enum Main {

  WGL_NUM_VIDEO_SLOTS_NV       = 0x20F0,

};

GLint wglEnumerateVideoDevicesNV(void* hDC, void* * phDeviceList);
GLboolean wglBindVideoDeviceNV(void* hDC, GLuint uVideoSlot, void* hVideoDevice, const GLint * piAttribList);
GLboolean wglQueryCurrentContextNV(GLint iAttribute, GLint * piValue);
