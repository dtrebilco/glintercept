#define GLI_INCLUDE_EXT_DIRECT_STATE_ACCESS


enum Main {
  
  GL_PROGRAM_MATRIX_EXT                =  0x8E2D,
  GL_TRANSPOSE_PROGRAM_MATRIX_EXT      =  0x8E2E,
  GL_PROGRAM_MATRIX_STACK_DEPTH_EXT    =  0x8E2F,

};

void glClientAttribDefaultEXT(GLbitfield[Mask_ClientAttrib] mask);
void glPushClientAttribDefaultEXT(GLbitfield[Mask_ClientAttrib] mask);

void glMatrixLoadfEXT(GLenum[Main] matrixMode, const GLfloat *m);
void glMatrixLoaddEXT(GLenum[Main] matrixMode, const GLdouble *m);

void glMatrixMultfEXT(GLenum[Main] matrixMode, const GLfloat *m);
void glMatrixMultdEXT(GLenum[Main] matrixMode, const GLdouble *m);

void glMatrixLoadIdentityEXT(GLenum[Main] matrixMode);

void glMatrixRotatefEXT(GLenum[Main] matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void glMatrixRotatedEXT(GLenum[Main] matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);

void glMatrixScalefEXT(GLenum[Main] matrixMode, GLfloat x, GLfloat y, GLfloat z);
void glMatrixScaledEXT(GLenum[Main] matrixMode, GLdouble x, GLdouble y, GLdouble z);

void glMatrixTranslatefEXT(GLenum[Main] matrixMode, GLfloat x, GLfloat y, GLfloat z);
void glMatrixTranslatedEXT(GLenum[Main] matrixMode, GLdouble x, GLdouble y, GLdouble z);

void glMatrixOrthoEXT(GLenum[Main] matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
void glMatrixFrustumEXT(GLenum[Main] matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);

void glMatrixPushEXT(GLenum[Main] matrixMode);
void glMatrixPopEXT(GLenum[Main] matrixMode);

void glTextureParameteriEXT(GLuint texture, GLenum[Main] target, GLenum[Main] pname, GLint param);
void glTextureParameterivEXT(GLuint texture, GLenum[Main] target, GLenum[Main] pname, const GLint *param);

void glTextureParameterfEXT(GLuint texture, GLenum[Main] target, GLenum[Main] pname, GLfloat param);
void glTextureParameterfvEXT(GLuint texture, GLenum[Main] target, GLenum[Main] pname, const GLfloat *param);

void glTextureImage1DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glTextureImage2DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum[Main] format, GLenum[Main] type, const void *pixels);

void glTextureSubImage1DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint xoffset, GLsizei width, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glTextureSubImage2DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, const void *pixels);

void glCopyTextureImage1DEXT(GLuint texture, GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width, GLint border);
void glCopyTextureImage2DEXT(GLuint texture, GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);

void glCopyTextureSubImage1DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void glCopyTextureSubImage2DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

void glGetTextureImageEXT(GLuint texture, GLenum[Main] target, GLint level, GLenum[Main] format, GLenum[Main] type, void *pixels);

void glGetTextureParameterfvEXT(GLuint texture, GLenum[Main] target, GLenum[Main] pname, GLfloat *params);
void glGetTextureParameterivEXT(GLuint texture, GLenum[Main] target, GLenum[Main] pname, GLint *params);

void glGetTextureLevelParameterfvEXT(GLuint texture, GLenum[Main] target, GLint level, GLenum[Main] pname, GLfloat *params);
void glGetTextureLevelParameterivEXT(GLuint texture, GLenum[Main] target, GLint level, GLenum[Main] pname, GLint *params);


void glTextureImage3DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glTextureSubImage3DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glCopyTextureSubImage3DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

void glBindMultiTextureEXT(GLenum[Main] texunit, GLenum[Main] target, GLuint texture);
void glMultiTexCoordPointerEXT(GLenum[Main] texunit, GLint size, GLenum[Main] type, GLsizei stride, const void *pointer);

void glMultiTexEnvfEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, GLfloat param);
void glMultiTexEnvfvEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, const GLfloat *params);
void glMultiTexEnviEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, GLint param);
void glMultiTexEnvivEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, const GLint *params);

void glMultiTexGendEXT(GLenum[Main] texunit, GLenum[Main] coord, GLenum[Main] pname, GLdouble param);
void glMultiTexGendvEXT(GLenum[Main] texunit, GLenum[Main] coord, GLenum[Main] pname, const GLdouble *params);
void glMultiTexGenfEXT(GLenum[Main] texunit, GLenum[Main] coord, GLenum[Main] pname, GLfloat param);
void glMultiTexGenfvEXT(GLenum[Main] texunit, GLenum[Main] coord, GLenum[Main] pname, const GLfloat *params);
void glMultiTexGeniEXT(GLenum[Main] texunit, GLenum[Main] coord, GLenum[Main] pname, GLint param);
void glMultiTexGenivEXT(GLenum[Main] texunit, GLenum[Main] coord, GLenum[Main] pname, const GLint *params);

void glGetMultiTexEnvfvEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, GLfloat *params);
void glGetMultiTexEnvivEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, GLint *params);

void glGetMultiTexGendvEXT(GLenum[Main] texunit, GLenum[Main] coord, GLenum[Main] pname, GLdouble *params);
void glGetMultiTexGenfvEXT(GLenum[Main] texunit, GLenum[Main] coord, GLenum[Main] pname, GLfloat *params);
void glGetMultiTexGenivEXT(GLenum[Main] texunit, GLenum[Main] coord, GLenum[Main] pname, GLint *params);

void glMultiTexParameteriEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, GLint param);
void glMultiTexParameterivEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, const GLint *param);

void glMultiTexParameterfEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, GLfloat param);
void glMultiTexParameterfvEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, const GLfloat *param);

void glMultiTexImage1DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glMultiTexImage2DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum[Main] format, GLenum[Main] type, const void *pixels);

void glMultiTexSubImage1DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint xoffset, GLsizei width, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glMultiTexSubImage2DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, const void *pixels);

void glCopyMultiTexImage1DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width, GLint border);
void glCopyMultiTexImage2DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);

void glCopyMultiTexSubImage1DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void glCopyMultiTexSubImage2DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

void glGetMultiTexImageEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLenum[Main] format, GLenum[Main] type, void *pixels);

void glGetMultiTexParameterfvEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, GLfloat *params);
void glGetMultiTexParameterivEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, GLint *params);

void glGetMultiTexLevelParameterfvEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLenum[Main] pname, GLfloat *params);
void glGetMultiTexLevelParameterivEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLenum[Main] pname, GLint *params);

void glMultiTexImage3DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glMultiTexSubImage3DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glCopyMultiTexSubImage3DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

void glEnableClientStateIndexedEXT(GLenum[Main] array, GLuint index);
void glDisableClientStateIndexedEXT(GLenum[Main] array, GLuint index);

void glEnableClientStateiEXT(GLenum[Main] array, GLuint index);
void glDisableClientStateiEXT(GLenum[Main] array, GLuint index);

void glGetFloatIndexedvEXT(GLenum[Main] target, GLuint index, GLfloat *params);
void glGetDoubleIndexedvEXT(GLenum[Main] target, GLuint index, GLdouble *params);

void glGetPointerIndexedvEXT(GLenum[Main] target, GLuint index, void **params);

void glGetFloati_vEXT(GLenum[Main] pname, GLuint index, GLfloat *params);
void glGetDoublei_vEXT(GLenum[Main] pname, GLuint index, GLdouble *params);

void glGetPointeri_vEXT(GLenum[Main] pname, GLuint index, void **params);

// EXT_draw_buffers2 entry points
//void glEnableIndexedEXT(GLenum[Main] cap, GLuint index);
//void glDisableIndexedEXT(GLenum[Main] cap, GLuint index);

//GLboolean glIsEnabledIndexedEXT(GLenum[Main] target, GLuint index);

//void glGetIntegerIndexedvEXT(GLenum[Main] target, GLuint index, GLint *params);
//void glGetBooleanIndexedvEXT(GLenum[Main] target, GLuint index, GLboolean *params);

void glNamedProgramStringEXT(GLuint program, GLenum[Main] target, GLenum[Main] format, GLsizei len, const void *string); 

void glNamedProgramLocalParameter4dEXT(GLuint program, GLenum[Main] target, GLuint index,  GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glNamedProgramLocalParameter4dvEXT(GLuint program, GLenum[Main] target, GLuint index, const GLdouble *params);
void glNamedProgramLocalParameter4fEXT(GLuint program, GLenum[Main] target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glNamedProgramLocalParameter4fvEXT(GLuint program, GLenum[Main] target, GLuint index, const GLfloat *params);

void glGetNamedProgramLocalParameterdvEXT(GLuint program, GLenum[Main] target, GLuint index, GLdouble *params);
void glGetNamedProgramLocalParameterfvEXT(GLuint program, GLenum[Main] target, GLuint index, GLfloat *params);
void glGetNamedProgramivEXT(GLuint program, GLenum[Main] target, GLenum[Main] pname, GLint *params);
void glGetNamedProgramStringEXT(GLuint program, GLenum[Main] target, GLenum[Main] pname, void *string);

void glCompressedTextureImage3DEXT(GLuint texture, GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
void glCompressedTextureImage2DEXT(GLuint texture, GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
void glCompressedTextureImage1DEXT(GLuint texture, GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
void glCompressedTextureSubImage3DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLsizei imageSize, const void *data);
void glCompressedTextureSubImage2DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum[Main] format, GLsizei imageSize, const void *data);
void glCompressedTextureSubImage1DEXT(GLuint texture, GLenum[Main] target, GLint level, GLint xoffset, GLsizei width, GLenum[Main] format, GLsizei imageSize, const void *data);

void glGetCompressedTextureImageEXT(GLuint texture, GLenum[Main] target, GLint level, void *img);

void glCompressedMultiTexImage3DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
void glCompressedMultiTexImage2DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
void glCompressedMultiTexImage1DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLenum[Main] internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
void glCompressedMultiTexSubImage3DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLsizei imageSize, const void *data);
void glCompressedMultiTexSubImage2DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum[Main] format, GLsizei imageSize, const void *data);
void glCompressedMultiTexSubImage1DEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, GLint xoffset, GLsizei width, GLenum[Main] format, GLsizei imageSize, const void *data);

void glGetCompressedMultiTexImageEXT(GLenum[Main] texunit, GLenum[Main] target, GLint level, void *img);

void glMatrixLoadTransposefEXT(GLenum[Main] matrixMode, const GLfloat *m);
void glMatrixLoadTransposedEXT(GLenum[Main] matrixMode, const GLdouble *m);

void glMatrixMultTransposefEXT(GLenum[Main] matrixMode, const GLfloat *m);
void glMatrixMultTransposedEXT(GLenum[Main] matrixMode, const GLdouble *m);

void glNamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const void *data, GLenum[Main] usage);
void glNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
void* glMapNamedBufferEXT(GLuint buffer, GLenum[Main] access);
GLboolean glUnmapNamedBufferEXT(GLuint buffer);
void glGetNamedBufferParameterivEXT(GLuint buffer, GLenum[Main] pname, GLint *params);
void glGetNamedBufferPointervEXT(GLuint buffer, GLenum[Main] pname, void* *params);
void glGetNamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);

void glProgramUniform1fEXT(GLuint program, GLint location, GLfloat v0);
void glProgramUniform2fEXT(GLuint program, GLint location, GLfloat v0, GLfloat v1);
void glProgramUniform3fEXT(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void glProgramUniform4fEXT(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

void glProgramUniform1iEXT(GLuint program, GLint location, GLint v0);
void glProgramUniform2iEXT(GLuint program, GLint location, GLint v0, GLint v1);
void glProgramUniform3iEXT(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
void glProgramUniform4iEXT(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);

void glProgramUniform1fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value);
void glProgramUniform2fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value);
void glProgramUniform3fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value);
void glProgramUniform4fvEXT(GLuint program, GLint location, GLsizei count, const GLfloat *value);

void glProgramUniform1ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value);
void glProgramUniform2ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value);
void glProgramUniform3ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value);
void glProgramUniform4ivEXT(GLuint program, GLint location, GLsizei count, const GLint *value);

void glProgramUniformMatrix2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glProgramUniformMatrix3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glProgramUniformMatrix4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

void glProgramUniformMatrix2x3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glProgramUniformMatrix3x2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glProgramUniformMatrix2x4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glProgramUniformMatrix4x2fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glProgramUniformMatrix3x4fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glProgramUniformMatrix4x3fvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

void glTextureBufferEXT(GLuint texture, GLenum[Main] target, GLenum[Main] internalformat, GLuint buffer);

void glMultiTexBufferEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] internalformat, GLuint buffer);

void glTextureParameterIivEXT(GLuint texture, GLenum[Main] target, GLenum[Main] pname, const GLint *params);
void glTextureParameterIuivEXT(GLuint texture, GLenum[Main] target, GLenum[Main] pname, const GLuint *params);

void glGetTextureParameterIivEXT(GLuint texture, GLenum[Main] target, GLenum[Main] pname, GLint *params);
void glGetTextureParameterIuivEXT(GLuint texture, GLenum[Main] target, GLenum[Main] pname, GLuint *params);

void glMultiTexParameterIivEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, const GLint *params);
void glMultiTexParameterIuivEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, const GLuint *params);

void glGetMultiTexParameterIivEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, GLint *params);
void glGetMultiTexParameterIuivEXT(GLenum[Main] texunit, GLenum[Main] target, GLenum[Main] pname, GLuint *params);

void glProgramUniform1uiEXT(GLuint program, GLint location, GLuint v0);
void glProgramUniform2uiEXT(GLuint program, GLint location, GLuint v0, GLuint v1);
void glProgramUniform3uiEXT(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
void glProgramUniform4uiEXT(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

void glProgramUniform1uivEXT(GLuint program, GLint location, GLsizei count, const GLuint *value);
void glProgramUniform2uivEXT(GLuint program, GLint location, GLsizei count, const GLuint *value);
void glProgramUniform3uivEXT(GLuint program, GLint location, GLsizei count, const GLuint *value);
void glProgramUniform4uivEXT(GLuint program, GLint location, GLsizei count, const GLuint *value);

void glNamedProgramLocalParameters4fvEXT(GLuint program, GLenum[Main] target, GLuint index, GLsizei count, const GLfloat *params); 

void glNamedProgramLocalParameterI4iEXT(GLuint program, GLenum[Main] target, GLuint index, GLint x, GLint y, GLint z, GLint w);
void glNamedProgramLocalParameterI4ivEXT(GLuint program, GLenum[Main] target, GLuint index, const GLint *params);
void glNamedProgramLocalParametersI4ivEXT(GLuint program, GLenum[Main] target, GLuint index, GLsizei count, const GLint *params);
void glNamedProgramLocalParameterI4uiEXT(GLuint program, GLenum[Main] target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void glNamedProgramLocalParameterI4uivEXT(GLuint program, GLenum[Main] target, GLuint index, const GLuint *params);
void glNamedProgramLocalParametersI4uivEXT(GLuint program, GLenum[Main] target, GLuint index, GLsizei count, const GLuint *params);

void glGetNamedProgramLocalParameterIivEXT(GLuint program, GLenum[Main] target, GLuint index, GLint *params);
void glGetNamedProgramLocalParameterIuivEXT(GLuint program, GLenum[Main] target, GLuint index, GLuint *params);

void glNamedRenderbufferStorageEXT(GLuint renderbuffer, GLenum[Main] internalformat, GLsizei width, GLsizei height);

void glGetNamedRenderbufferParameterivEXT(GLuint renderbuffer, GLenum[Main] pname, GLint *params);
void glNamedRenderbufferStorageMultisampleEXT(GLuint renderbuffer, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height);
void glNamedRenderbufferStorageMultisampleCoverageEXT(GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum[Main] internalformat, GLsizei width, GLsizei height);

GLenum[Main] glCheckNamedFramebufferStatusEXT(GLuint framebuffer, GLenum[Main] target);
void glNamedFramebufferTexture1DEXT(GLuint framebuffer, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture,GLint level);
void glNamedFramebufferTexture2DEXT(GLuint framebuffer, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture,GLint level);
void glNamedFramebufferTexture3DEXT(GLuint framebuffer, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level, GLint zoffset);

void glNamedFramebufferRenderbufferEXT(GLuint framebuffer, GLenum[Main] attachment, GLenum[Main] renderbuffertarget, GLuint renderbuffer);

void glGetNamedFramebufferAttachmentParameterivEXT(GLuint framebuffer, GLenum[Main] attachment, GLenum[Main] pname,GLint *params);
void glGenerateTextureMipmapEXT(GLuint texture, GLenum[Main] target);
void glGenerateMultiTexMipmapEXT(GLenum[Main] texunit, GLenum[Main] target);

void glFramebufferDrawBufferEXT(GLuint framebuffer, GLenum[Main] mode);
void glFramebufferDrawBuffersEXT(GLuint framebuffer, GLsizei n, const GLenum[Main] *bufs);
void glFramebufferReadBufferEXT(GLuint framebuffer, GLenum[Main] mode);

void glGetFramebufferParameterivEXT(GLuint framebuffer, GLenum[Main] pname, GLint *param);

void glNamedCopyBufferSubDataEXT(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);

void glNamedFramebufferTextureEXT(GLuint framebuffer, GLenum[Main] attachment, GLuint texture, GLint level);
void glNamedFramebufferTextureLayerEXT(GLuint framebuffer, GLenum[Main] attachment, GLuint texture, GLint level, GLint layer);
void glNamedFramebufferTextureFaceEXT(GLuint framebuffer, GLenum[Main] attachment, GLuint texture, GLint level, GLenum[Main] face);

void glTextureRenderbufferEXT(GLuint texture, GLenum[Main] target, GLuint renderbuffer);

void glMultiTexRenderbufferEXT(GLenum[Main] texunit, GLenum[Main] target, GLuint renderbuffer);

void glVertexArrayVertexOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum[Main] type, GLsizei stride, GLintptr offset);
void glVertexArrayColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum[Main] type, GLsizei stride, GLintptr offset);
void glVertexArrayEdgeFlagOffsetEXT(GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
void glVertexArrayIndexOffsetEXT(GLuint vaobj, GLuint buffer, GLenum[Main] type, GLsizei stride, GLintptr offset);
void glVertexArrayNormalOffsetEXT(GLuint vaobj, GLuint buffer, GLenum[Main] type, GLsizei stride, GLintptr offset);
void glVertexArrayTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum[Main] type, GLsizei stride, GLintptr offset);
void glVertexArrayMultiTexCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum[Main] texunit, GLint size, GLenum[Main] type, GLsizei stride, GLintptr offset);
void glVertexArrayFogCoordOffsetEXT(GLuint vaobj, GLuint buffer, GLenum[Main] type, GLsizei stride, GLintptr offset);
void glVertexArraySecondaryColorOffsetEXT(GLuint vaobj, GLuint buffer, GLint size, GLenum[Main] type, GLsizei stride, GLintptr offset);
void glVertexArrayVertexAttribOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum[Main] type, GLboolean normalized, GLsizei stride, GLintptr offset);
void glVertexArrayVertexAttribIOffsetEXT(GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum[Main] type, GLsizei stride, GLintptr offset);

void glEnableVertexArrayEXT(GLuint vaobj, GLenum[Main] array);
void glDisableVertexArrayEXT(GLuint vaobj, GLenum[Main] array);

void glEnableVertexArrayAttribEXT(GLuint vaobj, GLuint index);
void glDisableVertexArrayAttribEXT(GLuint vaobj, GLuint index);

void glGetVertexArrayIntegervEXT(GLuint vaobj, GLenum[Main] pname, GLint *param);
void glGetVertexArrayPointervEXT(GLuint vaobj, GLenum[Main] pname, void **param);
void glGetVertexArrayIntegeri_vEXT(GLuint vaobj, GLuint index, GLenum[Main] pname, GLint *param);
void glGetVertexArrayPointeri_vEXT(GLuint vaobj, GLuint index, GLenum[Main] pname, void **param);

void *glMapNamedBufferRangeEXT(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
void glFlushMappedNamedBufferRangeEXT(GLuint buffer, GLintptr offset, GLsizeiptr length);


void glProgramUniform1dEXT(GLuint program, GLint location, GLdouble x);
void glProgramUniform1dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble * value);
void glProgramUniform2dEXT(GLuint program, GLint location, GLdouble x, GLdouble y);
void glProgramUniform2dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble * value);
void glProgramUniform3dEXT(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
void glProgramUniform3dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble * value);
void glProgramUniform4dEXT(GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void glProgramUniform4dvEXT(GLuint program, GLint location, GLsizei count, const GLdouble * value);

void glProgramUniformMatrix2dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix2x3dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix2x4dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix3dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix3x2dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix3x4dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix4dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix4x2dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);
void glProgramUniformMatrix4x3dvEXT(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value);

void glNamedBufferStorageEXT(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
void glTexturePageCommitmentEXT(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean resident);
void glVertexArrayVertexAttribDivisorEXT(GLuint vaobj, GLuint index, GLuint divisor);

