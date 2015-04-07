#define GLI_INCLUDE_GL_SUN_TRIANGLE_LIST

enum Main {

  //GL_RESTART_SUN                              = 0x0001,
  //GL_REPLACE_MIDDLE_SUN                       = 0x0002,
  //GL_REPLACE_OLDEST_SUN                       = 0x0003,
  GL_TRIANGLE_LIST_SUN                        = 0x81D7,
  GL_REPLACEMENT_CODE_SUN                     = 0x81D8,
  GL_REPLACEMENT_CODE_ARRAY_SUN               = 0x85C0,
  GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN          = 0x85C1,
  GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN        = 0x85C2,
  GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN       = 0x85C3,
  GL_R1UI_V3F_SUN                             = 0x85C4,
  GL_R1UI_C4UB_V3F_SUN                        = 0x85C5,
  GL_R1UI_C3F_V3F_SUN                         = 0x85C6,
  GL_R1UI_N3F_V3F_SUN                         = 0x85C7,
  GL_R1UI_C4F_N3F_V3F_SUN                     = 0x85C8,
  GL_R1UI_T2F_V3F_SUN                         = 0x85C9,
  GL_R1UI_T2F_N3F_V3F_SUN                     = 0x85CA,
  GL_R1UI_T2F_C4F_N3F_V3F_SUN                 = 0x85CB,

};

void glReplacementCodeuiSUN(GLuint code);
void glReplacementCodeusSUN(GLushort code);
void glReplacementCodeubSUN(GLubyte code);
void glReplacementCodeuivSUN(const GLuint * code);
void glReplacementCodeusvSUN(const GLushort * code);
void glReplacementCodeubvSUN(const GLubyte * code);
void glReplacementCodePointerSUN(GLenum[Main] type, GLsizei stride, const GLvoid* * pointer);
