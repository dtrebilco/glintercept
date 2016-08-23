#define GLI_INCLUDE_GL_NV_CONSERVATIVE_RASTER_PRE_SNAP_TRIANGLES

enum Main {

  GL_CONSERVATIVE_RASTER_MODE_NV                          = 0x954D,
  GL_CONSERVATIVE_RASTER_MODE_POST_SNAP_NV                = 0x954E,
  GL_CONSERVATIVE_RASTER_MODE_PRE_SNAP_TRIANGLES_NV       = 0x954F,

};

void glConservativeRasterParameteriNV(GLenum[Main] pname, GLint param);
