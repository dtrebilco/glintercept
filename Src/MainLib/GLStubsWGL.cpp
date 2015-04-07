/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "GLInterceptConfig.h"
#include "GLDriver.h"
#include "GLWindows.h"

USING_ERRORLOG

//The real OpenGL windows calls
extern WGLDriver    GLW;

//The version of the OpenGL functions that the builtin wrappers call
// (Functions in this table may be over-ridden/replaced)
extern WGLDriver    GLW_Builtin;

//The function table of all known functions
extern FunctionTable * functionTable;


#ifdef GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//
bool MapWGLFunctions(DLLLoader &openGLLib)
{
  void **tempfunc;

  //Test if the function table has been init
  if(!functionTable)
  {
    GLDriver::InitFunctionTable();
  }

#define GLW_FUNC_LOOKUP(string)                 \
  tempfunc = (void**)(&GLW.string);             \
  (*tempfunc) = openGLLib.GetFunction(#string); \
  if(GLW.string == NULL)                        \
  {LOGERR(("Function %s unable to be mapped",#string)) return false;} \
  functionTable->UpdateBuiltinFunction(#string, (void*)GLW.string);

  GLW_FUNC_LOOKUP(wglChoosePixelFormat)
  GLW_FUNC_LOOKUP(wglCopyContext)
  GLW_FUNC_LOOKUP(wglCreateContext)
  GLW_FUNC_LOOKUP(wglCreateLayerContext)
  GLW_FUNC_LOOKUP(wglDeleteContext)
  GLW_FUNC_LOOKUP(wglDescribeLayerPlane)
  GLW_FUNC_LOOKUP(wglDescribePixelFormat)
  GLW_FUNC_LOOKUP(wglGetCurrentContext)
  GLW_FUNC_LOOKUP(wglGetCurrentDC)
  GLW_FUNC_LOOKUP(wglGetDefaultProcAddress)
  GLW_FUNC_LOOKUP(wglGetLayerPaletteEntries)
  GLW_FUNC_LOOKUP(wglGetPixelFormat)
  GLW_FUNC_LOOKUP(wglGetProcAddress)
  GLW_FUNC_LOOKUP(wglMakeCurrent)
  GLW_FUNC_LOOKUP(wglRealizeLayerPalette)
  GLW_FUNC_LOOKUP(wglSetLayerPaletteEntries)
  GLW_FUNC_LOOKUP(wglSetPixelFormat)
  GLW_FUNC_LOOKUP(wglShareLists)
  GLW_FUNC_LOOKUP(wglSwapBuffers)
  GLW_FUNC_LOOKUP(wglSwapLayerBuffers)
  GLW_FUNC_LOOKUP(wglSwapMultipleBuffers)
  GLW_FUNC_LOOKUP(wglUseFontBitmapsA)
  GLW_FUNC_LOOKUP(wglUseFontBitmapsW)
  GLW_FUNC_LOOKUP(wglUseFontOutlinesA)
  GLW_FUNC_LOOKUP(wglUseFontOutlinesW)

  //Manually assign the generic extension entry point
  GLW.glGetProcAddress  = GLW.wglGetProcAddress;

#undef GLW_FUNC_LOOKUP

  //Assign the structure of functions that the built-in wrappers call
  GLW_Builtin = GLW;

  return true;
}



#define DUMMY_LOG(name) LOGERR(("Call to %s made outside of context/init",#name));

///////////////////////////////////////////////////////////////////////////////
//
int WGLAPIENTRY DummywglChoosePixelFormat (HDC a, CONST PIXELFORMATDESCRIPTOR *b)
{
  DUMMY_LOG(wglChoosePixelFormat);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglCopyContext (HGLRC a, HGLRC b, UINT c)
{
  DUMMY_LOG(wglCopyContext);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
HGLRC WGLAPIENTRY DummywglCreateContext (HDC a)
{
  DUMMY_LOG(wglCreateContext);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
HGLRC WGLAPIENTRY DummywglCreateLayerContext (HDC a, int b)
{
  DUMMY_LOG(wglCreateLayerContext);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglDeleteContext (HGLRC a)
{
  DUMMY_LOG(wglDeleteContext);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglDescribeLayerPlane (HDC a, int b, int c, UINT d, LPLAYERPLANEDESCRIPTOR e)
{
  DUMMY_LOG(wglDescribeLayerPlane);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
int WGLAPIENTRY DummywglDescribePixelFormat (HDC a, int b, UINT c, LPPIXELFORMATDESCRIPTOR d)
{
  DUMMY_LOG(wglDescribePixelFormat);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
HGLRC WGLAPIENTRY DummywglGetCurrentContext (void)
{
  DUMMY_LOG(wglGetCurrentContext);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
HDC WGLAPIENTRY DummywglGetCurrentDC (void)
{
  DUMMY_LOG(wglGetCurrentDC);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
PROC WGLAPIENTRY DummywglGetDefaultProcAddress (LPCSTR a)
{
  DUMMY_LOG(wglGetDefaultProcAddress);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
int WGLAPIENTRY DummywglGetLayerPaletteEntries (HDC a, int b, int c, int d, COLORREF *e)
{
  DUMMY_LOG(wglGetLayerPaletteEntries);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
int WGLAPIENTRY DummywglGetPixelFormat (HDC a)
{
  DUMMY_LOG(wglGetPixelFormat);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
PROC WGLAPIENTRY DummywglGetProcAddress (LPCSTR a)
{
  DUMMY_LOG(wglGetProcAddress);
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglMakeCurrent (HDC a, HGLRC b)
{
  DUMMY_LOG(wglMakeCurrent);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglRealizeLayerPalette (HDC a, int b, BOOL c)
{
  DUMMY_LOG(wglRealizeLayerPalette);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
int WGLAPIENTRY DummywglSetLayerPaletteEntries (HDC a, int b, int c, int d, CONST COLORREF *e)
{
  DUMMY_LOG(wglSetLayerPaletteEntries);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglSetPixelFormat (HDC a, int b, CONST PIXELFORMATDESCRIPTOR *c)
{
  DUMMY_LOG(wglSetPixelFormat);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglShareLists (HGLRC a, HGLRC b)
{
  DUMMY_LOG(wglShareLists);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglSwapBuffers (HDC a)
{
  DUMMY_LOG(wglSwapBuffers);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglSwapLayerBuffers (HDC a, UINT b)
{
  DUMMY_LOG(wglSwapLayerBuffers);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
long WGLAPIENTRY DummywglSwapMultipleBuffers(UINT a, const WGLSWAP * b)
{
  DUMMY_LOG(wglSwapMultipleBuffers);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglUseFontBitmapsA (HDC a, DWORD b, DWORD c, DWORD d)
{
  DUMMY_LOG(wglUseFontBitmapsA);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglUseFontBitmapsW (HDC a, DWORD b, DWORD c, DWORD d)
{
  DUMMY_LOG(wglUseFontBitmapsW);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglUseFontOutlinesA (HDC a, DWORD b, DWORD c, DWORD d, FLOAT e, FLOAT f, int g, LPGLYPHMETRICSFLOAT h)
{
  DUMMY_LOG(wglUseFontOutlinesA);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL WGLAPIENTRY DummywglUseFontOutlinesW (HDC a, DWORD b, DWORD c, DWORD d, FLOAT e, FLOAT f, int g, LPGLYPHMETRICSFLOAT h)
{
  DUMMY_LOG(wglUseFontOutlinesW);
  return 0;
}

//Init the function table with stubs
WGLDriver GLW=
{
  DummywglChoosePixelFormat,
  DummywglCopyContext,
  DummywglCreateContext,
  DummywglCreateLayerContext,
  DummywglDeleteContext,
  DummywglDescribeLayerPlane,
  DummywglDescribePixelFormat,
  DummywglGetCurrentContext,
  DummywglGetCurrentDC,
  DummywglGetDefaultProcAddress,
  DummywglGetLayerPaletteEntries,
  DummywglGetPixelFormat,
  DummywglGetProcAddress,
  DummywglMakeCurrent,
  DummywglRealizeLayerPalette,
  DummywglSetLayerPaletteEntries,
  DummywglSetPixelFormat,
  DummywglShareLists,
  DummywglSwapBuffers,
  DummywglSwapLayerBuffers,
  DummywglSwapMultipleBuffers,
  DummywglUseFontBitmapsA,
  DummywglUseFontBitmapsW,
  DummywglUseFontOutlinesA,
  DummywglUseFontOutlinesW,
  
  //Independant glGetProcAddress
  DummywglGetProcAddress,
 
};

//The version of the GL windows functions that he built-in functions call
WGLDriver GLW_Builtin = GLW;


#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

///////////////////////////////////////////////////////////////////////////////
//
bool MapWGLFunctions(DLLLoader &openGLLib)
{
  void **tempfunc;

  //Test if the function table has been init
  if(!functionTable)
  {
    GLDriver::InitFunctionTable();
  }

#define GLW_FUNC_LOOKUP(string)                 \
  tempfunc = (void**)(&GLW.string);             \
  (*tempfunc) = openGLLib.GetFunction(#string); \
  if(GLW.string == NULL)                        \
  {LOGERR(("Function %s unable to be mapped",#string)) return false;} \
  functionTable->UpdateBuiltinFunction(#string, (void*)GLW.string);

  GLW_FUNC_LOOKUP(glXChooseVisual)
  GLW_FUNC_LOOKUP(glXCopyContext)
  GLW_FUNC_LOOKUP(glXCreateContext)
  GLW_FUNC_LOOKUP(glXCreateGLXPixmap)
  GLW_FUNC_LOOKUP(glXDestroyContext)
  GLW_FUNC_LOOKUP(glXDestroyGLXPixmap)
  GLW_FUNC_LOOKUP(glXGetConfig)
  GLW_FUNC_LOOKUP(glXGetCurrentContext)
  GLW_FUNC_LOOKUP(glXGetCurrentDrawable)
  GLW_FUNC_LOOKUP(glXIsDirect)
  GLW_FUNC_LOOKUP(glXMakeCurrent)
  GLW_FUNC_LOOKUP(glXQueryExtension)
  GLW_FUNC_LOOKUP(glXQueryVersion)
  GLW_FUNC_LOOKUP(glXSwapBuffers)
  GLW_FUNC_LOOKUP(glXUseXFont)
  GLW_FUNC_LOOKUP(glXWaitGL)
  GLW_FUNC_LOOKUP(glXWaitX)
  GLW_FUNC_LOOKUP(glXGetClientString)
  GLW_FUNC_LOOKUP(glXQueryServerString)
  GLW_FUNC_LOOKUP(glXQueryExtensionsString)

  GLW_FUNC_LOOKUP(glXGetFBConfigs)
  GLW_FUNC_LOOKUP(glXChooseFBConfig)
  GLW_FUNC_LOOKUP(glXGetFBConfigAttrib)
  GLW_FUNC_LOOKUP(glXGetVisualFromFBConfig)
  GLW_FUNC_LOOKUP(glXCreateWindow)
  GLW_FUNC_LOOKUP(glXDestroyWindow)
  GLW_FUNC_LOOKUP(glXCreatePixmap)
  GLW_FUNC_LOOKUP(glXDestroyPixmap)
  GLW_FUNC_LOOKUP(glXCreatePbuffer)
  GLW_FUNC_LOOKUP(glXDestroyPbuffer)
  GLW_FUNC_LOOKUP(glXQueryDrawable)
  GLW_FUNC_LOOKUP(glXCreateNewContext)
  GLW_FUNC_LOOKUP(glXMakeContextCurrent)
  GLW_FUNC_LOOKUP(glXGetCurrentReadDrawable)
  GLW_FUNC_LOOKUP(glXGetCurrentDisplay)
  GLW_FUNC_LOOKUP(glXQueryContext)
  GLW_FUNC_LOOKUP(glXSelectEvent)
  GLW_FUNC_LOOKUP(glXGetSelectedEvent)

  GLW_FUNC_LOOKUP(glXGetProcAddressARB)

  //Manually assign the GLX1.4 entry point
  GLW.glXGetProcAddress = GLW.glXGetProcAddressARB;

  //Manually assign the generic extension entry point 
  // (C-cast exists as GLX takes unsigned chars)
  GLW.glGetProcAddress  = (void*(*)(const char*))GLW.glXGetProcAddressARB;  

#undef GLW_FUNC_LOOKUP

  //Assign the structure of functions that the built-in wrappers call
  GLW_Builtin = GLW;

  return true;
}


#define DUMMY_LOG(name) LOGERR(("Call to %s made outside of context/init",#name));

///////////////////////////////////////////////////////////////////////////////
//
XVisualInfo* DummyglXChooseVisual (Display *dpy, int screen, int *attribList)
{
  DUMMY_LOG(glXChooseVisual);
  return 0;
}
///////////////////////////////////////////////////////////////////////////////
//
void DummyglXCopyContext (Display *dpy, GLXContext src, GLXContext dst, unsigned long mask)
{
  DUMMY_LOG(glXCopyContext);
}
///////////////////////////////////////////////////////////////////////////////
//
GLXContext DummyglXCreateContext (Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct)
{
  DUMMY_LOG(glXCreateContext);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
GLXPixmap DummyglXCreateGLXPixmap (Display *dpy, XVisualInfo *vis, Pixmap pixmap)
{
  DUMMY_LOG(glXCreateGLXPixmap);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXDestroyContext (Display *dpy, GLXContext ctx)
{
  DUMMY_LOG(glXDestroyContext);
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXDestroyGLXPixmap (Display *dpy, GLXPixmap pix)
{
  DUMMY_LOG(glXDestroyGLXPixmap);
}

///////////////////////////////////////////////////////////////////////////////
//
int DummyglXGetConfig (Display *dpy, XVisualInfo *vis, int attrib, int *value)
{
  DUMMY_LOG(glXGetConfig);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
GLXContext DummyglXGetCurrentContext (void)
{
  DUMMY_LOG(glXGetCurrentContext);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
GLXDrawable DummyglXGetCurrentDrawable (void)
{
  DUMMY_LOG(glXGetCurrentDrawable);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
Bool DummyglXIsDirect (Display *dpy, GLXContext ctx)
{
  DUMMY_LOG(glXIsDirect);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
Bool DummyglXMakeCurrent (Display *dpy, GLXDrawable drawable, GLXContext ctx)
{
  DUMMY_LOG(glXMakeCurrent);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
Bool DummyglXQueryExtension (Display *dpy, int *errorBase, int *eventBase)
{
  DUMMY_LOG(glXQueryExtension);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
Bool DummyglXQueryVersion (Display *dpy, int *major, int *minor)
{
  DUMMY_LOG(glXQueryVersion);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXSwapBuffers (Display *dpy, GLXDrawable drawable)
{
  DUMMY_LOG(glXSwapBuffers);
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXUseXFont (Font font, int first, int count, int listBase)
{
  DUMMY_LOG(glXUseXFont);
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXWaitGL (void)
{
  DUMMY_LOG(glXWaitGL);
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXWaitX (void)
{
  DUMMY_LOG(glXWaitX);
}

///////////////////////////////////////////////////////////////////////////////
//
const char * DummyglXGetClientString (Display *dpy, int name )
{
  DUMMY_LOG(glXGetClientString);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
const char * DummyglXQueryServerString (Display *dpy, int screen, int name )
{
  DUMMY_LOG(glXQueryServerString);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
const char * DummyglXQueryExtensionsString (Display *dpy, int screen )
{
  DUMMY_LOG(glXQueryExtensionsString);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
GLXFBConfig* DummyglXGetFBConfigs (Display *dpy, int screen, int *nelements)
{
  DUMMY_LOG(glXGetFBConfigs);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
GLXFBConfig* DummyglXChooseFBConfig (Display *dpy, int screen, const int *attrib_list, int *nelements)
{
  DUMMY_LOG(glXChooseFBConfig);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
int DummyglXGetFBConfigAttrib (Display *dpy, GLXFBConfig config, int attribute, int *value)
{
  DUMMY_LOG(glXGetFBConfigAttrib);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
XVisualInfo* DummyglXGetVisualFromFBConfig (Display *dpy, GLXFBConfig config)
{
  DUMMY_LOG(glXGetVisualFromFBConfig);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
GLXWindow DummyglXCreateWindow (Display *dpy, GLXFBConfig config, Window win, const int *attrib_list)
{
  DUMMY_LOG(glXCreateWindow);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXDestroyWindow (Display *dpy, GLXWindow win)
{
  DUMMY_LOG(glXDestroyWindow);
}

///////////////////////////////////////////////////////////////////////////////
//
GLXPixmap DummyglXCreatePixmap (Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list)
{
  DUMMY_LOG(glXCreatePixmap);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXDestroyPixmap (Display *dpy, GLXPixmap pixmap)
{
  DUMMY_LOG(glXDestroyPixmap);
}

///////////////////////////////////////////////////////////////////////////////
//
GLXPbuffer DummyglXCreatePbuffer (Display *dpy, GLXFBConfig config, const int *attrib_list)
{
  DUMMY_LOG(glXCreatePbuffer);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXDestroyPbuffer (Display *dpy, GLXPbuffer pbuf)
{
  DUMMY_LOG(glXDestroyPbuffer);
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXQueryDrawable (Display *dpy, GLXDrawable draw, int attribute, unsigned int *value)
{
  DUMMY_LOG(glXQueryDrawable);
}

///////////////////////////////////////////////////////////////////////////////
//
GLXContext DummyglXCreateNewContext (Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct)
{
  DUMMY_LOG(glXCreateNewContext);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
Bool DummyglXMakeContextCurrent (Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx)
{
  DUMMY_LOG(glXMakeContextCurrent);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
GLXDrawable DummyglXGetCurrentReadDrawable(void)
{
  DUMMY_LOG(glXGetCurrentReadDrawable);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
Display * DummyglXGetCurrentDisplay (void)
{
  DUMMY_LOG(glXGetCurrentDisplay);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
int DummyglXQueryContext (Display *dpy, GLXContext ctx, int attribute, int *value)
{
  DUMMY_LOG(glXQueryContext);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXSelectEvent (Display *dpy, GLXDrawable draw, unsigned long event_mask)
{
  DUMMY_LOG(glXSelectEvent);
}

///////////////////////////////////////////////////////////////////////////////
//
void DummyglXGetSelectedEvent (Display *dpy, GLXDrawable draw, unsigned long *event_mask)
{
  DUMMY_LOG(glXGetSelectedEvent);
}

///////////////////////////////////////////////////////////////////////////////
//
void * DummyglXGetProcAddressARB (const GLubyte *procname)
{
  DUMMY_LOG(glXGetProcAddressARB);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void * DummyglGetProcAddressARB (const char *procname)
{
  DUMMY_LOG(glGetProcAddressARB);
  return 0;
}


//Init the function table with stubs
WGLDriver GLW=  
{
  //GLX 1.2
  DummyglXChooseVisual,
  DummyglXCopyContext,
  DummyglXCreateContext,
  DummyglXCreateGLXPixmap,
  DummyglXDestroyContext,
  DummyglXDestroyGLXPixmap,
  DummyglXGetConfig,
  DummyglXGetCurrentContext,
  DummyglXGetCurrentDrawable,
  DummyglXIsDirect,
  DummyglXMakeCurrent,
  DummyglXQueryExtension,
  DummyglXQueryVersion,
  DummyglXSwapBuffers,
  DummyglXUseXFont,
  DummyglXWaitGL,
  DummyglXWaitX,
  DummyglXGetClientString,
  DummyglXQueryServerString,
  DummyglXQueryExtensionsString,

  //GLX 1.3
  DummyglXGetFBConfigs,
  DummyglXChooseFBConfig,
  DummyglXGetFBConfigAttrib,
  DummyglXGetVisualFromFBConfig,
  DummyglXCreateWindow,
  DummyglXDestroyWindow,
  DummyglXCreatePixmap,
  DummyglXDestroyPixmap,
  DummyglXCreatePbuffer,
  DummyglXDestroyPbuffer,
  DummyglXQueryDrawable,
  DummyglXCreateNewContext,
  DummyglXMakeContextCurrent,
  DummyglXGetCurrentReadDrawable,
  DummyglXGetCurrentDisplay,
  DummyglXQueryContext,
  DummyglXSelectEvent,
  DummyglXGetSelectedEvent,

  //GLX 1.4
  DummyglXGetProcAddressARB,
  DummyglXGetProcAddressARB,

  //Independant glGetProcAddress
  DummyglGetProcAddressARB,

};

//The version of the GL windows functions that he built-in functions call
WGLDriver GLW_Builtin = GLW;


#endif //GLI_BUILD_LINUX

