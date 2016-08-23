#define GLI_INCLUDE_GL_NV_CLIP_SPACE_W_SCALING

enum Main {

  GL_VIEWPORT_POSITION_W_SCALE_NV               = 0x937C,
  GL_VIEWPORT_POSITION_W_SCALE_X_COEFF_NV       = 0x937D,
  GL_VIEWPORT_POSITION_W_SCALE_Y_COEFF_NV       = 0x937E,

};

void glViewportPositionWScaleNV(GLuint index, GLfloat xcoeff, GLfloat ycoeff);
