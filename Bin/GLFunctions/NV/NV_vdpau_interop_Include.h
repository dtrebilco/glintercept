#define GLI_INCLUDE_GL_NV_VDPAU_INTEROP

enum Main {

  GL_SURFACE_STATE_NV            = 0x86EB,
  GL_SURFACE_REGISTERED_NV       = 0x86FD,
  GL_SURFACE_MAPPED_NV           = 0x8700,
  GL_WRITE_DISCARD_NV            = 0x88BE,

};

// Reaplace GLintptr with GLintptr

void glVDPAUInitNV(const GLvoid * vdpDevice, const GLvoid * getProcAddress);
void glVDPAUFiniNV(void);
GLintptr glVDPAURegisterVideoSurfaceNV(GLvoid * vdpSurface, GLenum[Main] target, GLsizei numTextureNames, const GLuint * textureNames);
GLintptr glVDPAURegisterOutputSurfaceNV(GLvoid * vdpSurface, GLenum[Main] target, GLsizei numTextureNames, const GLuint * textureNames);
GLboolean glVDPAUIsSurfaceNV(GLintptr surface);
void glVDPAUUnregisterSurfaceNV(GLintptr surface);
void glVDPAUGetSurfaceivNV(GLintptr surface, GLenum[Main] pname, GLsizei bufSize, GLsizei * length, GLint * values);
void glVDPAUSurfaceAccessNV(GLintptr surface, GLenum[Main] access);
void glVDPAUMapSurfacesNV(GLsizei numSurfaces, const GLintptr * surfaces);
void glVDPAUUnmapSurfacesNV(GLsizei numSurface, const GLintptr * surfaces);
