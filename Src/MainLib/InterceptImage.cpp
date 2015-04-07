/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptImage.h"
#include "FileUtils.h"
#include "GLDriver.h"

//The real OpenGL driver
extern GLCoreDriver GLV;
extern WGLDriver    GLW;

USING_ERRORLOG

//NOTE: Need to add GLX support for p-buffers


//Flags to indicate what action to take when we get an
//  image function
enum ImageFunctionAction
{
  IFA_UNKNOWN =0,

  IFA_1D_TEX_READY,
  IFA_2D_TEX_READY,               // 2D is used for both 2D/Rect textures and cubemaps
  IFA_3D_TEX_READY,

  IFA_COMPRESS_1D_TEX_READY,
  IFA_COMPRESS_2D_TEX_READY,      // 2D is used for both 2D/Rect textures and cubemaps
  IFA_COMPRESS_3D_TEX_READY,

  IFA_1D_TEX_DIRTY,
  IFA_2D_TEX_DIRTY,               // 2D is used for both 2D/Rect textures and cubemaps
  IFA_3D_TEX_DIRTY,

  IFA_IMAGE_BIND,
  IFA_IMAGE_CREATE,
  IFA_IMAGE_DELETE,

  IFA_PBUFFER_BIND,
  IFA_PBUFFER_RELEASE,
  IFA_PBUFFER_DESTROY

};

enum ImageFunctionType
{
  GL_GENTEXTURES =0,
  GL_DELETETEXTURES,
  GL_BINDTEXTURE,

  //Loading of texture data
  GL_TEXIMAGE1D,
  GL_TEXIMAGE2D,
  GL_TEXIMAGE3D,

  GL_COPYTEXIMAGE1D,
  GL_COPYTEXIMAGE2D,

  //Conpressed versions of image loads
  GL_COMPRESSEDTEXIMAGE1D,
  GL_COMPRESSEDTEXIMAGE2D,
  GL_COMPRESSEDTEXIMAGE3D,

  //Image dirty functions
  GL_TEXSUBIMAGE1D,
  GL_TEXSUBIMAGE2D,
  GL_TEXSUBIMAGE3D,

  GL_COMPRESSEDTEXSUBIMAGE1D,
  GL_COMPRESSEDTEXSUBIMAGE2D,
  GL_COMPRESSEDTEXSUBIMAGE3D,

  GL_COPYTEXSUBIMAGE1D,
  GL_COPYTEXSUBIMAGE2D,
  GL_COPYTEXSUBIMAGE3D,

  GL_TEXTUREBUFFER,

  WGL_BINDTEXIMAGE,
  WGL_RELEASETEXIMAGE,
  WGL_DESTROYPBUFFER
};

// Structure to hold the name of functions that affect the image state
struct ImageLogData
{
  ImageLogData(const char * funcName, ImageFunctionType funcType, ImageFunctionAction newAction);

  string              functionName;  //The name of the function
  ImageFunctionAction action;        //What the function does and what action to take
  ImageFunctionType   funcType;      //The type of the function (enum version of name)
  int                 functionIndex; //Index of the function (in the function table, -may be -1 if unknown)

};

//The array of built-in known functions
ImageLogData knownImageFunctions[] = 
{
  //General creation/deletion of textures
  ImageLogData("glGenTextures"   ,GL_GENTEXTURES   ,IFA_IMAGE_CREATE),
  ImageLogData("glGenTexturesEXT",GL_GENTEXTURES   ,IFA_IMAGE_CREATE),

  ImageLogData("glDeleteTextures",GL_DELETETEXTURES,IFA_IMAGE_DELETE),
  ImageLogData("glDeleteTexturesEXT",GL_DELETETEXTURES,IFA_IMAGE_DELETE),

  ImageLogData("glBindTexture"   ,GL_BINDTEXTURE   ,IFA_IMAGE_BIND),
  ImageLogData("glBindTextureEXT",GL_BINDTEXTURE   ,IFA_IMAGE_BIND),

  //Loading of texture data
  ImageLogData("glTexImage1D",GL_TEXIMAGE1D,IFA_1D_TEX_READY),
  ImageLogData("glTexImage2D",GL_TEXIMAGE2D,IFA_2D_TEX_READY),
  ImageLogData("glTexImage3D",GL_TEXIMAGE3D,IFA_3D_TEX_READY),
  ImageLogData("glTexImage3DEXT",GL_TEXIMAGE3D,IFA_3D_TEX_READY),

  ImageLogData("glCopyTexImage1D",   GL_COPYTEXIMAGE1D,IFA_1D_TEX_READY),
  ImageLogData("glCopyTexImage1DEXT",GL_COPYTEXIMAGE1D,IFA_1D_TEX_READY),
  ImageLogData("glCopyTexImage2D",   GL_COPYTEXIMAGE2D,IFA_2D_TEX_READY),
  ImageLogData("glCopyTexImage2DEXT",GL_COPYTEXIMAGE2D,IFA_2D_TEX_READY),

  //Conpressed versions of image loads
  ImageLogData("glCompressedTexImage1D",   GL_COMPRESSEDTEXIMAGE1D,IFA_COMPRESS_1D_TEX_READY),
  ImageLogData("glCompressedTexImage1DARB",GL_COMPRESSEDTEXIMAGE1D,IFA_COMPRESS_1D_TEX_READY),
  ImageLogData("glCompressedTexImage2D",   GL_COMPRESSEDTEXIMAGE2D,IFA_COMPRESS_2D_TEX_READY),
  ImageLogData("glCompressedTexImage2DARB",GL_COMPRESSEDTEXIMAGE2D,IFA_COMPRESS_2D_TEX_READY),
  ImageLogData("glCompressedTexImage3D",   GL_COMPRESSEDTEXIMAGE3D,IFA_COMPRESS_3D_TEX_READY),
  ImageLogData("glCompressedTexImage3DARB",GL_COMPRESSEDTEXIMAGE3D,IFA_COMPRESS_3D_TEX_READY),

  //Image dirty functions
  ImageLogData("glTexSubImage1D",   GL_TEXSUBIMAGE1D,IFA_1D_TEX_DIRTY),
  ImageLogData("glTexSubImage1DEXT",GL_TEXSUBIMAGE1D,IFA_1D_TEX_DIRTY),
  ImageLogData("glTexSubImage2D",   GL_TEXSUBIMAGE2D,IFA_2D_TEX_DIRTY),
  ImageLogData("glTexSubImage2DEXT",GL_TEXSUBIMAGE2D,IFA_2D_TEX_DIRTY),
  ImageLogData("glTexSubImage3D",   GL_TEXSUBIMAGE3D,IFA_3D_TEX_DIRTY),
  ImageLogData("glTexSubImage3DEXT",GL_TEXSUBIMAGE3D,IFA_3D_TEX_DIRTY),

  ImageLogData("glCompressedTexSubImage1D",   GL_COMPRESSEDTEXSUBIMAGE1D,IFA_1D_TEX_DIRTY),
  ImageLogData("glCompressedTexSubImage1DARB",GL_COMPRESSEDTEXSUBIMAGE1D,IFA_1D_TEX_DIRTY),
  ImageLogData("glCompressedTexSubImage2D",   GL_COMPRESSEDTEXSUBIMAGE2D,IFA_2D_TEX_DIRTY),
  ImageLogData("glCompressedTexSubImage2DARB",GL_COMPRESSEDTEXSUBIMAGE2D,IFA_2D_TEX_DIRTY),
  ImageLogData("glCompressedTexSubImage3D",   GL_COMPRESSEDTEXSUBIMAGE3D,IFA_3D_TEX_DIRTY),
  ImageLogData("glCompressedTexSubImage3DARB",GL_COMPRESSEDTEXSUBIMAGE3D,IFA_3D_TEX_DIRTY),

  ImageLogData("glCopyTexSubImage1D",   GL_COPYTEXSUBIMAGE1D,IFA_1D_TEX_DIRTY),
  ImageLogData("glCopyTexSubImage1DEXT",GL_COPYTEXSUBIMAGE1D,IFA_1D_TEX_DIRTY),
  ImageLogData("glCopyTexSubImage2D",   GL_COPYTEXSUBIMAGE2D,IFA_2D_TEX_DIRTY),
  ImageLogData("glCopyTexSubImage2DEXT",GL_COPYTEXSUBIMAGE2D,IFA_2D_TEX_DIRTY),
  ImageLogData("glCopyTexSubImage3D",   GL_COPYTEXSUBIMAGE3D,IFA_3D_TEX_DIRTY),
  ImageLogData("glCopyTexSubImage3DEXT",GL_COPYTEXSUBIMAGE3D,IFA_3D_TEX_DIRTY),

  ImageLogData("glTexBuffer",   GL_TEXTUREBUFFER,IFA_1D_TEX_READY),
  ImageLogData("glTexBufferARB",GL_TEXTUREBUFFER,IFA_1D_TEX_READY),

#ifdef GLI_BUILD_WINDOWS

  ImageLogData("wglBindTexImageARB",WGL_BINDTEXIMAGE,IFA_PBUFFER_BIND),
  ImageLogData("wglReleaseTexImageARB",WGL_RELEASETEXIMAGE,IFA_PBUFFER_RELEASE),
  ImageLogData("wglDestroyPbufferARB",WGL_DESTROYPBUFFER,IFA_PBUFFER_DESTROY)
  
#endif //GLI_BUILD_WINDOWS
  
};



#define NUM_IMAGE_LOG_FUNCTIONS sizeof(knownImageFunctions)/sizeof(ImageLogData)


///////////////////////////////////////////////////////////////////////////////
//
ImageLogData::ImageLogData(const char * funcName,ImageFunctionType type, ImageFunctionAction newAction):
functionName(funcName),
action(newAction),
funcType(type),
functionIndex(-1)
{
}


///////////////////////////////////////////////////////////////////////////////
//
InterceptImage::InterceptImage(GLDriver *ogldriver,FunctionTable * functionTable,const ConfigData &configData):
InterceptData(ogldriver),
imgManager(),
imgSaver(configData),
initialized(false),
initFailed(false),

multiTextureSupport(false),
maxNumImageUnits(1),
maxCombinedImageUnits(1),

oglVersion(1.0f),
texture3DSupport(false),
textureCubeMapSupport(false),
textureNVRectSupport(false),
textureArraySupport(false),
textureBufferSupport(false),

extensionARBFragmentProgram(false),
extensionNVFragmentProgram(false),
extensionNVTextureShader(false),
extensionARBGLSLProgram(false),
extensionFBO(false),
maxFBOColorAttachments(0),

numNewTextures(0),
newTextureIDs(NULL),
textureTarget(0),
validTextureReady(false),
validTextureDirty(false),
updateFBOTextures(false),

#ifdef GLI_BUILD_WINDOWS
pBufferHandle(NULL),
pBufferBufType(0),
iwglQueryPbuffer(NULL),
#endif //GLI_BUILD_WINDOWS

iglActiveTexture(NULL),
iglGetHandleARB(NULL),
iglGetFramebufferAttachmentParameterivEXT(NULL)
{

  //Register the image based functions with the function table
  for(uint i=0;i<NUM_IMAGE_LOG_FUNCTIONS;i++)
  {
    //Set the flag
    functionTable->SetFunctionFlag(knownImageFunctions[i].functionName,FDF_IMAGE_LOG);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptImage::~InterceptImage()
{
  //Turn off image saving to flush out-standing textures
  if(GetDataSaving())
  {
    SetDataSaving(false);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptImage::Init()
{
  //Test if we can proceed
  if(!driver || !driver->GetInternalGLCallMode() || initFailed)
  {
    return false;
  }

  //Flag that init has been called
  initialized  = true;

  //Get the openGL version
  oglVersion = driver->GetOpenGLVersion();

  //Check for at least version 1.1
  if(oglVersion < 1.1f)
  {
    LOGERR(("Need OpenGL version 1.1 or greater for image logging"));
    return false;
  }

  //Init the image saver
  if(!imgSaver.Init(driver))
  {
    LOGERR(("Unable to init image saver"));
    return false;
  }

  //Get texture support flags
  if(oglVersion >= 1.2f || driver->IsExtensionSupported("GL_EXT_texture3D"))
  {
    texture3DSupport =true;
  }
  if(oglVersion >= 1.3f || 
     driver->IsExtensionSupported("GL_ARB_texture_cube_map") || 
     driver->IsExtensionSupported("GL_EXT_texture_cube_map"))
  {
    textureCubeMapSupport =true;
  }
  if(oglVersion >= 3.1f ||
     driver->IsExtensionSupported("GL_ARB_texture_rectangle") || 
     driver->IsExtensionSupported("GL_EXT_texture_rectangle") || 
     driver->IsExtensionSupported("GL_NV_texture_rectangle"))
  {
    textureNVRectSupport =true;
  }

  if(oglVersion >= 3.0f ||
     driver->IsExtensionSupported("GL_EXT_texture_array"))
  {
    textureArraySupport = true;
  }

  if(oglVersion >= 3.1f ||
     driver->IsExtensionSupported("GL_ARB_texture_buffer_object"))
  {
    textureBufferSupport = true;
  }

  //Get fragment program support
  extensionARBFragmentProgram = driver->IsExtensionSupported("GL_ARB_fragment_program"); 
  extensionNVFragmentProgram  = driver->IsExtensionSupported("GL_NV_fragment_program");
  extensionNVTextureShader    = driver->IsExtensionSupported("GL_NV_texture_shader");

  //Get FBO support
  extensionFBO = driver->GetOpenGLVersion() >= 3.0f ||
                 driver->IsExtensionSupported("GL_EXT_framebuffer_object") || 
                 driver->IsExtensionSupported("GL_ARB_framebuffer_object");
  if(extensionFBO)
  {
    iglGetFramebufferAttachmentParameterivEXT = (void (GLAPIENTRY *)(GLenum, GLenum, GLenum, GLint *))GLW.glGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
    if(iglGetFramebufferAttachmentParameterivEXT == NULL)
    {
      iglGetFramebufferAttachmentParameterivEXT = (void (GLAPIENTRY *)(GLenum, GLenum, GLenum, GLint *))GLW.glGetProcAddress("glGetFramebufferAttachmentParameteriv");
    }

    //Get the list of attachments in the FBO
    maxFBOColorAttachments = 0;
    GLV.glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxFBOColorAttachments);
  
    //Clamp the result to be safe 
    // (This is clamped to 16 as that is the max the enum values go up to (GL_COLOR_ATTACHMENT15_EXT) 
    //   - if this goes higher in future update code that uses maxFBOColorAttachments and relies on the values being sequental)
    if(maxFBOColorAttachments > 16)
    {
      maxFBOColorAttachments = 16;
    }
    if(maxFBOColorAttachments < 0)
    {
      maxFBOColorAttachments = 0;
    }
  }

  //Get if the old ARB shader object extension is supported 
  if(driver->IsExtensionSupported("GL_ARB_shader_objects"))
  {
    extensionARBGLSLProgram =true;

    //Assign the retrieve handle
    iglGetHandleARB = (GLhandle (GLAPIENTRY *) (GLenum pname))GLW.glGetProcAddress("glGetHandleARB");
  }

  //Get if multi-texturing is supported
  if(oglVersion >= 1.3f || 
     driver->IsExtensionSupported("GL_ARB_multitexture"))
  {
    multiTextureSupport = true;

    //Get the active texture function pointer
    iglActiveTexture = (void (GLAPIENTRY *)(GLenum texture))GLW.glGetProcAddress("glActiveTexture");
    if(!iglActiveTexture)
    {
      iglActiveTexture = (void (GLAPIENTRY *)(GLenum texture))GLW.glGetProcAddress("glActiveTextureARB");
    }
    
    //If a ARB fragment program is availbale, more image units can be used
    if(oglVersion >= 2.0f ||
       driver->IsExtensionSupported("GL_NV_fragment_program") ||
       driver->IsExtensionSupported("GL_ARB_fragment_program") ||
       driver->IsExtensionSupported("GL_ARB_fragment_shader"))
    {
      GLV.glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&maxNumImageUnits);
    }
    else
    {
      //Use the old old interface for older OpenGL (do not call in core profiles)
      GLV.glGetIntegerv(GL_MAX_TEXTURE_UNITS,&maxNumImageUnits);
    }

    // Over-ride bad values
    if(maxNumImageUnits <= 0)
    {
      maxNumImageUnits = 1;
    }

    //Init the combined units variable to be at least the number of image units
    maxCombinedImageUnits = maxNumImageUnits;
  }


  //Get the maximum number of combined image units
  if(oglVersion >= 2.0f ||
     driver->IsExtensionSupported("GL_ARB_vertex_shader"))
  {
    GLV.glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxCombinedImageUnits);
    if(maxCombinedImageUnits < maxNumImageUnits)
    {
      maxCombinedImageUnits = maxNumImageUnits;
    }
  }
  else
  {
    maxCombinedImageUnits = maxNumImageUnits;
  }

#ifdef GLI_BUILD_WINDOWS

  //Get the p-buffer querty function
  // (This is called without checking for the extension as the 
  //  method call will only be made on valid p-buffer calls)
  iwglQueryPbuffer = (BOOL (WINAPI *)(HPBUFFERARB, int, int *))GLW.wglGetProcAddress("wglQueryPbufferARB");

#endif //GLI_WINDOWS

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptImage::DriverAvailable()
{
  //Get the state of the driver
  if(!driver || !driver->GetInternalGLCallMode() || initFailed)
  {
    return false;
  }

  //Test if we have been init
  if(!initialized)
  {
    if(!Init())
    {
      initFailed = true;
      return false;
    }
  }
  
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::SetDataSaving(bool flag)
{
  //If we are disabling saving, write out all pending "ready" textures
  if(!flag)
  {
    SaveCachedReadyImages();
  }

  //Assign the flag
  InterceptData::SetDataSaving(flag);

  //If enabling texture writing, save all currently bound textures
  if(flag && initialized && DriverAvailable())
  {
    //Get all the dirty images
    vector<ImageData *> dirtyImages;
    imgManager.GetAllDirtyData(dirtyImages);

    //Get the old texture stage
    uint oldTexStage = GetActiveTextureStage();

    //Loop for all image units
    for(uint t=0;t<(uint)maxCombinedImageUnits;t++)
    {
      //Set the active texture stage
      SetActiveTextureStage(t);
       
      //Loop for all dirty images
      for(uint i=0;i<dirtyImages.size();i++)
      {
        //If texture is bound and dirty write it out 
        //  (if it is bound to more than one stage it may not be dirty anymore)
        if(dirtyImages[i]->GetGLID() !=0 && dirtyImages[i]->IsDirty() &&
           GetActiveTextureID(dirtyImages[i]->GetGLType()) == dirtyImages[i]->GetGLID())
        {
          SaveTextureImage(dirtyImages[i]);
        }
      }
    }

    //Set the old stage back
    SetActiveTextureStage(oldTexStage);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::SetDataSavingPath(const string &newPath)
{
  //Append the Images directory then pass to the base class
  string path = newPath + "Images" + FileUtils::dirSeparator;

  //Call base class to set
  InterceptData::SetDataSavingPath(path);
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{
  //Return if GL calls cannot be made 
  if(!DriverAvailable())
  {
    return;
  }

  //Determine if this is a render call
  if(driver->GetCurrentContext() != NULL &&
     driver->GetCurrentContext()->IsRenderFuncion(funcData,index,args))
  {
    //If this is a render function, save out all cached "ready" images
    SaveCachedReadyImages();

    //If textures are being saved out, set that any current FBO textures need 
    // to be updated. This will occur after the glEnd() when immediate mode is
    // used as the driver is not available between glBegin/glEnd.
    if(GetDataSaving())
    {
      updateFBOTextures = true;
    }
  }

  //If this is not an imaging function, return now
  if(!funcData->IsFlagSet(FDF_IMAGE_LOG))
  {
    return;
  }

  //Get the action required for the function
  int actionIndex = GetImageActionIndex(funcData->GetName(),index);
  if(actionIndex == -1)
  {
    return;
  }
  ImageFunctionAction action = knownImageFunctions[actionIndex].action;

  //Create a processing version of the arguments
  FunctionArgs newArgs(args);

  //Perform the action
  switch(action)
  {

    case(IFA_1D_TEX_READY) :
    case(IFA_2D_TEX_READY) :
    case(IFA_3D_TEX_READY) :
    case(IFA_COMPRESS_1D_TEX_READY) :
    case(IFA_COMPRESS_2D_TEX_READY) :
    case(IFA_COMPRESS_3D_TEX_READY) :
      SetImageReadyPre(actionIndex,funcData,index,newArgs);
      break;

    case(IFA_1D_TEX_DIRTY) :
    case(IFA_2D_TEX_DIRTY) :
    case(IFA_3D_TEX_DIRTY) :
      SetImageDirtyPre(actionIndex,funcData,index,newArgs);
      break;

    case(IFA_IMAGE_BIND) :
      BindTexturesPre(funcData,index,newArgs);
      break;
    case(IFA_IMAGE_CREATE) :
      CreateTexturesPre(funcData,index,newArgs);
      break;
    case(IFA_IMAGE_DELETE) :
      DeleteTextures(funcData,index,newArgs);
      break;

#ifdef GLI_BUILD_WINDOWS

    case(IFA_PBUFFER_BIND):
      BindPBufferPre(actionIndex,funcData,index,newArgs);
      break;
    case(IFA_PBUFFER_RELEASE):
      ReleasePBufferPre(actionIndex,funcData,index,newArgs);
      break;
    case(IFA_PBUFFER_DESTROY):
      DestroyPBufferPre(actionIndex,funcData,index,newArgs);
      break;
      
#endif //GLI_BUILD_WINDOWS

    default:
       LOGERR(("InterceptImage::LogFunctionPre - Unknown action"));
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return;
  }

  //If saving and an update is required, set any FBO textures to be dirty if necessary
  if(GetDataSaving() && updateFBOTextures)
  {
    SetFBOTexturesDirty();

    //Reset the flag
    updateFBOTextures = false;
  }

  //If this is not an imaging function, return now
  if(!funcData->IsFlagSet(FDF_IMAGE_LOG))
  {
    return;
  }

  //Get the action required for the function
  int actionIndex = GetImageActionIndex(funcData->GetName(),index);
  if(actionIndex == -1)
  {
    return;
  }
  ImageFunctionAction action = knownImageFunctions[actionIndex].action;

  //Perform the action
  switch(action)
  {
    case(IFA_1D_TEX_READY) :
    case(IFA_2D_TEX_READY) :
    case(IFA_3D_TEX_READY) :
    case(IFA_COMPRESS_1D_TEX_READY) :
    case(IFA_COMPRESS_2D_TEX_READY) :
    case(IFA_COMPRESS_3D_TEX_READY) :
      SetImageReadyPost(actionIndex,funcData,index);
      break;

    case(IFA_1D_TEX_DIRTY) :
    case(IFA_2D_TEX_DIRTY) :
    case(IFA_3D_TEX_DIRTY) :
      SetImageDirtyPost(actionIndex,funcData,index);
      break;

    case(IFA_IMAGE_BIND) :
      BindTexturesPost(funcData,index);
      break;
    case(IFA_IMAGE_CREATE) :
      CreateTexturesPost(funcData,index);
      break;
    case(IFA_IMAGE_DELETE) :
      break;

#ifdef GLI_BUILD_WINDOWS

    case(IFA_PBUFFER_BIND):
      BindPBufferPost(actionIndex,funcData,index,retVal);
      break;
    case(IFA_PBUFFER_RELEASE):
      ReleasePBufferPost(actionIndex,funcData,index,retVal);
      break;
    case(IFA_PBUFFER_DESTROY):
      DestroyPBufferPost(actionIndex,funcData,index,retVal);
      break;
      
#endif //GLI_BUILD_WINDOWS

    default:
       LOGERR(("InterceptImage::LogFunctionPost - Unknown action"));
  }

}
  

///////////////////////////////////////////////////////////////////////////////
//
int InterceptImage::GetImageActionIndex(const string &functionName, int funcIndex)
{
  uint i;

  //Check the list to find the index 
  for(i=0;i<NUM_IMAGE_LOG_FUNCTIONS;i++)
  {
    //If the index is the same, we have mapped this function previously 
    if(knownImageFunctions[i].functionIndex == funcIndex)
    {
      return i;
    }
  }

  //If this function has not been called previously, find the name and assign the index
  for(i=0;i<NUM_IMAGE_LOG_FUNCTIONS;i++)
  {
    //If the name is equal, assign the index
    if(knownImageFunctions[i].functionName == functionName)
    {
      knownImageFunctions[i].functionIndex = funcIndex;
      return i;
    }

  }

  //If the function is not found, something has broken with the mapping/registery
  LOGERR(("InterceptImage::GetImageAction - Attempting to log unknown function %s",functionName.c_str()));

  return -1;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::SetFBOTexturesDirty()
{
  //If no FBO support, return now
  if(!extensionFBO)
  {
    return;
  }

  //Get if a FBO is bound
  GLint boundFBO = 0;
  GLV.glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFBO);
  if(boundFBO == 0)
  {
    return;
  }

  //Loop for all color attachments
  for(GLint i=0; i<maxFBOColorAttachments; i++)
  {
    SetFBOAttachmentTextureDirty(GL_COLOR_ATTACHMENT0 + i);
  }
  
  //Set possible depth/stencil attachments
  SetFBOAttachmentTextureDirty(GL_DEPTH_ATTACHMENT);
  SetFBOAttachmentTextureDirty(GL_STENCIL_ATTACHMENT);

}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::SetFBOAttachmentTextureDirty(GLenum currAttachmentCheck)
{
  //Check that the entry point was loaded
  if(iglGetFramebufferAttachmentParameterivEXT == NULL)
  {
    return;
  }
  
  //Get if the attachment type is a texture
  GLint attachType = 0;
  iglGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER, currAttachmentCheck,
                                            GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachType);

  //If the attachment is a texture
  if(attachType == GL_TEXTURE)
  {
    //Get the texture ID
    GLint texID = 0;
    iglGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER, currAttachmentCheck,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &texID);

    //Find the images and set them as "dirty"
    if(texID != 0)
    {
      ImageData * imageData = imgManager.GetData(texID);
      if(imageData != NULL)
      {
        //Flag the image as dirty
        imageData->SetDirty(true);
      }
    }
  }    

}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::CreateTexturesPre(const FunctionData *funcData,uint index, FunctionArgs & args)
{
  //Get the number of texture ID's created
  args.Get(numNewTextures);

  //Get the texture array
  args.Get(newTextureIDs);

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::CreateTexturesPost(const FunctionData *funcData,uint index)
{
  //Check the array
  if(newTextureIDs == NULL || numNewTextures <0)
  {
    LOGERR(("InterceptImage::CreateTexturesPost - Invalid texture ID array"));
    return;
  }
  
  //Loop for the returned texture ID's
  for(uint i=0;i<(uint)numNewTextures;i++)
  {
    imgManager.AddData(newTextureIDs[i]);
  }

  //Reset the arrays
  numNewTextures = 0;
  newTextureIDs  = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::BindTexturesPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the target type
  GLenum newType;
  args.Get(newType);

  //Get the texture id
  GLuint texID;
  args.Get(texID);

  //Check if the texture exists yet or not
  ImageData * imageData = imgManager.GetData(texID);
  if(!imageData)
  {
    //Add the image data to the array
    imgManager.AddData(texID);
  }
  else
  {
    //If the texture exists and is ready and dirty, save it out
    if(imageData->IsReady() && imageData->IsDirty())
    {
      SaveTextureImage(imageData);
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::BindTexturesPost(const FunctionData *funcData,uint funcTableIndex)
{

}



///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::DeleteTextures(const FunctionData *funcData,uint index, FunctionArgs & args)
{
  //Get the count
  GLsizei numTextures;
  args.Get(numTextures);

  //Get the texture array
  const GLuint *textureIDs;
  args.Get(textureIDs);

  //Check the array
  if(textureIDs == NULL || numTextures < 0)
  {
    LOGERR(("InterceptImage::DeleteTextures - Invalid texture ID array"));
    return;
  }

  //Loop for all the textures
  for(uint i=0;i<(uint)numTextures;i++)
  {
    //Attempt to get the texture ID and save the image if it is dirty
    ImageData * imageData = imgManager.GetData(textureIDs[i]);
    if(imageData)
    {
      if(imageData->IsReady() && imageData->IsDirty())
      {
        //Save the dirty image before it is deleted
        SaveTextureImage(imageData);
      }

      //Remove the texture ID
      // (don't remove if not found, as it could be a proxy texture)
      imgManager.RemoveData(textureIDs[i]);
      imageData = NULL;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::SetImageReadyPre(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the function type
  ImageFunctionType funcType = knownImageFunctions[actionIndex].funcType;

  //Indicate that this may not be a valid texture upload call
  validTextureReady = false;

  //DT_TODO: Get mip-level?

  //Get the image type that is now ready
  switch(funcType)
  {
    //Loading of texture data
    case(GL_TEXIMAGE1D) : 
    case(GL_COPYTEXIMAGE1D) :
    case(GL_COMPRESSEDTEXIMAGE1D) :
      {
        //Get the target type (and validate)
        GLenum texType;
        args.Get(texType);
        if(texType == GL_TEXTURE_1D)
        {
          textureTarget = GL_TEXTURE_1D;
          validTextureReady = true;  
        }
        break;
      }
    case(GL_TEXIMAGE2D) :
    case(GL_COPYTEXIMAGE2D) :
    case(GL_COMPRESSEDTEXIMAGE2D) : 
      {
        //Get the target type (and validate)
        GLenum texType;
        args.Get(texType);
        if(texType == GL_TEXTURE_2D)
        {
          textureTarget = GL_TEXTURE_2D;
          validTextureReady = true;  
        }
        //Check for cube map
        if(texType == GL_TEXTURE_CUBE_MAP_POSITIVE_X || texType == GL_TEXTURE_CUBE_MAP_NEGATIVE_X ||
           texType == GL_TEXTURE_CUBE_MAP_POSITIVE_Y || texType == GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ||
           texType == GL_TEXTURE_CUBE_MAP_POSITIVE_Z || texType == GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
        {
          textureTarget = GL_TEXTURE_CUBE_MAP;
          validTextureReady = true;  
        }

        //Check for rect image
        if(texType == GL_TEXTURE_RECTANGLE)
        {
          textureTarget = GL_TEXTURE_RECTANGLE;
          validTextureReady = true;  
        }

        //Check for 1D array image
        if(texType == GL_TEXTURE_1D_ARRAY)
        {
          textureTarget = GL_TEXTURE_1D_ARRAY;
          validTextureReady = true;  
        }

        break;
      }

    case(GL_TEXIMAGE3D) :
    case(GL_COMPRESSEDTEXIMAGE3D) :  
      {
        //Get the target type (and validate)
        GLenum texType;
        args.Get(texType);
        if(texType == GL_TEXTURE_3D ||
           texType == GL_TEXTURE_2D_ARRAY)
        {
          textureTarget = texType;
          validTextureReady = true;  
        }
        break;
      }

    case(GL_TEXTUREBUFFER) :
      {
        //Get the target type (and validate)
        GLenum texType;
        args.Get(texType);
        if(texType == GL_TEXTURE_BUFFER)
        {
          textureTarget = texType;
          validTextureReady = true;  
        }
        break;
      }

    default:
      LOGERR(("InterceptImage::SetImageReadyPre - Invalid function type"));
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::SetImageReadyPost(uint actionIndex,const FunctionData *funcData,uint funcTableIndex)
{
  //The the texture is not ready (ie. proxy texture), return now
  if(!validTextureReady)
  {
    return;
  }

  //Re-set the flag
  validTextureReady = false;

  //Get the current image index for the target
  uint texID = GetActiveTextureID(textureTarget);

  //Get the data for the image
  ImageData * imageData = imgManager.GetData(texID);
  if(!imageData)
  {
    LOGERR(("InterceptImage::SetImageReadyPost - Unknown or invalid image? TexID = %u",texID));
    return;
  }

  //If the image is currently bound as a p-buffer, un-bind it
  if(imageData->IsPBufferBound())
  {
    imageData->SetPBufferBound(false,textureTarget);

    //Indicate that this may be an error
    LOGERR(("Texture %u was bound to a p-buffer but was forced to release on texture upload call",imageData->GetGLID()));
  }

  //Init the image data variables
  imageData->SetReady(textureTarget);

  //If we are currently saving images, cache this image number to be saved later 
  //  (as the "ready" call may take many mip-maps/sides (cube-maps)
  if(GetDataSaving())
  {
    //Loop and get if the image is already in the array
    uint i;
    for(i=0;i<cachedReadyImages.size();i++)
    {
      if(cachedReadyImages[i] == texID)
      {
        break; 
      }
    }

    //If not found, add to the array
    if(i >= cachedReadyImages.size())
    {
      cachedReadyImages.push_back(texID);
    }

  }
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::SetImageDirtyPre(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the function type
  ImageFunctionType funcType = knownImageFunctions[actionIndex].funcType;

  //Init the valid flag
  validTextureDirty =false;
  
  //Determine the function type
  switch(funcType)
  {
    case(GL_TEXSUBIMAGE1D):
    case(GL_COMPRESSEDTEXSUBIMAGE1D):
    case(GL_COPYTEXSUBIMAGE1D):
      {
        //Get the target type (and validate)
        GLenum texType;
        args.Get(texType);
        if(texType == GL_TEXTURE_1D)
        {
          textureTarget = GL_TEXTURE_1D;
          validTextureDirty = true;  
        }
        break;
      }
    case(GL_TEXSUBIMAGE2D):
    case(GL_COMPRESSEDTEXSUBIMAGE2D):
    case(GL_COPYTEXSUBIMAGE2D):
      {
        //Get the target type (and validate)
        GLenum texType;
        args.Get(texType);
        if(texType == GL_TEXTURE_2D)
        {
          textureTarget = GL_TEXTURE_2D;
          validTextureDirty = true;  
        }
        //Check for cube map
        if(texType == GL_TEXTURE_CUBE_MAP_POSITIVE_X || texType == GL_TEXTURE_CUBE_MAP_NEGATIVE_X ||
           texType == GL_TEXTURE_CUBE_MAP_POSITIVE_Y || texType == GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ||
           texType == GL_TEXTURE_CUBE_MAP_POSITIVE_Z || texType == GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
        {
          textureTarget = GL_TEXTURE_CUBE_MAP;
          validTextureDirty = true;  
        }
        //Check for rect texture
        if(texType == GL_TEXTURE_RECTANGLE)
        {
          textureTarget = GL_TEXTURE_RECTANGLE;
          validTextureDirty = true;  
        }
        //Check for 1D array texture
        if(texType == GL_TEXTURE_1D_ARRAY)
        {
          textureTarget = GL_TEXTURE_1D_ARRAY;
          validTextureDirty = true;  
        }

        break;
      }

    case(GL_TEXSUBIMAGE3D):
    case(GL_COMPRESSEDTEXSUBIMAGE3D):
    case(GL_COPYTEXSUBIMAGE3D):
      {
        //Get the target type (and validate)
        GLenum texType;
        args.Get(texType);
        if(texType == GL_TEXTURE_3D ||
           texType == GL_TEXTURE_2D_ARRAY)
        {
          textureTarget = texType;
          validTextureDirty = true;  
        }
        break;
      }
      
    default:
      LOGERR(("InterceptImage::SetImageDirtyPre - Unknown image function call"));
      break;
  }

  //Save the current image if the image we are about to over-write is dirty
  if(validTextureDirty)
  {
    //Get the current image index for the target
    uint texID = GetActiveTextureID(textureTarget);

    //Get the index for the image
    ImageData * imageData = imgManager.GetData(texID);
    if(!imageData)
    {
      LOGERR(("InterceptImage::SetImageDirtyPre - Unknown or invalid image? TexID = %u",texID));
      return;
    }

    //Save the image if ready and dirty
    if(imageData->IsReady() && imageData->IsDirty())
    {
      //Save the dirty image before it is deleted
      SaveTextureImage(imageData);
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::SetImageDirtyPost(uint actionIndex,const FunctionData *funcData,uint funcTableIndex)
{
  //The the texture is not ready (ie. proxy texture), return now
  if(!validTextureDirty)
  {
    return;
  }

  //Re-set the flag
  validTextureDirty = false;

  //Get the current image index for the target
  uint texID = GetActiveTextureID(textureTarget);

  //Get the index for the image
  ImageData * imageData = imgManager.GetData(texID);
  if(!imageData)
  {
    LOGERR(("InterceptImage::SetImageDirtyPost - Unknown or invalid image? TexID = %u",texID));
    return;
  }

  //Check the current image state
  if(!imageData->IsReady() || imageData->GetGLType() != textureTarget)
  {
    LOGERR(("InterceptImage::SetImageDirtyPost - Flagging an image as dirty when it is not ready/init?"));
    return;
  }

  //DT_TODO: Don't flag the image as ditrty if the change occured on a mip-map and we are not saving mip-maps
  //Flag that the image is now dirty 
  imageData->SetDirty(true);

  //Save the image if possible (since it is now dirty)
  SaveTextureImage(imageData);

}

#ifdef GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::BindPBufferPre(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Save the pbuffer handle
  void *dummyHandle;
  args.Get(dummyHandle);
  pBufferHandle = (HPBUFFERARB)dummyHandle;

  //Save the p-buffer handle type
  args.Get(pBufferBufType);
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::BindPBufferPost(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal)
{
  //Return now if the bind call was bad
  BOOL value;
  retVal.Get(value);
  if(value == FALSE)
  {
    return;
  }

  //Get the target type (check for WGL_NO_TEXTURE_ARB)
  int textureTarget;
  if(!iwglQueryPbuffer || !iwglQueryPbuffer(pBufferHandle,WGL_TEXTURE_TARGET_ARB,&textureTarget))
  {
    LOGERR(("InterceptImage::BindPBufferPost  - Unable to get texture target"));
    return;
  }

  //Get the OpenGL texture format from the target
  GLenum glTextureFormat;
  switch(textureTarget)
  {
    case(WGL_TEXTURE_1D_ARB):
      glTextureFormat = GL_TEXTURE_1D;
      break;
    case(WGL_TEXTURE_2D_ARB):
      glTextureFormat = GL_TEXTURE_2D;
      break;
    case(WGL_TEXTURE_CUBE_MAP_ARB):
      glTextureFormat = GL_TEXTURE_CUBE_MAP;
      break;
    case(WGL_TEXTURE_RECTANGLE_NV):
      glTextureFormat = GL_TEXTURE_RECTANGLE;
      break;
    default:
      LOGERR(("InterceptImage::BindPBufferPost  - Unable to determine the texture targets' format"));
      return;
  }

  //Get the current texture ID for that target 
  uint texID = GetActiveTextureID(glTextureFormat);

  //If tex ID == 0 break as tex ID 0 does not have to have a specific texture target
  if(texID == 0)
  {
    LOGERR(("InterceptImage::BindPBufferPost  - P-Buffer logging not supported when binding to texture ID 0"));
    return;
  }

  //Look up texture Id
  ImageData * imageData = imgManager.GetData(texID);
  if(!imageData)
  {
    LOGERR(("InterceptImage::BindPBufferPost - Unknown or invalid image? TexID = %u",texID));
    return;
  }

  //If already bound to a p-buffer, log a warning
  if(imageData->IsPBufferBound())
  {
    LOGERR(("InterceptImage::BindPBufferPost - PBuffer bound to texture more than once? TexID = %u",texID));
  }

  //Set the p-Buffer as bound (log an error if false is returned.)
  if(!imageData->SetPBufferBound(true,glTextureFormat))
  {
    LOGERR(("InterceptImage::BindPBufferPost - PBuffer log bind error. TexID = %u",texID));
  }

  //Set the p-buffer data
  imageData->SetPBufferHandle(pBufferHandle);
  imageData->SetPBufferBufType(pBufferBufType);

  //Write the image out
  if(imageData->IsReady() && imageData->IsDirty())
  {
    SaveTextureImage(imageData);
  }
  
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::ReleasePBufferPre(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Save the pbuffer handle
  void *dummyHandle;
  args.Get(dummyHandle);
  pBufferHandle = (HPBUFFERARB)dummyHandle;

  //Save the p-buffer handle type
  args.Get(pBufferBufType);

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::ReleasePBufferPost(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal)
{
  
  //Return now if the bind call was bad
  BOOL value;
  retVal.Get(value);
  if(value == FALSE)
  {
    return;
  }

  ImageData * imageData = NULL;

  //Get all the images for the passed handle
  vector<ImageData *> pBufferImages;
  imgManager.GetPBufferImages(pBufferHandle,pBufferImages);

  //Look up texture Id
  for(uint i=0;i<pBufferImages.size();i++)
  {
    //If the buffer type equals the delete buffer type, the image is found
    if(pBufferImages[i]->GetPBufferBufType() == pBufferBufType)
    {
      imageData = pBufferImages[i];
      break;
    }
  }

  //Log an error if not found
  if(!imageData)
  {
    LOGERR(("InterceptImage::ReleasePBufferPost - Unknown or invalid image on release?"));
    return;
  }

  //Set the p-Buffer as un-bound 
  imageData->SetPBufferBound(false,imageData->GetGLType());

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::DestroyPBufferPre(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the p-buffer handle
  void *dummyHandle;
  args.Get(dummyHandle);
  pBufferHandle = (HPBUFFERARB)dummyHandle;

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::DestroyPBufferPost(uint actionIndex,const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal)
{
  //Return now if the bind call was bad
  BOOL value;
  retVal.Get(value);
  if(value == FALSE)
  {
    return;
  }

  //Get all the images for the passed handle
  vector<ImageData *> pBufferImages;
  imgManager.GetPBufferImages(pBufferHandle,pBufferImages);

  //Loop an release all the p-buffer images
  for(uint i=0;i<pBufferImages.size();i++)
  {
    //Set the p-Buffer as un-bound 
    pBufferImages[i]->SetPBufferBound(false,pBufferImages[i]->GetGLType());

    //Let the user know of the outstanding bindings.
    LOGERR(("Pbuffer %p was still bound to texture %u on pbuffer delete",pBufferHandle,pBufferImages[i]->GetGLID()));
  }

}

#endif //GLI_BUILD_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//
uint InterceptImage::GetActiveTextureID(GLenum oglTarget)
{
  int retVal = 0;

  //Assume OpenGL 1.1 support
  switch(oglTarget)
  {
    case(GL_TEXTURE_1D) :
        GLV.glGetIntegerv(GL_TEXTURE_BINDING_1D,&retVal);
        break;

    case(GL_TEXTURE_2D) :
        GLV.glGetIntegerv(GL_TEXTURE_BINDING_2D,&retVal);
        break;

    case(GL_TEXTURE_3D) :
        GLV.glGetIntegerv(GL_TEXTURE_BINDING_3D,&retVal);
        break;

    case(GL_TEXTURE_CUBE_MAP) :
        GLV.glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP,&retVal);
        break;

    case(GL_TEXTURE_RECTANGLE) :
        GLV.glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE,&retVal);
        break;

    case(GL_TEXTURE_1D_ARRAY) :
        GLV.glGetIntegerv(GL_TEXTURE_BINDING_1D_ARRAY,&retVal);
        break;

    case(GL_TEXTURE_2D_ARRAY) :
        GLV.glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY,&retVal);
        break;

    case(GL_TEXTURE_BUFFER) :
        GLV.glGetIntegerv(GL_TEXTURE_BINDING_BUFFER,&retVal);
        break;

    default:
        LOGERR(("Unknown texture target %x ",oglTarget));
  }

  return retVal;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptImage::GetActiveTextureID(GLenum oglTarget,uint texStage,uint &retTexID)
{
  //Return if GL calls cannot be made (or not init)
  if(!DriverAvailable())
  {
    return false;
  }

  //Get the old stage
  uint oldTextStage = GetActiveTextureStage();

  //Set the new stage
  if(!SetActiveTextureStage(texStage))
  {
    return false;
  }

  //Get the texture ID
  retTexID = GetActiveTextureID(oglTarget);

  //Reset the texture stage
  SetActiveTextureStage(oldTextStage);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
uint InterceptImage::GetActiveTextureStage()
{
  //Return if GL calls cannot be made (or not init)
  if(!DriverAvailable())
  {
    return 0;
  }

  //If multi-texture is not supported, return 0
  if(!multiTextureSupport)
  {
    return 0;
  }

  //Get the active texture stage
  GLint retTexStage;
  GLV.glGetIntegerv(GL_ACTIVE_TEXTURE,&retTexStage);

  //Subtract the offset
  retTexStage = retTexStage - GL_TEXTURE0;
  return retTexStage;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptImage::SetActiveTextureStage(uint stage)
{
  //Return if GL calls cannot be made (or not init)
  if(!DriverAvailable())
  {
    return false;
  }

  //Check for multitexturing
  if(!multiTextureSupport && stage != 0)
  {
    LOGERR(("InterceptImage::SetActiveTextureStage - Attempt to bind stage %u with no multi-texturing support",stage));
    return false;
  }
  
  // Check the stage number
  if(stage >= (uint)maxCombinedImageUnits)
  {
    LOGERR(("InterceptImage::SetActiveTextureStage - Attempt to bind stage %u (max is %u stage)",stage,maxCombinedImageUnits-1));
    return false;
  }
  
  //If there is no multitexture support (and stage is equal to 0), just return true
  if(!multiTextureSupport)
  {
    return true;
  }

  //If the function is not bound, return false
  if(!iglActiveTexture)
  {
    LOGERR(("InterceptImage::SetActiveTextureStage - Active texture function is NULL"));
    return false;
  }

  //Set the texture stage
  iglActiveTexture(GL_TEXTURE0 + stage);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::GetBoundTextures(BoundTextureArray &retArray)
{
  //Empty the array
  retArray.clear();

  //If the driver is not available, return now
  if(!DriverAvailable())
  {
    return;
  }

  //Assign the number of stages to check (just check them all - modern OpenGL uses shaders)
  uint numCheckStages = maxCombinedImageUnits;

  //Get the old texture stage
  uint oldTexStage = GetActiveTextureStage();

  //Loop for all texture units
  for(uint t=0;t<numCheckStages;t++)
  {
    uint texID=0;

    //Set the active texture stage
    SetActiveTextureStage(t);

    //Get 1D texture
    texID = GetActiveTextureID(GL_TEXTURE_1D);
    if(texID != 0)
    {
      BoundTexture newBoundTexture;
      newBoundTexture.texID = texID;
      newBoundTexture.texStage = t;
      retArray.push_back(newBoundTexture);
    }

    //Get 2D texture
    texID = GetActiveTextureID(GL_TEXTURE_2D);
    if(texID != 0)
    {
      BoundTexture newBoundTexture;
      newBoundTexture.texID = texID;
      newBoundTexture.texStage = t;
      retArray.push_back(newBoundTexture);
    }

    //Get NV rect texture
    if(textureNVRectSupport)
    {
      texID = GetActiveTextureID(GL_TEXTURE_RECTANGLE);
      if(texID != 0)
      {
        BoundTexture newBoundTexture;
        newBoundTexture.texID = texID;
        newBoundTexture.texStage = t;
        retArray.push_back(newBoundTexture);
      }
    }

    //Get 3D texture
    if(texture3DSupport)
    {
      texID = GetActiveTextureID(GL_TEXTURE_3D);
      if(texID != 0)
      {
        BoundTexture newBoundTexture;
        newBoundTexture.texID = texID;
        newBoundTexture.texStage = t;
        retArray.push_back(newBoundTexture);
      }
    }

    //Get cube map texture
    if(textureCubeMapSupport)
    {
      texID = GetActiveTextureID(GL_TEXTURE_CUBE_MAP);
      if(texID != 0)
      {
        BoundTexture newBoundTexture;
        newBoundTexture.texID = texID;
        newBoundTexture.texStage = t;
        retArray.push_back(newBoundTexture);
      }
    }

    if(textureArraySupport)
    {
      texID = GetActiveTextureID(GL_TEXTURE_1D_ARRAY);
      if(texID != 0)
      {
        BoundTexture newBoundTexture;
        newBoundTexture.texID = texID;
        newBoundTexture.texStage = t;
        retArray.push_back(newBoundTexture);
      }
    }

    if(textureArraySupport)
    {
      texID = GetActiveTextureID(GL_TEXTURE_2D_ARRAY);
      if(texID != 0)
      {
        BoundTexture newBoundTexture;
        newBoundTexture.texID = texID;
        newBoundTexture.texStage = t;
        retArray.push_back(newBoundTexture);
      }
    }
    
    if(textureBufferSupport)
    {
      texID = GetActiveTextureID(GL_TEXTURE_BUFFER);
      if(texID != 0)
      {
        BoundTexture newBoundTexture;
        newBoundTexture.texID = texID;
        newBoundTexture.texStage = t;
        retArray.push_back(newBoundTexture);
      }
    }
  }

  //Set the old stage back
  SetActiveTextureStage(oldTexStage);
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::SaveCachedReadyImages()
{
  //Loop for all images on the list
  for(uint i=0;i<cachedReadyImages.size();i++)
  {
    //Get the data for the image
    ImageData * imageData = imgManager.GetData(cachedReadyImages[i]);

    //If the image is valid, ready and dirty 
    //  (It is ok if this is not true as the image may have been saved/deleted 
    //   before this method is called)
    if(imageData && imageData->IsReady() && imageData->IsDirty())
    {
      SaveTextureImage(imageData);
    }
  }

  //Empty the list
  cachedReadyImages.clear();
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptImage::SaveAllDirtyData() 
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return;
  }

  vector<ImageData *> dirtyImages;

  //Get all the dirty images
  imgManager.GetAllDirtyData(dirtyImages);

  //Loop for all images
  for(uint i=0;i<dirtyImages.size();i++)
  {
    //If the save is successful, flag it as non-dirty
    SaveTextureImage(dirtyImages[i]);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptImage::GetTextureFileNames(GLint texID, ImageSaveFiles &retSaveFiles)
{
  //Get the data for the image
  ImageData * imageData = imgManager.GetData(texID);
  if(!imageData)
  {
    LOGERR(("InterceptImage::GetTextureFileNames - Unknown or invalid image? TexID = %u",texID));
    return false;
  }

  //If the image is ready, save it
  if(imageData->IsReady())
  {
    //Save the image if dirty
    if(imageData->IsDirty())
    {
      //If there is an error in saving, return now
      if(!SaveTextureImage(imageData))
      {
        return false;
      }
    }
  
    //Copy the file names
    retSaveFiles = imageData->GetImageSaveFileNames();
    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptImage::SaveTextureImage(ImageData * imageData)  
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return false;
  }

  //If we are not saving images, return now
  if(!GetDataSaving())
  {
    return false;
  }

  //Ensure that the image is ready
  if(!imageData->IsReady())
  {
    LOGERR(("InterceptImage::SaveTextureImage - Texture %u has not been uploaded yet",imageData->GetGLID()));
    return false;
  }

  //Get the current image index for the target
  uint saveTexID = GetActiveTextureID(imageData->GetGLType());
  
  //Ensure the texture is bound
  if(saveTexID != imageData->GetGLID())
  {
    //Bind the texture
    GLV.glBindTexture(imageData->GetGLType(),imageData->GetGLID());
  }

  //Get the file base name
  string imageName;
  if(!imageData->GetUniqueFileName(imageName))
  {
    return false;
  }

  //Append the images's directory name
  imageName = GetDataSavingPath() + imageName;

  //Write the image out
  ImageSaveFiles retFileNames;
  if(!imgSaver.SaveImage(imageName,imageData,retFileNames))
  {
    LOGERR(("InterceptImage::SaveTextureImage - Error saving image %u",imageData->GetGLID()));
  }

  //Assign the return file names
  imageData->GetImageSaveFileNames() = retFileNames;

  //Flag that the image is no longer dirty 
  //   (set this reguardless or whether image saving was successful)
  imageData->SetDirty(false);

  //Reset the bound texture
  if(saveTexID != imageData->GetGLID())
  {
    //Restore texture properties
    GLV.glBindTexture(imageData->GetGLType(),saveTexID);
  }

  return true;
}
