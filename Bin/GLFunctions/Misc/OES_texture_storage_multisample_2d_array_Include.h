#define GLI_INCLUDE_GL_OES_TEXTURE_STORAGE_MULTISAMPLE_2D_ARRAY

enum Main {

  //GL_TEXTURE_2D_MULTISAMPLE_ARRAY_OES                    = 0x9102,
  //GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY_OES            = 0x9105,
  //GL_SAMPLER_2D_MULTISAMPLE_ARRAY_OES                    = 0x910B,
  //GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY_OES                = 0x910C,
  //GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY_OES       = 0x910D,

};

void glTexStorage3DMultisampleOES(GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
