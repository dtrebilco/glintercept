#define GLI_INCLUDE_NV_FRAMEBUFFER_MULTISAMPLE_COVERAGE

enum Main {

  //GL_RENDERBUFFER_COVERAGE_SAMPLES_NV    = 0x8CAB,
  GL_RENDERBUFFER_COLOR_SAMPLES_NV       = 0x8E10,
  GL_MAX_MULTISAMPLE_COVERAGE_MODES_NV   = 0x8E11,
  GL_MULTISAMPLE_COVERAGE_MODES_NV       = 0x8E12,

};

void glRenderbufferStorageMultisampleCoverageNV(GLenum[Main] target, GLsizei coverageSamples, GLsizei colorSamples, GLenum[Main] internalformat, GLsizei width, GLsizei height);
