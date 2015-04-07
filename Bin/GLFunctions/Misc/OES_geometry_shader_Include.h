#define GLI_INCLUDE_GL_OES_GEOMETRY_SHADER

enum Main {

  //GL_GEOMETRY_SHADER_OES                                = 0x8DD9,
  
  //GL_GEOMETRY_SHADER_BIT_OES                            = 0x00000004,
  
  //GL_GEOMETRY_LINKED_VERTICES_OUT_OES                   = 0x8916,
  //GL_GEOMETRY_LINKED_INPUT_TYPE_OES                     = 0x8917,
  //GL_GEOMETRY_LINKED_OUTPUT_TYPE_OES                    = 0x8918,
  //GL_GEOMETRY_SHADER_INVOCATIONS_OES                    = 0x887F,
  //GL_LAYER_PROVOKING_VERTEX_OES                         = 0x825E,
  
  //GL_LINES_ADJACENCY_OES                                = 0x000A,
  //GL_LINE_STRIP_ADJACENCY_OES                           = 0x000B,
  //GL_TRIANGLES_ADJACENCY_OES                            = 0x000C,
  //GL_TRIANGLE_STRIP_ADJACENCY_OES                       = 0x000D,

  //GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_OES                = 0x8DDF,
  //GL_MAX_GEOMETRY_UNIFORM_BLOCKS_OES                    = 0x8A2C,
  //GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS_OES       = 0x8A32,
  //GL_MAX_GEOMETRY_INPUT_COMPONENTS_OES                  = 0x9123,
  //GL_MAX_GEOMETRY_OUTPUT_COMPONENTS_OES                 = 0x9124,
  //GL_MAX_GEOMETRY_OUTPUT_VERTICES_OES                   = 0x8DE0,
  //GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_OES           = 0x8DE1,
  //GL_MAX_GEOMETRY_SHADER_INVOCATIONS_OES                = 0x8E5A,
  //GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_OES               = 0x8C29,
  //GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS_OES            = 0x92CF,
  //GL_MAX_GEOMETRY_ATOMIC_COUNTERS_OES                   = 0x92D5,
  //GL_MAX_GEOMETRY_IMAGE_UNIFORMS_OES                    = 0x90CD,
  //GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS_OES             = 0x90D7,
  //GL_FIRST_VERTEX_CONVENTION_OES                        = 0x8E4D,
  //GL_LAST_VERTEX_CONVENTION_OES                         = 0x8E4E,
  //GL_UNDEFINED_VERTEX_OES                               = 0x8260,
  //GL_PRIMITIVES_GENERATED_OES                           = 0x8C87,
  //GL_FRAMEBUFFER_DEFAULT_LAYERS_OES                     = 0x9312,
  //GL_MAX_FRAMEBUFFER_LAYERS_OES                         = 0x9317,
  //GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_OES           = 0x8DA8,
  //GL_FRAMEBUFFER_ATTACHMENT_LAYERED_OES                 = 0x8DA7,
  //GL_REFERENCED_BY_GEOMETRY_SHADER_OES                  = 0x9309,

};

void glFramebufferTextureOES(GLenum[Main] target, GLenum[Main] attachment, GLuint texture, GLint level);
