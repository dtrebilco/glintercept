#define GLI_INCLUDE_NV_VERTEX_ARRAY_RANGE

enum Main {
  
  GL_VERTEX_ARRAY_RANGE_NV                        =   0x851D,
  GL_VERTEX_ARRAY_RANGE_LENGTH_NV                 =   0x851E,
  GL_VERTEX_ARRAY_RANGE_VALID_NV                  =   0x851F,
  GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV            =   0x8520,
  GL_VERTEX_ARRAY_RANGE_POINTER_NV                =   0x8521,

  //NV_vertex_array_range2
  GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV          =   0x8533,
};



void glVertexArrayRangeNV(GLsizei length, const void *pointer);
void glFlushVertexArrayRangeNV(void);


void *wglAllocateMemoryNV(GLsizei size,GLfloat readFrequency,GLfloat writeFrequency,GLfloat priority);
void wglFreeMemoryNV(void *pointer);