#define GLI_INCLUDE_GL_AMD_SAMPLE_POSITIONS

enum Main {

  GL_SUBSAMPLE_DISTANCE_AMD       = 0x883F,

};

void glSetMultisamplefvAMD(GLenum[Main] pname, GLuint index, const GLfloat * val);
