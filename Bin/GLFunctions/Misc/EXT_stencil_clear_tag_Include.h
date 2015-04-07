#define GLI_INCLUDE_EXT_STENCIL_CLEAR_TAG


enum Main {
  
  GL_STENCIL_TAG_BITS_EXT             =   0x88F2,
  GL_STENCIL_CLEAR_TAG_VALUE_EXT      =   0x88F3,

};

void glStencilClearTagEXT (GLsizei stencilTagBits, GLuint stencilClearTag);

