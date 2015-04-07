#define GLI_INCLUDE_ATI_VERTEX_OBJECT

enum Main {

  GL_STATIC_ATI                 =   0x8760,
  GL_DYNAMIC_ATI                =   0x8761,
  GL_PRESERVE_ATI               =   0x8762,
  GL_DISCARD_ATI                =   0x8763,
  //GL_OBJECT_BUFFER_SIZE_ATI     =   0x8764,
  //GL_OBJECT_BUFFER_USAGE_ATI    =   0x8765,
  GL_ARRAY_OBJECT_BUFFER_ATI    =   0x8766,
  GL_ARRAY_OBJECT_OFFSET_ATI    =   0x8767,

  
  //ATI_element_array
  GL_ELEMENT_ARRAY_ATI          =   0x8768,
  GL_ELEMENT_ARRAY_TYPE_ATI     =   0x8769,
  GL_ELEMENT_ARRAY_POINTER_ATI  =   0x876A,

};


//ATI_vertex_array_object
GLuint glNewObjectBufferATI(GLsizei size, const void *pointer, GLenum[Main] usage);

GLboolean glIsObjectBufferATI(GLuint buffer);

void glUpdateObjectBufferATI(GLuint buffer, GLuint offset, GLsizei size, const void *pointer, GLenum[Main] preserve);

void glGetObjectBufferfvATI(GLuint buffer, GLenum[Main] pname, GLfloat *params);

void glGetObjectBufferivATI(GLuint buffer, GLenum[Main] pname, GLint *params);

void glFreeObjectBufferATI(GLuint buffer);


void glArrayObjectATI(GLenum[Main] array, GLint size, GLenum[Main] type, GLsizei stride, GLuint buffer, GLuint offset);


void glGetArrayObjectfvATI(GLenum[Main] array, GLenum[Main] pname, GLfloat *params);
void glGetArrayObjectivATI(GLenum[Main] array, GLenum[Main] pname, GLint *params);


void glVariantArrayObjectATI(GLuint id, GLenum[Main] type, GLsizei stride, GLuint buffer, GLuint offset);

void glGetVariantArrayObjectfvATI(GLuint id, GLenum[Main] pname, GLfloat *params);
void glGetVariantArrayObjectivATI(GLuint id, GLenum[Main] pname, GLint *params);


//ATI_vertex_attrib_array_object
void glVertexAttribArrayObjectATI(GLuint index, GLint size, GLenum[Main] type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset);

void glGetVertexAttribArrayObjectfvATI(GLuint index, GLenum pname, GLfloat *params);

void glGetVertexAttribArrayObjectivATI(GLuint index, GLenum pname, GLint *params);


//ATI_element_array
void glElementPointerATI(GLenum[Main] type, const void *pointer);
    
void glDrawElementArrayATI(GLenum[Main] mode, GLsizei count);
    
void glDrawRangeElementArrayATI(GLenum[Main] mode, GLuint start, GLuint end, GLsizei count);
    
//ATI_map_object_buffer    
void *glMapObjectBufferATI(GLuint buffer);
    
void glUnmapObjectBufferATI(GLuint buffer);
