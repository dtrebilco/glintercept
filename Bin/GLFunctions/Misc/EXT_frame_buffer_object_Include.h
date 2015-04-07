#define GLI_INCLUDE_EXT_FRAME_BUFFER_OBJECT


enum Main {

    //GL_FRAMEBUFFER_EXT                 =    0x8D40,

    //GL_RENDERBUFFER_EXT                =    0x8D41,

    GL_STENCIL_INDEX_EXT               =    0x8D45,
    //GL_STENCIL_INDEX1_EXT              =    0x8D46,
    //GL_STENCIL_INDEX4_EXT              =    0x8D47,
    //GL_STENCIL_INDEX8_EXT              =    0x8D48,
    //GL_STENCIL_INDEX16_EXT             =    0x8D49,

    //GL_RENDERBUFFER_WIDTH_EXT          =    0x8D42,
    //GL_RENDERBUFFER_HEIGHT_EXT         =    0x8D43,
    //GL_RENDERBUFFER_INTERNAL_FORMAT_EXT =    0x8D44,

    //GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT        =    0x8CD0,
    //GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT        =    0x8CD1,
    //GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT      =    0x8CD2,
    //GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT = 0x8CD3,
    //GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT =    0x8CD4,

    //GL_COLOR_ATTACHMENT0_EXT            =    0x8CE0,
    //GL_COLOR_ATTACHMENT1_EXT            =    0x8CE1,
    //GL_COLOR_ATTACHMENT2_EXT            =    0x8CE2,
    //GL_COLOR_ATTACHMENT3_EXT            =    0x8CE3,
    //GL_COLOR_ATTACHMENT4_EXT            =    0x8CE4,
    //GL_COLOR_ATTACHMENT5_EXT            =    0x8CE5,
    //GL_COLOR_ATTACHMENT6_EXT            =    0x8CE6,
    //GL_COLOR_ATTACHMENT7_EXT            =    0x8CE7,
    //GL_COLOR_ATTACHMENT8_EXT            =    0x8CE8,
    //GL_COLOR_ATTACHMENT9_EXT            =    0x8CE9,
    //GL_COLOR_ATTACHMENT10_EXT           =    0x8CEA,
    //GL_COLOR_ATTACHMENT11_EXT           =    0x8CEB,
    //GL_COLOR_ATTACHMENT12_EXT           =    0x8CEC,
    //GL_COLOR_ATTACHMENT13_EXT           =    0x8CED,
    //GL_COLOR_ATTACHMENT14_EXT           =    0x8CEE,
    //GL_COLOR_ATTACHMENT15_EXT           =    0x8CEF,
    //GL_DEPTH_ATTACHMENT_EXT             =    0x8D00,
    //GL_STENCIL_ATTACHMENT_EXT           =    0x8D20,

    //GL_FRAMEBUFFER_COMPLETE_EXT                      =    0x8CD5,
    //GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT         =    0x8CD6,
    //GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT =    0x8CD7,
    GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT =  0x8CD8,
    GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT         =    0x8CD9,
    GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT            =    0x8CDA,
    //GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT        =    0x8CDB,
    //GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT        =    0x8CDC,
    //GL_FRAMEBUFFER_UNSUPPORTED_EXT                   =    0x8CDD,
    GL_FRAMEBUFFER_STATUS_ERROR_EXT                  =    0x8CDE,

    //GL_FRAMEBUFFER_BINDING_EXT         =    0x8CA6,
    //GL_RENDERBUFFER_BINDING_EXT        =    0x8CA7,
    //GL_MAX_COLOR_ATTACHMENTS_EXT       =    0x8CDF,
    //GL_MAX_RENDERBUFFER_SIZE_EXT       =    0x84E8,

};

//New error code
enum Main_Error {

  //GL_INVALID_FRAMEBUFFER_OPERATION_EXT  =  0x0506,

};



GLboolean glIsRenderbufferEXT(GLuint renderbuffer);
void glBindRenderbufferEXT(GLenum[Main] target, GLuint renderbuffer);

void glDeleteRenderbuffersEXT(GLsizei n, const GLuint *renderbuffers);
void glGenRenderbuffersEXT(GLsizei n, GLuint *renderbuffers);

void glRenderbufferStorageEXT(GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLsizei height);

void glGetRenderbufferParameterivEXT(GLenum[Main] target, GLenum[Main] pname, GLint* params);

GLboolean glIsFramebufferEXT(GLuint framebuffer);
void glBindFramebufferEXT(GLenum[Main] target, GLuint framebuffer);
void glDeleteFramebuffersEXT(GLsizei n, const GLuint *framebuffers);
void glGenFramebuffersEXT(GLsizei n, GLuint *framebuffers);

GLenum[Main] glCheckFramebufferStatusEXT(GLenum[Main] target);

void glFramebufferTexture1DEXT(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level);
void glFramebufferTexture2DEXT(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level);
void glFramebufferTexture3DEXT(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] textarget, GLuint texture, GLint level, GLint zoffset);

void glFramebufferRenderbufferEXT(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] renderbuffertarget, GLuint renderbuffer);

void glGetFramebufferAttachmentParameterivEXT(GLenum[Main] target, GLenum[Main] attachment, GLenum[Main] pname, GLint *params);

void glGenerateMipmapEXT(GLenum[Main] target);
