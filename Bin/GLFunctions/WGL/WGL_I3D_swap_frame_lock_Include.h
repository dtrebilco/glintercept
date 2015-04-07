#define GLI_INCLUDE_WGL_I3D_SWAP_FRAME_LOCK

GLboolean wglEnableFrameLockI3D(void);
GLboolean wglDisableFrameLockI3D(void);
GLboolean wglIsEnabledFrameLockI3D(GLboolean * pFlag);
GLboolean wglQueryFrameLockMasterI3D(GLboolean * pFlag);
