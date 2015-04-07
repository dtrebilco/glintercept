#define GLI_INCLUDE_GL_NV_FRAMEBUFFER_MIXED_SAMPLES

enum Main {

  GL_COVERAGE_MODULATION_TABLE_NV             = 0x9331,
  //GL_COLOR_SAMPLES_NV                         = 0x8E20,
  GL_DEPTH_SAMPLES_NV                         = 0x932D,
  GL_STENCIL_SAMPLES_NV                       = 0x932E,
  GL_MIXED_DEPTH_SAMPLES_SUPPORTED_NV         = 0x932F,
  GL_MIXED_STENCIL_SAMPLES_SUPPORTED_NV       = 0x9330,
  GL_COVERAGE_MODULATION_NV                   = 0x9332,
  GL_COVERAGE_MODULATION_TABLE_SIZE_NV        = 0x9333,

};

void glCoverageModulationTableNV(GLsizei n, const GLfloat *v);
void glGetCoverageModulationTableNV(GLsizei bufsize, GLfloat *v);
void glCoverageModulationNV(GLenum[Main] components);
