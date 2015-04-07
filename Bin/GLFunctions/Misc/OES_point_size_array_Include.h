#define GLI_INCLUDE_GL_OES_POINT_SIZE_ARRAY

enum Main {

  GL_POINT_SIZE_ARRAY_OES                      = 0x8B9C,
  GL_POINT_SIZE_ARRAY_TYPE_OES                 = 0x898A,
  GL_POINT_SIZE_ARRAY_STRIDE_OES               = 0x898B,
  GL_POINT_SIZE_ARRAY_POINTER_OES              = 0x898C,
  GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES       = 0x8B9F,

};

void glPointSizePointerOES(GLenum[Main] type, GLsizei stride, const void *pointer);
