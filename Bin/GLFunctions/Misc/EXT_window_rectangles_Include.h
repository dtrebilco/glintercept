#define GLI_INCLUDE_GL_EXT_WINDOW_RECTANGLES

enum Main {

  GL_INCLUSIVE_EXT                   = 0x8F10,
  GL_EXCLUSIVE_EXT                   = 0x8F11,
  GL_WINDOW_RECTANGLE_EXT            = 0x8F12,
  GL_WINDOW_RECTANGLE_MODE_EXT       = 0x8F13,
  GL_MAX_WINDOW_RECTANGLES_EXT       = 0x8F14,
  GL_NUM_WINDOW_RECTANGLES_EXT       = 0x8F15,

};

void glWindowRectanglesEXT(GLenum[Main] mode, GLsizei count, const GLint *box);
