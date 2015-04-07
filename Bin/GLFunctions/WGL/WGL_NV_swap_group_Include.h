#define GLI_INCLUDE_WGL_NV_SWAP_GROUP

GLboolean wglJoinSwapGroupNV(void* hDC, GLuint group);
GLboolean wglBindSwapBarrierNV(GLuint group, GLuint barrier);
GLboolean wglQuerySwapGroupNV(void* hDC, GLuint * group, GLuint * barrier);
GLboolean wglQueryMaxSwapGroupsNV(void* hDC, GLuint * maxGroups, GLuint * maxBarriers);
GLboolean wglQueryFrameCountNV(void* hDC, GLuint * count);
GLboolean wglResetFrameCountNV(void* hDC);
