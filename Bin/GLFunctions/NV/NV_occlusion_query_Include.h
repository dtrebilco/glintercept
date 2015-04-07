#define GLI_INCLUDE_NV_OCCLUSION_QUERY


void glGenOcclusionQueriesNV(GLsizei n, GLuint *ids);
void glDeleteOcclusionQueriesNV(GLsizei n, const GLuint *ids);
GLboolean glIsOcclusionQueryNV(GLuint id);
void glBeginOcclusionQueryNV(GLuint id);
void glEndOcclusionQueryNV(void);
void glGetOcclusionQueryivNV(GLuint id, GLenum[Main] pname, GLint *params);
void glGetOcclusionQueryuivNV(GLuint id, GLenum[Main] pname, GLuint *params);

