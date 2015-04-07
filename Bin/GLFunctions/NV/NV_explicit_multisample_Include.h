#define GLI_INCLUDE_NV_EXPLICIT_MULTISAMPLE

enum Main {

  //GL_SAMPLE_POSITION_NV                         = 0x8E50,
  //GL_SAMPLE_MASK_NV                             = 0x8E51,
  //GL_SAMPLE_MASK_VALUE_NV                       = 0x8E52,
  GL_TEXTURE_BINDING_RENDERBUFFER_NV            = 0x8E53,
  GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV = 0x8E54,
  //GL_MAX_SAMPLE_MASK_WORDS_NV                   = 0x8E59,
  GL_TEXTURE_RENDERBUFFER_NV                    = 0x8E55,
  GL_SAMPLER_RENDERBUFFER_NV                    = 0x8E56,
  GL_INT_SAMPLER_RENDERBUFFER_NV                = 0x8E57,
  GL_UNSIGNED_INT_SAMPLER_RENDERBUFFER_NV       = 0x8E58,

};


//void glGetBooleanIndexedvEXT( GLenum[Main] value, GLuint index, GLboolean *data);
//void glGetIntegerIndexedvEXT( GLenum[Main] value, GLuint index, GLint *data);
void glGetMultisamplefvNV( GLenum[Main] pname, GLuint index, GLfloat *val);
void glSampleMaskIndexedNV(GLuint index, GLbitfield mask);
void glTexRenderbufferNV( GLenum[Main] target, GLuint renderbuffer);


