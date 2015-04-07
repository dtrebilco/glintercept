#define GLI_INCLUDE_NV_CONDITIONAL_RENDER

enum Main {

  //GL_QUERY_WAIT_NV                = 0x8E13,
  //GL_QUERY_NO_WAIT_NV             = 0x8E14,
  //GL_QUERY_BY_REGION_WAIT_NV      = 0x8E15,
  //GL_QUERY_BY_REGION_NO_WAIT_NV   = 0x8E16,

};


void glBeginConditionalRenderNV(GLuint id,  GLenum[Main] mode);
void glEndConditionalRenderNV(void);



