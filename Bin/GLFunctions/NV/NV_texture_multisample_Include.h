#define GLI_INCLUDE_GL_NV_TEXTURE_MULTISAMPLE

enum Main {

  GL_TEXTURE_COVERAGE_SAMPLES_NV       = 0x9045,
  GL_TEXTURE_COLOR_SAMPLES_NV          = 0x9046,

};

void glTexImage2DMultisampleCoverageNV(GLenum[Main] target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
void glTexImage3DMultisampleCoverageNV(GLenum[Main] target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
void glTextureImage2DMultisampleNV(GLuint texture, GLenum[Main] target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
void glTextureImage3DMultisampleNV(GLuint texture, GLenum[Main] target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
void glTextureImage2DMultisampleCoverageNV(GLuint texture, GLenum[Main] target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
void glTextureImage3DMultisampleCoverageNV(GLuint texture, GLenum[Main] target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
