//Movie maker - Inspired ny Nvidia's MovieMaker class

#ifndef __MOVIE_MAKER_H
#define __MOVIE_MAKER_H

#ifdef GLI_BUILD_WINDOWS

#include <windows.h>
#include <vfw.h>

#endif //GLI_BUILD_WINDOWS


#include <vector>
#include <string>

typedef unsigned int uint;

using namespace std;

//@
//  Summary:
//    Class for saving arrays of bitmaps as a movie stream.
//  
class MovieMaker 
{
public:

  //@
  //  Summary:
  //    Constructor.
  //  
  MovieMaker();

  //@
  //  Summary:
  //    Destructor. Completes any current movie being created and
  //    releases all memory. 
  //  
  virtual ~MovieMaker();

  //@
  //  Summary:
  //    To create a new movie of the specified name. 
  //  
  //  Parameters:
  //    fileName  - The name of the movie to create.
  //
  //    width     - The width of the frame data.
  //
  //    height    - The height of the frame data.
  //
  //    frameRate - The frame rate of the movie to be created.
  //
  //    codecs    - The codecs to use when saving the movie. 
  //                (First succesful codec is used) Special codec tokens are:
  //                menu - brings up the menu of available codecs
  //                none - no compression
  //
  //  Returns:
  //    True is returned on success, false if otherwise.
  //
  bool Init(const char *fileName, uint width, uint height, uint frameRate, const vector<string> &codecs);

  //@
  //  Summary:
  //    To add a frame to the current movie. 
  //    (Init must be called successfully first)
  //  
  //  Parameters:
  //    bitmapData  - The frame data to add. Must be 32 bit and of
  //                  the specified dimensions.
  //
  //  Returns:
  //    True is returned on if the frame could be added, false if otherwise.
  //
  bool AddFrame(void * bitmapData);

  //@
  //  Summary:
  //    To get the width of the current movie.
  //    (Data supplied to AddFrame must be of this width)
  //  
  //  Returns:
  //    The current width is returned.
  //
  inline uint GetWidth() const;

  //@
  //  Summary:
  //    To get the height of the current movie.
  //    (Data supplied to AddFrame must be of this height)
  //  
  //  Returns:
  //    The current height is returned.
  //
  inline uint GetHeight() const;

  //@
  //  Summary:
  //    To get the count of the number of frames saved so far.
  //  
  //  Returns:
  //    The number of frame saved (via AddFrame) is returned.
  //
  inline uint GetNumFrames() const;


protected:

  bool aviSysOpen;                                // If the AVI system has been created
  bool initSuccess;                               // If initialization has been successful

  uint   width;                                   // The width of the frames saved
  uint   height;                                  // The height of the frames saved  
  uint   numFrames;                               // The number of frames saved so far 

#ifdef GLI_BUILD_WINDOWS

  PAVIFILE pfile;                                 // Pointer to the movie file
  PAVISTREAM ps;                                  // Pointer to the movie stream
  PAVISTREAM psCompressed;                        // Pointer to the movie compressed stream

  //@
  //  Summary:
  //    To create the compressed stream using the supplied codec.
  //  
  //  Parameters:
  //    codec  - The codec to use.
  //
  //    frameRate - The frame rate used to create the compressed stream
  //
  //  Returns:
  //    True is returned if the compressed stream was created, false if
  //    otherwise.
  //
  bool CreateCompressedStream(const string &codec, uint frameRate);

#endif //GLI_BUILD_WINDOWS

};


///////////////////////////////////////////////////////////////////////////////
//
inline uint MovieMaker::GetWidth() const
{
  return width;
}


///////////////////////////////////////////////////////////////////////////////
//
inline uint MovieMaker::GetHeight() const
{
  return height;
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint MovieMaker::GetNumFrames() const
{
  return numFrames;
}

#endif //__MOVIE_MAKER_H
