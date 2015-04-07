#define GLI_INCLUDE_GLCORE4_0

enum Primitives {

  GL_PATCHES                               = 0x000E,

};

enum Main {

  GL_SAMPLE_SHADING                                           = 0x8C36,
  GL_MIN_SAMPLE_SHADING_VALUE                                 = 0x8C37,
  GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET                        = 0x8E5E,
  GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET                        = 0x8E5F,
  GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS                    = 0x8F9F,
  GL_TEXTURE_CUBE_MAP_ARRAY                                   = 0x9009,
  GL_TEXTURE_BINDING_CUBE_MAP_ARRAY                           = 0x900A,
  GL_PROXY_TEXTURE_CUBE_MAP_ARRAY                             = 0x900B,
  GL_SAMPLER_CUBE_MAP_ARRAY                                   = 0x900C,
  GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW                            = 0x900D,
  GL_INT_SAMPLER_CUBE_MAP_ARRAY                               = 0x900E,
  GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY                      = 0x900F,
  GL_DRAW_INDIRECT_BUFFER                                     = 0x8F3F,
  GL_DRAW_INDIRECT_BUFFER_BINDING                             = 0x8F43,
  GL_GEOMETRY_SHADER_INVOCATIONS                              = 0x887F,
  GL_MAX_GEOMETRY_SHADER_INVOCATIONS                          = 0x8E5A,
  GL_MIN_FRAGMENT_INTERPOLATION_OFFSET                        = 0x8E5B,
  GL_MAX_FRAGMENT_INTERPOLATION_OFFSET                        = 0x8E5C,
  GL_FRAGMENT_INTERPOLATION_OFFSET_BITS                       = 0x8E5D,
  GL_MAX_VERTEX_STREAMS                                       = 0x8E71,
  GL_DOUBLE_VEC2                                              = 0x8FFC,
  GL_DOUBLE_VEC3                                              = 0x8FFD,
  GL_DOUBLE_VEC4                                              = 0x8FFE,
  GL_DOUBLE_MAT2                                              = 0x8F46,
  GL_DOUBLE_MAT3                                              = 0x8F47,
  GL_DOUBLE_MAT4                                              = 0x8F48,
  GL_DOUBLE_MAT2x3                                            = 0x8F49,
  GL_DOUBLE_MAT2x4                                            = 0x8F4A,
  GL_DOUBLE_MAT3x2                                            = 0x8F4B,
  GL_DOUBLE_MAT3x4                                            = 0x8F4C,
  GL_DOUBLE_MAT4x2                                            = 0x8F4D,
  GL_DOUBLE_MAT4x3                                            = 0x8F4E,
  GL_ACTIVE_SUBROUTINES                                       = 0x8DE5,
  GL_ACTIVE_SUBROUTINE_UNIFORMS                               = 0x8DE6,
  GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS                      = 0x8E47,
  GL_ACTIVE_SUBROUTINE_MAX_LENGTH                             = 0x8E48,
  GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH                     = 0x8E49,
  GL_MAX_SUBROUTINES                                          = 0x8DE7,
  GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS                         = 0x8DE8,
  GL_NUM_COMPATIBLE_SUBROUTINES                               = 0x8E4A,
  GL_COMPATIBLE_SUBROUTINES                                   = 0x8E4B,
  GL_PATCH_VERTICES                                           = 0x8E72,
  GL_PATCH_DEFAULT_INNER_LEVEL                                = 0x8E73,
  GL_PATCH_DEFAULT_OUTER_LEVEL                                = 0x8E74,
  GL_TESS_CONTROL_OUTPUT_VERTICES                             = 0x8E75,
  GL_TESS_GEN_MODE                                            = 0x8E76,
  GL_TESS_GEN_SPACING                                         = 0x8E77,
  GL_TESS_GEN_VERTEX_ORDER                                    = 0x8E78,
  GL_TESS_GEN_POINT_MODE                                      = 0x8E79,
  GL_ISOLINES                                                 = 0x8E7A,
  GL_FRACTIONAL_ODD                                           = 0x8E7B,
  GL_FRACTIONAL_EVEN                                          = 0x8E7C,
  GL_MAX_PATCH_VERTICES                                       = 0x8E7D,
  GL_MAX_TESS_GEN_LEVEL                                       = 0x8E7E,
  GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS                      = 0x8E7F,
  GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS                   = 0x8E80,
  GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS                     = 0x8E81,
  GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS                  = 0x8E82,
  GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS                       = 0x8E83,
  GL_MAX_TESS_PATCH_COMPONENTS                                = 0x8E84,
  GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS                 = 0x8E85,
  GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS                    = 0x8E86,
  GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS                          = 0x8E89,
  GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS                       = 0x8E8A,
  GL_MAX_TESS_CONTROL_INPUT_COMPONENTS                        = 0x886C,
  GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS                     = 0x886D,
  GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS             = 0x8E1E,
  GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS          = 0x8E1F,
  GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER          = 0x84F0,
  GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER       = 0x84F1,
  GL_TESS_EVALUATION_SHADER                                   = 0x8E87,
  GL_TESS_CONTROL_SHADER                                      = 0x8E88,
  GL_TRANSFORM_FEEDBACK                                       = 0x8E22,
  GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED                         = 0x8E23,
  GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE                         = 0x8E24,
  GL_TRANSFORM_FEEDBACK_BINDING                               = 0x8E25,
  GL_MAX_TRANSFORM_FEEDBACK_BUFFERS                           = 0x8E70,

};

void glMinSampleShading(GLfloat value);
void glBlendEquationi(GLuint buf, GLenum[Main] mode);
void glBlendEquationSeparatei(GLuint buf, GLenum[Main] modeRGB, GLenum[Main] modeAlpha);
void glBlendFunci(GLuint buf, GLenum[Main] src, GLenum[Main] dst);
void glBlendFuncSeparatei(GLuint buf, GLenum[Main] srcRGB, GLenum[Main] dstRGB, GLenum[Main] srcAlpha, GLenum[Main] dstAlpha);

void glDrawArraysIndirect(GLenum[Primitives] mode, const GLvoid * indirect);
void glDrawElementsIndirect(GLenum[Primitives] mode, GLenum[Main] type, const GLvoid * indirect);
void glDrawTransformFeedback(GLenum[Primitives] mode, GLuint id);
void glDrawTransformFeedbackStream(GLenum[Primitives] mode, GLuint id, GLuint stream);

void glUniform1d(GLint location, GLdouble x);
void glUniform2d(GLint location, GLdouble x, GLdouble y);
void glUniform3d(GLint location, GLdouble x, GLdouble y, GLdouble z);
void glUniform4d(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glUniform1dv(GLint location, GLsizei count, const GLdouble * value);
void glUniform2dv(GLint location, GLsizei count, const GLdouble * value);
void glUniform3dv(GLint location, GLsizei count, const GLdouble * value);
void glUniform4dv(GLint location, GLsizei count, const GLdouble * value);
void glUniformMatrix2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glUniformMatrix3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glUniformMatrix4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glUniformMatrix2x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glUniformMatrix2x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glUniformMatrix3x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glUniformMatrix3x4dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glUniformMatrix4x2dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glUniformMatrix4x3dv(GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glGetUniformdv(GLuint program, GLint location, GLdouble * params);

GLint glGetSubroutineUniformLocation(GLuint program, GLenum[Main] shadertype, const GLchar * name);
GLuint glGetSubroutineIndex(GLuint program, GLenum[Main] shadertype, const GLchar * name);
void glGetActiveSubroutineUniformiv(GLuint program, GLenum[Main] shadertype, GLuint index, GLenum[Main] pname, GLint * values);
void glGetActiveSubroutineUniformName(GLuint program, GLenum[Main] shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name);
void glGetActiveSubroutineName(GLuint program, GLenum[Main] shadertype, GLuint index, GLsizei bufsize, GLsizei * length, GLchar * name);
void glUniformSubroutinesuiv(GLenum[Main] shadertype, GLsizei count, const GLuint * indices);
void glGetUniformSubroutineuiv(GLenum[Main] shadertype, GLint location, GLuint * params);
void glGetProgramStageiv(GLuint program, GLenum[Main] shadertype, GLenum[Main] pname, GLint * values);

void glPatchParameteri(GLenum[Main] pname, GLint value);
void glPatchParameterfv(GLenum[Main] pname, const GLfloat * values);

void glBindTransformFeedback(GLenum[Main] target, GLuint id);
void glDeleteTransformFeedbacks(GLsizei n, const GLuint * ids);
void glGenTransformFeedbacks(GLsizei n, GLuint * ids);
GLboolean glIsTransformFeedback(GLuint id);
void glPauseTransformFeedback(void);
void glResumeTransformFeedback(void);

void glBeginQueryIndexed(GLenum[Main] target, GLuint index, GLuint id);
void glEndQueryIndexed(GLenum[Main] target, GLuint index);
void glGetQueryIndexediv(GLenum[Main] target, GLuint index, GLenum[Main] pname, GLint * params);
