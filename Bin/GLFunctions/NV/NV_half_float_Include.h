#define GLI_INCLUDE_NV_HALF_FLOAT

///////////////////////////////////////////////////////////////////////////////
// Main GLenum
///////////////////////////////////////////////////////////////////////////////

enum Main {

  //GL_HALF_FLOAT_NV                =  0x140B,
};

//Note: Instead of defining a new GLhalf type, GLushort is used instead (same 16 bit size)
//      This means the logging will not be of the correct values

void glVertex2hNV(GLushort x, GLushort y);
void glVertex2hvNV(const GLushort *v);
void glVertex3hNV(GLushort x, GLushort y, GLushort z);
void glVertex3hvNV(const GLushort *v);
void glVertex4hNV(GLushort x, GLushort y, GLushort z, GLushort w);
void glVertex4hvNV(const GLushort *v);
void glNormal3hNV(GLushort nx, GLushort ny, GLushort nz);
void glNormal3hvNV(const GLushort *v);
void glColor3hNV(GLushort red, GLushort green, GLushort blue);
void glColor3hvNV(const GLushort *v);
void glColor4hNV(GLushort red, GLushort green, GLushort blue, GLushort alpha);
void glColor4hvNV(const GLushort *v);
void glTexCoord1hNV(GLushort s);
void glTexCoord1hvNV(const GLushort *v);
void glTexCoord2hNV(GLushort s, GLushort t);
void glTexCoord2hvNV(const GLushort *v);
void glTexCoord3hNV(GLushort s, GLushort t, GLushort r);
void glTexCoord3hvNV(const GLushort *v);
void glTexCoord4hNV(GLushort s, GLushort t, GLushort r, GLushort q);
void glTexCoord4hvNV(const GLushort *v);
void glMultiTexCoord1hNV(GLenum[Main] target, GLushort s);
void glMultiTexCoord1hvNV(GLenum[Main] target, const GLushort *v);
void glMultiTexCoord2hNV(GLenum[Main] target, GLushort s, GLushort t);
void glMultiTexCoord2hvNV(GLenum[Main] target, const GLushort *v);
void glMultiTexCoord3hNV(GLenum[Main] target, GLushort s, GLushort t, GLushort r);
void glMultiTexCoord3hvNV(GLenum[Main] target, const GLushort *v);
void glMultiTexCoord4hNV(GLenum[Main] target, GLushort s, GLushort t, GLushort r, GLushort q);
void glMultiTexCoord4hvNV(GLenum[Main] target, const GLushort *v);
void glFogCoordhNV(GLushort fog);
void glFogCoordhvNV(const GLushort *fog);
void glSecondaryColor3hNV(GLushort red, GLushort green, GLushort blue);
void glSecondaryColor3hvNV(const GLushort *v);
void glVertexWeighthNV(GLushort weight);
void glVertexWeighthvNV(const GLushort *weight);
void glVertexAttrib1hNV(GLuint index, GLushort x);
void glVertexAttrib1hvNV(GLuint index, const GLushort *v);
void glVertexAttrib2hNV(GLuint index, GLushort x, GLushort y);
void glVertexAttrib2hvNV(GLuint index, const GLushort *v);
void glVertexAttrib3hNV(GLuint index, GLushort x, GLushort y, GLushort z);
void glVertexAttrib3hvNV(GLuint index, const GLushort *v);
void glVertexAttrib4hNV(GLuint index, GLushort x, GLushort y, GLushort z, GLushort w);
void glVertexAttrib4hvNV(GLuint index, const GLushort *v);
void glVertexAttribs1hvNV(GLuint index, GLsizei n, const GLushort *v);
void glVertexAttribs2hvNV(GLuint index, GLsizei n, const GLushort *v);
void glVertexAttribs3hvNV(GLuint index, GLsizei n, const GLushort *v);
void glVertexAttribs4hvNV(GLuint index, GLsizei n, const GLushort *v);
