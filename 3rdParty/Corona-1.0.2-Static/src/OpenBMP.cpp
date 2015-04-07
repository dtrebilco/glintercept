// format information gleaned from
//   http://www.daubnet.com/formats/BMP.html
// and
//   http://www.edm2.com/0107/os2bmp.html
//
// If you have Visual Studio.NET:
//   ms-help://MS.VSCC/MS.MSDNVS/gdi/bitmaps_7c36.htm

#include <string.h>
#include "corona.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  struct Header {
    bool os2;

    int file_size;
    int data_offset;
    int width;
    int height;
    int bpp;
    int compression;

    int pitch;  // number of bytes in each scanline
    int image_size;

    auto_array<BGR> palette;
    int palette_size;

    // for bitfield specification...
    //                               /*- 16-bit only -*/
    u32 bf_red_mask,   bf_red_shift,   bf_red_rshift;
    u32 bf_green_mask, bf_green_shift, bf_green_rshift;
    u32 bf_blue_mask,  bf_blue_shift,  bf_blue_rshift;
  };


  bool   ReadHeader(File* file, Header& h);
  bool   ReadInfoHeader(File* file, Header& h);
  bool   ReadPalette(File* file, Header& h);
  Image* DecodeBitmap(File* file, const Header& h);

  
  Image* OpenBMP(File* file) {
    Header h;
    if (ReadHeader(file, h) &&
        ReadInfoHeader(file, h) &&
        ReadPalette(file, h)) {

      return DecodeBitmap(file, h);

    } else {

      return 0;

    }
  }


  bool ReadHeader(File* file, Header& h) {
    byte header[14];
    if (file->read(header, 14) != 14) {
      return false;
    }

    // check signature
    if (header[0] != 'B' || header[1] != 'M') {
      return false;
    }

    h.file_size   = read32_le(header + 2);
    h.data_offset = read32_le(header + 10);
    return true;
  }


  bool ReadInfoHeader(File* file, Header& h) {

    const int HEADER_READ_SIZE = 24;

    // read the only part of the header we need
    byte header[HEADER_READ_SIZE];
    if (file->read(header, HEADER_READ_SIZE) != HEADER_READ_SIZE) {
      return false;
    }

    int size = read32_le(header + 0);
    int width;
    int height;
    int planes;
    int bpp;
    int compression;
    int image_size;

    if (size < 40) {  // assume OS/2 bitmap
      if (size < 12) {
        return false;
      }

      h.os2 = true;
      width  = read16_le(header + 4);
      height = read16_le(header + 6);
      planes = read16_le(header + 8);
      bpp    = read16_le(header + 10);
      compression = 0;
      image_size = 0;
      
    } else {

      h.os2 = false;
      width       = read32_le(header + 4);
      height      = read32_le(header + 8);
      planes      = read16_le(header + 12);
      bpp         = read16_le(header + 14);
      compression = read32_le(header + 16);
      image_size  = read32_le(header + 20);

    }
    
    // sanity check the info header
    if (planes != 1) {
      return false;
    }

    // adjust image_size
    // (if compression == 0 or 3, manually calculate image size)
    int line_size = 0;
    if (compression == 0 || compression == 3) {
      line_size = (width * bpp + 7) / 8;
      line_size = (line_size + 3) / 4 * 4;  // 32-bit-aligned
      image_size = line_size * height;
    }

    h.width       = width;
    h.height      = height;
    h.bpp         = bpp;
    h.compression = compression;
    h.pitch       = line_size;
    h.image_size  = image_size;

    // jump forward (backward in the OS/2 case :) to the palette data
    file->seek(size - HEADER_READ_SIZE, File::CURRENT);

    return true;
  }

  // count the number of consecutive zeroes on the right side of a
  // binary number
  // 0x00F0 will return 4
  int count_right_zeroes(u32 n) {
    int total = 0;
    u32 c = 1;
    while ((total < 32) && ((n & c) == 0)) {
      c <<= 1;
      ++total;
    }
    return total;
  }

  // count the number of ones in a binary number
  // 0x00F1 will return 5
  int count_ones(u32 n) {
    int total = 0;
    u32 c = 1;
    for (int i = 0; i < 32; ++i) {
      if (n & c) {
        ++total;
      }
      c <<= 1;
    }
    return total;
  }

  bool ReadPalette(File* file, Header& h) {

    // initialize bit masks/shifts...  just in case
    h.bf_red_mask   = h.bf_red_shift   = h.bf_red_rshift   = 0;
    h.bf_green_mask = h.bf_green_shift = h.bf_green_rshift = 0;
    h.bf_blue_mask  = h.bf_blue_shift  = h.bf_blue_rshift  = 0;

    // if we're not a palettized image, don't even read a palette
    if (h.bpp > 8) {
      h.palette_size = 0;

      // do we have bitfields?
      if (h.compression == 3) {

        auto_array<byte> bitfields(new byte[12]);
        if (file->read(bitfields, 12) != 12) {
          return false;
        }

        h.bf_red_mask   = read32_le((byte*)bitfields);
        h.bf_green_mask = read32_le((byte*)bitfields + 4);
        h.bf_blue_mask  = read32_le((byte*)bitfields + 8);

        // calculate shifts
        h.bf_red_shift    = count_right_zeroes(h.bf_red_mask);
        h.bf_green_shift  = count_right_zeroes(h.bf_green_mask);
        h.bf_blue_shift   = count_right_zeroes(h.bf_blue_mask);
        h.bf_red_rshift   = 8 - count_ones(h.bf_red_mask);
        h.bf_green_rshift = 8 - count_ones(h.bf_green_mask);
        h.bf_blue_rshift  = 8 - count_ones(h.bf_blue_mask);

      // otherwise, set default bitfield entries
      } else {

        if (h.bpp == 16) {

          h.bf_red_mask     = 0x7C00;
          h.bf_red_shift    = 10;
          h.bf_red_rshift   = 3;

          h.bf_green_mask   = 0x03E0;
          h.bf_green_shift  = 5;
          h.bf_green_rshift = 3;

          h.bf_blue_mask    = 0x001F;
          h.bf_blue_shift   = 0;
          h.bf_blue_rshift  = 3;

        } else if (h.bpp == 32) {

          // these don't need any rshift
          h.bf_red_mask   = 0x00FF0000; h.bf_red_shift   = 16;
          h.bf_green_mask = 0x0000FF00; h.bf_green_shift = 8;
          h.bf_blue_mask  = 0x000000FF; h.bf_blue_shift  = 0;

        }
      }

      return true;
    }

    if (h.bpp <= 8) {
      h.palette_size = 1 << h.bpp;
    } else {
      h.palette_size = 0;
      return true;
    }
    h.palette = new BGR[h.palette_size];

    // read the BMP color table
    const int buffer_size = h.palette_size * (h.os2 ? 3 : 4);
    auto_array<byte> buffer(new byte[buffer_size]);
    if (file->read(buffer, buffer_size) != buffer_size) {
      return false;
    }

    byte* in = buffer;
    BGR* out = h.palette;
    for (int i = 0; i < h.palette_size; ++i) {
      out->blue  = *in++;
      out->green = *in++;
      out->red   = *in++;
      if (!h.os2) {
        ++in;  // skip alpha
      }
      ++out;
    }

    return true;
  }


  bool advance(int& x, int& y, const Header& h) {
    if (++x >= h.width) {
      x = 0;
      if (++y >= h.height) {
        return false;
      }
    }
    return true;
  }

  Image* ReadBitmap1(const byte* raster_data, const Header& h) {
    auto_array<byte> pixels(new byte[h.width * h.height]);
    
    auto_array<BGR> palette(new BGR[256]);
    memset(palette, 0, 256 * sizeof(BGR));
    memcpy(palette, h.palette, h.palette_size * sizeof(BGR));

    for (int i = 0; i < h.height; ++i) {
      const byte* in = raster_data + i * h.pitch;
      byte* out = pixels + (h.height - i - 1) * h.width;

      int mask = 128;
      for (int j = 0; j < h.width; ++j) {
        *out++ = (*in & mask) > 0;

        mask >>= 1;
        if (mask == 0) {
          ++in;
          mask = 128;
        }
      }
    }

    return new SimpleImage(h.width, h.height, PF_I8, pixels.release(),
                           (byte*)palette.release(), 256, PF_B8G8R8);
  }

  Image* ReadBitmap4(const byte* raster_data, const Header& h) {
    auto_array<byte> pixels(new byte[h.width * h.height]);
    
    auto_array<BGR> palette(new BGR[256]);
    memset(palette, 0, 256 * sizeof(BGR));
    memcpy(palette, h.palette, h.palette_size * sizeof(BGR));

    for (int i = 0; i < h.height; ++i) {
      const byte* in = raster_data + i * h.pitch;
      byte* out = pixels + (h.height - i - 1) * h.width;

      for (int j = 0; j < h.width / 2; ++j) {
        *out++ = (*in >> 4);
        *out++ = (*in & 0x0F);

        ++in;
      }

      if (h.width % 2) {
        *out++ = (*in >> 4);
      }
    }

    return new SimpleImage(h.width, h.height, PF_I8, pixels.release(),
                           (byte*)palette.release(), 256, PF_B8G8R8);
  }

  Image* ReadBitmapRLE4(const byte* raster_data, const Header& h) {
    auto_array<byte> pixels(new byte[h.width * h.height]);
    
    auto_array<BGR> palette(new BGR[256]);
    memset(palette, 0, 256 * sizeof(BGR));
    memcpy(palette, h.palette, h.palette_size * sizeof(BGR));

    // by default, we have an empty bitmap
    memset(pixels, 0, h.width * h.height);

    // we read the image from the bottom down, and then flip it when
    // we're done
    int x = 0;
    int y = 0;

    const byte* in = raster_data;
    while (in - raster_data < h.image_size - 1) {
      byte n = *in++;
      byte c = *in++;

      if (n == 0) {  // escape code

        if (c == 0) {         // end of line
          x = 0;

          //++y;  // XXXaegis uhhh...  uhhh...  :)  it works this way...

          if (y >= h.height) {
            // did we go too far?
            break;
          }
        } else if (c == 1) {  // end of bitmap
          break;
        } else if (c == 2) {  // delta

          // do we have enough space?
          if (in - raster_data >= h.image_size - 1) {
            break;
          }

          // I have no idea how I'm supposed to do this...
          // Let's take a guess!
          int dx = *in++;
          int dy = *in++;
          x = (x + dx) % h.width;
          y += dy + (x + dx) / h.width;
          if (y >= h.height) {
            // if we went too far, stop now
            break;
          }

        } else {              // read uncompressed

          // the input raster data is padded on DWORD boundaries
          // c == num_pixels
          int num_bytes = (c + 3) / 4 * 2;
          
          // make sure we have enough space
          if (in - raster_data > h.image_size - num_bytes) {
            break;
          }

          // nasty decoding loop...
          int i = 0;
          int j = 0;
          while (true) {
            byte l = (in[j] & 0xF0) >> 4;
            byte r = (in[j] & 0x0F);
            ++j;

            pixels[y * h.width + x] = l;
            if (!advance(x, y, h) || ++i >= c) {
              break;
            }

            pixels[y * h.width + x] = r;
            if (!advance(x, y, h) || ++i >= c) {
              break;
            }
          }
          // make SURE we move forward the right number of bytes
          in += num_bytes;
        }

      } else {

        // a less nasty decoding loop...
        byte lc = (c & 0xF0) >> 4;
        byte rc = c & 0x0F;

        int i = 0;
        while (true) {
          pixels[y * h.width + x] = lc;
          if (!advance(x, y, h) || ++i >= n) {
            break;
          }

          pixels[y * h.width + x] = rc;
          if (!advance(x, y, h) || ++i >= n) {
            break;
          }
        }

      } // end if
    } // end while

    // flippy flippy!
    int pitch = h.width;
    auto_array<byte> row(new byte[pitch]);
    for (int i = 0; i < h.height / 2; ++i) {
      int j = h.height - i - 1;
      memcpy((byte*)row,         pixels + i * pitch, pitch);
      memcpy(pixels + i * pitch, pixels + j * pitch, pitch);
      memcpy(pixels + j * pitch, (byte*)row,         pitch);
    }

    return new SimpleImage(h.width, h.height, PF_I8, pixels.release(),
                           (byte*)palette.release(), 256, PF_B8G8R8);
  }

  Image* ReadBitmap8(const byte* raster_data, const Header& h) {
    auto_array<byte> pixels(new byte[h.width * h.height]);
    
    auto_array<BGR> palette(new BGR[256]);
    memset(palette, 0, 256 * sizeof(BGR));
    memcpy(palette, h.palette, h.palette_size * sizeof(BGR));

    for (int i = 0; i < h.height; ++i) {
      const byte* in = raster_data + i * h.pitch;
      byte* out = pixels + (h.height - i - 1) * h.width;

      for (int j = 0; j < h.width; ++j) {
        *out++ = *in++;
      }
    }

    return new SimpleImage(h.width, h.height, PF_I8, pixels.release(),
                           (byte*)palette.release(), 256, PF_B8G8R8);
  }

  Image* ReadBitmapRLE8(const byte* raster_data, const Header& h) {
    auto_array<byte> pixels(new byte[h.width * h.height]);
    
    auto_array<BGR> palette(new BGR[256]);
    memset(palette, 0, 256 * sizeof(BGR));
    memcpy(palette, h.palette, h.palette_size * sizeof(BGR));

    // by default, we have an empty bitmap
    memset(pixels, 0, h.width * h.height);

    // we read the image from the bottom down, and then flip it when
    // we're done
    int x = 0;
    int y = 0;

    const byte* in = raster_data;
    while (in - raster_data < h.image_size - 1) {
      byte n = *in++;
      byte c = *in++;

      if (n == 0) {  // escape code

        if (c == 0) {         // end of line
          x = 0;

          //++y;  // XXXaegis uhhh...  uhhh...  :)  it works this way...

          if (y >= h.height) {
            // did we go too far?
            break;
          }
        } else if (c == 1) {  // end of bitmap
          break;
        } else if (c == 2) {  // delta

          // do we have enough space?
          if (in - raster_data >= h.image_size - 1) {
            break;
          }

          // I have no idea how I'm supposed to do this...
          // Let's take a guess!
          int dx = *in++;
          int dy = *in++;
          x = (x + dx) % h.width;
          y += dy + (x + dx) / h.width;
          if (y >= h.height) {
            // if we went too far, stop now
            break;
          }

        } else {              // read uncompressed

          // c == num_pixels
          int num_bytes = (c + 1) / 2 * 2;
          
          // make sure we have enough space
          if (in - raster_data > h.image_size - num_bytes) {
            break;
          }

          // decoding loop...
          int i = 0;
          int j = 0;
          while (true) {
            pixels[y * h.width + x] = in[j++];
            if (!advance(x, y, h) || ++i >= c) {
              break;
            }
          }
          // make SURE we move forward the right number of bytes
          in += num_bytes;
        }

      } else {

        int i = 0;
        while (true) {
          pixels[y * h.width + x] = c;
          if (!advance(x, y, h) || ++i >= n) {
            break;
          }
        }

      } // end if
    } // end while

    // flippy flippy!
    int pitch = h.width;
    auto_array<byte> row(new byte[pitch]);
    for (int i = 0; i < h.height / 2; ++i) {
      int j = h.height - i - 1;
      memcpy((byte*)row,         pixels + i * pitch, pitch);
      memcpy(pixels + i * pitch, pixels + j * pitch, pitch);
      memcpy(pixels + j * pitch, (byte*)row,         pitch);
    }

    return new SimpleImage(h.width, h.height, PF_I8, pixels.release(),
                           (byte*)palette.release(), 256, PF_B8G8R8);
  }
  
  Image* ReadBitmap16(const byte* raster_data, const Header& h) {
    auto_array<RGB> pixels(new RGB[h.width * h.height]);

    for (int i = 0; i < h.height; ++i) {
      const byte* in = raster_data + i * h.pitch;
      RGB* out = pixels + (h.height - i - 1) * h.width;

      for (int j = 0; j < h.width; ++j) {
        int clr = read16_le(in);
        in += 2;

#define C16(C) \
  (byte)( ((clr & h.bf_##C##_mask) >> h.bf_##C##_shift) << h.bf_##C##_rshift);

        out->red   = C16(red);
        out->green = C16(green);
        out->blue  = C16(blue);
        ++out;

#undef C16
      }
    }

    return new SimpleImage(h.width, h.height, PF_R8G8B8,
                           (byte*)pixels.release());
  }

  Image* ReadBitmap24(const byte* raster_data, const Header& h) {
    auto_array<BGR> pixels(new BGR[h.width * h.height]);

    for (int i = 0; i < h.height; ++i) {
      const byte* in = raster_data + i * h.pitch;
      BGR* out = pixels + (h.height - i - 1) * h.width;

      for (int j = 0; j < h.width; ++j) {
        out->blue  = *in++;
        out->green = *in++;
        out->red   = *in++;
        ++out;
      }
    }

    return new SimpleImage(h.width, h.height, PF_B8G8R8,
                           (byte*)pixels.release());
  }

  Image* ReadBitmap32(const byte* raster_data, const Header& h) {
    auto_array<RGB> pixels(new RGB[h.width * h.height]);

    for (int i = 0; i < h.height; ++i) {
      const byte* in = raster_data + i * h.pitch;
      RGB* out = pixels + (h.height - i - 1) * h.width;

      for (int j = 0; j < h.width; ++j) {
        u32 pixel = read32_le(in);
        in += 4;
        out->red   = (byte)((pixel & h.bf_red_mask)   >> h.bf_red_shift);
        out->green = (byte)((pixel & h.bf_green_mask) >> h.bf_green_shift);
        out->blue  = (byte)((pixel & h.bf_blue_mask)  >> h.bf_blue_shift);
        ++out;
      }
    }

    return new SimpleImage(h.width, h.height, PF_R8G8B8,
                           (byte*)pixels.release());
  }

  Image* DecodeBitmap(File* file, const Header& h) {

    if (!file->seek(h.data_offset, File::BEGIN)) {
      return 0;
    }

    // the raster data stored in the file
    auto_array<byte> raster_data(new byte[h.image_size]);
    if (file->read(raster_data, h.image_size) != h.image_size) {
      return 0;
    }

    // the output pixel buffer (parameter to new SimpleImage)
    auto_array<byte> pixels(new byte[h.width * h.height * 3]);

    typedef Image* (*Decoder)(const byte* raster_data, const Header& h);

    Decoder decoder = 0;

    if      (h.bpp == 1  &&  h.compression == 0)  { decoder = ReadBitmap1;    }
    else if (h.bpp == 4  &&  h.compression == 0)  { decoder = ReadBitmap4;    }
    else if (h.bpp == 4  &&  h.compression == 2)  { decoder = ReadBitmapRLE4; }
    else if (h.bpp == 8  &&  h.compression == 0)  { decoder = ReadBitmap8;    }
    else if (h.bpp == 8  &&  h.compression == 1)  { decoder = ReadBitmapRLE8; }
    else if (h.bpp == 16 && (h.compression == 0 ||
                             h.compression == 3)) { decoder = ReadBitmap16;   }
    else if (h.bpp == 24 &&  h.compression == 0)  { decoder = ReadBitmap24;   }
    else if (h.bpp == 32 && (h.compression == 0 ||
                             h.compression == 3)) { decoder = ReadBitmap32;   }

    if (decoder) {
      return decoder(raster_data.get(), h);
    } else {
      return 0;
    }
  }

}
