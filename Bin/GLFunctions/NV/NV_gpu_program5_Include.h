#define GLI_INCLUDE_GL_NV_GPU_PROGRAM5

enum Main {

  //GL_MAX_GEOMETRY_PROGRAM_INVOCATIONS_NV                 = 0x8E5A,
  //GL_MIN_FRAGMENT_INTERPOLATION_OFFSET_NV                = 0x8E5B,
  //GL_MAX_FRAGMENT_INTERPOLATION_OFFSET_NV                = 0x8E5C,
  //GL_FRAGMENT_PROGRAM_INTERPOLATION_OFFSET_BITS_NV       = 0x8E5D,
  //GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_NV                = 0x8E5E,
  //GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_NV                = 0x8E5F,
  GL_MAX_PROGRAM_SUBROUTINE_PARAMETERS_NV                = 0x8F44,
  GL_MAX_PROGRAM_SUBROUTINE_NUM_NV                       = 0x8F45,

};

void glProgramSubroutineParametersuivNV(GLenum[Main] target, GLsizei count, const GLuint * params);
void glGetProgramSubroutineParameteruivNV(GLenum[Main] target, GLuint index, GLuint * param);
