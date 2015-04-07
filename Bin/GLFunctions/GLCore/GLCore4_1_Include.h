#define GLI_INCLUDE_GLCORE4_1

enum Mask_ShaderProgramStages {

  GL_VERTEX_SHADER_BIT                      = 0x00000001,
  GL_FRAGMENT_SHADER_BIT                    = 0x00000002,
  GL_GEOMETRY_SHADER_BIT                    = 0x00000004,
  GL_TESS_CONTROL_SHADER_BIT                = 0x00000008,
  GL_TESS_EVALUATION_SHADER_BIT             = 0x00000010,
  //GL_ALL_SHADER_BITS                        = 0xFFFFFFFF,

};

enum Main {

  GL_FIXED                                  = 0x140C,
  GL_IMPLEMENTATION_COLOR_READ_TYPE         = 0x8B9A,
  GL_IMPLEMENTATION_COLOR_READ_FORMAT       = 0x8B9B,
  GL_LOW_FLOAT                              = 0x8DF0,
  GL_MEDIUM_FLOAT                           = 0x8DF1,
  GL_HIGH_FLOAT                             = 0x8DF2,
  GL_LOW_INT                                = 0x8DF3,
  GL_MEDIUM_INT                             = 0x8DF4,
  GL_HIGH_INT                               = 0x8DF5,
  GL_SHADER_COMPILER                        = 0x8DFA,
  GL_SHADER_BINARY_FORMATS                  = 0x8DF8,
  GL_NUM_SHADER_BINARY_FORMATS              = 0x8DF9,
  GL_MAX_VERTEX_UNIFORM_VECTORS             = 0x8DFB,
  GL_MAX_VARYING_VECTORS                    = 0x8DFC,
  GL_MAX_FRAGMENT_UNIFORM_VECTORS           = 0x8DFD,
  GL_RGB565                                 = 0x8D62,
  GL_PROGRAM_BINARY_RETRIEVABLE_HINT        = 0x8257,
  GL_PROGRAM_BINARY_LENGTH                  = 0x8741,
  GL_NUM_PROGRAM_BINARY_FORMATS             = 0x87FE,
  GL_PROGRAM_BINARY_FORMATS                 = 0x87FF,
  GL_PROGRAM_SEPARABLE                      = 0x8258,
  GL_ACTIVE_PROGRAM                         = 0x8259,
  GL_PROGRAM_PIPELINE_BINDING               = 0x825A,
  GL_MAX_VIEWPORTS                          = 0x825B,
  GL_VIEWPORT_SUBPIXEL_BITS                 = 0x825C,
  GL_VIEWPORT_BOUNDS_RANGE                  = 0x825D,
  GL_LAYER_PROVOKING_VERTEX                 = 0x825E,
  GL_VIEWPORT_INDEX_PROVOKING_VERTEX        = 0x825F,
  GL_UNDEFINED_VERTEX                       = 0x8260,

};

void glReleaseShaderCompiler(void);
void glShaderBinary(GLsizei count, const GLuint * shaders, GLenum[Main] binaryformat, const GLvoid * binary, GLsizei length);
void glGetShaderPrecisionFormat(GLenum[Main] shadertype, GLenum[Main] precisiontype, GLint * range, GLint * precision);
void glDepthRangef(GLfloat n, GLfloat f);
void glClearDepthf(GLfloat d);
void glGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, GLvoid * binary);
void glProgramBinary(GLuint program, GLenum[Main] binaryFormat, const GLvoid * binary, GLsizei length);
void glProgramParameteri(GLuint program, GLenum[Main] pname, GLint value);
void glUseProgramStages(GLuint pipeline, GLbitfield[Mask_ShaderProgramStages] stages, GLuint program);
void glActiveShaderProgram(GLuint pipeline, GLuint program);
GLuint glCreateShaderProgramv(GLenum[Main] type, GLsizei count, const GLchar* * strings);
void glBindProgramPipeline(GLuint pipeline);
void glDeleteProgramPipelines(GLsizei n, const GLuint * pipelines);
void glGenProgramPipelines(GLsizei n, GLuint * pipelines);
GLboolean glIsProgramPipeline(GLuint pipeline);
void glGetProgramPipelineiv(GLuint pipeline, GLenum[Main] pname, GLint * params);
void glProgramUniform1i(GLuint program, GLint location, GLint v0);
void glProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint * value);
void glProgramUniform1f(GLuint program, GLint location, GLfloat v0);
void glProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat * value);
void glProgramUniform1d(GLuint program, GLint location, GLdouble v0);
void glProgramUniform1dv(GLuint program, GLint location, GLsizei count, const GLdouble * value);
void glProgramUniform1ui(GLuint program, GLint location, GLuint v0);
void glProgramUniform1uiv(GLuint program, GLint location, GLsizei count, const GLuint * value);
void glProgramUniform2i(GLuint program, GLint location, GLint v0, GLint v1);
void glProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint * value);
void glProgramUniform2f(GLuint program, GLint location, GLfloat v0, GLfloat v1);
void glProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat * value);
void glProgramUniform2d(GLuint program, GLint location, GLdouble v0, GLdouble v1);
void glProgramUniform2dv(GLuint program, GLint location, GLsizei count, const GLdouble * value);
void glProgramUniform2ui(GLuint program, GLint location, GLuint v0, GLuint v1);
void glProgramUniform2uiv(GLuint program, GLint location, GLsizei count, const GLuint * value);
void glProgramUniform3i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
void glProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint * value);
void glProgramUniform3f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void glProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat * value);
void glProgramUniform3d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
void glProgramUniform3dv(GLuint program, GLint location, GLsizei count, const GLdouble * value);
void glProgramUniform3ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
void glProgramUniform3uiv(GLuint program, GLint location, GLsizei count, const GLuint * value);
void glProgramUniform4i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void glProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint * value);
void glProgramUniform4f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void glProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat * value);
void glProgramUniform4d(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
void glProgramUniform4dv(GLuint program, GLint location, GLsizei count, const GLdouble * value);
void glProgramUniform4ui(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void glProgramUniform4uiv(GLuint program, GLint location, GLsizei count, const GLuint * value);
void glProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
void glProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
void glProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
void glProgramUniformMatrix2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix2x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
void glProgramUniformMatrix3x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
void glProgramUniformMatrix2x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
void glProgramUniformMatrix4x2fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
void glProgramUniformMatrix3x4fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
void glProgramUniformMatrix4x3fv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
void glProgramUniformMatrix2x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix3x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix2x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix4x2dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix3x4dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix4x3dv(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glValidateProgramPipeline(GLuint pipeline);
void glGetProgramPipelineInfoLog(GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog);

void glVertexAttribL1d(GLuint index, GLdouble x);
void glVertexAttribL2d(GLuint index, GLdouble x, GLdouble y);
void glVertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void glVertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glVertexAttribL1dv(GLuint index, const GLdouble * v);
void glVertexAttribL2dv(GLuint index, const GLdouble * v);
void glVertexAttribL3dv(GLuint index, const GLdouble * v);
void glVertexAttribL4dv(GLuint index, const GLdouble * v);
void glVertexAttribLPointer(GLuint index, GLint size, GLenum[Main] type, GLsizei stride, const GLvoid * pointer);
void glGetVertexAttribLdv(GLuint index, GLenum[Main] pname, GLdouble * params);

void glViewportArrayv(GLuint first, GLsizei count, const GLfloat * v);
void glViewportIndexedf(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
void glViewportIndexedfv(GLuint index, const GLfloat * v);
void glScissorArrayv(GLuint first, GLsizei count, const GLint * v);
void glScissorIndexed(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
void glScissorIndexedv(GLuint index, const GLint * v);
void glDepthRangeArrayv(GLuint first, GLsizei count, const GLdouble * v);
void glDepthRangeIndexed(GLuint index, GLdouble n, GLdouble f);
void glGetFloati_v(GLenum[Main] target, GLuint index, GLfloat * data);
void glGetDoublei_v(GLenum[Main] target, GLuint index, GLdouble * data);


