#define GLI_INCLUDE_GL_EXT_SPARSE_TEXTURE

enum Main {

  //GL_TEXTURE_SPARSE_EXT                               = 0x91A6,
  //GL_VIRTUAL_PAGE_SIZE_INDEX_EXT                      = 0x91A7,
  //GL_NUM_SPARSE_LEVELS_EXT                            = 0x91AA,
  //GL_NUM_VIRTUAL_PAGE_SIZES_EXT                       = 0x91A8,
  //GL_VIRTUAL_PAGE_SIZE_X_EXT                          = 0x9195,
  //GL_VIRTUAL_PAGE_SIZE_Y_EXT                          = 0x9196,
  //GL_VIRTUAL_PAGE_SIZE_Z_EXT                          = 0x9197,
  //GL_TEXTURE_CUBE_MAP_ARRAY_OES                       = 0x9009,
  //GL_MAX_SPARSE_TEXTURE_SIZE_EXT                      = 0x9198,
  //GL_MAX_SPARSE_3D_TEXTURE_SIZE_EXT                   = 0x9199,
  //GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_EXT              = 0x919A,
  //GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_EXT       = 0x91A9,

};

void glTexPageCommitmentEXT(GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);
