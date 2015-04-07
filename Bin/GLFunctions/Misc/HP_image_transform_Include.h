#define GLI_INCLUDE_GL_HP_IMAGE_TRANSFORM

enum Main {

  GL_IMAGE_SCALE_X_HP                                = 0x8155,
  GL_IMAGE_SCALE_Y_HP                                = 0x8156,
  GL_IMAGE_TRANSLATE_X_HP                            = 0x8157,
  GL_IMAGE_TRANSLATE_Y_HP                            = 0x8158,
  GL_IMAGE_ROTATE_ANGLE_HP                           = 0x8159,
  GL_IMAGE_ROTATE_ORIGIN_X_HP                        = 0x815A,
  GL_IMAGE_ROTATE_ORIGIN_Y_HP                        = 0x815B,
  GL_IMAGE_MAG_FILTER_HP                             = 0x815C,
  GL_IMAGE_MIN_FILTER_HP                             = 0x815D,
  GL_IMAGE_CUBIC_WEIGHT_HP                           = 0x815E,
  GL_CUBIC_HP                                        = 0x815F,
  GL_AVERAGE_HP                                      = 0x8160,
  GL_IMAGE_TRANSFORM_2D_HP                           = 0x8161,
  GL_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP             = 0x8162,
  GL_PROXY_POST_IMAGE_TRANSFORM_COLOR_TABLE_HP       = 0x8163,

};

void glImageTransformParameteriHP(GLenum[Main] target, GLenum[Main] pname, GLint param);
void glImageTransformParameterfHP(GLenum[Main] target, GLenum[Main] pname, GLfloat param);
void glImageTransformParameterivHP(GLenum[Main] target, GLenum[Main] pname, const GLint * params);
void glImageTransformParameterfvHP(GLenum[Main] target, GLenum[Main] pname, const GLfloat * params);
void glGetImageTransformParameterivHP(GLenum[Main] target, GLenum[Main] pname, GLint * params);
void glGetImageTransformParameterfvHP(GLenum[Main] target, GLenum[Main] pname, GLfloat * params);
