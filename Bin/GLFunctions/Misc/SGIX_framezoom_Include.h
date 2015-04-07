#define GLI_INCLUDE_GL_SGIX_FRAMEZOOM

enum Main {

  GL_FRAMEZOOM_SGIX                  = 0x818B,
  GL_FRAMEZOOM_FACTOR_SGIX           = 0x818C,
  GL_MAX_FRAMEZOOM_FACTOR_SGIX       = 0x818D,

};

void glFrameZoomSGIX(GLint factor);
