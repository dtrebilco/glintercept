#define GLI_INCLUDE_WGL_AMD_GPU_ASSOCIATION

enum Main {

  //WGL_GPU_VENDOR_AMD                      = 0x1F00,
  //WGL_GPU_RENDERER_STRING_AMD             = 0x1F01,
  //WGL_GPU_OPENGL_VERSION_STRING_AMD       = 0x1F02,
  WGL_GPU_FASTEST_TARGET_GPUS_AMD         = 0x21A2,
  WGL_GPU_RAM_AMD                         = 0x21A3,
  WGL_GPU_CLOCK_AMD                       = 0x21A4,
  WGL_GPU_NUM_PIPES_AMD                   = 0x21A5,
  WGL_GPU_NUM_SIMD_AMD                    = 0x21A6,
  WGL_GPU_NUM_RB_AMD                      = 0x21A7,
  WGL_GPU_NUM_SPI_AMD                     = 0x21A8,

};

GLuint wglGetGPUIDsAMD(GLuint maxCount, GLuint * ids);
GLint wglGetGPUInfoAMD(GLuint id, GLint property, GLenum[Main] dataType, GLuint size, void* data);
GLuint wglGetContextGPUIDAMD(void * hglrc);
void* wglCreateAssociatedContextAMD(GLuint id);
void* wglCreateAssociatedContextAttribsAMD(GLuint id, void* hShareContext, const GLint * attribList);
GLboolean wglDeleteAssociatedContextAMD(void* hglrc);
GLboolean wglMakeAssociatedContextCurrentAMD(void* hglrc);
void* wglGetCurrentAssociatedContextAMD(void);
void wglBlitContextFramebufferAMD(void* dstCtx, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum[Main] filter);
