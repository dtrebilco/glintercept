#include <memory>
#include <string>
#include <vector>
#include <string.h>
#include <ctype.h>
#include "corona.h"
#include "MemoryFile.h"
#include "Open.h"
#include "Save.h"
#include "SimpleImage.h"


namespace corona {
  namespace hidden {

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(const char*) CorGetVersion() {
      return "1.0.2";
    }

    ///////////////////////////////////////////////////////////////////////////

    class FFDImpl : public FileFormatDesc {
    public:
      FFDImpl(FileFormat format, const char* description, const char* exts) {
        m_format = format;
        m_description = description;

        const char* ext = exts;
        while (*ext) {
          m_extensions.push_back(ext);
          ext += strlen(ext) + 1;
        }
      }

      FileFormat getFormat()             { return m_format;                }
      const char* getDescription()       { return m_description.c_str();   }
      size_t getExtensionCount()         { return m_extensions.size();     }
      const char* getExtension(size_t i) { return m_extensions[i].c_str(); }

    private:
      FileFormat m_format;
      std::string m_description;
      std::vector<std::string> m_extensions;
    };

    FFDImpl ffPNG (FF_PNG,  "PNG Files",  "png\0");
    FFDImpl ffJPEG(FF_JPEG, "JPEG Files", "jpeg\0jpg\0");
    FFDImpl ffPCX (FF_PCX,  "PCX Files",  "pcx\0");
    FFDImpl ffBMP (FF_BMP,  "BMP Files",  "bmp\0");
    FFDImpl ffTGA (FF_TGA,  "TGA Files",  "tga\0");
    FFDImpl ffGIF (FF_GIF,  "GIF Files",  "gif\0");

    const int MAX_FORMAT_COUNT = 64;
    FileFormatDesc** g_read_formats  = 0;
    FileFormatDesc** g_write_formats = 0;
    FileFormatDesc* g_read_array[MAX_FORMAT_COUNT + 1]  = {0};
    FileFormatDesc* g_write_array[MAX_FORMAT_COUNT + 1] = {0};


    COR_EXPORT(FileFormatDesc**) CorGetSupportedReadFormats() {
      if (!g_read_formats) {
        g_read_formats = g_read_array;
        FileFormatDesc** f = g_read_formats;
#ifndef NO_PNG
        *f++ = &ffPNG;
#endif
#ifndef NO_JPEG
        *f++ = &ffJPEG;
#endif
        *f++ = &ffPCX;
        *f++ = &ffBMP;
        *f++ = &ffTGA;
        *f++ = &ffGIF;
      }
      return g_read_formats;
    }


    COR_EXPORT(FileFormatDesc**) CorGetSupportedWriteFormats() {
      if (!g_write_formats) {
        g_write_formats = g_write_array;
        FileFormatDesc** f = g_write_formats;
#ifndef NO_PNG
	*f++ = &ffPNG;
#endif
        *f++ = &ffTGA;
      }
      return g_write_formats;
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*) CorCreateImage(
      int width,
      int height,
      PixelFormat format)
    {
      return CreateImage(width, height, format, 0);
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*) CorCreateImageWithPixels(
      int width,
      int height,
      PixelFormat format,
      void* pixels)
    {
      // this function only supports creation of non-palettized images
      if (!IsDirect(format)) {
        return 0;
      }

      int size = width * height * GetPixelSize(format);
      byte* p = new byte[size];
      if (pixels) {
        memcpy(p, pixels, size);
      } else {
        memset(p, 0, size);
      }
      return new SimpleImage(width, height, format, p);
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*) CorCreatePalettizedImage(
      int width,
      int height,
      PixelFormat format,
      int palette_size,
      PixelFormat palette_format)
    {
      // only support creation of palettized images
      if (!IsPalettized(format) || !IsDirect(palette_format)) {
        return 0;
      }

      // make sure the palette is the right size
      if (palette_size != GetPaletteSize(format)) {
        return 0;
      }

      int size = width * height * GetPixelSize(format);
      byte* pixels = new byte[size];
      memset(pixels, 0, size);

      int palette_bytes = palette_size * GetPixelSize(palette_format);
      byte* palette = new byte[palette_bytes];
      memset(palette, 0, palette_bytes);

      return new SimpleImage(width, height, format, pixels,
                             palette, palette_size, palette_format);      
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*) CorCloneImage(
      Image* source,
      PixelFormat format)
    {
      if (!source) {
        // we need an image to clone :)
        return 0;
      }

      const int width = source->getWidth();
      const int height = source->getHeight();
      const PixelFormat source_format = source->getFormat();

      const int source_pixel_size = GetPixelSize(source_format);
      if (source_pixel_size == 0) {
        // unknown pixel size?
        return 0;
      }

      // duplicate the image
      int image_size = width * height * source_pixel_size;
      byte* pixels = new byte[image_size];
      memcpy(pixels, source->getPixels(), image_size);
      
      if (IsPalettized(source_format)) {
        // clone palette
        int palette_size = source->getPaletteSize();
        PixelFormat palette_format = source->getPaletteFormat();
        int palette_bytes = palette_size * GetPixelSize(palette_format);
        byte* palette = new byte[palette_bytes];
        memcpy(palette, source->getPalette(), palette_bytes);
        Image* image = new SimpleImage(width, height, source_format, pixels,
                                       palette, palette_size, palette_format);
        return ConvertImage(image, format);
      }

      Image* image = new SimpleImage(width, height, source_format, pixels);
      return ConvertImage(image, format);
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*) CorOpenImage(
      const char* filename,
      FileFormat file_format)
    {
      if (!filename) {
        return 0;
      }

      std::auto_ptr<File> file(OpenFile(filename, false));
      return CorOpenImageFromFile(file.get(), file_format);
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*) CorOpenImageFromFile(
      File* file,
      FileFormat file_format)
    {
      if (!file) {
        return 0;
      }

#define TRY_TYPE(type)                                 \
  {                                                    \
    Image* image = CorOpenImageFromFile(file, (type)); \
    if (image) { return image; }                       \
  }

      file->seek(0, File::BEGIN);
      switch (file_format) {
        case FF_AUTODETECT: {
#ifndef NO_PNG
          TRY_TYPE(FF_PNG);
#endif
#ifndef NO_JPEG
          TRY_TYPE(FF_JPEG);
#endif
          TRY_TYPE(FF_PCX);
          TRY_TYPE(FF_BMP);
          TRY_TYPE(FF_TGA);
          TRY_TYPE(FF_GIF);
          return 0;
        }

//Don't include image opening (not needed for GLI)
#ifndef GLI_CHANGES          

#ifndef NO_PNG
        case FF_PNG:  return OpenPNG(file);
#endif
#ifndef NO_JPEG
        case FF_JPEG: return OpenJPEG(file);
#endif
        case FF_PCX:  return OpenPCX(file);
        case FF_BMP:  return OpenBMP(file);
        case FF_TGA:  return OpenTGA(file);
        case FF_GIF:  return OpenGIF(file);

#endif //!GLI_CHANGES          

        default:      return 0;
      }
    }

    ///////////////////////////////////////////////////////////////////////////

    int strcmp_ci(const char* a, const char* b) {
      while (*a && *b) {
        const int diff = tolower(*a) - tolower(*b);
        if (diff != 0) {
          return diff;
        }
        ++a;
        ++b;
      }
      return tolower(*a) - tolower(*b);
    }

    bool ends_with(const char* str, const char* ext) {
      const int str_len = strlen(str);
      const int ext_len = strlen(ext);
      return (str_len >= ext_len &&
              strcmp_ci(str + str_len - ext_len, ext) == 0);
    }

    COR_EXPORT(bool) CorSaveImage(
      const char* filename,
      FileFormat file_format,
      Image* image)
    {
      if (!filename) {
        return false;
      }

      if (file_format == FF_AUTODETECT) {
        if (ends_with(filename, ".png")) {
          file_format = FF_PNG;
        } else if (ends_with(filename, ".tga")) {
          file_format = FF_TGA;

#ifdef GLI_CHANGES
        } else if (ends_with(filename, ".jpg") || ends_with(filename, ".jpeg")) {
          file_format = FF_JPEG;
#endif //GLI_CHANGES
        
        } else {
          return false;
        }
      }

      std::auto_ptr<File> file(OpenFile(filename, true));
      return CorSaveImageToFile(file.get(), file_format, image);
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(bool) CorSaveImageToFile(
      File* file,
      FileFormat file_format,
      Image* image)
    {
      if (!file || !image) {
        return false;
      }

      switch (file_format) {
#ifndef NO_PNG
        case FF_PNG:  return SavePNG(file, image);
#endif

#ifdef GLI_CHANGES
        case FF_JPEG: return SaveJPEG(file, image);
#else  //GLI_CHANGES
        case FF_JPEG: return false;
#endif //GLI_CHANGES

        case FF_PCX:  return false;
        case FF_BMP:  return false;
        case FF_TGA:  return SaveTGA(file, image);
        case FF_GIF:  return false;
        default:      return false;
      }
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(int) CorGetPixelSize(PixelFormat format) {
      switch (format) {
        case PF_R8G8B8A8: return 4;
        case PF_R8G8B8:   return 3;
        case PF_B8G8R8A8: return 4;
        case PF_B8G8R8:   return 3;
        case PF_I8:       return 1;
        default:          return 0;
      }
    }

    ///////////////////////////////////////////////////////////////////////////

  }
}
