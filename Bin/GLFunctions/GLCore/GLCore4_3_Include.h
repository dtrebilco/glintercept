#define GLI_INCLUDE_GLCORE4_3

enum Main {

  //GL_STACK_OVERFLOW                                           = 0x0503,
  //GL_STACK_UNDERFLOW                                          = 0x0504,
  GL_NUM_SHADING_LANGUAGE_VERSIONS                            = 0x82E9,
  GL_VERTEX_ATTRIB_ARRAY_LONG                                 = 0x874E,
  GL_COMPRESSED_RGB8_ETC2                                     = 0x9274,
  GL_COMPRESSED_SRGB8_ETC2                                    = 0x9275,
  GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2                 = 0x9276,
  GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2                = 0x9277,
  GL_COMPRESSED_RGBA8_ETC2_EAC                                = 0x9278,
  GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC                         = 0x9279,
  GL_COMPRESSED_R11_EAC                                       = 0x9270,
  GL_COMPRESSED_SIGNED_R11_EAC                                = 0x9271,
  GL_COMPRESSED_RG11_EAC                                      = 0x9272,
  GL_COMPRESSED_SIGNED_RG11_EAC                               = 0x9273,
  GL_PRIMITIVE_RESTART_FIXED_INDEX                            = 0x8D69,
  GL_ANY_SAMPLES_PASSED_CONSERVATIVE                          = 0x8D6A,
  GL_MAX_ELEMENT_INDEX                                        = 0x8D6B,
  GL_COMPUTE_SHADER                                           = 0x91B9,
  GL_MAX_COMPUTE_UNIFORM_BLOCKS                               = 0x91BB,
  GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS                          = 0x91BC,
  GL_MAX_COMPUTE_IMAGE_UNIFORMS                               = 0x91BD,
  GL_MAX_COMPUTE_SHARED_MEMORY_SIZE                           = 0x8262,
  GL_MAX_COMPUTE_UNIFORM_COMPONENTS                           = 0x8263,
  GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS                       = 0x8264,
  GL_MAX_COMPUTE_ATOMIC_COUNTERS                              = 0x8265,
  GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS                  = 0x8266,
  GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS                       = 0x90EB,
  GL_MAX_COMPUTE_WORK_GROUP_COUNT                             = 0x91BE,
  GL_MAX_COMPUTE_WORK_GROUP_SIZE                              = 0x91BF,
  GL_COMPUTE_WORK_GROUP_SIZE                                  = 0x8267,
  GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER               = 0x90EC,
  GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER       = 0x90ED,
  GL_DISPATCH_INDIRECT_BUFFER                                 = 0x90EE,
  GL_DISPATCH_INDIRECT_BUFFER_BINDING                         = 0x90EF,
  GL_DEBUG_OUTPUT_SYNCHRONOUS                                 = 0x8242,
  GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH                         = 0x8243,
  GL_DEBUG_CALLBACK_FUNCTION                                  = 0x8244,
  GL_DEBUG_CALLBACK_USER_PARAM                                = 0x8245,
  GL_DEBUG_SOURCE_API                                         = 0x8246,
  GL_DEBUG_SOURCE_WINDOW_SYSTEM                               = 0x8247,
  GL_DEBUG_SOURCE_SHADER_COMPILER                             = 0x8248,
  GL_DEBUG_SOURCE_THIRD_PARTY                                 = 0x8249,
  GL_DEBUG_SOURCE_APPLICATION                                 = 0x824A,
  GL_DEBUG_SOURCE_OTHER                                       = 0x824B,
  GL_DEBUG_TYPE_ERROR                                         = 0x824C,
  GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR                           = 0x824D,
  GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR                            = 0x824E,
  GL_DEBUG_TYPE_PORTABILITY                                   = 0x824F,
  GL_DEBUG_TYPE_PERFORMANCE                                   = 0x8250,
  GL_DEBUG_TYPE_OTHER                                         = 0x8251,
  GL_MAX_DEBUG_MESSAGE_LENGTH                                 = 0x9143,
  GL_MAX_DEBUG_LOGGED_MESSAGES                                = 0x9144,
  GL_DEBUG_LOGGED_MESSAGES                                    = 0x9145,
  GL_DEBUG_SEVERITY_HIGH                                      = 0x9146,
  GL_DEBUG_SEVERITY_MEDIUM                                    = 0x9147,
  GL_DEBUG_SEVERITY_LOW                                       = 0x9148,
  GL_DEBUG_TYPE_MARKER                                        = 0x8268,
  GL_DEBUG_TYPE_PUSH_GROUP                                    = 0x8269,
  GL_DEBUG_TYPE_POP_GROUP                                     = 0x826A,
  GL_DEBUG_SEVERITY_NOTIFICATION                              = 0x826B,
  GL_MAX_DEBUG_GROUP_STACK_DEPTH                              = 0x826C,
  GL_DEBUG_GROUP_STACK_DEPTH                                  = 0x826D,
  GL_BUFFER                                                   = 0x82E0,
  GL_SHADER                                                   = 0x82E1,
  GL_PROGRAM                                                  = 0x82E2,
  GL_QUERY                                                    = 0x82E3,
  GL_PROGRAM_PIPELINE                                         = 0x82E4,
  GL_SAMPLER                                                  = 0x82E6,
  GL_DISPLAY_LIST                                             = 0x82E7,
  GL_MAX_LABEL_LENGTH                                         = 0x82E8,
  GL_DEBUG_OUTPUT                                             = 0x92E0,
  GL_MAX_UNIFORM_LOCATIONS                                    = 0x826E,
  GL_FRAMEBUFFER_DEFAULT_WIDTH                                = 0x9310,
  GL_FRAMEBUFFER_DEFAULT_HEIGHT                               = 0x9311,
  GL_FRAMEBUFFER_DEFAULT_LAYERS                               = 0x9312,
  GL_FRAMEBUFFER_DEFAULT_SAMPLES                              = 0x9313,
  GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS               = 0x9314,
  GL_MAX_FRAMEBUFFER_WIDTH                                    = 0x9315,
  GL_MAX_FRAMEBUFFER_HEIGHT                                   = 0x9316,
  GL_MAX_FRAMEBUFFER_LAYERS                                   = 0x9317,
  GL_MAX_FRAMEBUFFER_SAMPLES                                  = 0x9318,
  GL_INTERNALFORMAT_SUPPORTED                                 = 0x826F,
  GL_INTERNALFORMAT_PREFERRED                                 = 0x8270,
  GL_INTERNALFORMAT_RED_SIZE                                  = 0x8271,
  GL_INTERNALFORMAT_GREEN_SIZE                                = 0x8272,
  GL_INTERNALFORMAT_BLUE_SIZE                                 = 0x8273,
  GL_INTERNALFORMAT_ALPHA_SIZE                                = 0x8274,
  GL_INTERNALFORMAT_DEPTH_SIZE                                = 0x8275,
  GL_INTERNALFORMAT_STENCIL_SIZE                              = 0x8276,
  GL_INTERNALFORMAT_SHARED_SIZE                               = 0x8277,
  GL_INTERNALFORMAT_RED_TYPE                                  = 0x8278,
  GL_INTERNALFORMAT_GREEN_TYPE                                = 0x8279,
  GL_INTERNALFORMAT_BLUE_TYPE                                 = 0x827A,
  GL_INTERNALFORMAT_ALPHA_TYPE                                = 0x827B,
  GL_INTERNALFORMAT_DEPTH_TYPE                                = 0x827C,
  GL_INTERNALFORMAT_STENCIL_TYPE                              = 0x827D,
  GL_MAX_WIDTH                                                = 0x827E,
  GL_MAX_HEIGHT                                               = 0x827F,
  GL_MAX_DEPTH                                                = 0x8280,
  GL_MAX_LAYERS                                               = 0x8281,
  GL_MAX_COMBINED_DIMENSIONS                                  = 0x8282,
  GL_COLOR_COMPONENTS                                         = 0x8283,
  GL_DEPTH_COMPONENTS                                         = 0x8284,
  GL_STENCIL_COMPONENTS                                       = 0x8285,
  GL_COLOR_RENDERABLE                                         = 0x8286,
  GL_DEPTH_RENDERABLE                                         = 0x8287,
  GL_STENCIL_RENDERABLE                                       = 0x8288,
  GL_FRAMEBUFFER_RENDERABLE                                   = 0x8289,
  GL_FRAMEBUFFER_RENDERABLE_LAYERED                           = 0x828A,
  GL_FRAMEBUFFER_BLEND                                        = 0x828B,
  GL_READ_PIXELS                                              = 0x828C,
  GL_READ_PIXELS_FORMAT                                       = 0x828D,
  GL_READ_PIXELS_TYPE                                         = 0x828E,
  GL_TEXTURE_IMAGE_FORMAT                                     = 0x828F,
  GL_TEXTURE_IMAGE_TYPE                                       = 0x8290,
  GL_GET_TEXTURE_IMAGE_FORMAT                                 = 0x8291,
  GL_GET_TEXTURE_IMAGE_TYPE                                   = 0x8292,
  GL_MIPMAP                                                   = 0x8293,
  GL_MANUAL_GENERATE_MIPMAP                                   = 0x8294,
  GL_AUTO_GENERATE_MIPMAP                                     = 0x8295,
  GL_COLOR_ENCODING                                           = 0x8296,
  GL_SRGB_READ                                                = 0x8297,
  GL_SRGB_WRITE                                               = 0x8298,
  GL_FILTER                                                   = 0x829A,
  GL_VERTEX_TEXTURE                                           = 0x829B,
  GL_TESS_CONTROL_TEXTURE                                     = 0x829C,
  GL_TESS_EVALUATION_TEXTURE                                  = 0x829D,
  GL_GEOMETRY_TEXTURE                                         = 0x829E,
  GL_FRAGMENT_TEXTURE                                         = 0x829F,
  GL_COMPUTE_TEXTURE                                          = 0x82A0,
  GL_TEXTURE_SHADOW                                           = 0x82A1,
  GL_TEXTURE_GATHER                                           = 0x82A2,
  GL_TEXTURE_GATHER_SHADOW                                    = 0x82A3,
  GL_SHADER_IMAGE_LOAD                                        = 0x82A4,
  GL_SHADER_IMAGE_STORE                                       = 0x82A5,
  GL_SHADER_IMAGE_ATOMIC                                      = 0x82A6,
  GL_IMAGE_TEXEL_SIZE                                         = 0x82A7,
  GL_IMAGE_COMPATIBILITY_CLASS                                = 0x82A8,
  GL_IMAGE_PIXEL_FORMAT                                       = 0x82A9,
  GL_IMAGE_PIXEL_TYPE                                         = 0x82AA,
  GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST                      = 0x82AC,
  GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST                    = 0x82AD,
  GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE                     = 0x82AE,
  GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE                   = 0x82AF,
  GL_TEXTURE_COMPRESSED_BLOCK_WIDTH                           = 0x82B1,
  GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT                          = 0x82B2,
  GL_TEXTURE_COMPRESSED_BLOCK_SIZE                            = 0x82B3,
  GL_CLEAR_BUFFER                                             = 0x82B4,
  GL_TEXTURE_VIEW                                             = 0x82B5,
  GL_VIEW_COMPATIBILITY_CLASS                                 = 0x82B6,
  GL_FULL_SUPPORT                                             = 0x82B7,
  GL_CAVEAT_SUPPORT                                           = 0x82B8,
  GL_IMAGE_CLASS_4_X_32                                       = 0x82B9,
  GL_IMAGE_CLASS_2_X_32                                       = 0x82BA,
  GL_IMAGE_CLASS_1_X_32                                       = 0x82BB,
  GL_IMAGE_CLASS_4_X_16                                       = 0x82BC,
  GL_IMAGE_CLASS_2_X_16                                       = 0x82BD,
  GL_IMAGE_CLASS_1_X_16                                       = 0x82BE,
  GL_IMAGE_CLASS_4_X_8                                        = 0x82BF,
  GL_IMAGE_CLASS_2_X_8                                        = 0x82C0,
  GL_IMAGE_CLASS_1_X_8                                        = 0x82C1,
  GL_IMAGE_CLASS_11_11_10                                     = 0x82C2,
  GL_IMAGE_CLASS_10_10_10_2                                   = 0x82C3,
  GL_VIEW_CLASS_128_BITS                                      = 0x82C4,
  GL_VIEW_CLASS_96_BITS                                       = 0x82C5,
  GL_VIEW_CLASS_64_BITS                                       = 0x82C6,
  GL_VIEW_CLASS_48_BITS                                       = 0x82C7,
  GL_VIEW_CLASS_32_BITS                                       = 0x82C8,
  GL_VIEW_CLASS_24_BITS                                       = 0x82C9,
  GL_VIEW_CLASS_16_BITS                                       = 0x82CA,
  GL_VIEW_CLASS_8_BITS                                        = 0x82CB,
  GL_VIEW_CLASS_S3TC_DXT1_RGB                                 = 0x82CC,
  GL_VIEW_CLASS_S3TC_DXT1_RGBA                                = 0x82CD,
  GL_VIEW_CLASS_S3TC_DXT3_RGBA                                = 0x82CE,
  GL_VIEW_CLASS_S3TC_DXT5_RGBA                                = 0x82CF,
  GL_VIEW_CLASS_RGTC1_RED                                     = 0x82D0,
  GL_VIEW_CLASS_RGTC2_RG                                      = 0x82D1,
  GL_VIEW_CLASS_BPTC_UNORM                                    = 0x82D2,
  GL_VIEW_CLASS_BPTC_FLOAT                                    = 0x82D3,
  GL_UNIFORM                                                  = 0x92E1,
  GL_UNIFORM_BLOCK                                            = 0x92E2,
  GL_PROGRAM_INPUT                                            = 0x92E3,
  GL_PROGRAM_OUTPUT                                           = 0x92E4,
  GL_BUFFER_VARIABLE                                          = 0x92E5,
  GL_SHADER_STORAGE_BLOCK                                     = 0x92E6,
  GL_VERTEX_SUBROUTINE                                        = 0x92E8,
  GL_TESS_CONTROL_SUBROUTINE                                  = 0x92E9,
  GL_TESS_EVALUATION_SUBROUTINE                               = 0x92EA,
  GL_GEOMETRY_SUBROUTINE                                      = 0x92EB,
  GL_FRAGMENT_SUBROUTINE                                      = 0x92EC,
  GL_COMPUTE_SUBROUTINE                                       = 0x92ED,
  GL_VERTEX_SUBROUTINE_UNIFORM                                = 0x92EE,
  GL_TESS_CONTROL_SUBROUTINE_UNIFORM                          = 0x92EF,
  GL_TESS_EVALUATION_SUBROUTINE_UNIFORM                       = 0x92F0,
  GL_GEOMETRY_SUBROUTINE_UNIFORM                              = 0x92F1,
  GL_FRAGMENT_SUBROUTINE_UNIFORM                              = 0x92F2,
  GL_COMPUTE_SUBROUTINE_UNIFORM                               = 0x92F3,
  GL_TRANSFORM_FEEDBACK_VARYING                               = 0x92F4,
  GL_ACTIVE_RESOURCES                                         = 0x92F5,
  GL_MAX_NAME_LENGTH                                          = 0x92F6,
  GL_MAX_NUM_ACTIVE_VARIABLES                                 = 0x92F7,
  GL_MAX_NUM_COMPATIBLE_SUBROUTINES                           = 0x92F8,
  GL_NAME_LENGTH                                              = 0x92F9,
  GL_TYPE                                                     = 0x92FA,
  GL_ARRAY_SIZE                                               = 0x92FB,
  GL_OFFSET                                                   = 0x92FC,
  GL_BLOCK_INDEX                                              = 0x92FD,
  GL_ARRAY_STRIDE                                             = 0x92FE,
  GL_MATRIX_STRIDE                                            = 0x92FF,
  GL_IS_ROW_MAJOR                                             = 0x9300,
  GL_ATOMIC_COUNTER_BUFFER_INDEX                              = 0x9301,
  GL_BUFFER_BINDING                                           = 0x9302,
  GL_BUFFER_DATA_SIZE                                         = 0x9303,
  GL_NUM_ACTIVE_VARIABLES                                     = 0x9304,
  GL_ACTIVE_VARIABLES                                         = 0x9305,
  GL_REFERENCED_BY_VERTEX_SHADER                              = 0x9306,
  GL_REFERENCED_BY_TESS_CONTROL_SHADER                        = 0x9307,
  GL_REFERENCED_BY_TESS_EVALUATION_SHADER                     = 0x9308,
  GL_REFERENCED_BY_GEOMETRY_SHADER                            = 0x9309,
  GL_REFERENCED_BY_FRAGMENT_SHADER                            = 0x930A,
  GL_REFERENCED_BY_COMPUTE_SHADER                             = 0x930B,
  GL_TOP_LEVEL_ARRAY_SIZE                                     = 0x930C,
  GL_TOP_LEVEL_ARRAY_STRIDE                                   = 0x930D,
  GL_LOCATION                                                 = 0x930E,
  GL_LOCATION_INDEX                                           = 0x930F,
  GL_IS_PER_PATCH                                             = 0x92E7,
  GL_SHADER_STORAGE_BUFFER                                    = 0x90D2,
  GL_SHADER_STORAGE_BUFFER_BINDING                            = 0x90D3,
  GL_SHADER_STORAGE_BUFFER_START                              = 0x90D4,
  GL_SHADER_STORAGE_BUFFER_SIZE                               = 0x90D5,
  GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS                         = 0x90D6,
  GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS                       = 0x90D7,
  GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS                   = 0x90D8,
  GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS                = 0x90D9,
  GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS                       = 0x90DA,
  GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS                        = 0x90DB,
  GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS                       = 0x90DC,
  GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS                       = 0x90DD,
  GL_MAX_SHADER_STORAGE_BLOCK_SIZE                            = 0x90DE,
  GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT                   = 0x90DF,
//  GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES                     = MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS,
  GL_DEPTH_STENCIL_TEXTURE_MODE                               = 0x90EA,
  GL_TEXTURE_BUFFER_OFFSET                                    = 0x919D,
  GL_TEXTURE_BUFFER_SIZE                                      = 0x919E,
  GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT                          = 0x919F,
  GL_TEXTURE_VIEW_MIN_LEVEL                                   = 0x82DB,
  GL_TEXTURE_VIEW_NUM_LEVELS                                  = 0x82DC,
  GL_TEXTURE_VIEW_MIN_LAYER                                   = 0x82DD,
  GL_TEXTURE_VIEW_NUM_LAYERS                                  = 0x82DE,
  GL_TEXTURE_IMMUTABLE_LEVELS                                 = 0x82DF,
  GL_VERTEX_ATTRIB_BINDING                                    = 0x82D4,
  GL_VERTEX_ATTRIB_RELATIVE_OFFSET                            = 0x82D5,
  GL_VERTEX_BINDING_DIVISOR                                   = 0x82D6,
  GL_VERTEX_BINDING_OFFSET                                    = 0x82D7,
  GL_VERTEX_BINDING_STRIDE                                    = 0x82D8,
  GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET                        = 0x82D9,
  GL_MAX_VERTEX_ATTRIB_BINDINGS                               = 0x82DA,
  GL_SRGB_DECODE_ARB                                          = 0x8299,
  GL_VERTEX_BINDING_BUFFER                                    = 0x8F4F,
};


enum Mask_ShaderProgramStages {

  GL_COMPUTE_SHADER_BIT                                       = 0x00000020,

};

enum Mask_MemBarrierMask {

  GL_SHADER_STORAGE_BARRIER_BIT                               = 0x2000,

};

//GL_CONTEXT_FLAG_DEBUG_BIT                                   = 0x00000002,

void glDebugMessageControl(GLenum[Main] source, GLenum[Main] type, GLenum[Main] severity, GLsizei count, const GLuint * ids, GLboolean enabled);
void glDebugMessageInsert(GLenum[Main] source, GLenum[Main] type, GLuint id, GLenum[Main] severity, GLsizei length, const GLchar * buf);
//void glDebugMessageCallback(GLDEBUGPROC callback, const void * userParam);
void glDebugMessageCallback(GLvoid* callback, const void * userParam);
GLuint glGetDebugMessageLog(GLuint count, GLsizei bufsize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog);
void glPushDebugGroup(GLenum[Main] source, GLuint id, GLsizei length, const GLchar * message);
void glPopDebugGroup(void);

void glObjectLabel(GLenum[Main] identifier, GLuint name, GLsizei length, const GLchar * label);
void glGetObjectLabel(GLenum[Main] identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label);
void glObjectPtrLabel(const void * ptr, GLsizei length, const GLchar * label);
void glGetObjectPtrLabel(const void * ptr, GLsizei bufSize, GLsizei * length, GLchar * label);

void glClearBufferData(GLenum[Main] target, GLenum[Main] internalformat, GLenum[Main] format, GLenum[Main] type, const void * data);
void glClearBufferSubData(GLenum[Main] target, GLenum[Main] internalformat, GLintptr offset, GLsizeiptr size, GLenum[Main] format, GLenum[Main] type, const void * data);

void glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
void glDispatchComputeIndirect(GLintptr indirect);

void glCopyImageSubData(GLuint srcName, GLenum[Main] srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum[Main] dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
void glTextureView(GLuint texture, GLenum[Main] target, GLuint origtexture, GLenum[Main] internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
void glBindVertexBuffer(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void glVertexAttribFormat(GLuint attribindex, GLint size, GLenum[Main] type, GLboolean normalized, GLuint relativeoffset);
void glVertexAttribIFormat(GLuint attribindex, GLint size, GLenum[Main] type, GLuint relativeoffset);
void glVertexAttribLFormat(GLuint attribindex, GLint size, GLenum[Main] type, GLuint relativeoffset);
void glVertexAttribBinding(GLuint attribindex, GLuint bindingindex);
void glVertexBindingDivisor(GLuint bindingindex, GLuint divisor);

void glFramebufferParameteri(GLenum[Main] target, GLenum[Main] pname, GLint param);
void glGetFramebufferParameteriv(GLenum[Main] target, GLenum[Main] pname, GLint * params);
void glGetInternalformati64v(GLenum[Main] target, GLenum[Main] internalformat, GLenum[Main] pname, GLsizei bufSize, GLint64 * params);

void glInvalidateTexSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
void glInvalidateTexImage(GLuint texture, GLint level);
void glInvalidateBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr length);
void glInvalidateBufferData(GLuint buffer);
void glInvalidateFramebuffer(GLenum[Main] target, GLsizei numAttachments, const GLenum * attachments);
void glInvalidateSubFramebuffer(GLenum[Main] target, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height);

void glMultiDrawArraysIndirect(GLenum[Main] mode, const void * indirect, GLsizei drawcount, GLsizei stride);
void glMultiDrawElementsIndirect(GLenum[Main] mode, GLenum[Main] type, const void * indirect, GLsizei drawcount, GLsizei stride);

void glGetProgramInterfaceiv(GLuint program, GLenum[Main] programInterface, GLenum[Main] pname, GLint * params);
GLuint glGetProgramResourceIndex(GLuint program, GLenum[Main] programInterface, const GLchar * name);
void glGetProgramResourceName(GLuint program, GLenum[Main] programInterface, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name);
void glGetProgramResourceiv(GLuint program, GLenum[Main] programInterface, GLuint index, GLsizei propCount, const GLenum * props, GLsizei bufSize, GLsizei * length, GLint * params);
GLint glGetProgramResourceLocation(GLuint program, GLenum[Main] programInterface, const GLchar * name);
GLint glGetProgramResourceLocationIndex(GLuint program, GLenum[Main] programInterface, const GLchar * name);

void glShaderStorageBlockBinding(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);

void glTexBufferRange(GLenum[Main] target, GLenum[Main] internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glTexStorage2DMultisample(GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void glTexStorage3DMultisample(GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);


void glVertexArrayBindVertexBufferEXT(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void glVertexArrayVertexAttribFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum[Main] type, GLboolean normalized, GLuint relativeoffset);
void glVertexArrayVertexAttribIFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum[Main] type, GLuint relativeoffset);
void glVertexArrayVertexAttribLFormatEXT(GLuint vaobj, GLuint attribindex, GLint size, GLenum[Main] type, GLuint relativeoffset);
void glVertexArrayVertexAttribBindingEXT(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
void glVertexArrayVertexBindingDivisorEXT(GLuint vaobj, GLuint bindingindex, GLuint divisor);

void glNamedFramebufferParameteriEXT(GLuint framebuffer, GLenum[Main] pname, GLint param);
void glGetNamedFramebufferParameterivEXT(GLuint framebuffer, GLenum[Main] pname, GLint * params);

void glClearNamedBufferDataEXT(GLuint buffer, GLenum[Main] internalformat, GLenum[Main] format, GLenum[Main] type, const void * data);
void glClearNamedBufferSubDataEXT(GLuint buffer, GLenum[Main] internalformat, GLsizeiptr offset, GLsizeiptr size, GLenum[Main] format, GLenum[Main] type, const void * data);

void glTextureBufferRangeEXT(GLuint texture, GLenum[Main] target, GLenum[Main] internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glTextureStorage2DMultisampleEXT(GLuint texture, GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void glTextureStorage3DMultisampleEXT(GLuint texture, GLenum[Main] target, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);

