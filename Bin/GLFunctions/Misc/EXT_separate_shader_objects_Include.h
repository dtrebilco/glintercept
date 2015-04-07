#define GLI_INCLUDE_EXT_SEPARATE_SHADER_OBJECTS

enum Main {

  //GL_ACTIVE_PROGRAM_EXT         = 0x8B8D, //(alias for CURRENT_PROGRAM)

};


void glUseShaderProgramEXT(GLenum[Main] type, GLuint program);
void glActiveProgramEXT(GLuint program);
GLuint glCreateShaderProgramEXT(GLenum[Main] type, const GLchar *string);

// ES2 entry points
void glActiveShaderProgramEXT(GLuint pipeline, GLuint program);
void glBindProgramPipelineEXT(GLuint pipeline);
GLuint glCreateShaderProgramvEXT(GLenum[Main] type, GLsizei count, const GLchar **strings);
void glDeleteProgramPipelinesEXT(GLsizei n, const GLuint *pipelines);
void glGenProgramPipelinesEXT(GLsizei n, GLuint *pipelines);
void glGetProgramPipelineInfoLogEXT(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void glGetProgramPipelineivEXT(GLuint pipeline, GLenum[Main] pname, GLint *params);
GLboolean glIsProgramPipelineEXT(GLuint pipeline);
void glUseProgramStagesEXT(GLuint pipeline, GLbitfield stages, GLuint program);
void glValidateProgramPipelineEXT(GLuint pipeline);
