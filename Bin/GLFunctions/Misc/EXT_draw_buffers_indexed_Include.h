#define GLI_INCLUDE_GL_EXT_DRAW_BUFFERS_INDEXED

void glEnableiEXT(GLenum[Main] target, GLuint index);
void glDisableiEXT(GLenum[Main] target, GLuint index);
void glBlendEquationiEXT(GLuint buf, GLenum[Main] mode);
void glBlendEquationSeparateiEXT(GLuint buf, GLenum[Main] modeRGB, GLenum[Main] modeAlpha);
void glBlendFunciEXT(GLuint buf, GLenum[Main] src, GLenum[Main] dst);
void glBlendFuncSeparateiEXT(GLuint buf, GLenum[Main] srcRGB, GLenum[Main] dstRGB, GLenum[Main] srcAlpha, GLenum[Main] dstAlpha);
void glColorMaskiEXT(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
GLboolean glIsEnablediEXT(GLenum[Main] target, GLuint index);
