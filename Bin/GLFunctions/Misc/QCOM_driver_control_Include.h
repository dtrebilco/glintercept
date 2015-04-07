#define GLI_INCLUDE_GL_QCOM_DRIVER_CONTROL

void glGetDriverControlsQCOM(GLint *num, GLsizei size, GLuint *driverControls);
void glGetDriverControlStringQCOM(GLuint driverControl, GLsizei bufSize, GLsizei *length, GLchar *driverControlString);
void glEnableDriverControlQCOM(GLuint driverControl);
void glDisableDriverControlQCOM(GLuint driverControl);
