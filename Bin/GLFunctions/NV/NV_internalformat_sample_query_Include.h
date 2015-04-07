#define GLI_INCLUDE_GL_NV_INTERNALFORMAT_SAMPLE_QUERY

enum Main {

  GL_MULTISAMPLES_NV              = 0x9371,
  GL_SUPERSAMPLE_SCALE_X_NV       = 0x9372,
  GL_SUPERSAMPLE_SCALE_Y_NV       = 0x9373,
  GL_CONFORMANT_NV                = 0x9374,

};

void glGetInternalformatSampleivNV(GLenum[Main] target, GLenum[Main] internalformat, GLsizei samples, GLenum[Main] pname, GLsizei bufSize, GLint *params);
