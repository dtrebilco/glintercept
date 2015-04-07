/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_IMAGE_H_
#define __INTERCEPT_IMAGE_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"
#include "ImageManager.h"
#include "ImageSaveManager.h"
#include "InterceptData.h"

#include <string>
#include <vector>

using namespace std;



class GLDriver;

//@
//  Summary:
//    This struct is used in the retrieval of what textures are bound to what
//    stages.
//  
struct BoundTexture
{
  uint texID;        // The texture ID that is bound            
  uint texStage;     // The stage the ID is bound to
};

typedef vector<BoundTexture> BoundTextureArray;

//NOTE: Texture ID 0 is not treated correctly as it is treated as if it is a standard texture ID. 
//      This is not the case as texture ID 0 can have multiple texture types active. (ie 1D 2D 3D etc)
//      but this situation is so rare that it is not taken inito account.

//@
//  Summary:
//    This class intercepts OpenGL texture based calls and prepares them for
//    logging.
//  
class InterceptImage : public InterceptData 
{
public:

  //@
  //  Summary:
  //    Constructor 
  //  
  //  Parameters:
  //    driver  - The driver used to make queries on the OpenGL state.
  //
  //    functionTable - The table of all functions that are known/are being logged.
  //
  //    configData    - The configuration options for image interception.
  //
  InterceptImage(GLDriver *driver,FunctionTable * functionTable,const ConfigData &configData);

  //@
  //  Summary:
  //    To log the passed function and function data 
  //    (Before the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args);

  //@
  //  Summary:
  //    To perform any post-call logging of a function.
  //    (After the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value (if any).
  //
  virtual void LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal);
  
  //@
  //  Summary:
  //    To enable/disable image saving. Ensure image path is set before
  //    enabling.
  //  
  //  Parameters:
  //    flag - Flag to enable/disable image saving.
  //
  virtual void SetDataSaving(bool flag);

  //@
  //  Summary:
  //    To set the path where images are saved (including trailing seperator)
  //  
  //  Parameters:
  //    newPath - The new path to save images to.
  //
  virtual void SetDataSavingPath(const string &newPath);

  //@
  //  Summary:
  //    To save the passed image data to disk. Images are only saved if 
  //    image saving is enabled. The state of the images' dirty flag is
  //    re-set on successful saving. 
  //  
  //  Parameters:
  //    imageData  - The image data to save.
  //
  //  Returns:
  //    If saving was successful, true is returned. Else false is returned.
  //
  bool SaveTextureImage(ImageData *imageData);

  //@
  //  Summary:
  //    To get the saved image names for the passed texture ID. If the 
  //    image is dirty, an attempt to save the image is made. 
  //  
  //  Parameters:
  //    texId  - The texture ID of the images to save.
  //
  //    retSaveFiles - The array of image file names used to save the texture.
  //
  //  Returns:
  //    If saving/retrieval was successful, true is returned. Else false is returned.
  //
  bool GetTextureFileNames(GLint texId, ImageSaveFiles &retSaveFiles);

  //@
  //  Summary:
  //    To save all the currently dirty textures to disk.
  //  
  virtual void SaveAllDirtyData();

  //@
  //  Summary:
  //    To set all the textures/images that are currently being tracked, as 
  //    dirty. (ie. needs saving)
  //  
  virtual inline void SetAllDataDirty();

  //Enable mip-map saving

  //@
  //  Summary:
  //    To get an array of texture IDs that are bound. 
  //    (All texture/image stages are checked) Unless a fragment program is
  //    active, texturing must also be enabled for a texture to be "bound".
  //  
  //  Parameters:
  //    retArray  - The return array of texture IDs (and the stages) that are bound.
  //
  void GetBoundTextures(BoundTextureArray &retArray);

  //@
  //  Summary:
  //    To get from the OpenGL state what texture ID is active for the 
  //    passed texture stage and passed texture target.
  //  
  //  Parameters:
  //    oglTarget  - The OpenGL target of the texture ID to get.
  //
  //    texStage   - The texture stage to get the OpenGL target from.
  //
  //    retTexID   - The returned texture ID.
  //
  //  Returns:
  //    If the texture ID could be retrieved, true is returned and retTexID 
  //    contains the texture ID, Else false is returned.
  //
  bool GetActiveTextureID(GLenum oglTarget,uint texStage,uint &retTexID);

protected:

  ImageManager     imgManager;                    // The manager of images/textures
  ImageSaveManager imgSaver;                      // The manager of saving textures to disk

  bool      initialized;                          // Flag to indicate if we have been initialized
  bool      initFailed;                           // Flag to indicate if initialization failed

  bool      multiTextureSupport;                  // Flag to indicate if multi-texturing is supported
  GLint     maxNumImageUnits;                     // The number of imaging units supported (is at least number of texture stages)
  GLint     maxCombinedImageUnits;                // The number of imaging units supported (in GLSL fragment and vertex) (is at least number of texture stages)

  float     oglVersion;                           // The current OpenGL version

  bool      texture3DSupport;                     // Flag to indicate if 3D texturing is supported
  bool      textureCubeMapSupport;                // Flag to indicate if cube map texturing is supported
  bool      textureNVRectSupport;                 // Flag to indicate if NV rect texturing is supported
  bool      textureArraySupport;                  // Flag to indicate if Array texturing is supported
  bool      textureBufferSupport;                 // Flag to indicate if texture buffers are supported

  bool      extensionARBFragmentProgram;          // Flag to indicate ARB fragment program support
  bool      extensionNVFragmentProgram;           // Flag to indicate NV fragment program support
  bool      extensionNVTextureShader;             // Flag to indicate NV texture shader support
  bool      extensionARBGLSLProgram;              // Flag to indicate GLSL programs are supported 
  bool      extensionFBO;                         // Flag to indicate if FBO is supported 

  GLint     maxFBOColorAttachments;               // The maximum number of FBO color attachments that can exist

  vector<uint> cachedReadyImages;                 // An array of image numbers that have been flagged 
                                                  //  as ready but have not yet been written out.

  //@
  //  Summary:
  //    Destructor.
  //  
  virtual ~InterceptImage();

  //@
  //  Summary:
  //    To initialize constants and arrays etc.
  //  
  //  Returns:
  //    If initialization was successful, true is returned. Else false is returned.
  //
  bool Init();

  //@
  //  Summary:
  //    To determine if the driver is available and perform initialization if
  //    necessary.
  //  
  //  Returns:
  //    If the driver is available, true is returned. Else false is returned.
  //
  bool DriverAvailable();

  //@
  //  Summary:
  //    To get the action index (what action needs to be taken) for the 
  //    passed function name and index. (the index is assumed to remain the
  //    same for a given function name)
  //  
  //  Returns:
  //    If a action is valid for the index it is returned. Else -1 is returned.
  //
  int GetImageActionIndex(const string &functionName, int funcIndex); 

  //@
  //  Summary:
  //    To get from the OpenGL state what texture ID is active for the 
  //    current texture stage and passed texture target.
  //  
  //  Returns:
  //    The OpenGL texture ID for the current stage and passed target 
  //    is returned.
  //
  uint GetActiveTextureID(GLenum oglTarget);

  //@
  //  Summary:
  //    To get from the OpenGL state the current texture stage.
  //  
  //  Returns:
  //    The OpenGL texture/image stage that is currently active is returned.
  //
  uint GetActiveTextureStage();

  //@
  //  Summary:
  //    To set from the OpenGL state to the passed texture/image stage.
  //  
  //  Parameters:
  //    stage  - The texture/image stage to set.
  //
  //  Returns:
  //    If the OpenGL state could be set, true is returned. Else false
  //    is returned.
  //
  bool SetActiveTextureStage(uint stage);

  //@
  //  Summary:
  //    When a texture has image data uploaded, it is not written to disk 
  //    immediatly as other mip/depth levels may follow. This method flushes
  //    any images that have been flagged as ready but have not yet been 
  //    written to disk.
  //  
  void SaveCachedReadyImages();

  //@
  //  Summary:
  //    To set any textures that are bound to the current FBO 
  //    (frame buffer object) as dirty. If no FBO is currently bound this
  //    method does nothing.
  //  
  void SetFBOTexturesDirty();

  //@
  //  Summary:
  //    To set the texture at the specified attachment point of the current 
  //    FBO as dirty. If there is no texture at the attachment point, 
  //    this method does nothing.
  //
  //    Ensure a FBO is in use before calling this method.
  //  
  //  Parameters:
  //    currAttachmentCheck - The attachment point on the FBO to check.
  //
  void SetFBOAttachmentTextureDirty(GLenum currAttachmentCheck);

private:

  GLsizei numNewTextures;                         // Internal variable to cache data on texture creation
  const GLuint *newTextureIDs;                    // Internal variable to cache data on texture creation
  
  GLenum  textureTarget;                          // Internal variable to cache data on texture upload
  bool    validTextureReady;                      // Internal variable to cache data on texture upload
  bool    validTextureDirty;                      // Internal variable to cache data on texture dirty

  bool    updateFBOTextures;                      // Flag to indicate that any bound FBO textures need to be flagged as dirty 

#ifdef GLI_BUILD_WINDOWS

  HPBUFFERARB pBufferHandle;                      // Internal variable to cache data on pbuffer usage
  int         pBufferBufType;                     // Internal variable to cache data on pbuffer usage

  BOOL (WINAPI *iwglQueryPbuffer) (HPBUFFERARB, int, int *); //P-buffer query function
  
#endif //GLI_BUILD_WINDOWS 

  void (GLAPIENTRY *iglActiveTexture)(GLenum texture);       //Active texture function 
  GLhandle (GLAPIENTRY *iglGetHandleARB) (GLenum pname);      //Old ARB get current GLSL program

  void (GLAPIENTRY *iglGetFramebufferAttachmentParameterivEXT) (GLenum target, GLenum attachment, GLenum pname, GLint *params); //FBO Entry point

  //Creates a texture
  void CreateTexturesPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void CreateTexturesPost(const FunctionData *funcData,uint funcTableIndex);

  //Binds a texture (May also create a texture)
  void BindTexturesPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void BindTexturesPost(const FunctionData *funcData,uint funcTableIndex);

  //Delete a texture
  void DeleteTextures(const FunctionData *funcData,uint index, FunctionArgs & args);

  //Set image ready
  void SetImageReadyPre(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void SetImageReadyPost(uint actionIndex,const FunctionData *funcData,uint funcTableIndex);

  //Set image dirty
  void SetImageDirtyPre(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void SetImageDirtyPost(uint actionIndex,const FunctionData *funcData,uint funcTableIndex);


#ifdef GLI_BUILD_WINDOWS

  //P-Buffer binding
  void BindPBufferPre(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void BindPBufferPost(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal);

  //P-Buffer release
  void ReleasePBufferPre(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void ReleasePBufferPost(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal);

  //P-Buffer delete
  void DestroyPBufferPre(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);
  void DestroyPBufferPost(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal);
  
#endif //GLI_BUILD_WINDOWS  

};


///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptImage::SetAllDataDirty()
{
  imgManager.SetAllDataDirty();
}


#endif // __INTERCEPT_IMAGE_H_
