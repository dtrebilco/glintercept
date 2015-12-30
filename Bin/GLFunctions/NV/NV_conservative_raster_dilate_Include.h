#define GLI_INCLUDE_GL_NV_CONSERVATIVE_RASTER_DILATE

enum Main {

  GL_CONSERVATIVE_RASTER_DILATE_NV                   = 0x9379,
  GL_CONSERVATIVE_RASTER_DILATE_RANGE_NV             = 0x937A,
  GL_CONSERVATIVE_RASTER_DILATE_GRANULARITY_NV       = 0x937B,

};

void glConservativeRasterParameterfNV(GLenum[Main] pname, GLfloat value);
