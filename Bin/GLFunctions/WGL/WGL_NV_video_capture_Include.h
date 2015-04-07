#define GLI_INCLUDE_WGL_NV_VIDEO_CAPTURE

enum Main {

  WGL_UNIQUE_ID_NV                     = 0x20CE,
  WGL_NUM_VIDEO_CAPTURE_SLOTS_NV       = 0x20CF,

};

GLboolean wglBindVideoCaptureDeviceNV(GLuint uVideoSlot, void* hDevice);
GLuint wglEnumerateVideoCaptureDevicesNV(void* hDc, void* * phDeviceList);
GLboolean wglLockVideoCaptureDeviceNV(void* hDc, void* hDevice);
GLboolean wglQueryVideoCaptureDeviceNV(void* hDc, void* hDevice, GLint iAttribute, GLint * piValue);
GLboolean wglReleaseVideoCaptureDeviceNV(void* hDc, void* hDevice);
