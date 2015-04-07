#define GLI_INCLUDE_GL_ARB_CL_EVENT

enum Main {

  GL_SYNC_CL_EVENT_ARB                = 0x8240,
  GL_SYNC_CL_EVENT_COMPLETE_ARB       = 0x8241,

};

//GLsync glCreateSyncFromCLeventARB(struct _cl_context * context, struct _cl_event * event, GLbitfield flags);
GLsync glCreateSyncFromCLeventARB(GLvoid * context, GLvoid * event, GLbitfield flags);