#define GLI_INCLUDE_GL_EXT_INDEX_MATERIAL

enum Main {

  GL_INDEX_MATERIAL_EXT                 = 0x81B8,
  GL_INDEX_MATERIAL_PARAMETER_EXT       = 0x81B9,
  GL_INDEX_MATERIAL_FACE_EXT            = 0x81BA,

};

void glIndexMaterialEXT(GLenum[Main] face, GLenum[Main] mode);
