/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __GL_WINDOWS_H_
#define __GL_WINDOWS_H_

#include "GLInterceptConfig.h"
#include "GLDriver.h"

//Windows specific header
#ifdef GLI_BUILD_WINDOWS

#ifdef _MSC_VER
#pragma warning (disable:4273)   // No complaints about DLL linkage.
#endif // _MSC_VER 


#define WGLAPI __declspec(dllexport)
#define WGLAPIENTRY __stdcall

extern "C" {

WGLAPI int   WGLAPIENTRY wglChoosePixelFormat     (HDC a, CONST PIXELFORMATDESCRIPTOR *b);
WGLAPI BOOL  WGLAPIENTRY wglCopyContext           (HGLRC a, HGLRC b, UINT c);
WGLAPI HGLRC WGLAPIENTRY wglCreateContext         (HDC a);
WGLAPI HGLRC WGLAPIENTRY wglCreateLayerContext    (HDC a, int b);
WGLAPI BOOL  WGLAPIENTRY wglDeleteContext         (HGLRC a);
WGLAPI BOOL  WGLAPIENTRY wglDescribeLayerPlane    (HDC a, int b, int c, UINT d, LPLAYERPLANEDESCRIPTOR e);
WGLAPI int   WGLAPIENTRY wglDescribePixelFormat   (HDC a, int b, UINT c, LPPIXELFORMATDESCRIPTOR d);
WGLAPI HGLRC WGLAPIENTRY wglGetCurrentContext     (void);
WGLAPI HDC   WGLAPIENTRY wglGetCurrentDC          (void);
WGLAPI PROC  WGLAPIENTRY wglGetDefaultProcAddress (LPCSTR a);
WGLAPI int   WGLAPIENTRY wglGetLayerPaletteEntries(HDC a, int b, int c, int d, COLORREF *e);
WGLAPI int   WGLAPIENTRY wglGetPixelFormat        (HDC a);
WGLAPI PROC  WGLAPIENTRY wglGetProcAddress        (LPCSTR a);
WGLAPI BOOL  WGLAPIENTRY wglMakeCurrent           (HDC a, HGLRC b);
WGLAPI BOOL  WGLAPIENTRY wglRealizeLayerPalette   (HDC a, int b, BOOL c);
WGLAPI int   WGLAPIENTRY wglSetLayerPaletteEntries(HDC a, int b, int c, int d, CONST COLORREF *e);
WGLAPI BOOL  WGLAPIENTRY wglSetPixelFormat        (HDC a, int b, CONST PIXELFORMATDESCRIPTOR *c);
WGLAPI BOOL  WGLAPIENTRY wglShareLists            (HGLRC a, HGLRC b);
WGLAPI BOOL  WGLAPIENTRY wglSwapBuffers           (HDC a);
WGLAPI BOOL  WGLAPIENTRY wglSwapLayerBuffers      (HDC a, UINT b);
WGLAPI DWORD WGLAPIENTRY wglSwapMultipleBuffers   (UINT, const WGLSWAP *);
WGLAPI BOOL  WGLAPIENTRY wglUseFontBitmapsA       (HDC a, DWORD b, DWORD c, DWORD d);
WGLAPI BOOL  WGLAPIENTRY wglUseFontBitmapsW       (HDC a, DWORD b, DWORD c, DWORD d);
WGLAPI BOOL  WGLAPIENTRY wglUseFontOutlinesA      (HDC a, DWORD b, DWORD c, DWORD d, FLOAT e, FLOAT f, int g, LPGLYPHMETRICSFLOAT h);
WGLAPI BOOL  WGLAPIENTRY wglUseFontOutlinesW      (HDC a, DWORD b, DWORD c, DWORD d, FLOAT e, FLOAT f, int g, LPGLYPHMETRICSFLOAT h);

}

#endif // GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

extern "C" {

//Core GLX 1.2
XVisualInfo* glXChooseVisual          (Display *dpy, int screen, int *attribList);
void         glXCopyContext           (Display *dpy, GLXContext src, GLXContext dst, unsigned long mask);
GLXContext   glXCreateContext         (Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
GLXPixmap    glXCreateGLXPixmap       (Display *dpy, XVisualInfo *vis, Pixmap pixmap);
void         glXDestroyContext        (Display *dpy, GLXContext ctx);
void         glXDestroyGLXPixmap      (Display *dpy, GLXPixmap pix);
int          glXGetConfig             (Display *dpy, XVisualInfo *vis, int attrib, int *value);
GLXContext   glXGetCurrentContext     (void);
GLXDrawable  glXGetCurrentDrawable    (void);
Bool         glXIsDirect              (Display *dpy, GLXContext ctx);
Bool         glXMakeCurrent           (Display *dpy, GLXDrawable drawable, GLXContext ctx);
Bool         glXQueryExtension        (Display *dpy, int *errorBase, int *eventBase);
Bool         glXQueryVersion          (Display *dpy, int *major, int *minor);
void         glXSwapBuffers           (Display *dpy, GLXDrawable drawable);
void         glXUseXFont              (Font font, int first, int count, int listBase);
void         glXWaitGL                (void);
void         glXWaitX                 (void);
const char * glXGetClientString       (Display *dpy, int name );
const char * glXQueryServerString     (Display *dpy, int screen, int name );
const char * glXQueryExtensionsString (Display *dpy, int screen );

//GLX 1.3
GLXFBConfig* glXGetFBConfigs          (Display *dpy, int screen, int *nelements);
GLXFBConfig* glXChooseFBConfig        (Display *dpy, int screen, const int *attrib_list, int *nelements);
int          glXGetFBConfigAttrib     (Display *dpy, GLXFBConfig config, int attribute, int *value);
XVisualInfo* glXGetVisualFromFBConfig (Display *dpy, GLXFBConfig config);
GLXWindow    glXCreateWindow          (Display *dpy, GLXFBConfig config, Window win, const int *attrib_list);
void         glXDestroyWindow         (Display *dpy, GLXWindow win);
GLXPixmap    glXCreatePixmap          (Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list);
void         glXDestroyPixmap         (Display *dpy, GLXPixmap pixmap);
GLXPbuffer   glXCreatePbuffer         (Display *dpy, GLXFBConfig config, const int *attrib_list);
void         glXDestroyPbuffer        (Display *dpy, GLXPbuffer pbuf);
void         glXQueryDrawable         (Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
GLXContext   glXCreateNewContext      (Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
Bool         glXMakeContextCurrent    (Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
GLXDrawable  glXGetCurrentReadDrawable(void);
Display *    glXGetCurrentDisplay     (void);
int          glXQueryContext          (Display *dpy, GLXContext ctx, int attribute, int *value);
void         glXSelectEvent           (Display *dpy, GLXDrawable draw, unsigned long event_mask);
void         glXGetSelectedEvent      (Display *dpy, GLXDrawable draw, unsigned long *event_mask);

//Required ARB extension
void *       glXGetProcAddressARB     (const GLubyte *procname);

//GLX 1.4
void *       glXGetProcAddress        (const GLubyte *procname);

}

#endif //GLI_BUILD_LINUX

#endif // __GL_WINDOWS_H_

