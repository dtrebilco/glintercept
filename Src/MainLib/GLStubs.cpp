/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "GLInterceptConfig.h"
#include "GLDriver.h"


USING_ERRORLOG

//The real OpenGL driver
extern GLCoreDriver GLV;

//The version of the OpenGL functions that the builtin wrappers call
// (Functions in this table may be over-ridden/replaced)
extern GLCoreDriver GLV_Builtin;

//The function table of all known functions
extern FunctionTable * functionTable;

///////////////////////////////////////////////////////////////////////////////
//
bool MapCoreOpenGLFunctions(DLLLoader &openGLLib)
{
  void **tempfunc;

  //Test if the function table has been init
  if(!functionTable)
  {
    GLDriver::InitFunctionTable();
  }

#define GLC_FUNC_LOOKUP(string)                 \
  tempfunc = (void**)(&GLV.string);           \
  (*tempfunc) = openGLLib.GetFunction(#string); \
  if(GLV.string == NULL)                      \
  {LOGERR(("Function %s unable to be mapped",#string)) return false;} \
  functionTable->UpdateBuiltinFunction(#string, (void*)GLV.string);

  GLC_FUNC_LOOKUP(glAccum);
  GLC_FUNC_LOOKUP(glAlphaFunc)
  GLC_FUNC_LOOKUP(glAreTexturesResident)
  GLC_FUNC_LOOKUP(glArrayElement)
  GLC_FUNC_LOOKUP(glBegin)
  GLC_FUNC_LOOKUP(glBindTexture)
  GLC_FUNC_LOOKUP(glBitmap)
  GLC_FUNC_LOOKUP(glBlendFunc)
  GLC_FUNC_LOOKUP(glCallList)
  GLC_FUNC_LOOKUP(glCallLists)
  GLC_FUNC_LOOKUP(glClear)
  GLC_FUNC_LOOKUP(glClearAccum)
  GLC_FUNC_LOOKUP(glClearColor)
  GLC_FUNC_LOOKUP(glClearDepth)
  GLC_FUNC_LOOKUP(glClearIndex)
  GLC_FUNC_LOOKUP(glClearStencil)
  GLC_FUNC_LOOKUP(glClipPlane)
  GLC_FUNC_LOOKUP(glColor3b)
  GLC_FUNC_LOOKUP(glColor3bv)
  GLC_FUNC_LOOKUP(glColor3d)
  GLC_FUNC_LOOKUP(glColor3dv)
  GLC_FUNC_LOOKUP(glColor3f)
  GLC_FUNC_LOOKUP(glColor3fv)
  GLC_FUNC_LOOKUP(glColor3i)
  GLC_FUNC_LOOKUP(glColor3iv)
  GLC_FUNC_LOOKUP(glColor3s)
  GLC_FUNC_LOOKUP(glColor3sv)
  GLC_FUNC_LOOKUP(glColor3ub)
  GLC_FUNC_LOOKUP(glColor3ubv)
  GLC_FUNC_LOOKUP(glColor3ui)
  GLC_FUNC_LOOKUP(glColor3uiv)
  GLC_FUNC_LOOKUP(glColor3us)
  GLC_FUNC_LOOKUP(glColor3usv)
  GLC_FUNC_LOOKUP(glColor4b)
  GLC_FUNC_LOOKUP(glColor4bv)
  GLC_FUNC_LOOKUP(glColor4d)
  GLC_FUNC_LOOKUP(glColor4dv)
  GLC_FUNC_LOOKUP(glColor4f)
  GLC_FUNC_LOOKUP(glColor4fv)
  GLC_FUNC_LOOKUP(glColor4i)
  GLC_FUNC_LOOKUP(glColor4iv)
  GLC_FUNC_LOOKUP(glColor4s)
  GLC_FUNC_LOOKUP(glColor4sv)
  GLC_FUNC_LOOKUP(glColor4ub)
  GLC_FUNC_LOOKUP(glColor4ubv)
  GLC_FUNC_LOOKUP(glColor4ui)
  GLC_FUNC_LOOKUP(glColor4uiv)
  GLC_FUNC_LOOKUP(glColor4us)
  GLC_FUNC_LOOKUP(glColor4usv)
  GLC_FUNC_LOOKUP(glColorMask)
  GLC_FUNC_LOOKUP(glColorMaterial)
  GLC_FUNC_LOOKUP(glColorPointer)
  GLC_FUNC_LOOKUP(glCopyPixels)
  GLC_FUNC_LOOKUP(glCopyTexImage1D)
  GLC_FUNC_LOOKUP(glCopyTexImage2D)
  GLC_FUNC_LOOKUP(glCopyTexSubImage1D)
  GLC_FUNC_LOOKUP(glCopyTexSubImage2D)
  GLC_FUNC_LOOKUP(glCullFace)
  GLC_FUNC_LOOKUP(glDeleteLists)
  GLC_FUNC_LOOKUP(glDeleteTextures)
  GLC_FUNC_LOOKUP(glDepthFunc)
  GLC_FUNC_LOOKUP(glDepthMask)
  GLC_FUNC_LOOKUP(glDepthRange)
  GLC_FUNC_LOOKUP(glDisable)
  GLC_FUNC_LOOKUP(glDisableClientState)
  GLC_FUNC_LOOKUP(glDrawArrays)
  GLC_FUNC_LOOKUP(glDrawBuffer)
  GLC_FUNC_LOOKUP(glDrawElements)
  GLC_FUNC_LOOKUP(glDrawPixels)
  GLC_FUNC_LOOKUP(glEdgeFlag)
  GLC_FUNC_LOOKUP(glEdgeFlagPointer)
  GLC_FUNC_LOOKUP(glEdgeFlagv)
  GLC_FUNC_LOOKUP(glEnable)
  GLC_FUNC_LOOKUP(glEnableClientState)
  GLC_FUNC_LOOKUP(glEnd)
  GLC_FUNC_LOOKUP(glEndList)
  GLC_FUNC_LOOKUP(glEvalCoord1d)
  GLC_FUNC_LOOKUP(glEvalCoord1dv)
  GLC_FUNC_LOOKUP(glEvalCoord1f)
  GLC_FUNC_LOOKUP(glEvalCoord1fv)
  GLC_FUNC_LOOKUP(glEvalCoord2d)
  GLC_FUNC_LOOKUP(glEvalCoord2dv)
  GLC_FUNC_LOOKUP(glEvalCoord2f)
  GLC_FUNC_LOOKUP(glEvalCoord2fv)
  GLC_FUNC_LOOKUP(glEvalMesh1)
  GLC_FUNC_LOOKUP(glEvalMesh2)
  GLC_FUNC_LOOKUP(glEvalPoint1)
  GLC_FUNC_LOOKUP(glEvalPoint2)
  GLC_FUNC_LOOKUP(glFeedbackBuffer)
  GLC_FUNC_LOOKUP(glFinish)
  GLC_FUNC_LOOKUP(glFlush)
  GLC_FUNC_LOOKUP(glFogf)
  GLC_FUNC_LOOKUP(glFogfv)
  GLC_FUNC_LOOKUP(glFogi)
  GLC_FUNC_LOOKUP(glFogiv)
  GLC_FUNC_LOOKUP(glFrontFace)
  GLC_FUNC_LOOKUP(glFrustum)
  GLC_FUNC_LOOKUP(glGenLists)
  GLC_FUNC_LOOKUP(glGenTextures)
  GLC_FUNC_LOOKUP(glGetBooleanv)
  GLC_FUNC_LOOKUP(glGetClipPlane)
  GLC_FUNC_LOOKUP(glGetDoublev)
  GLC_FUNC_LOOKUP(glGetError)
  GLC_FUNC_LOOKUP(glGetFloatv)
  GLC_FUNC_LOOKUP(glGetIntegerv)
  GLC_FUNC_LOOKUP(glGetLightfv)
  GLC_FUNC_LOOKUP(glGetLightiv)
  GLC_FUNC_LOOKUP(glGetMapdv)
  GLC_FUNC_LOOKUP(glGetMapfv)
  GLC_FUNC_LOOKUP(glGetMapiv)
  GLC_FUNC_LOOKUP(glGetMaterialfv)
  GLC_FUNC_LOOKUP(glGetMaterialiv)
  GLC_FUNC_LOOKUP(glGetPixelMapfv)
  GLC_FUNC_LOOKUP(glGetPixelMapuiv)
  GLC_FUNC_LOOKUP(glGetPixelMapusv)
  GLC_FUNC_LOOKUP(glGetPointerv)
  GLC_FUNC_LOOKUP(glGetPolygonStipple)
  GLC_FUNC_LOOKUP(glGetString)
  GLC_FUNC_LOOKUP(glGetTexEnvfv)
  GLC_FUNC_LOOKUP(glGetTexEnviv)
  GLC_FUNC_LOOKUP(glGetTexGendv)
  GLC_FUNC_LOOKUP(glGetTexGenfv)
  GLC_FUNC_LOOKUP(glGetTexGeniv)
  GLC_FUNC_LOOKUP(glGetTexImage)
  GLC_FUNC_LOOKUP(glGetTexLevelParameterfv)
  GLC_FUNC_LOOKUP(glGetTexLevelParameteriv)
  GLC_FUNC_LOOKUP(glGetTexParameterfv)
  GLC_FUNC_LOOKUP(glGetTexParameteriv)
  GLC_FUNC_LOOKUP(glHint)
  GLC_FUNC_LOOKUP(glIndexMask)
  GLC_FUNC_LOOKUP(glIndexPointer)
  GLC_FUNC_LOOKUP(glIndexd)
  GLC_FUNC_LOOKUP(glIndexdv)
  GLC_FUNC_LOOKUP(glIndexf)
  GLC_FUNC_LOOKUP(glIndexfv)
  GLC_FUNC_LOOKUP(glIndexi)
  GLC_FUNC_LOOKUP(glIndexiv)
  GLC_FUNC_LOOKUP(glIndexs)
  GLC_FUNC_LOOKUP(glIndexsv)
  GLC_FUNC_LOOKUP(glIndexub)
  GLC_FUNC_LOOKUP(glIndexubv)
  GLC_FUNC_LOOKUP(glInitNames)
  GLC_FUNC_LOOKUP(glInterleavedArrays)
  GLC_FUNC_LOOKUP(glIsEnabled)
  GLC_FUNC_LOOKUP(glIsList)
  GLC_FUNC_LOOKUP(glIsTexture)
  GLC_FUNC_LOOKUP(glLightModelf)
  GLC_FUNC_LOOKUP(glLightModelfv)
  GLC_FUNC_LOOKUP(glLightModeli)
  GLC_FUNC_LOOKUP(glLightModeliv)
  GLC_FUNC_LOOKUP(glLightf)
  GLC_FUNC_LOOKUP(glLightfv)
  GLC_FUNC_LOOKUP(glLighti)
  GLC_FUNC_LOOKUP(glLightiv)
  GLC_FUNC_LOOKUP(glLineStipple)
  GLC_FUNC_LOOKUP(glLineWidth)
  GLC_FUNC_LOOKUP(glListBase)
  GLC_FUNC_LOOKUP(glLoadIdentity)
  GLC_FUNC_LOOKUP(glLoadMatrixd)
  GLC_FUNC_LOOKUP(glLoadMatrixf)
  GLC_FUNC_LOOKUP(glLoadName)
  GLC_FUNC_LOOKUP(glLogicOp)
  GLC_FUNC_LOOKUP(glMap1d)
  GLC_FUNC_LOOKUP(glMap1f)
  GLC_FUNC_LOOKUP(glMap2d)
  GLC_FUNC_LOOKUP(glMap2f)
  GLC_FUNC_LOOKUP(glMapGrid1d)
  GLC_FUNC_LOOKUP(glMapGrid1f)
  GLC_FUNC_LOOKUP(glMapGrid2d)
  GLC_FUNC_LOOKUP(glMapGrid2f)
  GLC_FUNC_LOOKUP(glMaterialf)
  GLC_FUNC_LOOKUP(glMaterialfv)
  GLC_FUNC_LOOKUP(glMateriali)
  GLC_FUNC_LOOKUP(glMaterialiv)
  GLC_FUNC_LOOKUP(glMatrixMode)
  GLC_FUNC_LOOKUP(glMultMatrixd)
  GLC_FUNC_LOOKUP(glMultMatrixf)
  GLC_FUNC_LOOKUP(glNewList)
  GLC_FUNC_LOOKUP(glNormal3b)
  GLC_FUNC_LOOKUP(glNormal3bv)
  GLC_FUNC_LOOKUP(glNormal3d)
  GLC_FUNC_LOOKUP(glNormal3dv)
  GLC_FUNC_LOOKUP(glNormal3f)
  GLC_FUNC_LOOKUP(glNormal3fv)
  GLC_FUNC_LOOKUP(glNormal3i)
  GLC_FUNC_LOOKUP(glNormal3iv)
  GLC_FUNC_LOOKUP(glNormal3s)
  GLC_FUNC_LOOKUP(glNormal3sv)
  GLC_FUNC_LOOKUP(glNormalPointer)
  GLC_FUNC_LOOKUP(glOrtho)
  GLC_FUNC_LOOKUP(glPassThrough)
  GLC_FUNC_LOOKUP(glPixelMapfv)
  GLC_FUNC_LOOKUP(glPixelMapuiv)
  GLC_FUNC_LOOKUP(glPixelMapusv)
  GLC_FUNC_LOOKUP(glPixelStoref)
  GLC_FUNC_LOOKUP(glPixelStorei)
  GLC_FUNC_LOOKUP(glPixelTransferf)
  GLC_FUNC_LOOKUP(glPixelTransferi)
  GLC_FUNC_LOOKUP(glPixelZoom)
  GLC_FUNC_LOOKUP(glPointSize)
  GLC_FUNC_LOOKUP(glPolygonMode)
  GLC_FUNC_LOOKUP(glPolygonOffset)
  GLC_FUNC_LOOKUP(glPolygonStipple)
  GLC_FUNC_LOOKUP(glPopAttrib)
  GLC_FUNC_LOOKUP(glPopClientAttrib)
  GLC_FUNC_LOOKUP(glPopMatrix)
  GLC_FUNC_LOOKUP(glPopName)
  GLC_FUNC_LOOKUP(glPrioritizeTextures)
  GLC_FUNC_LOOKUP(glPushAttrib)
  GLC_FUNC_LOOKUP(glPushClientAttrib)
  GLC_FUNC_LOOKUP(glPushMatrix)
  GLC_FUNC_LOOKUP(glPushName)
  GLC_FUNC_LOOKUP(glRasterPos2d)
  GLC_FUNC_LOOKUP(glRasterPos2dv)
  GLC_FUNC_LOOKUP(glRasterPos2f)
  GLC_FUNC_LOOKUP(glRasterPos2fv)
  GLC_FUNC_LOOKUP(glRasterPos2i)
  GLC_FUNC_LOOKUP(glRasterPos2iv)
  GLC_FUNC_LOOKUP(glRasterPos2s)
  GLC_FUNC_LOOKUP(glRasterPos2sv)
  GLC_FUNC_LOOKUP(glRasterPos3d)
  GLC_FUNC_LOOKUP(glRasterPos3dv)
  GLC_FUNC_LOOKUP(glRasterPos3f)
  GLC_FUNC_LOOKUP(glRasterPos3fv)
  GLC_FUNC_LOOKUP(glRasterPos3i)
  GLC_FUNC_LOOKUP(glRasterPos3iv)
  GLC_FUNC_LOOKUP(glRasterPos3s)
  GLC_FUNC_LOOKUP(glRasterPos3sv)
  GLC_FUNC_LOOKUP(glRasterPos4d)
  GLC_FUNC_LOOKUP(glRasterPos4dv)
  GLC_FUNC_LOOKUP(glRasterPos4f)
  GLC_FUNC_LOOKUP(glRasterPos4fv)
  GLC_FUNC_LOOKUP(glRasterPos4i)
  GLC_FUNC_LOOKUP(glRasterPos4iv)
  GLC_FUNC_LOOKUP(glRasterPos4s)
  GLC_FUNC_LOOKUP(glRasterPos4sv)
  GLC_FUNC_LOOKUP(glReadBuffer)
  GLC_FUNC_LOOKUP(glReadPixels)
  GLC_FUNC_LOOKUP(glRectd)
  GLC_FUNC_LOOKUP(glRectdv)
  GLC_FUNC_LOOKUP(glRectf)
  GLC_FUNC_LOOKUP(glRectfv)
  GLC_FUNC_LOOKUP(glRecti)
  GLC_FUNC_LOOKUP(glRectiv)
  GLC_FUNC_LOOKUP(glRects)
  GLC_FUNC_LOOKUP(glRectsv)
  GLC_FUNC_LOOKUP(glRenderMode)
  GLC_FUNC_LOOKUP(glRotated)
  GLC_FUNC_LOOKUP(glRotatef)
  GLC_FUNC_LOOKUP(glScaled)
  GLC_FUNC_LOOKUP(glScalef)
  GLC_FUNC_LOOKUP(glScissor)
  GLC_FUNC_LOOKUP(glSelectBuffer)
  GLC_FUNC_LOOKUP(glShadeModel)
  GLC_FUNC_LOOKUP(glStencilFunc)
  GLC_FUNC_LOOKUP(glStencilMask)
  GLC_FUNC_LOOKUP(glStencilOp)
  GLC_FUNC_LOOKUP(glTexCoord1d)
  GLC_FUNC_LOOKUP(glTexCoord1dv)
  GLC_FUNC_LOOKUP(glTexCoord1f)
  GLC_FUNC_LOOKUP(glTexCoord1fv)
  GLC_FUNC_LOOKUP(glTexCoord1i)
  GLC_FUNC_LOOKUP(glTexCoord1iv)
  GLC_FUNC_LOOKUP(glTexCoord1s)
  GLC_FUNC_LOOKUP(glTexCoord1sv)
  GLC_FUNC_LOOKUP(glTexCoord2d)
  GLC_FUNC_LOOKUP(glTexCoord2dv)
  GLC_FUNC_LOOKUP(glTexCoord2f)
  GLC_FUNC_LOOKUP(glTexCoord2fv)
  GLC_FUNC_LOOKUP(glTexCoord2i)
  GLC_FUNC_LOOKUP(glTexCoord2iv)
  GLC_FUNC_LOOKUP(glTexCoord2s)
  GLC_FUNC_LOOKUP(glTexCoord2sv)
  GLC_FUNC_LOOKUP(glTexCoord3d)
  GLC_FUNC_LOOKUP(glTexCoord3dv)
  GLC_FUNC_LOOKUP(glTexCoord3f)
  GLC_FUNC_LOOKUP(glTexCoord3fv)
  GLC_FUNC_LOOKUP(glTexCoord3i)
  GLC_FUNC_LOOKUP(glTexCoord3iv)
  GLC_FUNC_LOOKUP(glTexCoord3s)
  GLC_FUNC_LOOKUP(glTexCoord3sv)
  GLC_FUNC_LOOKUP(glTexCoord4d)
  GLC_FUNC_LOOKUP(glTexCoord4dv)
  GLC_FUNC_LOOKUP(glTexCoord4f)
  GLC_FUNC_LOOKUP(glTexCoord4fv)
  GLC_FUNC_LOOKUP(glTexCoord4i)
  GLC_FUNC_LOOKUP(glTexCoord4iv)
  GLC_FUNC_LOOKUP(glTexCoord4s)
  GLC_FUNC_LOOKUP(glTexCoord4sv)
  GLC_FUNC_LOOKUP(glTexCoordPointer)
  GLC_FUNC_LOOKUP(glTexEnvf)
  GLC_FUNC_LOOKUP(glTexEnvfv)
  GLC_FUNC_LOOKUP(glTexEnvi)
  GLC_FUNC_LOOKUP(glTexEnviv)
  GLC_FUNC_LOOKUP(glTexGend)
  GLC_FUNC_LOOKUP(glTexGendv)
  GLC_FUNC_LOOKUP(glTexGenf)
  GLC_FUNC_LOOKUP(glTexGenfv)
  GLC_FUNC_LOOKUP(glTexGeni)
  GLC_FUNC_LOOKUP(glTexGeniv)
  GLC_FUNC_LOOKUP(glTexImage1D)
  GLC_FUNC_LOOKUP(glTexImage2D)
  GLC_FUNC_LOOKUP(glTexParameterf)
  GLC_FUNC_LOOKUP(glTexParameterfv)
  GLC_FUNC_LOOKUP(glTexParameteri)
  GLC_FUNC_LOOKUP(glTexParameteriv)
  GLC_FUNC_LOOKUP(glTexSubImage1D)
  GLC_FUNC_LOOKUP(glTexSubImage2D)
  GLC_FUNC_LOOKUP(glTranslated)
  GLC_FUNC_LOOKUP(glTranslatef)
  GLC_FUNC_LOOKUP(glVertex2d)
  GLC_FUNC_LOOKUP(glVertex2dv)
  GLC_FUNC_LOOKUP(glVertex2f)
  GLC_FUNC_LOOKUP(glVertex2fv)
  GLC_FUNC_LOOKUP(glVertex2i)
  GLC_FUNC_LOOKUP(glVertex2iv)
  GLC_FUNC_LOOKUP(glVertex2s)
  GLC_FUNC_LOOKUP(glVertex2sv)
  GLC_FUNC_LOOKUP(glVertex3d)
  GLC_FUNC_LOOKUP(glVertex3dv)
  GLC_FUNC_LOOKUP(glVertex3f)
  GLC_FUNC_LOOKUP(glVertex3fv)
  GLC_FUNC_LOOKUP(glVertex3i)
  GLC_FUNC_LOOKUP(glVertex3iv)
  GLC_FUNC_LOOKUP(glVertex3s)
  GLC_FUNC_LOOKUP(glVertex3sv)
  GLC_FUNC_LOOKUP(glVertex4d)
  GLC_FUNC_LOOKUP(glVertex4dv)
  GLC_FUNC_LOOKUP(glVertex4f)
  GLC_FUNC_LOOKUP(glVertex4fv)
  GLC_FUNC_LOOKUP(glVertex4i)
  GLC_FUNC_LOOKUP(glVertex4iv)
  GLC_FUNC_LOOKUP(glVertex4s)
  GLC_FUNC_LOOKUP(glVertex4sv)
  GLC_FUNC_LOOKUP(glVertexPointer)
  GLC_FUNC_LOOKUP(glViewport)

#ifdef GLI_BUILD_LINUX

  //OpenGL 1.2
  GLC_FUNC_LOOKUP(glDrawRangeElements)
  GLC_FUNC_LOOKUP(glTexImage3D)
  GLC_FUNC_LOOKUP(glTexSubImage3D)
  GLC_FUNC_LOOKUP(glCopyTexSubImage3D)
  GLC_FUNC_LOOKUP(glColorTable)
  GLC_FUNC_LOOKUP(glColorSubTable)
  GLC_FUNC_LOOKUP(glColorTableParameteriv)
  GLC_FUNC_LOOKUP(glColorTableParameterfv)
  GLC_FUNC_LOOKUP(glCopyColorSubTable)
  GLC_FUNC_LOOKUP(glCopyColorTable)
  GLC_FUNC_LOOKUP(glGetColorTable)
  GLC_FUNC_LOOKUP(glGetColorTableParameterfv)
  GLC_FUNC_LOOKUP(glGetColorTableParameteriv)
  GLC_FUNC_LOOKUP(glBlendEquation)
  GLC_FUNC_LOOKUP(glBlendColor)
  GLC_FUNC_LOOKUP(glHistogram)
  GLC_FUNC_LOOKUP(glResetHistogram)
  GLC_FUNC_LOOKUP(glGetHistogram)
  GLC_FUNC_LOOKUP(glGetHistogramParameterfv)
  GLC_FUNC_LOOKUP(glGetHistogramParameteriv)
  GLC_FUNC_LOOKUP(glMinmax)
  GLC_FUNC_LOOKUP(glResetMinmax)
  GLC_FUNC_LOOKUP(glGetMinmax)
  GLC_FUNC_LOOKUP(glGetMinmaxParameterfv)
  GLC_FUNC_LOOKUP(glGetMinmaxParameteriv)
  GLC_FUNC_LOOKUP(glConvolutionFilter1D)
  GLC_FUNC_LOOKUP(glConvolutionFilter2D)
  GLC_FUNC_LOOKUP(glConvolutionParameterf)
  GLC_FUNC_LOOKUP(glConvolutionParameterfv)
  GLC_FUNC_LOOKUP(glConvolutionParameteri)
  GLC_FUNC_LOOKUP(glConvolutionParameteriv)
  GLC_FUNC_LOOKUP(glCopyConvolutionFilter1D)
  GLC_FUNC_LOOKUP(glCopyConvolutionFilter2D)
  GLC_FUNC_LOOKUP(glGetConvolutionFilter)
  GLC_FUNC_LOOKUP(glGetConvolutionParameterfv)
  GLC_FUNC_LOOKUP(glGetConvolutionParameteriv)
  GLC_FUNC_LOOKUP(glSeparableFilter2D)
  GLC_FUNC_LOOKUP(glGetSeparableFilter)

  //ARB multitexture
  GLC_FUNC_LOOKUP(glActiveTextureARB)
  GLC_FUNC_LOOKUP(glClientActiveTextureARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord1dARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord1dvARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord1fARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord1fvARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord1iARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord1ivARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord1sARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord1svARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord2dARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord2dvARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord2fARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord2fvARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord2iARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord2ivARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord2sARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord2svARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord3dARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord3dvARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord3fARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord3fvARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord3iARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord3ivARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord3sARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord3svARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord4dARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord4dvARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord4fARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord4fvARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord4iARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord4ivARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord4sARB)
  GLC_FUNC_LOOKUP(glMultiTexCoord4svARB)

#endif //GLI_BUILD_LINUX

#undef GLC_FUNC_LOOKUP

  //Assign the structure of functions that the built-in wrappers call
  GLV_Builtin = GLV;

  return true;
}



#define DUMMY_LOG(name) LOGERR(("Call to %s made outside of context/init",#name));


///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglAccum (GLenum op, GLfloat value)
{
  DUMMY_LOG(glAccum);
}


///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglAlphaFunc (GLenum func, GLfloat ref)
{
  DUMMY_LOG(glAlphaFunc);
}

///////////////////////////////////////////////////////////////////////////////
//
GLboolean GLAPIENTRY DummyglAreTexturesResident (GLsizei n, const GLuint *textures, GLboolean *residences)
{
  DUMMY_LOG(glAreTexturesResident);

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglArrayElement (GLint i)
{
  DUMMY_LOG(glArrayElement);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglBegin (GLenum mode)
{
  DUMMY_LOG(glBegin);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglBindTexture (GLenum target, GLuint texture)
{
  DUMMY_LOG(glBindTexture);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglBitmap (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
{
  DUMMY_LOG(glBitmap);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglBlendFunc (GLenum sfactor, GLenum dfactor)
{
  DUMMY_LOG(glBlendFunc);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCallList (GLuint list)
{
  DUMMY_LOG(glCallList);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCallLists (GLsizei n, GLenum type, const GLvoid *lists)
{
  DUMMY_LOG(glCallLists);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglClear (GLbitfield mask)
{
  DUMMY_LOG(glClear);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglClearAccum (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  DUMMY_LOG(glClearAccum);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  DUMMY_LOG(glClearColor);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglClearDepth (GLdouble depth)
{
  DUMMY_LOG(glClearDepth);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglClearIndex (GLfloat c)
{
  DUMMY_LOG(glClearIndex);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglClearStencil (GLint s)
{
  DUMMY_LOG(glClearStencil);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglClipPlane (GLenum plane, const GLdouble *equation)
{
  DUMMY_LOG(glClipPlane);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3b (GLbyte red, GLbyte green, GLbyte blue)
{
  DUMMY_LOG(glColor3b);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3bv (const GLbyte *v)
{
  DUMMY_LOG(glColor3bv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3d (GLdouble red, GLdouble green, GLdouble blue)
{
  DUMMY_LOG(glColor3d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3dv (const GLdouble *v)
{
  DUMMY_LOG(glColor3dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3f (GLfloat red, GLfloat green, GLfloat blue)
{
  DUMMY_LOG(glColor3f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3fv (const GLfloat *v)
{
  DUMMY_LOG(glColor3fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3i (GLint red, GLint green, GLint blue)
{
  DUMMY_LOG(glColor3i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3iv (const GLint *v)
{
  DUMMY_LOG(glColor3iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3s (GLshort red, GLshort green, GLshort blue)
{
  DUMMY_LOG(glColor3s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3sv (const GLshort *v)
{
  DUMMY_LOG(glColor3sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3ub (GLubyte red, GLubyte green, GLubyte blue)
{
  DUMMY_LOG(glColor3ub);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3ubv (const GLubyte *v)
{
  DUMMY_LOG(glColor3ubv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3ui (GLuint red, GLuint green, GLuint blue)
{
  DUMMY_LOG(glColor3ui);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3uiv (const GLuint *v)
{
  DUMMY_LOG(glColor3uiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3us (GLushort red, GLushort green, GLushort blue)
{
  DUMMY_LOG(glColor3us);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor3usv (const GLushort *v)
{
  DUMMY_LOG(glColor3usv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4b (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
{
  DUMMY_LOG(glColor4b);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4bv (const GLbyte *v)
{
  DUMMY_LOG(glColor4bv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4d (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
  DUMMY_LOG(glColor4d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4dv (const GLdouble *v)
{
  DUMMY_LOG(glColor4dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  DUMMY_LOG(glColor4f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4fv (const GLfloat *v)
{
  DUMMY_LOG(glColor4fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4i (GLint red, GLint green, GLint blue, GLint alpha)
{
  DUMMY_LOG(glColor4i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4iv (const GLint *v)
{                  
  DUMMY_LOG(glColor4iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4s (GLshort red, GLshort green, GLshort blue, GLshort alpha)
{
  DUMMY_LOG(glColor4s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4sv (const GLshort *v)
{
  DUMMY_LOG(glColor4sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
  DUMMY_LOG(glColor4ub);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4ubv (const GLubyte *v)
{
  DUMMY_LOG(glColor4ubv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4ui (GLuint red, GLuint green, GLuint blue, GLuint alpha)
{
  DUMMY_LOG(glColor4ui);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4uiv (const GLuint *v)
{
  DUMMY_LOG(glColor4uiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4us (GLushort red, GLushort green, GLushort blue, GLushort alpha)
{
  DUMMY_LOG(glColor4us);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColor4usv (const GLushort *v)
{
  DUMMY_LOG(glColor4usv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
  DUMMY_LOG(glColorMask);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColorMaterial (GLenum face, GLenum mode)
{
  DUMMY_LOG(glColorMaterial);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  DUMMY_LOG(glColorPointer);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCopyPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
{
  DUMMY_LOG(glCopyPixels);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCopyTexImage1D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border)
{
  DUMMY_LOG(glCopyTexImage1D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
  DUMMY_LOG(glCopyTexImage2D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
  DUMMY_LOG(glCopyTexSubImage1D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
  DUMMY_LOG(glCopyTexSubImage2D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCullFace (GLenum mode)
{
  DUMMY_LOG(glCullFace);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDeleteLists (GLuint list, GLsizei range)
{
  DUMMY_LOG(glDeleteLists);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDeleteTextures (GLsizei n, const GLuint *textures)
{
  DUMMY_LOG(glDeleteTextures);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDepthFunc (GLenum func)
{
  DUMMY_LOG(glDepthFunc);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDepthMask (GLboolean flag)
{
  DUMMY_LOG(glDepthMask);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDepthRange (GLdouble zNear, GLdouble zFar)
{
  DUMMY_LOG(glDepthRange);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDisable (GLenum cap)
{
  DUMMY_LOG(glDisable);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDisableClientState (GLenum array)
{
  DUMMY_LOG(glDisableClientState);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDrawArrays (GLenum mode, GLint first, GLsizei count)
{
  DUMMY_LOG(glDrawArrays);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDrawBuffer (GLenum mode)
{
  DUMMY_LOG(glDrawBuffer);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
  DUMMY_LOG(glDrawElements);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDrawPixels (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
  DUMMY_LOG(glDrawPixels);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEdgeFlag (GLboolean flag)
{
  DUMMY_LOG(glEdgeFlag);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEdgeFlagPointer (GLsizei stride, const GLvoid *pointer)
{
  DUMMY_LOG(glEdgeFlagPointer);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEdgeFlagv (const GLboolean *flag)
{
  DUMMY_LOG(glEdgeFlagv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEnable (GLenum cap)
{
  DUMMY_LOG(glEnable);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEnableClientState (GLenum array)
{
  DUMMY_LOG(glEnableClientState);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEnd ()
{
  DUMMY_LOG(glEnd);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEndList ()
{
  DUMMY_LOG(glEndList);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalCoord1d (GLdouble u)
{
  DUMMY_LOG(glEvalCoord1d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalCoord1dv (const GLdouble *u)
{
  DUMMY_LOG(glEvalCoord1dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalCoord1f (GLfloat u)
{
  DUMMY_LOG(glEvalCoord1f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalCoord1fv (const GLfloat *u)
{
  DUMMY_LOG(glEvalCoord1fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalCoord2d (GLdouble u, GLdouble v)
{
  DUMMY_LOG(glEvalCoord2d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalCoord2dv (const GLdouble *u)
{
  DUMMY_LOG(glEvalCoord2dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalCoord2f (GLfloat u, GLfloat v)
{
  DUMMY_LOG(glEvalCoord2f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalCoord2fv (const GLfloat *u)
{
  DUMMY_LOG(glEvalCoord2fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalMesh1 (GLenum mode, GLint i1, GLint i2)
{
  DUMMY_LOG(glEvalMesh1);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalMesh2 (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
{
  DUMMY_LOG(glEvalMesh2);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalPoint1 (GLint i)
{
  DUMMY_LOG(glEvalPoint1);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglEvalPoint2 (GLint i, GLint j)
{
  DUMMY_LOG(glEvalPoint2);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer)
{
  DUMMY_LOG(glFeedbackBuffer);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglFinish ()
{
  DUMMY_LOG(glFinish);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglFlush ()
{
  DUMMY_LOG(glFlush);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglFogf (GLenum pname, GLfloat param)
{
  DUMMY_LOG(glFogf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglFogfv (GLenum pname, const GLfloat *params)
{
  DUMMY_LOG(glFogfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglFogi (GLenum pname, GLint param)
{
  DUMMY_LOG(glFogi);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglFogiv (GLenum pname, const GLint *params)
{
  DUMMY_LOG(glFogiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglFrontFace (GLenum mode)
{
  DUMMY_LOG(glFrontFace);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
  DUMMY_LOG(glFrustum);
}

///////////////////////////////////////////////////////////////////////////////
//
GLuint GLAPIENTRY DummyglGenLists (GLsizei range)
{
  DUMMY_LOG(glGenLists);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGenTextures (GLsizei n, GLuint *textures)
{
  DUMMY_LOG(glGenTextures);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetBooleanv (GLenum pname, GLboolean *params)
{
  DUMMY_LOG(glGetBooleanv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetClipPlane (GLenum plane, GLdouble *equation)
{
  DUMMY_LOG(glGetClipPlane);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetDoublev (GLenum pname, GLdouble *params)
{
  DUMMY_LOG(glGetDoublev);
}

///////////////////////////////////////////////////////////////////////////////
//
GLenum GLAPIENTRY DummyglGetError ()
{
  DUMMY_LOG(glGetError);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetFloatv (GLenum pname, GLfloat *params)
{
  DUMMY_LOG(glGetFloatv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetIntegerv (GLenum pname, GLint *params)
{
  DUMMY_LOG(glGetIntegerv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetLightfv (GLenum light, GLenum pname, GLfloat *params)
{
  DUMMY_LOG(glGetLightfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetLightiv (GLenum light, GLenum pname, GLint *params)
{
  DUMMY_LOG(glGetLightiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetMapdv (GLenum target, GLenum query, GLdouble *v)
{
  DUMMY_LOG(glGetMapdv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetMapfv (GLenum target, GLenum query, GLfloat *v)
{
  DUMMY_LOG(glGetMapfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetMapiv (GLenum target, GLenum query, GLint *v)
{
  DUMMY_LOG(glGetMapiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetMaterialfv (GLenum face, GLenum pname, GLfloat *params)
{
  DUMMY_LOG(glGetMaterialfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetMaterialiv (GLenum face, GLenum pname, GLint *params)
{
  DUMMY_LOG(glGetMaterialiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetPixelMapfv (GLenum map, GLfloat *values)
{
  DUMMY_LOG(glGetPixelMapfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetPixelMapuiv (GLenum map, GLuint *values)
{
  DUMMY_LOG(glGetPixelMapuiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetPixelMapusv (GLenum map, GLushort *values)
{
  DUMMY_LOG(glGetPixelMapusv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetPointerv (GLenum pname, GLvoid* *params)
{
  DUMMY_LOG(glGetPointerv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetPolygonStipple (GLubyte *mask)
{
  DUMMY_LOG(glGetPolygonStipple);
}

///////////////////////////////////////////////////////////////////////////////
//
const GLubyte* GLAPIENTRY DummyglGetString (GLenum name)
{
  DUMMY_LOG(glGetString);
  static GLubyte Dummystring[] = "Dummy String";
  return Dummystring;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetTexEnvfv (GLenum target, GLenum pname, GLfloat *params)
{
  DUMMY_LOG(glGetTexEnvfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetTexEnviv (GLenum target, GLenum pname, GLint *params)
{
  DUMMY_LOG(glGetTexEnviv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetTexGendv (GLenum coord, GLenum pname, GLdouble *params)
{
  DUMMY_LOG(glGetTexGendv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetTexGenfv (GLenum coord, GLenum pname, GLfloat *params)
{
  DUMMY_LOG(glGetTexGenfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetTexGeniv (GLenum coord, GLenum pname, GLint *params)
{
  DUMMY_LOG(glGetTexGeniv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetTexImage (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
{
  DUMMY_LOG(glGetTexImage);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname, GLfloat *params)
{
  DUMMY_LOG(glGetTexLevelParameterfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params)
{
  DUMMY_LOG(glGetTexLevelParameteriv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params)
{
  DUMMY_LOG(glGetTexParameterfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetTexParameteriv (GLenum target, GLenum pname, GLint *params)
{
  DUMMY_LOG(glGetTexParameteriv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglHint (GLenum target, GLenum mode)
{
  DUMMY_LOG(glHint);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexMask (GLuint mask)
{
  DUMMY_LOG(glIndexMask);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexPointer (GLenum type, GLsizei stride, const GLvoid *pointer)
{
  DUMMY_LOG(glIndexPointer);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexd (GLdouble c)
{
  DUMMY_LOG(glIndexd);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexdv (const GLdouble *c)
{
  DUMMY_LOG(glIndexdv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexf (GLfloat c)
{
  DUMMY_LOG(glIndexf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexfv (const GLfloat *c)
{
  DUMMY_LOG(glIndexfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexi (GLint c)
{
  DUMMY_LOG(glIndexi);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexiv (const GLint *c)
{
  DUMMY_LOG(glIndexiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexs (GLshort c)
{
  DUMMY_LOG(glIndexs);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexsv (const GLshort *c)
{
  DUMMY_LOG(glIndexsv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexub (GLubyte c)
{
  DUMMY_LOG(glIndexub);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglIndexubv (const GLubyte *c)
{
  DUMMY_LOG(glIndexubv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglInitNames ()
{
  DUMMY_LOG(glInitNames);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglInterleavedArrays (GLenum format, GLsizei stride, const GLvoid *pointer)
{
  DUMMY_LOG(glInterleavedArrays);
}

///////////////////////////////////////////////////////////////////////////////
//
GLboolean GLAPIENTRY DummyglIsEnabled (GLenum cap)
{
  DUMMY_LOG(glIsEnabled);
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
GLboolean GLAPIENTRY DummyglIsList (GLuint list)
{
  DUMMY_LOG(glIsList);
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
GLboolean GLAPIENTRY DummyglIsTexture (GLuint texture)
{
  DUMMY_LOG(glIsTexture);
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLightModelf (GLenum pname, GLfloat param)
{
  DUMMY_LOG(glLightModelf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLightModelfv (GLenum pname, const GLfloat *params)
{
  DUMMY_LOG(glLightModelfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLightModeli (GLenum pname, GLint param)
{
  DUMMY_LOG(glLightModeli);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLightModeliv (GLenum pname, const GLint *params)
{
  DUMMY_LOG(glLightModeliv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLightf (GLenum light, GLenum pname, GLfloat param)
{
  DUMMY_LOG(glLightf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLightfv (GLenum light, GLenum pname, const GLfloat *params)
{
  DUMMY_LOG(glLightfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLighti (GLenum light, GLenum pname, GLint param)
{
  DUMMY_LOG(glLighti);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLightiv (GLenum light, GLenum pname, const GLint *params)
{
  DUMMY_LOG(glLightiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLineStipple (GLint factor, GLushort pattern)
{
  DUMMY_LOG(glLineStipple);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLineWidth (GLfloat width)
{
  DUMMY_LOG(glLineWidth);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglListBase (GLuint base)
{
  DUMMY_LOG(glListBase);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLoadIdentity ()
{
  DUMMY_LOG(glLoadIdentity);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLoadMatrixd (const GLdouble *m)
{
  DUMMY_LOG(glLoadMatrixd);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLoadMatrixf (const GLfloat *m)
{
  DUMMY_LOG(glLoadMatrixf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLoadName (GLuint name)
{
  DUMMY_LOG(glLoadName);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglLogicOp (GLenum opcode)
{
  DUMMY_LOG(glLogicOp);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMap1d (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points)
{
  DUMMY_LOG(glMap1d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMap1f (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points)
{
  DUMMY_LOG(glMap1f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMap2d (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points)
{
  DUMMY_LOG(glMap2d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMap2f (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points)
{
  DUMMY_LOG(glMap2f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMapGrid1d (GLint un, GLdouble u1, GLdouble u2)
{
  DUMMY_LOG(glMapGrid1d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMapGrid1f (GLint un, GLfloat u1, GLfloat u2)
{
  DUMMY_LOG(glMapGrid1f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMapGrid2d (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2)
{
  DUMMY_LOG(glMapGrid2d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMapGrid2f (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2)
{
  DUMMY_LOG(glMapGrid2f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMaterialf (GLenum face, GLenum pname, GLfloat param)
{
  DUMMY_LOG(glMaterialf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMaterialfv (GLenum face, GLenum pname, const GLfloat *params)
{
  DUMMY_LOG(glMaterialfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMateriali (GLenum face, GLenum pname, GLint param)
{
  DUMMY_LOG(glMateriali);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMaterialiv (GLenum face, GLenum pname, const GLint *params)
{
  DUMMY_LOG(glMaterialiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMatrixMode (GLenum mode)
{
  DUMMY_LOG(glMatrixMode);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultMatrixd (const GLdouble *m)
{
  DUMMY_LOG(glMultMatrixd);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultMatrixf (const GLfloat *m)
{
  DUMMY_LOG(glMultMatrixf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNewList (GLuint list, GLenum mode)
{
  DUMMY_LOG(glNewList);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormal3b (GLbyte nx, GLbyte ny, GLbyte nz)
{
  DUMMY_LOG(glNormal3b);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormal3bv (const GLbyte *v)
{
  DUMMY_LOG(glNormal3bv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormal3d (GLdouble nx, GLdouble ny, GLdouble nz)
{
  DUMMY_LOG(glNormal3d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormal3dv (const GLdouble *v)
{
  DUMMY_LOG(glNormal3dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormal3f (GLfloat nx, GLfloat ny, GLfloat nz)
{
  DUMMY_LOG(glNormal3f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormal3fv (const GLfloat *v)
{
  DUMMY_LOG(glNormal3fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormal3i (GLint nx, GLint ny, GLint nz)
{
  DUMMY_LOG(glNormal3i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormal3iv (const GLint *v)
{
  DUMMY_LOG(glNormal3iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormal3s (GLshort nx, GLshort ny, GLshort nz)
{
  DUMMY_LOG(glNormal3s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormal3sv (const GLshort *v)
{
  DUMMY_LOG(glNormal3sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer)
{
  DUMMY_LOG(glNormalPointer);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
  DUMMY_LOG(glOrtho);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPassThrough (GLfloat token)
{
  DUMMY_LOG(glPassThrough);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPixelMapfv (GLenum map, GLsizei mapsize, const GLfloat *values)
{
  DUMMY_LOG(glPixelMapfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPixelMapuiv (GLenum map, GLsizei mapsize, const GLuint *values)
{
  DUMMY_LOG(glPixelMapuiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPixelMapusv (GLenum map, GLsizei mapsize, const GLushort *values)
{
  DUMMY_LOG(glPixelMapusv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPixelStoref (GLenum pname, GLfloat param)
{
  DUMMY_LOG(glPixelStoref);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPixelStorei (GLenum pname, GLint param)
{
  DUMMY_LOG(glPixelStorei);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPixelTransferf (GLenum pname, GLfloat param)
{
  DUMMY_LOG(glPixelTransferf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPixelTransferi (GLenum pname, GLint param)
{
  DUMMY_LOG(glPixelTransferi);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPixelZoom (GLfloat xfactor, GLfloat yfactor)
{
  DUMMY_LOG(glPixelZoom);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPointSize (GLfloat size)
{
  DUMMY_LOG(glPointSize);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPolygonMode (GLenum face, GLenum mode)
{
  DUMMY_LOG(glPolygonMode);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPolygonOffset (GLfloat factor, GLfloat units)
{
  DUMMY_LOG(glPolygonOffset);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPolygonStipple (const GLubyte *mask)
{
  DUMMY_LOG(glPolygonStipple);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPopAttrib ()
{
  DUMMY_LOG(glPopAttrib);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPopClientAttrib ()
{
  DUMMY_LOG(glPopClientAttrib);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPopMatrix ()
{
  DUMMY_LOG(glPopMatrix);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPopName ()
{
  DUMMY_LOG(glPopName);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPrioritizeTextures (GLsizei n, const GLuint *textures, const GLfloat *priorities)
{
  DUMMY_LOG(glPrioritizeTextures);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPushAttrib (GLbitfield mask)
{
  DUMMY_LOG(glPushAttrib);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPushClientAttrib (GLbitfield mask)
{
  DUMMY_LOG(glPushClientAttrib);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPushMatrix ()
{
  DUMMY_LOG(glPushMatrix);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglPushName (GLuint name)
{
  DUMMY_LOG(glPushName);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos2d (GLdouble x, GLdouble y)
{
  DUMMY_LOG(glRasterPos2d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos2dv (const GLdouble *v)
{
  DUMMY_LOG(glRasterPos2dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos2f (GLfloat x, GLfloat y)
{
  DUMMY_LOG(glRasterPos2f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos2fv (const GLfloat *v)
{
  DUMMY_LOG(glRasterPos2fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos2i (GLint x, GLint y)
{
  DUMMY_LOG(glRasterPos2i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos2iv (const GLint *v)
{
  DUMMY_LOG(glRasterPos2iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos2s (GLshort x, GLshort y)
{
  DUMMY_LOG(glRasterPos2s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos2sv (const GLshort *v)
{
  DUMMY_LOG(glRasterPos2sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos3d (GLdouble x, GLdouble y, GLdouble z)
{
  DUMMY_LOG(glRasterPos3d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos3dv (const GLdouble *v)
{
  DUMMY_LOG(glRasterPos3dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos3f (GLfloat x, GLfloat y, GLfloat z)
{
  DUMMY_LOG(glRasterPos3f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos3fv (const GLfloat *v)
{
  DUMMY_LOG(glRasterPos3fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos3i (GLint x, GLint y, GLint z)
{
  DUMMY_LOG(glRasterPos3i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos3iv (const GLint *v)
{
  DUMMY_LOG(glRasterPos3iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos3s (GLshort x, GLshort y, GLshort z)
{
  DUMMY_LOG(glRasterPos3s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos3sv (const GLshort *v)
{
  DUMMY_LOG(glRasterPos3sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
  DUMMY_LOG(glRasterPos4d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos4dv (const GLdouble *v)
{
  DUMMY_LOG(glRasterPos4dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  DUMMY_LOG(glRasterPos4f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos4fv (const GLfloat *v)
{
  DUMMY_LOG(glRasterPos4fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos4i (GLint x, GLint y, GLint z, GLint w)
{
  DUMMY_LOG(glRasterPos4i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos4iv (const GLint *v)
{
  DUMMY_LOG(glRasterPos4iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos4s (GLshort x, GLshort y, GLshort z, GLshort w)
{
  DUMMY_LOG(glRasterPos4s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRasterPos4sv (const GLshort *v)
{
  DUMMY_LOG(glRasterPos4sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglReadBuffer (GLenum mode)
{
  DUMMY_LOG(glReadBuffer);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
  DUMMY_LOG(glReadPixels);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRectd (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
  DUMMY_LOG(glRectd);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRectdv (const GLdouble *v1, const GLdouble *v2)
{
  DUMMY_LOG(glRectdv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRectf (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
  DUMMY_LOG(glRectf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRectfv (const GLfloat *v1, const GLfloat *v2)
{
  DUMMY_LOG(glRectfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRecti (GLint x1, GLint y1, GLint x2, GLint y2)
{
  DUMMY_LOG(glRecti);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRectiv (const GLint *v1, const GLint *v2)
{
  DUMMY_LOG(glRectiv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRects (GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
  DUMMY_LOG(glRects);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRectsv (const GLshort *v1, const GLshort *v2)
{
  DUMMY_LOG(glRectsv);
}

///////////////////////////////////////////////////////////////////////////////
//
GLint GLAPIENTRY DummyglRenderMode (GLenum mode)
{
  DUMMY_LOG(glRenderMode);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRotated (GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
  DUMMY_LOG(glRotated);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
  DUMMY_LOG(glRotatef);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglScaled (GLdouble x, GLdouble y, GLdouble z)
{
  DUMMY_LOG(glScaled);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglScalef (GLfloat x, GLfloat y, GLfloat z)
{
  DUMMY_LOG(glScalef);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglScissor (GLint x, GLint y, GLsizei width, GLsizei height)
{
  DUMMY_LOG(glScissor);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglSelectBuffer (GLsizei size, GLuint *buffer)
{
  DUMMY_LOG(glSelectBuffer);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglShadeModel (GLenum mode)
{
  DUMMY_LOG(glShadeModel);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglStencilFunc (GLenum func, GLint ref, GLuint mask)
{
  DUMMY_LOG(glStencilFunc);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglStencilMask (GLuint mask)
{
  DUMMY_LOG(glStencilMask);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglStencilOp (GLenum fail, GLenum zfail, GLenum zpass)
{
  DUMMY_LOG(glStencilOp);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord1d (GLdouble s)
{
  DUMMY_LOG(glTexCoord1d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord1dv (const GLdouble *v)
{
  DUMMY_LOG(glTexCoord1dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord1f (GLfloat s)
{
  DUMMY_LOG(glTexCoord1f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord1fv (const GLfloat *v)
{
  DUMMY_LOG(glTexCoord1fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord1i (GLint s)
{
  DUMMY_LOG(glTexCoord1i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord1iv (const GLint *v)
{
  DUMMY_LOG(glTexCoord1iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord1s (GLshort s)
{
  DUMMY_LOG(glTexCoord1s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord1sv (const GLshort *v)
{
  DUMMY_LOG(glTexCoord1sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord2d (GLdouble s, GLdouble t)
{
  DUMMY_LOG(glTexCoord2d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord2dv (const GLdouble *v)
{
  DUMMY_LOG(glTexCoord2dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord2f (GLfloat s, GLfloat t)
{
  DUMMY_LOG(glTexCoord2f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord2fv (const GLfloat *v)
{
  DUMMY_LOG(glTexCoord2fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord2i (GLint s, GLint t)
{
  DUMMY_LOG(glTexCoord2i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord2iv (const GLint *v)
{
  DUMMY_LOG(glTexCoord2iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord2s (GLshort s, GLshort t)
{
  DUMMY_LOG(glTexCoord2s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord2sv (const GLshort *v)
{
  DUMMY_LOG(glTexCoord2sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord3d (GLdouble s, GLdouble t, GLdouble r)
{
  DUMMY_LOG(glTexCoord3d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord3dv (const GLdouble *v)
{
  DUMMY_LOG(glTexCoord3dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord3f (GLfloat s, GLfloat t, GLfloat r)
{
  DUMMY_LOG(glTexCoord3f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord3fv (const GLfloat *v)
{
  DUMMY_LOG(glTexCoord3fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord3i (GLint s, GLint t, GLint r)
{
  DUMMY_LOG(glTexCoord3i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord3iv (const GLint *v)
{
  DUMMY_LOG(glTexCoord3iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord3s (GLshort s, GLshort t, GLshort r)
{
  DUMMY_LOG(glTexCoord3s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord3sv (const GLshort *v)
{
  DUMMY_LOG(glTexCoord3sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord4d (GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
  DUMMY_LOG(glTexCoord4d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord4dv (const GLdouble *v)
{
  DUMMY_LOG(glTexCoord4dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord4f (GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
  DUMMY_LOG(glTexCoord4f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord4fv (const GLfloat *v)
{
  DUMMY_LOG(glTexCoord4fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord4i (GLint s, GLint t, GLint r, GLint q)
{
  DUMMY_LOG(glTexCoord4i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord4iv (const GLint *v)
{
  DUMMY_LOG(glTexCoord4iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord4s (GLshort s, GLshort t, GLshort r, GLshort q)
{
  DUMMY_LOG(glTexCoord4s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoord4sv (const GLshort *v)
{
  DUMMY_LOG(glTexCoord4sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  DUMMY_LOG(glTexCoordPointer);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexEnvf (GLenum target, GLenum pname, GLfloat param)
{
  DUMMY_LOG(glTexEnvf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexEnvfv (GLenum target, GLenum pname, const GLfloat *params)
{
  DUMMY_LOG(glTexEnvfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexEnvi (GLenum target, GLenum pname, GLint param)
{
  DUMMY_LOG(glTexEnvi);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexEnviv (GLenum target, GLenum pname, const GLint *params)
{
  DUMMY_LOG(glTexEnviv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexGend (GLenum coord, GLenum pname, GLdouble param)
{
  DUMMY_LOG(glTexGend);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexGendv (GLenum coord, GLenum pname, const GLdouble *params)
{
  DUMMY_LOG(glTexGendv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexGenf (GLenum coord, GLenum pname, GLfloat param)
{
  DUMMY_LOG(glTexGenf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexGenfv (GLenum coord, GLenum pname, const GLfloat *params)
{
  DUMMY_LOG(glTexGenfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexGeni (GLenum coord, GLenum pname, GLint param)
{
  DUMMY_LOG(glTexGeni);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexGeniv (GLenum coord, GLenum pname, const GLint *params)
{
  DUMMY_LOG(glTexGeniv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexImage1D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
  DUMMY_LOG(glTexImage1D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
  DUMMY_LOG(glTexImage2D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexParameterf (GLenum target, GLenum pname, GLfloat param)
{
  DUMMY_LOG(glTexParameterf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexParameterfv (GLenum target, GLenum pname, const GLfloat *params)
{
  DUMMY_LOG(glTexParameterfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexParameteri (GLenum target, GLenum pname, GLint param)
{
  DUMMY_LOG(glTexParameteri);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexParameteriv (GLenum target, GLenum pname, const GLint *params)
{
  DUMMY_LOG(glTexParameteriv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels)
{
  DUMMY_LOG(glTexSubImage1D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
  DUMMY_LOG(glTexSubImage2D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTranslated (GLdouble x, GLdouble y, GLdouble z)
{
  DUMMY_LOG(glTranslated);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTranslatef (GLfloat x, GLfloat y, GLfloat z)
{
  DUMMY_LOG(glTranslatef);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex2d (GLdouble x, GLdouble y)
{
  DUMMY_LOG(glVertex2d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex2dv (const GLdouble *v)
{
  DUMMY_LOG(glVertex2dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex2f (GLfloat x, GLfloat y)
{
  DUMMY_LOG(glVertex2f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex2fv (const GLfloat *v)
{
  DUMMY_LOG(glVertex2fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex2i (GLint x, GLint y)
{
  DUMMY_LOG(glVertex2i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex2iv (const GLint *v)
{
  DUMMY_LOG(glVertex2iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex2s (GLshort x, GLshort y)
{
  DUMMY_LOG(glVertex2s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex2sv (const GLshort *v)
{
  DUMMY_LOG(glVertex2sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex3d (GLdouble x, GLdouble y, GLdouble z)
{
  DUMMY_LOG(glVertex3d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex3dv (const GLdouble *v)
{
  DUMMY_LOG(glVertex3dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex3f (GLfloat x, GLfloat y, GLfloat z)
{
  DUMMY_LOG(glVertex3f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex3fv (const GLfloat *v)
{
  DUMMY_LOG(glVertex3fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex3i (GLint x, GLint y, GLint z)
{
  DUMMY_LOG(glVertex3i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex3iv (const GLint *v)
{
  DUMMY_LOG(glVertex3iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex3s (GLshort x, GLshort y, GLshort z)
{
  DUMMY_LOG(glVertex3s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex3sv (const GLshort *v)
{
  DUMMY_LOG(glVertex3sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
  DUMMY_LOG(glVertex4d);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex4dv (const GLdouble *v)
{
  DUMMY_LOG(glVertex4dv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  DUMMY_LOG(glVertex4f);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex4fv (const GLfloat *v)
{
  DUMMY_LOG(glVertex4fv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex4i (GLint x, GLint y, GLint z, GLint w)
{
  DUMMY_LOG(glVertex4i);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex4iv (const GLint *v)
{
  DUMMY_LOG(glVertex4iv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex4s (GLshort x, GLshort y, GLshort z, GLshort w)
{
  DUMMY_LOG(glVertex4s);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertex4sv (const GLshort *v)
{
  DUMMY_LOG(glVertex4sv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  DUMMY_LOG(glVertexPointer);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
  DUMMY_LOG(glViewport);
}

#ifdef GLI_BUILD_LINUX

//OpenGL 1.2
///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglDrawRangeElements ( GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices )
{
  DUMMY_LOG(glDrawRangeElements);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexImage3D ( GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
  DUMMY_LOG(glTexImage3D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglTexSubImage3D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels)
{
  DUMMY_LOG(glTexSubImage3D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCopyTexSubImage3D ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height )
{
  DUMMY_LOG(glCopyTexSubImage3D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColorTable ( GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table )
{
  DUMMY_LOG(glColorTable);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColorSubTable ( GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data )
{
  DUMMY_LOG(glColorSubTable);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColorTableParameteriv (GLenum target, GLenum pname, const GLint *params)
{
  DUMMY_LOG(glColorTableParameteriv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglColorTableParameterfv (GLenum target, GLenum pname, const GLfloat *params)
{
  DUMMY_LOG(glColorTableParameterfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCopyColorSubTable ( GLenum target, GLsizei start, GLint x, GLint y, GLsizei width )
{
  DUMMY_LOG(glCopyColorSubTable);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCopyColorTable ( GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width )
{
  DUMMY_LOG(glCopyColorTable);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetColorTable ( GLenum target, GLenum format, GLenum type, GLvoid *table )
{
  DUMMY_LOG(glGetColorTable);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetColorTableParameterfv ( GLenum target, GLenum pname, GLfloat *params )
{
  DUMMY_LOG(glGetColorTableParameterfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetColorTableParameteriv ( GLenum target, GLenum pname, GLint *params )
{
  DUMMY_LOG(glGetColorTableParameteriv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglBlendEquation ( GLenum mode )
{
  DUMMY_LOG(glBlendEquation);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglBlendColor ( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha )
{
  DUMMY_LOG(glBlendColor);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglHistogram ( GLenum target, GLsizei width, GLenum internalformat, GLboolean sink )
{
  DUMMY_LOG(glHistogram);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglResetHistogram ( GLenum target )
{
  DUMMY_LOG(glResetHistogram);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetHistogram ( GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values )
{
  DUMMY_LOG(glGetHistogram);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetHistogramParameterfv ( GLenum target, GLenum pname, GLfloat *params )
{
  DUMMY_LOG(glGetHistogramParameterfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetHistogramParameteriv ( GLenum target, GLenum pname, GLint *params )
{
  DUMMY_LOG(glGetHistogramParameteriv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMinmax ( GLenum target, GLenum internalformat, GLboolean sink )
{
  DUMMY_LOG(glMinmax);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglResetMinmax ( GLenum target )
{
  DUMMY_LOG(glResetMinmax);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetMinmax ( GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values )
{
  DUMMY_LOG(glGetMinmax);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetMinmaxParameterfv ( GLenum target, GLenum pname, GLfloat *params )
{
  DUMMY_LOG(glGetMinmaxParameterfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetMinmaxParameteriv ( GLenum target, GLenum pname, GLint *params )
{
  DUMMY_LOG(glGetMinmaxParameteriv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglConvolutionFilter1D ( GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image )
{
  DUMMY_LOG(glConvolutionFilter1D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglConvolutionFilter2D ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image )
{
  DUMMY_LOG(glConvolutionFilter2D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglConvolutionParameterf ( GLenum target, GLenum pname, GLfloat params )
{
  DUMMY_LOG(glConvolutionParameterf);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglConvolutionParameterfv ( GLenum target, GLenum pname, const GLfloat *params )
{
  DUMMY_LOG(glConvolutionParameterfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglConvolutionParameteri ( GLenum target, GLenum pname, GLint params )
{
  DUMMY_LOG(glConvolutionParameteri);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglConvolutionParameteriv ( GLenum target, GLenum pname, const GLint *params )
{
  DUMMY_LOG(glConvolutionParameteriv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCopyConvolutionFilter1D ( GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width )
{
  DUMMY_LOG(glCopyConvolutionFilter1D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglCopyConvolutionFilter2D ( GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height)
{
  DUMMY_LOG(glCopyConvolutionFilter2D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetConvolutionFilter ( GLenum target, GLenum format, GLenum type, GLvoid *image )
{
  DUMMY_LOG(glGetConvolutionFilter);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetConvolutionParameterfv ( GLenum target, GLenum pname, GLfloat *params )
{
  DUMMY_LOG(glGetConvolutionParameterfv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetConvolutionParameteriv ( GLenum target, GLenum pname, GLint *params )
{
  DUMMY_LOG(glGetConvolutionParameteriv);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglSeparableFilter2D ( GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column )
{
  DUMMY_LOG(glSeparableFilter2D);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglGetSeparableFilter ( GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span )
{
  DUMMY_LOG(glGetSeparableFilter);
}

//ARB multitexture
///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglActiveTextureARB (GLenum texture)
{
  DUMMY_LOG(glActiveTextureARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglClientActiveTextureARB (GLenum texture)
{
  DUMMY_LOG(glClientActiveTextureARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord1dARB (GLenum target, GLdouble s)
{
  DUMMY_LOG(glMultiTexCoord1dARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord1dvARB (GLenum target, const GLdouble *v)
{
  DUMMY_LOG(glMultiTexCoord1dvARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord1fARB (GLenum target, GLfloat s)
{
  DUMMY_LOG(glMultiTexCoord1fARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord1fvARB (GLenum target, const GLfloat *v)
{
  DUMMY_LOG(glMultiTexCoord1fvARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord1iARB (GLenum target, GLint s)
{
  DUMMY_LOG(glMultiTexCoord1iARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord1ivARB (GLenum target, const GLint *v)
{
  DUMMY_LOG(glMultiTexCoord1ivARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord1sARB (GLenum target, GLshort s)
{
  DUMMY_LOG(glMultiTexCoord1sARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord1svARB (GLenum target, const GLshort *v)
{
  DUMMY_LOG(glMultiTexCoord1svARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord2dARB (GLenum target, GLdouble s, GLdouble t)
{
  DUMMY_LOG(glMultiTexCoord2dARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord2dvARB (GLenum target, const GLdouble *v)
{
  DUMMY_LOG(glMultiTexCoord2dvARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord2fARB (GLenum target, GLfloat s, GLfloat t)
{
  DUMMY_LOG(glMultiTexCoord2fARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord2fvARB (GLenum target, const GLfloat *v)
{
  DUMMY_LOG(glMultiTexCoord2fvARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord2iARB (GLenum target, GLint s, GLint t)
{
  DUMMY_LOG(glMultiTexCoord2iARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord2ivARB (GLenum target, const GLint *v)
{
  DUMMY_LOG(glMultiTexCoord2ivARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord2sARB (GLenum target, GLshort s, GLshort t)
{
  DUMMY_LOG(glMultiTexCoord2sARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord2svARB (GLenum target, const GLshort *v)
{
  DUMMY_LOG(glMultiTexCoord2svARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord3dARB (GLenum target, GLdouble s, GLdouble t, GLdouble r)
{
  DUMMY_LOG(glMultiTexCoord3dARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord3dvARB (GLenum target, const GLdouble *v)
{
  DUMMY_LOG(glMultiTexCoord3dvARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord3fARB (GLenum target, GLfloat s, GLfloat t, GLfloat r)
{
  DUMMY_LOG(glMultiTexCoord3fARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord3fvARB (GLenum target, const GLfloat *v)
{
  DUMMY_LOG(glMultiTexCoord3fvARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord3iARB (GLenum target, GLint s, GLint t, GLint r)
{
  DUMMY_LOG(glMultiTexCoord3iARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord3ivARB (GLenum target, const GLint *v)
{
  DUMMY_LOG(glMultiTexCoord3ivARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord3sARB (GLenum target, GLshort s, GLshort t, GLshort r)
{
  DUMMY_LOG(glMultiTexCoord3sARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord3svARB (GLenum target, const GLshort *v)
{
  DUMMY_LOG(glMultiTexCoord3svARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord4dARB (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
  DUMMY_LOG(glMultiTexCoord4dARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord4dvARB (GLenum target, const GLdouble *v)
{
  DUMMY_LOG(glMultiTexCoord4dvARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord4fARB (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
  DUMMY_LOG(glMultiTexCoord4fARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord4fvARB (GLenum target, const GLfloat *v)
{
  DUMMY_LOG(glMultiTexCoord4fvARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord4iARB (GLenum target, GLint s, GLint t, GLint r, GLint q)
{
  DUMMY_LOG(glMultiTexCoord4iARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord4ivARB (GLenum target, const GLint *v)
{
  DUMMY_LOG(glMultiTexCoord4ivARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord4sARB (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
{
  DUMMY_LOG(glMultiTexCoord4sARB);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY DummyglMultiTexCoord4svARB (GLenum target, const GLshort *v)
{
  DUMMY_LOG(glMultiTexCoord4svARB);
}

#endif //GLI_BUILD_LINUX

//Initialize with stubs
GLCoreDriver GLV=
{
  //Core methods
  DummyglAccum,
  DummyglAlphaFunc,
  DummyglAreTexturesResident,
  DummyglArrayElement,
  DummyglBegin,
  DummyglBindTexture,
  DummyglBitmap,
  DummyglBlendFunc,
  DummyglCallList,
  DummyglCallLists,
  DummyglClear,
  DummyglClearAccum,
  DummyglClearColor,
  DummyglClearDepth,
  DummyglClearIndex,
  DummyglClearStencil,
  DummyglClipPlane,
  DummyglColor3b,
  DummyglColor3bv,
  DummyglColor3d,
  DummyglColor3dv,
  DummyglColor3f,
  DummyglColor3fv,
  DummyglColor3i,
  DummyglColor3iv,
  DummyglColor3s,
  DummyglColor3sv,
  DummyglColor3ub,
  DummyglColor3ubv,
  DummyglColor3ui,
  DummyglColor3uiv,
  DummyglColor3us,
  DummyglColor3usv,
  DummyglColor4b,
  DummyglColor4bv,
  DummyglColor4d,
  DummyglColor4dv,
  DummyglColor4f,
  DummyglColor4fv,
  DummyglColor4i,
  DummyglColor4iv,
  DummyglColor4s,
  DummyglColor4sv,
  DummyglColor4ub,
  DummyglColor4ubv,
  DummyglColor4ui,
  DummyglColor4uiv,
  DummyglColor4us,
  DummyglColor4usv,
  DummyglColorMask,
  DummyglColorMaterial,
  DummyglColorPointer,
  DummyglCopyPixels,
  DummyglCopyTexImage1D,
  DummyglCopyTexImage2D,
  DummyglCopyTexSubImage1D,
  DummyglCopyTexSubImage2D,
  DummyglCullFace,
  DummyglDeleteLists,
  DummyglDeleteTextures,
  DummyglDepthFunc,
  DummyglDepthMask,
  DummyglDepthRange,
  DummyglDisable,
  DummyglDisableClientState,
  DummyglDrawArrays,
  DummyglDrawBuffer,
  DummyglDrawElements,
  DummyglDrawPixels,
  DummyglEdgeFlag,
  DummyglEdgeFlagPointer,
  DummyglEdgeFlagv,
  DummyglEnable,
  DummyglEnableClientState,
  DummyglEnd,
  DummyglEndList,
  DummyglEvalCoord1d,
  DummyglEvalCoord1dv,
  DummyglEvalCoord1f,
  DummyglEvalCoord1fv,
  DummyglEvalCoord2d,
  DummyglEvalCoord2dv,
  DummyglEvalCoord2f,
  DummyglEvalCoord2fv,
  DummyglEvalMesh1,
  DummyglEvalMesh2,
  DummyglEvalPoint1,
  DummyglEvalPoint2,
  DummyglFeedbackBuffer,
  DummyglFinish,
  DummyglFlush,
  DummyglFogf,
  DummyglFogfv,
  DummyglFogi,
  DummyglFogiv,
  DummyglFrontFace,
  DummyglFrustum,
  DummyglGenLists,
  DummyglGenTextures,
  DummyglGetBooleanv,
  DummyglGetClipPlane,
  DummyglGetDoublev,
  DummyglGetError,
  DummyglGetFloatv,
  DummyglGetIntegerv,
  DummyglGetLightfv,
  DummyglGetLightiv,
  DummyglGetMapdv,
  DummyglGetMapfv,
  DummyglGetMapiv,
  DummyglGetMaterialfv,
  DummyglGetMaterialiv,
  DummyglGetPixelMapfv,
  DummyglGetPixelMapuiv,
  DummyglGetPixelMapusv,
  DummyglGetPointerv,
  DummyglGetPolygonStipple,
  DummyglGetString,
  DummyglGetTexEnvfv,
  DummyglGetTexEnviv,
  DummyglGetTexGendv,
  DummyglGetTexGenfv,
  DummyglGetTexGeniv,
  DummyglGetTexImage,
  DummyglGetTexLevelParameterfv,
  DummyglGetTexLevelParameteriv,
  DummyglGetTexParameterfv,
  DummyglGetTexParameteriv,
  DummyglHint,
  DummyglIndexMask,
  DummyglIndexPointer,
  DummyglIndexd,
  DummyglIndexdv,
  DummyglIndexf,
  DummyglIndexfv,
  DummyglIndexi,
  DummyglIndexiv,
  DummyglIndexs,
  DummyglIndexsv,
  DummyglIndexub,
  DummyglIndexubv,
  DummyglInitNames,
  DummyglInterleavedArrays,
  DummyglIsEnabled,
  DummyglIsList,
  DummyglIsTexture,
  DummyglLightModelf,
  DummyglLightModelfv,
  DummyglLightModeli,
  DummyglLightModeliv,
  DummyglLightf,
  DummyglLightfv,
  DummyglLighti,
  DummyglLightiv,
  DummyglLineStipple,
  DummyglLineWidth,
  DummyglListBase,
  DummyglLoadIdentity,
  DummyglLoadMatrixd,
  DummyglLoadMatrixf,
  DummyglLoadName,
  DummyglLogicOp,
  DummyglMap1d,
  DummyglMap1f,
  DummyglMap2d,
  DummyglMap2f,
  DummyglMapGrid1d,
  DummyglMapGrid1f,
  DummyglMapGrid2d,
  DummyglMapGrid2f,
  DummyglMaterialf,
  DummyglMaterialfv,
  DummyglMateriali,
  DummyglMaterialiv,
  DummyglMatrixMode,
  DummyglMultMatrixd,
  DummyglMultMatrixf,
  DummyglNewList,
  DummyglNormal3b,
  DummyglNormal3bv,
  DummyglNormal3d,
  DummyglNormal3dv,
  DummyglNormal3f,
  DummyglNormal3fv,
  DummyglNormal3i,
  DummyglNormal3iv,
  DummyglNormal3s,
  DummyglNormal3sv,
  DummyglNormalPointer,
  DummyglOrtho,
  DummyglPassThrough,
  DummyglPixelMapfv,
  DummyglPixelMapuiv,
  DummyglPixelMapusv,
  DummyglPixelStoref,
  DummyglPixelStorei,
  DummyglPixelTransferf,
  DummyglPixelTransferi,
  DummyglPixelZoom,
  DummyglPointSize,
  DummyglPolygonMode,
  DummyglPolygonOffset,
  DummyglPolygonStipple,
  DummyglPopAttrib,
  DummyglPopClientAttrib,
  DummyglPopMatrix,
  DummyglPopName,
  DummyglPrioritizeTextures,
  DummyglPushAttrib,
  DummyglPushClientAttrib,
  DummyglPushMatrix,
  DummyglPushName,
  DummyglRasterPos2d,
  DummyglRasterPos2dv,
  DummyglRasterPos2f,
  DummyglRasterPos2fv,
  DummyglRasterPos2i,
  DummyglRasterPos2iv,
  DummyglRasterPos2s,
  DummyglRasterPos2sv,
  DummyglRasterPos3d,
  DummyglRasterPos3dv,
  DummyglRasterPos3f,
  DummyglRasterPos3fv,
  DummyglRasterPos3i,
  DummyglRasterPos3iv,
  DummyglRasterPos3s,
  DummyglRasterPos3sv,
  DummyglRasterPos4d,
  DummyglRasterPos4dv,
  DummyglRasterPos4f,
  DummyglRasterPos4fv,
  DummyglRasterPos4i,
  DummyglRasterPos4iv,
  DummyglRasterPos4s,
  DummyglRasterPos4sv,
  DummyglReadBuffer,
  DummyglReadPixels,
  DummyglRectd,
  DummyglRectdv,
  DummyglRectf,
  DummyglRectfv,
  DummyglRecti,
  DummyglRectiv,
  DummyglRects,
  DummyglRectsv,
  DummyglRenderMode,
  DummyglRotated,
  DummyglRotatef,
  DummyglScaled,
  DummyglScalef,
  DummyglScissor,
  DummyglSelectBuffer,
  DummyglShadeModel,
  DummyglStencilFunc,
  DummyglStencilMask,
  DummyglStencilOp,
  DummyglTexCoord1d,
  DummyglTexCoord1dv,
  DummyglTexCoord1f,
  DummyglTexCoord1fv,
  DummyglTexCoord1i,
  DummyglTexCoord1iv,
  DummyglTexCoord1s,
  DummyglTexCoord1sv,
  DummyglTexCoord2d,
  DummyglTexCoord2dv,
  DummyglTexCoord2f,
  DummyglTexCoord2fv,
  DummyglTexCoord2i,
  DummyglTexCoord2iv,
  DummyglTexCoord2s,
  DummyglTexCoord2sv,
  DummyglTexCoord3d,
  DummyglTexCoord3dv,
  DummyglTexCoord3f,
  DummyglTexCoord3fv,
  DummyglTexCoord3i,
  DummyglTexCoord3iv,
  DummyglTexCoord3s,
  DummyglTexCoord3sv,
  DummyglTexCoord4d,
  DummyglTexCoord4dv,
  DummyglTexCoord4f,
  DummyglTexCoord4fv,
  DummyglTexCoord4i,
  DummyglTexCoord4iv,
  DummyglTexCoord4s,
  DummyglTexCoord4sv,
  DummyglTexCoordPointer,
  DummyglTexEnvf,
  DummyglTexEnvfv,
  DummyglTexEnvi,
  DummyglTexEnviv,
  DummyglTexGend,
  DummyglTexGendv,
  DummyglTexGenf,
  DummyglTexGenfv,
  DummyglTexGeni,
  DummyglTexGeniv,
  DummyglTexImage1D,
  DummyglTexImage2D,
  DummyglTexParameterf,
  DummyglTexParameterfv,
  DummyglTexParameteri,
  DummyglTexParameteriv,
  DummyglTexSubImage1D,
  DummyglTexSubImage2D,
  DummyglTranslated,
  DummyglTranslatef,
  DummyglVertex2d,
  DummyglVertex2dv,
  DummyglVertex2f,
  DummyglVertex2fv,
  DummyglVertex2i,
  DummyglVertex2iv,
  DummyglVertex2s,
  DummyglVertex2sv,
  DummyglVertex3d,
  DummyglVertex3dv,
  DummyglVertex3f,
  DummyglVertex3fv,
  DummyglVertex3i,
  DummyglVertex3iv,
  DummyglVertex3s,
  DummyglVertex3sv,
  DummyglVertex4d,
  DummyglVertex4dv,
  DummyglVertex4f,
  DummyglVertex4fv,
  DummyglVertex4i,
  DummyglVertex4iv,
  DummyglVertex4s,
  DummyglVertex4sv,
  DummyglVertexPointer,
  DummyglViewport,

#ifdef GLI_BUILD_LINUX

  //OpenGL 1.2
  DummyglDrawRangeElements,
  DummyglTexImage3D,
  DummyglTexSubImage3D,
  DummyglCopyTexSubImage3D,
  DummyglColorTable,
  DummyglColorSubTable,
  DummyglColorTableParameteriv,
  DummyglColorTableParameterfv,
  DummyglCopyColorSubTable,
  DummyglCopyColorTable,
  DummyglGetColorTable,
  DummyglGetColorTableParameterfv,
  DummyglGetColorTableParameteriv,
  DummyglBlendEquation,
  DummyglBlendColor,
  DummyglHistogram,
  DummyglResetHistogram,
  DummyglGetHistogram,
  DummyglGetHistogramParameterfv,
  DummyglGetHistogramParameteriv,
  DummyglMinmax,
  DummyglResetMinmax,
  DummyglGetMinmax,
  DummyglGetMinmaxParameterfv,
  DummyglGetMinmaxParameteriv,
  DummyglConvolutionFilter1D,
  DummyglConvolutionFilter2D,
  DummyglConvolutionParameterf,
  DummyglConvolutionParameterfv,
  DummyglConvolutionParameteri,
  DummyglConvolutionParameteriv,
  DummyglCopyConvolutionFilter1D,
  DummyglCopyConvolutionFilter2D,
  DummyglGetConvolutionFilter,
  DummyglGetConvolutionParameterfv,
  DummyglGetConvolutionParameteriv,
  DummyglSeparableFilter2D,
  DummyglGetSeparableFilter,

  //ARB multitexture
  DummyglActiveTextureARB,
  DummyglClientActiveTextureARB,
  DummyglMultiTexCoord1dARB,
  DummyglMultiTexCoord1dvARB,
  DummyglMultiTexCoord1fARB,
  DummyglMultiTexCoord1fvARB,
  DummyglMultiTexCoord1iARB,
  DummyglMultiTexCoord1ivARB,
  DummyglMultiTexCoord1sARB,
  DummyglMultiTexCoord1svARB,
  DummyglMultiTexCoord2dARB,
  DummyglMultiTexCoord2dvARB,
  DummyglMultiTexCoord2fARB,
  DummyglMultiTexCoord2fvARB,
  DummyglMultiTexCoord2iARB,
  DummyglMultiTexCoord2ivARB,
  DummyglMultiTexCoord2sARB,
  DummyglMultiTexCoord2svARB,
  DummyglMultiTexCoord3dARB,
  DummyglMultiTexCoord3dvARB,
  DummyglMultiTexCoord3fARB,
  DummyglMultiTexCoord3fvARB,
  DummyglMultiTexCoord3iARB,
  DummyglMultiTexCoord3ivARB,
  DummyglMultiTexCoord3sARB,
  DummyglMultiTexCoord3svARB,
  DummyglMultiTexCoord4dARB,
  DummyglMultiTexCoord4dvARB,
  DummyglMultiTexCoord4fARB,
  DummyglMultiTexCoord4fvARB,
  DummyglMultiTexCoord4iARB,
  DummyglMultiTexCoord4ivARB,
  DummyglMultiTexCoord4sARB,
  DummyglMultiTexCoord4svARB

#endif //GLI_BUILD_LINUX

};

//The version of the GL windows functions that the built-in functions call  
GLCoreDriver GLV_Builtin = GLV; 
  
  
  
  
  



