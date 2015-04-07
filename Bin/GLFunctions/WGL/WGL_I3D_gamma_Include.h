#define GLI_INCLUDE_WGL_I3D_GAMMA

enum Main {

  WGL_GAMMA_TABLE_SIZE_I3D            = 0x204E,
  WGL_GAMMA_EXCLUDE_DESKTOP_I3D       = 0x204F,

};

GLboolean wglGetGammaTableParametersI3D(void* hDC, GLint iAttribute, GLint * piValue);
GLboolean wglSetGammaTableParametersI3D(void* hDC, GLint iAttribute, const GLint * piValue);
GLboolean wglGetGammaTableI3D(void* hDC, GLint iEntries, GLushort * puRed, GLushort * puGreen, GLushort * puBlue);
GLboolean wglSetGammaTableI3D(void* hDC, GLint iEntries, const GLushort * puRed, const GLushort * puGreen, const GLushort * puBlue);
