#include <stdio.h>
#include <string.h>
#include "Debug.h"
#include "Open.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  /*
  struct PCX_HEADER {
    byte manufacturer;
    byte version;
    byte encoding;
    byte bits_per_pixel;
    word xmin;
    word ymin;
    word xmax;
    word ymax;
    word hdpi;
    word vdpi;
    byte colormap[48];
    byte reserved;
    byte num_planes;
    word bytes_per_line;
    word palette_info;
    word h_screen_size;
    word v_screen_size;
    byte filler[54];
  };
  */


  //////////////////////////////////////////////////////////////////////////////

  bool ReadScanline(File* file, int scansize, byte* scanline) {
    byte* out = scanline;
    while (out - scanline < scansize) {

      // read a byte!
      byte data;
      if (file->read(&data, 1) != 1) {
        return false;
      }

      if ((data & 0xC0) != 0xC0) {  // non RLE
        *out++ = data;
      } else {                      // RLE

        // read the repeated byte
        int numbytes = data & 0x3F;
        if (file->read(&data, 1) != 1) {
          return false;
        }

        while (numbytes-- && out - scanline < scansize) {
          *out++ = data;
        }

      }
    }

    return true;
  }

  //////////////////////////////////////////////////////////////////////////////

  Image* OpenPCX(File* file) {
    COR_GUARD("OpenPCX");

    // read the header block
    byte pcx_header[128];
    int read = file->read(pcx_header, 128);
    if (read != 128) {
      return 0;
    }

    // parse the header...
    //int manufacturer   = pcx_header[0];
    //int version        = pcx_header[1];
    int encoding       = pcx_header[2];
    int bpp            = pcx_header[3];
    int xmin           = read16_le(pcx_header + 4);
    int ymin           = read16_le(pcx_header + 6);
    int xmax           = read16_le(pcx_header + 8);
    int ymax           = read16_le(pcx_header + 10);
    int num_planes     = pcx_header[65];
    int bytes_per_line = read16_le(pcx_header + 66);


    // verify the header

    // we only support RLE encoding
    if (encoding != 1) {
      COR_LOG("Unsupported encoding");
      return 0;
    }

    COR_IF_DEBUG {
      char str[100];
      sprintf(str, "bits per pixel - %d", bpp);
      COR_LOG(str);
      sprintf(str, "bytes per line - %d", bytes_per_line);
      COR_LOG(str);
    }

    // we only support 8 bits per pixel
    if (bpp != 8) {
      COR_LOG("Unsupported bpp");
      return 0;
    }

    // create the image structure
    int width  = xmax - xmin + 1;
    int height = ymax - ymin + 1;

    auto_array<byte> scanline(new byte[bytes_per_line]);
    auto_array<byte> pixels(new byte[width * height * 3]);

    // decode the pixel data

    if (num_planes == 1) {               // 256 colors

      auto_array<RGB> palette(new RGB[256]);
      auto_array<byte> image(new byte[width * height]);

      // read all of the scanlines
      for (int iy = 0; iy < height; ++iy) {
        if (!ReadScanline(file, bytes_per_line, scanline)) {
          COR_LOG("Failure reading scanline");
          return 0;
        }
        memcpy((byte*)image + iy * width, scanline, width);
      }

      // seek back from the end 769 bytes
      if (!file->seek(-769, File::END)) {
        COR_LOG("Failure seeking to palette");
        return 0;
      }

      // do we have a palette?
      byte has_palette;
      if (file->read(&has_palette, 1) != 1 || has_palette != 12) {
        COR_LOG("Failure testing for existence of palette");
        return 0;
      }

      // read palette
      if (file->read(palette, 3 * 256) != 3 * 256) {
        COR_LOG("Failure reading palette");
        return 0;
      }

      return new SimpleImage(width, height, PF_I8, image.release(),
                             (byte*)palette.release(), 256, PF_R8G8B8);

    } else if (num_planes == 3) { // 24-bit color

      auto_array<byte> scanline(new byte[3 * bytes_per_line]);

      byte* out = pixels;
      for (int iy = 0; iy < height; ++iy) {
        if (!ReadScanline(file, 3 * bytes_per_line, scanline)) {
          COR_LOG("Failure reading scanline");
          return 0;
        }

        byte* r = scanline;
        byte* g = scanline + bytes_per_line;
        byte* b = scanline + bytes_per_line * 2;
        for (int ix = 0; ix < width; ++ix) {
          *out++ = *r++;
          *out++ = *g++;
          *out++ = *b++;
        }
      }

      return new SimpleImage(width, height, PF_R8G8B8, pixels.release());

    } else {
      COR_LOG("Unknown number of planes");
      return 0;
    }
  }

  //////////////////////////////////////////////////////////////////////////////

}
