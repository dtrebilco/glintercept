/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptImage.h"
#include "GLDriver.h"
#include "ConfigData.h"
#include <string.h>

//The real OpenGL driver
extern GLCoreDriver GLV;
extern WGLDriver    GLW;

USING_ERRORLOG


//Look-up types for cube maps
GLenum cubeMapTypes[6] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X,GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                          GL_TEXTURE_CUBE_MAP_POSITIVE_Y,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                          GL_TEXTURE_CUBE_MAP_POSITIVE_Z,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

char* cubeMapFaceNames[6] = {"_0","_1","_2","_3","_4","_5"};


///////////////////////////////////////////////////////////////////////////////
//
ImageSaveManager::ImageSaveManager(const ConfigData &configData):
saveIcon(configData.imageSaveIcon),
iconSize(configData.imageIconSize),
iconExtension(configData.imageIconFormat),
savePNG(configData.imageSavePNG),
saveTGA(configData.imageSaveTGA),
saveJPG(configData.imageSaveJPG),
flipXAxis(configData.imageFlipXAxis),
cubeMapTile(configData.imageCubeMapTile),
save1DTex(configData.imageSave1D),
save2DTex(configData.imageSave2D),
save3DTex(configData.imageSave3D),
saveCubeTex(configData.imageSaveCube),
savePbufTex(configData.imageSavePBufferTex),
texture3DSupport(false),
depthTextureSupport(false),
extensionPBO(false),
iglBindBuffer(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////
//
ImageSaveManager::~ImageSaveManager()
{
}

///////////////////////////////////////////////////////////////////////////////
//
bool ImageSaveManager::Init(GLDriver *driver)
{
  //If unable to make OpenGL calls, return false
  if(!driver->GetInternalGLCallMode())
  {
    return false;
  }

  // Determine if 3D textures are supported
  if(driver->GetOpenGLVersion() >= 1.2f ||
     driver->IsExtensionSupported("GL_EXT_texture3D"))
  {
    texture3DSupport = true;
  }

  //Determine if depth textures are supported
  if(driver->GetOpenGLVersion() >= 1.4f ||
     driver->IsExtensionSupported("GL_ARB_depth_texture"))
  {
    depthTextureSupport = true;
  }


  //Get if PBO's are supported
  if(driver->GetOpenGLVersion() >= 2.1f ||
     driver->IsExtensionSupported("GL_ARB_pixel_buffer_object") ||
     driver->IsExtensionSupported("GL_EXT_pixel_buffer_object"))
  {
    extensionPBO = true;

    //Get the buffer object entry point
    iglBindBuffer = (void (GLAPIENTRY *)(GLenum target, GLuint buffer))GLW.glGetProcAddress("glBindBuffer");

    //Get the ARB entry point
    if(iglBindBuffer == NULL)
    {
      iglBindBuffer = (void (GLAPIENTRY *)(GLenum target, GLuint buffer))GLW.glGetProcAddress("glBindBufferARB");
    }

    //If we could not get the entry point, flag the extension as unsupported
    if(iglBindBuffer == NULL)
    {
      extensionPBO = false;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ImageSaveManager::SaveImage(const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const
{
  bool retValue = false;
  retFileNames.iconName = "";
  retFileNames.imageFileNames.clear();

  //If the image is bound to a p-buffer and p-buffer saving is off, return now
  if(imageData->IsPBufferBound() && !savePbufTex)
  {
    return false;
  }

  //Save the pixel transfer modes
  GLint oldPackAlignment = 1, 
        oldLsbFirst = 0,
        oldRowLength = 0, 
        oldSkipPixels = 0,
        oldSkipRows = 0,
        oldSwapBytes = 0, 
        oldImageHeight = 0, 
        oldSkipImages = 0;

	GLV.glGetIntegerv(GL_PACK_ALIGNMENT,    &oldPackAlignment);
	GLV.glGetIntegerv(GL_PACK_LSB_FIRST,    &oldLsbFirst);

	GLV.glGetIntegerv(GL_PACK_ROW_LENGTH,   &oldRowLength);
	GLV.glGetIntegerv(GL_PACK_SKIP_PIXELS,  &oldSkipPixels);

	GLV.glGetIntegerv(GL_PACK_SKIP_ROWS,    &oldSkipRows);
  GLV.glGetIntegerv(GL_PACK_SWAP_BYTES,   &oldSwapBytes);

  //Set the required pack operations
  GLV.glPixelStorei(GL_PACK_ALIGNMENT,  1); 
  GLV.glPixelStorei(GL_PACK_LSB_FIRST,  0); 

  GLV.glPixelStorei(GL_PACK_ROW_LENGTH, 0); 
  GLV.glPixelStorei(GL_PACK_SKIP_PIXELS,0); 
  
  GLV.glPixelStorei(GL_PACK_SKIP_ROWS,  0); 
  GLV.glPixelStorei(GL_PACK_SWAP_BYTES, 0); 

  // If 3D textures are supported
  if(texture3DSupport)
  {
    GLV.glGetIntegerv(GL_PACK_IMAGE_HEIGHT, &oldImageHeight);
	  GLV.glGetIntegerv(GL_PACK_SKIP_IMAGES,  &oldSkipImages);

    GLV.glPixelStorei(GL_PACK_IMAGE_HEIGHT,0); 
    GLV.glPixelStorei(GL_PACK_SKIP_IMAGES, 0); 
  }

  //If PBO support, check if a GL_PIXEL_PACK_BUFFER is bound
  GLint oldPBO = 0;
  if(extensionPBO)
  {
    //Get the existing PBO
    GLV.glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &oldPBO);
    if(oldPBO != 0)
    {
      //If there is a pack PBO bound, disable it while reading pixels
      iglBindBuffer(GL_PIXEL_PACK_BUFFER,0); 
    }
  }

  //Save the images based on the different types
  switch(imageData->GetGLType())
  {
    case(GL_TEXTURE_1D):
      if(save1DTex)
      {
        retValue = Save1DImage(fileName,imageData,retFileNames);
      }
      break;

    case(GL_TEXTURE_2D):
      if(save2DTex)
      {
        retValue = Save2DImage(GL_TEXTURE_2D,fileName,imageData,retFileNames);
      }
      break;

    case(GL_TEXTURE_3D):
      if(save3DTex)
      {
        retValue = Save3DImage(GL_TEXTURE_3D,fileName,imageData,retFileNames);
      }
      break;

    case(GL_TEXTURE_CUBE_MAP):
      if(saveCubeTex)
      {
        retValue = SaveCubeImage(fileName,imageData,retFileNames);
      }
      break;

    case(GL_TEXTURE_RECTANGLE):
      if(save2DTex)
      {
        retValue = Save2DImage(GL_TEXTURE_RECTANGLE,fileName,imageData,retFileNames);
      }
      break;

    case(GL_TEXTURE_1D_ARRAY):
      if(save2DTex)
      {
        retValue = Save2DImage(GL_TEXTURE_1D_ARRAY,fileName,imageData,retFileNames);
      }
      break;

    case(GL_TEXTURE_2D_ARRAY):
      if(save3DTex)
      {
        retValue = Save3DImage(GL_TEXTURE_2D_ARRAY,fileName,imageData,retFileNames);
      }
      break;

    case(GL_TEXTURE_BUFFER):
      retValue = SaveDummyImage(fileName,imageData,retFileNames);
      break;

    default:
      LOGERR(("ImageSaveManager::SaveImage - Unknown image type"));
  }


  //If PBO support, check if a GL_PIXEL_PACK_BUFFER was bound and restore it
  if(extensionPBO && oldPBO != 0)
  {
    iglBindBuffer(GL_PIXEL_PACK_BUFFER, oldPBO); 
  }

  //Restore the pixel store modes
  GLV.glPixelStorei(GL_PACK_ALIGNMENT,  oldPackAlignment); 
  GLV.glPixelStorei(GL_PACK_LSB_FIRST,  oldLsbFirst); 

  GLV.glPixelStorei(GL_PACK_ROW_LENGTH, oldRowLength); 
  GLV.glPixelStorei(GL_PACK_SKIP_PIXELS,oldSkipPixels); 
  
  GLV.glPixelStorei(GL_PACK_SKIP_ROWS,  oldSkipRows); 
  GLV.glPixelStorei(GL_PACK_SWAP_BYTES, oldSwapBytes); 

  if(texture3DSupport)
  {
    GLV.glPixelStorei(GL_PACK_IMAGE_HEIGHT,oldImageHeight); 
    GLV.glPixelStorei(GL_PACK_SKIP_IMAGES, oldSkipImages); 
  }

  return retValue;
}



///////////////////////////////////////////////////////////////////////////////
//
bool ImageSaveManager::Save1DImage(const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const
{
  GLint width;
  GLV.glGetTexLevelParameteriv(GL_TEXTURE_1D,0,GL_TEXTURE_WIDTH,&width);

  //Catch bad values
  if(width == 0)
  {
    LOGERR(("ImageSaveManager::Save1DImage - invalid width on image"));
    return false;
  }

  //Get the basic type of this texture (RGB,depth etc)
  GLenum baseTextureFormat;
  if(!GetBaseTextureFormat(GL_TEXTURE_1D,baseTextureFormat))
  {
    return false;
  }

  //Create a new image to write to
  corona::Image * newImage = corona::CreateImage(width,1,corona::PF_R8G8B8A8);
  if(!newImage)
  {
    return false;
  }

  //Fill the image buffer
  RetrieveImageData(GL_TEXTURE_1D, baseTextureFormat, newImage);

  //Write the image out
  if(!WriteImageFile(fileName,newImage,retFileNames))
  {
    delete newImage;
    return false;
  }

  //Write an icon version (can fail if icon writing is not enabled)
  WriteImageIconFile(fileName,newImage,retFileNames);

  //Clean up
  delete newImage;

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ImageSaveManager::Save2DImage(GLuint glType, const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const
{
  if(glType != GL_TEXTURE_2D &&
     glType != GL_TEXTURE_RECTANGLE &&
     glType != GL_TEXTURE_1D_ARRAY)
  {
    LOGERR(("ImageSaveManager::Save2DImage - unsupported image type"));
    return false;
  }

  GLint width,height;
  GLV.glGetTexLevelParameteriv(glType,0,GL_TEXTURE_WIDTH,&width);
  GLV.glGetTexLevelParameteriv(glType,0,GL_TEXTURE_HEIGHT,&height);

  //Catch bad values
  if(width == 0 || height == 0)
  {
    LOGERR(("ImageSaveManager::Save2DImage - invalid width/height on image"));
    return false;
  }

  //Get the basic type of this texture (RGB,depth etc)
  GLenum baseTextureFormat;
  if(!GetBaseTextureFormat(glType,baseTextureFormat))
  {
    return false;
  }

  //Create a new image to write to
  corona::Image * newImage = corona::CreateImage(width,height,corona::PF_R8G8B8A8);
  if(!newImage)
  {
    return false;
  }

  //Fill the image buffer
  RetrieveImageData(glType, baseTextureFormat, newImage);

  //Write the image out
  if(!WriteImageFile(fileName,newImage,retFileNames))
  {
    delete newImage;
    return false;
  }

  //Write an icon version (can fail if icon writing is not enabled)
  WriteImageIconFile(fileName,newImage,retFileNames);

  //Clean up
  delete newImage;

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ImageSaveManager::Save3DImage(GLuint glType, const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const
{
  if(glType != GL_TEXTURE_3D &&
     glType != GL_TEXTURE_2D_ARRAY)
  {
    LOGERR(("ImageSaveManager::Save3DImage - unsupported image type"));
    return false;
  }

  GLint width,height,depth;
  GLV.glGetTexLevelParameteriv(glType,0,GL_TEXTURE_WIDTH,&width);
  GLV.glGetTexLevelParameteriv(glType,0,GL_TEXTURE_HEIGHT,&height);
  GLV.glGetTexLevelParameteriv(glType,0,GL_TEXTURE_DEPTH,&depth);

  //Catch bad values
  if(width == 0 || height == 0 || depth == 0)
  {
    LOGERR(("ImageSaveManager::Save3DImage - invalid width/height/depth on image"));
    return false;
  }

  //Get the basic type of this texture (RGB,depth etc)
  GLenum baseTextureFormat;
  if(!GetBaseTextureFormat(glType,baseTextureFormat))
  {
    return false;
  }

  // Layout 2D array images in a different order to 3D images (vertically - so each image is stored correctly)
  GLint layoutWidth = width*depth;
  GLint layoutHeight = height;
  if(glType == GL_TEXTURE_2D_ARRAY)
  {
    layoutWidth = width;
    layoutHeight = height*depth;
  }

  //Create a new image to write to
  corona::Image * newImage = corona::CreateImage(layoutWidth,layoutHeight,corona::PF_R8G8B8A8);
  if(!newImage)
  {
    return false;
  }

  //Set the required pack operations
  GLV.glPixelStorei(GL_PACK_IMAGE_HEIGHT,height);
  GLV.glPixelStorei(GL_PACK_ROW_LENGTH,  width);

  //Fill the image buffer
  RetrieveImageData(glType, baseTextureFormat, newImage);

  //Write the image out
  if(!WriteImageFile(fileName,newImage,retFileNames))
  {
    delete newImage;
    return false;
  }

  //Write an icon version (can fail if icon writing is not enabled)
  WriteImageIconFile(fileName,newImage,retFileNames);

  //Clean up
  delete newImage;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ImageSaveManager::SaveCubeImage(const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const
{
  uint i;
  GLint width[6],height[6];
  GLenum baseTextureFormat[6];

  //DT_TODO: Need to handle cube maps where not all faces are filled out.

  //Get the width/height of all faces
  for(i=0;i<6;i++)
  {
    GLV.glGetTexLevelParameteriv(cubeMapTypes[i],0,GL_TEXTURE_WIDTH,&width[i]);
    GLV.glGetTexLevelParameteriv(cubeMapTypes[i],0,GL_TEXTURE_HEIGHT,&height[i]);

    //If the widths are valid 
    if(width[i] > 0 && height[i] > 0)
    {
      //Get the basic type of this texture (RGB,depth etc)
      if(!GetBaseTextureFormat(cubeMapTypes[i],baseTextureFormat[i]))
      {
        return false;
      }
    }
    else
    {
      //Init to zero
      baseTextureFormat[i] = 0;
    }
  }

  //Get the max width and height
  GLint maxWidth  = width[0];
  GLint maxHeight = height[0];
  for(i=1;i<6;i++)
  {
    //Get the maximum
    if(width[i] > maxWidth)
    {
      maxWidth = width[i];
    }
    if(height[i] > maxHeight)
    {
      maxHeight = height[i];
    }
  }

  //Catch bad values
  if(maxWidth == 0 || maxHeight == 0)
  {
    LOGERR(("ImageSaveManager::SaveCubeImage - invalid width/height on image"));
    return false;
  }

  //Create all the images
  corona::Image *newImages[6];
  for(i=0;i<6;i++)
  {
    //Loop and get all the images
    if(width[i] > 0 && height[i] > 0)
    {
      newImages[i] = corona::CreateImage(width[i],height[i],corona::PF_R8G8B8A8);  

      //Only get for a valid texture
      if(newImages[i])
      {
        //Fill the image buffer
        RetrieveImageData(cubeMapTypes[i], baseTextureFormat[i],newImages[i]);
      }
    }
    else
    {
      newImages[i] = NULL;
    }
  }

  bool retValue = false;

  //If we are not tiling the images, write each individually
  if(!cubeMapTile)
  {
    //The index of the image to be used as an icon
    int iconIndex = -1; 

    //Write all the images out
    for(i=0;i<6;i++)
    {
      //If the image exists, write it
      if(newImages[i])
      {
        //Assign a unique name to the face
        string faceName = fileName + cubeMapFaceNames[i];

        //Flag this image to be the icon version
        iconIndex=i;

        //Cache return result
        if(WriteImageFile(faceName,newImages[i],retFileNames))
        {
          retValue = true;
        }
      }
    }

    //If there is a valid image to be used as an icon, write it out
    if(iconIndex != -1)
    {
      //Write an icon version (can fail if icon writing is not enabled)
      WriteImageIconFile(fileName,newImages[iconIndex],retFileNames);
    }
  }
  else
  {
    //Tile the cube maps together into one image
    corona::Image * tileImage = CreateTileCubeMapImage(maxWidth,maxHeight,newImages);
    if(tileImage)
    {
      //Cache return result 
      retValue = WriteImageFile(fileName,tileImage,retFileNames);
      
      //Write a icon version
      WriteImageIconFile(fileName,tileImage,retFileNames);

      //Delete the tile image
      delete tileImage;
    }
    else
    {
      LOGERR(("ImageSaveManager::SaveCubeImage - Unable to tile cube map images"));
    }
  }

  //Clean up
  for(i=0;i<6;i++)
  {
    //If the image exists, delete it
    if(newImages[i])
    {
      delete newImages[i];
    }
  }

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ImageSaveManager::SaveDummyImage(const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const
{
  const uint DUMMY_IMG_SIZE = 14;

  //Create a new image to write to
  corona::Image * newImage = corona::CreateImage(DUMMY_IMG_SIZE,DUMMY_IMG_SIZE,corona::PF_R8G8B8A8);
  if(!newImage)
  {
    return false;
  }

  // Load in a question mark as the dummy image
  static GLubyte questionMarkImg[DUMMY_IMG_SIZE*DUMMY_IMG_SIZE] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,53,58,23,0,0,0,0,0,0,0,0,10,165,
    252,255,255,254,177,18,0,0,0,0,0,0,152,255,255,255,255,255,255,174,0,0,
    0,0,0,0,191,226,246,66,36,255,255,246,0,0,0,0,0,0,0,0,0,1,127,255,255,200,
    0,0,0,0,0,0,0,0,14,180,255,255,218,35,0,0,0,0,0,0,0,0,162,255,255,174,13,
    0,0,0,0,0,0,0,0,0,226,255,244,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,0,0,255,255,255,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

  GLubyte * dstPixels = (GLubyte *)newImage->getPixels();
  for(uint i = 0; i < (DUMMY_IMG_SIZE * DUMMY_IMG_SIZE); i++)
  {
    dstPixels[0] = questionMarkImg[i];
    dstPixels[1] = questionMarkImg[i];
    dstPixels[2] = questionMarkImg[i];
    dstPixels[3] = 255;
    dstPixels += 4;
  }

  //Write the image out
  if(!WriteImageFile(fileName,newImage,retFileNames))
  {
    delete newImage;
    return false;
  }

  //Write an icon version (can fail if icon writing is not enabled)
  WriteImageIconFile(fileName,newImage,retFileNames);

  //Clean up
  delete newImage;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ImageSaveManager::WriteImageFile(const string &fileName,corona::Image *imgData,ImageSaveFiles &retFileNames) const
{
  //Save as PNG if requested
  if(savePNG)
  {
    //Save pngs
    string outName = fileName + ".png";

    //Write the file out
    if(!corona::SaveImage(outName.c_str(),corona::FF_PNG,imgData))
    {
      LOGERR(("ImageSaveManager::WriteImageFile - Unable to save image %s",outName.c_str()));
      return false;
    }

    //Add the name to the saved filenames
    retFileNames.imageFileNames.push_back(outName);

  }

  //Save as TGA if requested
  if(saveTGA)
  {
    //Save tgas
    string outName = fileName + ".tga";

    //Write the file out
    if(!corona::SaveImage(outName.c_str(),corona::FF_TGA,imgData))
    {
      LOGERR(("ImageSaveManager::WriteImageFile - Unable to save image %s",outName.c_str()));
      return false;
    }

    //Add the name to the saved filenames
    retFileNames.imageFileNames.push_back(outName);
  }

  //Save as JPEG if requested
  if(saveJPG)
  {
    //Save tgas
    string outName = fileName + ".jpg";

    //Write the file out
    if(!corona::SaveImage(outName.c_str(),corona::FF_JPEG,imgData))
    {
      LOGERR(("ImageSaveManager::WriteImageFile - Unable to save image %s",outName.c_str()));
      return false;
    }

    //Add the name to the saved filenames
    retFileNames.imageFileNames.push_back(outName);
  }


  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ImageSaveManager::WriteImageIconFile(const string &fileName,corona::Image *imgData,ImageSaveFiles &retFileNames) const
{
  //Test if we save icons
  if(!saveIcon || iconSize == 0)
  {
    return false;
  }

  //Create a scaled version of the image
  corona::Image * iconImage =ScaleImage(iconSize,iconSize,imgData);
  
  //Save a icon version of the image
  if(iconImage)
  {
    string outName = fileName + "_icon." + iconExtension;

    //Write the file out
    if(!corona::SaveImage(outName.c_str(), corona::FF_AUTODETECT, iconImage))
    {
      LOGERR(("ImageSaveManager::WriteImageIconFile - Unable to save image %s",outName.c_str()));
      delete iconImage;
      return false;
    }

    //Save the icon name
    retFileNames.iconName = outName;

    //Clean up
    delete iconImage;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void ImageSaveManager::RetrieveImageData(GLenum imageType, GLenum baseInternalFormat, corona::Image * newImage) const
{
  //Check the pixel format and width/height
  if(newImage &&  newImage->getWidth() > 0 && newImage->getHeight() > 0 &&
     newImage->getFormat() != corona::PF_R8G8B8A8)
  {
    LOGERR(("ImageSaveManager::RetrieveImageData - Attempt to get bad image"));
    return;
  }

  //If not a currently supported base format
  if(baseInternalFormat != GL_RGB   && 
     baseInternalFormat != GL_RGBA &&
     baseInternalFormat != GL_ALPHA && 
     baseInternalFormat != GL_DEPTH_COMPONENT)
  {
    LOGERR(("ImageSaveManager::RetrieveImageData - Unsupported base internal format 0x%x",baseInternalFormat));
    return;
  }

  //Get the width/height
  uint width  = newImage->getWidth();
  uint height = newImage->getHeight();

  //Test the internal format of the texture
  if(baseInternalFormat == GL_DEPTH_COMPONENT)
  {
    //Get the depth image data
    GLubyte *buffer = new GLubyte[width*height];

    //Get the depth image data
    GLV.glGetTexImage(imageType,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,buffer);

    //Get the pixel buffer
    GLubyte * srcImg  = buffer;
    GLubyte * dstImg  = (GLubyte *)newImage->getPixels();

    //Loop for the width/height
    for (uint i = 0; i < width * height; i++)
    {
      //Assign the new color
      dstImg[0] = *srcImg;
      dstImg[1] = *srcImg;
      dstImg[2] = *srcImg;
      dstImg[3] = 255;

      //Increment the source and destination
      srcImg++;
      dstImg+=4;
    }

    //Clean up the buffer
    delete buffer;
  }
  
  //Add other exotic texture types here

  else
  {
    //Get the color image data
    GLV.glGetTexImage(imageType,0,GL_RGBA,GL_UNSIGNED_BYTE,newImage->getPixels());

    //Fill out the green and blue for other 1,2 component texture types
    if(baseInternalFormat == GL_ALPHA)
    {
      //Get the pixel buffer
      udword * srcImg  = (udword *)newImage->getPixels();

      //Loop for the width/height
      for (uint i = 0; i < width * height; i++)
      {
        //Get the red and alpha color
        udword red   = *srcImg & 0x000000FF;
        udword alpha = *srcImg & 0xFF000000;

        //Assign the new color
        *srcImg = red | (red << 8) | (red << 16) | alpha;
    
        //Increment the source
        srcImg++;
      }
    }

  }

  //Test for X axis flipping
  if(flipXAxis)
  {
    //Flip image on X axis
    corona::FlipImage(newImage,corona::CA_X);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool ImageSaveManager::GetBaseTextureFormat(GLenum imageType, GLenum &baseInternalFormat) const
{
  //Init return data
  baseInternalFormat = 0;

  GLint redBits   =0,
        greenBits =0,
        blueBits  =0,
        alphaBits =0;
  GLint depthBits =0; 

  //Get the data from the GL state
  GLV.glGetTexLevelParameteriv(imageType,0,GL_TEXTURE_RED_SIZE,  &redBits);
  GLV.glGetTexLevelParameteriv(imageType,0,GL_TEXTURE_GREEN_SIZE,&greenBits);
  GLV.glGetTexLevelParameteriv(imageType,0,GL_TEXTURE_BLUE_SIZE, &blueBits);
  GLV.glGetTexLevelParameteriv(imageType,0,GL_TEXTURE_ALPHA_SIZE,&alphaBits);

  //If depth textures are supported
  if(depthTextureSupport)
  {
    GLV.glGetTexLevelParameteriv(imageType,0,GL_TEXTURE_DEPTH_SIZE,&depthBits);
  }

  //Determine if it is an alpha texture
  if(alphaBits > 0 && redBits == 0 && greenBits == 0 && blueBits == 0)
  {
    baseInternalFormat = GL_ALPHA;
    return true;
  }

  //Determine if it is a color format
  if(redBits > 0 || greenBits > 0 || blueBits > 0)
  {
    baseInternalFormat = GL_RGB;
    if(alphaBits > 0)
    {
      baseInternalFormat = GL_RGBA;
    }
    return true;
  }

  //Check for a depth texture
  if(depthBits > 0)
  {
    baseInternalFormat = GL_DEPTH_COMPONENT;
    return true;
  }

  GLint  internalFormat;
  GLV.glGetTexLevelParameteriv(imageType,0,GL_TEXTURE_INTERNAL_FORMAT,&internalFormat);

  //Unknown/unsupported texture format (NV HILO/ palette texture etc)
  LOGERR(("ImageSaveManager::GetBaseTextureFormat - Unknown texture image format 0x%x - using RGBA8",internalFormat));
  
  // Return true for unknown formats and hope it works (OK for luminance etc) 
  baseInternalFormat = GL_RGBA;
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
corona::Image * ImageSaveManager::ScaleImage(uint newWidth, uint newHeight, corona::Image * oldImage) 
{
  //Check image
  if(!oldImage)
  {
    return NULL;
  }

  uint oldWidth  = oldImage->getWidth();
  uint oldHeight = oldImage->getHeight();

  //Check the pixel format and width/height
  if(oldWidth == 0 || oldHeight == 0 || newWidth == 0 || newHeight == 0 ||
     oldImage->getFormat() != corona::PF_R8G8B8A8)
  {
    LOGERR(("ImageSaveManager::Scale - Attempt to scale bad image"));
    return NULL;
  }

  //Create a new Image
  corona::Image * newImage = corona::CreateImage(newWidth,newHeight,corona::PF_R8G8B8A8);

  //Get the source and destination buffers
  udword * destImg = (udword *)newImage->getPixels();
  udword * srcImg  = (udword *)oldImage->getPixels();

  //Loop for the width/height
  uint sampleY, sampleX;
  for (uint y = 0; y < newHeight; y++)
  {
    sampleY = (oldHeight-1)*y / (newHeight-1);

    for (uint x = 0; x < newWidth; x++)
    {
      sampleX = (oldWidth-1)*x / (newWidth-1);

      //Copy the pixel
      *destImg = srcImg[(sampleY * oldWidth) + sampleX];
      
      //Increment the destination
      destImg++;
    }
  }

  return newImage;
}


///////////////////////////////////////////////////////////////////////////////
//
corona::Image * ImageSaveManager::CreateTileCubeMapImage(uint maxWidth, uint maxHeight, corona::Image * images[6]) const
{
  //Check max width and height
  if(maxWidth == 0 || maxHeight == 0)
  {
    LOGERR(("ImageSaveManager::CreateTileCubeMapImage - Zero width and height"));
    return NULL;
  }

  //Offset arrays for image positioning
  uint XOffsetArray[6] = {2,0,1,1,1,3};
  uint YOffsetArray[6] = {1,1,0,2,1,1};

  //Flip the positioning of the "Y" pieces on X axis flip
  if(flipXAxis)
  {
    YOffsetArray[2] = 2;
    YOffsetArray[3] = 0;
  }


  //Create a new blank image 
  corona::Image * retImage = corona::CreateImage(maxWidth * 4,maxHeight * 3, corona::PF_R8G8B8A8);

  //Clear the starting buffer with white
  memset(retImage->getPixels(), 0xFF, retImage->getWidth() * retImage->getHeight() * 4);

  //Loop for all images
  for(uint i=0;i<6;i++)
  {
    //If there is a valid image
    if(images[i])
    {
      corona::Image * tmpImage = images[i];

      //If the dimensions do not equal the requested image size,
      //  create a new image
      if((uint)images[i]->getWidth() != maxWidth || (uint)images[i]->getHeight() != maxHeight)
      {
        tmpImage = ScaleImage(maxWidth,maxHeight,images[i]);
        if(!tmpImage)
        {
          LOGERR(("ImageSaveManager::CreateTileCubeMapImage -Unable to scale image"));
          return retImage;
        }
      }

      //Get a pointer to the source and destination pixels
      udword * srcImg  = (udword *)tmpImage->getPixels();
      udword * destImg = (udword *)retImage->getPixels() + 
                         ((YOffsetArray[i]*maxHeight) * maxWidth*4) +
                         ((XOffsetArray[i]*maxWidth));

      //Loop and copy the image data
      for(uint h=0;h<maxHeight;h++)
      {
        //Copy a single line
        memcpy(destImg, srcImg, maxWidth*4);

        //Increment the pointers
        destImg += maxWidth*4;
        srcImg  += maxWidth;
      }

      //Delete the temporary image if one was assigned
      if(tmpImage != images[i])
      {
        delete tmpImage;
      }
    }
  }

  return retImage;
}
