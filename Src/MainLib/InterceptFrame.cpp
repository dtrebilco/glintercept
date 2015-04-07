/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptFrame.h"
#include "FileUtils.h"
#include "GLDriver.h"
#include "ImageSaveManager.h"
#include "MovieMaker.h"

//The real OpenGL driver
extern GLCoreDriver GLV;
extern WGLDriver    GLW;

// NOTE: This logger does not currently handle WGL_ARB_make_current_read, add support?


USING_ERRORLOG

///////////////////////////////////////////////////////////////////////////////
//
FrameInterceptFileNames::FrameInterceptFileNames():
numColorBuffers(1)
{
  //Reset
  Reset();
}

///////////////////////////////////////////////////////////////////////////////
//
void FrameInterceptFileNames::Reset()
{
  //Set all file names to be empty
  for(uint i=0;i<FIDT_MAX;i++)
  {
    depthBufNames[i]   = "";
    stencilBufNames[i] = "";

    depthBufNamesIcon[i]   = "";
    stencilBufNamesIcon[i] = "";

    //Loop for all possible color buffers
    for(uint i2=0; i2<MAX_COLOR_BUFFERS; i2++)
    {
      colorBufNames    [i2][i]   = "";
      colorBufNamesIcon[i2][i]   = "";
    }
  }

  //Note: The movie frame name is not reset
}

///////////////////////////////////////////////////////////////////////////////
//
FrameInterceptFileNames & FrameInterceptFileNames::operator=(const FrameInterceptFileNames & src)
{
  //Assign the single values
  numColorBuffers = src.numColorBuffers;
  movieFileName   = src.movieFileName;

  //Loop and assign all the buffer file names
  for(uint i=0;i<FIDT_MAX;i++)
  {
    depthBufNames[i]   = src.depthBufNames[i];
    stencilBufNames[i] = src.stencilBufNames[i];

    depthBufNamesIcon[i]   = src.depthBufNamesIcon[i];
    stencilBufNamesIcon[i] = src.stencilBufNamesIcon[i];

    //Loop for all possible color buffers
    for(uint i2=0; i2<MAX_COLOR_BUFFERS; i2++)
    {
      colorBufNames    [i2][i] = src.colorBufNames    [i2][i];
      colorBufNamesIcon[i2][i] = src.colorBufNamesIcon[i2][i];
    }
  }
 
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptFrame::SaveFrameData::SaveFrameData():
savePre(false),
savePost(false),
saveDiff(false),
preData(NULL),
postData(NULL),
diffData(NULL),
bufType(GL_NONE),
bufferCounter(0)
{
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptFrame::SaveFrameData::SaveFrameData(GLenum type, bool pre,bool post,bool diff):
savePre(pre),
savePost(post),
saveDiff(diff),
preData(NULL),
postData(NULL),
diffData(NULL),
bufType(type),
bufferCounter(0)
{
}


///////////////////////////////////////////////////////////////////////////////
//
InterceptFrame::SaveFrameData::~SaveFrameData()
{
  ResetBuffers();
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptFrame::SaveFrameData::ResetBuffers()
{
  //Delete the buffers
  if(preData)
  { 
    delete preData;
    preData = NULL;
  }
  if(postData)
  { 
    delete postData;
    postData = NULL;
  }
  if(diffData)
  {
    delete diffData;
    diffData = NULL;
  }
}



///////////////////////////////////////////////////////////////////////////////
//
InterceptFrame::InterceptFrame(GLDriver *ogldriver,FunctionTable * functionTable,const ConfigData &configData):
InterceptData(ogldriver),

initialized(false),
initFailed(false),

extensionDrawBuffers(false),
extensionPBO(false),
extensionFBO(false),
numDrawBuffers(1),

imageExtension(configData.frameImageFormat),
inBeginEndSection(false),
isRenderFunction(false),

saveIconFiles(configData.frameIconSave),
iconSize(configData.frameIconSize),
iconExtension(configData.frameIconImageFormat),

iglBindBuffer(NULL),
iglGetFramebufferAttachmentParameteriv(NULL),
iglBindFramebuffer(NULL),

depthSaveData(GL_DEPTH_COMPONENT, configData.framePreDepthSave,configData.framePostDepthSave,configData.frameDiffDepthSave),
stencilSaveData(GL_STENCIL_INDEX, configData.framePreStencilSave,configData.framePostStencilSave,configData.frameDiffStencilSave),

movieSaveEnabled(configData.frameMovieEnabled),
movieInitAttempt(false),
currFrameMovie(NULL),
movieWidth(configData.frameMovieWidth),
movieHeight(configData.frameMovieHeight),
movieFrameRate(configData.frameMovieRate),
movieCodecs(configData.frameMovieCodecs)

{

  //Init the defaults for the color buffer array
  for(uint colorNum=0; colorNum<MAX_COLOR_BUFFERS; colorNum++)
  {
    colorSaveData[colorNum].savePre  = configData.framePreColorSave;
    colorSaveData[colorNum].savePost = configData.framePostColorSave;
    colorSaveData[colorNum].saveDiff = configData.frameDiffColorSave;

    //Set the color buffer counter for multi-render targets
    colorSaveData[colorNum].bufType       = GL_RGBA;
    colorSaveData[colorNum].bufferCounter = colorNum;
  }

  udword currColor = 0xFFFFFFFF;
  for(uint i=0;i<IF_NUM_STENCIL_COLORS;i++)
  {
    //Assign the direct index as the defuault value
    currColor = 0xFF000000 | (i << 16) | (i << 8) | i;

    //Loop for all the stencil index/color pairs
    for(uint i2=0;i2<configData.frameStencilColors.size();i2+=2)
    {
      //Stop when reaching a color index that is greater that the current index
      if(configData.frameStencilColors[i2] > i)
      {
        break;
      }

      //Assign the current color if the index matches
      if(configData.frameStencilColors[i2] == i &&
         i2+1 < configData.frameStencilColors.size())
      {
        currColor = configData.frameStencilColors[i2+1];
        break;
      }
    }

    //Set the current stencil color
    stencilColorLookup[i] = currColor;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
InterceptFrame::~InterceptFrame()
{
  //Delete any buffers
  ResetBuffers();

  //Delete any frame movie
  if(currFrameMovie)
  {
    delete currFrameMovie;
    currFrameMovie = NULL;
  }

}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptFrame::Init()
{
  //Test if we can proceed
  if(!driver || !driver->GetInternalGLCallMode() || initFailed)
  {
    return false;
  }

  //Flag that init has been called
  initialized  = true;

  //Check extension support
  extensionDrawBuffers = false;
  numDrawBuffers       = 1; 

  //Get OpenGL 2.0 or ARB/ATI draw buffer support
  if(driver->GetOpenGLVersion() >= 2.0f || 
     driver->IsExtensionSupported("GL_ARB_draw_buffers") ||
     driver->IsExtensionSupported("GL_ATI_draw_buffers"))
  {
    extensionDrawBuffers = true;

    //Get the maximum number of draw buffers
    GLV.glGetIntegerv(GL_MAX_DRAW_BUFFERS, &numDrawBuffers);

    //Clamp to the max
    if(numDrawBuffers > MAX_COLOR_BUFFERS)
    {
      numDrawBuffers = MAX_COLOR_BUFFERS;
    }
    
    //Clamp to the min
    if(numDrawBuffers < 1)
    {
      numDrawBuffers = 1;
    }
  }

  //Set the excess number of draw buffers to false
  for(uint i=numDrawBuffers; i<MAX_COLOR_BUFFERS; i++)
  {
    colorSaveData[i].savePre  = false;
    colorSaveData[i].savePost = false;
    colorSaveData[i].saveDiff = false;
  }

  //Assign the number of color buffer filenames in use
  saveFileNames.numColorBuffers = numDrawBuffers;


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

  // Get the frame buffer extensions entry points (only use ARB/core version of FBO)
  if(driver->GetOpenGLVersion() >= 3.0f ||
     driver->IsExtensionSupported("GL_ARB_framebuffer_object"))
  {
    // Only set this as true if the core or ARB version of FBO is supported
    extensionFBO = true;

    iglGetFramebufferAttachmentParameteriv = (void (GLAPIENTRY *)(GLenum, GLenum, GLenum, GLint *))GLW.glGetProcAddress("glGetFramebufferAttachmentParameteriv");
    iglBindFramebuffer = (void (GLAPIENTRY *)(GLenum, GLuint))GLW.glGetProcAddress("glBindFramebuffer");
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptFrame::DriverAvailable()
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
void InterceptFrame::SetDataSaving(bool flag)
{
  //Test for flag change
  if(GetDataSaving() != flag)
  {
    //Assign the flag
    InterceptData::SetDataSaving(flag);

    //Reset other flags
    inBeginEndSection = false;

    //If saving movies of the frames
    if(movieSaveEnabled)
    {
      //End any current movie
      if(currFrameMovie)
      {
        delete currFrameMovie;
        currFrameMovie = NULL;
      }

      //If enabling
      if(flag)
      {
        //Flag that any current attempt is false
        movieInitAttempt = false;
      }
    }

  }
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptFrame::SetDataSavingPath(const string &newPath)
{
  //Append the FrameBuffer directory then pass to the base class
  string path = newPath + "FrameBuffer" + FileUtils::dirSeparator;

  //Call base class to set
  InterceptData::SetDataSavingPath(path);

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptFrame::ResetBuffers()
{
  //Delete any outstanding data
  for(uint i=0; i<MAX_COLOR_BUFFERS; i++)
  {
    colorSaveData[i].ResetBuffers();
  }

  depthSaveData.ResetBuffers();
  stencilSaveData.ResetBuffers();
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptFrame::LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{
  //Reset the flag
  isRenderFunction = false;

  //Return if GL calls cannot be made  or it is not a render function
  if(!DriverAvailable() || 
     !driver->GetCurrentContext() ||
     !driver->GetCurrentContext()->IsRenderFuncion(funcData,index,args))
  {
    return;
  }

  //Clear old file names
  saveFileNames.Reset();

  //Only proceed if frame saving is enabled
  if(!GetDataSaving())
  {
    return;
  }
  
  //Flag to the "post" function that this is a render function
  isRenderFunction = true;

  //Store a unique save count number
  static uint saveCount=0;
  saveCount++;

  //Get the pre color buffers
  for(GLint i=0; i<numDrawBuffers; i++) 
  {
    GetBufferDataPre(&colorSaveData[i], saveFileNames.colorBufNames[i], saveFileNames.colorBufNamesIcon[i], saveCount);
  }

  //Get the pre depth buffers 
  GetBufferDataPre(&depthSaveData, saveFileNames.depthBufNames, saveFileNames.depthBufNamesIcon, saveCount);

  //Get the pre stencil buffers 
  GetBufferDataPre(&stencilSaveData, saveFileNames.stencilBufNames, saveFileNames.stencilBufNamesIcon, saveCount);

  //Initialize the movie frame 
  if(movieSaveEnabled && !currFrameMovie)
  {
    InitMovieFrame();
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptFrame::LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{
  //If in a begin/end block, set the flag
  if(driver->GetBeginEndState())
  {
    //Set the flag to do the post render later
    if(isRenderFunction)
    {
      inBeginEndSection = true;
    }
    return;
  }

  //Return if GL calls cannot be made or frame saving is disabled 
  if(!DriverAvailable() || !GetDataSaving())
  {
    return;  
  }

  //Return if it is not a render call 
  //  (and the frame did not just came off a begin/end block)
  if(!isRenderFunction && !inBeginEndSection)
  {
    return;
  }

  //Reset the flag
  inBeginEndSection = false;

  //Get the post color buffers 
  for(GLint i=0; i<numDrawBuffers; i++)
  {
    GetBufferDataPost(&colorSaveData[i], saveFileNames.colorBufNames[i], saveFileNames.colorBufNamesIcon[i]);
  }

  //Get the post depth buffers 
  GetBufferDataPost(&depthSaveData, saveFileNames.depthBufNames, saveFileNames.depthBufNamesIcon);

  //Get the post stencil buffers 
  GetBufferDataPost(&stencilSaveData, saveFileNames.stencilBufNames, saveFileNames.stencilBufNamesIcon);

  //If movie saving is enabled, add the frame data
  if(movieSaveEnabled)
  {
    AddMovieFrame();
  }

  //Reset all buffers
  ResetBuffers();
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptFrame::GetBufferDataPre(SaveFrameData *bufferData, string saveNames[FIDT_MAX], string saveNamesIcon[FIDT_MAX], uint saveCount) 
{
  //Return now if none of the buffers are being saved
  if(!bufferData->savePre && !bufferData->savePost && !bufferData->saveDiff)
  {
    return;
  }
  
  //Reset the save names
  saveNames[FIDT_PRE_FRAME]  = "";
  saveNames[FIDT_POST_FRAME] = "";
  saveNames[FIDT_DIFF_FRAME] = "";

  saveNamesIcon[FIDT_PRE_FRAME]  = "";
  saveNamesIcon[FIDT_POST_FRAME] = "";
  saveNamesIcon[FIDT_DIFF_FRAME] = "";


  //Return now if the buffer type is not readable
  if(!IsBufferReadable(bufferData->bufType, bufferData->bufferCounter))
  {
    return;
  }

  //Get the frame filename start from the type
  string bufString;
  switch(bufferData->bufType)
  {
    case(GL_RGBA):
      StringPrintF(bufString,"%sFrameColor_%04u_%04u_%u_",GetDataSavingPath().c_str(),
                                                          driver->GetFrameNumber(), saveCount, 
                                                          bufferData->bufferCounter);
      break;
    case(GL_DEPTH_COMPONENT):
      StringPrintF(bufString,"%sFrameDepth_%04u_%04u_",GetDataSavingPath().c_str(),
                                                       driver->GetFrameNumber(), saveCount);
      break;
    case(GL_STENCIL_INDEX):
      StringPrintF(bufString,"%sFrameStencil_%04u_%04u_",GetDataSavingPath().c_str(),
                                                         driver->GetFrameNumber(), saveCount);
      break;
    default:
      StringPrintF(bufString,"%sFrame_%04u_%04u_",GetDataSavingPath().c_str(),
                                                  driver->GetFrameNumber(), saveCount);
      break;
  }


  //Assign buffer names
  if(bufferData->savePre)
  {
    saveNames[FIDT_PRE_FRAME]  = bufString + "Pre." + imageExtension;
    if(saveIconFiles)
    {
      saveNamesIcon[FIDT_PRE_FRAME]  = bufString + "Pre_Icon." + iconExtension;
    }
  }
  if(bufferData->savePost)
  {
    saveNames[FIDT_POST_FRAME] = bufString + "Post." + imageExtension;
    if(saveIconFiles)
    {
      saveNamesIcon[FIDT_POST_FRAME]  = bufString + "Post_Icon." + iconExtension;
    }

  }
  if(bufferData->saveDiff)
  {
    saveNames[FIDT_DIFF_FRAME] = bufString + "Diff." + imageExtension;
    if(saveIconFiles)
    {
      saveNamesIcon[FIDT_DIFF_FRAME]  = bufString + "Diff_Icon." + iconExtension;
    }
  }

  //Get the pre-buffers
  if(bufferData->savePre || bufferData->saveDiff)
  {
    bufferData->preData = GetBuffer(bufferData->bufType, bufferData->bufferCounter);
  }

  //Empty the file name of buffers that could not be retrieved
  if(bufferData->preData == NULL)
  {
    saveNames[FIDT_PRE_FRAME] = "";
    saveNames[FIDT_DIFF_FRAME] = "";

    saveNamesIcon[FIDT_PRE_FRAME] = "";
    saveNamesIcon[FIDT_DIFF_FRAME] = "";
  }

  //Save the buffer if it can be saved
  if(bufferData->preData && saveNames[FIDT_PRE_FRAME].length() > 0)
  {
    //Attempt to save
    if(!SaveBuffer(saveNames[FIDT_PRE_FRAME],saveNamesIcon[FIDT_PRE_FRAME],bufferData->preData))
    {
      saveNames[FIDT_PRE_FRAME] = "";
      saveNamesIcon[FIDT_PRE_FRAME] = "";
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptFrame::GetBufferDataPost(SaveFrameData *bufferData,string saveNames[FIDT_MAX], string saveNamesIcon[FIDT_MAX]) const
{

  //Return now if none of the buffers are being saved
  if(!bufferData->savePre && !bufferData->savePost && !bufferData->saveDiff)
  {
    return;
  }

  //Return now if the buffer type is not readable
  if(!IsBufferReadable(bufferData->bufType, bufferData->bufferCounter))
  {
    return;
  }

  //Get the post buffers
  if(bufferData->savePost || bufferData->saveDiff)
  {
    bufferData->postData = GetBuffer(bufferData->bufType, bufferData->bufferCounter);
  }

  //Calculate the diffs
  if(bufferData->saveDiff && bufferData->preData && bufferData->postData)
  {
    bufferData->diffData = CalculateImageDiff(bufferData->preData,bufferData->postData);
  }

  //Empty the file name of buffers that could not be retrieved
  if(bufferData->postData == NULL)
  {
    saveNames[FIDT_POST_FRAME] = "";
    saveNamesIcon[FIDT_POST_FRAME] = "";
  }
  if(bufferData->diffData == NULL)
  {
    saveNames[FIDT_DIFF_FRAME] = "";
    saveNamesIcon[FIDT_DIFF_FRAME] = "";
  }

  //Save post buffers
  if(bufferData->postData && saveNames[FIDT_POST_FRAME].length() > 0)
  {
    //Attempt to save
    if(!SaveBuffer(saveNames[FIDT_POST_FRAME],saveNamesIcon[FIDT_POST_FRAME],bufferData->postData))
    {
      saveNames[FIDT_POST_FRAME] = "";
      saveNamesIcon[FIDT_POST_FRAME] = "";
    }
  }

  //Save diffs
  if(bufferData->diffData && saveNames[FIDT_DIFF_FRAME].length() > 0)
  {
    //Attempt to save
    if(!SaveBuffer(saveNames[FIDT_DIFF_FRAME],saveNamesIcon[FIDT_DIFF_FRAME],bufferData->diffData))
    {
      saveNames[FIDT_DIFF_FRAME] = "";
      saveNamesIcon[FIDT_POST_FRAME] = "";
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptFrame::SaveBuffer(const string &fileName, const string &fileNameIcon, corona::Image * saveBuffer) const
{
  //Write the file out
  if(!corona::SaveImage(fileName.c_str(),corona::FF_AUTODETECT,saveBuffer))
  {
    LOGERR(("InterceptFrame::SaveBuffer - Unable to save image %s",fileName.c_str()));
    return false;
  }

  //If saving icon files
  if(saveIconFiles && fileNameIcon.length() > 0)
  {
    //Generate the icon file
    corona::Image *iconBuffer = ImageSaveManager::ScaleImage(iconSize, iconSize, saveBuffer);
    if(!iconBuffer)
    {
      LOGERR(("InterceptFrame::SaveBuffer - Unable generate icon for file %s",fileName.c_str()));
      return false;
    }

    //Save the icon
    if(!corona::SaveImage(fileNameIcon.c_str(),corona::FF_AUTODETECT,iconBuffer))
    {
      LOGERR(("InterceptFrame::SaveBuffer - Unable to save image %s",fileNameIcon.c_str()));
      delete iconBuffer;
      return false;
    }

    //Delete the scaled image
    delete iconBuffer;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptFrame::IsBufferReadable(GLenum bufType, uint bufferCount) const
{
  // Get if a FBO is bound
  GLint boundFBO = 0;
  if(extensionFBO)
  {
    GLV.glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFBO);

    if(boundFBO != 0)
    {
      // If there is multisampling - don't try and get the buffer (for now)
      GLint sampleBuffers = 0;
      GLV.glGetIntegerv(GL_SAMPLE_BUFFERS, &sampleBuffers);

      if(sampleBuffers > 0)
      {
        // Only log the message once
        static bool saveFBOmessage = true;
        if(saveFBOmessage)
        {
          saveFBOmessage = false;
          LOGERR(("InterceptFrame - Unable to save out multisample FBOs"));
        }

        return false;
      }
    }
  }

  //Determine if the requested buffer can be retrieved
  switch(bufType)
  {
    case(GL_RGBA) : 
      {
        //If draw buffers supported
        GLint currDrawBuffer = GL_NONE;
        if(extensionDrawBuffers)
        {
          //Get the draw attachment
          GLV.glGetIntegerv(GL_DRAW_BUFFER0 + bufferCount, &currDrawBuffer);
          
          //If GL_NONE return
          if(currDrawBuffer == GL_NONE)
          {
            return false;
          }
        }

        // If there is a frame buffer attachment function
        if(iglGetFramebufferAttachmentParameteriv)
        {
          if(boundFBO == 0)
          {
            // No FBO bound, return true by default
            return true;
          }
          
          if(currDrawBuffer >= GL_COLOR_ATTACHMENT0 &&
             currDrawBuffer <= GL_COLOR_ATTACHMENT15)
          {
            // Check that there exists a FBO color attachment at the draw buffer
            GLint attachType = GL_NONE;
            iglGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, currDrawBuffer,
                                                   GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachType);
            if(attachType != GL_NONE)
            {
              return true;
            }
          }
        }
        else
        {
          //Get the number of color bits
          GLint redBits   =0;
          GLint greenBits =0;
          GLint blueBits  =0;
          GLint alphaBits =0;

          //Only proceed if there is some color data
          GLV.glGetIntegerv(GL_RED_BITS,  &redBits);
          if(redBits > 0)
          {
            return true;
          }
          GLV.glGetIntegerv(GL_GREEN_BITS,&greenBits);
          if(greenBits > 0)
          {
            return true;
          }
          GLV.glGetIntegerv(GL_BLUE_BITS, &blueBits);
          if(blueBits > 0)
          {
            return true;
          }
          GLV.glGetIntegerv(GL_ALPHA_BITS, &alphaBits);
          if(alphaBits > 0)
          {
            return true;
          }
        }
      }
      break;

    case(GL_DEPTH_COMPONENT) : 
      {
        if(iglGetFramebufferAttachmentParameteriv)
        {
          // Test if FBO is bound 
          if(boundFBO == 0)
          {
            // No FBO bound, return true by default
            return true;
          }

          // Test if the FBO has a depth buffer
          GLint attachType = GL_NONE;
          iglGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                                 GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachType);
          if(attachType != GL_NONE)
          {
            return true;
          }
        }
        else
        {
          //Get the number of depth bits
          GLint numDepthBits=0;
          GLV.glGetIntegerv(GL_DEPTH_BITS,&numDepthBits);
    
          //Only proceed if there is a depth buffer
          if(numDepthBits > 0)
          {
            return true;
          }
        }
      }
      break;

    case(GL_STENCIL_INDEX) : 
      {
        if(iglGetFramebufferAttachmentParameteriv)
        {
          // Test if FBO is bound 
          if(boundFBO == 0)
          {
            // No FBO bound, return true by default
            return true;
          }

          // Test if the FBO has a stencil buffer
          GLint attachType = GL_NONE;
          iglGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                                                 GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachType);
          if(attachType != GL_NONE)
          {
            return true;
          }
        }
        else
        {
          //Get the number of stencil bits
          GLint numStencilBits=0;
          GLV.glGetIntegerv(GL_STENCIL_BITS,&numStencilBits);
          if(numStencilBits > 0)
          {
            return true;
          }
        }
      }
      break;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
corona::Image *InterceptFrame::GetBuffer(GLenum bufType, uint bufferCount) const
{
  //Get the size of the viewport
  GLint viewSize[4];
  GLV.glGetIntegerv(GL_VIEWPORT,&viewSize[0]);

  //If the viewport does not have a size, return NULL
  if(viewSize[2] == 0 || viewSize[3] == 0)
  {
    return NULL;
  }

  //Allocate the return image
  corona::Image * newImage = corona::CreateImage(viewSize[2],viewSize[3],corona::PF_R8G8B8A8);
  if(!newImage)
  {
    return NULL;
  }

  // Get if the draw FBO (if there is one) is different from the read FBO
  GLint oldDrawFBO = 0;
  GLint oldReadFBO = 0;
  if(extensionFBO)
  {
    GLV.glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &oldDrawFBO);
    GLV.glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &oldReadFBO);
    
    // Bind the read FBO as the current draw FBO
    if(oldDrawFBO != oldReadFBO)
    {
      iglBindFramebuffer(GL_READ_FRAMEBUFFER, oldDrawFBO);
    }
  }

  //Set pixel transfer modes
  GLint oldPackAlignment = 1, 
        oldLsbFirst = 0,
        oldRowLength = 0, 
        oldSkipPixels = 0,
        oldSkipRows = 0,
        oldSwapBytes = 0; 

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
 
  //Read the data based on the buffer type
  if(bufType == GL_RGBA)
  {
    //Ensure we read from the same buffer that was written to
    GLint oldReadBuffer = GL_NONE, drawBuffer = GL_NONE;

    //Get the current read buffer type
    GLV.glGetIntegerv(GL_READ_BUFFER,&oldReadBuffer);

    //If draw buffers supported
    if(extensionDrawBuffers)
    {
      //Get the draw attachment
      GLV.glGetIntegerv(GL_DRAW_BUFFER0 + bufferCount, &drawBuffer);
    }
    else
    {
      //Get the old drawbuffer value
      GLV.glGetIntegerv(GL_DRAW_BUFFER, &drawBuffer);
    }

    //If there is a buffer to retrieve, get it
    if(drawBuffer != GL_NONE)
    {
      //Set the read buffer to the current draw buffer if necessary
      if(oldReadBuffer != drawBuffer)
      {
        GLV.glReadBuffer(drawBuffer);
      }

      //Read the pixels //DT_TODO: What about ARB imaging modifying the return values? (see glReadPixels,glPixelTransfer)
      GLV.glReadPixels(viewSize[0],viewSize[1],viewSize[2],viewSize[3],bufType,GL_UNSIGNED_BYTE,newImage->getPixels());

      //Restore the read buffer (if it was changed)
      if(oldReadBuffer != drawBuffer)
      {
        GLV.glReadBuffer(oldReadBuffer);
      }
    }
  }
  else if(bufType == GL_DEPTH_COMPONENT)
  {
    //DT_TODO: What about depth_bias and depth_scale
    //Allocate a retrieval array
    GLubyte * depthBuffer = new GLubyte[(viewSize[2] *viewSize[3])];
  
    //Read the pixels 
    GLV.glReadPixels(viewSize[0],viewSize[1],viewSize[2],viewSize[3],bufType,GL_UNSIGNED_BYTE,&depthBuffer[0]);

    //Pack the return data into the array
    GLubyte * srcImg   = &depthBuffer[0];
    GLubyte * dstImg   = (GLubyte *)newImage->getPixels();
    for(uint i=0;i<(uint)newImage->getWidth()*newImage->getHeight();i++)
    {
      //Copy the data
      dstImg[0] = *srcImg;
      dstImg[1] = *srcImg;
      dstImg[2] = *srcImg;
      dstImg[3] = 0xFF;

      //Increment the counters
      dstImg+=4;
      srcImg++;
    }

    //Delete the array
    delete [] depthBuffer;
  }
  else if(bufType == GL_STENCIL_INDEX)
  {
    //DT_TODO - Disable map stencil? remove index shift/offset? (see glReadPixels, glPixelTransfer)
  
    //Allocate a retrieval array
    GLubyte * stencilBuffer = new GLubyte[(viewSize[2] *viewSize[3])];
  
    //Read the pixels 
    GLV.glReadPixels(viewSize[0],viewSize[1],viewSize[2],viewSize[3],bufType,GL_UNSIGNED_BYTE,&stencilBuffer[0]);

    //Pack the return data into the array
    GLubyte  * srcImg   = &stencilBuffer[0];
    udword   * dstImg   = (udword *)newImage->getPixels();
    for(uint i=0;i<(uint)newImage->getWidth()*newImage->getHeight();i++)
    {
      //Copy the data
      uint currValue = *srcImg;
      if(currValue < IF_NUM_STENCIL_COLORS)
      {
        *dstImg = stencilColorLookup[currValue];
      }
      else
      {
        *dstImg = 0xFFFFFFFF;
      }

      //Increment the counters
      dstImg++;
      srcImg++;
    }

    //Delete the array
    delete [] stencilBuffer;
  }

  //If PBO support, check if a GL_PIXEL_PACK_BUFFER was bound and restore it
  if(extensionPBO && oldPBO != 0)
  {
    iglBindBuffer(GL_PIXEL_PACK_BUFFER, oldPBO); 
  }

  //Restore transfer mode
  GLV.glPixelStorei(GL_PACK_ALIGNMENT,  oldPackAlignment); 
  GLV.glPixelStorei(GL_PACK_LSB_FIRST,  oldLsbFirst); 

  GLV.glPixelStorei(GL_PACK_ROW_LENGTH, oldRowLength); 
  GLV.glPixelStorei(GL_PACK_SKIP_PIXELS,oldSkipPixels); 
  
  GLV.glPixelStorei(GL_PACK_SKIP_ROWS,  oldSkipRows); 
  GLV.glPixelStorei(GL_PACK_SWAP_BYTES, oldSwapBytes); 

  // Restore the read FBO if necessary
  if(oldDrawFBO != oldReadFBO)
  {
    iglBindFramebuffer(GL_READ_FRAMEBUFFER, oldReadFBO);
  }

  //Flip the image 
  corona::FlipImage(newImage,corona::CA_X);

  //Return the image
  return newImage;

}

///////////////////////////////////////////////////////////////////////////////
//
corona::Image *InterceptFrame::CalculateImageDiff(corona::Image * src1,corona::Image * src2) const
{
  //If the image sizes are not the same, return 
  if(!src1 || !src2 ||
     src1->getWidth() != src2->getWidth() ||
     src1->getHeight() != src2->getHeight() ||
     src1->getFormat() != corona::PF_R8G8B8A8 || src2->getFormat() != corona::PF_R8G8B8A8)
  {
    LOGERR(("InterceptFrame::CalculateImageDiff - Invalid width/height/format"));
    return NULL;
  }

  //Create a new image of the requested dimensions
  corona::Image * newImage = corona::CreateImage(src1->getWidth(),src1->getHeight(),corona::PF_R8G8B8A8);
  if(!newImage)
  {
    return NULL;
  }

  //Flag to indcate if the images are identical
  bool noDiff = true;

  //Get the starting pointers
  udword * src1Img  = (udword *)src1->getPixels();
  udword * src2Img  = (udword *)src2->getPixels();
  udword * dstImg   = (udword *)newImage->getPixels();

  //Loop for all image elements
  for(uint i=0; i<(uint)newImage->getWidth()*newImage->getHeight();i++)
  {
    //Check if the images are different
    if(*src1Img != *src2Img)
    {
      //Assign the destination from the second image for a difference
      *dstImg = *src2Img;
      noDiff  = false;
    }
    else
    {
      //Just assign green 
      *dstImg =  0xFF00FF00;
    }

    //Increment counters
    src1Img++; 
    src2Img++;
    dstImg++; 
  } 

  //If there was no difference, assign all red
  if(noDiff)
  {
    //Reset the counter and loop for all pixels
    udword * dstImg = (udword *)newImage->getPixels();
    for(uint i=0; i<(uint)newImage->getWidth()*newImage->getHeight();i++)
    {
      //Just assign red
      *dstImg =  0xFF0000FF;
      dstImg++; 
    }
  }
  
  return newImage;
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptFrame::InitMovieFrame()
{
  //If the movie frme has not been created
  if(!currFrameMovie)
  {
    //If init, has already been attempted, prevent further attempts
    if(movieInitAttempt)
    {
      return false;
    }

    //Create the save name
    saveFileNames.movieFileName = GetDataSavingPath() + "FrameMovie.avi";

    //Flag that init has been attempted
    movieInitAttempt = true;
    
    //Calculate the width and height of the movie to save
    uint createWidth =0;
    uint createHeight=0;

    if(colorSaveData[0].savePost || colorSaveData[0].saveDiff)
    {
      createWidth += movieWidth;
    }
    if(depthSaveData.savePost || depthSaveData.saveDiff)
    {
      createWidth += movieWidth;
    }
    if(stencilSaveData.savePost || stencilSaveData.saveDiff)
    {
      createWidth += movieWidth;
    }
    if(colorSaveData[0].savePost || depthSaveData.savePost || stencilSaveData.savePost)
    {
      createHeight += movieHeight;
    }
    if(colorSaveData[0].saveDiff || depthSaveData.saveDiff || stencilSaveData.saveDiff)
    {
      createHeight += movieHeight;
    }
    if(createWidth == 0 || createHeight == 0)
    {
      return false;
    }


    //Create the movie object
    currFrameMovie =  new MovieMaker();
    if(!currFrameMovie->Init(saveFileNames.movieFileName.c_str(), createWidth, createHeight, movieFrameRate, movieCodecs))
    {
      //Delete the current movie and return false
      delete currFrameMovie;
      currFrameMovie = NULL;
      return false;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptFrame::AddMovieFrame()
{
  //If no movie frame, return now
  if(!currFrameMovie)
  { 
    return false;
  }

  //Create the add frame
  corona::Image * newFrame = corona::CreateImage(currFrameMovie->GetWidth(),
                                                 currFrameMovie->GetHeight(),corona::PF_R8G8B8A8);
  udword *frameData    = (udword*)newFrame->getPixels();
  udword *frameDataEnd = frameData + newFrame->getWidth();

  const SaveFrameData *processData[3] = {&colorSaveData[0], &depthSaveData, &stencilSaveData};

  //Loop for all buffers
  // (Post buffers are tiled along the top with diff buffers underneath)
  for(uint i=0; i<3; i++)
  {
    //If saving the buffer data
    if(processData[i]->savePost || processData[i]->saveDiff)
    {
      if(processData[i]->postData)
      {
        //Add the post data to the top of the frame
        if(!CopyMovieFrameImage(frameData + (currFrameMovie->GetHeight() - movieHeight) * currFrameMovie->GetWidth(),
                                currFrameMovie->GetWidth(), 
                                processData[i]->postData, movieWidth, movieHeight))
        {
          LOGERR(("Error sub loading frame image data to the movie format"));
        }
      }
      if(processData[i]->diffData)
      {
        //Add the diff data to the bottom of the frame
        if(!CopyMovieFrameImage(frameData, currFrameMovie->GetWidth(), 
                                processData[i]->diffData, movieWidth, movieHeight))
        {
          LOGERR(("Error sub loading frame image data to the movie format"));
        }
      }

      //Add to the frame data pointer
      frameData += movieWidth;
      
      //Sanity check to ensure no buffer overflows
      if(frameData >= frameDataEnd)
      {
        break;
      }
    }
  }

  //Add the frame data
  bool retValue = currFrameMovie->AddFrame(newFrame->getPixels());
  
  //Delete the created frame
  delete newFrame;

  return retValue;
}





///////////////////////////////////////////////////////////////////////////////
//
bool InterceptFrame::CopyMovieFrameImage(udword *dstBuffer, uint dstStride, corona::Image *srcBuffer, uint newWidth, uint newHeight) const
{
  //Check image
  if(!dstBuffer || !srcBuffer)
  {
    return false;
  }

  uint oldWidth  = srcBuffer->getWidth();
  uint oldHeight = srcBuffer->getHeight();

  //Check the pixel format and width/height
  if(oldWidth == 0 || oldHeight == 0 || newWidth == 0 || newHeight == 0 ||
     dstStride < newWidth ||
     srcBuffer->getFormat() != corona::PF_R8G8B8A8)
  {
    LOGERR(("InterceptFrame::CopyMovieFrameImage - Attempt to copy a bad image"));
    return false;
  }

  //Get the source buffer
  udword * srcImg  = (udword *)srcBuffer->getPixels();
  
  //Calculate the padding at the end of the image
  uint imagePadding = dstStride - newWidth; 

  //Loop for the width/height
  uint sampleY, sampleX;
  for (uint y = 0; y < newHeight; y++)
  {
    sampleY = (oldHeight-1)*y / (newHeight-1);
    
    //Flip the image
    sampleY = (oldHeight-1-sampleY) *oldWidth; 

    for (uint x = 0; x < newWidth; x++)
    {
      sampleX = (oldWidth-1)*x / (newWidth-1);

      //Copy the pixel
      udword copyData = srcImg[sampleY + sampleX];

      //Assign the data (in ABGR format)
      *dstBuffer = (copyData & 0xFF00FF00) |
                   ((copyData & 0x00FF0000) >> 16) |
                   ((copyData & 0x000000FF) << 16);

      //Increment the destination
      dstBuffer++;
    }

    //Add the padding (stride - newWidth) 
    dstBuffer += imagePadding;
  }

  return true;
}


