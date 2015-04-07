/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "GLFrameBufferCache.h"
#include <CommonErrorLog.h>

USING_ERRORLOG

///////////////////////////////////////////////////////////////////////////////
//
FrameBufferData::FrameBufferData():
bufferType(0),
drawBuffer(GL_NONE),
bufferWidth(0),
bufferHeight(0),
numPixelValues(0)
{
}

///////////////////////////////////////////////////////////////////////////////
//
bool FrameBufferData::Init(GLint newBufferType, GLint newDrawBuffer, GLuint newNumPixelValues, GLuint newBufferWidth, GLuint newBufferHeight)
{
  //Don't accept invalid values
  if(newNumPixelValues <=0 || newBufferWidth == 0 || newBufferHeight == 0)
  {
    return false;
  }
  
  //Assign the type data
  bufferType     = newBufferType;
  drawBuffer     = newDrawBuffer;

  bufferWidth    = newBufferWidth;
  bufferHeight   = newBufferHeight;
  numPixelValues = newNumPixelValues;

  //Allocate the storage vectors
  preBuffer.resize(numPixelValues * bufferWidth * bufferHeight);
  postBuffer.resize(numPixelValues * bufferWidth * bufferHeight);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
GLFrameBufferCache::GLFrameBufferCache(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),
GLV(callBacks->GetCoreGLFunctions()),
numDrawBuffers(0),
extensionPBO(false),
iglBindBuffer(NULL)
{
  //Init the viewport size array
  viewSize[0] = 0;
  viewSize[1] = 0;
  viewSize[2] = 0;
  viewSize[3] = 0;

  //Reserve some frame buffer data space to avoid expensive resizes
  frameData.reserve(20);
}

///////////////////////////////////////////////////////////////////////////////
//
GLFrameBufferCache::~GLFrameBufferCache()
{
  //Clear any existing buffers
  frameData.clear();
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLFrameBufferCache::Init()
{
  //Return if we cannot make OpenGL calls
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return false;
  }

  //DT_TODO: May have to worry about multi-sample FBO and read/write FBO setting

  // DT_TODO: Update this with FBO? - This is a bit extreme for saving the frame buffer
  // but it is only intened to be used with the shader debugger

  //Test extension support
  float glVersion = gliCallBacks->GetGLVersion();
  if(glVersion < 2.0f)
  {
    LOGERR(("GLFrameBufferCache::Init - Need OpenGL 2.0 implementation to use the frame buffer cache"));
    return false;
  }
 

  //Get the maximum number of draw buffers
  GLV->glGetIntegerv(GL_MAX_DRAW_BUFFERS, &numDrawBuffers);
  if(numDrawBuffers < 1)
  { 
    LOGERR(("GLFrameBufferCache::Init - Invalid draw buffer count"));
    return false;
  }

  //Get if PBO's are supported
  if(glVersion >= 2.1f ||
     gliCallBacks->IsGLExtensionSupported("GL_ARB_pixel_buffer_object") ||
     gliCallBacks->IsGLExtensionSupported("GL_EXT_pixel_buffer_object"))
  {
    extensionPBO = true;
  }

  void **loadFunc;

  //Helper define for function lookups
#define GL_FUNC_LOOKUP(string)                           \
  loadFunc  = (void**)(&i##string);                      \
  *loadFunc = gliCallBacks->GetGLFunction(#string);      \
  if(*loadFunc == NULL)                                  \
  {                                                      \
    LOGERR(("Function %s unable to be mapped",#string)); \
    return false;                                        \
  }                                                      

  //Init function pointers
  GL_FUNC_LOOKUP(glBindBuffer);

#undef GL_FUNC_LOOKUP

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
GLFrameBufferCache *GLFrameBufferCache::CreateInstance(InterceptPluginCallbacks *callBacks)
{
  //Create the new render cache
  GLFrameBufferCache * newCache = new GLFrameBufferCache(callBacks);

  //Attempt to init
  if(!newCache->Init())
  {
    LOGERR(("GLFrameBufferCache::CreateInstance - Unable to init new frame buffer cache"));
    newCache->ReleaseReference();
    return NULL;
  }

  return newCache;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLFrameBufferCache::CacheBuffersPre()
{
/* TODO: update this for OpenGL 3.0 core context - no GL_DEPTH_BITS etc
  //Clear any existing buffers
  frameData.clear();
   
  //Get the size of the current viewport
  GLV->glGetIntegerv(GL_VIEWPORT, &viewSize[0]);

  //If the viewport does not have a size, return now
  if(viewSize[2] <= 0 || viewSize[3] <= 0)
  {
    return;
  }

  //Get how may draw buffers are in use (depth+color) and the format of each buffer
  for(GLint i=0; i<numDrawBuffers; i++)
  {
    //Get if there is a draw buffer active at the specific slot
    GLint currDrawBuffer = GL_NONE;
    GLV->glGetIntegerv(GL_DRAW_BUFFER0 + i, &currDrawBuffer);
    if(currDrawBuffer != GL_NONE)
    {
      //DT_TODO: Catch buffer values like GL_FRONT_AND_BACK?  

      //In future, it may be faster to get 8-bit buffers as 8-bits.
      //  However, for simplicity all buffers are retrieved in float format.

      //Add a new frame buffer item
      frameData.push_back(FrameBufferData());

      //Initialize it 
      if(!frameData.back().Init(GL_RGBA, currDrawBuffer, 4, viewSize[2], viewSize[3]))
      {
        //Remove it from the array
        frameData.pop_back();
        LOGERR(("GLFrameBufferCache::CacheBuffersPre - Unable to cache color buffer"));
      }
    }
  }

  //Get if there is a depth buffer
  GLint numDepthBits=0;
  GLV->glGetIntegerv(GL_DEPTH_BITS, &numDepthBits);
  if(numDepthBits > 0)
  {
    //Add a new frame buffer item
    frameData.push_back(FrameBufferData());

    //Initialize it 
    if(!frameData.back().Init(GL_DEPTH_COMPONENT, GL_NONE, 1, viewSize[2], viewSize[3]))
    {
      //Remove it from the array
      frameData.pop_back();

      LOGERR(("GLFrameBufferCache::CacheBuffersPre - Unable to cache depth buffer"));
    }
  }

  //Get the pre buffer data
  GetCurrentBufferData(false);
*/
}

///////////////////////////////////////////////////////////////////////////////
//
void GLFrameBufferCache::CacheBuffersPost()
{
  //Get the post buffer data
  GetCurrentBufferData(true);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLFrameBufferCache::GetCurrentBufferData(bool isPostData)
{
  //If there are no buffers to get, return now
  if(frameData.size() == 0)
  {
    return;
  }

  // Update this to not use glPushClientAttrib etc (use only core OpenGL3.0)
/*

  //Set pixel transfer modes
  GLV->glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

  //Set the required pack operations
  GLV->glPixelStorei(GL_PACK_ALIGNMENT,  1); 
  GLV->glPixelStorei(GL_PACK_LSB_FIRST,  0); 

  GLV->glPixelStorei(GL_PACK_ROW_LENGTH, 0); 
  GLV->glPixelStorei(GL_PACK_SKIP_PIXELS,0); 
  
  GLV->glPixelStorei(GL_PACK_SKIP_ROWS,  0); 
  GLV->glPixelStorei(GL_PACK_SWAP_BYTES, 0); 

  //If PBO support, check if a GL_PIXEL_PACK_BUFFER is bound
  GLint oldPBO = 0;
  if(extensionPBO)
  {
    //Get the existing PBO
    GLV->glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING_ARB, &oldPBO);
    if(oldPBO != 0)
    {
      //If there is a pack PBO bound, disable it while reading pixels
      iglBindBuffer(GL_PIXEL_PACK_BUFFER_ARB,0); 
    }
  }

  //Save the old read buffer state
  GLint oldReadBuffer = GL_NONE;
  GLV->glGetIntegerv(GL_READ_BUFFER, &oldReadBuffer);  

  //DT_TODO: What about depth_bias and depth_scale?
  //         What about ARB imaging modifying the return values? (see glReadPixels,glPixelTransfer)

  //Loop for all the buffers 
  for(uint i=0; i<frameData.size(); i++)
  {
    //Assign the buffer store pointer
    float * storeBuffer = &frameData[i].preBuffer[0];
    if(isPostData)
    {
      storeBuffer = &frameData[i].postBuffer[0];
    }

    //Assign a read buffer if a different draw buffer is used
    if(frameData[i].drawBuffer != GL_NONE)
    {
      GLV->glReadBuffer(frameData[i].drawBuffer);      
    }
    
    //Read the pixels 
    GLV->glReadPixels(viewSize[0],viewSize[1],viewSize[2],viewSize[3], frameData[i].bufferType, GL_FLOAT, storeBuffer);
          
    //Reset the read buffer
    if(frameData[i].drawBuffer != GL_NONE)
    {
      GLV->glReadBuffer(oldReadBuffer);      
    }
  }

  //Restore the old read buffer
  GLV->glReadBuffer(oldReadBuffer);

  //If PBO support, check if a GL_PIXEL_PACK_BUFFER was bound and restore it
  if(extensionPBO && oldPBO != 0)
  {
    iglBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, oldPBO); 
  }

  //Restore transfer mode
  GLV->glPopClientAttrib();
*/
}

