#include <stdio.h>
#include <string.h>
extern "C" {
  #include <gif_lib.h>
}
#include "Debug.h"
#include "Open.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  int InputFunc(GifFileType* gif, GifByteType* bytes, int size) {
    File* file = (File*)gif->UserData;
    return file->read(bytes, size);
  }

  Image* OpenGIF(File* file) {
    COR_GUARD("OpenGIF");

    // open GIF file
    GifFileType* gif = DGifOpen(file, InputFunc);
    if (!gif) {
      COR_LOG("DGifOpen failed");
      return 0;
    }

    // read GIF image
    if (DGifSlurp(gif) != GIF_OK) {
      COR_LOG("DGifSlurp failed");
      DGifCloseFile(gif);
      return 0;
    }
    ColorMapObject* cmap = gif->SColorMap;

    // validate attributes
    if (cmap == 0 ||
        gif->ImageCount < 1 ||
        cmap->ColorCount != (1 << cmap->BitsPerPixel))
    {
      COR_IF_DEBUG {
        char str[1024];
        sprintf(str, "incorrect gif attributes:\n");
        sprintf(str + strlen(str), " ImageCount: %d\n", gif->ImageCount);
        if (cmap) {
          sprintf(str + strlen(str), " BitsPerPixel: %d\n", cmap->BitsPerPixel);
          sprintf(str + strlen(str), " ColorCount: %d\n", cmap->ColorCount);
        }
        COR_LOG(str);
      }
      
      DGifCloseFile(gif);
      return 0;
    }

    // use the first frame of the animation
    SavedImage* gif_image = gif->SavedImages + 0;

    const int width = gif->SWidth;
    const int height = gif->SHeight;
    auto_array<byte> image(new byte[width * height]);
    auto_array<RGBA> palette(new RGBA[256]);

    // look for the transparent color extension
    int transparent = -1;
    for (int i = 0; i < gif_image->ExtensionBlockCount; ++i) {
      ExtensionBlock* eb = gif_image->ExtensionBlocks + i;
      if (eb->Function == 0xF9 && 
          eb->ByteCount == 4) {
        bool has_transparency = ((eb->Bytes[0] & 1) == 1);
        if (has_transparency) {
          transparent = eb->Bytes[3];
        }
      }
    }

    // copy over the palette
    memset(palette, 0, 256 * 4);
    for (int i = 0; i < cmap->ColorCount; ++i) {
      palette[i].red   = cmap->Colors[i].Red;
      palette[i].green = cmap->Colors[i].Green;
      palette[i].blue  = cmap->Colors[i].Blue;
      palette[i].alpha = (transparent == i ? 0 : 255);
    }

    byte* in = (byte*)gif_image->RasterBits;
    byte* out = image;
    if (gif->Image.Interlace) {

      // deinterlace

      // group 1 - every 8th row, starting with row 0
      for (int row = 0; row < height; row += 8) {
        memcpy(out + width * row, in, width);
        in += width;
      }

      // group 2 - every 8th row, starting with row 4
      for (int row = 4; row < height; row += 8) {
        memcpy(out + width * row, in, width);
        in += width;
      }

      // group 3 - every 4th row, starting with row 2
      for (int row = 2; row < height; row += 4) {
        memcpy(out + width * row, in, width);
        in += width;
      }

      for (int row = 1; row < height; row += 2) {
        memcpy(out + width * row, in, width);
        in += width;
      }

    } else {
      memcpy(out, in, width * height);
    }

    DGifCloseFile(gif);
    return new SimpleImage(width, height, PF_I8, image.release(),
                           (byte*)palette.release(), 256, PF_R8G8B8A8);
  }

}
