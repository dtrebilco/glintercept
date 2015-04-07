#define GLI_INCLUDE_GL_ANGLE_TRANSLATED_SHADER_SOURCE

enum Main {

  GL_TRANSLATED_SHADER_SOURCE_LENGTH_ANGLE       = 0x93A0,

};

void glGetTranslatedShaderSourceANGLE(GLuint shader, GLsizei bufsize, GLsizei *length, GLchar *source);
