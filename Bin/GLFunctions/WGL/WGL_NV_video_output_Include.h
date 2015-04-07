#define GLI_INCLUDE_WGL_NV_VIDEO_OUTPUT

enum Main {

  WGL_BIND_TO_VIDEO_RGB_NV                 = 0x20C0,
  WGL_BIND_TO_VIDEO_RGBA_NV                = 0x20C1,
  WGL_BIND_TO_VIDEO_RGB_AND_DEPTH_NV       = 0x20C2,
  WGL_VIDEO_OUT_COLOR_NV                   = 0x20C3,
  WGL_VIDEO_OUT_ALPHA_NV                   = 0x20C4,
  WGL_VIDEO_OUT_DEPTH_NV                   = 0x20C5,
  WGL_VIDEO_OUT_COLOR_AND_ALPHA_NV         = 0x20C6,
  WGL_VIDEO_OUT_COLOR_AND_DEPTH_NV         = 0x20C7,
  WGL_VIDEO_OUT_FRAME                      = 0x20C8,
  WGL_VIDEO_OUT_FIELD_1                    = 0x20C9,
  WGL_VIDEO_OUT_FIELD_2                    = 0x20CA,
  WGL_VIDEO_OUT_STACKED_FIELDS_1_2         = 0x20CB,
  WGL_VIDEO_OUT_STACKED_FIELDS_2_1         = 0x20CC,

};

GLboolean wglGetVideoDeviceNV(void* hDC, GLint numDevices, void* * hVideoDevice);
GLboolean wglReleaseVideoDeviceNV(void* hVideoDevice);
GLboolean wglBindVideoImageNV(void* hVideoDevice, void* hPbuffer, GLint iVideoBuffer);
GLboolean wglReleaseVideoImageNV(void* hPbuffer, GLint iVideoBuffer);
GLboolean wglSendPbufferToVideoNV(void* hPbuffer, GLint iBufferType, GLuint * pulCounterPbuffer, GLboolean bBlock);
GLboolean wglGetVideoInfoNV(void* hpVideoDevice, GLuint * pulCounterOutputPbuffer, GLuint * pulCounterOutputVideo);
