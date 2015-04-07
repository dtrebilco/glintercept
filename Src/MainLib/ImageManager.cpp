/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptImage.h"
#include "GLDriver.h"


USING_ERRORLOG

//Set the class name for the image manager
template<>
char *InterceptDataManager<ImageData>::className = "Image";


///////////////////////////////////////////////////////////////////////////////
//
ImageData::ImageData(uint glID):
InterceptDataType(glID),
glType(0),
pBufferBound(false)

#ifdef GLI_BUILD_WINDOWS

,pBufferHandle(NULL)
,pBufferBufType(0)
 
#endif //GLI_BUILD_WINDOWS
 
{
}

///////////////////////////////////////////////////////////////////////////////
//
bool ImageData::GetUniqueFileName(string &retString) const
{
  //Set the initial image name
  retString = "Image_";

  //Append the image type
  switch(glType)
  {
    case(GL_TEXTURE_1D):
      retString = retString + "1D_";
      break;
    case(GL_TEXTURE_2D):
      retString = retString + "2D_";
      break;
    case(GL_TEXTURE_3D):
      retString = retString + "3D_";
      break;
    case(GL_TEXTURE_CUBE_MAP):
      retString = retString + "CUBE_";
      break;
    case(GL_TEXTURE_RECTANGLE):
      retString = retString + "NVRECT_";
      break;
    case(GL_TEXTURE_1D_ARRAY):
      retString = retString + "1D_ARRAY_";
      break;
    case(GL_TEXTURE_2D_ARRAY):
      retString = retString + "2D_ARRAY_";
      break;
    case(GL_TEXTURE_BUFFER):
      retString = retString + "TEX_BUFFER_";
      break;
    default:
      retString = retString + "UNKNOWN_";
      break;
  }

  //Add an extra flag for p-buffers
  if(IsPBufferBound())
  {
    retString = retString + "PBuf_";
  }

  //Add the texture ID
  string bufString;
  StringPrintF(bufString,"%04u_",id);
  retString = retString + bufString;
  
  //Add the save count
  static uint saveCount=0;
  saveCount++;
  StringPrintF(bufString,"%04u",saveCount);
  retString = retString + bufString;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
ImageManager::ImageManager()
{

  //Add a texture image (the zero image always exists)
  AddData(0);
}

///////////////////////////////////////////////////////////////////////////////
//
ImageManager::~ImageManager()
{

  //Remove the 0 texture image
  InterceptDataManager<ImageData>::RemoveData(0);
}


#ifdef GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//
void ImageManager::GetPBufferImages(HPBUFFERARB handle,vector<ImageData *> &pBufferImages)
{
  //Empty the array
  pBufferImages.clear();

  //Loop for all images
  for(DataArrayType::iterator i=dataArray.begin(); i!=dataArray.end(); ++i)
  {
    //If the image is ready and a p-buffer, save it
    if(i->second.IsReady() &&
       i->second.IsPBufferBound() && i->second.GetPBufferHandle() == handle)
    { 
      //Add the image to the return array
      pBufferImages.push_back(&(i->second));
    }
  }
}

#endif //GLI_BUILD_WINDOWS


///////////////////////////////////////////////////////////////////////////////
//
bool ImageManager::RemoveData(uint glId)
{
  //Never delete 0
  if(glId != 0)
  {
    return InterceptDataManager<ImageData>::RemoveData(glId);
  }

  return true;
}

