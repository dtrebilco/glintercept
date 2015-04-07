#define GLI_INCLUDE_GL_ARB_INDIRECT_PARAMETERS

enum Main {

  GL_PARAMETER_BUFFER_ARB               = 0x80EE,
  GL_PARAMETER_BUFFER_BINDING_ARB       = 0x80EF,

};

void glMultiDrawArraysIndirectCountARB(GLenum[Main] mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
void glMultiDrawElementsIndirectCountARB(GLenum[Main] mode, GLenum[Main] type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
