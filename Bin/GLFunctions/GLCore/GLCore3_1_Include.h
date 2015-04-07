#define GLI_INCLUDE_GLCORE3_1

///////////////////////////////////////////////////////////////////////////////
// Masks
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Error enum
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Main enum
///////////////////////////////////////////////////////////////////////////////

// Main enum for all standard enums
//  (may want to break up for speed)
enum Main {

  GL_SAMPLER_2D_RECT                    = 0x8B63, // new samplers
  GL_SAMPLER_2D_RECT_SHADOW             = 0x8B64,
  GL_SAMPLER_BUFFER                     = 0x8DC2, // new sampler buffer type
  GL_INT_SAMPLER_2D_RECT                = 0x8DCD,
  GL_INT_SAMPLER_BUFFER                 = 0x8DD0,
  GL_UNSIGNED_INT_SAMPLER_2D_RECT       = 0x8DD5, // new samplers
  GL_UNSIGNED_INT_SAMPLER_BUFFER        = 0x8DD8,
  //GL_TEXTURE_BUFFER                     = 0x8C2A, // texture buffer
  GL_MAX_TEXTURE_BUFFER_SIZE            = 0x8C2B,
  GL_TEXTURE_BINDING_BUFFER             = 0x8C2C,
  GL_TEXTURE_BUFFER_DATA_STORE_BINDING  = 0x8C2D,
  GL_TEXTURE_BUFFER_FORMAT              = 0x8C2E,
  GL_TEXTURE_RECTANGLE                  = 0x84F5,
  GL_TEXTURE_BINDING_RECTANGLE          = 0x84F6,
  GL_PROXY_TEXTURE_RECTANGLE            = 0x84F7,
  GL_MAX_RECTANGLE_TEXTURE_SIZE         = 0x84F8,

  GL_RED_SNORM                          = 0x8F90,
  GL_RG_SNORM                           = 0x8F91,
  GL_RGB_SNORM                          = 0x8F92,
  GL_RGBA_SNORM                         = 0x8F93,
  GL_R8_SNORM                           = 0x8F94,
  GL_RG8_SNORM                          = 0x8F95,
  GL_RGB8_SNORM                         = 0x8F96,
  GL_RGBA8_SNORM                        = 0x8F97,
  GL_R16_SNORM                          = 0x8F98,
  GL_RG16_SNORM                         = 0x8F99,
  GL_RGB16_SNORM                        = 0x8F9A,
  GL_RGBA16_SNORM                       = 0x8F9B,
  GL_SIGNED_NORMALIZED                  = 0x8F9C,
  GL_PRIMITIVE_RESTART                  = 0x8F9D,
  GL_PRIMITIVE_RESTART_INDEX            = 0x8F9E,
  
  GL_COPY_READ_BUFFER                   = 0x8F36,
  GL_COPY_WRITE_BUFFER                  = 0x8F37,

  GL_UNIFORM_BUFFER                                =  0x8A11,
  GL_UNIFORM_BUFFER_BINDING                        =  0x8A28,

  GL_UNIFORM_BUFFER_START                          =  0x8A29,
  GL_UNIFORM_BUFFER_SIZE                           =  0x8A2A,

  GL_MAX_VERTEX_UNIFORM_BLOCKS                     =  0x8A2B,
  GL_MAX_GEOMETRY_UNIFORM_BLOCKS                   =  0x8A2C,
  GL_MAX_FRAGMENT_UNIFORM_BLOCKS                   =  0x8A2D,
  GL_MAX_COMBINED_UNIFORM_BLOCKS                   =  0x8A2E,
  GL_MAX_UNIFORM_BUFFER_BINDINGS                   =  0x8A2F,
  GL_MAX_UNIFORM_BLOCK_SIZE                        =  0x8A30,
  GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS        =  0x8A31,
  GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS      =  0x8A32,
  GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS      =  0x8A33,
  GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT               =  0x8A34,

  GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH          =  0x8A35,
  GL_ACTIVE_UNIFORM_BLOCKS                         =  0x8A36,

  GL_UNIFORM_TYPE                                  =  0x8A37,
  GL_UNIFORM_SIZE                                  =  0x8A38,
  GL_UNIFORM_NAME_LENGTH                           =  0x8A39,
  GL_UNIFORM_BLOCK_INDEX                           =  0x8A3A,
  GL_UNIFORM_OFFSET                                =  0x8A3B,
  GL_UNIFORM_ARRAY_STRIDE                          =  0x8A3C,
  GL_UNIFORM_MATRIX_STRIDE                         =  0x8A3D,
  GL_UNIFORM_IS_ROW_MAJOR                          =  0x8A3E,

  GL_UNIFORM_BLOCK_BINDING                         =  0x8A3F,
  GL_UNIFORM_BLOCK_DATA_SIZE                       =  0x8A40,
  GL_UNIFORM_BLOCK_NAME_LENGTH                     =  0x8A41,
  GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS                 =  0x8A42,
  GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES          =  0x8A43,
  GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER     =  0x8A44,
  GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER   =  0x8A45,
  GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER   =  0x8A46,

};

///////////////////////////////////////////////////////////////////////////////
// OpenGL 3.1 functions
///////////////////////////////////////////////////////////////////////////////

void glDrawArraysInstanced (GLenum[Primitives] mode, GLint first, GLsizei count, GLsizei primcount);
void glDrawElementsInstanced (GLenum[Primitives] mode, GLsizei count, GLenum[Main] type, const GLvoid *indices, GLsizei primcount);
void glTexBuffer (GLenum[Main] target, GLenum[Main] internalformat, GLuint buffer);
void glPrimitiveRestartIndex (GLuint index);

void glCopyBufferSubData(GLenum[Main] readtarget, GLenum[Main] writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);

void glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar** uniformNames, GLuint* uniformIndices);

void glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum[Main] pname, GLint* params);
void glGetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);

GLuint glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName);

void glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum[Main] pname, GLint* params);
void glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);

void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
