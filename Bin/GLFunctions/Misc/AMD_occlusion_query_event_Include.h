#define GLI_INCLUDE_GL_AMD_OCCLUSION_QUERY_EVENT

enum Main {

  GL_OCCLUSION_QUERY_EVENT_MASK_AMD              = 0x874F,
  //GL_QUERY_DEPTH_PASS_EVENT_BIT_AMD              = 0x00000001,
  //GL_QUERY_DEPTH_FAIL_EVENT_BIT_AMD              = 0x00000002,
  //GL_QUERY_STENCIL_FAIL_EVENT_BIT_AMD            = 0x00000004,
  //GL_QUERY_DEPTH_BOUNDS_FAIL_EVENT_BIT_AMD       = 0x00000008,
  //GL_QUERY_ALL_EVENT_BITS_AMD                    = 0xFFFFFFFF,

};

void glQueryObjectParameteruiAMD(GLenum[Main] target, GLuint id, GLenum[Main] pname, GLuint param);
