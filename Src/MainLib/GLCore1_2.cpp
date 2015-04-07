/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "GLCore1_2.h"
#include "BuiltInFunction.h"

//Function prototypes. Only use in core override files.
#include "GLFunctions.h"

// Core OpenGL 1.2 + ARB multitexture function handlers 
#ifdef GLI_BUILD_LINUX

//The driver to log calls by
extern GLDriver glDriver;

//The version of the OpenGL functions that the builtin wrappers call
// (Functions in this table may be over-ridden/replaced)
extern GLCoreDriver GLV_Builtin;

  //Core 1.2
  GL_FUNCTION_WRAPPER(glDrawRangeElements,(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices), (mode, start, end, count, type, indices), mode);
  GL_FUNCTION_WRAPPER(glTexImage3D,(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels), (target, level, internalFormat, width, height, depth, border, format, type, pixels), target);
  GL_FUNCTION_WRAPPER(glTexSubImage3D,(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels), (target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels), target);
  GL_FUNCTION_WRAPPER(glCopyTexSubImage3D,(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height), (target, level, xoffset, yoffset, zoffset, x, y, width, height), target);
  GL_FUNCTION_WRAPPER(glColorTable,(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table), (target, internalformat, width, format, type, table), target);
  GL_FUNCTION_WRAPPER(glColorSubTable,(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data), (target, start, count, format, type, data), target);
  GL_FUNCTION_WRAPPER(glColorTableParameteriv,(GLenum target, GLenum pname, const GLint *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glColorTableParameterfv,(GLenum target, GLenum pname, const GLfloat *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glCopyColorSubTable,(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width), (target, start, x, y, width), target);
  GL_FUNCTION_WRAPPER(glCopyColorTable,(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width), (target, internalformat, x, y, width), target);
  GL_FUNCTION_WRAPPER(glGetColorTable,(GLenum target, GLenum format, GLenum type, GLvoid *table), (target, format, type, table), target);
  GL_FUNCTION_WRAPPER(glGetColorTableParameterfv,(GLenum target, GLenum pname, GLfloat *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glGetColorTableParameteriv,(GLenum target, GLenum pname, GLint *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glBlendEquation,(GLenum mode), (mode), mode);
  GL_FUNCTION_WRAPPER(glBlendColor,(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glHistogram,(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink), (target, width, internalformat, sink), target);
  GL_FUNCTION_WRAPPER(glResetHistogram,(GLenum target), (target), target);
  GL_FUNCTION_WRAPPER(glGetHistogram,(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values), (target, reset, format, type, values), target);
  GL_FUNCTION_WRAPPER(glGetHistogramParameterfv,(GLenum target, GLenum pname, GLfloat *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glGetHistogramParameteriv,(GLenum target, GLenum pname, GLint *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glMinmax,(GLenum target, GLenum internalformat, GLboolean sink), (target, internalformat, sink), target);
  GL_FUNCTION_WRAPPER(glResetMinmax,(GLenum target), (target), target);
  GL_FUNCTION_WRAPPER(glGetMinmax,(GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values), (target, reset, format, types, values), target);
  GL_FUNCTION_WRAPPER(glGetMinmaxParameterfv,(GLenum target, GLenum pname, GLfloat *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glGetMinmaxParameteriv,(GLenum target, GLenum pname, GLint *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glConvolutionFilter1D,(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image), (target, internalformat, width, format, type, image), target);
  GL_FUNCTION_WRAPPER(glConvolutionFilter2D,(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image), (target, internalformat, width, height, format, type, image), target);
  GL_FUNCTION_WRAPPER(glConvolutionParameterf,(GLenum target, GLenum pname, GLfloat params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glConvolutionParameterfv,(GLenum target, GLenum pname, const GLfloat *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glConvolutionParameteri,(GLenum target, GLenum pname, GLint params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glConvolutionParameteriv,(GLenum target, GLenum pname, const GLint *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glCopyConvolutionFilter1D,(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width), (target, internalformat, x, y, width), target);
  GL_FUNCTION_WRAPPER(glCopyConvolutionFilter2D,(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height), (target, internalformat, x, y, width, height), target);
  GL_FUNCTION_WRAPPER(glGetConvolutionFilter,(GLenum target, GLenum format, GLenum type, GLvoid *image), (target, format, type, image), target);
  GL_FUNCTION_WRAPPER(glGetConvolutionParameterfv,(GLenum target, GLenum pname, GLfloat *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glGetConvolutionParameteriv,(GLenum target, GLenum pname, GLint *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glSeparableFilter2D,(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column), (target, internalformat, width, height, format, type, row, column), target);
  GL_FUNCTION_WRAPPER(glGetSeparableFilter,(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span), (target, format, type, row, column, span), target);

  //ARB multitexture extension
  GL_FUNCTION_WRAPPER(glActiveTextureARB,(GLenum texture), (texture), texture);
  GL_FUNCTION_WRAPPER(glClientActiveTextureARB,(GLenum texture), (texture), texture);
  GL_FUNCTION_WRAPPER(glMultiTexCoord1dARB,(GLenum target, GLdouble s), (target, s), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord1dvARB,(GLenum target, const GLdouble *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord1fARB,(GLenum target, GLfloat s), (target, s), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord1fvARB,(GLenum target, const GLfloat *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord1iARB,(GLenum target, GLint s), (target, s), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord1ivARB,(GLenum target, const GLint *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord1sARB,(GLenum target, GLshort s), (target, s), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord1svARB,(GLenum target, const GLshort *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord2dARB,(GLenum target, GLdouble s, GLdouble t), (target, s, t), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord2dvARB,(GLenum target, const GLdouble *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord2fARB,(GLenum target, GLfloat s, GLfloat t), (target, s, t), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord2fvARB,(GLenum target, const GLfloat *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord2iARB,(GLenum target, GLint s, GLint t), (target, s, t), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord2ivARB,(GLenum target, const GLint *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord2sARB,(GLenum target, GLshort s, GLshort t), (target, s, t), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord2svARB,(GLenum target, const GLshort *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord3dARB,(GLenum target, GLdouble s, GLdouble t, GLdouble r), (target, s, t, r), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord3dvARB,(GLenum target, const GLdouble *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord3fARB,(GLenum target, GLfloat s, GLfloat t, GLfloat r), (target, s, t, r), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord3fvARB,(GLenum target, const GLfloat *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord3iARB,(GLenum target, GLint s, GLint t, GLint r), (target, s, t, r), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord3ivARB,(GLenum target, const GLint *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord3sARB,(GLenum target, GLshort s, GLshort t, GLshort r), (target, s, t, r), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord3svARB,(GLenum target, const GLshort *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord4dARB,(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q), (target, s, t, r, q), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord4dvARB,(GLenum target, const GLdouble *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord4fARB,(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q), (target, s, t, r, q), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord4fvARB,(GLenum target, const GLfloat *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord4iARB,(GLenum target, GLint s, GLint t, GLint r, GLint q), (target, s, t, r, q), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord4ivARB,(GLenum target, const GLint *v), (target, v), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord4sARB,(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q), (target, s, t, r, q), target);
  GL_FUNCTION_WRAPPER(glMultiTexCoord4svARB,(GLenum target, const GLshort *v), (target, v), target);



#endif //GLI_BUILD_LINUX

