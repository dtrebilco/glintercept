#define GLI_INCLUDE_EXT_COMPILED_VERTEX_ARRY

//The extension is not finished?
enum Main {
  
  GL_ARRAY_ELEMENT_LOCK_FIRST_EXT   = 0x81A8,
  GL_ARRAY_ELEMENT_LOCK_COUNT_EXT   = 0x81A9,
};


void glLockArraysEXT (GLint first, GLsizei count);
void glUnlockArraysEXT (void);


