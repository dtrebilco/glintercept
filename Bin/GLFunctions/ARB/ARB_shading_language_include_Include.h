#define GLI_INCLUDE_GL_ARB_SHADING_LANGUAGE_INCLUDE

enum Main {

  GL_SHADER_INCLUDE_ARB            = 0x8DAE,
  GL_NAMED_STRING_LENGTH_ARB       = 0x8DE9,
  GL_NAMED_STRING_TYPE_ARB         = 0x8DEA,

};

void glNamedStringARB(GLenum[Main] type, GLint namelen, const GLchar * name, GLint stringlen, const GLchar * string);
void glDeleteNamedStringARB(GLint namelen, const GLchar * name);
void glCompileShaderIncludeARB(GLuint shader, GLsizei count, const GLchar* * path, const GLint * length);
GLboolean glIsNamedStringARB(GLint namelen, const GLchar * name);
void glGetNamedStringARB(GLint namelen, const GLchar * name, GLsizei bufSize, GLint * stringlen, GLchar * string);
void glGetNamedStringivARB(GLint namelen, const GLchar * name, GLenum[Main] pname, GLint * params);
