#define GLI_INCLUDE_GL_SGIX_POLYNOMIAL_FFD

enum Main {

  GL_GEOMETRY_DEFORMATION_SGIX        = 0x8194,
  GL_TEXTURE_DEFORMATION_SGIX         = 0x8195,
  GL_DEFORMATIONS_MASK_SGIX           = 0x8196,
  GL_MAX_DEFORMATION_ORDER_SGIX       = 0x8197,

};

void glDeformationMap3dSGIX(GLenum[Main] target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble * points);
void glDeformationMap3fSGIX(GLenum[Main] target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat * points);
void glDeformSGIX(GLbitfield mask);
void glLoadIdentityDeformationMapSGIX(GLbitfield mask);
