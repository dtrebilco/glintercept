#define GLI_INCLUDE_XGL_SGI_VIDEO_SYNC

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


GLint glXGetVideoSyncSGI (GLuint *count);
GLint glXWaitVideoSyncSGI(GLint divisor, GLint remainder, GLuint *count);


