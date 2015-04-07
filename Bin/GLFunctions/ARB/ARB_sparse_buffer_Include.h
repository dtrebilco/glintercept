#define GLI_INCLUDE_GL_ARB_SPARSE_BUFFER

enum Mask_Access {

  GL_SPARSE_STORAGE_BIT_ARB            = 0x0400,

};

enum Main {

  GL_SPARSE_BUFFER_PAGE_SIZE_ARB       = 0x82F8,

};

void glBufferPageCommitmentARB(GLenum[Main] target, GLintptr offset, GLsizeiptr size, GLboolean commit);
void glNamedBufferPageCommitmentEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
void glNamedBufferPageCommitmentARB(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
