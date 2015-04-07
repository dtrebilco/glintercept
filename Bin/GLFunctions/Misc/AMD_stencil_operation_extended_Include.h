#define GLI_INCLUDE_GL_AMD_STENCIL_OPERATION_EXTENDED

enum Main {

  GL_SET_AMD                     =    0x874A,
  //GL_AND                         =    0x1501,
  //GL_XOR                         =    0x1506,
  //GL_OR                          =    0x1507,
  //GL_NOR                         =    0x1508,
  //GL_EQUIV                       =    0x1509,
  //GL_NAND                        =    0x150E,
  GL_REPLACE_VALUE_AMD           =    0x874B,
  GL_STENCIL_OP_VALUE_AMD        =    0x874C,
  GL_STENCIL_BACK_OP_VALUE_AMD   =    0x874D,

};


void glStencilOpValueAMD(GLenum[Main] face, GLuint value);