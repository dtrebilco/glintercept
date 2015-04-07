#define GLI_INCLUDE_EXT_GEOMETRY_SHADER4


enum Main {
  
  //GL_GEOMETRY_SHADER_EXT                      = 0x8DD9,
/* reuse GL_GEOMETRY_VERTICES_OUT_EXT */
/* reuse GL_GEOMETRY_INPUT_TYPE_EXT */
/* reuse GL_GEOMETRY_OUTPUT_TYPE_EXT */
/* reuse GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT */
  //GL_MAX_GEOMETRY_VARYING_COMPONENTS_EXT      = 0x8DDD,
  //GL_MAX_VERTEX_VARYING_COMPONENTS_EXT        = 0x8DDE,
  //GL_MAX_VARYING_COMPONENTS_EXT               = 0x8B4B,
  //GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT      = 0x8DDF,
  //GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT         = 0x8DE0,
  //GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT = 0x8DE1,
/* reuse GL_LINES_ADJACENCY_EXT */
/* reuse GL_LINE_STRIP_ADJACENCY_EXT */
/* reuse GL_TRIANGLES_ADJACENCY_EXT */
/* reuse GL_TRIANGLE_STRIP_ADJACENCY_EXT */
/* reuse GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT */
/* reuse GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_EXT */
/* reuse GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT */
/* reuse GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT */
/* reuse GL_PROGRAM_POINT_SIZE_EXT */

};

void glProgramParameteriEXT(GLuint program, GLenum[Main] pname, GLint value);
void glFramebufferTextureEXT(GLenum[Main] target, GLenum[Main] attachment, GLuint texture, GLint level);
void glFramebufferTextureLayerEXT(GLenum[Main] target, GLenum[Main] attachment, GLuint texture, GLint level, GLint layer);
void glFramebufferTextureFaceEXT(GLenum[Main] target, GLenum[Main] attachment, GLuint texture, GLint level, GLenum[Main] face);
