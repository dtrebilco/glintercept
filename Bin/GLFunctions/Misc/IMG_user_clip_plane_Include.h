#define GLI_INCLUDE_GL_IMG_USER_CLIP_PLANE

enum Main {

  //GL_CLIP_PLANE0_IMG           = 0x3000,
  //GL_CLIP_PLANE1_IMG           = 0x3001,
  //GL_CLIP_PLANE2_IMG           = 0x3002,
  //GL_CLIP_PLANE3_IMG           = 0x3003,
  //GL_CLIP_PLANE4_IMG           = 0x3004,
  //GL_CLIP_PLANE5_IMG           = 0x3005,
  //GL_MAX_CLIP_PLANES_IMG       = 0x0D32,

};

void glClipPlanefIMG(GLenum[Main] p, const GLfloat *eqn);
void glClipPlanexIMG(GLenum[Main] p, const GLfixed *eqn);
