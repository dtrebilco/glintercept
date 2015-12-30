#define GLI_INCLUDE_GL_ARB_PARALLEL_SHADER_COMPILE

enum Main {

  GL_MAX_SHADER_COMPILER_THREADS_ARB       = 0x91B0,
  GL_COMPLETION_STATUS_ARB                 = 0x91B1,

};

void glMaxShaderCompilerThreadsARB(GLuint count);
