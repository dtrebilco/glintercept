#define GLI_INCLUDE_EXT_BLEND


enum Main {
  
  //GL_BLEND_EQUATION_ALPHA_EXT       = 0x883D,
};


void glBlendColorEXT(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha);

void glBlendFuncSeparateEXT(GLenum[Main] sfactorRGB, GLenum[Main] dfactorRGB, GLenum[Main] sfactorAlpha, GLenum[Main] dfactorAlpha);

void glBlendEquationEXT(GLenum[Main] mode);

void glBlendEquationSeparateEXT(GLenum[Main] modeRGB, GLenum[Main] modeAlpha);
