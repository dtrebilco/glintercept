#define GLI_INCLUDE_XGL_EXT_IMPORT_CONTEXT

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


void *  glXGetCurrentDisplayEXT(void);
GLint   glXQueryContextInfoEXT (void *dpy, void * context,GLint attribute,GLint *value);
GLuint  glXGetContextIDEXT     (const void * context);
void *  glXImportContextEXT    (void *dpy, GLuint contextID);
void    glXFreeContextEXT      (void *dpy, void * context);



