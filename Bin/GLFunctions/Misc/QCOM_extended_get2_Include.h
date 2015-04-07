#define GLI_INCLUDE_GL_QCOM_EXTENDED_GET2

void glExtGetShadersQCOM(GLuint *shaders, GLint maxShaders, GLint *numShaders);
void glExtGetProgramsQCOM(GLuint *programs, GLint maxPrograms, GLint *numPrograms);
GLboolean glExtIsProgramBinaryQCOM(GLuint program);
void glExtGetProgramBinarySourceQCOM(GLuint program, GLenum[Main] shadertype, GLchar *source, GLint *length);
