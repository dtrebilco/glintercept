#define GLI_INCLUDE_WGL_I3D_DIGITAL_VIDEO_CONTROL

enum Main {

  WGL_DIGITAL_VIDEO_CURSOR_ALPHA_FRAMEBUFFER_I3D       = 0x2050,
  WGL_DIGITAL_VIDEO_CURSOR_ALPHA_VALUE_I3D             = 0x2051,
  WGL_DIGITAL_VIDEO_CURSOR_INCLUDED_I3D                = 0x2052,
  WGL_DIGITAL_VIDEO_GAMMA_CORRECTED_I3D                = 0x2053,

};

GLboolean wglGetDigitalVideoParametersI3D(void* hDC, GLint iAttribute, GLint * piValue);
GLboolean wglSetDigitalVideoParametersI3D(void* hDC, GLint iAttribute, const GLint * piValue);
