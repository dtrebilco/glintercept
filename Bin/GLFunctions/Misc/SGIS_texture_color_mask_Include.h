#define GLI_INCLUDE_GL_SGIS_TEXTURE_COLOR_MASK

enum Main {

  GL_TEXTURE_COLOR_WRITEMASK_SGIS       = 0x81EF,

};

void glTextureColorMaskSGIS(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
