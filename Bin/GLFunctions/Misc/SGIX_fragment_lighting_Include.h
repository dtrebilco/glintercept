#define GLI_INCLUDE_GL_SGIX_FRAGMENT_LIGHTING

enum Main {

  GL_FRAGMENT_LIGHTING_SGIX                               = 0x8400,
  GL_FRAGMENT_COLOR_MATERIAL_SGIX                         = 0x8401,
  GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX                    = 0x8402,
  GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX               = 0x8403,
  GL_MAX_FRAGMENT_LIGHTS_SGIX                             = 0x8404,
  GL_MAX_ACTIVE_LIGHTS_SGIX                               = 0x8405,
  GL_CURRENT_RASTER_NORMAL_SGIX                           = 0x8406,
  GL_LIGHT_ENV_MODE_SGIX                                  = 0x8407,
  GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX               = 0x8408,
  GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX                   = 0x8409,
  GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX                    = 0x840A,
  GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX       = 0x840B,
  GL_FRAGMENT_LIGHT0_SGIX                                 = 0x840C,
  GL_FRAGMENT_LIGHT1_SGIX                                 = 0x840D,
  GL_FRAGMENT_LIGHT2_SGIX                                 = 0x840E,
  GL_FRAGMENT_LIGHT3_SGIX                                 = 0x840F,
  GL_FRAGMENT_LIGHT4_SGIX                                 = 0x8410,
  GL_FRAGMENT_LIGHT5_SGIX                                 = 0x8411,
  GL_FRAGMENT_LIGHT6_SGIX                                 = 0x8412,
  GL_FRAGMENT_LIGHT7_SGIX                                 = 0x8413,

};

void glFragmentColorMaterialSGIX(GLenum[Main] face, GLenum[Main] mode);
void glFragmentLightfSGIX(GLenum[Main] light, GLenum[Main] pname, GLfloat param);
void glFragmentLightfvSGIX(GLenum[Main] light, GLenum[Main] pname, const GLfloat * params);
void glFragmentLightiSGIX(GLenum[Main] light, GLenum[Main] pname, GLint param);
void glFragmentLightivSGIX(GLenum[Main] light, GLenum[Main] pname, const GLint * params);
void glFragmentLightModelfSGIX(GLenum[Main] pname, GLfloat param);
void glFragmentLightModelfvSGIX(GLenum[Main] pname, const GLfloat * params);
void glFragmentLightModeliSGIX(GLenum[Main] pname, GLint param);
void glFragmentLightModelivSGIX(GLenum[Main] pname, const GLint * params);
void glFragmentMaterialfSGIX(GLenum[Main] face, GLenum[Main] pname, GLfloat param);
void glFragmentMaterialfvSGIX(GLenum[Main] face, GLenum[Main] pname, const GLfloat * params);
void glFragmentMaterialiSGIX(GLenum[Main] face, GLenum[Main] pname, GLint param);
void glFragmentMaterialivSGIX(GLenum[Main] face, GLenum[Main] pname, const GLint * params);
void glGetFragmentLightfvSGIX(GLenum[Main] light, GLenum[Main] pname, GLfloat * params);
void glGetFragmentLightivSGIX(GLenum[Main] light, GLenum[Main] pname, GLint * params);
void glGetFragmentMaterialfvSGIX(GLenum[Main] face, GLenum[Main] pname, GLfloat * params);
void glGetFragmentMaterialivSGIX(GLenum[Main] face, GLenum[Main] pname, GLint * params);
void glLightEnviSGIX(GLenum[Main] pname, GLint param);
