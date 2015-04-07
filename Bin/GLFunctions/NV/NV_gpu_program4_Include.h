#define GLI_INCLUDE_NV_GPU_PROGRAM4


enum Main {
  
  //GL_MIN_PROGRAM_TEXEL_OFFSET_NV   = 0x8904,
  //GL_MAX_PROGRAM_TEXEL_OFFSET_NV   = 0x8905,
  GL_PROGRAM_ATTRIB_COMPONENTS_NV  = 0x8906,
  GL_PROGRAM_RESULT_COMPONENTS_NV  = 0x8907,
  GL_MAX_PROGRAM_ATTRIB_COMPONENTS_NV = 0x8908,
  GL_MAX_PROGRAM_RESULT_COMPONENTS_NV = 0x8909,
  GL_MAX_PROGRAM_GENERIC_ATTRIBS_NV = 0x8DA5,
  GL_MAX_PROGRAM_GENERIC_RESULTS_NV = 0x8DA6,

};

void glProgramLocalParameterI4iNV(GLenum[Main] target, GLuint index, GLint x, GLint y, GLint z, GLint w);
void glProgramLocalParameterI4ivNV(GLenum[Main] target, GLuint index, const GLint *params);
void glProgramLocalParametersI4ivNV(GLenum[Main] target, GLuint index, GLsizei count, const GLint *params);
void glProgramLocalParameterI4uiNV(GLenum[Main] target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void glProgramLocalParameterI4uivNV(GLenum[Main] target, GLuint index, const GLuint *params);
void glProgramLocalParametersI4uivNV(GLenum[Main] target, GLuint index, GLsizei count, const GLuint *params);

void glProgramEnvParameterI4iNV(GLenum[Main] target, GLuint index, GLint x, GLint y, GLint z, GLint w);
void glProgramEnvParameterI4ivNV(GLenum[Main] target, GLuint index, const GLint *params);
void glProgramEnvParametersI4ivNV(GLenum[Main] target, GLuint index, GLsizei count, const GLint *params);
void glProgramEnvParameterI4uiNV(GLenum[Main] target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void glProgramEnvParameterI4uivNV(GLenum[Main] target, GLuint index, const GLuint *params);
void glProgramEnvParametersI4uivNV(GLenum[Main] target, GLuint index, GLsizei count, const GLuint *params);

void glGetProgramLocalParameterIivNV(GLenum[Main] target, GLuint index, GLint *params);
void glGetProgramLocalParameterIuivNV(GLenum[Main] target, GLuint index, GLuint *params);
void glGetProgramEnvParameterIivNV(GLenum[Main] target, GLuint index, GLint *params);
void glGetProgramEnvParameterIuivNV(GLenum[Main] target, GLuint index, GLuint *params);
