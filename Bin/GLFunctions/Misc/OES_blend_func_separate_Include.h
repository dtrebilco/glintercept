#define GLI_INCLUDE_GL_OES_BLEND_FUNC_SEPARATE

enum Main {

  //GL_BLEND_DST_RGB_OES         = 0x80C8,
  //GL_BLEND_SRC_RGB_OES         = 0x80C9,
  //GL_BLEND_DST_ALPHA_OES       = 0x80CA,
  //GL_BLEND_SRC_ALPHA_OES       = 0x80CB,

};

void glBlendFuncSeparateOES(GLenum[Main] srcRGB, GLenum[Main] dstRGB, GLenum[Main] srcAlpha, GLenum[Main] dstAlpha);
