#define GLI_INCLUDE_GL_EXT_LIGHT_TEXTURE

enum Main {

  GL_FRAGMENT_MATERIAL_EXT                = 0x8349,
  GL_FRAGMENT_NORMAL_EXT                  = 0x834A,
  GL_FRAGMENT_COLOR_EXT                   = 0x834C,
  GL_ATTENUATION_EXT                      = 0x834D,
  GL_SHADOW_ATTENUATION_EXT               = 0x834E,
  GL_TEXTURE_APPLICATION_MODE_EXT         = 0x834F,
  GL_TEXTURE_LIGHT_EXT                    = 0x8350,
  GL_TEXTURE_MATERIAL_FACE_EXT            = 0x8351,
  GL_TEXTURE_MATERIAL_PARAMETER_EXT       = 0x8352,
  //GL_FRAGMENT_DEPTH_EXT                   = 0x8452,

};

void glApplyTextureEXT(GLenum[Main] mode);
void glTextureLightEXT(GLenum[Main] pname);
void glTextureMaterialEXT(GLenum[Main] face, GLenum[Main] mode);
