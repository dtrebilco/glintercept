#define GLI_INCLUDE_NV_GEOMETRY_PROGRAM4


enum Main {
  
  //GL_LINES_ADJACENCY_EXT                      = 0x000A,
  //GL_LINE_STRIP_ADJACENCY_EXT                 = 0x000B,
  //GL_TRIANGLES_ADJACENCY_EXT                  = 0x000C,
  //GL_TRIANGLE_STRIP_ADJACENCY_EXT             = 0x000D,
  GL_GEOMETRY_PROGRAM_NV                      = 0x8C26,
  GL_MAX_PROGRAM_OUTPUT_VERTICES_NV           = 0x8C27,
  GL_MAX_PROGRAM_TOTAL_OUTPUT_COMPONENTS_NV   = 0x8C28,
  //GL_GEOMETRY_VERTICES_OUT_EXT                = 0x8DDA,
  //GL_GEOMETRY_INPUT_TYPE_EXT                  = 0x8DDB,
  //GL_GEOMETRY_OUTPUT_TYPE_EXT                 = 0x8DDC,
  //GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT     = 0x8C29,
  //GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT       = 0x8DA7,
  //GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT = 0x8DA8,
  //GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_EXT   = 0x8DA9,
  //GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT = 0x8CD4,
  //GL_PROGRAM_POINT_SIZE_EXT                   = 0x8642,

};

void glProgramVertexLimitNV(GLenum[Main] target, GLint limit);

//void glFramebufferTextureEXT(GLenum target, GLenum attachment, GLuint texture, GLint level);
//void glFramebufferTextureLayerEXT(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
//void glFramebufferTextureFaceEXT(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
