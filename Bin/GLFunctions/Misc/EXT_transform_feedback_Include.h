#define GLI_INCLUDE_GL_EXT_TRANSFORM_FEEDBACK

enum Main {

  //GL_TRANSFORM_FEEDBACK_BUFFER_EXT                           = 0x8C8E,
  //GL_TRANSFORM_FEEDBACK_BUFFER_START_EXT                     = 0x8C84,
  //GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_EXT                      = 0x8C85,
  //GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_EXT                   = 0x8C8F,
  //GL_INTERLEAVED_ATTRIBS_EXT                                 = 0x8C8C,
  //GL_SEPARATE_ATTRIBS_EXT                                    = 0x8C8D,
  //GL_PRIMITIVES_GENERATED_EXT                                = 0x8C87,
  //GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_EXT               = 0x8C88,
  //GL_RASTERIZER_DISCARD_EXT                                  = 0x8C89,
  //GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_EXT       = 0x8C8A,
  //GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_EXT             = 0x8C8B,
  //GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_EXT          = 0x8C80,
  //GL_TRANSFORM_FEEDBACK_VARYINGS_EXT                         = 0x8C83,
  //GL_TRANSFORM_FEEDBACK_BUFFER_MODE_EXT                      = 0x8C7F,
  //GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH_EXT               = 0x8C76,

};

void glBeginTransformFeedbackEXT(GLenum[Primitives] primitiveMode);
void glEndTransformFeedbackEXT(void);
void glBindBufferRangeEXT(GLenum[Main] target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glBindBufferOffsetEXT(GLenum[Main] target, GLuint index, GLuint buffer, GLintptr offset);
void glBindBufferBaseEXT(GLenum[Main] target, GLuint index, GLuint buffer);
void glTransformFeedbackVaryingsEXT(GLuint program, GLsizei count, const GLchar* * varyings, GLenum[Main] bufferMode);
void glGetTransformFeedbackVaryingEXT(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
