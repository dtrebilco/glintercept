/* $Id: gl.h,v 1.44.4.3 2001/03/21 15:59:49 brianp Exp $ */

/*
 * Mesa 3-D graphics library
 * Version:  3.4
 *
 * Copyright (C) 1999-2000  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef __gl_h_
#define __gl_h_

//GLIntercept has split defines and function prototypes into two seperate 
// files. This helps ensure the OpenGL interface functions are not accidently 
// called.
#include "GLDefines.h"

#ifdef __cplusplus
extern "C" {
#endif



#define GL_VERSION_1_1   1
#define GL_VERSION_1_2   1


#if defined(macintosh) && PRAGMA_IMPORT_SUPPORTED
#pragma import on
#endif


#if defined(__BEOS__) || defined(__QUICKDRAW__)
#pragma export on
#endif


/*
 * Miscellaneous
 */

GLAPI void GLAPIENTRY glClearIndex( GLfloat c );

GLAPI void GLAPIENTRY glClearColor( GLfloat red,
                          GLfloat green,
                          GLfloat blue,
                          GLfloat alpha );

GLAPI void GLAPIENTRY glClear( GLbitfield mask );

GLAPI void GLAPIENTRY glIndexMask( GLuint mask );

GLAPI void GLAPIENTRY glColorMask( GLboolean red, GLboolean green,
                         GLboolean blue, GLboolean alpha );

GLAPI void GLAPIENTRY glAlphaFunc( GLenum func, GLfloat ref );

GLAPI void GLAPIENTRY glBlendFunc( GLenum sfactor, GLenum dfactor );

GLAPI void GLAPIENTRY glLogicOp( GLenum opcode );

GLAPI void GLAPIENTRY glCullFace( GLenum mode );

GLAPI void GLAPIENTRY glFrontFace( GLenum mode );

GLAPI void GLAPIENTRY glPointSize( GLfloat size );

GLAPI void GLAPIENTRY glLineWidth( GLfloat width );

GLAPI void GLAPIENTRY glLineStipple( GLint factor, GLushort pattern );

GLAPI void GLAPIENTRY glPolygonMode( GLenum face, GLenum mode );

GLAPI void GLAPIENTRY glPolygonOffset( GLfloat factor, GLfloat units );

GLAPI void GLAPIENTRY glPolygonStipple( const GLubyte *mask );

GLAPI void GLAPIENTRY glGetPolygonStipple( GLubyte *mask );

GLAPI void GLAPIENTRY glEdgeFlag( GLboolean flag );

GLAPI void GLAPIENTRY glEdgeFlagv( const GLboolean *flag );

GLAPI void GLAPIENTRY glScissor( GLint x, GLint y,
                                   GLsizei width, GLsizei height);

GLAPI void GLAPIENTRY glClipPlane( GLenum plane, const GLdouble *equation );

GLAPI void GLAPIENTRY glGetClipPlane( GLenum plane, GLdouble *equation );

GLAPI void GLAPIENTRY glDrawBuffer( GLenum mode );

GLAPI void GLAPIENTRY glReadBuffer( GLenum mode );

GLAPI void GLAPIENTRY glEnable( GLenum cap );

GLAPI void GLAPIENTRY glDisable( GLenum cap );

GLAPI GLboolean GLAPIENTRY glIsEnabled( GLenum cap );


GLAPI void GLAPIENTRY glEnableClientState( GLenum cap );  /* 1.1 */

GLAPI void GLAPIENTRY glDisableClientState( GLenum cap );  /* 1.1 */


GLAPI void GLAPIENTRY glGetBooleanv( GLenum pname, GLboolean *params );

GLAPI void GLAPIENTRY glGetDoublev( GLenum pname, GLdouble *params );

GLAPI void GLAPIENTRY glGetFloatv( GLenum pname, GLfloat *params );

GLAPI void GLAPIENTRY glGetIntegerv( GLenum pname, GLint *params );


GLAPI void GLAPIENTRY glPushAttrib( GLbitfield mask );

GLAPI void GLAPIENTRY glPopAttrib( void );


GLAPI void GLAPIENTRY glPushClientAttrib( GLbitfield mask );  /* 1.1 */

GLAPI void GLAPIENTRY glPopClientAttrib( void );  /* 1.1 */


GLAPI GLint GLAPIENTRY glRenderMode( GLenum mode );

GLAPI GLenum GLAPIENTRY glGetError( void );

GLAPI const GLubyte* GLAPIENTRY glGetString( GLenum name );

GLAPI void GLAPIENTRY glFinish( void );

GLAPI void GLAPIENTRY glFlush( void );

GLAPI void GLAPIENTRY glHint( GLenum target, GLenum mode );



/*
 * Depth Buffer
 */

GLAPI void GLAPIENTRY glClearDepth( GLdouble depth );

GLAPI void GLAPIENTRY glDepthFunc( GLenum func );

GLAPI void GLAPIENTRY glDepthMask( GLboolean flag );

GLAPI void GLAPIENTRY glDepthRange( GLdouble near_val, GLdouble far_val );


/*
 * Accumulation Buffer
 */

GLAPI void GLAPIENTRY glClearAccum( GLfloat red, GLfloat green,
                                      GLfloat blue, GLfloat alpha );

GLAPI void GLAPIENTRY glAccum( GLenum op, GLfloat value );



/*
 * Transformation
 */

GLAPI void GLAPIENTRY glMatrixMode( GLenum mode );

GLAPI void GLAPIENTRY glOrtho( GLdouble left, GLdouble right,
                                 GLdouble bottom, GLdouble top,
                                 GLdouble near_val, GLdouble far_val );

GLAPI void GLAPIENTRY glFrustum( GLdouble left, GLdouble right,
                                   GLdouble bottom, GLdouble top,
                                   GLdouble near_val, GLdouble far_val );

GLAPI void GLAPIENTRY glViewport( GLint x, GLint y,
                                    GLsizei width, GLsizei height );

GLAPI void GLAPIENTRY glPushMatrix( void );

GLAPI void GLAPIENTRY glPopMatrix( void );

GLAPI void GLAPIENTRY glLoadIdentity( void );

GLAPI void GLAPIENTRY glLoadMatrixd( const GLdouble *m );
GLAPI void GLAPIENTRY glLoadMatrixf( const GLfloat *m );

GLAPI void GLAPIENTRY glMultMatrixd( const GLdouble *m );
GLAPI void GLAPIENTRY glMultMatrixf( const GLfloat *m );

GLAPI void GLAPIENTRY glRotated( GLdouble angle,
                                   GLdouble x, GLdouble y, GLdouble z );
GLAPI void GLAPIENTRY glRotatef( GLfloat angle,
                                   GLfloat x, GLfloat y, GLfloat z );

GLAPI void GLAPIENTRY glScaled( GLdouble x, GLdouble y, GLdouble z );
GLAPI void GLAPIENTRY glScalef( GLfloat x, GLfloat y, GLfloat z );

GLAPI void GLAPIENTRY glTranslated( GLdouble x, GLdouble y, GLdouble z );
GLAPI void GLAPIENTRY glTranslatef( GLfloat x, GLfloat y, GLfloat z );



/*
 * Display Lists
 */

GLAPI GLboolean GLAPIENTRY glIsList( GLuint list );

GLAPI void GLAPIENTRY glDeleteLists( GLuint list, GLsizei range );

GLAPI GLuint GLAPIENTRY glGenLists( GLsizei range );

GLAPI void GLAPIENTRY glNewList( GLuint list, GLenum mode );

GLAPI void GLAPIENTRY glEndList( void );

GLAPI void GLAPIENTRY glCallList( GLuint list );

GLAPI void GLAPIENTRY glCallLists( GLsizei n, GLenum type,
                                     const GLvoid *lists );

GLAPI void GLAPIENTRY glListBase( GLuint base );



/*
 * Drawing Functions
 */

GLAPI void GLAPIENTRY glBegin( GLenum mode );

GLAPI void GLAPIENTRY glEnd( void );


GLAPI void GLAPIENTRY glVertex2d( GLdouble x, GLdouble y );
GLAPI void GLAPIENTRY glVertex2f( GLfloat x, GLfloat y );
GLAPI void GLAPIENTRY glVertex2i( GLint x, GLint y );
GLAPI void GLAPIENTRY glVertex2s( GLshort x, GLshort y );

GLAPI void GLAPIENTRY glVertex3d( GLdouble x, GLdouble y, GLdouble z );
GLAPI void GLAPIENTRY glVertex3f( GLfloat x, GLfloat y, GLfloat z );
GLAPI void GLAPIENTRY glVertex3i( GLint x, GLint y, GLint z );
GLAPI void GLAPIENTRY glVertex3s( GLshort x, GLshort y, GLshort z );

GLAPI void GLAPIENTRY glVertex4d( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
GLAPI void GLAPIENTRY glVertex4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
GLAPI void GLAPIENTRY glVertex4i( GLint x, GLint y, GLint z, GLint w );
GLAPI void GLAPIENTRY glVertex4s( GLshort x, GLshort y, GLshort z, GLshort w );

GLAPI void GLAPIENTRY glVertex2dv( const GLdouble *v );
GLAPI void GLAPIENTRY glVertex2fv( const GLfloat *v );
GLAPI void GLAPIENTRY glVertex2iv( const GLint *v );
GLAPI void GLAPIENTRY glVertex2sv( const GLshort *v );

GLAPI void GLAPIENTRY glVertex3dv( const GLdouble *v );
GLAPI void GLAPIENTRY glVertex3fv( const GLfloat *v );
GLAPI void GLAPIENTRY glVertex3iv( const GLint *v );
GLAPI void GLAPIENTRY glVertex3sv( const GLshort *v );

GLAPI void GLAPIENTRY glVertex4dv( const GLdouble *v );
GLAPI void GLAPIENTRY glVertex4fv( const GLfloat *v );
GLAPI void GLAPIENTRY glVertex4iv( const GLint *v );
GLAPI void GLAPIENTRY glVertex4sv( const GLshort *v );


GLAPI void GLAPIENTRY glNormal3b( GLbyte nx, GLbyte ny, GLbyte nz );
GLAPI void GLAPIENTRY glNormal3d( GLdouble nx, GLdouble ny, GLdouble nz );
GLAPI void GLAPIENTRY glNormal3f( GLfloat nx, GLfloat ny, GLfloat nz );
GLAPI void GLAPIENTRY glNormal3i( GLint nx, GLint ny, GLint nz );
GLAPI void GLAPIENTRY glNormal3s( GLshort nx, GLshort ny, GLshort nz );

GLAPI void GLAPIENTRY glNormal3bv( const GLbyte *v );
GLAPI void GLAPIENTRY glNormal3dv( const GLdouble *v );
GLAPI void GLAPIENTRY glNormal3fv( const GLfloat *v );
GLAPI void GLAPIENTRY glNormal3iv( const GLint *v );
GLAPI void GLAPIENTRY glNormal3sv( const GLshort *v );


GLAPI void GLAPIENTRY glIndexd( GLdouble c );
GLAPI void GLAPIENTRY glIndexf( GLfloat c );
GLAPI void GLAPIENTRY glIndexi( GLint c );
GLAPI void GLAPIENTRY glIndexs( GLshort c );
GLAPI void GLAPIENTRY glIndexub( GLubyte c );  /* 1.1 */

GLAPI void GLAPIENTRY glIndexdv( const GLdouble *c );
GLAPI void GLAPIENTRY glIndexfv( const GLfloat *c );
GLAPI void GLAPIENTRY glIndexiv( const GLint *c );
GLAPI void GLAPIENTRY glIndexsv( const GLshort *c );
GLAPI void GLAPIENTRY glIndexubv( const GLubyte *c );  /* 1.1 */

GLAPI void GLAPIENTRY glColor3b( GLbyte red, GLbyte green, GLbyte blue );
GLAPI void GLAPIENTRY glColor3d( GLdouble red, GLdouble green, GLdouble blue );
GLAPI void GLAPIENTRY glColor3f( GLfloat red, GLfloat green, GLfloat blue );
GLAPI void GLAPIENTRY glColor3i( GLint red, GLint green, GLint blue );
GLAPI void GLAPIENTRY glColor3s( GLshort red, GLshort green, GLshort blue );
GLAPI void GLAPIENTRY glColor3ub( GLubyte red, GLubyte green, GLubyte blue );
GLAPI void GLAPIENTRY glColor3ui( GLuint red, GLuint green, GLuint blue );
GLAPI void GLAPIENTRY glColor3us( GLushort red, GLushort green, GLushort blue );

GLAPI void GLAPIENTRY glColor4b( GLbyte red, GLbyte green,
                                   GLbyte blue, GLbyte alpha );
GLAPI void GLAPIENTRY glColor4d( GLdouble red, GLdouble green,
                                   GLdouble blue, GLdouble alpha );
GLAPI void GLAPIENTRY glColor4f( GLfloat red, GLfloat green,
                                   GLfloat blue, GLfloat alpha );
GLAPI void GLAPIENTRY glColor4i( GLint red, GLint green,
                                   GLint blue, GLint alpha );
GLAPI void GLAPIENTRY glColor4s( GLshort red, GLshort green,
                                   GLshort blue, GLshort alpha );
GLAPI void GLAPIENTRY glColor4ub( GLubyte red, GLubyte green,
                                    GLubyte blue, GLubyte alpha );
GLAPI void GLAPIENTRY glColor4ui( GLuint red, GLuint green,
                                    GLuint blue, GLuint alpha );
GLAPI void GLAPIENTRY glColor4us( GLushort red, GLushort green,
                                    GLushort blue, GLushort alpha );


GLAPI void GLAPIENTRY glColor3bv( const GLbyte *v );
GLAPI void GLAPIENTRY glColor3dv( const GLdouble *v );
GLAPI void GLAPIENTRY glColor3fv( const GLfloat *v );
GLAPI void GLAPIENTRY glColor3iv( const GLint *v );
GLAPI void GLAPIENTRY glColor3sv( const GLshort *v );
GLAPI void GLAPIENTRY glColor3ubv( const GLubyte *v );
GLAPI void GLAPIENTRY glColor3uiv( const GLuint *v );
GLAPI void GLAPIENTRY glColor3usv( const GLushort *v );

GLAPI void GLAPIENTRY glColor4bv( const GLbyte *v );
GLAPI void GLAPIENTRY glColor4dv( const GLdouble *v );
GLAPI void GLAPIENTRY glColor4fv( const GLfloat *v );
GLAPI void GLAPIENTRY glColor4iv( const GLint *v );
GLAPI void GLAPIENTRY glColor4sv( const GLshort *v );
GLAPI void GLAPIENTRY glColor4ubv( const GLubyte *v );
GLAPI void GLAPIENTRY glColor4uiv( const GLuint *v );
GLAPI void GLAPIENTRY glColor4usv( const GLushort *v );


GLAPI void GLAPIENTRY glTexCoord1d( GLdouble s );
GLAPI void GLAPIENTRY glTexCoord1f( GLfloat s );
GLAPI void GLAPIENTRY glTexCoord1i( GLint s );
GLAPI void GLAPIENTRY glTexCoord1s( GLshort s );

GLAPI void GLAPIENTRY glTexCoord2d( GLdouble s, GLdouble t );
GLAPI void GLAPIENTRY glTexCoord2f( GLfloat s, GLfloat t );
GLAPI void GLAPIENTRY glTexCoord2i( GLint s, GLint t );
GLAPI void GLAPIENTRY glTexCoord2s( GLshort s, GLshort t );

GLAPI void GLAPIENTRY glTexCoord3d( GLdouble s, GLdouble t, GLdouble r );
GLAPI void GLAPIENTRY glTexCoord3f( GLfloat s, GLfloat t, GLfloat r );
GLAPI void GLAPIENTRY glTexCoord3i( GLint s, GLint t, GLint r );
GLAPI void GLAPIENTRY glTexCoord3s( GLshort s, GLshort t, GLshort r );

GLAPI void GLAPIENTRY glTexCoord4d( GLdouble s, GLdouble t, GLdouble r, GLdouble q );
GLAPI void GLAPIENTRY glTexCoord4f( GLfloat s, GLfloat t, GLfloat r, GLfloat q );
GLAPI void GLAPIENTRY glTexCoord4i( GLint s, GLint t, GLint r, GLint q );
GLAPI void GLAPIENTRY glTexCoord4s( GLshort s, GLshort t, GLshort r, GLshort q );

GLAPI void GLAPIENTRY glTexCoord1dv( const GLdouble *v );
GLAPI void GLAPIENTRY glTexCoord1fv( const GLfloat *v );
GLAPI void GLAPIENTRY glTexCoord1iv( const GLint *v );
GLAPI void GLAPIENTRY glTexCoord1sv( const GLshort *v );

GLAPI void GLAPIENTRY glTexCoord2dv( const GLdouble *v );
GLAPI void GLAPIENTRY glTexCoord2fv( const GLfloat *v );
GLAPI void GLAPIENTRY glTexCoord2iv( const GLint *v );
GLAPI void GLAPIENTRY glTexCoord2sv( const GLshort *v );

GLAPI void GLAPIENTRY glTexCoord3dv( const GLdouble *v );
GLAPI void GLAPIENTRY glTexCoord3fv( const GLfloat *v );
GLAPI void GLAPIENTRY glTexCoord3iv( const GLint *v );
GLAPI void GLAPIENTRY glTexCoord3sv( const GLshort *v );

GLAPI void GLAPIENTRY glTexCoord4dv( const GLdouble *v );
GLAPI void GLAPIENTRY glTexCoord4fv( const GLfloat *v );
GLAPI void GLAPIENTRY glTexCoord4iv( const GLint *v );
GLAPI void GLAPIENTRY glTexCoord4sv( const GLshort *v );


GLAPI void GLAPIENTRY glRasterPos2d( GLdouble x, GLdouble y );
GLAPI void GLAPIENTRY glRasterPos2f( GLfloat x, GLfloat y );
GLAPI void GLAPIENTRY glRasterPos2i( GLint x, GLint y );
GLAPI void GLAPIENTRY glRasterPos2s( GLshort x, GLshort y );

GLAPI void GLAPIENTRY glRasterPos3d( GLdouble x, GLdouble y, GLdouble z );
GLAPI void GLAPIENTRY glRasterPos3f( GLfloat x, GLfloat y, GLfloat z );
GLAPI void GLAPIENTRY glRasterPos3i( GLint x, GLint y, GLint z );
GLAPI void GLAPIENTRY glRasterPos3s( GLshort x, GLshort y, GLshort z );

GLAPI void GLAPIENTRY glRasterPos4d( GLdouble x, GLdouble y, GLdouble z, GLdouble w );
GLAPI void GLAPIENTRY glRasterPos4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
GLAPI void GLAPIENTRY glRasterPos4i( GLint x, GLint y, GLint z, GLint w );
GLAPI void GLAPIENTRY glRasterPos4s( GLshort x, GLshort y, GLshort z, GLshort w );

GLAPI void GLAPIENTRY glRasterPos2dv( const GLdouble *v );
GLAPI void GLAPIENTRY glRasterPos2fv( const GLfloat *v );
GLAPI void GLAPIENTRY glRasterPos2iv( const GLint *v );
GLAPI void GLAPIENTRY glRasterPos2sv( const GLshort *v );

GLAPI void GLAPIENTRY glRasterPos3dv( const GLdouble *v );
GLAPI void GLAPIENTRY glRasterPos3fv( const GLfloat *v );
GLAPI void GLAPIENTRY glRasterPos3iv( const GLint *v );
GLAPI void GLAPIENTRY glRasterPos3sv( const GLshort *v );

GLAPI void GLAPIENTRY glRasterPos4dv( const GLdouble *v );
GLAPI void GLAPIENTRY glRasterPos4fv( const GLfloat *v );
GLAPI void GLAPIENTRY glRasterPos4iv( const GLint *v );
GLAPI void GLAPIENTRY glRasterPos4sv( const GLshort *v );


GLAPI void GLAPIENTRY glRectd( GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2 );
GLAPI void GLAPIENTRY glRectf( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2 );
GLAPI void GLAPIENTRY glRecti( GLint x1, GLint y1, GLint x2, GLint y2 );
GLAPI void GLAPIENTRY glRects( GLshort x1, GLshort y1, GLshort x2, GLshort y2 );


GLAPI void GLAPIENTRY glRectdv( const GLdouble *v1, const GLdouble *v2 );
GLAPI void GLAPIENTRY glRectfv( const GLfloat *v1, const GLfloat *v2 );
GLAPI void GLAPIENTRY glRectiv( const GLint *v1, const GLint *v2 );
GLAPI void GLAPIENTRY glRectsv( const GLshort *v1, const GLshort *v2 );



/*
 * Vertex Arrays  (1.1)
 */

GLAPI void GLAPIENTRY glVertexPointer( GLint size, GLenum type,
                                       GLsizei stride, const GLvoid *ptr );

GLAPI void GLAPIENTRY glNormalPointer( GLenum type, GLsizei stride,
                                       const GLvoid *ptr );

GLAPI void GLAPIENTRY glColorPointer( GLint size, GLenum type,
                                      GLsizei stride, const GLvoid *ptr );

GLAPI void GLAPIENTRY glIndexPointer( GLenum type, GLsizei stride,
                                      const GLvoid *ptr );

GLAPI void GLAPIENTRY glTexCoordPointer( GLint size, GLenum type,
                                         GLsizei stride, const GLvoid *ptr );

GLAPI void GLAPIENTRY glEdgeFlagPointer( GLsizei stride, const GLvoid *ptr );

GLAPI void GLAPIENTRY glGetPointerv( GLenum pname, void **params );

GLAPI void GLAPIENTRY glArrayElement( GLint i );

GLAPI void GLAPIENTRY glDrawArrays( GLenum mode, GLint first, GLsizei count );

GLAPI void GLAPIENTRY glDrawElements( GLenum mode, GLsizei count,
                                      GLenum type, const GLvoid *indices );

GLAPI void GLAPIENTRY glInterleavedArrays( GLenum format, GLsizei stride,
                                           const GLvoid *pointer );


/*
 * Lighting
 */

GLAPI void GLAPIENTRY glShadeModel( GLenum mode );

GLAPI void GLAPIENTRY glLightf( GLenum light, GLenum pname, GLfloat param );
GLAPI void GLAPIENTRY glLighti( GLenum light, GLenum pname, GLint param );
GLAPI void GLAPIENTRY glLightfv( GLenum light, GLenum pname,
                                 const GLfloat *params );
GLAPI void GLAPIENTRY glLightiv( GLenum light, GLenum pname,
                                 const GLint *params );

GLAPI void GLAPIENTRY glGetLightfv( GLenum light, GLenum pname,
                                    GLfloat *params );
GLAPI void GLAPIENTRY glGetLightiv( GLenum light, GLenum pname,
                                    GLint *params );

GLAPI void GLAPIENTRY glLightModelf( GLenum pname, GLfloat param );
GLAPI void GLAPIENTRY glLightModeli( GLenum pname, GLint param );
GLAPI void GLAPIENTRY glLightModelfv( GLenum pname, const GLfloat *params );
GLAPI void GLAPIENTRY glLightModeliv( GLenum pname, const GLint *params );

GLAPI void GLAPIENTRY glMaterialf( GLenum face, GLenum pname, GLfloat param );
GLAPI void GLAPIENTRY glMateriali( GLenum face, GLenum pname, GLint param );
GLAPI void GLAPIENTRY glMaterialfv( GLenum face, GLenum pname, const GLfloat *params );
GLAPI void GLAPIENTRY glMaterialiv( GLenum face, GLenum pname, const GLint *params );

GLAPI void GLAPIENTRY glGetMaterialfv( GLenum face, GLenum pname, GLfloat *params );
GLAPI void GLAPIENTRY glGetMaterialiv( GLenum face, GLenum pname, GLint *params );

GLAPI void GLAPIENTRY glColorMaterial( GLenum face, GLenum mode );




/*
 * Raster functions
 */

GLAPI void GLAPIENTRY glPixelZoom( GLfloat xfactor, GLfloat yfactor );

GLAPI void GLAPIENTRY glPixelStoref( GLenum pname, GLfloat param );
GLAPI void GLAPIENTRY glPixelStorei( GLenum pname, GLint param );

GLAPI void GLAPIENTRY glPixelTransferf( GLenum pname, GLfloat param );
GLAPI void GLAPIENTRY glPixelTransferi( GLenum pname, GLint param );

GLAPI void GLAPIENTRY glPixelMapfv( GLenum map, GLsizei mapsize,
                                    const GLfloat *values );
GLAPI void GLAPIENTRY glPixelMapuiv( GLenum map, GLsizei mapsize,
                                     const GLuint *values );
GLAPI void GLAPIENTRY glPixelMapusv( GLenum map, GLsizei mapsize,
                                     const GLushort *values );

GLAPI void GLAPIENTRY glGetPixelMapfv( GLenum map, GLfloat *values );
GLAPI void GLAPIENTRY glGetPixelMapuiv( GLenum map, GLuint *values );
GLAPI void GLAPIENTRY glGetPixelMapusv( GLenum map, GLushort *values );

GLAPI void GLAPIENTRY glBitmap( GLsizei width, GLsizei height,
                                GLfloat xorig, GLfloat yorig,
                                GLfloat xmove, GLfloat ymove,
                                const GLubyte *bitmap );

GLAPI void GLAPIENTRY glReadPixels( GLint x, GLint y,
                                    GLsizei width, GLsizei height,
                                    GLenum format, GLenum type,
                                    GLvoid *pixels );

GLAPI void GLAPIENTRY glDrawPixels( GLsizei width, GLsizei height,
                                    GLenum format, GLenum type,
                                    const GLvoid *pixels );

GLAPI void GLAPIENTRY glCopyPixels( GLint x, GLint y,
                                    GLsizei width, GLsizei height,
                                    GLenum type );



/*
 * Stenciling
 */

GLAPI void GLAPIENTRY glStencilFunc( GLenum func, GLint ref, GLuint mask );

GLAPI void GLAPIENTRY glStencilMask( GLuint mask );

GLAPI void GLAPIENTRY glStencilOp( GLenum fail, GLenum zfail, GLenum zpass );

GLAPI void GLAPIENTRY glClearStencil( GLint s );



/*
 * Texture mapping
 */

GLAPI void GLAPIENTRY glTexGend( GLenum coord, GLenum pname, GLdouble param );
GLAPI void GLAPIENTRY glTexGenf( GLenum coord, GLenum pname, GLfloat param );
GLAPI void GLAPIENTRY glTexGeni( GLenum coord, GLenum pname, GLint param );

GLAPI void GLAPIENTRY glTexGendv( GLenum coord, GLenum pname, const GLdouble *params );
GLAPI void GLAPIENTRY glTexGenfv( GLenum coord, GLenum pname, const GLfloat *params );
GLAPI void GLAPIENTRY glTexGeniv( GLenum coord, GLenum pname, const GLint *params );

GLAPI void GLAPIENTRY glGetTexGendv( GLenum coord, GLenum pname, GLdouble *params );
GLAPI void GLAPIENTRY glGetTexGenfv( GLenum coord, GLenum pname, GLfloat *params );
GLAPI void GLAPIENTRY glGetTexGeniv( GLenum coord, GLenum pname, GLint *params );


GLAPI void GLAPIENTRY glTexEnvf( GLenum target, GLenum pname, GLfloat param );
GLAPI void GLAPIENTRY glTexEnvi( GLenum target, GLenum pname, GLint param );

GLAPI void GLAPIENTRY glTexEnvfv( GLenum target, GLenum pname, const GLfloat *params );
GLAPI void GLAPIENTRY glTexEnviv( GLenum target, GLenum pname, const GLint *params );

GLAPI void GLAPIENTRY glGetTexEnvfv( GLenum target, GLenum pname, GLfloat *params );
GLAPI void GLAPIENTRY glGetTexEnviv( GLenum target, GLenum pname, GLint *params );


GLAPI void GLAPIENTRY glTexParameterf( GLenum target, GLenum pname, GLfloat param );
GLAPI void GLAPIENTRY glTexParameteri( GLenum target, GLenum pname, GLint param );

GLAPI void GLAPIENTRY glTexParameterfv( GLenum target, GLenum pname,
                                          const GLfloat *params );
GLAPI void GLAPIENTRY glTexParameteriv( GLenum target, GLenum pname,
                                          const GLint *params );

GLAPI void GLAPIENTRY glGetTexParameterfv( GLenum target,
                                           GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetTexParameteriv( GLenum target,
                                           GLenum pname, GLint *params );

GLAPI void GLAPIENTRY glGetTexLevelParameterfv( GLenum target, GLint level,
                                                GLenum pname, GLfloat *params );
GLAPI void GLAPIENTRY glGetTexLevelParameteriv( GLenum target, GLint level,
                                                GLenum pname, GLint *params );


GLAPI void GLAPIENTRY glTexImage1D( GLenum target, GLint level,
                                    GLint internalFormat,
                                    GLsizei width, GLint border,
                                    GLenum format, GLenum type,
                                    const GLvoid *pixels );

GLAPI void GLAPIENTRY glTexImage2D( GLenum target, GLint level,
                                    GLint internalFormat,
                                    GLsizei width, GLsizei height,
                                    GLint border, GLenum format, GLenum type,
                                    const GLvoid *pixels );

GLAPI void GLAPIENTRY glGetTexImage( GLenum target, GLint level,
                                     GLenum format, GLenum type,
                                     GLvoid *pixels );



/* 1.1 functions */

GLAPI void GLAPIENTRY glGenTextures( GLsizei n, GLuint *textures );

GLAPI void GLAPIENTRY glDeleteTextures( GLsizei n, const GLuint *textures);

GLAPI void GLAPIENTRY glBindTexture( GLenum target, GLuint texture );

GLAPI void GLAPIENTRY glPrioritizeTextures( GLsizei n,
                                            const GLuint *textures,
                                            const GLfloat *priorities );

GLAPI GLboolean GLAPIENTRY glAreTexturesResident( GLsizei n,
                                                  const GLuint *textures,
                                                  GLboolean *residences );

GLAPI GLboolean GLAPIENTRY glIsTexture( GLuint texture );


GLAPI void GLAPIENTRY glTexSubImage1D( GLenum target, GLint level,
                                       GLint xoffset,
                                       GLsizei width, GLenum format,
                                       GLenum type, const GLvoid *pixels );


GLAPI void GLAPIENTRY glTexSubImage2D( GLenum target, GLint level,
                                       GLint xoffset, GLint yoffset,
                                       GLsizei width, GLsizei height,
                                       GLenum format, GLenum type,
                                       const GLvoid *pixels );


GLAPI void GLAPIENTRY glCopyTexImage1D( GLenum target, GLint level,
                                        GLenum internalformat,
                                        GLint x, GLint y,
                                        GLsizei width, GLint border );


GLAPI void GLAPIENTRY glCopyTexImage2D( GLenum target, GLint level,
                                        GLenum internalformat,
                                        GLint x, GLint y,
                                        GLsizei width, GLsizei height,
                                        GLint border );


GLAPI void GLAPIENTRY glCopyTexSubImage1D( GLenum target, GLint level,
                                           GLint xoffset, GLint x, GLint y,
                                           GLsizei width );


GLAPI void GLAPIENTRY glCopyTexSubImage2D( GLenum target, GLint level,
                                           GLint xoffset, GLint yoffset,
                                           GLint x, GLint y,
                                           GLsizei width, GLsizei height );




/*
 * Evaluators
 */

GLAPI void GLAPIENTRY glMap1d( GLenum target, GLdouble u1, GLdouble u2,
                               GLint stride,
                               GLint order, const GLdouble *points );
GLAPI void GLAPIENTRY glMap1f( GLenum target, GLfloat u1, GLfloat u2,
                               GLint stride,
                               GLint order, const GLfloat *points );

GLAPI void GLAPIENTRY glMap2d( GLenum target,
                     GLdouble u1, GLdouble u2, GLint ustride, GLint uorder,
                     GLdouble v1, GLdouble v2, GLint vstride, GLint vorder,
                     const GLdouble *points );
GLAPI void GLAPIENTRY glMap2f( GLenum target,
                     GLfloat u1, GLfloat u2, GLint ustride, GLint uorder,
                     GLfloat v1, GLfloat v2, GLint vstride, GLint vorder,
                     const GLfloat *points );

GLAPI void GLAPIENTRY glGetMapdv( GLenum target, GLenum query, GLdouble *v );
GLAPI void GLAPIENTRY glGetMapfv( GLenum target, GLenum query, GLfloat *v );
GLAPI void GLAPIENTRY glGetMapiv( GLenum target, GLenum query, GLint *v );

GLAPI void GLAPIENTRY glEvalCoord1d( GLdouble u );
GLAPI void GLAPIENTRY glEvalCoord1f( GLfloat u );

GLAPI void GLAPIENTRY glEvalCoord1dv( const GLdouble *u );
GLAPI void GLAPIENTRY glEvalCoord1fv( const GLfloat *u );

GLAPI void GLAPIENTRY glEvalCoord2d( GLdouble u, GLdouble v );
GLAPI void GLAPIENTRY glEvalCoord2f( GLfloat u, GLfloat v );

GLAPI void GLAPIENTRY glEvalCoord2dv( const GLdouble *u );
GLAPI void GLAPIENTRY glEvalCoord2fv( const GLfloat *u );

GLAPI void GLAPIENTRY glMapGrid1d( GLint un, GLdouble u1, GLdouble u2 );
GLAPI void GLAPIENTRY glMapGrid1f( GLint un, GLfloat u1, GLfloat u2 );

GLAPI void GLAPIENTRY glMapGrid2d( GLint un, GLdouble u1, GLdouble u2,
                                   GLint vn, GLdouble v1, GLdouble v2 );
GLAPI void GLAPIENTRY glMapGrid2f( GLint un, GLfloat u1, GLfloat u2,
                                   GLint vn, GLfloat v1, GLfloat v2 );

GLAPI void GLAPIENTRY glEvalPoint1( GLint i );

GLAPI void GLAPIENTRY glEvalPoint2( GLint i, GLint j );

GLAPI void GLAPIENTRY glEvalMesh1( GLenum mode, GLint i1, GLint i2 );

GLAPI void GLAPIENTRY glEvalMesh2( GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2 );



/*
 * Fog
 */

GLAPI void GLAPIENTRY glFogf( GLenum pname, GLfloat param );

GLAPI void GLAPIENTRY glFogi( GLenum pname, GLint param );

GLAPI void GLAPIENTRY glFogfv( GLenum pname, const GLfloat *params );

GLAPI void GLAPIENTRY glFogiv( GLenum pname, const GLint *params );



/*
 * Selection and Feedback
 */

GLAPI void GLAPIENTRY glFeedbackBuffer( GLsizei size, GLenum type, GLfloat *buffer );

GLAPI void GLAPIENTRY glPassThrough( GLfloat token );

GLAPI void GLAPIENTRY glSelectBuffer( GLsizei size, GLuint *buffer );

GLAPI void GLAPIENTRY glInitNames( void );

GLAPI void GLAPIENTRY glLoadName( GLuint name );

GLAPI void GLAPIENTRY glPushName( GLuint name );

GLAPI void GLAPIENTRY glPopName( void );



/* 1.2 functions */
GLAPI void GLAPIENTRY glDrawRangeElements( GLenum mode, GLuint start,
        GLuint end, GLsizei count, GLenum type, const GLvoid *indices );

GLAPI void GLAPIENTRY glTexImage3D( GLenum target, GLint level,
                                      GLint internalFormat,
                                      GLsizei width, GLsizei height,
                                      GLsizei depth, GLint border,
                                      GLenum format, GLenum type,
                                      const GLvoid *pixels );

GLAPI void GLAPIENTRY glTexSubImage3D( GLenum target, GLint level,
                                         GLint xoffset, GLint yoffset,
                                         GLint zoffset, GLsizei width,
                                         GLsizei height, GLsizei depth,
                                         GLenum format,
                                         GLenum type, const GLvoid *pixels);

GLAPI void GLAPIENTRY glCopyTexSubImage3D( GLenum target, GLint level,
                                             GLint xoffset, GLint yoffset,
                                             GLint zoffset, GLint x,
                                             GLint y, GLsizei width,
                                             GLsizei height );


/* 1.2 imaging extension functions */

GLAPI void GLAPIENTRY glColorTable( GLenum target, GLenum internalformat,
                                    GLsizei width, GLenum format,
                                    GLenum type, const GLvoid *table );

GLAPI void GLAPIENTRY glColorSubTable( GLenum target,
                                       GLsizei start, GLsizei count,
                                       GLenum format, GLenum type,
                                       const GLvoid *data );

GLAPI void GLAPIENTRY glColorTableParameteriv(GLenum target, GLenum pname,
                                              const GLint *params);

GLAPI void GLAPIENTRY glColorTableParameterfv(GLenum target, GLenum pname,
                                              const GLfloat *params);

GLAPI void GLAPIENTRY glCopyColorSubTable( GLenum target, GLsizei start,
                                           GLint x, GLint y, GLsizei width );

GLAPI void GLAPIENTRY glCopyColorTable( GLenum target, GLenum internalformat,
                                        GLint x, GLint y, GLsizei width );

GLAPI void GLAPIENTRY glGetColorTable( GLenum target, GLenum format,
                                       GLenum type, GLvoid *table );

GLAPI void GLAPIENTRY glGetColorTableParameterfv( GLenum target, GLenum pname,
                                                  GLfloat *params );

GLAPI void GLAPIENTRY glGetColorTableParameteriv( GLenum target, GLenum pname,
                                                  GLint *params );

GLAPI void GLAPIENTRY glBlendEquation( GLenum mode );

GLAPI void GLAPIENTRY glBlendColor( GLfloat red, GLfloat green,
                                    GLfloat blue, GLfloat alpha );

GLAPI void GLAPIENTRY glHistogram( GLenum target, GLsizei width,
                                   GLenum internalformat, GLboolean sink );

GLAPI void GLAPIENTRY glResetHistogram( GLenum target );

GLAPI void GLAPIENTRY glGetHistogram( GLenum target, GLboolean reset,
                                      GLenum format, GLenum type,
                                      GLvoid *values );

GLAPI void GLAPIENTRY glGetHistogramParameterfv( GLenum target, GLenum pname,
                                                 GLfloat *params );

GLAPI void GLAPIENTRY glGetHistogramParameteriv( GLenum target, GLenum pname,
                                                 GLint *params );

GLAPI void GLAPIENTRY glMinmax( GLenum target, GLenum internalformat,
                                GLboolean sink );

GLAPI void GLAPIENTRY glResetMinmax( GLenum target );

GLAPI void GLAPIENTRY glGetMinmax( GLenum target, GLboolean reset,
                                   GLenum format, GLenum types,
                                   GLvoid *values );

GLAPI void GLAPIENTRY glGetMinmaxParameterfv( GLenum target, GLenum pname,
                                              GLfloat *params );

GLAPI void GLAPIENTRY glGetMinmaxParameteriv( GLenum target, GLenum pname,
                                              GLint *params );

GLAPI void GLAPIENTRY glConvolutionFilter1D( GLenum target,
        GLenum internalformat, GLsizei width, GLenum format, GLenum type,
        const GLvoid *image );

GLAPI void GLAPIENTRY glConvolutionFilter2D( GLenum target,
        GLenum internalformat, GLsizei width, GLsizei height, GLenum format,
        GLenum type, const GLvoid *image );

GLAPI void GLAPIENTRY glConvolutionParameterf( GLenum target, GLenum pname,
        GLfloat params );

GLAPI void GLAPIENTRY glConvolutionParameterfv( GLenum target, GLenum pname,
        const GLfloat *params );

GLAPI void GLAPIENTRY glConvolutionParameteri( GLenum target, GLenum pname,
        GLint params );

GLAPI void GLAPIENTRY glConvolutionParameteriv( GLenum target, GLenum pname,
        const GLint *params );

GLAPI void GLAPIENTRY glCopyConvolutionFilter1D( GLenum target,
        GLenum internalformat, GLint x, GLint y, GLsizei width );

GLAPI void GLAPIENTRY glCopyConvolutionFilter2D( GLenum target,
        GLenum internalformat, GLint x, GLint y, GLsizei width,
        GLsizei height);

GLAPI void GLAPIENTRY glGetConvolutionFilter( GLenum target, GLenum format,
        GLenum type, GLvoid *image );

GLAPI void GLAPIENTRY glGetConvolutionParameterfv( GLenum target, GLenum pname,
        GLfloat *params );

GLAPI void GLAPIENTRY glGetConvolutionParameteriv( GLenum target, GLenum pname,
        GLint *params );

GLAPI void GLAPIENTRY glSeparableFilter2D( GLenum target,
        GLenum internalformat, GLsizei width, GLsizei height, GLenum format,
        GLenum type, const GLvoid *row, const GLvoid *column );

GLAPI void GLAPIENTRY glGetSeparableFilter( GLenum target, GLenum format,
        GLenum type, GLvoid *row, GLvoid *column, GLvoid *span );



/*
 * GL_ARB_multitexture (ARB extension 1 and OpenGL 1.2.1)
 */
#ifndef GL_ARB_multitexture
#define GL_ARB_multitexture 1


GLAPI void GLAPIENTRY glActiveTextureARB(GLenum texture);
GLAPI void GLAPIENTRY glClientActiveTextureARB(GLenum texture);
GLAPI void GLAPIENTRY glMultiTexCoord1dARB(GLenum target, GLdouble s);
GLAPI void GLAPIENTRY glMultiTexCoord1dvARB(GLenum target, const GLdouble *v);
GLAPI void GLAPIENTRY glMultiTexCoord1fARB(GLenum target, GLfloat s);
GLAPI void GLAPIENTRY glMultiTexCoord1fvARB(GLenum target, const GLfloat *v);
GLAPI void GLAPIENTRY glMultiTexCoord1iARB(GLenum target, GLint s);
GLAPI void GLAPIENTRY glMultiTexCoord1ivARB(GLenum target, const GLint *v);
GLAPI void GLAPIENTRY glMultiTexCoord1sARB(GLenum target, GLshort s);
GLAPI void GLAPIENTRY glMultiTexCoord1svARB(GLenum target, const GLshort *v);
GLAPI void GLAPIENTRY glMultiTexCoord2dARB(GLenum target, GLdouble s, GLdouble t);
GLAPI void GLAPIENTRY glMultiTexCoord2dvARB(GLenum target, const GLdouble *v);
GLAPI void GLAPIENTRY glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t);
GLAPI void GLAPIENTRY glMultiTexCoord2fvARB(GLenum target, const GLfloat *v);
GLAPI void GLAPIENTRY glMultiTexCoord2iARB(GLenum target, GLint s, GLint t);
GLAPI void GLAPIENTRY glMultiTexCoord2ivARB(GLenum target, const GLint *v);
GLAPI void GLAPIENTRY glMultiTexCoord2sARB(GLenum target, GLshort s, GLshort t);
GLAPI void GLAPIENTRY glMultiTexCoord2svARB(GLenum target, const GLshort *v);
GLAPI void GLAPIENTRY glMultiTexCoord3dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r);
GLAPI void GLAPIENTRY glMultiTexCoord3dvARB(GLenum target, const GLdouble *v);
GLAPI void GLAPIENTRY glMultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r);
GLAPI void GLAPIENTRY glMultiTexCoord3fvARB(GLenum target, const GLfloat *v);
GLAPI void GLAPIENTRY glMultiTexCoord3iARB(GLenum target, GLint s, GLint t, GLint r);
GLAPI void GLAPIENTRY glMultiTexCoord3ivARB(GLenum target, const GLint *v);
GLAPI void GLAPIENTRY glMultiTexCoord3sARB(GLenum target, GLshort s, GLshort t, GLshort r);
GLAPI void GLAPIENTRY glMultiTexCoord3svARB(GLenum target, const GLshort *v);
GLAPI void GLAPIENTRY glMultiTexCoord4dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
GLAPI void GLAPIENTRY glMultiTexCoord4dvARB(GLenum target, const GLdouble *v);
GLAPI void GLAPIENTRY glMultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLAPI void GLAPIENTRY glMultiTexCoord4fvARB(GLenum target, const GLfloat *v);
GLAPI void GLAPIENTRY glMultiTexCoord4iARB(GLenum target, GLint s, GLint t, GLint r, GLint q);
GLAPI void GLAPIENTRY glMultiTexCoord4ivARB(GLenum target, const GLint *v);
GLAPI void GLAPIENTRY glMultiTexCoord4sARB(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
GLAPI void GLAPIENTRY glMultiTexCoord4svARB(GLenum target, const GLshort *v);

#endif /* GL_ARB_multitexture */


/**********************************************************************
 * Begin system-specific stuff
 */
#if defined(__BEOS__) || defined(__QUICKDRAW__)
#pragma export off
#endif

#if defined(macintosh) && PRAGMA_IMPORT_SUPPORTED
#pragma import off
#endif
/*
 * End system-specific stuff
 **********************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* __gl_h_ */
