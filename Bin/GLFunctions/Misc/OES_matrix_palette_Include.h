#define GLI_INCLUDE_GL_OES_MATRIX_PALETTE

enum Main {

  //GL_MAX_VERTEX_UNITS_OES                        = 0x86A4,
  //GL_MAX_PALETTE_MATRICES_OES                    = 0x8842,
  //GL_MATRIX_PALETTE_OES                          = 0x8840,
  //GL_MATRIX_INDEX_ARRAY_OES                      = 0x8844,
  //GL_WEIGHT_ARRAY_OES                            = 0x86AD,
  //GL_CURRENT_PALETTE_MATRIX_OES                  = 0x8843,
  //GL_MATRIX_INDEX_ARRAY_SIZE_OES                 = 0x8846,
  //GL_MATRIX_INDEX_ARRAY_TYPE_OES                 = 0x8847,
  //GL_MATRIX_INDEX_ARRAY_STRIDE_OES               = 0x8848,
  //GL_MATRIX_INDEX_ARRAY_POINTER_OES              = 0x8849,
  GL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES       = 0x8B9E,
  //GL_WEIGHT_ARRAY_SIZE_OES                       = 0x86AB,
  //GL_WEIGHT_ARRAY_TYPE_OES                       = 0x86A9,
  //GL_WEIGHT_ARRAY_STRIDE_OES                     = 0x86AA,
  //GL_WEIGHT_ARRAY_POINTER_OES                    = 0x86AC,
  //GL_WEIGHT_ARRAY_BUFFER_BINDING_OES             = 0x889E,

};

void glCurrentPaletteMatrixOES(GLuint matrixpaletteindex);
void glLoadPaletteFromModelViewMatrixOES(void);
void glMatrixIndexPointerOES(GLint size, GLenum[Main] type, GLsizei stride, const void *pointer);
void glWeightPointerOES(GLint size, GLenum[Main] type, GLsizei stride, const void *pointer);
