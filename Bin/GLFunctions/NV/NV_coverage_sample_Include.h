#define GLI_INCLUDE_GL_NV_COVERAGE_SAMPLE

enum Main {

  GL_COVERAGE_COMPONENT_NV            = 0x8ED0,
  GL_COVERAGE_COMPONENT4_NV           = 0x8ED1,
  GL_COVERAGE_ATTACHMENT_NV           = 0x8ED2,
  GL_COVERAGE_BUFFERS_NV              = 0x8ED3,
  GL_COVERAGE_SAMPLES_NV              = 0x8ED4,
  GL_COVERAGE_ALL_FRAGMENTS_NV        = 0x8ED5,
  GL_COVERAGE_EDGE_FRAGMENTS_NV       = 0x8ED6,
  GL_COVERAGE_AUTOMATIC_NV            = 0x8ED7,
  //GL_COVERAGE_BUFFER_BIT_NV           = 0x00008000,

};

void glCoverageMaskNV(GLboolean mask);
void glCoverageOperationNV(GLenum[Main] operation);
