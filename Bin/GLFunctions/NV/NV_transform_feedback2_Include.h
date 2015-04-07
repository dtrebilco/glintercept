#define GLI_INCLUDE_GL_NV_TRANSFORM_FEEDBACK2

enum Main {

  //GL_TRANSFORM_FEEDBACK_NV                     = 0x8E22,
  //GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED_NV       = 0x8E23,
  //GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE_NV       = 0x8E24,
  //GL_TRANSFORM_FEEDBACK_BINDING_NV             = 0x8E25,

};

void glBindTransformFeedbackNV(GLenum[Main] target, GLuint id);
void glDeleteTransformFeedbacksNV(GLsizei n, const GLuint * ids);
void glGenTransformFeedbacksNV(GLsizei n, GLuint * ids);
GLboolean glIsTransformFeedbackNV(GLuint id);
void glPauseTransformFeedbackNV(void);
void glResumeTransformFeedbackNV(void);
void glDrawTransformFeedbackNV(GLenum[Main] mode, GLuint id);
