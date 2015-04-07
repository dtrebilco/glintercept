#define GLI_INCLUDE_ARB_COLOR_BUFFER_FLOAT

enum Main {

  GL_RGBA_FLOAT_MODE_ARB                  =   0x8820,

  //GL_CLAMP_VERTEX_COLOR_ARB               =   0x891A,
  //GL_CLAMP_FRAGMENT_COLOR_ARB             =   0x891B,
  //GL_CLAMP_READ_COLOR_ARB                 =   0x891C,

  //GL_FIXED_ONLY_ARB                       =   0x891D,

};



void glClampColorARB(GLenum[Main] target, GLenum[Main] clamp);
