#define GLI_INCLUDE_ARB_MATRIX_PALETTE

enum Main {

  GL_MATRIX_PALETTE_ARB                  =  0x8840,

  GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB  =  0x8841,
  GL_MAX_PALETTE_MATRICES_ARB            =  0x8842,
  GL_CURRENT_PALETTE_MATRIX_ARB          =  0x8843,
  

  GL_MATRIX_INDEX_ARRAY_ARB              =  0x8844,

  GL_CURRENT_MATRIX_INDEX_ARB            =  0x8845,
  GL_MATRIX_INDEX_ARRAY_SIZE_ARB         =  0x8846,
  GL_MATRIX_INDEX_ARRAY_TYPE_ARB         =  0x8847,
  GL_MATRIX_INDEX_ARRAY_STRIDE_ARB       =  0x8848,
  GL_MATRIX_INDEX_ARRAY_POINTER_ARB      =  0x8849,


};


void glCurrentPaletteMatrixARB(GLint index);

void glMatrixIndexubvARB (GLint size, const GLubyte *indices);
void glMatrixIndexuivARB (GLint size, const GLuint *indices);
void glMatrixIndexusvARB (GLint size, const GLushort *indices);

void glMatrixIndexPointerARB(GLint size, GLenum[Main] type, GLsizei stride, const GLvoid *pointer);
