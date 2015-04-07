#define GLI_INCLUDE_GL_EXT_VERTEX_WEIGHTING

enum Main {

//  GL_MODELVIEW0_STACK_DEPTH_EXT            = MODELVIEW_STACK_DEPTH,
  GL_MODELVIEW1_STACK_DEPTH_EXT            = 0x8502,
//  GL_MODELVIEW0_MATRIX_EXT                 = MODELVIEW_MATRIX,
  GL_MODELVIEW1_MATRIX_EXT                 = 0x8506,
  GL_VERTEX_WEIGHTING_EXT                  = 0x8509,
//  GL_MODELVIEW0_EXT                        = MODELVIEW,
  //GL_MODELVIEW1_EXT                        = 0x850A,
  GL_CURRENT_VERTEX_WEIGHT_EXT             = 0x850B,
  GL_VERTEX_WEIGHT_ARRAY_EXT               = 0x850C,
  GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT          = 0x850D,
  GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT          = 0x850E,
  GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT        = 0x850F,
  GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT       = 0x8510,

};

void glVertexWeightfEXT(GLfloat weight);
void glVertexWeightfvEXT(const GLfloat * weight);
//void glVertexWeightPointerEXT(GLsizei size, GLenum[Main] type, GLsizei stride, const GLvoid * pointer);
void glVertexWeightPointerEXT(GLint size, GLenum[Main] type, GLsizei stride, const GLvoid * pointer);