#define GLI_INCLUDE_GL_OES_TEXTURE_BORDER_CLAMP

enum Main {

  //GL_TEXTURE_BORDER_COLOR_OES       = 0x1004,
  //GL_CLAMP_TO_BORDER_OES            = 0x812D,

};

void glTexParameterIivOES(GLenum[Main] target, GLenum[Main] pname, const GLint *params);
void glTexParameterIuivOES(GLenum[Main] target, GLenum[Main] pname, const GLuint *params);
void glGetTexParameterIivOES(GLenum[Main] target, GLenum[Main] pname, GLint *params);
void glGetTexParameterIuivOES(GLenum[Main] target, GLenum[Main] pname, GLuint *params);
void glSamplerParameterIivOES(GLuint sampler, GLenum[Main] pname, const GLint *param);
void glSamplerParameterIuivOES(GLuint sampler, GLenum[Main] pname, const GLuint *param);
void glGetSamplerParameterIivOES(GLuint sampler, GLenum[Main] pname, GLint *params);
void glGetSamplerParameterIuivOES(GLuint sampler, GLenum[Main] pname, GLuint *params);
