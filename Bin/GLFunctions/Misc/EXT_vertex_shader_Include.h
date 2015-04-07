#define GLI_INCLUDE_EXT_VERTEX_SHADER

enum Main {

  GL_VERTEX_SHADER_EXT                    =  0x8780,
  GL_VERTEX_SHADER_BINDING_EXT            =  0x8781,
  GL_OP_INDEX_EXT                         =  0x8782,
  GL_OP_NEGATE_EXT                        =  0x8783,
  GL_OP_DOT3_EXT                          =  0x8784,
  GL_OP_DOT4_EXT                          =  0x8785,
  GL_OP_MUL_EXT                           =  0x8786,
  GL_OP_ADD_EXT                           =  0x8787,
  GL_OP_MADD_EXT                          =  0x8788,
  GL_OP_FRAC_EXT                          =  0x8789,
  GL_OP_MAX_EXT                           =  0x878A,
  GL_OP_MIN_EXT                           =  0x878B,
  GL_OP_SET_GE_EXT                        =  0x878C,
  GL_OP_SET_LT_EXT                        =  0x878D,
  GL_OP_CLAMP_EXT                         =  0x878E,
  GL_OP_FLOOR_EXT                         =  0x878F,
  GL_OP_ROUND_EXT                         =  0x8790,
  GL_OP_EXP_BASE_2_EXT                    =  0x8791,
  GL_OP_LOG_BASE_2_EXT                    =  0x8792,
  GL_OP_POWER_EXT                         =  0x8793,
  GL_OP_RECIP_EXT                         =  0x8794,
  GL_OP_RECIP_SQRT_EXT                    =  0x8795,
  GL_OP_SUB_EXT                           =  0x8796,
  GL_OP_CROSS_PRODUCT_EXT                 =  0x8797,
  GL_OP_MULTIPLY_MATRIX_EXT               =  0x8798,
  GL_OP_MOV_EXT                           =  0x8799,
  GL_OUTPUT_VERTEX_EXT                    =  0x879A,
  GL_OUTPUT_COLOR0_EXT                    =  0x879B,
  GL_OUTPUT_COLOR1_EXT                    =  0x879C,
  GL_OUTPUT_TEXTURE_COORD0_EXT            =  0x879D,
  GL_OUTPUT_TEXTURE_COORD1_EXT            =  0x879E,
  GL_OUTPUT_TEXTURE_COORD2_EXT            =  0x879F,
  GL_OUTPUT_TEXTURE_COORD3_EXT            =  0x87A0,
  GL_OUTPUT_TEXTURE_COORD4_EXT            =  0x87A1,
  GL_OUTPUT_TEXTURE_COORD5_EXT            =  0x87A2,
  GL_OUTPUT_TEXTURE_COORD6_EXT            =  0x87A3,
  GL_OUTPUT_TEXTURE_COORD7_EXT            =  0x87A4,
  GL_OUTPUT_TEXTURE_COORD8_EXT            =  0x87A5,
  GL_OUTPUT_TEXTURE_COORD9_EXT            =  0x87A6,
  GL_OUTPUT_TEXTURE_COORD10_EXT           =  0x87A7,
  GL_OUTPUT_TEXTURE_COORD11_EXT           =  0x87A8,
  GL_OUTPUT_TEXTURE_COORD12_EXT           =  0x87A9,
  GL_OUTPUT_TEXTURE_COORD13_EXT           =  0x87AA,
  GL_OUTPUT_TEXTURE_COORD14_EXT           =  0x87AB,
  GL_OUTPUT_TEXTURE_COORD15_EXT           =  0x87AC,
  GL_OUTPUT_TEXTURE_COORD16_EXT           =  0x87AD,
  GL_OUTPUT_TEXTURE_COORD17_EXT           =  0x87AE,
  GL_OUTPUT_TEXTURE_COORD18_EXT           =  0x87AF,
  GL_OUTPUT_TEXTURE_COORD19_EXT           =  0x87B0,
  GL_OUTPUT_TEXTURE_COORD20_EXT           =  0x87B1,
  GL_OUTPUT_TEXTURE_COORD21_EXT           =  0x87B2,
  GL_OUTPUT_TEXTURE_COORD22_EXT           =  0x87B3,
  GL_OUTPUT_TEXTURE_COORD23_EXT           =  0x87B4,
  GL_OUTPUT_TEXTURE_COORD24_EXT           =  0x87B5,
  GL_OUTPUT_TEXTURE_COORD25_EXT           =  0x87B6,
  GL_OUTPUT_TEXTURE_COORD26_EXT           =  0x87B7,
  GL_OUTPUT_TEXTURE_COORD27_EXT           =  0x87B8,
  GL_OUTPUT_TEXTURE_COORD28_EXT           =  0x87B9,
  GL_OUTPUT_TEXTURE_COORD29_EXT           =  0x87BA,
  GL_OUTPUT_TEXTURE_COORD30_EXT           =  0x87BB,
  GL_OUTPUT_TEXTURE_COORD31_EXT           =  0x87BC,
  GL_OUTPUT_FOG_EXT                       =  0x87BD,
  GL_SCALAR_EXT                           =  0x87BE,
  GL_VECTOR_EXT                           =  0x87BF,
  GL_MATRIX_EXT                           =  0x87C0,
  GL_VARIANT_EXT                          =  0x87C1,
  GL_INVARIANT_EXT                        =  0x87C2,
  GL_LOCAL_CONSTANT_EXT                   =  0x87C3,
  GL_LOCAL_EXT                            =  0x87C4,
  GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT   =  0x87C5,
  GL_MAX_VERTEX_SHADER_VARIANTS_EXT       =  0x87C6,
  GL_MAX_VERTEX_SHADER_INVARIANTS_EXT     =  0x87C7,
  GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT =  0x87C8,
  GL_MAX_VERTEX_SHADER_LOCALS_EXT          =  0x87C9,
  GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT          = 0x87CA,
  GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT              = 0x87CB,
  GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT       = 0x87CC,
  GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT            = 0x87CD,
  GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT                = 0x87CE,
  GL_VERTEX_SHADER_INSTRUCTIONS_EXT       =  0x87CF,
  GL_VERTEX_SHADER_VARIANTS_EXT           =  0x87D0,
  GL_VERTEX_SHADER_INVARIANTS_EXT         =  0x87D1,
  GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT    =  0x87D2,
  GL_VERTEX_SHADER_LOCALS_EXT             =  0x87D3,
  GL_VERTEX_SHADER_OPTIMIZED_EXT          =  0x87D4,
  GL_X_EXT                                =  0x87D5,
  GL_Y_EXT                                =  0x87D6,
  GL_Z_EXT                                =  0x87D7,
  GL_W_EXT                                =  0x87D8,
  GL_NEGATIVE_X_EXT                       =  0x87D9,
  GL_NEGATIVE_Y_EXT                       =  0x87DA,
  GL_NEGATIVE_Z_EXT                       =  0x87DB,
  GL_NEGATIVE_W_EXT                       =  0x87DC,
  GL_ZERO_EXT                             =  0x87DD,
  GL_ONE_EXT                              =  0x87DE,
  GL_NEGATIVE_ONE_EXT                     =  0x87DF,
  GL_NORMALIZED_RANGE_EXT                 =  0x87E0,
  GL_FULL_RANGE_EXT                       =  0x87E1,
  GL_CURRENT_VERTEX_EXT                   =  0x87E2,
  GL_MVP_MATRIX_EXT                       =  0x87E3,
  GL_VARIANT_VALUE_EXT                    =  0x87E4,
  GL_VARIANT_DATATYPE_EXT                 =  0x87E5,
  GL_VARIANT_ARRAY_STRIDE_EXT             =  0x87E6,
  GL_VARIANT_ARRAY_TYPE_EXT               =  0x87E7,
  GL_VARIANT_ARRAY_EXT                    =  0x87E8,
  GL_VARIANT_ARRAY_POINTER_EXT            =  0x87E9,
  GL_INVARIANT_VALUE_EXT                  =  0x87EA,
  GL_INVARIANT_DATATYPE_EXT               =  0x87EB,
  GL_LOCAL_CONSTANT_VALUE_EXT             =  0x87EC,
  GL_LOCAL_CONSTANT_DATATYPE_EXT          =  0x87ED,

};



void glBeginVertexShaderEXT( void );
void glEndVertexShaderEXT( void );
void glBindVertexShaderEXT( GLuint id );
GLuint glGenVertexShadersEXT( GLuint range );
void glDeleteVertexShaderEXT( GLuint id );
void glShaderOp1EXT( GLenum[Main] op, GLuint res, GLuint arg1 );
void glShaderOp2EXT( GLenum[Main] op, GLuint res, GLuint arg1, GLuint arg2 );
void glShaderOp3EXT( GLenum[Main] op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3 );
void glSwizzleEXT( GLuint res, GLuint in, GLenum[Main] outX, GLenum[Main] outY, GLenum[Main] outZ, GLenum[Main] outW );
void glWriteMaskEXT( GLuint res, GLuint in, GLenum[Main] outX, GLenum[Main] outY, GLenum[Main] outZ, GLenum[Main] outW );
void glInsertComponentEXT( GLuint res, GLuint src, GLuint num );
void glExtractComponentEXT( GLuint res, GLuint src, GLuint num );

GLuint glGenSymbolsEXT( GLenum[Main] datatype, GLenum[Main] storagetype, GLenum[Main] range, GLuint components );
void glSetInvariantEXT( GLuint id, GLenum[Main] type, const GLvoid * addr );
void glSetLocalConstantEXT( GLuint id, GLenum[Main] type, const GLvoid * addr );

void glVariantbvEXT (GLuint id, const GLbyte *addr);
void glVariantsvEXT (GLuint id, const GLshort *addr);
void glVariantivEXT (GLuint id, const GLint *addr);
void glVariantfvEXT (GLuint id, const GLfloat *addr);
void glVariantdvEXT (GLuint id, const GLdouble *addr);
void glVariantubvEXT (GLuint id, const GLubyte *addr);
void glVariantusvEXT (GLuint id, const GLushort *addr);
void glVariantuivEXT (GLuint id, const GLuint *addr);

void glVariantPointerEXT( GLuint id, GLenum[Main] type, GLuint stride, const GLvoid * addr );
void glEnableVariantClientStateEXT( GLuint id);
void glDisableVariantClientStateEXT( GLuint id);

GLuint glBindLightParameterEXT( GLenum[Main] light, GLenum[Main] value);
GLuint glBindMaterialParameterEXT( GLenum[Main] face, GLenum[Main] value);
GLuint glBindTexGenParameterEXT( GLenum[Main] unit, GLenum[Main] coord, GLenum[Main] value);
GLuint glBindTextureUnitParameterEXT( GLenum[Main] unit, GLenum[Main] value);
GLuint glBindParameterEXT( GLenum[Main] value);

GLboolean glIsVariantEnabledEXT( GLuint id, GLenum[Main] cap);

void glGetVariantBooleanvEXT( GLuint id, GLenum[Main] value, GLboolean *data);
void glGetVariantIntegervEXT( GLuint id, GLenum[Main] value, GLint *data);
void glGetVariantFloatvEXT( GLuint id, GLenum[Main] value, GLfloat *data);
void glGetVariantPointervEXT( GLuint id, GLenum[Main] value, void **data);
void glGetInvariantBooleanvEXT( GLuint id, GLenum[Main] value, GLboolean *data);
void glGetInvariantIntegervEXT( GLuint id, GLenum[Main] value, GLint *data);
void glGetInvariantFloatvEXT( GLuint id, GLenum[Main] value, GLfloat *data);
void glGetLocalConstantBooleanvEXT( GLuint id, GLenum[Main] value, GLboolean *data);
void glGetLocalConstantIntegervEXT( GLuint id, GLenum[Main] value, GLint *data);
void glGetLocalConstantFloatvEXT( GLuint id, GLenum[Main] value, GLfloat *data);

