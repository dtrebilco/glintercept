#define GLI_INCLUDE_GL_OES_TEXTURE_VIEW

enum Main {

  //GL_TEXTURE_VIEW_MIN_LEVEL_OES        = 0x82DB,
  //GL_TEXTURE_VIEW_NUM_LEVELS_OES       = 0x82DC,
  //GL_TEXTURE_VIEW_MIN_LAYER_OES        = 0x82DD,
  //GL_TEXTURE_VIEW_NUM_LAYERS_OES       = 0x82DE,

};

void glTextureViewOES(GLuint texture, GLenum[Main] target, GLuint origtexture, GLenum[Main] internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
