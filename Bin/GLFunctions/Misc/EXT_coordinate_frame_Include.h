#define GLI_INCLUDE_GL_EXT_COORDINATE_FRAME

enum Main {

  GL_TANGENT_ARRAY_EXT                = 0x8439,
  GL_BINORMAL_ARRAY_EXT               = 0x843A,
  GL_CURRENT_TANGENT_EXT              = 0x843B,
  GL_CURRENT_BINORMAL_EXT             = 0x843C,
  GL_TANGENT_ARRAY_TYPE_EXT           = 0x843E,
  GL_TANGENT_ARRAY_STRIDE_EXT         = 0x843F,
  GL_BINORMAL_ARRAY_TYPE_EXT          = 0x8440,
  GL_BINORMAL_ARRAY_STRIDE_EXT        = 0x8441,
  GL_TANGENT_ARRAY_POINTER_EXT        = 0x8442,
  GL_BINORMAL_ARRAY_POINTER_EXT       = 0x8443,
  GL_MAP1_TANGENT_EXT                 = 0x8444,
  GL_MAP2_TANGENT_EXT                 = 0x8445,
  GL_MAP1_BINORMAL_EXT                = 0x8446,
  GL_MAP2_BINORMAL_EXT                = 0x8447,

};

void glTangent3bEXT(GLbyte tx, GLbyte ty, GLbyte tz);
void glTangent3bvEXT(const GLbyte * v);
void glTangent3dEXT(GLdouble tx, GLdouble ty, GLdouble tz);
void glTangent3dvEXT(const GLdouble * v);
void glTangent3fEXT(GLfloat tx, GLfloat ty, GLfloat tz);
void glTangent3fvEXT(const GLfloat * v);
void glTangent3iEXT(GLint tx, GLint ty, GLint tz);
void glTangent3ivEXT(const GLint * v);
void glTangent3sEXT(GLshort tx, GLshort ty, GLshort tz);
void glTangent3svEXT(const GLshort * v);
void glBinormal3bEXT(GLbyte bx, GLbyte by, GLbyte bz);
void glBinormal3bvEXT(const GLbyte * v);
void glBinormal3dEXT(GLdouble bx, GLdouble by, GLdouble bz);
void glBinormal3dvEXT(const GLdouble * v);
void glBinormal3fEXT(GLfloat bx, GLfloat by, GLfloat bz);
void glBinormal3fvEXT(const GLfloat * v);
void glBinormal3iEXT(GLint bx, GLint by, GLint bz);
void glBinormal3ivEXT(const GLint * v);
void glBinormal3sEXT(GLshort bx, GLshort by, GLshort bz);
void glBinormal3svEXT(const GLshort * v);
void glTangentPointerEXT(GLenum[Main] type, GLsizei stride, const GLvoid * pointer);
void glBinormalPointerEXT(GLenum[Main] type, GLsizei stride, const GLvoid * pointer);
