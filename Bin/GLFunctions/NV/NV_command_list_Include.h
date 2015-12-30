#define GLI_INCLUDE_GL_NV_COMMAND_LIST

enum Main {

  //GL_TERMINATE_SEQUENCE_COMMAND_NV            = 0x0000,
  //GL_NOP_COMMAND_NV                           = 0x0001,
  //GL_DRAW_ELEMENTS_COMMAND_NV                 = 0x0002,
  //GL_DRAW_ARRAYS_COMMAND_NV                   = 0x0003,
  //GL_DRAW_ELEMENTS_STRIP_COMMAND_NV           = 0x0004,
  //GL_DRAW_ARRAYS_STRIP_COMMAND_NV             = 0x0005,
  //GL_DRAW_ELEMENTS_INSTANCED_COMMAND_NV       = 0x0006,
  //GL_DRAW_ARRAYS_INSTANCED_COMMAND_NV         = 0x0007,
  //GL_ELEMENT_ADDRESS_COMMAND_NV               = 0x0008,
  //GL_ATTRIBUTE_ADDRESS_COMMAND_NV             = 0x0009,
  //GL_UNIFORM_ADDRESS_COMMAND_NV               = 0x000A,
  //GL_BLEND_COLOR_COMMAND_NV                   = 0x000B,
  //GL_STENCIL_REF_COMMAND_NV                   = 0x000C,
  //GL_LINE_WIDTH_COMMAND_NV                    = 0x000D,
  //GL_POLYGON_OFFSET_COMMAND_NV                = 0x000E,
  //GL_ALPHA_REF_COMMAND_NV                     = 0x000F,
  //GL_VIEWPORT_COMMAND_NV                      = 0x0010,
  //GL_SCISSOR_COMMAND_NV                       = 0x0011,
  //GL_FRONT_FACE_COMMAND_NV                    = 0x0012,

};

void glCreateStatesNV(GLsizei n, GLuint *states);
void glDeleteStatesNV(GLsizei n, const GLuint *states);
GLboolean glIsStateNV(GLuint state);
void glStateCaptureNV(GLuint state, GLenum[Main] mode);
GLuint glGetCommandHeaderNV(GLenum[Main] tokenID, GLuint size);
GLushort glGetStageIndexNV(GLenum[Main] shadertype);
void glDrawCommandsNV(GLenum[Main] primitiveMode, GLuint buffer, const GLintptr *indirects, const GLsizei *sizes, GLuint count);
void glDrawCommandsAddressNV(GLenum[Main] primitiveMode, const GLuint64 *indirects, const GLsizei *sizes, GLuint count);
void glDrawCommandsStatesNV(GLuint buffer, const GLintptr *indirects, const GLsizei *sizes, const GLuint *states, const GLuint *fbos, GLuint count);
void glDrawCommandsStatesAddressNV(const GLuint64 *indirects, const GLsizei *sizes, const GLuint *states, const GLuint *fbos, GLuint count);
void glCreateCommandListsNV(GLsizei n, GLuint *lists);
void glDeleteCommandListsNV(GLsizei n, const GLuint *lists);
GLboolean glIsCommandListNV(GLuint list);
void glListDrawCommandsStatesClientNV(GLuint list, GLuint segment, const void **indirects, const GLsizei *sizes, const GLuint *states, const GLuint *fbos, GLuint count);
void glCommandListSegmentsNV(GLuint list, GLuint segments);
void glCompileCommandListNV(GLuint list);
void glCallCommandListNV(GLuint list);
