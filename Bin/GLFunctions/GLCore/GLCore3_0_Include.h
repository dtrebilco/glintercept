#define GLI_INCLUDE_GLCORE3_0


///////////////////////////////////////////////////////////////////////////////
// Masks
///////////////////////////////////////////////////////////////////////////////
enum Mask_Access {

  GL_MAP_READ_BIT		            = 0x0001,
  GL_MAP_WRITE_BIT		          = 0x0002,
  GL_MAP_INVALIDATE_RANGE_BIT   = 0x0004,
  GL_MAP_INVALIDATE_BUFFER_BIT  = 0x0008,
  GL_MAP_FLUSH_EXPLICIT_BIT	    = 0x0010,
  GL_MAP_UNSYNCHRONIZED_BIT	    = 0x0020,

};

///////////////////////////////////////////////////////////////////////////////
// Error enum
///////////////////////////////////////////////////////////////////////////////
enum Main_Error {

  GL_INVALID_FRAMEBUFFER_OPERATION = 0x0506,

};


///////////////////////////////////////////////////////////////////////////////
// Main enum
///////////////////////////////////////////////////////////////////////////////

// Main enum for all standard enums
//  (may want to break up for speed)
enum Main {

  //GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT = 0x0001,
  GL_COMPARE_REF_TO_TEXTURE              = 0x884E,

  GL_MAX_CLIP_DISTANCES                  = 0x0D32,

  GL_CLIP_DISTANCE0                      = 0x3000,
  GL_CLIP_DISTANCE1                      = 0x3001,
  GL_CLIP_DISTANCE2                      = 0x3002,
  GL_CLIP_DISTANCE3                      = 0x3003,
  GL_CLIP_DISTANCE4                      = 0x3004,
  GL_CLIP_DISTANCE5                      = 0x3005,
  GL_CLIP_DISTANCE6                      = 0x3006,
  GL_CLIP_DISTANCE7                      = 0x3007,

  GL_MAX_VARYING_COMPONENTS              = 0x8B4B,

  GL_MAJOR_VERSION                       = 0x821B,
  GL_MINOR_VERSION                       = 0x821C,
  GL_NUM_EXTENSIONS                      = 0x821D,
  GL_CONTEXT_FLAGS                       = 0x821E,
  GL_DEPTH_BUFFER                        = 0x8223,
  GL_STENCIL_BUFFER                      = 0x8224,
  GL_COMPRESSED_RED                      = 0x8225,
  GL_COMPRESSED_RG                       = 0x8226,
  GL_RGBA32F                             = 0x8814,
  GL_RGB32F                              = 0x8815,
  GL_RGBA16F                             = 0x881A,
  GL_RGB16F                              = 0x881B,
  GL_VERTEX_ATTRIB_ARRAY_INTEGER         = 0x88FD,
  GL_MAX_ARRAY_TEXTURE_LAYERS            = 0x88FF,
  GL_MIN_PROGRAM_TEXEL_OFFSET            = 0x8904,
  GL_MAX_PROGRAM_TEXEL_OFFSET            = 0x8905,
  GL_CLAMP_VERTEX_COLOR                  = 0x891A,
  GL_CLAMP_FRAGMENT_COLOR                = 0x891B,
  GL_CLAMP_READ_COLOR                    = 0x891C,
  GL_FIXED_ONLY                          = 0x891D,
  GL_TEXTURE_RED_TYPE                    = 0x8C10,
  GL_TEXTURE_GREEN_TYPE                  = 0x8C11,
  GL_TEXTURE_BLUE_TYPE                   = 0x8C12,
  GL_TEXTURE_ALPHA_TYPE                  = 0x8C13,
  GL_TEXTURE_LUMINANCE_TYPE              = 0x8C14,
  GL_TEXTURE_INTENSITY_TYPE              = 0x8C15,
  GL_TEXTURE_DEPTH_TYPE                  = 0x8C16,
  GL_UNSIGNED_NORMALIZED                 = 0x8C17,
  GL_TEXTURE_1D_ARRAY                    = 0x8C18, // new tex type
  GL_PROXY_TEXTURE_1D_ARRAY              = 0x8C19,
  GL_TEXTURE_2D_ARRAY                    = 0x8C1A, // new tex type
  GL_PROXY_TEXTURE_2D_ARRAY              = 0x8C1B,
  GL_TEXTURE_BINDING_1D_ARRAY            = 0x8C1C,
  GL_TEXTURE_BINDING_2D_ARRAY            = 0x8C1D,
  GL_R11F_G11F_B10F                      = 0x8C3A,
  GL_UNSIGNED_INT_10F_11F_11F_REV        = 0x8C3B,
  GL_RGB9_E5                             = 0x8C3D,
  GL_UNSIGNED_INT_5_9_9_9_REV            = 0x8C3E,
  GL_TEXTURE_SHARED_SIZE                 = 0x8C3F,
  
  GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH         = 0x8C76,
  GL_TRANSFORM_FEEDBACK_BUFFER_MODE                = 0x8C7F,
  GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS    = 0x8C80,
  GL_TRANSFORM_FEEDBACK_VARYINGS                   = 0x8C83,
  GL_TRANSFORM_FEEDBACK_BUFFER_START               = 0x8C84,
  GL_TRANSFORM_FEEDBACK_BUFFER_SIZE                = 0x8C85,
  GL_PRIMITIVES_GENERATED                          = 0x8C87,
  GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN         = 0x8C88,
  GL_RASTERIZER_DISCARD                            = 0x8C89,
  GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS = 0x8C8A,
  GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS       = 0x8C8B,
  GL_INTERLEAVED_ATTRIBS                           = 0x8C8C,
  GL_SEPARATE_ATTRIBS                              = 0x8C8D,
  GL_TRANSFORM_FEEDBACK_BUFFER                     = 0x8C8E,
  GL_TRANSFORM_FEEDBACK_BUFFER_BINDING             = 0x8C8F,
  
  GL_RGBA32UI                  = 0x8D70,
  GL_RGB32UI                   = 0x8D71,
  GL_RGBA16UI                  = 0x8D76,
  GL_RGB16UI                   = 0x8D77,
  GL_RGBA8UI                   = 0x8D7C,
  GL_RGB8UI                    = 0x8D7D,
  GL_RGBA32I                   = 0x8D82,
  GL_RGB32I                    = 0x8D83,
  GL_RGBA16I                   = 0x8D88,
  GL_RGB16I                    = 0x8D89,
  GL_RGBA8I                    = 0x8D8E,
  GL_RGB8I                     = 0x8D8F,
  GL_RED_INTEGER               = 0x8D94,
  GL_GREEN_INTEGER             = 0x8D95,
  GL_BLUE_INTEGER              = 0x8D96,
  GL_ALPHA_INTEGER             = 0x8D97,
  GL_RGB_INTEGER               = 0x8D98,
  GL_RGBA_INTEGER              = 0x8D99,
  GL_BGR_INTEGER               = 0x8D9A,
  GL_BGRA_INTEGER              = 0x8D9B,
  GL_SAMPLER_1D_ARRAY          = 0x8DC0, //new samplers
  GL_SAMPLER_2D_ARRAY          = 0x8DC1,
  GL_SAMPLER_1D_ARRAY_SHADOW   = 0x8DC3,
  GL_SAMPLER_2D_ARRAY_SHADOW   = 0x8DC4,
  GL_SAMPLER_CUBE_SHADOW       = 0x8DC5,
  GL_UNSIGNED_INT_VEC2         = 0x8DC6,  // New GLSL types?
  GL_UNSIGNED_INT_VEC3         = 0x8DC7,
  GL_UNSIGNED_INT_VEC4         = 0x8DC8,
  GL_INT_SAMPLER_1D            = 0x8DC9,  // New sampler types
  GL_INT_SAMPLER_2D            = 0x8DCA,
  GL_INT_SAMPLER_3D            = 0x8DCB,
  GL_INT_SAMPLER_CUBE          = 0x8DCC,
  GL_INT_SAMPLER_1D_ARRAY      = 0x8DCE,
  GL_INT_SAMPLER_2D_ARRAY      = 0x8DCF,
  GL_UNSIGNED_INT_SAMPLER_1D   = 0x8DD1,
  GL_UNSIGNED_INT_SAMPLER_2D   = 0x8DD2,
  GL_UNSIGNED_INT_SAMPLER_3D   = 0x8DD3,
  GL_UNSIGNED_INT_SAMPLER_CUBE = 0x8DD4,
  GL_UNSIGNED_INT_SAMPLER_1D_ARRAY = 0x8DD6,
  GL_UNSIGNED_INT_SAMPLER_2D_ARRAY = 0x8DD7,
  
  GL_QUERY_WAIT                = 0x8E13,
  GL_QUERY_NO_WAIT             = 0x8E14,
  GL_QUERY_BY_REGION_WAIT      = 0x8E15,
  GL_QUERY_BY_REGION_NO_WAIT   = 0x8E16,

  GL_BUFFER_ACCESS_FLAGS       = 0x911F,
  GL_BUFFER_MAP_LENGTH         = 0x9120,
  GL_BUFFER_MAP_OFFSET         = 0x9121,
 
  GL_FRAMEBUFFER                   =  0x8D40,
  GL_READ_FRAMEBUFFER              =  0x8CA8,
  GL_DRAW_FRAMEBUFFER              =  0x8CA9,

  GL_RENDERBUFFER                  =  0x8D41,

  GL_STENCIL_INDEX1                =  0x8D46,
  GL_STENCIL_INDEX4                =  0x8D47,
  GL_STENCIL_INDEX8                =  0x8D48,
  GL_STENCIL_INDEX16               =  0x8D49,

  GL_RENDERBUFFER_WIDTH            =  0x8D42,
  GL_RENDERBUFFER_HEIGHT           =  0x8D43,
  GL_RENDERBUFFER_INTERNAL_FORMAT  =  0x8D44,
  GL_RENDERBUFFER_RED_SIZE         =  0x8D50,
  GL_RENDERBUFFER_GREEN_SIZE       =  0x8D51,
  GL_RENDERBUFFER_BLUE_SIZE        =  0x8D52,
  GL_RENDERBUFFER_ALPHA_SIZE       =  0x8D53,
  GL_RENDERBUFFER_DEPTH_SIZE       =  0x8D54,
  GL_RENDERBUFFER_STENCIL_SIZE     =  0x8D55,
  GL_RENDERBUFFER_SAMPLES          =  0x8CAB,

  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE           =  0x8CD0,
  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME           =  0x8CD1,
  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL         =  0x8CD2,
  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE =  0x8CD3,
  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER         =  0x8CD4,
  GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING        =  0x8210,
  GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE        =  0x8211,
  GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE              =  0x8212,
  GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE            =  0x8213,
  GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE             =  0x8214,
  GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE            =  0x8215,
  GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE            =  0x8216,
  GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE          =  0x8217,

  //GL_SRGB                       =  0x8C40,
  //GL_UNSIGNED_NORMALIZED        =  0x8C17,
  GL_FRAMEBUFFER_DEFAULT        =  0x8218,
  GL_INDEX                      =  0x8222,

  GL_COLOR_ATTACHMENT0          =  0x8CE0,
  GL_COLOR_ATTACHMENT1          =  0x8CE1,
  GL_COLOR_ATTACHMENT2          =  0x8CE2,
  GL_COLOR_ATTACHMENT3          =  0x8CE3,
  GL_COLOR_ATTACHMENT4          =  0x8CE4,
  GL_COLOR_ATTACHMENT5          =  0x8CE5,
  GL_COLOR_ATTACHMENT6          =  0x8CE6,
  GL_COLOR_ATTACHMENT7          =  0x8CE7,
  GL_COLOR_ATTACHMENT8          =  0x8CE8,
  GL_COLOR_ATTACHMENT9          =  0x8CE9,
  GL_COLOR_ATTACHMENT10         =  0x8CEA,
  GL_COLOR_ATTACHMENT11         =  0x8CEB,
  GL_COLOR_ATTACHMENT12         =  0x8CEC,
  GL_COLOR_ATTACHMENT13         =  0x8CED,
  GL_COLOR_ATTACHMENT14         =  0x8CEE,
  GL_COLOR_ATTACHMENT15         =  0x8CEF,
  GL_DEPTH_ATTACHMENT           =  0x8D00,
  GL_STENCIL_ATTACHMENT         =  0x8D20,
  GL_DEPTH_STENCIL_ATTACHMENT   =  0x821A,

  GL_MAX_SAMPLES                =  0x8D57,

  GL_FRAMEBUFFER_COMPLETE                      =  0x8CD5,
  GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT         =  0x8CD6,
  GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT =  0x8CD7,
  GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER        =  0x8CDB,
  GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER        =  0x8CDC,
  GL_FRAMEBUFFER_UNSUPPORTED                   =  0x8CDD,
  GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE        =  0x8D56,
  GL_FRAMEBUFFER_UNDEFINED                     =  0x8219,

  //GL_FRAMEBUFFER_BINDING           =  0x8CA6,
  GL_DRAW_FRAMEBUFFER_BINDING      =  0x8CA6,
  GL_READ_FRAMEBUFFER_BINDING      =  0x8CAA,
  GL_RENDERBUFFER_BINDING          =  0x8CA7,
  GL_MAX_COLOR_ATTACHMENTS         =  0x8CDF,
  GL_MAX_RENDERBUFFER_SIZE         =  0x84E8,

  GL_DEPTH_STENCIL                 =  0x84F9,
  GL_UNSIGNED_INT_24_8             =  0x84FA,

  GL_DEPTH24_STENCIL8              =  0x88F0,

  GL_TEXTURE_STENCIL_SIZE          =  0x88F1,

  GL_VERTEX_ARRAY_BINDING          =  0x85B5,
  GL_HALF_FLOAT                    =  0x140B,
  GL_FRAMEBUFFER_SRGB              =  0x8DB9,

  GL_COMPRESSED_RED_RGTC1          = 0x8DBB,
  GL_COMPRESSED_SIGNED_RED_RGTC1   = 0x8DBC,
  GL_COMPRESSED_RG_RGTC2           = 0x8DBD,
  GL_COMPRESSED_SIGNED_RG_RGTC2    = 0x8DBE,
  GL_RG                            = 0x8227,
  GL_RG_INTEGER                    = 0x8228,
  GL_R8                            = 0x8229,
  GL_R16                           = 0x822A,
  GL_RG8                           = 0x822B,
  GL_RG16                          = 0x822C,
  GL_R16F                          = 0x822D,
  GL_R32F                          = 0x822E,
  GL_RG16F                         = 0x822F,
  GL_RG32F                         = 0x8230,
  GL_R8I                           = 0x8231,
  GL_R8UI                          = 0x8232,
  GL_R16I                          = 0x8233,
  GL_R16UI                         = 0x8234,
  GL_R32I                          = 0x8235,
  GL_R32UI                         = 0x8236,
  GL_RG8I                          = 0x8237,
  GL_RG8UI                         = 0x8238,
  GL_RG16I                         = 0x8239,
  GL_RG16UI                        = 0x823A,
  GL_RG32I                         = 0x823B,
  GL_RG32UI                        = 0x823C,

  GL_DEPTH_COMPONENT32F             = 0x8CAC,
  GL_DEPTH32F_STENCIL8              = 0x8CAD,
  GL_FLOAT_32_UNSIGNED_INT_24_8_REV = 0x8DAD,

};

///////////////////////////////////////////////////////////////////////////////
// OpenGL 3.0 functions
///////////////////////////////////////////////////////////////////////////////

void glBeginConditionalRender (GLuint id, GLenum[Main] mode);
void glBeginTransformFeedback  (GLenum[Primitives] primitiveMode);
void glBindBufferBase  (GLenum[Main] target, GLuint index, GLuint buffer);
void glBindBufferRange (GLenum[Main] target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);

//void glBindFragDataLocation (GLuint program, GLuint color, const GLchar * name);
void glBindFragDataLocation (GLuint program, GLuint color, const GLasciistring * name);
void glClampColor (GLenum[Main] target, GLenum[Main] clamp);
void glClearBufferfi (GLenum[Main] buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
void glClearBufferfv (GLenum[Main] buffer, GLint drawbuffer, const GLfloat * value);
void glClearBufferiv (GLenum[Main] buffer, GLint drawbuffer, const GLint * value);
void glClearBufferuiv  (GLenum[Main] buffer, GLint drawbuffer, const GLuint * value);
void glColorMaski (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);

void glDisablei (GLenum[Main] target, GLuint index);
void glEnablei (GLenum[Main] target, GLuint index);

void glEndConditionalRender (void);
void glEndTransformFeedback (void);

void glGetBooleani_v (GLenum[Main] target, GLuint index, GLboolean * data);
//GLint glGetFragDataLocation (GLuint program, const GLchar * name);
GLint glGetFragDataLocation (GLuint program, const GLasciistring * name);
void glGetIntegeri_v  (GLenum[Main] target, GLuint index, GLint * data);
const GLasciistring* glGetStringi (GLenum[Main] name, GLuint index);

void glGetTexParameterIiv (GLenum[Main] target, GLenum[Main] pname, GLint * params);
void glGetTexParameterIuiv (GLenum[Main] target, GLenum[Main] pname, GLuint * params);
void glGetTransformFeedbackVarying (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
void glGetUniformuiv (GLuint program, GLint location, GLuint * params);
void glGetVertexAttribIiv (GLuint index, GLenum[Main] pname, GLint * params);
void glGetVertexAttribIuiv (GLuint index, GLenum[Main] pname, GLuint * params);

GLboolean glIsEnabledi  (GLenum[Main] target, GLuint index);

void glTexParameterIiv (GLenum[Main] target, GLenum[Main] pname, const GLint * params);
void glTexParameterIuiv (GLenum[Main] target, GLenum[Main] pname, const GLuint * params);
void glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar ** varyings, GLenum[Main] bufferMode);

// New uniforms to handle
void glUniform1ui (GLint location, GLuint v0);
void glUniform1uiv (GLint location, GLsizei count, const GLuint* value);
void glUniform2ui (GLint location, GLuint v0, GLuint v1);
void glUniform2uiv (GLint location, GLsizei count, const GLuint* value);
void glUniform3ui (GLint location, GLuint v0, GLuint v1, GLuint v2);
void glUniform3uiv (GLint location, GLsizei count, const GLuint* value);
void glUniform4ui (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void glUniform4uiv (GLint location, GLsizei count, const GLuint* value);

void glVertexAttribI1i (GLuint index, GLint x);
void glVertexAttribI1iv (GLuint index, const GLint* v);
void glVertexAttribI1ui (GLuint index, GLuint x);
void glVertexAttribI1uiv (GLuint index, const GLuint* v);
void glVertexAttribI2i (GLuint index, GLint x, GLint y);
void glVertexAttribI2iv (GLuint index, const GLint* v);
void glVertexAttribI2ui (GLuint index, GLuint x, GLuint y);
void glVertexAttribI2uiv (GLuint index, const GLuint* v);
void glVertexAttribI3i (GLuint index, GLint x, GLint y, GLint z);
void glVertexAttribI3iv (GLuint index, const GLint* v);
void glVertexAttribI3ui (GLuint index, GLuint x, GLuint y, GLuint z);
void glVertexAttribI3uiv (GLuint index, const GLuint* v);
void glVertexAttribI4bv (GLuint index, const GLbyte* v);
void glVertexAttribI4i (GLuint index, GLint x, GLint y, GLint z, GLint w);
void glVertexAttribI4iv (GLuint index, const GLint* v);
void glVertexAttribI4sv (GLuint index, const GLshort* v);
void glVertexAttribI4ubv (GLuint index, const GLubyte* v);
void glVertexAttribI4ui (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void glVertexAttribI4uiv (GLuint index, const GLuint* v);
void glVertexAttribI4usv (GLuint index, const GLushort* v);
void glVertexAttribIPointer (GLuint index, GLint size, GLenum[Main] type, GLsizei stride, const GLvoid * pointer);

// FBO
GLboolean glIsRenderbuffer(GLuint renderbuffer);
void glBindRenderbuffer(GLenum[Main] target, GLuint renderbuffer);
void glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
void glGenRenderbuffers(GLsizei n, GLuint *renderbuffers);

void glRenderbufferStorage(GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLsizei height);

void glRenderbufferStorageMultisample(GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height);

void glGetRenderbufferParameteriv(GLenum[Main] target, GLenum[Main] pname, GLint *params);

GLboolean glIsFramebuffer(GLuint framebuffer);
void glBindFramebuffer(GLenum[Main] target, GLuint framebuffer);
void glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
void glGenFramebuffers(GLsizei n, GLuint *framebuffers);

GLenum[Main] glCheckFramebufferStatus(GLenum[Main] target);

void glFramebufferTexture1D(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level);
void glFramebufferTexture2D(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level);
void glFramebufferTexture3D(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level, GLint layer);
void glFramebufferTextureLayer(GLenum[Main] target,GLenum[Main] attachment, GLuint texture, GLint level, GLint layer);

void glFramebufferRenderbuffer(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] renderbuffertarget, GLuint renderbuffer);

void glGetFramebufferAttachmentParameteriv(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] pname, GLint *params);

void glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum[Main] filter);

void glGenerateMipmap(GLenum[Main] target);

// Map buffer range
void *glMapBufferRange( GLenum[Main] target, GLintptr offset, GLsizeiptr length, GLbitfield[Mask_Access] access);
void glFlushMappedBufferRange( GLenum[Main] target, GLintptr offset, GLsizeiptr length );

// VAO
void glBindVertexArray(GLuint array);
void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
void glGenVertexArrays(GLsizei n, GLuint *arrays);
GLboolean glIsVertexArray(GLuint array);
