#define GLI_INCLUDE_NV_PIXEL_DATA_RANGE

enum Main {

  GL_WRITE_PIXEL_DATA_RANGE_NV               =       0x8878,
  GL_READ_PIXEL_DATA_RANGE_NV                =       0x8879,
  GL_WRITE_PIXEL_DATA_RANGE_LENGTH_NV        =       0x887A,
  GL_READ_PIXEL_DATA_RANGE_LENGTH_NV         =       0x887B,
  GL_WRITE_PIXEL_DATA_RANGE_POINTER_NV       =       0x887C,
  GL_READ_PIXEL_DATA_RANGE_POINTER_NV        =       0x887D,
};



void glPixelDataRangeNV(GLenum[Main] target, GLsizei length, void *pointer);
void glFlushPixelDataRangeNV(GLenum[Main] target);


