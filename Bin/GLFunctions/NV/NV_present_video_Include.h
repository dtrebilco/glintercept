#define GLI_INCLUDE_GL_NV_PRESENT_VIDEO

enum Main {

  GL_FRAME_NV                  = 0x8E26,
  GL_FIELDS_NV                 = 0x8E27,
  //GL_CURRENT_TIME_NV           = 0x8E28,
  GL_NUM_FILL_STREAMS_NV       = 0x8E29,
  GL_PRESENT_TIME_NV           = 0x8E2A,
  GL_PRESENT_DURATION_NV       = 0x8E2B,

};

void glPresentFrameKeyedNV(GLuint video_slot, GLuint64 minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum[Main] type, GLenum[Main] target0, GLuint fill0, GLuint key0, GLenum[Main] target1, GLuint fill1, GLuint key1);
void glPresentFrameDualFillNV(GLuint video_slot, GLuint64 minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum[Main] type, GLenum[Main] target0, GLuint fill0, GLenum[Main] target1, GLuint fill1, GLenum[Main] target2, GLuint fill2, GLenum[Main] target3, GLuint fill3);
void glGetVideoivNV(GLuint video_slot, GLenum[Main] pname, GLint * params);
void glGetVideouivNV(GLuint video_slot, GLenum[Main] pname, GLuint * params);
void glGetVideoi64vNV(GLuint video_slot, GLenum[Main] pname, GLint64 * params);
void glGetVideoui64vNV(GLuint video_slot, GLenum[Main] pname, GLuint64 * params);
