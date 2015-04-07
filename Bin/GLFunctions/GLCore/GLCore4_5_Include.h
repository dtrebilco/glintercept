#define GLI_INCLUDE_GL_VERSION_4_5

enum Main_Error {

  GL_CONTEXT_LOST                       = 0x0507,

};

enum Main {

  GL_NEGATIVE_ONE_TO_ONE                        = 0x935E,
  GL_ZERO_TO_ONE                                = 0x935F,
  GL_CLIP_ORIGIN                                = 0x935C,
  GL_CLIP_DEPTH_MODE                            = 0x935D,

  GL_QUERY_WAIT_INVERTED                        = 0x8E17,
  GL_QUERY_NO_WAIT_INVERTED                     = 0x8E18,
  GL_QUERY_BY_REGION_WAIT_INVERTED              = 0x8E19,
  GL_QUERY_BY_REGION_NO_WAIT_INVERTED           = 0x8E1A,
  GL_MAX_CULL_DISTANCES                         = 0x82F9,
  GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES       = 0x82FA,

  GL_TEXTURE_TARGET                             = 0x1006,

  GL_QUERY_TARGET                               = 0x82EA,
  GL_GUILTY_CONTEXT_RESET                       = 0x8253,
  GL_INNOCENT_CONTEXT_RESET                     = 0x8254,
  GL_UNKNOWN_CONTEXT_RESET                      = 0x8255,
  GL_RESET_NOTIFICATION_STRATEGY                = 0x8256,
  GL_LOSE_CONTEXT_ON_RESET                      = 0x8252,
  GL_NO_RESET_NOTIFICATION                      = 0x8261,
  //GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT             = 0x00000004,
  GL_CONTEXT_RELEASE_BEHAVIOR                   = 0x82FB,
  GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH             = 0x82FC,

};

void glClipControl(GLenum[Main] origin, GLenum[Main] depth);
void glCreateTransformFeedbacks(GLsizei n, GLuint *ids);
void glTransformFeedbackBufferBase(GLuint xfb, GLuint index, GLuint buffer);
void glTransformFeedbackBufferRange(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glGetTransformFeedbackiv(GLuint xfb, GLenum[Main] pname, GLint *param);
void glGetTransformFeedbacki_v(GLuint xfb, GLenum[Main] pname, GLuint index, GLint *param);
void glGetTransformFeedbacki64_v(GLuint xfb, GLenum[Main] pname, GLuint index, GLint64 *param);
void glCreateBuffers(GLsizei n, GLuint *buffers);
void glNamedBufferStorage(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
void glNamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum[Main] usage);
void glNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
void glCopyNamedBufferSubData(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void glClearNamedBufferData(GLuint buffer, GLenum[Main] internalformat, GLenum[Main] format, GLenum[Main] type, const void *data);
void glClearNamedBufferSubData(GLuint buffer, GLenum[Main] internalformat, GLintptr offset, GLsizeiptr size, GLenum[Main] format, GLenum[Main] type, const void *data);
void *glMapNamedBuffer(GLuint buffer, GLenum[Main] access);
void *glMapNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLboolean glUnmapNamedBuffer(GLuint buffer);
void glFlushMappedNamedBufferRange(GLuint buffer, GLintptr offset, GLsizeiptr length);
void glGetNamedBufferParameteriv(GLuint buffer, GLenum[Main] pname, GLint *params);
void glGetNamedBufferParameteri64v(GLuint buffer, GLenum[Main] pname, GLint64 *params);
void glGetNamedBufferPointerv(GLuint buffer, GLenum[Main] pname, void **params);
void glGetNamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
void glCreateFramebuffers(GLsizei n, GLuint *framebuffers);
void glNamedFramebufferRenderbuffer(GLuint framebuffer, GLenum[Main] attachment, GLenum[Main] renderbuffertarget, GLuint renderbuffer);
void glNamedFramebufferParameteri(GLuint framebuffer, GLenum[Main] pname, GLint param);
void glNamedFramebufferTexture(GLuint framebuffer, GLenum[Main] attachment, GLuint texture, GLint level);
void glNamedFramebufferTextureLayer(GLuint framebuffer, GLenum[Main] attachment, GLuint texture, GLint level, GLint layer);
void glNamedFramebufferDrawBuffer(GLuint framebuffer, GLenum[Main] buf);
void glNamedFramebufferDrawBuffers(GLuint framebuffer, GLsizei n, const GLenum[Main] *bufs);
void glNamedFramebufferReadBuffer(GLuint framebuffer, GLenum[Main] src);
void glInvalidateNamedFramebufferData(GLuint framebuffer, GLsizei numAttachments, const GLenum[Main] *attachments);
void glInvalidateNamedFramebufferSubData(GLuint framebuffer, GLsizei numAttachments, const GLenum[Main] *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
void glClearNamedFramebufferiv(GLuint framebuffer, GLenum[Main] buffer, GLint drawbuffer, const GLint *value);
void glClearNamedFramebufferuiv(GLuint framebuffer, GLenum[Main] buffer, GLint drawbuffer, const GLuint *value);
void glClearNamedFramebufferfv(GLuint framebuffer, GLenum[Main] buffer, GLint drawbuffer, const GLfloat *value);
void glClearNamedFramebufferfi(GLuint framebuffer, GLenum[Main] buffer, const GLfloat depth, GLint stencil);
void glBlitNamedFramebuffer(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum[Main] filter);
GLenum[Main] glCheckNamedFramebufferStatus(GLuint framebuffer, GLenum[Main] target);
void glGetNamedFramebufferParameteriv(GLuint framebuffer, GLenum[Main] pname, GLint *param);
void glGetNamedFramebufferAttachmentParameteriv(GLuint framebuffer, GLenum[Main] attachment, GLenum[Main] pname, GLint *params);
void glCreateRenderbuffers(GLsizei n, GLuint *renderbuffers);
void glNamedRenderbufferStorage(GLuint renderbuffer, GLenum[Main] internalformat, GLsizei width, GLsizei height);
void glNamedRenderbufferStorageMultisample(GLuint renderbuffer, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height);
void glGetNamedRenderbufferParameteriv(GLuint renderbuffer, GLenum[Main] pname, GLint *params);
void glCreateTextures(GLenum[Main] target, GLsizei n, GLuint *textures);
void glTextureBuffer(GLuint texture, GLenum[Main] internalformat, GLuint buffer);
void glTextureBufferRange(GLuint texture, GLenum[Main] internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void glTextureStorage1D(GLuint texture, GLsizei levels, GLenum[Main] internalformat, GLsizei width);
void glTextureStorage2D(GLuint texture, GLsizei levels, GLenum[Main] internalformat, GLsizei width, GLsizei height);
void glTextureStorage3D(GLuint texture, GLsizei levels, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth);
void glTextureStorage2DMultisample(GLuint texture, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void glTextureStorage3DMultisample(GLuint texture, GLsizei samples, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void glTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLenum[Main] type, const void *pixels);
void glCompressedTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum[Main] format, GLsizei imageSize, const void *data);
void glCompressedTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum[Main] format, GLsizei imageSize, const void *data);
void glCompressedTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLsizei imageSize, const void *data);
void glCopyTextureSubImage1D(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void glCopyTextureSubImage2D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void glCopyTextureSubImage3D(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void glTextureParameterf(GLuint texture, GLenum[Main] pname, GLfloat param);
void glTextureParameterfv(GLuint texture, GLenum[Main] pname, const GLfloat *param);
void glTextureParameteri(GLuint texture, GLenum[Main] pname, GLint param);
void glTextureParameterIiv(GLuint texture, GLenum[Main] pname, const GLint *params);
void glTextureParameterIuiv(GLuint texture, GLenum[Main] pname, const GLuint *params);
void glTextureParameteriv(GLuint texture, GLenum[Main] pname, const GLint *param);
void glGenerateTextureMipmap(GLuint texture);
void glBindTextureUnit(GLuint unit, GLuint texture);
void glGetTextureImage(GLuint texture, GLint level, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, void *pixels);
void glGetCompressedTextureImage(GLuint texture, GLint level, GLsizei bufSize, void *pixels);
void glGetTextureLevelParameterfv(GLuint texture, GLint level, GLenum[Main] pname, GLfloat *params);
void glGetTextureLevelParameteriv(GLuint texture, GLint level, GLenum[Main] pname, GLint *params);
void glGetTextureParameterfv(GLuint texture, GLenum[Main] pname, GLfloat *params);
void glGetTextureParameterIiv(GLuint texture, GLenum[Main] pname, GLint *params);
void glGetTextureParameterIuiv(GLuint texture, GLenum[Main] pname, GLuint *params);
void glGetTextureParameteriv(GLuint texture, GLenum[Main] pname, GLint *params);
void glCreateVertexArrays(GLsizei n, GLuint *arrays);
void glDisableVertexArrayAttrib(GLuint vaobj, GLuint index);
void glEnableVertexArrayAttrib(GLuint vaobj, GLuint index);
void glVertexArrayElementBuffer(GLuint vaobj, GLuint buffer);
void glVertexArrayVertexBuffer(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void glVertexArrayVertexBuffers(GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
void glVertexArrayAttribBinding(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
void glVertexArrayAttribFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum[Main] type, GLboolean normalized, GLuint relativeoffset);
void glVertexArrayAttribIFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum[Main] type, GLuint relativeoffset);
void glVertexArrayAttribLFormat(GLuint vaobj, GLuint attribindex, GLint size, GLenum[Main] type, GLuint relativeoffset);
void glVertexArrayBindingDivisor(GLuint vaobj, GLuint bindingindex, GLuint divisor);
void glGetVertexArrayiv(GLuint vaobj, GLenum[Main] pname, GLint *param);
void glGetVertexArrayIndexediv(GLuint vaobj, GLuint index, GLenum[Main] pname, GLint *param);
void glGetVertexArrayIndexed64iv(GLuint vaobj, GLuint index, GLenum[Main] pname, GLint64 *param);
void glCreateSamplers(GLsizei n, GLuint *samplers);
void glCreateProgramPipelines(GLsizei n, GLuint *pipelines);
void glCreateQueries(GLenum[Main] target, GLsizei n, GLuint *ids);
void glGetQueryBufferObjecti64v(GLuint id, GLuint buffer, GLenum[Main] pname, GLintptr offset);
void glGetQueryBufferObjectiv(GLuint id, GLuint buffer, GLenum[Main] pname, GLintptr offset);
void glGetQueryBufferObjectui64v(GLuint id, GLuint buffer, GLenum[Main] pname, GLintptr offset);
void glGetQueryBufferObjectuiv(GLuint id, GLuint buffer, GLenum[Main] pname, GLintptr offset);
void glMemoryBarrierByRegion(GLbitfield barriers);
void glGetTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, void *pixels);
void glGetCompressedTextureSubImage(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels);
GLenum[Main] glGetGraphicsResetStatus(void);
void glGetnCompressedTexImage(GLenum[Main] target, GLint lod, GLsizei bufSize, void *pixels);
void glGetnTexImage(GLenum[Main] target, GLint level, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, void *pixels);
void glGetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
void glGetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
void glGetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params);
void glGetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
void glReadnPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, void *data);
void glGetnMapdv(GLenum[Main] target, GLenum[Main] query, GLsizei bufSize, GLdouble *v);
void glGetnMapfv(GLenum[Main] target, GLenum[Main] query, GLsizei bufSize, GLfloat *v);
void glGetnMapiv(GLenum[Main] target, GLenum[Main] query, GLsizei bufSize, GLint *v);
void glGetnPixelMapfv(GLenum[Main] map, GLsizei bufSize, GLfloat *values);
void glGetnPixelMapuiv(GLenum[Main] map, GLsizei bufSize, GLuint *values);
void glGetnPixelMapusv(GLenum[Main] map, GLsizei bufSize, GLushort *values);
void glGetnPolygonStipple(GLsizei bufSize, GLubyte *pattern);
void glGetnColorTable(GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, void *table);
void glGetnConvolutionFilter(GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, void *image);
void glGetnSeparableFilter(GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLsizei rowBufSize, void *row, GLsizei columnBufSize, void *column, void *span);
void glGetnHistogram(GLenum[Main] target, GLboolean reset, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, void *values);
void glGetnMinmax(GLenum[Main] target, GLboolean reset, GLenum[Main] format, GLenum[Main] type, GLsizei bufSize, void *values);
void glTextureBarrier(void);
