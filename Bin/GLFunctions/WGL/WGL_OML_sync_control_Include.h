#define GLI_INCLUDE_WGL_OML_SYNC_CONTROL

GLboolean wglGetSyncValuesOML(void* hdc, GLint64 * ust, GLint64 * msc, GLint64 * sbc);
GLboolean wglGetMscRateOML(void* hdc, GLint * numerator, GLint * denominator);

// GLIntercept does not currenmtly support 64 bit return types....will probably break if used
//GLint64 wglSwapBuffersMscOML(void* hdc, GLint64 target_msc, GLint64 divisor, GLint64 remainder);
//GLint64 wglSwapLayerBuffersMscOML(void* hdc, int fuPlanes, GLint64 target_msc, GLint64 divisor, GLint64 remainder);
GLboolean wglWaitForMscOML(void* hdc, GLint64 target_msc, GLint64 divisor, GLint64 remainder, GLint64 * ust, GLint64 * msc, GLint64 * sbc);
GLboolean wglWaitForSbcOML(void* hdc, GLint64 target_sbc, GLint64 * ust, GLint64 * msc, GLint64 * sbc);
