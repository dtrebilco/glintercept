#define GLI_INCLUDE_GLCORE1_2


///////////////////////////////////////////////////////////////////////////////
// Masks
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Error enum
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Main enum
///////////////////////////////////////////////////////////////////////////////

// Main enum for all standard enums
//  (may want to break up for speed)
enum Main {

/* OpenGL 1.2 */
  GL_RESCALE_NORMAL                     =  0x803A,
  GL_CLAMP_TO_EDGE                      =  0x812F,
  GL_MAX_ELEMENTS_VERTICES              =  0x80E8,
  GL_MAX_ELEMENTS_INDICES               =  0x80E9,
  GL_BGR                                =  0x80E0,
  GL_BGRA                               =  0x80E1,
  GL_UNSIGNED_BYTE_3_3_2                =  0x8032,
  GL_UNSIGNED_BYTE_2_3_3_REV            =  0x8362,
  GL_UNSIGNED_SHORT_5_6_5               =  0x8363,
  GL_UNSIGNED_SHORT_5_6_5_REV           =  0x8364,
  GL_UNSIGNED_SHORT_4_4_4_4             =  0x8033,
  GL_UNSIGNED_SHORT_4_4_4_4_REV         =  0x8365,
  GL_UNSIGNED_SHORT_5_5_5_1             =  0x8034,
  GL_UNSIGNED_SHORT_1_5_5_5_REV         =  0x8366,
  GL_UNSIGNED_INT_8_8_8_8               =  0x8035,
  GL_UNSIGNED_INT_8_8_8_8_REV           =  0x8367,
  GL_UNSIGNED_INT_10_10_10_2            =  0x8036,
  GL_UNSIGNED_INT_2_10_10_10_REV        =  0x8368,
  GL_LIGHT_MODEL_COLOR_CONTROL          =  0x81F8,
  GL_SINGLE_COLOR                       =  0x81F9,
  GL_SEPARATE_SPECULAR_COLOR            =  0x81FA,
  GL_TEXTURE_MIN_LOD                    =  0x813A,
  GL_TEXTURE_MAX_LOD                    =  0x813B,
  GL_TEXTURE_BASE_LEVEL                 =  0x813C,
  GL_TEXTURE_MAX_LEVEL                  =  0x813D,
/*  TODO: Resolve
  GL_SMOOTH_POINT_SIZE_RANGE            =  0x0B12,
  GL_SMOOTH_POINT_SIZE_GRANULARITY      =  0x0B13,
  GL_SMOOTH_LINE_WIDTH_RANGE            =  0x0B22,
  GL_SMOOTH_LINE_WIDTH_GRANULARITY      =  0x0B23,
*/
  GL_ALIASED_POINT_SIZE_RANGE           =  0x846D,
  GL_ALIASED_LINE_WIDTH_RANGE           =  0x846E,



// OpenGL 1.2 imaging subset 
// GL_EXT_color_table 
  GL_COLOR_TABLE                        =  0x80D0,
  GL_POST_CONVOLUTION_COLOR_TABLE       =  0x80D1,
  GL_POST_COLOR_MATRIX_COLOR_TABLE      =  0x80D2,
  GL_PROXY_COLOR_TABLE                  =  0x80D3,
  GL_PROXY_POST_CONVOLUTION_COLOR_TABLE =  0x80D4,
  GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE = 0x80D5,
  GL_COLOR_TABLE_SCALE                  =  0x80D6,
  GL_COLOR_TABLE_BIAS                   =  0x80D7,
  GL_COLOR_TABLE_FORMAT                 =  0x80D8,
  GL_COLOR_TABLE_WIDTH                  =  0x80D9,
  GL_COLOR_TABLE_RED_SIZE               =  0x80DA,
  GL_COLOR_TABLE_GREEN_SIZE             =  0x80DB,
  GL_COLOR_TABLE_BLUE_SIZE              =  0x80DC,
  GL_COLOR_TABLE_ALPHA_SIZE             =  0x80DD,
  GL_COLOR_TABLE_LUMINANCE_SIZE         =  0x80DE,
  GL_COLOR_TABLE_INTENSITY_SIZE         =  0x80DF,
/* GL_EXT_convolution and GL_HP_convolution_border_modes */
  GL_CONVOLUTION_1D                     =  0x8010,
  GL_CONVOLUTION_2D                     =  0x8011,
  GL_SEPARABLE_2D                       =  0x8012,
  GL_CONVOLUTION_BORDER_MODE            =  0x8013,
  GL_CONVOLUTION_FILTER_SCALE           =  0x8014,
  GL_CONVOLUTION_FILTER_BIAS            =  0x8015,
  GL_REDUCE                             =  0x8016,
  GL_CONVOLUTION_FORMAT                 =  0x8017,
  GL_CONVOLUTION_WIDTH                  =  0x8018,
  GL_CONVOLUTION_HEIGHT                 =  0x8019,
  GL_MAX_CONVOLUTION_WIDTH              =  0x801A,
  GL_MAX_CONVOLUTION_HEIGHT             =  0x801B,
  GL_POST_CONVOLUTION_RED_SCALE         =  0x801C,
  GL_POST_CONVOLUTION_GREEN_SCALE       =  0x801D,
  GL_POST_CONVOLUTION_BLUE_SCALE        =  0x801E,
  GL_POST_CONVOLUTION_ALPHA_SCALE       =  0x801F,
  GL_POST_CONVOLUTION_RED_BIAS          =  0x8020,
  GL_POST_CONVOLUTION_GREEN_BIAS        =  0x8021,
  GL_POST_CONVOLUTION_BLUE_BIAS         =  0x8022,
  GL_POST_CONVOLUTION_ALPHA_BIAS        =  0x8023,
  GL_CONSTANT_BORDER                    =  0x8151,
  GL_REPLICATE_BORDER                   =  0x8153,
  GL_CONVOLUTION_BORDER_COLOR           =  0x8154,
/* GL_SGI_color_matrix */
  GL_COLOR_MATRIX                       =  0x80B1,
  GL_COLOR_MATRIX_STACK_DEPTH           =  0x80B2,
  GL_MAX_COLOR_MATRIX_STACK_DEPTH       =  0x80B3,
  GL_POST_COLOR_MATRIX_RED_SCALE        =  0x80B4,
  GL_POST_COLOR_MATRIX_GREEN_SCALE      =  0x80B5,
  GL_POST_COLOR_MATRIX_BLUE_SCALE       =  0x80B6,
  GL_POST_COLOR_MATRIX_ALPHA_SCALE      =  0x80B7,
  GL_POST_COLOR_MATRIX_RED_BIAS         =  0x80B8,
  GL_POST_COLOR_MATRIX_GREEN_BIAS       =  0x80B9,
  GL_POST_COLOR_MATRIX_BLUE_BIAS        =  0x80BA,
  GL_POST_COLOR_MATRIX_ALPHA_BIAS       =  0x80BB,
/* GL_EXT_histogram */
  GL_HISTOGRAM                          =  0x8024,
  GL_PROXY_HISTOGRAM                    =  0x8025,
  GL_HISTOGRAM_WIDTH                    =  0x8026,
  GL_HISTOGRAM_FORMAT                   =  0x8027,
  GL_HISTOGRAM_RED_SIZE                 =  0x8028,
  GL_HISTOGRAM_GREEN_SIZE               =  0x8029,
  GL_HISTOGRAM_BLUE_SIZE                =  0x802A,
  GL_HISTOGRAM_ALPHA_SIZE               =  0x802B,
  GL_HISTOGRAM_LUMINANCE_SIZE           =  0x802C,
  GL_HISTOGRAM_SINK                     =  0x802D,
  GL_MINMAX                             =  0x802E,
  GL_MINMAX_FORMAT                      =  0x802F,
  GL_MINMAX_SINK                        =  0x8030,
  GL_TABLE_TOO_LARGE                    =  0x8031,
/* GL_EXT_blend_color, GL_EXT_blend_minmax */
  GL_BLEND_EQUATION                     =  0x8009,
  GL_MIN                                =  0x8007,
  GL_MAX                                =  0x8008,
  GL_FUNC_ADD                           =  0x8006,
  GL_FUNC_SUBTRACT                      =  0x800A,
  GL_FUNC_REVERSE_SUBTRACT              =  0x800B,
  GL_BLEND_COLOR                        =  0x8005,


/* GL 1.2 texturing */
  GL_PACK_SKIP_IMAGES                   =  0x806B,
  GL_PACK_IMAGE_HEIGHT                  =  0x806C,
  GL_UNPACK_SKIP_IMAGES                 =  0x806D,
  GL_UNPACK_IMAGE_HEIGHT                =  0x806E,
  GL_TEXTURE_3D                         =  0x806F,
  GL_PROXY_TEXTURE_3D                   =  0x8070,
  GL_TEXTURE_DEPTH                      =  0x8071,
  GL_TEXTURE_WRAP_R                     =  0x8072,
  GL_MAX_3D_TEXTURE_SIZE                =  0x8073,
  GL_TEXTURE_BINDING_3D                 =  0x806A,

};

///////////////////////////////////////////////////////////////////////////////
// OpenGL 1.2 functions
///////////////////////////////////////////////////////////////////////////////

void glDrawRangeElements( GLenum[Primitives] mode, GLuint start, GLuint end, GLsizei count, GLenum[Main] type, const GLvoid *indices );

void glTexImage3D( GLenum[Main] target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum[Main] format, GLenum[Main] type, const GLvoid *pixels );

void glTexSubImage3D( GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,GLenum[Main] format, GLenum[Main] type, const GLvoid *pixels);

void glCopyTexSubImage3D( GLenum[Main] target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );


/* 1.2 imaging extension functions */
void glColorTable( GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLenum[Main] format, GLenum[Main] type, const GLvoid *table );

void glColorSubTable( GLenum[Main] target, GLsizei start, GLsizei count, GLenum[Main] format, GLenum[Main] type, const GLvoid *data );

void glColorTableParameteriv(GLenum[Main] target, GLenum[Main] pname, const GLint *params);

void glColorTableParameterfv(GLenum[Main] target, GLenum[Main] pname, const GLfloat *params);

void glCopyColorSubTable( GLenum[Main] target, GLsizei start, GLint x, GLint y, GLsizei width );

void glCopyColorTable( GLenum[Main] target, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width );

void glGetColorTable( GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLvoid *table );

void glGetColorTableParameterfv( GLenum[Main] target, GLenum[Main] pname, GLfloat *params );

void glGetColorTableParameteriv( GLenum[Main] target, GLenum[Main] pname, GLint *params );

void glBlendEquation( GLenum[Main] mode );

void glBlendColor( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha );

void glHistogram( GLenum[Main] target, GLsizei width, GLenum[Main] internalformat, GLboolean sink );

void glResetHistogram( GLenum[Main] target );

void glGetHistogram( GLenum[Main] target, GLboolean reset,GLenum[Main] format, GLenum[Main] type, GLvoid *values );

void glGetHistogramParameterfv( GLenum[Main] target, GLenum[Main] pname, GLfloat *params );

void glGetHistogramParameteriv( GLenum[Main] target, GLenum[Main] pname, GLint *params );

void glMinmax( GLenum[Main] target, GLenum[Main] internalformat, GLboolean sink );

void glResetMinmax( GLenum[Main] target );

void glGetMinmax( GLenum[Main] target, GLboolean reset, GLenum[Main] format, GLenum[Main] types, GLvoid *values );

void glGetMinmaxParameterfv( GLenum[Main] target, GLenum[Main] pname, GLfloat *params );

void glGetMinmaxParameteriv( GLenum[Main] target, GLenum[Main] pname, GLint *params );

void glConvolutionFilter1D( GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLenum[Main] format, GLenum[Main] type, const GLvoid *image );

void glConvolutionFilter2D( GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, const GLvoid *image );

void glConvolutionParameterf( GLenum[Main] target, GLenum[Main] pname, GLfloat params );

void glConvolutionParameterfv( GLenum[Main] target, GLenum[Main] pname, const GLfloat *params );

void glConvolutionParameteri( GLenum[Main] target, GLenum[Main] pname, GLint params );

void glConvolutionParameteriv( GLenum[Main] target, GLenum[Main] pname, const GLint *params );

void glCopyConvolutionFilter1D( GLenum[Main] target, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width );

void glCopyConvolutionFilter2D( GLenum[Main] target, GLenum[Main] internalformat, GLint x, GLint y, GLsizei width, GLsizei height);

void glGetConvolutionFilter( GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLvoid *image );

void glGetConvolutionParameterfv( GLenum[Main] target, GLenum[Main] pname, GLfloat *params );

void glGetConvolutionParameteriv( GLenum[Main] target, GLenum[Main] pname, GLint *params );

void glSeparableFilter2D( GLenum[Main] target, GLenum[Main] internalformat, GLsizei width, GLsizei height, GLenum[Main] format, GLenum[Main] type, const GLvoid *row, const GLvoid *column );

void glGetSeparableFilter( GLenum[Main] target, GLenum[Main] format, GLenum[Main] type, GLvoid *row, GLvoid *column, GLvoid *span );



