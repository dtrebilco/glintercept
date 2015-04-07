#define GLI_INCLUDE_GL_NV_VERTEX_BUFFER_UNIFIED_MEMORY

enum Main {

  GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV         = 0x8F1E,
  GL_ELEMENT_ARRAY_UNIFIED_NV               = 0x8F1F,
  GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV         = 0x8F20,
  GL_VERTEX_ARRAY_ADDRESS_NV                = 0x8F21,
  GL_NORMAL_ARRAY_ADDRESS_NV                = 0x8F22,
  GL_COLOR_ARRAY_ADDRESS_NV                 = 0x8F23,
  GL_INDEX_ARRAY_ADDRESS_NV                 = 0x8F24,
  GL_TEXTURE_COORD_ARRAY_ADDRESS_NV         = 0x8F25,
  GL_EDGE_FLAG_ARRAY_ADDRESS_NV             = 0x8F26,
  GL_SECONDARY_COLOR_ARRAY_ADDRESS_NV       = 0x8F27,
  GL_FOG_COORD_ARRAY_ADDRESS_NV             = 0x8F28,
  GL_ELEMENT_ARRAY_ADDRESS_NV               = 0x8F29,
  GL_VERTEX_ATTRIB_ARRAY_LENGTH_NV          = 0x8F2A,
  GL_VERTEX_ARRAY_LENGTH_NV                 = 0x8F2B,
  GL_NORMAL_ARRAY_LENGTH_NV                 = 0x8F2C,
  GL_COLOR_ARRAY_LENGTH_NV                  = 0x8F2D,
  GL_INDEX_ARRAY_LENGTH_NV                  = 0x8F2E,
  GL_TEXTURE_COORD_ARRAY_LENGTH_NV          = 0x8F2F,
  GL_EDGE_FLAG_ARRAY_LENGTH_NV              = 0x8F30,
  GL_SECONDARY_COLOR_ARRAY_LENGTH_NV        = 0x8F31,
  GL_FOG_COORD_ARRAY_LENGTH_NV              = 0x8F32,
  GL_ELEMENT_ARRAY_LENGTH_NV                = 0x8F33,
  GL_DRAW_INDIRECT_UNIFIED_NV               = 0x8F40,
  GL_DRAW_INDIRECT_ADDRESS_NV               = 0x8F41,
  GL_DRAW_INDIRECT_LENGTH_NV                = 0x8F42,

};

void glBufferAddressRangeNV(GLenum[Main] pname, GLuint index, GLuint64 address, GLsizeiptr length);
void glVertexFormatNV(GLint size, GLenum[Main] type, GLsizei stride);
void glNormalFormatNV(GLenum[Main] type, GLsizei stride);
void glColorFormatNV(GLint size, GLenum[Main] type, GLsizei stride);
void glIndexFormatNV(GLenum[Main] type, GLsizei stride);
void glTexCoordFormatNV(GLint size, GLenum[Main] type, GLsizei stride);
void glEdgeFlagFormatNV(GLsizei stride);
void glSecondaryColorFormatNV(GLint size, GLenum[Main] type, GLsizei stride);
void glFogCoordFormatNV(GLenum[Main] type, GLsizei stride);
void glVertexAttribFormatNV(GLuint index, GLint size, GLenum[Main] type, GLboolean normalized, GLsizei stride);
void glVertexAttribIFormatNV(GLuint index, GLint size, GLenum[Main] type, GLsizei stride);
void glGetIntegerui64i_vNV(GLenum[Main] value, GLuint index, GLuint64 * result);
