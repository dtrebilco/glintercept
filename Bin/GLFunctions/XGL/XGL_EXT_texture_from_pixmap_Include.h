#define GLI_INCLUDE_XGL_EXT_TEXTURE_FROM_PIXMAP

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


void glXBindTexImageEXT (void *display, GLuint drawable, GLint buffer, GLint *attrib_list);

void glXReleaseTexImageEXT (void *display, GLuint drawable, GLint buffer);
