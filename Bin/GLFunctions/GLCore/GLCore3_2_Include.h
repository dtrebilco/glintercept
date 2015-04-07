#define GLI_INCLUDE_GLCORE3_2


///////////////////////////////////////////////////////////////////////////////
// Masks
///////////////////////////////////////////////////////////////////////////////

enum Mask_ClientWait {

  GL_SYNC_FLUSH_COMMANDS_BIT  = 0x00000001,

};

//GL_TIMEOUT_IGNORED  =  0xFFFFFFFFFFFFFFFFull,


///////////////////////////////////////////////////////////////////////////////
// Error enum
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Primitives enum
///////////////////////////////////////////////////////////////////////////////

enum Primitives {

  GL_LINES_ADJACENCY                       = 0x000A,
  GL_LINE_STRIP_ADJACENCY                  = 0x000B,
  GL_TRIANGLES_ADJACENCY                   = 0x000C,
  GL_TRIANGLE_STRIP_ADJACENCY              = 0x000D,

};

///////////////////////////////////////////////////////////////////////////////
// Main enum
///////////////////////////////////////////////////////////////////////////////

// Main enum for all standard enums
//  (may want to break up for speed)
enum Main {

  //GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
  //GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002

  GL_PROGRAM_POINT_SIZE                    = 0x8642,
  GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS      = 0x8C29,
  GL_FRAMEBUFFER_ATTACHMENT_LAYERED        = 0x8DA7,
  GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS  = 0x8DA8,
  GL_GEOMETRY_SHADER                       = 0x8DD9,
  GL_GEOMETRY_VERTICES_OUT                 = 0x8916,
  GL_GEOMETRY_INPUT_TYPE                   = 0x8917,
  GL_GEOMETRY_OUTPUT_TYPE                  = 0x8918,
  GL_MAX_GEOMETRY_UNIFORM_COMPONENTS       = 0x8DDF,
  GL_MAX_GEOMETRY_OUTPUT_VERTICES          = 0x8DE0,
  GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS  = 0x8DE1,
  GL_MAX_VERTEX_OUTPUT_COMPONENTS          = 0x9122,
  GL_MAX_GEOMETRY_INPUT_COMPONENTS         = 0x9123,
  GL_MAX_GEOMETRY_OUTPUT_COMPONENTS        = 0x9124,
  GL_MAX_FRAGMENT_INPUT_COMPONENTS         = 0x9125,
  GL_CONTEXT_PROFILE_MASK                  = 0x9126,
  
  GL_FIRST_VERTEX_CONVENTION                  =  0x8E4D,
  GL_LAST_VERTEX_CONVENTION                   =  0x8E4E,

  GL_PROVOKING_VERTEX                         =  0x8E4F,
  GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION =  0x8E4C,

  GL_TEXTURE_CUBE_MAP_SEAMLESS             = 0x884F,

  GL_MAX_SERVER_WAIT_TIMEOUT    =  0x9111,
  GL_OBJECT_TYPE                =  0x9112,
  GL_SYNC_CONDITION             =  0x9113,
  GL_SYNC_STATUS                =  0x9114,
  GL_SYNC_FLAGS                 =  0x9115,
  GL_SYNC_FENCE                 =  0x9116,
  GL_SYNC_GPU_COMMANDS_COMPLETE =  0x9117,
  GL_UNSIGNALED                 =  0x9118,
  GL_SIGNALED                   =  0x9119,

  GL_ALREADY_SIGNALED           =  0x911A,
  GL_TIMEOUT_EXPIRED            =  0x911B,
  GL_CONDITION_SATISFIED        =  0x911C,
  GL_WAIT_FAILED                =  0x911D,

  GL_SAMPLE_POSITION                             =  0x8E50,
  GL_SAMPLE_MASK                                 =  0x8E51,
  GL_SAMPLE_MASK_VALUE                           =  0x8E52,
  GL_TEXTURE_2D_MULTISAMPLE                      =  0x9100,
  GL_PROXY_TEXTURE_2D_MULTISAMPLE                =  0x9101,

  GL_TEXTURE_2D_MULTISAMPLE_ARRAY                =  0x9102,
  GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY          =  0x9103,

  GL_MAX_SAMPLE_MASK_WORDS                       =  0x8E59,
  GL_MAX_COLOR_TEXTURE_SAMPLES                   =  0x910E,
  GL_MAX_DEPTH_TEXTURE_SAMPLES                   =  0x910F,
  GL_MAX_INTEGER_SAMPLES                         =  0x9110,
  GL_TEXTURE_BINDING_2D_MULTISAMPLE              =  0x9104,
  GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY        =  0x9105,

  GL_TEXTURE_SAMPLES                             =  0x9106,
  GL_TEXTURE_FIXED_SAMPLE_LOCATIONS              =  0x9107,

  GL_SAMPLER_2D_MULTISAMPLE                      =  0x9108, // Add these sampler types
  GL_INT_SAMPLER_2D_MULTISAMPLE                  =  0x9109,
  GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE         =  0x910A,
  GL_SAMPLER_2D_MULTISAMPLE_ARRAY                =  0x910B,
  GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY            =  0x910C,
  GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY   =  0x910D,

  GL_DEPTH_CLAMP                                 =  0x864F,

};

///////////////////////////////////////////////////////////////////////////////
// OpenGL 3.2 functions
///////////////////////////////////////////////////////////////////////////////

void glGetInteger64i_v (GLenum[Main] target, GLuint index, GLint64 *data); 
void glGetBufferParameteri64v (GLenum[Main] target, GLenum[Main] pname, GLint64 *params);
void glFramebufferTexture (GLenum[Main] target, GLenum[Main] attachment, GLuint texture, GLint level);

void glDrawElementsBaseVertex(GLenum[Primitives] mode, GLsizei count, GLenum[Main] type, const GLvoid *indices, GLint basevertex);
void glDrawRangeElementsBaseVertex(GLenum[Primitives] mode, GLuint start, GLuint end, GLsizei count, GLenum[Main] type, const GLvoid *indices, GLint basevertex);
void glDrawElementsInstancedBaseVertex(GLenum[Primitives] mode, GLsizei count, GLenum[Main] type, const GLvoid *indices, GLsizei primcount, GLint basevertex);
void glMultiDrawElementsBaseVertex(GLenum[Primitives] mode, const GLsizei *count, GLenum[Main] type, const GLvoid **indices, GLsizei primcount, const GLint *basevertex);

void glProvokingVertex(GLenum[Main] mode);

GLsync glFenceSync(GLenum[Main] condition, GLbitfield flags);
GLboolean glIsSync(GLsync sync);
void glDeleteSync(GLsync sync);

GLenum[Main] glClientWaitSync(GLsync sync, GLbitfield[Mask_ClientWait] flags, GLuint64 timeout);
void glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);

void glGetInteger64v(GLenum[Main] pname, GLint64 *params);
void glGetSynciv(GLsync sync, GLenum[Main] pname, GLsizei bufSize, GLsizei *length, GLint *values);

void glTexImage2DMultisample(GLenum[Main] target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void glTexImage3DMultisample(GLenum[Main] target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void glGetMultisamplefv(GLenum[Main] pname, GLuint index, GLfloat *val);
void glSampleMaski(GLuint index, GLbitfield mask);

