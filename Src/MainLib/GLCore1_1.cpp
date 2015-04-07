/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "GLCore1_1.h"
#include "BuiltInFunction.h"

//Function prototypes. Only use in core override files.
#include "GLFunctions.h"

//
// Core OpenGL 1.1 function handlers
//
USING_ERRORLOG

//The driver to log calls by
extern GLDriver glDriver;

//The version of the OpenGL functions that the builtin wrappers call
// (Functions in this table may be over-ridden/replaced)
extern GLCoreDriver GLV_Builtin;

//A dummy value to pass to the logger
static int glcDummyValue=0xBAADF00D;


///////////////////////////////////////////////////////////////////////////////
//
GL_FUNCTION_WRAPPER_HEADER(glBegin, (GLenum mode), (mode))
{
  PRE_FUNCTION(glBegin,(mode),mode);

  //Test for existing glBegin mode
  if(glDriver.GetBeginEndState())
  {
    LOGERR(("glBegin called while previous glBegin was not closed"));
  }
  else
  {
    //Flag that we are now in a Begin/End section
    glDriver.SetBeginEndState(true);
  }

  GLV_Builtin.glBegin (mode);
  POST_FUNCTION(glBegin)
}

///////////////////////////////////////////////////////////////////////////////
//
GL_FUNCTION_WRAPPER_HEADER(glEnd, (), ())
{
  PRE_FUNCTION(glEnd,(),glcDummyValue);
  GLV_Builtin.glEnd();

  //Test for existing glBegin mode
  if(!glDriver.GetBeginEndState())
  {
    LOGERR(("glEnd called whithout a glBegin"));
  }
  else
  {
    //Flag that we are out of the Begin/End section
    glDriver.SetBeginEndState(false);
  }


  POST_FUNCTION(glEnd)
}


///////////////////////////////////////////////////////////////////////////////
//
GL_FUNCTION_WRAPPER_HEADER(glNewList, (GLuint list, GLenum mode), (list, mode))
{
  PRE_FUNCTION(glNewList,(list, mode),list);

  //Test for existing glNewList mode
  if(glDriver.GetNewListState())
  {
    LOGERR(("glNewList called while previous glNewList was not closed"));
  }
  else
  {
    //Flag that we are now in a glNewList/glEndList section
    glDriver.SetNewListState(true);
  }

  GLV_Builtin.glNewList (list,mode);
  POST_FUNCTION(glNewList)
}

///////////////////////////////////////////////////////////////////////////////
//
GL_FUNCTION_WRAPPER_HEADER(glEndList, (), ())
{
  PRE_FUNCTION(glEndList,(),glcDummyValue);
  GLV_Builtin.glEndList();

  //Test for existing glNewList mode
  if(!glDriver.GetNewListState())
  {
    LOGERR(("glEndList called whithout a glNewList"));
  }
  else
  {
    //Flag that we are out of the glNewList/glEndList section
    glDriver.SetNewListState(false);
  }

  POST_FUNCTION(glEndList)
}


///////////////////////////////////////////////////////////////////////////////
//
GL_FUNCTION_WRAPPER_HEADER_RET(glGetError, (), (), GLenum)
{
  PRE_FUNCTION(glGetError,(),glcDummyValue);
  
  //Get the cached value from the driver
  GLenum retValue = glDriver.GetCachedErrorCode();

  //If the cached value is "no-error" call and return the correct error
  if(retValue == GL_NO_ERROR)
  {
    retValue = GLV_Builtin.glGetError();
  }
  else
  {
    //Reset the cached error code (so further calls do not get the same error)
    glDriver.ResetCachedErrorCode();
  }
  
  POST_FUNCTION_RET(glGetError,retValue)

  return retValue;
}



//Generated built in wrappers
  GL_FUNCTION_WRAPPER(glClearIndex,(GLfloat c), (c), c);
  GL_FUNCTION_WRAPPER(glClearColor,(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glClear,(GLbitfield mask), (mask), mask);
  GL_FUNCTION_WRAPPER(glIndexMask,(GLuint mask), (mask), mask);
  GL_FUNCTION_WRAPPER(glColorMask,(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glAlphaFunc,(GLenum func, GLfloat ref), (func, ref), func);
  GL_FUNCTION_WRAPPER(glBlendFunc,(GLenum sfactor, GLenum dfactor), (sfactor, dfactor), sfactor);
  GL_FUNCTION_WRAPPER(glLogicOp,(GLenum opcode), (opcode), opcode);
  GL_FUNCTION_WRAPPER(glCullFace,(GLenum mode), (mode), mode);
  GL_FUNCTION_WRAPPER(glFrontFace,(GLenum mode), (mode), mode);
  GL_FUNCTION_WRAPPER(glPointSize,(GLfloat size), (size), size);
  GL_FUNCTION_WRAPPER(glLineWidth,(GLfloat width), (width), width);
  GL_FUNCTION_WRAPPER(glLineStipple,(GLint factor, GLushort pattern), (factor, pattern), factor);
  GL_FUNCTION_WRAPPER(glPolygonMode,(GLenum face, GLenum mode), (face, mode), face);
  GL_FUNCTION_WRAPPER(glPolygonOffset,(GLfloat factor, GLfloat units), (factor, units), factor);
  GL_FUNCTION_WRAPPER(glPolygonStipple,(const GLubyte *mask), (mask), mask);
  GL_FUNCTION_WRAPPER(glGetPolygonStipple,(GLubyte *mask), (mask), mask);
  GL_FUNCTION_WRAPPER(glEdgeFlag,(GLboolean flag), (flag), flag);
  GL_FUNCTION_WRAPPER(glEdgeFlagv,(const GLboolean *flag), (flag), flag);
  GL_FUNCTION_WRAPPER(glScissor,(GLint x, GLint y, GLsizei width, GLsizei height), (x, y, width, height), x);
  GL_FUNCTION_WRAPPER(glClipPlane,(GLenum plane, const GLdouble *equation), (plane, equation), plane);
  GL_FUNCTION_WRAPPER(glGetClipPlane,(GLenum plane, GLdouble *equation), (plane, equation), plane);
  GL_FUNCTION_WRAPPER(glDrawBuffer,(GLenum mode), (mode), mode);
  GL_FUNCTION_WRAPPER(glReadBuffer,(GLenum mode), (mode), mode);
  GL_FUNCTION_WRAPPER(glEnable,(GLenum cap), (cap), cap);
  GL_FUNCTION_WRAPPER(glDisable,(GLenum cap), (cap), cap);
  GL_FUNCTION_WRAPPER_RET(glIsEnabled,(GLenum cap), (cap), cap, GLboolean);
  GL_FUNCTION_WRAPPER(glEnableClientState,(GLenum cap), (cap), cap);
  GL_FUNCTION_WRAPPER(glDisableClientState,(GLenum cap), (cap), cap);
  GL_FUNCTION_WRAPPER(glGetBooleanv,(GLenum pname, GLboolean *params), (pname, params), pname);
  GL_FUNCTION_WRAPPER(glGetDoublev,(GLenum pname, GLdouble *params), (pname, params), pname);
  GL_FUNCTION_WRAPPER(glGetFloatv,(GLenum pname, GLfloat *params), (pname, params), pname);
  GL_FUNCTION_WRAPPER(glGetIntegerv,(GLenum pname, GLint *params), (pname, params), pname);
  GL_FUNCTION_WRAPPER(glPushAttrib,(GLbitfield mask), (mask), mask);
  GL_FUNCTION_WRAPPER(glPopAttrib,(), (), glcDummyValue);
  GL_FUNCTION_WRAPPER(glPushClientAttrib,(GLbitfield mask), (mask), mask);
  GL_FUNCTION_WRAPPER(glPopClientAttrib,(), (), glcDummyValue);
  GL_FUNCTION_WRAPPER_RET(glRenderMode,(GLenum mode), (mode), mode, GLint);
  //GL_FUNCTION_WRAPPER_RET(glGetError,(GLvoid ), (), glcDummyValue, GLenum);
  GL_FUNCTION_WRAPPER_RET(glGetString,(GLenum name), (name), name, const GLubyte *);
  GL_FUNCTION_WRAPPER(glFinish,(), (), glcDummyValue);
  GL_FUNCTION_WRAPPER(glFlush,(), (), glcDummyValue);
  GL_FUNCTION_WRAPPER(glHint,(GLenum target, GLenum mode), (target, mode), target);
  GL_FUNCTION_WRAPPER(glClearDepth,(GLdouble depth), (depth), depth);
  GL_FUNCTION_WRAPPER(glDepthFunc,(GLenum func), (func), func);
  GL_FUNCTION_WRAPPER(glDepthMask,(GLboolean flag), (flag), flag);
  GL_FUNCTION_WRAPPER(glDepthRange,(GLdouble near_val, GLdouble far_val), (near_val, far_val), near_val);
  GL_FUNCTION_WRAPPER(glClearAccum,(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glAccum,(GLenum op, GLfloat value), (op, value), op);
  GL_FUNCTION_WRAPPER(glMatrixMode,(GLenum mode), (mode), mode);
  GL_FUNCTION_WRAPPER(glOrtho,(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val), (left, right, bottom, top, near_val, far_val), left);
  GL_FUNCTION_WRAPPER(glFrustum,(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val), (left, right, bottom, top, near_val, far_val), left);
  GL_FUNCTION_WRAPPER(glViewport,(GLint x, GLint y, GLsizei width, GLsizei height), (x, y, width, height), x);
  GL_FUNCTION_WRAPPER(glPushMatrix,(), (), glcDummyValue);
  GL_FUNCTION_WRAPPER(glPopMatrix,(), (), glcDummyValue);
  GL_FUNCTION_WRAPPER(glLoadIdentity,(), (), glcDummyValue);
  GL_FUNCTION_WRAPPER(glLoadMatrixd,(const GLdouble *m), (m), m);
  GL_FUNCTION_WRAPPER(glLoadMatrixf,(const GLfloat *m), (m), m);
  GL_FUNCTION_WRAPPER(glMultMatrixd,(const GLdouble *m), (m), m);
  GL_FUNCTION_WRAPPER(glMultMatrixf,(const GLfloat *m), (m), m);
  GL_FUNCTION_WRAPPER(glRotated,(GLdouble angle, GLdouble x, GLdouble y, GLdouble z), (angle, x, y, z), angle);
  GL_FUNCTION_WRAPPER(glRotatef,(GLfloat angle, GLfloat x, GLfloat y, GLfloat z), (angle, x, y, z), angle);
  GL_FUNCTION_WRAPPER(glScaled,(GLdouble x, GLdouble y, GLdouble z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glScalef,(GLfloat x, GLfloat y, GLfloat z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glTranslated,(GLdouble x, GLdouble y, GLdouble z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glTranslatef,(GLfloat x, GLfloat y, GLfloat z), (x, y, z), x);
  GL_FUNCTION_WRAPPER_RET(glIsList,(GLuint list), (list), list, GLboolean);
  GL_FUNCTION_WRAPPER(glDeleteLists,(GLuint list, GLsizei range), (list, range), list);
  GL_FUNCTION_WRAPPER_RET(glGenLists,(GLsizei range), (range), range, GLuint);
  //GL_FUNCTION_WRAPPER(glNewList,(GLuint list, GLenum mode), (list, mode), list);
  //GL_FUNCTION_WRAPPER(glEndList,(GLvoid ), (), glcDummyValue);
  GL_FUNCTION_WRAPPER(glCallList,(GLuint list), (list), list);
  GL_FUNCTION_WRAPPER(glCallLists,(GLsizei n, GLenum type, const GLvoid *lists), (n, type, lists), n);
  GL_FUNCTION_WRAPPER(glListBase,(GLuint base), (base), base);
  //GL_FUNCTION_WRAPPER(glBegin,(GLenum mode), (mode), mode);
  //GL_FUNCTION_WRAPPER(glEnd,(GLvoid ), (), glcDummyValue);
  GL_FUNCTION_WRAPPER(glVertex2d,(GLdouble x, GLdouble y), (x, y), x);
  GL_FUNCTION_WRAPPER(glVertex2f,(GLfloat x, GLfloat y), (x, y), x);
  GL_FUNCTION_WRAPPER(glVertex2i,(GLint x, GLint y), (x, y), x);
  GL_FUNCTION_WRAPPER(glVertex2s,(GLshort x, GLshort y), (x, y), x);
  GL_FUNCTION_WRAPPER(glVertex3d,(GLdouble x, GLdouble y, GLdouble z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glVertex3f,(GLfloat x, GLfloat y, GLfloat z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glVertex3i,(GLint x, GLint y, GLint z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glVertex3s,(GLshort x, GLshort y, GLshort z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glVertex4d,(GLdouble x, GLdouble y, GLdouble z, GLdouble w), (x, y, z, w), x);
  GL_FUNCTION_WRAPPER(glVertex4f,(GLfloat x, GLfloat y, GLfloat z, GLfloat w), (x, y, z, w), x);
  GL_FUNCTION_WRAPPER(glVertex4i,(GLint x, GLint y, GLint z, GLint w), (x, y, z, w), x);
  GL_FUNCTION_WRAPPER(glVertex4s,(GLshort x, GLshort y, GLshort z, GLshort w), (x, y, z, w), x);
  GL_FUNCTION_WRAPPER(glVertex2dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex2fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex2iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex2sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex3dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex3fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex3iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex3sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex4dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex4fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex4iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glVertex4sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glNormal3b,(GLbyte nx, GLbyte ny, GLbyte nz), (nx, ny, nz), nx);
  GL_FUNCTION_WRAPPER(glNormal3d,(GLdouble nx, GLdouble ny, GLdouble nz), (nx, ny, nz), nx);
  GL_FUNCTION_WRAPPER(glNormal3f,(GLfloat nx, GLfloat ny, GLfloat nz), (nx, ny, nz), nx);
  GL_FUNCTION_WRAPPER(glNormal3i,(GLint nx, GLint ny, GLint nz), (nx, ny, nz), nx);
  GL_FUNCTION_WRAPPER(glNormal3s,(GLshort nx, GLshort ny, GLshort nz), (nx, ny, nz), nx);
  GL_FUNCTION_WRAPPER(glNormal3bv,(const GLbyte *v), (v), v);
  GL_FUNCTION_WRAPPER(glNormal3dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glNormal3fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glNormal3iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glNormal3sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glIndexd,(GLdouble c), (c), c);
  GL_FUNCTION_WRAPPER(glIndexf,(GLfloat c), (c), c);
  GL_FUNCTION_WRAPPER(glIndexi,(GLint c), (c), c);
  GL_FUNCTION_WRAPPER(glIndexs,(GLshort c), (c), c);
  GL_FUNCTION_WRAPPER(glIndexub,(GLubyte c), (c), c);
  GL_FUNCTION_WRAPPER(glIndexdv,(const GLdouble *c), (c), c);
  GL_FUNCTION_WRAPPER(glIndexfv,(const GLfloat *c), (c), c);
  GL_FUNCTION_WRAPPER(glIndexiv,(const GLint *c), (c), c);
  GL_FUNCTION_WRAPPER(glIndexsv,(const GLshort *c), (c), c);
  GL_FUNCTION_WRAPPER(glIndexubv,(const GLubyte *c), (c), c);
  GL_FUNCTION_WRAPPER(glColor3b,(GLbyte red, GLbyte green, GLbyte blue), (red, green, blue), red);
  GL_FUNCTION_WRAPPER(glColor3d,(GLdouble red, GLdouble green, GLdouble blue), (red, green, blue), red);
  GL_FUNCTION_WRAPPER(glColor3f,(GLfloat red, GLfloat green, GLfloat blue), (red, green, blue), red);
  GL_FUNCTION_WRAPPER(glColor3i,(GLint red, GLint green, GLint blue), (red, green, blue), red);
  GL_FUNCTION_WRAPPER(glColor3s,(GLshort red, GLshort green, GLshort blue), (red, green, blue), red);
  GL_FUNCTION_WRAPPER(glColor3ub,(GLubyte red, GLubyte green, GLubyte blue), (red, green, blue), red);
  GL_FUNCTION_WRAPPER(glColor3ui,(GLuint red, GLuint green, GLuint blue), (red, green, blue), red);
  GL_FUNCTION_WRAPPER(glColor3us,(GLushort red, GLushort green, GLushort blue), (red, green, blue), red);
  GL_FUNCTION_WRAPPER(glColor4b,(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glColor4d,(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glColor4f,(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glColor4i,(GLint red, GLint green, GLint blue, GLint alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glColor4s,(GLshort red, GLshort green, GLshort blue, GLshort alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glColor4ub,(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glColor4ui,(GLuint red, GLuint green, GLuint blue, GLuint alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glColor4us,(GLushort red, GLushort green, GLushort blue, GLushort alpha), (red, green, blue, alpha), red);
  GL_FUNCTION_WRAPPER(glColor3bv,(const GLbyte *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor3dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor3fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor3iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor3sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor3ubv,(const GLubyte *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor3uiv,(const GLuint *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor3usv,(const GLushort *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor4bv,(const GLbyte *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor4dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor4fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor4iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor4sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor4ubv,(const GLubyte *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor4uiv,(const GLuint *v), (v), v);
  GL_FUNCTION_WRAPPER(glColor4usv,(const GLushort *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord1d,(GLdouble s), (s), s);
  GL_FUNCTION_WRAPPER(glTexCoord1f,(GLfloat s), (s), s);
  GL_FUNCTION_WRAPPER(glTexCoord1i,(GLint s), (s), s);
  GL_FUNCTION_WRAPPER(glTexCoord1s,(GLshort s), (s), s);
  GL_FUNCTION_WRAPPER(glTexCoord2d,(GLdouble s, GLdouble t), (s, t), s);
  GL_FUNCTION_WRAPPER(glTexCoord2f,(GLfloat s, GLfloat t), (s, t), s);
  GL_FUNCTION_WRAPPER(glTexCoord2i,(GLint s, GLint t), (s, t), s);
  GL_FUNCTION_WRAPPER(glTexCoord2s,(GLshort s, GLshort t), (s, t), s);
  GL_FUNCTION_WRAPPER(glTexCoord3d,(GLdouble s, GLdouble t, GLdouble r), (s, t, r), s);
  GL_FUNCTION_WRAPPER(glTexCoord3f,(GLfloat s, GLfloat t, GLfloat r), (s, t, r), s);
  GL_FUNCTION_WRAPPER(glTexCoord3i,(GLint s, GLint t, GLint r), (s, t, r), s);
  GL_FUNCTION_WRAPPER(glTexCoord3s,(GLshort s, GLshort t, GLshort r), (s, t, r), s);
  GL_FUNCTION_WRAPPER(glTexCoord4d,(GLdouble s, GLdouble t, GLdouble r, GLdouble q), (s, t, r, q), s);
  GL_FUNCTION_WRAPPER(glTexCoord4f,(GLfloat s, GLfloat t, GLfloat r, GLfloat q), (s, t, r, q), s);
  GL_FUNCTION_WRAPPER(glTexCoord4i,(GLint s, GLint t, GLint r, GLint q), (s, t, r, q), s);
  GL_FUNCTION_WRAPPER(glTexCoord4s,(GLshort s, GLshort t, GLshort r, GLshort q), (s, t, r, q), s);
  GL_FUNCTION_WRAPPER(glTexCoord1dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord1fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord1iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord1sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord2dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord2fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord2iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord2sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord3dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord3fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord3iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord3sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord4dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord4fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord4iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glTexCoord4sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos2d,(GLdouble x, GLdouble y), (x, y), x);
  GL_FUNCTION_WRAPPER(glRasterPos2f,(GLfloat x, GLfloat y), (x, y), x);
  GL_FUNCTION_WRAPPER(glRasterPos2i,(GLint x, GLint y), (x, y), x);
  GL_FUNCTION_WRAPPER(glRasterPos2s,(GLshort x, GLshort y), (x, y), x);
  GL_FUNCTION_WRAPPER(glRasterPos3d,(GLdouble x, GLdouble y, GLdouble z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glRasterPos3f,(GLfloat x, GLfloat y, GLfloat z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glRasterPos3i,(GLint x, GLint y, GLint z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glRasterPos3s,(GLshort x, GLshort y, GLshort z), (x, y, z), x);
  GL_FUNCTION_WRAPPER(glRasterPos4d,(GLdouble x, GLdouble y, GLdouble z, GLdouble w), (x, y, z, w), x);
  GL_FUNCTION_WRAPPER(glRasterPos4f,(GLfloat x, GLfloat y, GLfloat z, GLfloat w), (x, y, z, w), x);
  GL_FUNCTION_WRAPPER(glRasterPos4i,(GLint x, GLint y, GLint z, GLint w), (x, y, z, w), x);
  GL_FUNCTION_WRAPPER(glRasterPos4s,(GLshort x, GLshort y, GLshort z, GLshort w), (x, y, z, w), x);
  GL_FUNCTION_WRAPPER(glRasterPos2dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos2fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos2iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos2sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos3dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos3fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos3iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos3sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos4dv,(const GLdouble *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos4fv,(const GLfloat *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos4iv,(const GLint *v), (v), v);
  GL_FUNCTION_WRAPPER(glRasterPos4sv,(const GLshort *v), (v), v);
  GL_FUNCTION_WRAPPER(glRectd,(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2), (x1, y1, x2, y2), x1);
  GL_FUNCTION_WRAPPER(glRectf,(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2), (x1, y1, x2, y2), x1);
  GL_FUNCTION_WRAPPER(glRecti,(GLint x1, GLint y1, GLint x2, GLint y2), (x1, y1, x2, y2), x1);
  GL_FUNCTION_WRAPPER(glRects,(GLshort x1, GLshort y1, GLshort x2, GLshort y2), (x1, y1, x2, y2), x1);
  GL_FUNCTION_WRAPPER(glRectdv,(const GLdouble *v1, const GLdouble *v2), (v1, v2), v1);
  GL_FUNCTION_WRAPPER(glRectfv,(const GLfloat *v1, const GLfloat *v2), (v1, v2), v1);
  GL_FUNCTION_WRAPPER(glRectiv,(const GLint *v1, const GLint *v2), (v1, v2), v1);
  GL_FUNCTION_WRAPPER(glRectsv,(const GLshort *v1, const GLshort *v2), (v1, v2), v1);
  GL_FUNCTION_WRAPPER(glVertexPointer,(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr), (size, type, stride, ptr), size);
  GL_FUNCTION_WRAPPER(glNormalPointer,(GLenum type, GLsizei stride,const  GLvoid *ptr), (type, stride, ptr), type);
  GL_FUNCTION_WRAPPER(glColorPointer,(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr), (size, type, stride, ptr), size);
  GL_FUNCTION_WRAPPER(glIndexPointer,(GLenum type, GLsizei stride, const GLvoid *ptr), (type, stride, ptr), type);
  GL_FUNCTION_WRAPPER(glTexCoordPointer,(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr), (size, type, stride, ptr), size);
  GL_FUNCTION_WRAPPER(glEdgeFlagPointer,(GLsizei stride, const GLvoid *ptr), (stride, ptr), stride);
  GL_FUNCTION_WRAPPER(glGetPointerv,(GLenum pname, GLvoid **params), (pname, params), pname);
  GL_FUNCTION_WRAPPER(glArrayElement,(GLint i), (i), i);
  GL_FUNCTION_WRAPPER(glDrawArrays,(GLenum mode, GLint first, GLsizei count), (mode, first, count), mode);
  GL_FUNCTION_WRAPPER(glDrawElements,(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices), (mode, count, type, indices), mode);
  GL_FUNCTION_WRAPPER(glInterleavedArrays,(GLenum format, GLsizei stride, const GLvoid *pointer), (format, stride, pointer), format);
  GL_FUNCTION_WRAPPER(glShadeModel,(GLenum mode), (mode), mode);
  GL_FUNCTION_WRAPPER(glLightf,(GLenum light, GLenum pname, GLfloat param), (light, pname, param), light);
  GL_FUNCTION_WRAPPER(glLighti,(GLenum light, GLenum pname, GLint param), (light, pname, param), light);
  GL_FUNCTION_WRAPPER(glLightfv,(GLenum light, GLenum pname, const GLfloat *params), (light, pname, params), light);
  GL_FUNCTION_WRAPPER(glLightiv,(GLenum light, GLenum pname, const GLint *params), (light, pname, params), light);
  GL_FUNCTION_WRAPPER(glGetLightfv,(GLenum light, GLenum pname, GLfloat *params), (light, pname, params), light);
  GL_FUNCTION_WRAPPER(glGetLightiv,(GLenum light, GLenum pname, GLint *params), (light, pname, params), light);
  GL_FUNCTION_WRAPPER(glLightModelf,(GLenum pname, GLfloat param), (pname, param), pname);
  GL_FUNCTION_WRAPPER(glLightModeli,(GLenum pname, GLint param), (pname, param), pname);
  GL_FUNCTION_WRAPPER(glLightModelfv,(GLenum pname, const GLfloat *params), (pname, params), pname);
  GL_FUNCTION_WRAPPER(glLightModeliv,(GLenum pname, const GLint *params), (pname, params), pname);
  GL_FUNCTION_WRAPPER(glMaterialf,(GLenum face, GLenum pname, GLfloat param), (face, pname, param), face);
  GL_FUNCTION_WRAPPER(glMateriali,(GLenum face, GLenum pname, GLint param), (face, pname, param), face);
  GL_FUNCTION_WRAPPER(glMaterialfv,(GLenum face, GLenum pname, const GLfloat *params), (face, pname, params), face);
  GL_FUNCTION_WRAPPER(glMaterialiv,(GLenum face, GLenum pname, const GLint *params), (face, pname, params), face);
  GL_FUNCTION_WRAPPER(glGetMaterialfv,(GLenum face, GLenum pname, GLfloat *params), (face, pname, params), face);
  GL_FUNCTION_WRAPPER(glGetMaterialiv,(GLenum face, GLenum pname, GLint *params), (face, pname, params), face);
  GL_FUNCTION_WRAPPER(glColorMaterial,(GLenum face, GLenum mode), (face, mode), face);
  GL_FUNCTION_WRAPPER(glPixelZoom,(GLfloat xfactor, GLfloat yfactor), (xfactor, yfactor), xfactor);
  GL_FUNCTION_WRAPPER(glPixelStoref,(GLenum pname, GLfloat param), (pname, param), pname);
  GL_FUNCTION_WRAPPER(glPixelStorei,(GLenum pname, GLint param), (pname, param), pname);
  GL_FUNCTION_WRAPPER(glPixelTransferf,(GLenum pname, GLfloat param), (pname, param), pname);
  GL_FUNCTION_WRAPPER(glPixelTransferi,(GLenum pname, GLint param), (pname, param), pname);
  GL_FUNCTION_WRAPPER(glPixelMapfv,(GLenum map, GLsizei mapsize, const GLfloat *values), (map, mapsize, values), map);
  GL_FUNCTION_WRAPPER(glPixelMapuiv,(GLenum map, GLsizei mapsize, const GLuint *values), (map, mapsize, values), map);
  GL_FUNCTION_WRAPPER(glPixelMapusv,(GLenum map, GLsizei mapsize, const GLushort *values), (map, mapsize, values), map);
  GL_FUNCTION_WRAPPER(glGetPixelMapfv,(GLenum map, GLfloat *values), (map, values), map);
  GL_FUNCTION_WRAPPER(glGetPixelMapuiv,(GLenum map, GLuint *values), (map, values), map);
  GL_FUNCTION_WRAPPER(glGetPixelMapusv,(GLenum map, GLushort *values), (map, values), map);
  GL_FUNCTION_WRAPPER(glBitmap,(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap), (width, height, xorig, yorig, xmove, ymove, bitmap), width);
  GL_FUNCTION_WRAPPER(glReadPixels,(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels), (x, y, width, height, format, type, pixels), x);
  GL_FUNCTION_WRAPPER(glDrawPixels,(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels), (width, height, format, type, pixels), width);
  GL_FUNCTION_WRAPPER(glCopyPixels,(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type), (x, y, width, height, type), x);
  GL_FUNCTION_WRAPPER(glStencilFunc,(GLenum func, GLint ref, GLuint mask), (func, ref, mask), func);
  GL_FUNCTION_WRAPPER(glStencilMask,(GLuint mask), (mask), mask);
  GL_FUNCTION_WRAPPER(glStencilOp,(GLenum fail, GLenum zfail, GLenum zpass), (fail, zfail, zpass), fail);
  GL_FUNCTION_WRAPPER(glClearStencil,(GLint s), (s), s);
  GL_FUNCTION_WRAPPER(glTexGend,(GLenum coord, GLenum pname, GLdouble param), (coord, pname, param), coord);
  GL_FUNCTION_WRAPPER(glTexGenf,(GLenum coord, GLenum pname, GLfloat param), (coord, pname, param), coord);
  GL_FUNCTION_WRAPPER(glTexGeni,(GLenum coord, GLenum pname, GLint param), (coord, pname, param), coord);
  GL_FUNCTION_WRAPPER(glTexGendv,(GLenum coord, GLenum pname, const GLdouble *params), (coord, pname, params), coord);
  GL_FUNCTION_WRAPPER(glTexGenfv,(GLenum coord, GLenum pname, const GLfloat *params), (coord, pname, params), coord);
  GL_FUNCTION_WRAPPER(glTexGeniv,(GLenum coord, GLenum pname, const GLint *params), (coord, pname, params), coord);
  GL_FUNCTION_WRAPPER(glGetTexGendv,(GLenum coord, GLenum pname, GLdouble *params), (coord, pname, params), coord);
  GL_FUNCTION_WRAPPER(glGetTexGenfv,(GLenum coord, GLenum pname, GLfloat *params), (coord, pname, params), coord);
  GL_FUNCTION_WRAPPER(glGetTexGeniv,(GLenum coord, GLenum pname, GLint *params), (coord, pname, params), coord);
  GL_FUNCTION_WRAPPER(glTexEnvf,(GLenum target, GLenum pname, GLfloat param), (target, pname, param), target);
  GL_FUNCTION_WRAPPER(glTexEnvi,(GLenum target, GLenum pname, GLint param), (target, pname, param), target);
  GL_FUNCTION_WRAPPER(glTexEnvfv,(GLenum target, GLenum pname, const GLfloat *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glTexEnviv,(GLenum target, GLenum pname, const GLint *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glGetTexEnvfv,(GLenum target, GLenum pname, GLfloat *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glGetTexEnviv,(GLenum target, GLenum pname, GLint *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glTexParameterf,(GLenum target, GLenum pname, GLfloat param), (target, pname, param), target);
  GL_FUNCTION_WRAPPER(glTexParameteri,(GLenum target, GLenum pname, GLint param), (target, pname, param), target);
  GL_FUNCTION_WRAPPER(glTexParameterfv,(GLenum target, GLenum pname, const GLfloat *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glTexParameteriv,(GLenum target, GLenum pname, const GLint *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glGetTexParameterfv,(GLenum target, GLenum pname, GLfloat *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glGetTexParameteriv,(GLenum target, GLenum pname, GLint *params), (target, pname, params), target);
  GL_FUNCTION_WRAPPER(glGetTexLevelParameterfv,(GLenum target, GLint level, GLenum pname, GLfloat *params), (target, level, pname, params), target);
  GL_FUNCTION_WRAPPER(glGetTexLevelParameteriv,(GLenum target, GLint level, GLenum pname, GLint *params), (target, level, pname, params), target);
  GL_FUNCTION_WRAPPER(glTexImage1D,(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels), (target, level, internalFormat, width, border, format, type, pixels), target);
  GL_FUNCTION_WRAPPER(glTexImage2D,(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels), (target, level, internalFormat, width, height, border, format, type, pixels), target);
  GL_FUNCTION_WRAPPER(glGetTexImage,(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels), (target, level, format, type, pixels), target);
  GL_FUNCTION_WRAPPER(glGenTextures,(GLsizei n, GLuint *textures), (n, textures), n);
  GL_FUNCTION_WRAPPER(glDeleteTextures,(GLsizei n, const GLuint *textures), (n, textures), n);
  GL_FUNCTION_WRAPPER(glBindTexture,(GLenum target, GLuint texture), (target, texture), target);
  GL_FUNCTION_WRAPPER(glPrioritizeTextures,(GLsizei n, const GLuint *textures, const GLfloat *priorities), (n, textures, priorities), n);
  GL_FUNCTION_WRAPPER_RET(glAreTexturesResident,(GLsizei n, const GLuint *textures, GLboolean *residences), (n, textures, residences), n, GLboolean);
  GL_FUNCTION_WRAPPER_RET(glIsTexture,(GLuint texture), (texture), texture, GLboolean);
  GL_FUNCTION_WRAPPER(glTexSubImage1D,(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels), (target, level, xoffset, width, format, type, pixels), target);
  GL_FUNCTION_WRAPPER(glTexSubImage2D,(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels), (target, level, xoffset, yoffset, width, height, format, type, pixels), target);
  GL_FUNCTION_WRAPPER(glCopyTexImage1D,(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border), (target, level, internalformat, x, y, width, border), target);
  GL_FUNCTION_WRAPPER(glCopyTexImage2D,(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border), (target, level, internalformat, x, y, width, height, border), target);
  GL_FUNCTION_WRAPPER(glCopyTexSubImage1D,(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width), (target, level, xoffset, x, y, width), target);
  GL_FUNCTION_WRAPPER(glCopyTexSubImage2D,(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height), (target, level, xoffset, yoffset, x, y, width, height), target);
  GL_FUNCTION_WRAPPER(glMap1d,(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points), (target, u1, u2, stride, order, points), target);
  GL_FUNCTION_WRAPPER(glMap1f,(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points), (target, u1, u2, stride, order, points), target);
  GL_FUNCTION_WRAPPER(glMap2d,(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points), (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points), target);
  GL_FUNCTION_WRAPPER(glMap2f,(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points), (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points), target);
  GL_FUNCTION_WRAPPER(glGetMapdv,(GLenum target, GLenum query, GLdouble *v), (target, query, v), target);
  GL_FUNCTION_WRAPPER(glGetMapfv,(GLenum target, GLenum query, GLfloat *v), (target, query, v), target);
  GL_FUNCTION_WRAPPER(glGetMapiv,(GLenum target, GLenum query, GLint *v), (target, query, v), target);
  GL_FUNCTION_WRAPPER(glEvalCoord1d,(GLdouble u), (u), u);
  GL_FUNCTION_WRAPPER(glEvalCoord1f,(GLfloat u), (u), u);
  GL_FUNCTION_WRAPPER(glEvalCoord1dv,(const GLdouble *u), (u), u);
  GL_FUNCTION_WRAPPER(glEvalCoord1fv,(const GLfloat *u), (u), u);
  GL_FUNCTION_WRAPPER(glEvalCoord2d,(GLdouble u, GLdouble v), (u, v), u);
  GL_FUNCTION_WRAPPER(glEvalCoord2f,(GLfloat u, GLfloat v), (u, v), u);
  GL_FUNCTION_WRAPPER(glEvalCoord2dv,(const GLdouble *u), (u), u);
  GL_FUNCTION_WRAPPER(glEvalCoord2fv,(const GLfloat *u), (u), u);
  GL_FUNCTION_WRAPPER(glMapGrid1d,(GLint un, GLdouble u1, GLdouble u2), (un, u1, u2), un);
  GL_FUNCTION_WRAPPER(glMapGrid1f,(GLint un, GLfloat u1, GLfloat u2), (un, u1, u2), un);
  GL_FUNCTION_WRAPPER(glMapGrid2d,(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2), (un, u1, u2, vn, v1, v2), un);
  GL_FUNCTION_WRAPPER(glMapGrid2f,(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2), (un, u1, u2, vn, v1, v2), un);
  GL_FUNCTION_WRAPPER(glEvalPoint1,(GLint i), (i), i);
  GL_FUNCTION_WRAPPER(glEvalPoint2,(GLint i, GLint j), (i, j), i);
  GL_FUNCTION_WRAPPER(glEvalMesh1,(GLenum mode, GLint i1, GLint i2), (mode, i1, i2), mode);
  GL_FUNCTION_WRAPPER(glEvalMesh2,(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2), (mode, i1, i2, j1, j2), mode);
  GL_FUNCTION_WRAPPER(glFogf,(GLenum pname, GLfloat param), (pname, param), pname);
  GL_FUNCTION_WRAPPER(glFogi,(GLenum pname, GLint param), (pname, param), pname);
  GL_FUNCTION_WRAPPER(glFogfv,(GLenum pname, const GLfloat *params), (pname, params), pname);
  GL_FUNCTION_WRAPPER(glFogiv,(GLenum pname, const GLint *params), (pname, params), pname);
  GL_FUNCTION_WRAPPER(glFeedbackBuffer,(GLsizei size, GLenum type, GLfloat *buffer), (size, type, buffer), size);
  GL_FUNCTION_WRAPPER(glPassThrough,(GLfloat token), (token), token);
  GL_FUNCTION_WRAPPER(glSelectBuffer,(GLsizei size, GLuint *buffer), (size, buffer), size);
  GL_FUNCTION_WRAPPER(glInitNames,(), (), glcDummyValue);
  GL_FUNCTION_WRAPPER(glLoadName,(GLuint name), (name), name);
  GL_FUNCTION_WRAPPER(glPushName,(GLuint name), (name), name);
  GL_FUNCTION_WRAPPER(glPopName,(), (), glcDummyValue);

