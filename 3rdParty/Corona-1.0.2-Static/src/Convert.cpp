/**
 * @file
 * @todo  allow conversions from direct color images to
 *        palettized images
 */
#include <map>
#include <utility>
#include <string.h>
#include "corona.h"
#include "Debug.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  Image* ExpandPalette(Image* image) {
    COR_GUARD("ExpandPalette()");

    // assert isPalettized(image->getFormat())

    const int width                  = image->getWidth();
    const int height                 = image->getHeight();
    const byte* in                   = (byte*)image->getPixels();
    const PixelFormat palette_format = image->getPaletteFormat();
    const int pixel_size             = GetPixelSize(palette_format);
    const byte* palette              = (byte*)image->getPalette();

    byte* pixels = new byte[width * height * pixel_size];
    byte* out = pixels;
    for (int i = 0; i < width * height; ++i) {
      memcpy(out, palette + (*in) * pixel_size, pixel_size);
      out += pixel_size;
      ++in;
    }
    delete image;
    return new SimpleImage(width, height, palette_format, pixels);
  }


  struct FormatDesc {
    FormatDesc(int r, int g, int b, int a, bool ha) {
      r_shift = r;
      g_shift = g;
      b_shift = b;
      a_shift = a;
      has_alpha = ha;
    }

    // shifts are in bytes from the right
    // In the case of RGBA, r_shift is 0, g_shift is 1, ...
    int r_shift;
    int g_shift;
    int b_shift;
    int a_shift;
    bool has_alpha;
  };


  #define DEFINE_DESC(format, desc)          \
    case format: {                           \
      COR_LOG(#format);                      \
      static FormatDesc format##_desc desc;  \
      return &format##_desc;                 \
    }

  FormatDesc* GetDescription(PixelFormat format) {
    // assert isDirect(image->getFormat())

    switch (format) {
      DEFINE_DESC(PF_R8G8B8A8, (0, 1, 2, 3, true));
      DEFINE_DESC(PF_R8G8B8,   (0, 1, 2, 0, false));
      DEFINE_DESC(PF_B8G8R8A8, (2, 1, 0, 3, true));
      DEFINE_DESC(PF_B8G8R8,   (2, 1, 0, 0, false));
      default: return 0;
    }
  }


  bool ConvertPixels(byte* out, PixelFormat out_format,
                     const byte* in,  PixelFormat in_format,
                     int pixel_count)
  {
    const FormatDesc* out_desc = GetDescription(out_format);
    const FormatDesc* in_desc  = GetDescription(in_format);
    if (!out_desc || !in_desc) {
      return false;
    }

    const int out_size = GetPixelSize(out_format);
    const int in_size  = GetPixelSize(in_format);

    for (int i = 0; i < pixel_count; ++i) {
      out[out_desc->r_shift] = in[in_desc->r_shift];
      out[out_desc->g_shift] = in[in_desc->g_shift];
      out[out_desc->b_shift] = in[in_desc->b_shift];

      if (out_desc->has_alpha) {
        if (in_desc->has_alpha) {
          out[out_desc->a_shift] = in[in_desc->a_shift];
        } else {
          out[out_desc->a_shift] = 255;
        }
      }

      in  += in_size;
      out += out_size;
    }

    return true;
  }


  Image* DirectConversion(Image* image, PixelFormat target_format) {
    COR_GUARD("DirectConversion()");

    // assert isDirect(image->getFormat())

    const int width                 = image->getWidth();
    const int height                = image->getHeight();
    const PixelFormat source_format = image->getFormat();
    const byte* in                  = (byte*)image->getPixels();

    if (source_format == target_format) {
        return image;
    }

    const int target_size = GetPixelSize(target_format);
    byte* out_pixels = new byte[width * height * target_size];
    if (!ConvertPixels(out_pixels, target_format,
                       in, source_format,
                       width * height))
    {
      delete[] out_pixels;
      delete image;
      return 0;
    }

    delete image;
    return new SimpleImage(width, height, target_format, out_pixels);
  }


  namespace hidden {

    COR_EXPORT(Image*) CorConvertImage(
      Image* image,
      PixelFormat target_format)
    {
      COR_GUARD("CorConvertImage");

      // if we don't have an image, user doesn't care about format, or
      // the formats match, don't do any conversion.
      if (!image ||
          target_format == PF_DONTCARE ||
          target_format == image->getFormat())
      {
        return image;
      }

      COR_LOG("Doing the conversion...");

      // if we have a palettized image, convert it to a direct color
      // image and then convert that
      if (IsPalettized(image->getFormat())) {
        image = ExpandPalette(image);
      }

      return DirectConversion(image, target_format);
    }


    COR_EXPORT(Image*) CorConvertPalette(
      Image* image,
      PixelFormat palette_format)
    {
      // do we need to convert?
      if (!image ||
          palette_format == PF_DONTCARE ||
          image->getPaletteFormat() == palette_format)
      {
        return image;
      }

      // do we have invalid data?
      if (!IsPalettized(image->getFormat()) ||
          !IsDirect(palette_format))
      {
        delete image;
        return 0;
      }

      const int width  = image->getWidth();
      const int height = image->getHeight();
      const PixelFormat format = image->getFormat();
      const int palette_size = image->getPaletteSize();

      // the palette indices don't change, so just make a copy
      const int image_size = width * height * GetPixelSize(format);
      byte* pixels = new byte[image_size];
      memcpy(pixels, image->getPixels(), image_size);

      byte* new_palette = new byte[
        palette_size * GetPixelSize(palette_format)];

      if (!ConvertPixels(new_palette, palette_format,
                         (byte*)image->getPalette(), image->getPaletteFormat(),
                         palette_size))
      {
        delete image;
        delete[] pixels;
        delete[] new_palette;
        return 0;
      }

      delete image;
      return new SimpleImage(
        width, height, format, pixels,
        new_palette, palette_size, palette_format);
    }

    COR_EXPORT(Image*) CorFlipImage(
      Image* image,
      int coordinate_axis)
    {
      COR_GUARD("CorFlipImage");

      // if we don't have an image, don't flip.
      if (!image) {
        return 0;
      }

      COR_LOG("Doing the flip...");

      const int width                = image->getWidth();
      const int height               = image->getHeight();
      byte* pixels                   = (byte*)image->getPixels();
      const PixelFormat pixel_format = image->getFormat();
      const int pixel_size           = GetPixelSize(pixel_format);

      // flip about the X axis
      if (coordinate_axis & CA_X) {

        byte* row = new byte[width * pixel_size];
        for (int h = 0; h < height / 2; ++h) {
          byte* top = pixels + h                * width * pixel_size;
          byte* bot = pixels + (height - h - 1) * width * pixel_size;
          memcpy(row, top, width * pixel_size);
          memcpy(top, bot, width * pixel_size);
          memcpy(bot, row, width * pixel_size);
        }
        delete[] row;

      }

      // flip about the Y axis
      if (coordinate_axis & CA_Y) {

        for (int h = 0; h < height; ++h) {
          byte* row = pixels + h * width * pixel_size;
          for (int w = 0; w < width / 2; ++w) {
            byte* left  = row + w               * pixel_size;
            byte* right = row + (width - w - 1) * pixel_size;
            for (int b = 0; b < pixel_size; ++b) {
              std::swap(left[b], right[b]);
            }
          }
        }

      }

      return image;
    }
  }

}
