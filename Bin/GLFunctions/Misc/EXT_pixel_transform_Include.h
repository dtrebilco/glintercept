#define GLI_INCLUDE_GL_EXT_PIXEL_TRANSFORM

enum Main {

  GL_PIXEL_TRANSFORM_2D_EXT                       = 0x8330,
  GL_PIXEL_MAG_FILTER_EXT                         = 0x8331,
  GL_PIXEL_MIN_FILTER_EXT                         = 0x8332,
  GL_PIXEL_CUBIC_WEIGHT_EXT                       = 0x8333,
  GL_CUBIC_EXT                                    = 0x8334,
  GL_AVERAGE_EXT                                  = 0x8335,
  GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT           = 0x8336,
  GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT       = 0x8337,
  GL_PIXEL_TRANSFORM_2D_MATRIX_EXT                = 0x8338,

};

void glPixelTransformParameteriEXT(GLenum[Main] target, GLenum[Main] pname, GLint param);
void glPixelTransformParameterfEXT(GLenum[Main] target, GLenum[Main] pname, GLfloat param);
void glPixelTransformParameterivEXT(GLenum[Main] target, GLenum[Main] pname, const GLint * params);
void glPixelTransformParameterfvEXT(GLenum[Main] target, GLenum[Main] pname, const GLfloat * params);
void glGetPixelTransformParameterivEXT(GLenum[Main] target, GLenum[Main] pname, const GLint* params);
void glGetPixelTransformParameterfvEXT(GLenum[Main] target, GLenum[Main] pname, const GLfloat* params);
