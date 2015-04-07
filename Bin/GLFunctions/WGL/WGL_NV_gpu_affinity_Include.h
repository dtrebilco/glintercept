#define GLI_INCLUDE_WGL_NV_GPU_AFFINITY

enum Main {

  WGL_ERROR_INCOMPATIBLE_AFFINITY_MASKS_NV       = 0x20D0,
  WGL_ERROR_MISSING_AFFINITY_MASK_NV             = 0x20D1,

};

GLboolean wglEnumGpusNV(GLuint iGpuIndex, void* * phGpu);
GLboolean wglEnumGpuDevicesNV(void* hGpu, GLuint iDeviceIndex, void* lpGpuDevice);
void* wglCreateAffinityDCNV(const void* * phGpuList);
GLboolean wglEnumGpusFromAffinityDCNV(void* hAffinityDC, GLuint iGpuIndex, void* * hGpu);
GLboolean wglDeleteDCNV(void* hdc);
