#define GLI_INCLUDE_GL_SGIX_INSTRUMENTS

enum Main {

  GL_INSTRUMENT_BUFFER_POINTER_SGIX       = 0x8180,
  GL_INSTRUMENT_MEASUREMENTS_SGIX         = 0x8181,

};

GLint glGetInstrumentsSGIX(void);
void glInstrumentsBufferSGIX(GLsizei size, GLint * buffer);
GLint glPollInstrumentsSGIX(GLint * marker_p);
void glReadInstrumentsSGIX(GLint marker);
void glStartInstrumentsSGIX(void);
void glStopInstrumentsSGIX(GLint marker);
