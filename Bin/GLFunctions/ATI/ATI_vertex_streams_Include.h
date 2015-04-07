#define GLI_INCLUDE_ATI_VERTEX_STREAMS

enum Main {

  GL_MAX_VERTEX_STREAMS_ATI                       =   0x876B,
  GL_VERTEX_STREAM0_ATI                           =   0x876C,
  GL_VERTEX_STREAM1_ATI                           =   0x876D,
  GL_VERTEX_STREAM2_ATI                           =   0x876E,
  GL_VERTEX_STREAM3_ATI                           =   0x876F,
  GL_VERTEX_STREAM4_ATI                           =   0x8770,
  GL_VERTEX_STREAM5_ATI                           =   0x8771,
  GL_VERTEX_STREAM6_ATI                           =   0x8772,
  GL_VERTEX_STREAM7_ATI                           =   0x8773,
  GL_VERTEX_SOURCE_ATI                            =   0x8774,

};

void glVertexStream1sATI  (GLenum[Main] stream, GLshort x);
void glVertexStream1svATI (GLenum[Main] stream, const GLshort *coords);
void glVertexStream1iATI  (GLenum[Main] stream, GLint x);
void glVertexStream1ivATI (GLenum[Main] stream, const GLint *coords);
void glVertexStream1fATI  (GLenum[Main] stream, GLfloat x);
void glVertexStream1fvATI (GLenum[Main] stream, const GLfloat *coords);
void glVertexStream1dATI  (GLenum[Main] stream, GLdouble x);
void glVertexStream1dvATI (GLenum[Main] stream, const GLdouble *coords);
void glVertexStream2sATI  (GLenum[Main] stream, GLshort x, GLshort y);
void glVertexStream2svATI (GLenum[Main] stream, const GLshort *coords);
void glVertexStream2iATI  (GLenum[Main] stream, GLint x, GLint y);
void glVertexStream2ivATI (GLenum[Main] stream, const GLint *coords);
void glVertexStream2fATI  (GLenum[Main] stream, GLfloat x, GLfloat y);
void glVertexStream2fvATI (GLenum[Main] stream, const GLfloat *coords);
void glVertexStream2dATI  (GLenum[Main] stream, GLdouble x, GLdouble y);
void glVertexStream2dvATI (GLenum[Main] stream, const GLdouble *coords);
void glVertexStream3sATI  (GLenum[Main] stream, GLshort x, GLshort y, GLshort z);
void glVertexStream3svATI (GLenum[Main] stream, const GLshort *coords);
void glVertexStream3iATI  (GLenum[Main] stream, GLint x, GLint y, GLint z);
void glVertexStream3ivATI (GLenum[Main] stream, const GLint *coords);
void glVertexStream3fATI  (GLenum[Main] stream, GLfloat x, GLfloat y, GLfloat z);
void glVertexStream3fvATI (GLenum[Main] stream, const GLfloat *coords);
void glVertexStream3dATI  (GLenum[Main] stream, GLdouble x, GLdouble y, GLdouble z);
void glVertexStream3dvATI (GLenum[Main] stream, const GLdouble *coords);
void glVertexStream4sATI  (GLenum[Main] stream, GLshort x, GLshort y, GLshort z, GLshort w);
void glVertexStream4svATI (GLenum[Main] stream, const GLshort *coords);
void glVertexStream4iATI  (GLenum[Main] stream, GLint x, GLint y, GLint z, GLint w);
void glVertexStream4ivATI (GLenum[Main] stream, const GLint *coords);
void glVertexStream4fATI  (GLenum[Main] stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glVertexStream4fvATI (GLenum[Main] stream, const GLfloat *coords);
void glVertexStream4dATI  (GLenum[Main] stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glVertexStream4dvATI (GLenum[Main] stream, const GLdouble *coords);
void glNormalStream3bATI  (GLenum[Main] stream, GLbyte nx, GLbyte ny, GLbyte nz);
void glNormalStream3bvATI (GLenum[Main] stream, const GLbyte *coords);
void glNormalStream3sATI  (GLenum[Main] stream, GLshort nx, GLshort ny, GLshort nz);
void glNormalStream3svATI (GLenum[Main] stream, const GLshort *coords);
void glNormalStream3iATI  (GLenum[Main] stream, GLint nx, GLint ny, GLint nz);
void glNormalStream3ivATI (GLenum[Main] stream, const GLint *coords);
void glNormalStream3fATI  (GLenum[Main] stream, GLfloat nx, GLfloat ny, GLfloat nz);
void glNormalStream3fvATI (GLenum[Main] stream, const GLfloat *coords);
void glNormalStream3dATI  (GLenum[Main] stream, GLdouble nx, GLdouble ny, GLdouble nz);
void glNormalStream3dvATI (GLenum[Main] stream, const GLdouble *coords);
void glClientActiveVertexStreamATI (GLenum[Main] stream);
void glVertexBlendEnviATI (GLenum[Main] pname, GLint param);
void glVertexBlendEnvfATI (GLenum[Main] pname, GLfloat param);

