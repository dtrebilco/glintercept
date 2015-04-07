#define GLI_INCLUDE_GLCORE4_2

enum Mask_MemBarrierMask {

  GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT                                  = 0x00000001,
  GL_ELEMENT_ARRAY_BARRIER_BIT                                        = 0x00000002,
  GL_UNIFORM_BARRIER_BIT                                              = 0x00000004,
  GL_TEXTURE_FETCH_BARRIER_BIT                                        = 0x00000008,
  GL_SHADER_IMAGE_ACCESS_BARRIER_BIT                                  = 0x00000020,
  GL_COMMAND_BARRIER_BIT                                              = 0x00000040,
  GL_PIXEL_BUFFER_BARRIER_BIT                                         = 0x00000080,
  GL_TEXTURE_UPDATE_BARRIER_BIT                                       = 0x00000100,
  GL_BUFFER_UPDATE_BARRIER_BIT                                        = 0x00000200,
  GL_FRAMEBUFFER_BARRIER_BIT                                          = 0x00000400,
  GL_TRANSFORM_FEEDBACK_BARRIER_BIT                                   = 0x00000800,
  GL_ATOMIC_COUNTER_BARRIER_BIT                                       = 0x00001000,
  //GL_ALL_BARRIER_BITS                                                 = 0xFFFFFFFF,
};

enum Main {

  GL_UNPACK_COMPRESSED_BLOCK_WIDTH                                    = 0x9127,
  GL_UNPACK_COMPRESSED_BLOCK_HEIGHT                                   = 0x9128,
  GL_UNPACK_COMPRESSED_BLOCK_DEPTH                                    = 0x9129,
  GL_UNPACK_COMPRESSED_BLOCK_SIZE                                     = 0x912A,
  GL_PACK_COMPRESSED_BLOCK_WIDTH                                      = 0x912B,
  GL_PACK_COMPRESSED_BLOCK_HEIGHT                                     = 0x912C,
  GL_PACK_COMPRESSED_BLOCK_DEPTH                                      = 0x912D,
  GL_PACK_COMPRESSED_BLOCK_SIZE                                       = 0x912E,
  GL_NUM_SAMPLE_COUNTS                                                = 0x9380,
  GL_MIN_MAP_BUFFER_ALIGNMENT                                         = 0x90BC,
  GL_ATOMIC_COUNTER_BUFFER                                            = 0x92C0,
  GL_ATOMIC_COUNTER_BUFFER_BINDING                                    = 0x92C1,
  GL_ATOMIC_COUNTER_BUFFER_START                                      = 0x92C2,
  GL_ATOMIC_COUNTER_BUFFER_SIZE                                       = 0x92C3,
  GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE                                  = 0x92C4,
  GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS                     = 0x92C5,
  GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES              = 0x92C6,
  GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER                = 0x92C7,
  GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER          = 0x92C8,
  GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER       = 0x92C9,
  GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER              = 0x92CA,
  GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER              = 0x92CB,
  GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS                                = 0x92CC,
  GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS                          = 0x92CD,
  GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS                       = 0x92CE,
  GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS                              = 0x92CF,
  GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS                              = 0x92D0,
  GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS                              = 0x92D1,
  GL_MAX_VERTEX_ATOMIC_COUNTERS                                       = 0x92D2,
  GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS                                 = 0x92D3,
  GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS                              = 0x92D4,
  GL_MAX_GEOMETRY_ATOMIC_COUNTERS                                     = 0x92D5,
  GL_MAX_FRAGMENT_ATOMIC_COUNTERS                                     = 0x92D6,
  GL_MAX_COMBINED_ATOMIC_COUNTERS                                     = 0x92D7,
  GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE                                   = 0x92D8,
  GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS                               = 0x92DC,
  GL_ACTIVE_ATOMIC_COUNTER_BUFFERS                                    = 0x92D9,
  GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX                              = 0x92DA,
  GL_UNSIGNED_INT_ATOMIC_COUNTER                                      = 0x92DB,
  GL_MAX_IMAGE_UNITS                                                  = 0x8F38,
  GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS                    = 0x8F39,
  GL_IMAGE_BINDING_NAME                                               = 0x8F3A,
  GL_IMAGE_BINDING_LEVEL                                              = 0x8F3B,
  GL_IMAGE_BINDING_LAYERED                                            = 0x8F3C,
  GL_IMAGE_BINDING_LAYER                                              = 0x8F3D,
  GL_IMAGE_BINDING_ACCESS                                             = 0x8F3E,
  GL_IMAGE_1D                                                         = 0x904C,
  GL_IMAGE_2D                                                         = 0x904D,
  GL_IMAGE_3D                                                         = 0x904E,
  GL_IMAGE_2D_RECT                                                    = 0x904F,
  GL_IMAGE_CUBE                                                       = 0x9050,
  GL_IMAGE_BUFFER                                                     = 0x9051,
  GL_IMAGE_1D_ARRAY                                                   = 0x9052,
  GL_IMAGE_2D_ARRAY                                                   = 0x9053,
  GL_IMAGE_CUBE_MAP_ARRAY                                             = 0x9054,
  GL_IMAGE_2D_MULTISAMPLE                                             = 0x9055,
  GL_IMAGE_2D_MULTISAMPLE_ARRAY                                       = 0x9056,
  GL_INT_IMAGE_1D                                                     = 0x9057,
  GL_INT_IMAGE_2D                                                     = 0x9058,
  GL_INT_IMAGE_3D                                                     = 0x9059,
  GL_INT_IMAGE_2D_RECT                                                = 0x905A,
  GL_INT_IMAGE_CUBE                                                   = 0x905B,
  GL_INT_IMAGE_BUFFER                                                 = 0x905C,
  GL_INT_IMAGE_1D_ARRAY                                               = 0x905D,
  GL_INT_IMAGE_2D_ARRAY                                               = 0x905E,
  GL_INT_IMAGE_CUBE_MAP_ARRAY                                         = 0x905F,
  GL_INT_IMAGE_2D_MULTISAMPLE                                         = 0x9060,
  GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY                                   = 0x9061,
  GL_UNSIGNED_INT_IMAGE_1D                                            = 0x9062,
  GL_UNSIGNED_INT_IMAGE_2D                                            = 0x9063,
  GL_UNSIGNED_INT_IMAGE_3D                                            = 0x9064,
  GL_UNSIGNED_INT_IMAGE_2D_RECT                                       = 0x9065,
  GL_UNSIGNED_INT_IMAGE_CUBE                                          = 0x9066,
  GL_UNSIGNED_INT_IMAGE_BUFFER                                        = 0x9067,
  GL_UNSIGNED_INT_IMAGE_1D_ARRAY                                      = 0x9068,
  GL_UNSIGNED_INT_IMAGE_2D_ARRAY                                      = 0x9069,
  GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY                                = 0x906A,
  GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE                                = 0x906B,
  GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY                          = 0x906C,
  GL_MAX_IMAGE_SAMPLES                                                = 0x906D,
  GL_IMAGE_BINDING_FORMAT                                             = 0x906E,
  GL_IMAGE_FORMAT_COMPATIBILITY_TYPE                                  = 0x90C7,
  GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE                               = 0x90C8,
  GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS                              = 0x90C9,
  GL_MAX_VERTEX_IMAGE_UNIFORMS                                        = 0x90CA,
  GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS                                  = 0x90CB,
  GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS                               = 0x90CC,
  GL_MAX_GEOMETRY_IMAGE_UNIFORMS                                      = 0x90CD,
  GL_MAX_FRAGMENT_IMAGE_UNIFORMS                                      = 0x90CE,
  GL_MAX_COMBINED_IMAGE_UNIFORMS                                      = 0x90CF,
  GL_COMPRESSED_RGBA_BPTC_UNORM                                       = 0x8E8C,
  GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM                                 = 0x8E8D,
  GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT                                 = 0x8E8E,
  GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT                               = 0x8E8F,
  GL_TEXTURE_IMMUTABLE_FORMAT                                         = 0x912F,

};


void glDrawArraysInstancedBaseInstance(GLenum[Primitives] mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
void glDrawElementsInstancedBaseInstance(GLenum[Primitives] mode, GLsizei count, GLenum[Main] type, const void * indices, GLsizei primcount, GLuint baseinstance);
void glDrawElementsInstancedBaseVertexBaseInstance(GLenum[Primitives] mode, GLsizei count, GLenum[Main] type, const void * indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);
void glDrawTransformFeedbackInstanced(GLenum[Primitives] mode, GLuint id, GLsizei primcount);
void glDrawTransformFeedbackStreamInstanced(GLenum[Primitives] mode, GLuint id, GLuint stream, GLsizei primcount);

void glGetInternalformativ(GLenum[Main] target, GLenum[Main] internalformat, GLenum[Main] pname, GLsizei bufSize, GLint * params);

void glGetActiveAtomicCounterBufferiv(GLuint program, GLuint bufferIndex, GLenum[Main] pname, GLint * params);

void glBindImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum[Main] access, GLenum[Main] format);
void glMemoryBarrier(GLbitfield[Mask_MemBarrierMask] barriers);

void glTexStorage1D(GLenum[Main] target, GLsizei levels, GLenum[Main] internalformat, GLsizei width);
void glTexStorage2D(GLenum[Main] target, GLsizei levels, GLenum[Main] internalformat, GLsizei width, GLsizei height);
void glTexStorage3D(GLenum[Main] target, GLsizei levels, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth);
