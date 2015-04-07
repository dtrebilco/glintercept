/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __GLSTUBS_WGL_H
#define __GLSTUBS_WGL_H

#include "OSDefines.h"
#include "GLDefines.h"

///////////////////////////////////////////////////////////////////////////////
#ifdef GLI_BUILD_WINDOWS

//Windows OpenGL API methods
struct WGLDriver
{
  int   (GLAPIENTRY *wglChoosePixelFormat)       (HDC a, CONST PIXELFORMATDESCRIPTOR *b);
  BOOL  (GLAPIENTRY *wglCopyContext)             (HGLRC a, HGLRC b, UINT c);
  HGLRC (GLAPIENTRY *wglCreateContext)           (HDC a);
  HGLRC (GLAPIENTRY *wglCreateLayerContext)      (HDC a, int b);
  BOOL  (GLAPIENTRY *wglDeleteContext)           (HGLRC a);
  BOOL  (GLAPIENTRY *wglDescribeLayerPlane)      (HDC a, int b, int c, UINT d, LPLAYERPLANEDESCRIPTOR e);
  int   (GLAPIENTRY *wglDescribePixelFormat)     (HDC a, int b, UINT c, LPPIXELFORMATDESCRIPTOR d);
  HGLRC (GLAPIENTRY *wglGetCurrentContext)       (void);
  HDC   (GLAPIENTRY *wglGetCurrentDC)            (void);
  PROC  (GLAPIENTRY *wglGetDefaultProcAddress)   (LPCSTR a);
  int   (GLAPIENTRY *wglGetLayerPaletteEntries)  (HDC a, int b, int c, int d, COLORREF *e);
  int   (GLAPIENTRY *wglGetPixelFormat)          (HDC a);
  PROC  (GLAPIENTRY *wglGetProcAddress)          (LPCSTR a);
  BOOL  (GLAPIENTRY *wglMakeCurrent)             (HDC a, HGLRC b);
  BOOL  (GLAPIENTRY *wglRealizeLayerPalette)     (HDC a, int b, BOOL c);
  int   (GLAPIENTRY *wglSetLayerPaletteEntries)  (HDC a, int b, int c, int d, CONST COLORREF *e);
  BOOL  (GLAPIENTRY *wglSetPixelFormat)          (HDC a, int b, CONST PIXELFORMATDESCRIPTOR *c);
  BOOL  (GLAPIENTRY *wglShareLists)              (HGLRC a, HGLRC b);
  BOOL  (GLAPIENTRY *wglSwapBuffers)             (HDC a);
  BOOL  (GLAPIENTRY *wglSwapLayerBuffers)        (HDC a, UINT b);
  long  (GLAPIENTRY *wglSwapMultipleBuffers)     (UINT, const WGLSWAP *);
  BOOL  (GLAPIENTRY *wglUseFontBitmapsA)         (HDC a, DWORD b, DWORD c, DWORD d);
  BOOL  (GLAPIENTRY *wglUseFontBitmapsW)         (HDC a, DWORD b, DWORD c, DWORD d);
  BOOL  (GLAPIENTRY *wglUseFontOutlinesA)        (HDC a, DWORD b, DWORD c, DWORD d, FLOAT e, FLOAT f, int g, LPGLYPHMETRICSFLOAT h);
  BOOL  (GLAPIENTRY *wglUseFontOutlinesW)        (HDC a, DWORD b, DWORD c, DWORD d, FLOAT e, FLOAT f, int g, LPGLYPHMETRICSFLOAT h);
  
  //OS independent extension retrieval
  PROC  (GLAPIENTRY *glGetProcAddress)           (LPCSTR a);
  
};

#endif //GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
#ifdef GLI_BUILD_LINUX

//X-Windows OpenGL API methods
struct WGLDriver
{
  //Core GLX 1.2
  XVisualInfo* (*glXChooseVisual)          (Display *dpy, int screen, int *attribList);
  void         (*glXCopyContext)           (Display *dpy, GLXContext src, GLXContext dst, unsigned long mask);
  GLXContext   (*glXCreateContext)         (Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
  GLXPixmap    (*glXCreateGLXPixmap)       (Display *dpy, XVisualInfo *vis, Pixmap pixmap);
  void         (*glXDestroyContext)        (Display *dpy, GLXContext ctx);
  void         (*glXDestroyGLXPixmap)      (Display *dpy, GLXPixmap pix);
  int          (*glXGetConfig)             (Display *dpy, XVisualInfo *vis, int attrib, int *value);
  GLXContext   (*glXGetCurrentContext)     (void);
  GLXDrawable  (*glXGetCurrentDrawable)    (void);
  Bool         (*glXIsDirect)              (Display *dpy, GLXContext ctx);
  Bool         (*glXMakeCurrent)           (Display *dpy, GLXDrawable drawable, GLXContext ctx);
  Bool         (*glXQueryExtension)        (Display *dpy, int *errorBase, int *eventBase);
  Bool         (*glXQueryVersion)          (Display *dpy, int *major, int *minor);
  void         (*glXSwapBuffers)           (Display *dpy, GLXDrawable drawable);
  void         (*glXUseXFont)              (Font font, int first, int count, int listBase);
  void         (*glXWaitGL)                (void);
  void         (*glXWaitX)                 (void);
  const char * (*glXGetClientString)       (Display *dpy, int name );
  const char * (*glXQueryServerString)     (Display *dpy, int screen, int name );
  const char * (*glXQueryExtensionsString) (Display *dpy, int screen );

  //GLX 1.3
  GLXFBConfig* (*glXGetFBConfigs)          (Display *dpy, int screen, int *nelements);
  GLXFBConfig* (*glXChooseFBConfig)        (Display *dpy, int screen, const int *attrib_list, int *nelements);
  int          (*glXGetFBConfigAttrib)     (Display *dpy, GLXFBConfig config, int attribute, int *value);
  XVisualInfo* (*glXGetVisualFromFBConfig) (Display *dpy, GLXFBConfig config);
  GLXWindow    (*glXCreateWindow)          (Display *dpy, GLXFBConfig config, Window win, const int *attrib_list);
  void         (*glXDestroyWindow)         (Display *dpy, GLXWindow win);
  GLXPixmap    (*glXCreatePixmap)          (Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list);
  void         (*glXDestroyPixmap)         (Display *dpy, GLXPixmap pixmap);
  GLXPbuffer   (*glXCreatePbuffer)         (Display *dpy, GLXFBConfig config, const int *attrib_list);
  void         (*glXDestroyPbuffer)        (Display *dpy, GLXPbuffer pbuf);
  void         (*glXQueryDrawable)         (Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
  GLXContext   (*glXCreateNewContext)      (Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
  Bool         (*glXMakeContextCurrent)    (Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
  GLXDrawable  (*glXGetCurrentReadDrawable)(void);
  Display *    (*glXGetCurrentDisplay)     (void);
  int          (*glXQueryContext)          (Display *dpy, GLXContext ctx, int attribute, int *value);
  void         (*glXSelectEvent)           (Display *dpy, GLXDrawable draw, unsigned long event_mask);
  void         (*glXGetSelectedEvent)      (Display *dpy, GLXDrawable draw, unsigned long *event_mask);

  //GLX 1.4 (In future these may have to return a function pointer typedef (not just void*))
  void *       (*glXGetProcAddress)        (const GLubyte *procname);
  void *       (*glXGetProcAddressARB)     (const GLubyte *procname);
  
  //OS independent extension retrieval
  void *       (*glGetProcAddress)         (const char *procname);

};

#endif //GLI_BUILD_LINUX


#endif // __GLSTUBS_WGL_H
