#include <memory>
#include <png.h>
#include "Debug.h"
#include "Save.h"
#include "Utility.h"


namespace corona {

  bool SaveTGA(File* file, Image* source) {
    COR_GUARD("SaveTGA");

    std::auto_ptr<Image> image(CloneImage(source, PF_B8G8R8A8));
    if (!image.get()) {
      return false;
    }

    const int width  = image->getWidth();
    const int height = image->getHeight();

    // generate the header
    byte header[18];
    header[0] = 0;               // id_length
    header[1] = 0;               // cm_type
    header[2] = 2;               // image_type (uncompressed true color)
    write16_le(header + 3,  0);  // cm_first
    write16_le(header + 5,  0);  // cm_length
    header[7] = 0;               // cm_entry_size
    write16_le(header + 8,  0);  // x_origin
    write16_le(header + 10, 0);  // y_origin
    write16_le(header + 12, width);
    write16_le(header + 14, height);
    header[16] = 32;             // pixel_depth
    header[17] = (1 << 5) | 7;   // origin at upper/left, 8 bits of alpha data

    if (file->write(header, 18) != 18) {
      return false;
    }

    // write pixels
    const int data_size = width * height * 4;
    if (file->write(image->getPixels(), data_size) != data_size) {
      return false;
    }

    return true;
  }

}
