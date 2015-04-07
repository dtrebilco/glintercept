#define GLI_INCLUDE_GL_AMD_DRAW_BUFFERS_BLEND

void glBlendFuncIndexedAMD(GLuint buf, GLenum[Main] src, GLenum[Main] dst);
void glBlendFuncSeparateIndexedAMD(GLuint buf, GLenum[Main] srcRGB, GLenum[Main] dstRGB, GLenum[Main] srcAlpha, GLenum[Main] dstAlpha);
void glBlendEquationIndexedAMD(GLuint buf, GLenum[Main] mode);
void glBlendEquationSeparateIndexedAMD(GLuint buf, GLenum[Main] modeRGB, GLenum[Main] modeAlpha);
