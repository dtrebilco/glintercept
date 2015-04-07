#include <memory>
#include "Debug.h"
#include "Save.h"
#include "Utility.h"
extern "C" {
#include <jpeglib.h>
}

#ifdef GLI_CHANGES

namespace corona {

  bool SaveJPEG(File* file, Image* image) {
    COR_GUARD("SaveJPEG");

    if (!image) {
      return false;
    }

    //Hack for now
    FILE* filePtr = (FILE*)file->GetFilePtr();
    if(!filePtr)
    {
      return false;
    }

    // If the image format isn't supported directly by this function,
    // clone to a supported format and try to save with that.
    switch (image->getFormat()) {
      case PF_R8G8B8:
	      break;
      default: {
	      COR_LOG("Unsupported pixel format... cloning");
	      std::auto_ptr<Image> cloned(CloneImage(image, PF_R8G8B8));
	      return SaveJPEG(file, cloned.get());
      }
    }

    const int width  = image->getWidth();
    const int height = image->getHeight();

   	jpeg_compress_struct cinfo;
	  jpeg_error_mgr jerr;

	  cinfo.err = jpeg_std_error(&jerr);
	  jpeg_create_compress(&cinfo);

	  int nChannels = 3;

	  cinfo.in_color_space = JCS_RGB;
	  jpeg_set_defaults(&cinfo);

	  cinfo.input_components = nChannels;
	  cinfo.num_components   = nChannels;
	  cinfo.image_width  = width;
	  cinfo.image_height = height;
	  cinfo.data_precision = 8;
	  cinfo.input_gamma = 1.0;

	  jpeg_set_quality(&cinfo, 75, FALSE);
    
	  jpeg_stdio_dest(&cinfo, filePtr);
	  jpeg_start_compress(&cinfo, TRUE);

	  unsigned char *curr_scanline = (unsigned char *)image->getPixels();

	  for (int y = 0; y < height; y++){
		  jpeg_write_scanlines(&cinfo, &curr_scanline, 1);
		  curr_scanline += nChannels * width;
	  }

	  jpeg_finish_compress(&cinfo);
	  jpeg_destroy_compress(&cinfo);

    return true;
  }

}

#endif //GLI_CHANGES


