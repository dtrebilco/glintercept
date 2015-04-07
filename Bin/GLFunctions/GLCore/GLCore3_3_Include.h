#define GLI_INCLUDE_GLCORE3_3

enum Main {

  GL_VERTEX_ATTRIB_ARRAY_DIVISOR        = 0x88FE,
  GL_SRC1_COLOR                         = 0x88F9,
  GL_ONE_MINUS_SRC1_COLOR               = 0x88FA,
  GL_ONE_MINUS_SRC1_ALPHA               = 0x88FB,
  GL_MAX_DUAL_SOURCE_DRAW_BUFFERS       = 0x88FC,
  GL_ANY_SAMPLES_PASSED                 = 0x8C2F,
  GL_SAMPLER_BINDING                    = 0x8919,
  GL_RGB10_A2UI                         = 0x906F,
  GL_TEXTURE_SWIZZLE_R                  = 0x8E42,
  GL_TEXTURE_SWIZZLE_G                  = 0x8E43,
  GL_TEXTURE_SWIZZLE_B                  = 0x8E44,
  GL_TEXTURE_SWIZZLE_A                  = 0x8E45,
  GL_TEXTURE_SWIZZLE_RGBA               = 0x8E46,
  GL_TIME_ELAPSED                       = 0x88BF,
  GL_TIMESTAMP                          = 0x8E28,
  GL_INT_2_10_10_10_REV                 = 0x8D9F,

};

void glVertexAttribDivisor(GLuint index, GLuint divisor);

void glBindFragDataLocationIndexed(GLuint program, GLuint colorNumber, GLuint index, const GLchar * name);
GLint glGetFragDataIndex(GLuint program, const GLchar * name);

void glGenSamplers(GLsizei count, GLuint * samplers);
void glDeleteSamplers(GLsizei count, const GLuint * samplers);
GLboolean glIsSampler(GLuint sampler);
void glBindSampler(GLuint unit, GLuint sampler);

void glSamplerParameteri(GLuint sampler, GLenum[Main] pname, GLint param);
void glSamplerParameteriv(GLuint sampler, GLenum[Main] pname, const GLint * param);
void glSamplerParameterf(GLuint sampler, GLenum[Main] pname, GLfloat param);
void glSamplerParameterfv(GLuint sampler, GLenum[Main] pname, const GLfloat * param);
void glSamplerParameterIiv(GLuint sampler, GLenum[Main] pname, const GLint * param);
void glSamplerParameterIuiv(GLuint sampler, GLenum[Main] pname, const GLuint * param);
void glGetSamplerParameteriv(GLuint sampler, GLenum[Main] pname, GLint * params);
void glGetSamplerParameterIiv(GLuint sampler, GLenum[Main] pname, GLint * params);
void glGetSamplerParameterfv(GLuint sampler, GLenum[Main] pname, GLfloat * params);
void glGetSamplerParameterIuiv(GLuint sampler, GLenum[Main] pname, GLuint * params);

void glQueryCounter(GLuint id, GLenum[Main] target);
void glGetQueryObjecti64v(GLuint id, GLenum[Main] pname, GLint64 * params);
void glGetQueryObjectui64v(GLuint id, GLenum[Main] pname, GLuint64 * params);

void glVertexP2ui(GLenum[Main] type, GLuint value);
void glVertexP2uiv(GLenum[Main] type, const GLuint * value);
void glVertexP3ui(GLenum[Main] type, GLuint value);
void glVertexP3uiv(GLenum[Main] type, const GLuint * value);
void glVertexP4ui(GLenum[Main] type, GLuint value);
void glVertexP4uiv(GLenum[Main] type, const GLuint * value);
void glTexCoordP1ui(GLenum[Main] type, GLuint coords);
void glTexCoordP1uiv(GLenum[Main] type, const GLuint * coords);
void glTexCoordP2ui(GLenum[Main] type, GLuint coords);
void glTexCoordP2uiv(GLenum[Main] type, const GLuint * coords);
void glTexCoordP3ui(GLenum[Main] type, GLuint coords);
void glTexCoordP3uiv(GLenum[Main] type, const GLuint * coords);
void glTexCoordP4ui(GLenum[Main] type, GLuint coords);
void glTexCoordP4uiv(GLenum[Main] type, const GLuint * coords);
void glMultiTexCoordP1ui(GLenum[Main] texture, GLenum[Main] type, GLuint coords);
void glMultiTexCoordP1uiv(GLenum[Main] texture, GLenum[Main] type, const GLuint * coords);
void glMultiTexCoordP2ui(GLenum[Main] texture, GLenum[Main] type, GLuint coords);
void glMultiTexCoordP2uiv(GLenum[Main] texture, GLenum[Main] type, const GLuint * coords);
void glMultiTexCoordP3ui(GLenum[Main] texture, GLenum[Main] type, GLuint coords);
void glMultiTexCoordP3uiv(GLenum[Main] texture, GLenum[Main] type, const GLuint * coords);
void glMultiTexCoordP4ui(GLenum[Main] texture, GLenum[Main] type, GLuint coords);
void glMultiTexCoordP4uiv(GLenum[Main] texture, GLenum[Main] type, const GLuint * coords);
void glNormalP3ui(GLenum[Main] type, GLuint coords);
void glNormalP3uiv(GLenum[Main] type, const GLuint * coords);
void glColorP3ui(GLenum[Main] type, GLuint color);
void glColorP3uiv(GLenum[Main] type, const GLuint * color);
void glColorP4ui(GLenum[Main] type, GLuint color);
void glColorP4uiv(GLenum[Main] type, const GLuint * color);
void glSecondaryColorP3ui(GLenum[Main] type, GLuint color);
void glSecondaryColorP3uiv(GLenum[Main] type, const GLuint * color);
void glVertexAttribP1ui(GLuint index, GLenum[Main] type, GLboolean normalized, GLuint value);
void glVertexAttribP1uiv(GLuint index, GLenum[Main] type, GLboolean normalized, const GLuint * value);
void glVertexAttribP2ui(GLuint index, GLenum[Main] type, GLboolean normalized, GLuint value);
void glVertexAttribP2uiv(GLuint index, GLenum[Main] type, GLboolean normalized, const GLuint * value);
void glVertexAttribP3ui(GLuint index, GLenum[Main] type, GLboolean normalized, GLuint value);
void glVertexAttribP3uiv(GLuint index, GLenum[Main] type, GLboolean normalized, const GLuint * value);
void glVertexAttribP4ui(GLuint index, GLenum[Main] type, GLboolean normalized, GLuint value);
void glVertexAttribP4uiv(GLuint index, GLenum[Main] type, GLboolean normalized, const GLuint * value);
