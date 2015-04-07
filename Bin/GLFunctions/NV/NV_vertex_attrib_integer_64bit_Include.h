#define GLI_INCLUDE_GL_NV_VERTEX_ATTRIB_INTEGER_64BIT

void glVertexAttribL1i64NV(GLuint index, GLint64 x);
void glVertexAttribL2i64NV(GLuint index, GLint64 x, GLint64 y);
void glVertexAttribL3i64NV(GLuint index, GLint64 x, GLint64 y, GLint64 z);
void glVertexAttribL4i64NV(GLuint index, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
void glVertexAttribL1i64vNV(GLuint index, const GLint64 * v);
void glVertexAttribL2i64vNV(GLuint index, const GLint64 * v);
void glVertexAttribL3i64vNV(GLuint index, const GLint64 * v);
void glVertexAttribL4i64vNV(GLuint index, const GLint64 * v);
void glVertexAttribL1ui64NV(GLuint index, GLuint64 x);
void glVertexAttribL2ui64NV(GLuint index, GLuint64 x, GLuint64 y);
void glVertexAttribL3ui64NV(GLuint index, GLuint64 x, GLuint64 y, GLuint64 z);
void glVertexAttribL4ui64NV(GLuint index, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
void glVertexAttribL1ui64vNV(GLuint index, const GLuint64 * v);
void glVertexAttribL2ui64vNV(GLuint index, const GLuint64 * v);
void glVertexAttribL3ui64vNV(GLuint index, const GLuint64 * v);
void glVertexAttribL4ui64vNV(GLuint index, const GLuint64 * v);
void glGetVertexAttribLi64vNV(GLuint index, GLenum[Main] pname, GLint64 * params);
void glGetVertexAttribLui64vNV(GLuint index, GLenum[Main] pname, GLuint64 * params);
void glVertexAttribLFormatNV(GLuint index, GLint size, GLenum[Main] type, GLsizei stride);
