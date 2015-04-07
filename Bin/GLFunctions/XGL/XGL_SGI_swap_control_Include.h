#define GLI_INCLUDE_XGL_SGI_SWAP_CONTROL

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

GLint glXSwapIntervalSGI(GLint interval);


