#define GLI_INCLUDE_GL_AMD_SPARSE_TEXTURE

enum Main {

  //GL_VIRTUAL_PAGE_SIZE_X_AMD               = 0x9195,
  //GL_VIRTUAL_PAGE_SIZE_Y_AMD               = 0x9196,
  //GL_VIRTUAL_PAGE_SIZE_Z_AMD               = 0x9197,
  //GL_MAX_SPARSE_TEXTURE_SIZE_AMD           = 0x9198,
  //GL_MAX_SPARSE_3D_TEXTURE_SIZE_AMD        = 0x9199,
  //GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS       = 0x919A,
  GL_MIN_SPARSE_LEVEL_AMD                  = 0x919B,
  GL_MIN_LOD_WARNING_AMD                   = 0x919C,


};

enum Mask_TexStorage {

  GL_TEXTURE_STORAGE_SPARSE_BIT_AMD        = 0x00000001,

};

void glTexStorageSparseAMD(GLenum[Main] target, GLenum[Main] internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield[Mask_TexStorage] flags);
void glTextureStorageSparseAMD(GLuint texture, GLenum[Main] target, GLenum[Main] internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield[Mask_TexStorage] flags);
