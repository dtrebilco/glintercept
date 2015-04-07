#define GLI_INCLUDE_ARB_VERTEX_FRAGMENT_SHADER


enum Main {

  //GL_FRAGMENT_SHADER_ARB                =  0x8B30,

  //GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB =  0x8B49,

  //GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB = 0x8B8B,

  //GL_VERTEX_SHADER_ARB                  =  0x8B31,

  //GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB  =  0x8B4A,
  //GL_MAX_VARYING_FLOATS_ARB             =  0x8B4B,

  //GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB =  0x8B4C,
  //GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB = 0x8B4D,
  
  //GL_OBJECT_ACTIVE_ATTRIBUTES_ARB       =  0x8B89,
  //GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB = 0x8B8A,

};



//void glBindAttribLocationARB(GLGLSL_ARB_handle programObj, GLuint index, const GLchar *name);
void glBindAttribLocationARB(GLGLSL_ARB_handle programObj, GLuint index, const GLasciistring *name);

void glGetActiveAttribARB(GLGLSL_ARB_handle programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLchar *name);

//GLint glGetAttribLocationARB(GLGLSL_ARB_handle programObj, const GLchar *name);
GLint glGetAttribLocationARB(GLGLSL_ARB_handle programObj, const GLasciistring *name);