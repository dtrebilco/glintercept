#ifndef CORONA_SAVE_H
#define CORONA_SAVE_H


#include "corona.h"


namespace corona {
#ifndef NO_PNG
  bool SavePNG(File* file, Image* image); // SavePNG.cpp
#endif
  bool SaveTGA(File* file, Image* image); // SaveTGA.cpp

#ifdef GLI_CHANGES
  bool SaveJPEG(File* file, Image* image); // SaveJPEG.cpp
#endif //GLI_CHANGES
}


#endif
