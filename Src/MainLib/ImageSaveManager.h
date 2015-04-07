/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __IMAGE_SAVE_MANAGER_H_
#define __IMAGE_SAVE_MANAGER_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"
#include <corona.h>

#include <string>
#include <vector>

using namespace std;

class GLDriver;
class ConfigData;

//@
//  Summary:
//    Class to handle the saving of image/pbuffer contents. 
//  
class ImageSaveManager
{
public:

  //@
  //  Summary:
  //    To set the initial saving options for the image saver.
  //  
  //  Parameters:
  //    configData  - The configuration data to copy the saving options from.
  //
  ImageSaveManager(const ConfigData &configData);

  //@
  //  Summary:
  //    Destructor
  //  
  virtual ~ImageSaveManager();

  //@
  //  Summary:
  //    To initialize this class with information from the OpenGL driver.
  //    (this must be called with a valid OpenGL context to succeed)
  //  
  //  Parameters:
  //    driver  - The driver to make queries on.
  //
  //  Returns:
  //    If initialization ws successful, true is returned. Else false is returned.
  //
  bool Init(GLDriver *driver);

  //@
  //  Summary:
  //    To save the passed image structure out. The passed texture ID must be 
  //    bound to the current OpenGL context in the current stage. OpenGL pack 
  //    alignment is handled in this method. (ie. is set and restored)
  //  
  //  Parameters:
  //    fileName  - The base file name to save the images out as. (without extension)
  //
  //    imageData  - The image data of the image to save. This image must be 
  //                 bound in the current context on the current stage.
  //
  //    retFileNames - The return array of all the names of images saved. 
  //                   (May be multiple for cube maps etc.)
  //
  //  Returns:
  //    True is returned if the image data was written successfully. 
  //    Otherwise false is returned.
  //
  bool SaveImage(const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const;

  //@
  //  Summary:
  //    To return a copy of the passed image scaled to the passed dimensions.
  //  
  //  Parameters:
  //    newWidth  - The new width of the returned image.
  //
  //    newHeight - The new height of the returned image.
  //
  //    oldImage  - The image to form the basis of the scaled returned image.
  //
  //  Returns:
  //    A scaled copy of the passed image is returned. If the image could not
  //    be scaled, NULL is returned.
  //
  static corona::Image * ScaleImage(uint newWidth, uint newHeight, corona::Image * oldImage);


  //Enable mip-map saving


protected:
  
  bool   saveIcon;                                // Save a icon version of the images 
  uint   iconSize;                                // The size of the icon if saving icons
  string iconExtension;                           // The extension of the icon files
 
  bool savePNG;                                   // Save the images in PNG format
  bool saveTGA;                                   // Save the images in TGA format
  bool saveJPG;                                   // Save the images in JPG format
  bool flipXAxis;                                 // Flip the images on the X axis before writting out

  bool cubeMapTile;                               // Flag to indicate if cube maps are tiled together or 
                                                  //    written as a single tiled image

  bool save1DTex;                                 // Flag to indicate if 1D textures are saved
  bool save2DTex;                                 // Flag to indicate if 2D textures are saved
  bool save3DTex;                                 // Flag to indicate if 3D textures are saved 
  bool saveCubeTex;                               // Flag to indicate if Cube textures are saved 
  bool savePbufTex;                               // Flag to indicate if p-buffer textures are saved

  bool texture3DSupport;                          // Flag if 3D textures are supported
  bool depthTextureSupport;                       // Flag if depth textures are supported
  bool extensionPBO;                              // Flag if the PBO extension is supported 

  void (GLAPIENTRY *iglBindBuffer) (GLenum target, GLuint buffer); //PBO buffer setting entry point

  //@
  //  Summary:
  //    To save a specific image type of image out to disk. The passed texture ID must be 
  //    bound to the current OpenGL context in the current stage. OpenGL pack 
  //    alignment must be handled before calling these methods.
  //  
  //  Parameters:
  //    fileName  - The base file name to save the images out as. (without extension)
  //
  //    imageData  - The image data of the image to save. This image must be 
  //                 bound in the current context on the current stage.
  //
  //    retFileNames - The return array of all the names of images saved. 
  //                   (May be multiple for cube maps etc.)
  //
  //  Returns:
  //    True is returned if the image data was written successfully.
  //    Otherwise false is returned.
  //
  bool Save1DImage(const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const;
  bool Save2DImage(GLuint glType, const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const;
  bool Save3DImage(GLuint glType, const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const;
  bool SaveCubeImage(const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const;
  bool SaveDummyImage(const string &fileName, const ImageData *imageData,ImageSaveFiles &retFileNames) const;

  //@
  //  Summary:
  //    To save an array of pixel data to disk.
  //  
  //  Parameters:
  //    fileName  - The base file name to save the images out as. (without extension)
  //
  //    imgData   - The array of pixel data to save.
  //
  //    retFileNames - The return array of all the names of images saved. 
  //                   (May be multiple for cube maps etc.)
  //
  //  Returns:
  //    True is returned if the data was written successfully. 
  //    Otherwise false is returned.
  //
  bool WriteImageFile(const string &fileName,corona::Image *imgData,ImageSaveFiles &retFileNames) const;

  //@
  //  Summary:
  //    To save an icon version of an array of pixel data to disk. 
  //    (The passed pixel data is scaled to an icon size before writing)
  //  
  //  Parameters:
  //    fileName  - The base file name to save the images out as. (without extension)
  //
  //    imgData   - The array of pixel data to save.
  //
  //    retFileNames - The return array of all the names of images saved. 
  //                   (May be multiple for cube maps etc.)
  //
  //  Returns:
  //    True is returned if the data was written successfully. 
  //    Otherwise false is returned.
  //
  bool WriteImageIconFile(const string &fileName,corona::Image *imgData,ImageSaveFiles &retFileNames) const;

  //@
  //  Summary:
  //    To retrieve the texture data from the OpenGL state. (via glGetTexImage)
  //  
  //  Parameters:
  //    imageType  - The type of image to get the data for (ie 1D,2D,3D etc).
  //
  //    baseInternalFormat  - The base internal format of the image (GL_RGB GL_ALPHA etc).
  //                          This value can be retrieved from GetBaseTextureFormat. 
  //
  //    newImage  - The image to store the resulting data in. This image must be big enough
  //                to hold the entire contents on the texture.
  //
  void RetrieveImageData(GLenum imageType, GLenum baseInternalFormat, corona::Image * newImage) const; 

  //@
  //  Summary:
  //    To guess at the base internal format of the texture. This is not simply the 
  //    internal format of the texture, (which can be retrieved from OpenGL) but determines
  //    if the texture is a color/depth or other type of texture. 
  //    (See base internal formats in the OpenGL spec)
  //  
  //  Parameters:
  //    imageType  - The type of image to get the data for (ie 1D,2D,3D etc).
  //
  //    baseInternalFormat  - The base internal format to be returned if true 
  //                          is returned by this function. 
  //
  //  Returns:
  //    If a base internal format could be determined for this texture, true is
  //    returned. Else false is returned.
  //
  bool GetBaseTextureFormat(GLenum imageType, GLenum &baseInternalFormat) const;

  //@
  //  Summary:
  //    To convert the passed array of six images into a tiled "cube-map" format.
  //    Images in the array must be in the order +X,-X,+Y,-Y,+Z,-Z. 
  //  
  //  Parameters:
  //    maxWidth  - The maximum width (or the requested width) of the images
  //                to be tiled.
  //
  //    maxWidth  - The maximum height (or the requested height) of the images
  //                to be tiled.
  //
  //    images    - The array of six images to be tiled. Images in this array
  //                can be NULL if no data is available.
  //
  //  Returns:
  //    If all data is valid, a tiled "cube-map" version of the passed six 
  //    images is returned. Else NULL is returned.
  //
  corona::Image * CreateTileCubeMapImage(uint maxWidth, uint maxHeight, corona::Image * images[6]) const;

};





#endif // __IMAGE_SAVE_MANAGER_H_
