#define GLI_INCLUDE_GL_OES_DRAW_TEXTURE

enum Main {

  GL_TEXTURE_CROP_RECT_OES       = 0x8B9D,

};

void glDrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height);
void glDrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height);
void glDrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height);
void glDrawTexsvOES(const GLshort *coords);
void glDrawTexivOES(const GLint *coords);
void glDrawTexxvOES(const GLfixed *coords);
void glDrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height);
void glDrawTexfvOES(const GLfloat *coords);
