#define GLI_INCLUDE_WGL_I3D_SWAP_FRAME_USAGE

GLboolean wglGetFrameUsageI3D(GLfloat * pUsage);
GLboolean wglBeginFrameTrackingI3D(void);
GLboolean wglEndFrameTrackingI3D(void);
GLboolean wglQueryFrameTrackingI3D(GLint * pFrameCount, GLint * pMissedFrames, GLfloat * pLastMissedUsage);
