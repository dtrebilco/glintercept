#define GLI_INCLUDE_GL_OES_EGL_IMAGE

//void glEGLImageTargetTexture2DOES(GLenum[Main] target, GLeglImageOES image);
//void glEGLImageTargetRenderbufferStorageOES(GLenum[Main] target, GLeglImageOES image);

void glEGLImageTargetTexture2DOES(GLenum[Main] target, void * image);
void glEGLImageTargetRenderbufferStorageOES(GLenum[Main] target, void * image);
