#include <memory>
#include <png.h>
#include "Debug.h"
#include "Save.h"
#include "Types.h"


namespace corona {

  void PNG_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    File* file = (File*)png_get_io_ptr(png_ptr);
    if (file->write(data, length) != int(length)) {
      png_error(png_ptr, "Write error");
    }
  }

  void PNG_flush(png_structp png_ptr) {
    // assume that files always flush
  }

  bool SavePNG(File* file, Image* image) {
    COR_GUARD("SavePNG");

    if (!image) {
      return false;
    }

    // If the image format isn't supported directly by this function,
    // clone to a supported format and try to save with that.
    switch (image->getFormat()) {
      case PF_R8G8B8A8:
      case PF_R8G8B8:
      case PF_I8:
	break;
      default: {
	COR_LOG("Unsupported pixel format... cloning");
	std::auto_ptr<Image> cloned(CloneImage(image, PF_R8G8B8A8));
	return SavePNG(file, cloned.get());
      }
    }

    // create write struct
    png_structp png_ptr = png_create_write_struct(
      PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
      return false;
    }

    // error handling!
    if (setjmp(png_jmpbuf(png_ptr))) {
      png_destroy_write_struct(&png_ptr, NULL);
      return false;
    }

    // create info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
      png_destroy_write_struct(&png_ptr, NULL);
      return false;
    }

    int width  = image->getWidth();
    int height = image->getHeight();

    // set image characteristics
    png_set_write_fn(png_ptr, file, PNG_write, PNG_flush);

    int color_format = 0; // png output format
    int color_format_bpp = 0; // png bytes per pixel
    bool color_format_paletted = false; // png palette needed flag

    // figure out output format
    switch (image->getFormat()) {
      case PF_R8G8B8A8:
        color_format = PNG_COLOR_TYPE_RGB_ALPHA;
        color_format_bpp = 4;
        break;
      case PF_R8G8B8:
        color_format = PNG_COLOR_TYPE_RGB;
        color_format_bpp = 3;
        break;
      case PF_I8:
        color_format = PNG_COLOR_TYPE_PALETTE;
        color_format_bpp = 1;
        color_format_paletted = true;
        break;
      default:
        // Unsupported format.  This should already be taken care of
        // by the test at the beginning of this function.
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    png_set_IHDR(
      png_ptr, info_ptr,
      width, height,
      8,
      color_format,
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT,
      PNG_FILTER_TYPE_DEFAULT);

    png_color* png_palette = 0;
    if (color_format_paletted) {
      COR_LOG("Saving palettized image...");

      int image_palette_format = image->getPaletteFormat(); // palette format
      int image_palette_size = image->getPaletteSize(); // palette size

      // allocate png palette and get pointer to image palette
      png_palette = (png_color*)png_malloc(
        png_ptr, sizeof(png_color) * image_palette_size);
      byte* image_palette = (byte*)image->getPalette();


      if (image_palette_format == PF_R8G8B8) {
	// 24 bit source palette
	for (int i = 0; i < image_palette_size; i++) {
	  // copy entry directly
	  png_palette[i].red   = *image_palette++;
	  png_palette[i].green = *image_palette++;
	  png_palette[i].blue  = *image_palette++;
	}
      } else if (image_palette_format == PF_R8G8B8A8) {
	// 32 bit source palette
	for (int i = 0; i < image_palette_size; i++) {
	  // copy entry, skip alpha
	  png_palette[i].red   = *image_palette++;
	  png_palette[i].green = *image_palette++;
	  png_palette[i].blue  = *image_palette++;
	  image_palette++;
	}
      }
      // write palette
      png_set_PLTE(png_ptr, info_ptr, png_palette, image_palette_size);
    }

    byte* pixels = (byte*)image->getPixels();

    // build rows
    void** rows = (void**)png_malloc(png_ptr, sizeof(void*) * height);
    for (int i = 0; i < height; ++i) {
      rows[i] = png_malloc(png_ptr, color_format_bpp * width);
      memcpy(rows[i], pixels, color_format_bpp * width);
      pixels += width * color_format_bpp;      
    }
    png_set_rows(png_ptr, info_ptr, (png_bytepp)rows);
    info_ptr->valid |= PNG_INFO_IDAT;

    // actually write the image
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // clean up memory
    for (int i = 0; i < height; ++i) {
      png_free(png_ptr, rows[i]);
    }
    png_free(png_ptr, rows);

    if (png_palette) {
      png_free(png_ptr, png_palette);
    }

    png_destroy_write_struct(&png_ptr, &info_ptr);

    return true;
  }

}
