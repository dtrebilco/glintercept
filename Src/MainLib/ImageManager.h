/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __IMAGE_MANAGER_H_
#define __IMAGE_MANAGER_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"
#include "InterceptDataManager.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;
class ImageManager;

//@
//  Summary:
//    Structure to hold data about the filenames used to save the image.
//  
struct ImageSaveFiles
{
  string  iconName;               // The name (if any) of the icon for this image
  vector<string> imageFileNames;  // An array of all the image filenames used when saving the image  
};


//@
//  Summary:
//    Structure of data about a OpenGL image
//  
class ImageData : public InterceptDataType
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //
  //  Parameters:
  //    glID  - The unique OpenGL ID of this type.
  //  
  ImageData(uint glID);

  //@
  //  Summary:
  //    To flag the image as ready and sets that the image can now be saved.
  //    (This call also flags the image as dirty)
  //  
  //  Parameters:
  //    type  - The type of the image that is now ready.
  //
  inline void SetReady(GLenum type);

  //@
  //  Summary:
  //    To set if the image is dirty or not. This flag should be set when the 
  //    OpenGL image changes and un-set when the image is written out.
  //  
  //  Parameters:
  //    flag  - Flag to indicate if this OpenGL image is now dirty or not.
  //
  inline void SetDirty(bool flag);

  //@
  //  Summary:
  //    To get the OpenGL texture type of this image. Ensure the image is ready
  //    (IsReady()) before calling or 0 will be returned. 
  //  
  //  Returns:
  //    The OpenGL texture type assigned to this image is returned.
  //
  inline GLenum GetGLType() const;

  //@
  //  Summary:
  //    To determine if a p-buffer is bound to this texture or not.
  //  
  //  Returns:
  //    If a p-buffer is bound to this texture, true is returned. Else false is returned.
  //
  inline bool IsPBufferBound() const;

  //@
  //  Summary:
  //    To set if a p-buufer is bound to the texture
  //  
  //  Parameters:
  //    flag  -  The flag to indicate if a p-buffer is now bound to this image/texture.
  //
  //    type  -  The OpenGL texture type that is being bound to the texture. 
  //             If the image is alreday ready, this type must match the current internal type.
  //
  //  Returns:
  //    Returns true if the buffer was bound/unbound without errors. False is returned otherwise.
  //
  inline bool SetPBufferBound(bool flag,GLenum type);


#ifdef GLI_BUILD_WINDOWS

  //@
  //  Summary:
  //    To get the p-buffer handle for the image. This may not be valid. 
  //    (see IsPBufferBound)
  //  
  //  Returns:
  //    The p-buffer handle is returned.
  //
  inline HPBUFFERARB GetPBufferHandle() const;

  //@
  //  Summary:
  //    To set the p-buffer handle for the image.
  //  
  //  Parameters:
  //    handle  - The p-buffer handle to set.
  //
  inline void SetPBufferHandle(HPBUFFERARB handle);

  //@
  //  Summary:
  //    To get the p-buffer buffer type handle for the image. 
  //    This may not be valid. (see IsPBufferBound)
  //  
  //  Returns:
  //    The p-buffer buffer type is returned.
  //
  inline int GetPBufferBufType() const;

  //@
  //  Summary:
  //    To set the p-buffer buffer type for the image.
  //  
  //  Parameters:
  //    type  - The p-buffer buffer type to set.
  //
  inline void SetPBufferBufType(int type);

#endif //GLI_BUILD_WINDOWS

  //@
  //  Summary:
  //    To get a unique texture filename (including type,textureID and savecount)
  //    without any extension. 
  //  
  //  Parameters:
  //    retString  - The string to return the unique name in.
  //
  //  Returns:
  //    If a unique string could be generated, true is returned. 
  //    Else false is returned.
  //
  bool GetUniqueFileName(string &retString) const;

  //@
  //  Summary:
  //    To get a reference to the filenames that were used to save the 
  //    image data out previously. (ie. cube-maps may have six images etc)
  //    The structure will be empty if the image has not yet been saved.
  //  
  //  Returns:
  //    A reference to the filenames of saved images is returned.
  //
  inline ImageSaveFiles & GetImageSaveFileNames();

private:

  GLenum glType;                                  // The OpenGL type of the image

  ImageSaveFiles saveFileNames;                   // Filenames that were used when the image was last saved.

  bool        pBufferBound;                       // Flag to indicate if there is a p-buffer bound to this texture

#ifdef GLI_BUILD_WINDOWS

  HPBUFFERARB pBufferHandle;                      // Handle to the p-buffer if a p-buffer is bound
  int         pBufferBufType;                     // The buffer type of the p-buffer that is bound to the image

#endif //GLI_BUILD_WINDOWS

};


//@
//  Summary:
//    This class maintains a list of texture image data and controls 
//    adding/deleting and retrival of image data.
//  
class ImageManager : public InterceptDataManager<ImageData>
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //  
	ImageManager();

  //@
  //  Summary:
  //    Destructor, destroys all array data and reports texture memory leaks.
  //  
  virtual ~ImageManager();


#ifdef GLI_BUILD_WINDOWS

  //@
  //  Summary:
  //    To get all images that use a p-buffer and have the the passed 
  //    p-buffer handle. 
  //  
  //  Parameters:
  //    handle         -  The pbuffer handle to get the images for.
  //
  //    pBufferImages  -  The array to return all the p-buffer images in.
  //
  void GetPBufferImages(HPBUFFERARB handle,vector<ImageData *> &pBufferImages);

#endif //GLI_BUILD_WINDOWS

  //@
  //  Summary:
  //    To remove an image from the image list.
  //  
  //  Parameters:
  //    glId  -  The OpenGL texture ID of the image to remove.
  //
  bool RemoveData(uint glId);


};



///////////////////////////////////////////////////////////////////////////////
//
inline void ImageData::SetReady(GLenum type)
{
  ready  = true;
  glType = type;

  //Set as dirty
  SetDirty(true);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ImageData::SetDirty(bool flag)
{
  //Chcek if the flag is different
  if(flag != dirty && ready)
  {
    //Assign the dirty flag
    dirty = flag;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
inline GLenum ImageData::GetGLType() const
{
  //Only return the stored type if a type has been set
  if(IsReady())
  {
    return glType;
  }

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
inline bool ImageData::IsPBufferBound() const
{
  return pBufferBound;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool ImageData::SetPBufferBound(bool flag,GLenum type)
{
  //If the flag does not change the state, return
  if(flag == pBufferBound)
  {
    return true;
  }

  //Check if the types are equal
  if(IsReady() && type != glType)
  {
    return false;
  }

  //Assign the flag
  pBufferBound = flag;

  //Re-set enable/dirty flags
  if(flag)
  {
    //Set as ready
    SetReady(type);
  }
  else
  {
    //Restore the ready state
    ready = false;

    //Since we are no longer ready, flag dirty as false
    dirty = false;
  }

  return true;
}

#ifdef GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//
inline HPBUFFERARB ImageData::GetPBufferHandle() const
{
  return pBufferHandle;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ImageData::SetPBufferHandle(HPBUFFERARB handle)
{
  pBufferHandle = handle;
}

///////////////////////////////////////////////////////////////////////////////
//
inline int ImageData::GetPBufferBufType() const
{
  return pBufferBufType;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ImageData::SetPBufferBufType(int type)
{
  pBufferBufType = type;
}

#endif //GLI_BUILD_WINDOWS


///////////////////////////////////////////////////////////////////////////////
//
inline ImageSaveFiles & ImageData::GetImageSaveFileNames()
{
  return saveFileNames;
}

#endif // __IMAGE_MANAGER_H_
