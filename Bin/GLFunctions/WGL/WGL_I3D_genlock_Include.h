#define GLI_INCLUDE_WGL_I3D_GENLOCK

enum Main {

  WGL_GENLOCK_SOURCE_MULTIVIEW_I3D           = 0x2044,
  WGL_GENLOCK_SOURCE_EXTERNAL_SYNC_I3D       = 0x2045,
  WGL_GENLOCK_SOURCE_EXTERNAL_FIELD_I3D      = 0x2046,
  WGL_GENLOCK_SOURCE_EXTERNAL_TTL_I3D        = 0x2047,
  WGL_GENLOCK_SOURCE_DIGITAL_SYNC_I3D        = 0x2048,
  WGL_GENLOCK_SOURCE_DIGITAL_FIELD_I3D       = 0x2049,
  WGL_GENLOCK_SOURCE_EDGE_FALLING_I3D        = 0x204A,
  WGL_GENLOCK_SOURCE_EDGE_RISING_I3D         = 0x204B,
  WGL_GENLOCK_SOURCE_EDGE_BOTH_I3D           = 0x204C,

};

GLboolean wglEnableGenlockI3D(void* hDC);
GLboolean wglDisableGenlockI3D(void* hDC);
GLboolean wglIsEnabledGenlockI3D(void* hDC, GLboolean * pFlag);
GLboolean wglGenlockSourceI3D(void* hDC, GLuint uSource);
GLboolean wglGetGenlockSourceI3D(void* hDC, GLuint * uSource);
GLboolean wglGenlockSourceEdgeI3D(void* hDC, GLuint uEdge);
GLboolean wglGetGenlockSourceEdgeI3D(void* hDC, GLuint * uEdge);
GLboolean wglGenlockSampleRateI3D(void* hDC, GLuint uRate);
GLboolean wglGetGenlockSampleRateI3D(void* hDC, GLuint * uRate);
GLboolean wglGenlockSourceDelayI3D(void* hDC, GLuint uDelay);
GLboolean wglGetGenlockSourceDelayI3D(void* hDC, GLuint * uDelay);
GLboolean wglQueryGenlockMaxSourceDelayI3D(void* hDC, GLuint * uMaxLineDelay, GLuint * uMaxPixelDelay);
