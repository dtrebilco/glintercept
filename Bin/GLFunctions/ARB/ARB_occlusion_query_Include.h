#define GLI_INCLUDE_ARB_OCCLUSION_QUERY


void glGenQueriesARB(GLsizei n, GLuint *ids);
void glDeleteQueriesARB(GLsizei n, const GLuint *ids);

GLboolean glIsQueryARB(GLuint id);

void glBeginQueryARB(GLenum[Main] target, GLuint id);
void glEndQueryARB(GLenum[Main] target);

void glGetQueryivARB(GLenum[Main] target, GLenum[Main] pname, GLint *params);
void glGetQueryObjectivARB(GLuint id, GLenum[Main] pname, GLint *params);
void glGetQueryObjectuivARB(GLuint id, GLenum[Main] pname, GLuint *params);
