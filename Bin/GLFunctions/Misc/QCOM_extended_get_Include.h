#define GLI_INCLUDE_GL_QCOM_EXTENDED_GET

enum Main {

  GL_TEXTURE_WIDTH_QCOM                 = 0x8BD2,
  GL_TEXTURE_HEIGHT_QCOM                = 0x8BD3,
  GL_TEXTURE_DEPTH_QCOM                 = 0x8BD4,
  GL_TEXTURE_INTERNAL_FORMAT_QCOM       = 0x8BD5,
  GL_TEXTURE_FORMAT_QCOM                = 0x8BD6,
  GL_TEXTURE_TYPE_QCOM                  = 0x8BD7,
  GL_TEXTURE_IMAGE_VALID_QCOM           = 0x8BD8,
  GL_TEXTURE_NUM_LEVELS_QCOM            = 0x8BD9,
  GL_TEXTURE_TARGET_QCOM                = 0x8BDA,
  GL_TEXTURE_OBJECT_VALID_QCOM          = 0x8BDB,
  GL_STATE_RESTORE                      = 0x8BDC,

};

void glExtGetTexturesQCOM(GLuint *textures, GLint maxTextures, GLint *numTextures);
void glExtGetBuffersQCOM(GLuint *buffers, GLint maxBuffers, GLint *numBuffers);
void glExtGetRenderbuffersQCOM(GLuint *renderbuffers, GLint maxRenderbuffers, GLint *numRenderbuffers);
void glExtGetFramebuffersQCOM(GLuint *framebuffers, GLint maxFramebuffers, GLint *numFramebuffers);
void glExtGetTexLevelParameterivQCOM(GLuint texture, GLenum[Main] face, GLint level, GLenum[Main] pname, GLint *params);
void glExtTexObjectStateOverrideiQCOM(GLenum[Main] target, GLenum[Main] pname, GLint param);
void glExtGetTexSubImageQCOM(GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLenum[Main] type, void *texels);
void glExtGetBufferPointervQCOM(GLenum[Main] target, void **params);
