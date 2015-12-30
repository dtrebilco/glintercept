#define GLI_INCLUDE_GL_ARB_SAMPLE_LOCATIONS

enum Main {

  GL_SAMPLE_LOCATION_SUBPIXEL_BITS_ARB                   = 0x933D,
  GL_SAMPLE_LOCATION_PIXEL_GRID_WIDTH_ARB                = 0x933E,
  GL_SAMPLE_LOCATION_PIXEL_GRID_HEIGHT_ARB               = 0x933F,
  GL_PROGRAMMABLE_SAMPLE_LOCATION_TABLE_SIZE_ARB         = 0x9340,
  //GL_SAMPLE_LOCATION_ARB                                 = 0x8E50,
  GL_PROGRAMMABLE_SAMPLE_LOCATION_ARB                    = 0x9341,
  GL_FRAMEBUFFER_PROGRAMMABLE_SAMPLE_LOCATIONS_ARB       = 0x9342,
  GL_FRAMEBUFFER_SAMPLE_LOCATION_PIXEL_GRID_ARB          = 0x9343,

};

void glFramebufferSampleLocationsfvARB(GLenum[Main] target, GLuint start, GLsizei count, const GLfloat *v);
void glNamedFramebufferSampleLocationsfvARB(GLuint framebuffer, GLuint start, GLsizei count, const GLfloat *v);
void glEvaluateDepthValuesARB(void);
