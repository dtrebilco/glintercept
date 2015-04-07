#define GLI_INCLUDE_GL_EXT_RASTER_MULTISAMPLE

enum Main {

  GL_RASTER_MULTISAMPLE_EXT                      = 0x9327,
  GL_RASTER_SAMPLES_EXT                          = 0x9328,
  GL_MAX_RASTER_SAMPLES_EXT                      = 0x9329,
  GL_RASTER_FIXED_SAMPLE_LOCATIONS_EXT           = 0x932A,
  GL_MULTISAMPLE_RASTERIZATION_ALLOWED_EXT       = 0x932B,
  GL_EFFECTIVE_RASTER_SAMPLES_EXT                = 0x932C,

};

void glRasterSamplesEXT(GLuint samples, GLboolean fixedsamplelocations);
