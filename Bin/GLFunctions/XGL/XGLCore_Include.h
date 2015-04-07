#define GLI_INCLUDE_XGLCORE

//Attempt to map X-Windows call parameters to OpenGL parameter types

// Update for 64 bit.
// This assumes:
//  GLXContext    = pointer
//  GLXFBConfig   = pointer
//  unsigned long = GLuint
//  Bool          = GLint
//  GLXPixmap     = GLuint
//  GLXDrawable   = GLuint
//  GLXWindow     = GLuint
//  GLXPbuffer    = GLuint
//  GLXContextID  = GLuint
//  Pixmap        = GLuint
//  Font          = GLuint
//  Window        = GLuint

//Core GLX 1.2
void *       glXChooseVisual          (void *dpy, GLint screen, GLint *attribList);
void         glXCopyContext           (void *dpy, void* src, void* dst, /*unsigned long*/GLuint mask); //64 bit fixup
void *       glXCreateContext         (void *dpy, void *vis, void* shareList, GLint direct);
GLuint       glXCreateGLXPixmap       (void *dpy, void *vis, GLuint pixmap);
void         glXDestroyContext        (void *dpy, void * ctx);
void         glXDestroyGLXPixmap      (void *dpy, GLuint pix);
GLint        glXGetConfig             (void *dpy, void *vis, GLint attrib, GLint *value);
void *       glXGetCurrentContext     (void);
GLuint       glXGetCurrentDrawable    (void);
GLint        glXIsDirect              (void *dpy, void * ctx);
GLint        glXMakeCurrent           (void *dpy, GLuint drawable, void * ctx);
GLint        glXQueryExtension        (void *dpy, GLint *errorBase, GLint *eventBase);
GLint        glXQueryVersion          (void *dpy, GLint *major, GLint *minor);
void         glXSwapBuffers           (void *dpy, GLuint drawable);
void         glXUseXFont              (GLuint font, GLint first, GLint count, GLint listBase);
void         glXWaitGL                (void);
void         glXWaitX                 (void);

const GLasciistring * glXGetClientString       (void *dpy, GLint name );
const GLasciistring * glXQueryServerString     (void *dpy, GLint screen, GLint name );
const GLasciistring * glXQueryExtensionsString (void *dpy, GLint screen );

//GLX 1.3
void *       glXGetFBConfigs          (void *dpy, GLint screen, GLint *nelements);
void *       glXChooseFBConfig        (void *dpy, GLint screen, const GLint *attrib_list, GLint *nelements);
GLint        glXGetFBConfigAttrib     (void *dpy, void* config, GLint attribute, GLint *value);
void *       glXGetVisualFromFBConfig (void *dpy, void* config);
GLuint       glXCreateWindow          (void *dpy, void* config, GLuint win, const GLint *attrib_list);
void         glXDestroyWindow         (void *dpy, GLuint win);
GLuint       glXCreatePixmap          (void *dpy, void* config, GLuint pixmap, const GLint *attrib_list);
void         glXDestroyPixmap         (void *dpy, GLuint pixmap);
GLuint       glXCreatePbuffer         (void *dpy, void* config, const GLint *attrib_list);
void         glXDestroyPbuffer        (void *dpy, GLuint pbuf);
void         glXQueryDrawable         (void *dpy, GLuint draw, GLint attribute, GLuint *value);
void *       glXCreateNewContext      (void *dpy, void* config, GLint render_type, void * share_list, GLint direct);
GLint        glXMakeContextCurrent    (void *display, GLuint draw, GLuint read, void * ctx);
GLuint       glXGetCurrentReadDrawable(void);
void *       glXGetCurrentDisplay     (void);
GLint        glXQueryContext          (void *dpy, void * ctx, GLint attribute, GLint *value);
void         glXSelectEvent           (void *dpy, GLuint draw, /*unsigned long*/GLuint event_mask); //64 bit fixup
void         glXGetSelectedEvent      (void *dpy, GLuint draw, /*unsigned long*/GLuint *event_mask);

//Required ARB extension
void *       glXGetProcAddressARB     (const GLasciistring *procname);

//GLX 1.4
void *       glXGetProcAddress        (const GLasciistring *procname);


