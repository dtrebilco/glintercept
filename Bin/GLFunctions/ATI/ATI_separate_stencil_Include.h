#define GLI_INCLUDE_ATI_SEPARATE_STENCIL

enum Main {

  //GL_STENCIL_BACK_FUNC_ATI                =   0x8800,
  //GL_STENCIL_BACK_FAIL_ATI                =   0x8801,
  //GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI     =   0x8802,
  //GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI     =   0x8803,

};


void glStencilOpSeparateATI( GLenum[Main] face, GLenum[Main] sfail, GLenum[Main] dpfail, GLenum[Main] dppass);
void glStencilFuncSeparateATI( GLenum[Main] frontfunc, GLenum[Main] backfunc, GLint ref, GLuint mask);



      
