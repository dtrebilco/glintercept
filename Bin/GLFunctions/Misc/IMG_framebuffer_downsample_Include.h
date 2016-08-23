#define GLI_INCLUDE_GL_IMG_FRAMEBUFFER_DOWNSAMPLE

enum Main {

  GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_AND_DOWNSAMPLE_IMG       = 0x913C,
  GL_NUM_DOWNSAMPLE_SCALES_IMG                                   = 0x913D,
  GL_DOWNSAMPLE_SCALES_IMG                                       = 0x913E,
  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_SCALE_IMG                    = 0x913F,

};

void glFramebufferTexture2DDownsampleIMG(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level, GLint xscale, GLint yscale);
void glFramebufferTextureLayerDownsampleIMG(GLenum[Main] target, GLenum[Main] attachment, GLuint texture, GLint level, GLint layer, GLint xscale, GLint yscale);
