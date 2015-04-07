
#include "MovieMaker.h"
#include <CommonErrorLog.h>

USING_ERRORLOG


#ifdef GLI_BUILD_WINDOWS
#pragma comment(lib, "vfw32.lib")

///////////////////////////////////////////////////////////////////////////////
//
MovieMaker::MovieMaker():
aviSysOpen(false),
initSuccess(false),

width(0),
height(0),
numFrames(0),

pfile(NULL),
ps(NULL),
psCompressed(NULL)
{


}

///////////////////////////////////////////////////////////////////////////////
//
MovieMaker::~MovieMaker()
{
  //Close the file and stream if opened
  if(ps)
  {
    AVIStreamClose(ps);
  }
  if (psCompressed)
  {
    AVIStreamClose(psCompressed);
  }
  if(pfile)
  {
    AVIFileClose(pfile);
  }

  //Close the AVI system if it was opened
  if(aviSysOpen)
  {
    AVIFileExit();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool MovieMaker::Init(const char *newFileName, uint newWidth, uint newHeight, uint frameRate, const vector<string> &codecs)
{
  //Test if init
  if(initSuccess)
  {
    LOGERR(("Init - Already init"));
    return false;
  }

  // Check VFW version.
  WORD wVer = HIWORD(VideoForWindowsVersion());
  if (wVer < 0x010A)
  {
    LOGERR(("Init - VFW version 0x%x is too old.\n",wVer));
    return false;
  }

  //Open the AVI system
  AVIFileInit();
  aviSysOpen = true;

  //Open the file
  HRESULT result = AVIFileOpen(&pfile,              // Returned file pointer
                               newFileName,         // File name
                               OF_WRITE | OF_CREATE,// Mode to open file with
                               NULL);               // Use handler determined
  if(result != AVIERR_OK)
  {
    LOGERR(("Init - Error creating file %s",newFileName));
    return false;
  }

  //Create the avi info structure
  AVISTREAMINFO aviInfo;

  memset(&aviInfo, 0, sizeof(aviInfo));

  aviInfo.fccType    = streamtypeVIDEO;
  aviInfo.fccHandler = 0;  

  aviInfo.dwQuality  = -1;
  aviInfo.dwLength   = 0; 

  aviInfo.dwScale    = 1;
  aviInfo.dwRate     = frameRate;

  aviInfo.dwSuggestedBufferSize  = newWidth * newHeight * 4;
  SetRect(&aviInfo.rcFrame, 0, 0, (int) newWidth, (int) newHeight);

  // And create the stream;
  result = AVIFileCreateStream(pfile,     // File pointer
                               &ps,       // Returned stream pointer
                               &aviInfo); // Stream header
  if (result != AVIERR_OK)
  {
    LOGERR(("Init - Error creating stream for file %s",newFileName));
    return false;
  }

  //Loop for all codecs
  bool codecSuccess = false;
  for(uint i=0; i<codecs.size(); i++)
  {
    //Attempt to create the codec and stop on the first successful codec
    if(CreateCompressedStream(codecs[i], frameRate))
    {
      codecSuccess = true;
      break;
    }
  }
  if(!codecSuccess)
  {
    LOGERR(("Init - No valid compression codecs for %s",newFileName));
    return false;
  }


  //Initialize bitmapInfo
  BITMAPINFOHEADER bitmapInfo;
  memset(&bitmapInfo,0, sizeof(BITMAPINFOHEADER));

  //Fill the bitmap info structure.
  bitmapInfo.biSize=sizeof(BITMAPINFOHEADER);
  bitmapInfo.biWidth =newWidth;
  bitmapInfo.biHeight=newHeight;
  bitmapInfo.biPlanes=1;
  bitmapInfo.biBitCount=32;
  bitmapInfo.biSizeImage=newWidth*newHeight*4; 
  bitmapInfo.biCompression=BI_RGB; //BI_RGB means BGR in reality

  //Set the format of a stream at the specified position
  result = AVIStreamSetFormat(psCompressed, 
                              0,               // position
                              &bitmapInfo,     // stream format
                              bitmapInfo.biSize +   // format size
                              bitmapInfo.biClrUsed * sizeof(RGBQUAD));
  if (result != AVIERR_OK)
  {
    LOGERR(("Init - AVI Compressed Stream format setting failed."));
    return false;
  }

  //Assign init success
  width  = newWidth;
  height = newHeight;
  initSuccess = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool MovieMaker::AddFrame(void * bitmapData)
{
  if(!initSuccess)
  {
    return false;
  }

  //Write the buffer
  HRESULT result = AVIStreamWrite(psCompressed, // stream pointer
                                  numFrames,    // time of this frame
                                  1,            // number to write
                                  (LPBYTE) bitmapData, //Data to write
                                  width*height*4,      // size of this frame
                                  AVIIF_KEYFRAME,      // flags....
                                  NULL,
                                  NULL);
  if (result != AVIERR_OK)
  {
    LOGERR(("AddFrame - data write failed"));
    initSuccess = false;
    return false;
  }
  numFrames++;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
DWORD CreateCodecDWORD(const string &codecStr)
{
  //Create a string of spaces
  char str[5] = "    ";

  //If none, assign uncompressed
  if(codecStr == "none")
  {
    str[0] = 'D';
    str[1] = 'I';
    str[2] = 'B';
  }
  else
  {
    //Pack the string into the character array
    for(uint i=0; i<4 && i<codecStr.size(); i++)
    {
      str[i] = codecStr[i];
    }
  }

  //Return the packed word
  return mmioFOURCC(str[0], str[1], str[2], str[3]);
}

///////////////////////////////////////////////////////////////////////////////
//
bool MovieMaker::CreateCompressedStream(const string &codec, uint frameRate)
{
  //Get the compression options
  HRESULT result = AVIERR_OK;
  AVICOMPRESSOPTIONS opts;
  AVICOMPRESSOPTIONS FAR * aopts[1] = {&opts};
  memset(&opts, 0, sizeof(opts));

  //If using the menu
  if(codec == "menu")
  {
    //Load the memu options
    if (!AVISaveOptions(GetActiveWindow(), 0, 1, &ps, (LPAVICOMPRESSOPTIONS FAR *)&aopts))
    {
      AVISaveOptionsFree(1,(LPAVICOMPRESSOPTIONS FAR *) &aopts);
      return false;
    }

    //Create a compressed stream using codec options.
    result = AVIMakeCompressedStream(&psCompressed, 
                                      ps, 
                                     &opts, 
                                      NULL);
    if (result != AVIERR_OK)
    {
      AVISaveOptionsFree(1,(LPAVICOMPRESSOPTIONS FAR *) &aopts);
      return false;
    }

    //Release save options
    result = AVISaveOptionsFree(1,(LPAVICOMPRESSOPTIONS FAR *) &aopts);
    if (result!=AVIERR_OK)
    {
      LOGERR(("CreateCompressedStream -Error releasing memory"));
      return false;
    }

  }
  else
  {
    opts.fccType=streamtypeVIDEO;
    opts.fccHandler= CreateCodecDWORD(codec); // mmioFOURCC('M','P','G','4');
    opts.dwFlags=AVICOMPRESSF_KEYFRAMES | AVICOMPRESSF_VALID;//|AVICOMPRESSF_DATARATE;
    opts.dwKeyFrameEvery=frameRate;

    //Create a compressed stream using codec options.
    result = AVIMakeCompressedStream(&psCompressed, 
                                      ps, 
                                     &opts, 
                                      NULL);
    if (result != AVIERR_OK)
    {
      return false;
    }
  }

  return true;
}

#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

///////////////////////////////////////////////////////////////////////////////
//
MovieMaker::MovieMaker():
aviSysOpen(false),
initSuccess(false),

width(0),
height(0),
numFrames(0)

{
  //DT_TODO: Implement this class
}

///////////////////////////////////////////////////////////////////////////////
//
MovieMaker::~MovieMaker()
{
}

///////////////////////////////////////////////////////////////////////////////
//
bool MovieMaker::Init(const char *newFileName, uint newWidth, uint newHeight, uint frameRate, const vector<string> &codecs)
{
  //Test if init
  if(initSuccess)
  {
    LOGERR(("Init - Already init"));
    return false;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
bool MovieMaker::AddFrame(void * bitmapData)
{
  if(!initSuccess)
  {
    return false;
  }

  return false;
}


#endif //GLI_BUILD_LINUX

