#define GLI_INCLUDE_GL_EXT_TEXTURE_BORDER_CLAMP

enum Main {

  //GL_TEXTURE_BORDER_COLOR_EXT       = 0x1004,
  //GL_CLAMP_TO_BORDER_EXT            = 0x812D,

};

//void glTexParameterIivEXT(GLenum[Main] target, GLenum[Main] pname, const GLint *params);
//void glTexParameterIuivEXT(GLenum[Main] target, GLenum[Main] pname, const GLuint *params);
//void glGetTexParameterIivEXT(GLenum[Main] target, GLenum[Main] pname, GLint *params);
//void glGetTexParameterIuivEXT(GLenum[Main] target, GLenum[Main] pname, GLuint *params);
void glSamplerParameterIivEXT(GLuint sampler, GLenum[Main] pname, const GLint *param);
void glSamplerParameterIuivEXT(GLuint sampler, GLenum[Main] pname, const GLuint *param);
void glGetSamplerParameterIivEXT(GLuint sampler, GLenum[Main] pname, GLint *params);
void glGetSamplerParameterIuivEXT(GLuint sampler, GLenum[Main] pname, GLuint *params);
