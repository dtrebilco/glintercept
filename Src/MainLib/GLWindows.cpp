/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "GLWindows.h"
#include "BuiltInFunction.h"
#include <stdlib.h>
#include <string.h>

// NOTE: There are a few hacks where some functions are not logged in the file - this is due to multi-thread calls made by some drivers (eg Nvidia)

extern GLDriver glDriver;
  
USING_ERRORLOG

static bool init=false; 
#define INIT_DRIVER if(!init) {init=true; if(!glDriver.Init()){exit(1);}}

//A dummy value to pass to the logger
int wglDummyValue=0xBAADF00D;

//The version of the OpenGL functions that the builtin wrappers call
// (Functions in this table may be over-ridden/replaced)
extern WGLDriver GLW_Builtin;

#ifdef GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglChoosePixelFormat)
int WGLAPIENTRY wglChoosePixelFormat (HDC a, CONST PIXELFORMATDESCRIPTOR *b)
{
  INIT_DRIVER

  PRE_FUNCTION(wglChoosePixelFormat,(a,b),a);
  int retValue = GLW_Builtin.wglChoosePixelFormat(a,b);
  POST_FUNCTION_RET(wglChoosePixelFormat,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglCopyContext)
BOOL WGLAPIENTRY wglCopyContext (HGLRC a, HGLRC b, UINT c)
{
  INIT_DRIVER

  PRE_FUNCTION(wglCopyContext,(a,b,c),a);
  BOOL retValue = GLW_Builtin.wglCopyContext(a,b,c);
  POST_FUNCTION_RET(wglCopyContext,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglCreateContext)
HGLRC WGLAPIENTRY wglCreateContext (HDC a)
{
  INIT_DRIVER

  PRE_FUNCTION(wglCreateContext,(a),a);
  HGLRC retValue = GLW_Builtin.wglCreateContext(a);
  POST_FUNCTION_RET(wglCreateContext,retValue)

  //Create our driver context
  if(retValue != NULL && glDriver.GetFunctionCallDepth() == 0)
  {
    glDriver.CreateOpenGLContext(retValue);
  }

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglCreateLayerContext)
HGLRC WGLAPIENTRY wglCreateLayerContext (HDC a, int b)
{
  INIT_DRIVER

  PRE_FUNCTION(wglCreateLayerContext,(a,b),a);
  HGLRC retValue = GLW_Builtin.wglCreateLayerContext(a,b);
  POST_FUNCTION_RET(wglCreateLayerContext,retValue)

  //Create our driver context
  if(retValue != NULL && glDriver.GetFunctionCallDepth() == 0)
  {
    glDriver.CreateOpenGLContext(retValue);
  }

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglDeleteContext)
BOOL WGLAPIENTRY wglDeleteContext (HGLRC a)
{
  INIT_DRIVER

  PRE_FUNCTION(wglDeleteContext,(a),a);
  BOOL retValue = GLW_Builtin.wglDeleteContext(a);

  //Delete for our context
  if(retValue)
  {
    glDriver.DeleteOpenGLContext(a);
  }
  
  POST_FUNCTION_RET(wglDeleteContext,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglDescribeLayerPlane)
BOOL WGLAPIENTRY wglDescribeLayerPlane (HDC a, int b, int c, UINT d, LPLAYERPLANEDESCRIPTOR e)
{
  INIT_DRIVER

  //DT_TODO: Hack  - not logging this function until GLIntercept is thread safe (can be called without a context - Nvidia drivers etc)
  //PRE_FUNCTION(wglDescribeLayerPlane,(a,b,c,d,e),a);
  BOOL retValue = GLW_Builtin.wglDescribeLayerPlane(a,b,c,d,e);
  //POST_FUNCTION_RET(wglDescribeLayerPlane,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglDescribePixelFormat)
int WGLAPIENTRY wglDescribePixelFormat (HDC a, int b, UINT c, LPPIXELFORMATDESCRIPTOR d)
{
  INIT_DRIVER

  //DT_TODO: Hack  - not logging this function until GLIntercept is thread safe (can be called without a context - Nvidia drivers etc)
  //PRE_FUNCTION(wglDescribePixelFormat,(a,b,c,d),a);
  int retValue = GLW_Builtin.wglDescribePixelFormat(a,b,c,d);
   //POST_FUNCTION_RET(wglDescribePixelFormat,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglGetCurrentContext)
HGLRC WGLAPIENTRY wglGetCurrentContext (void)
{
  INIT_DRIVER

  PRE_FUNCTION(wglGetCurrentContext,(),wglDummyValue);
  HGLRC retValue = GLW_Builtin.wglGetCurrentContext();
  POST_FUNCTION_RET(wglGetCurrentContext,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglGetCurrentDC)
HDC WGLAPIENTRY wglGetCurrentDC (void)
{
  INIT_DRIVER

  PRE_FUNCTION(wglGetCurrentDC,(),wglDummyValue);
  HDC retValue = GLW_Builtin.wglGetCurrentDC();
  POST_FUNCTION_RET(wglGetCurrentDC,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglGetDefaultProcAddress)
PROC WGLAPIENTRY wglGetDefaultProcAddress (LPCSTR a)
{
  INIT_DRIVER

  PRE_FUNCTION(wglGetDefaultProcAddress,(a),a);
  PROC retValue = GLW_Builtin.wglGetDefaultProcAddress(a);
  POST_FUNCTION_RET(wglGetDefaultProcAddress,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglGetLayerPaletteEntries)
int WGLAPIENTRY wglGetLayerPaletteEntries (HDC a, int b, int c, int d, COLORREF *e)
{
  INIT_DRIVER

  PRE_FUNCTION(wglGetLayerPaletteEntries,(a,b,c,d,e),a);
  int retValue = GLW_Builtin.wglGetLayerPaletteEntries(a,b,c,d,e);
  POST_FUNCTION_RET(wglGetLayerPaletteEntries,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglGetPixelFormat)
int WGLAPIENTRY wglGetPixelFormat (HDC a)
{
  INIT_DRIVER

  //DT_TODO: Hack  - not logging this function until GLIntercept is thread safe (can be called without a context - Nvidia drivers etc)
  //PRE_FUNCTION(wglGetPixelFormat,(a),a);
  int retValue = GLW_Builtin.wglGetPixelFormat(a);
  //POST_FUNCTION_RET(wglGetPixelFormat,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglGetProcAddress)
PROC WGLAPIENTRY wglGetProcAddress (LPCSTR a)
{
  INIT_DRIVER

  PRE_FUNCTION(wglGetProcAddress,(a),a);
  PROC retValue = GLW_Builtin.wglGetProcAddress(a);

  //Register the new function (NOTE: "NULL" functions can be overridden)
  retValue = (PROC)glDriver.AddExtensionFunction(a,retValue);
  
  POST_FUNCTION_RET(wglGetProcAddress,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglMakeCurrent)
BOOL WGLAPIENTRY wglMakeCurrent (HDC a, HGLRC b)
{
  INIT_DRIVER

  //Get the current context
  HGLRC oldContext = GLW_Builtin.wglGetCurrentContext();

  PRE_FUNCTION(wglMakeCurrent,(a,b),a);

  //NULL out the internal context
  glDriver.SetOpenGLContext(NULL);

  BOOL retValue = GLW_Builtin.wglMakeCurrent(a,b);
  
  //Call our driver to make the context valid
  if(retValue)
  {
    glDriver.SetOpenGLContext(b);
  }
  else
  {
    //Re-init the old context if the swap was not successful
    glDriver.SetOpenGLContext(oldContext);
  }

  POST_FUNCTION_RET(wglMakeCurrent,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglRealizeLayerPalette)
BOOL WGLAPIENTRY wglRealizeLayerPalette (HDC a, int b, BOOL c)
{
  INIT_DRIVER

  PRE_FUNCTION(wglRealizeLayerPalette,(a,b,c),a);
  BOOL retValue = GLW_Builtin.wglRealizeLayerPalette(a,b,c);
  POST_FUNCTION_RET(wglRealizeLayerPalette,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglSetLayerPaletteEntries)
int WGLAPIENTRY wglSetLayerPaletteEntries (HDC a, int b, int c, int d, CONST COLORREF *e)
{
  INIT_DRIVER

  PRE_FUNCTION(wglSetLayerPaletteEntries,(a,b,c,d,e),a);
  int retValue = GLW_Builtin.wglSetLayerPaletteEntries(a,b,c,d,e);
  POST_FUNCTION_RET(wglSetLayerPaletteEntries,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglSetPixelFormat)
BOOL WGLAPIENTRY wglSetPixelFormat (HDC a, int b, CONST PIXELFORMATDESCRIPTOR *c)
{
  INIT_DRIVER

  PRE_FUNCTION(wglSetPixelFormat,(a,b,c),a);
  BOOL retValue = GLW_Builtin.wglSetPixelFormat(a,b,c);
  POST_FUNCTION_RET(wglSetPixelFormat,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglShareLists)
BOOL WGLAPIENTRY wglShareLists (HGLRC a, HGLRC b)
{
  INIT_DRIVER

  PRE_FUNCTION(wglShareLists,(a,b),a);
  BOOL retValue = GLW_Builtin.wglShareLists(a,b);
 
  //If the sharing was successful, share our internal lists
  if(retValue)
  {
    glDriver.ShareOpenGLContext(a,b);
  }

  POST_FUNCTION_RET(wglShareLists,retValue)

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglSwapBuffers)
BOOL WGLAPIENTRY wglSwapBuffers (HDC a)
{
  INIT_DRIVER

  PRE_FUNCTION(wglSwapBuffers,(a),a);
  BOOL retValue = GLW_Builtin.wglSwapBuffers(a);
  POST_FUNCTION_RET(wglSwapBuffers,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglSwapLayerBuffers)
BOOL WGLAPIENTRY wglSwapLayerBuffers (HDC a, UINT b)
{
  INIT_DRIVER

  PRE_FUNCTION(wglSwapLayerBuffers,(a,b),a);
  BOOL retValue = GLW_Builtin.wglSwapLayerBuffers(a,b);
  POST_FUNCTION_RET(wglSwapLayerBuffers,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglSwapMultipleBuffers)
DWORD WGLAPIENTRY wglSwapMultipleBuffers (UINT a, const WGLSWAP * b)
{
  INIT_DRIVER

  PRE_FUNCTION(wglSwapMultipleBuffers,(a,b),a);
  DWORD retValue = GLW_Builtin.wglSwapMultipleBuffers(a,b);
  POST_FUNCTION_RET(wglSwapMultipleBuffers,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglUseFontBitmapsA)
BOOL WGLAPIENTRY wglUseFontBitmapsA (HDC a, DWORD b, DWORD c, DWORD d)
{
  INIT_DRIVER

  PRE_FUNCTION(wglUseFontBitmapsA,(a,b,c,d),a);
  BOOL retValue = GLW_Builtin.wglUseFontBitmapsA(a,b,c,d);
  POST_FUNCTION_RET(wglUseFontBitmapsA,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglUseFontBitmapsW)
BOOL WGLAPIENTRY wglUseFontBitmapsW (HDC a, DWORD b, DWORD c, DWORD d)
{
  INIT_DRIVER

  PRE_FUNCTION(wglUseFontBitmapsW,(a,b,c,d),a);
  BOOL retValue = GLW_Builtin.wglUseFontBitmapsW(a,b,c,d);
  POST_FUNCTION_RET(wglUseFontBitmapsW,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglUseFontOutlinesA)
BOOL WGLAPIENTRY wglUseFontOutlinesA (HDC a, DWORD b, DWORD c, DWORD d, FLOAT e, FLOAT f, int g, LPGLYPHMETRICSFLOAT h)
{
  INIT_DRIVER

  PRE_FUNCTION(wglUseFontOutlinesA,(a,b,c,d,e,f,g,h),a);
  BOOL retValue = GLW_Builtin.wglUseFontOutlinesA(a,b,c,d,e,f,g,h);
  POST_FUNCTION_RET(wglUseFontOutlinesA,retValue)
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(wglUseFontOutlinesW)
BOOL WGLAPIENTRY wglUseFontOutlinesW (HDC a, DWORD b, DWORD c, DWORD d, FLOAT e, FLOAT f, int g, LPGLYPHMETRICSFLOAT h)
{
  INIT_DRIVER

  PRE_FUNCTION(wglUseFontOutlinesW,(a,b,c,d,e,f,g,h),a);
  BOOL retValue = GLW_Builtin.wglUseFontOutlinesW(a,b,c,d,e,f,g,h);
  POST_FUNCTION_RET(wglUseFontOutlinesW,retValue)

  return retValue;
}

#endif //GLI_BUILD_WINDOWS


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// GLX - Linux 
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef GLI_BUILD_LINUX

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXCreateContext)
GLXContext glXCreateContext (Display *dpy, XVisualInfo *vis, GLXContext share_list, Bool direct)
{
  INIT_DRIVER

  PRE_FUNCTION(glXCreateContext, (dpy, vis, share_list, direct), dpy);
  GLXContext retValue = GLW_Builtin.glXCreateContext(dpy, vis, share_list, direct);
  POST_FUNCTION_RET(glXCreateContext, retValue)
  
  //Create our driver context
  if(retValue != NULL)
  {
    glDriver.CreateOpenGLContext(retValue);
    
    //If there is a context to be shared, share it
    if(share_list != NULL)
    {
      glDriver.ShareOpenGLContext(share_list, retValue);
    }
  }
 
  return retValue;
}


///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXCreateNewContext)
GLXContext glXCreateNewContext (Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct)
{
  INIT_DRIVER

  PRE_FUNCTION(glXCreateNewContext, (dpy, config, render_type, share_list, direct), dpy);
  GLXContext retValue = GLW_Builtin.glXCreateNewContext (dpy, config, render_type, share_list, direct);
  POST_FUNCTION_RET(glXCreateNewContext, retValue);

  //Create our driver context
  if(retValue != NULL)
  {
    glDriver.CreateOpenGLContext(retValue);
    
    //If there is a context to be shared, share it
    if(share_list != NULL)
    {
      glDriver.ShareOpenGLContext(share_list, retValue);
    }
  }

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXDestroyContext)
void glXDestroyContext (Display *dpy, GLXContext ctx)
{
  INIT_DRIVER

  PRE_FUNCTION(glXDestroyContext, (dpy, ctx), dpy);
  GLW_Builtin.glXDestroyContext (dpy, ctx);
  
  //Delete the context
  glDriver.DeleteOpenGLContext(ctx);
  
  POST_FUNCTION(glXDestroyContext);
}


///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXMakeCurrent)
Bool glXMakeCurrent (Display *dpy, GLXDrawable drawable, GLXContext ctx)
{
  INIT_DRIVER

  //Get the current context
  GLXContext oldContext = GLW_Builtin.glXGetCurrentContext();

  PRE_FUNCTION(glXMakeCurrent,(dpy, drawable, ctx), dpy);
  
  //NULL out the internal context
  glDriver.SetOpenGLContext(NULL);
  
  Bool retValue = GLW_Builtin.glXMakeCurrent(dpy, drawable, ctx);

  //Call our driver to make the context valid
  if(retValue)
  {
    glDriver.SetOpenGLContext(ctx);
  }
  else
  {
    //Re-init the old context if the swap was not successful
    glDriver.SetOpenGLContext(oldContext);
  }

  POST_FUNCTION_RET(glXMakeCurrent, retValue);
  
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXMakeContextCurrent)
Bool glXMakeContextCurrent (Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx)
{
  INIT_DRIVER

  //Get the current context
  GLXContext oldContext = GLW_Builtin.glXGetCurrentContext();

  PRE_FUNCTION(glXMakeContextCurrent,(display, draw, read, ctx), display);
  
  //NULL out the internal context
  glDriver.SetOpenGLContext(NULL);
  
  Bool retValue = GLW_Builtin.glXMakeContextCurrent (display, draw, read, ctx);
  
  //Call our driver to make the context valid
  if(retValue)
  {
    glDriver.SetOpenGLContext(ctx);
  }
  else
  {
    //Re-init the old context if the swap was not successful
    glDriver.SetOpenGLContext(oldContext);
  }

  POST_FUNCTION_RET(glXMakeContextCurrent, retValue);

  //Check the read and write drawable values. (May fix later in GLIntercept)
  if(draw != read)
  {
    LOGERR(("glXMakeContextCurrent - Warning: GLIntercept may not function correctly if read and write drawables are not the same"));
  }

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetProcAddressARB)
void * glXGetProcAddressARB (const GLubyte *procname)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetProcAddressARB, (procname), procname);
  void * retValue = GLW_Builtin.glXGetProcAddressARB (procname);
  
  //Register the new function (NOTE: "NULL" functions can be overridden)
  retValue = glDriver.AddExtensionFunction((char*)procname, retValue);
  
  POST_FUNCTION_RET(glXGetProcAddressARB, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetProcAddress)
void * glXGetProcAddress (const GLubyte *procname)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetProcAddress, (procname), procname);
  void * retValue = GLW_Builtin.glXGetProcAddress (procname);
  
  //Register the new function (NOTE: "NULL" functions can be overridden)
  retValue = glDriver.AddExtensionFunction((char*)procname, retValue);
  
  POST_FUNCTION_RET(glXGetProcAddress, retValue);

  return retValue;
}

//hide dlsym call so that application requesting ogl funcitons
//through this call can still be logged successfully
//this is required for SDL based games (glX functions are requested
//through dlsym directly from libraries)
#include <dlfcn.h>

extern "C" void * __libc_dlsym(void* handle, const char* procname);
void* dlsym (void* handle, const char* procname)
{
	void* retValue;
  //if its gl function, mimic behavior of glXGetProcAddress, but dont log dlsym
  //otherwise behave like dlsym usually does
  if(procname &&
     strlen(procname) > 2 &&
     procname[0] == 'g' &&
     procname[1] == 'l' )
  {
    INIT_DRIVER
    retValue = GLW_Builtin.glXGetProcAddress ((const GLubyte*)procname);
    retValue = glDriver.AddExtensionFunction(procname, retValue);
  }
  else
  {
    retValue = __libc_dlsym(handle, procname);
  }
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
// Standard Linux GLX methods
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXChooseVisual)
XVisualInfo* glXChooseVisual (Display *dpy, int screen, int *attribList)
{
  INIT_DRIVER

  PRE_FUNCTION(glXChooseVisual,(dpy,screen,attribList),dpy);
  XVisualInfo *retValue = GLW_Builtin.glXChooseVisual(dpy,screen,attribList);
  POST_FUNCTION_RET(glXChooseVisual, retValue);
  
  return retValue;  
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXCopyContext)
void glXCopyContext (Display *dpy, GLXContext src, GLXContext dst, unsigned long mask)
{
  INIT_DRIVER

  PRE_FUNCTION(glXCopyContext,(dpy,src,dst,mask),dpy);
  GLW_Builtin.glXCopyContext(dpy,src,dst,mask);
  POST_FUNCTION(glXCopyContext);
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXCreateGLXPixmap)
GLXPixmap glXCreateGLXPixmap (Display *dpy, XVisualInfo *vis, Pixmap pixmap)
{
  INIT_DRIVER

  PRE_FUNCTION(glXCreateGLXPixmap, (dpy, vis, pixmap), dpy);
  GLXPixmap retValue = GLW_Builtin.glXCreateGLXPixmap(dpy, vis, pixmap);
  POST_FUNCTION_RET(glXCreateGLXPixmap, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXDestroyGLXPixmap)
void glXDestroyGLXPixmap (Display *dpy, GLXPixmap pix)
{
  INIT_DRIVER

  PRE_FUNCTION(glXDestroyGLXPixmap, (dpy, pix), dpy);
  GLW_Builtin.glXDestroyGLXPixmap(dpy, pix);
  POST_FUNCTION(glXDestroyGLXPixmap);
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetConfig)
int glXGetConfig (Display *dpy, XVisualInfo *vis, int attrib, int *value)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetConfig, (dpy, vis, attrib, value), dpy);
  int retValue = GLW_Builtin.glXGetConfig(dpy, vis, attrib, value);
  POST_FUNCTION_RET(glXGetConfig, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetCurrentContext)
GLXContext glXGetCurrentContext (void)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetCurrentContext, (), wglDummyValue);
  GLXContext retValue = GLW_Builtin.glXGetCurrentContext();
  POST_FUNCTION_RET(glXGetCurrentContext, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetCurrentDrawable)
GLXDrawable glXGetCurrentDrawable (void)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetCurrentDrawable, (), wglDummyValue);
  GLXDrawable retValue = GLW_Builtin.glXGetCurrentDrawable();
  POST_FUNCTION_RET(glXGetCurrentDrawable, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXIsDirect)
Bool glXIsDirect (Display *dpy, GLXContext ctx)
{
  INIT_DRIVER

  PRE_FUNCTION(glXIsDirect, (dpy, ctx), dpy);
  Bool retValue = GLW_Builtin.glXIsDirect (dpy, ctx);
  POST_FUNCTION_RET(glXIsDirect, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXQueryExtension)
Bool glXQueryExtension (Display *dpy, int *errorBase, int *eventBase)
{
  INIT_DRIVER

  PRE_FUNCTION(glXQueryExtension, (dpy, errorBase, eventBase), dpy);
  Bool retValue = GLW_Builtin.glXQueryExtension (dpy, errorBase, eventBase);
  POST_FUNCTION_RET(glXQueryExtension, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXQueryExtensionsString)
const char *glXQueryExtensionsString (Display *dpy, int screen )
{
  INIT_DRIVER

  PRE_FUNCTION(glXQueryExtensionsString, (dpy, screen), dpy);
  const char *retValue = GLW_Builtin.glXQueryExtensionsString (dpy, screen);
  POST_FUNCTION_RET(glXQueryExtensionsString, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXQueryVersion)
Bool glXQueryVersion (Display *dpy, int *major, int *minor)
{
  INIT_DRIVER

  PRE_FUNCTION(glXQueryVersion, (dpy, major, minor), dpy);
  Bool retValue = GLW_Builtin.glXQueryVersion (dpy, major, minor);
  POST_FUNCTION_RET(glXQueryVersion, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXSwapBuffers)
void glXSwapBuffers (Display *dpy, GLXDrawable drawable)
{
  INIT_DRIVER

  PRE_FUNCTION(glXSwapBuffers, (dpy, drawable), dpy);
  GLW_Builtin.glXSwapBuffers(dpy, drawable);
  POST_FUNCTION(glXSwapBuffers);
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXUseXFont)
void glXUseXFont (Font font, int first, int count, int listBase)
{
  INIT_DRIVER

  PRE_FUNCTION(glXUseXFont, (font, first, count, listBase), font);
  GLW_Builtin.glXUseXFont(font, first, count, listBase);
  POST_FUNCTION(glXUseXFont);
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXWaitGL)
void glXWaitGL (void)
{
  INIT_DRIVER

  PRE_FUNCTION(glXWaitGL, (), wglDummyValue);
  GLW_Builtin.glXWaitGL();
  POST_FUNCTION(glXWaitGL);
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXWaitX)
void glXWaitX (void)
{
  INIT_DRIVER

  PRE_FUNCTION(glXWaitX, (), wglDummyValue);
  GLW_Builtin.glXWaitX();
  POST_FUNCTION(glXWaitX);
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetClientString)
const char *glXGetClientString (Display *dpy, int name )
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetClientString, (dpy, name), dpy);
  const char *retValue = GLW_Builtin.glXGetClientString (dpy, name);
  POST_FUNCTION_RET(glXGetClientString, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXQueryServerString)
const char *glXQueryServerString (Display *dpy, int screen, int name )
{
  INIT_DRIVER

  PRE_FUNCTION(glXQueryServerString, (dpy, screen, name), dpy);
  const char * retValue = GLW_Builtin.glXQueryServerString (dpy, screen, name);
  POST_FUNCTION_RET(glXQueryServerString, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetFBConfigs)
GLXFBConfig* glXGetFBConfigs (Display *dpy, int screen, int *nelements)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetFBConfigs, (dpy, screen, nelements), dpy);
  GLXFBConfig *retValue = GLW_Builtin.glXGetFBConfigs (dpy, screen, nelements);
  POST_FUNCTION_RET(glXGetFBConfigs, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXChooseFBConfig)
GLXFBConfig* glXChooseFBConfig (Display *dpy, int screen, const int *attrib_list, int *nelements)
{
  INIT_DRIVER

  PRE_FUNCTION(glXChooseFBConfig, (dpy, screen, attrib_list, nelements), dpy);
  GLXFBConfig * retValue = GLW_Builtin.glXChooseFBConfig (dpy, screen, attrib_list, nelements);
  POST_FUNCTION_RET(glXChooseFBConfig, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetFBConfigAttrib)
int glXGetFBConfigAttrib (Display *dpy, GLXFBConfig config, int attribute, int *value)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetFBConfigAttrib, (dpy, config, attribute, value), dpy);
  int retValue = GLW_Builtin.glXGetFBConfigAttrib (dpy, config, attribute, value);
  POST_FUNCTION_RET(glXGetFBConfigAttrib, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetVisualFromFBConfig)
XVisualInfo* glXGetVisualFromFBConfig(Display *dpy, GLXFBConfig config)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetVisualFromFBConfig, (dpy, config), dpy);
  XVisualInfo * retValue = GLW_Builtin.glXGetVisualFromFBConfig(dpy, config);
  POST_FUNCTION_RET(glXGetVisualFromFBConfig, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXCreateWindow)
GLXWindow glXCreateWindow (Display *dpy, GLXFBConfig config, Window win, const int *attrib_list)
{
  INIT_DRIVER

  PRE_FUNCTION(glXCreateWindow, (dpy, config, win, attrib_list), dpy);
  GLXWindow retValue = GLW_Builtin.glXCreateWindow (dpy, config, win, attrib_list);
  POST_FUNCTION_RET(glXCreateWindow, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXDestroyWindow)
void glXDestroyWindow (Display *dpy, GLXWindow win)
{
  INIT_DRIVER

  PRE_FUNCTION(glXDestroyWindow, (dpy, win), dpy);
  GLW_Builtin.glXDestroyWindow (dpy, win);
  POST_FUNCTION(glXDestroyWindow);
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXCreatePixmap)
GLXPixmap glXCreatePixmap (Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list)
{
  INIT_DRIVER

  PRE_FUNCTION(glXCreatePixmap, (dpy, config, pixmap, attrib_list), dpy);
  GLXPixmap retValue = GLW_Builtin.glXCreatePixmap (dpy, config, pixmap, attrib_list);
  POST_FUNCTION_RET(glXCreatePixmap, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXDestroyPixmap)
void glXDestroyPixmap (Display *dpy, GLXPixmap pixmap)
{
  INIT_DRIVER

  PRE_FUNCTION(glXDestroyPixmap, (dpy, pixmap), dpy);
  GLW_Builtin.glXDestroyPixmap (dpy, pixmap);
  POST_FUNCTION(glXDestroyPixmap);
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXCreatePbuffer)
GLXPbuffer glXCreatePbuffer (Display *dpy, GLXFBConfig config, const int *attrib_list)
{
  INIT_DRIVER

  PRE_FUNCTION(glXCreatePbuffer, (dpy, config, attrib_list), dpy);
  GLXPbuffer retValue = GLW_Builtin.glXCreatePbuffer (dpy, config, attrib_list);
  POST_FUNCTION_RET(glXCreatePbuffer, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXDestroyPbuffer)
void glXDestroyPbuffer (Display *dpy, GLXPbuffer pbuf)
{
  INIT_DRIVER

  PRE_FUNCTION(glXDestroyPbuffer, (dpy, pbuf), dpy);
  GLW_Builtin.glXDestroyPbuffer (dpy, pbuf);
  POST_FUNCTION(glXDestroyPbuffer);
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXQueryDrawable)
void glXQueryDrawable (Display *dpy, GLXDrawable draw, int attribute, unsigned int *value)
{
  INIT_DRIVER

  PRE_FUNCTION(glXQueryDrawable, (dpy, draw, attribute, value), dpy);
  GLW_Builtin.glXQueryDrawable (dpy, draw, attribute, value);
  POST_FUNCTION(glXQueryDrawable);
}


///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetCurrentReadDrawable)
GLXDrawable glXGetCurrentReadDrawable(void)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetCurrentReadDrawable, (), wglDummyValue);
  GLXDrawable retValue = GLW_Builtin.glXGetCurrentReadDrawable();
  POST_FUNCTION_RET(glXGetCurrentReadDrawable, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetCurrentDisplay)
Display * glXGetCurrentDisplay (void)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetCurrentDisplay, (), wglDummyValue);
  Display * retValue = GLW_Builtin.glXGetCurrentDisplay();
  POST_FUNCTION_RET(glXGetCurrentDisplay, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXQueryContext)
int glXQueryContext (Display *dpy, GLXContext ctx, int attribute, int *value)
{
  INIT_DRIVER

  PRE_FUNCTION(glXQueryContext, (dpy, ctx, attribute, value), dpy);
  int retValue = GLW_Builtin.glXQueryContext (dpy, ctx, attribute, value);
  POST_FUNCTION_RET(glXQueryContext, retValue);

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXSelectEvent)
void glXSelectEvent (Display *dpy, GLXDrawable draw, unsigned long event_mask)
{
  INIT_DRIVER

  PRE_FUNCTION(glXSelectEvent, (dpy, draw, event_mask), dpy);
  GLW_Builtin.glXSelectEvent (dpy, draw, event_mask);
  POST_FUNCTION(glXSelectEvent);
}

///////////////////////////////////////////////////////////////////////////////
//
WGL_BUILTIN_WRAPPER(glXGetSelectedEvent)
void glXGetSelectedEvent (Display *dpy, GLXDrawable draw, unsigned long *event_mask)
{
  INIT_DRIVER

  PRE_FUNCTION(glXGetSelectedEvent, (dpy, draw, event_mask), dpy);
  GLW_Builtin.glXGetSelectedEvent (dpy, draw, event_mask);
  POST_FUNCTION(glXGetSelectedEvent);
}

#undef INIT_DRIVER

#endif //GLI_BUILD_LINUX




