#define GLI_INCLUDE_GL_OES_DRAW_BUFFERS_INDEXED

void glEnableiOES(GLenum[Main] target, GLuint index);
void glDisableiOES(GLenum[Main] target, GLuint index);
void glBlendEquationiOES(GLuint buf, GLenum[Main] mode);
void glBlendEquationSeparateiOES(GLuint buf, GLenum[Main] modeRGB, GLenum[Main] modeAlpha);
void glBlendFunciOES(GLuint buf, GLenum[Main] src, GLenum[Main] dst);
void glBlendFuncSeparateiOES(GLuint buf, GLenum[Main] srcRGB, GLenum[Main] dstRGB, GLenum[Main] srcAlpha, GLenum[Main] dstAlpha);
void glColorMaskiOES(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
GLboolean glIsEnablediOES(GLenum[Main] target, GLuint index);
