/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __GL_FRAME_BUFFER_CACHE_H_
#define __GL_FRAME_BUFFER_CACHE_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include <ReferenceCount.h>

class GLDriver;


//@
//  Summary:
//    This structure contains the data saved from the frame buffer.
//  
struct FrameBufferData
{
  //@
  //  Summary:
  //    The default constructor.
  //  
  FrameBufferData();

  //@
  //  Summary:
  //    To initialize the frame buffer data.
  //  
  //  Parameters:
  //    bufferType     - The type of buffer (eg. color buffer binding, depth buffer)
  //
  //    drawBuffer     - The draw buffer that the data comes from (if any)
  //
  //    numPixelValues - The number of float values per pixel.
  //
  //    bufferWidth   - The buffer width.
  //
  //    bufferHeight  - The buffer height.
  //
  //  Returns:
  //    If a interface could be created and init, it is returned. 
  //    Else NULL is returned.
  //
  bool Init(GLint bufferType, GLint drawBuffer, GLuint numPixelValues, GLuint bufferWidth, GLuint bufferHeight);

  GLint  bufferType;         // The type of buffer the data comes from (eg. the RGBA or depth component)
  GLint  drawBuffer;         // If a specific draw buffer is used (eg. multiple color attachments)

  GLuint bufferWidth;        // The width of the buffer
  GLuint bufferHeight;       // The height of the buffer
  GLuint numPixelValues;     // The number of float values in a single pixel 

  vector<float> preBuffer;   // The pre-render buffer
  vector<float> postBuffer;  // The post-render buffer
};



//@
//  Summary:
//    This class saves the current render buffer pre and post render call.
//    The current render buffer includes all color buffers and depth buffers.
//  
class GLFrameBufferCache : public ReferenceCount
{
public:

  //@
  //  Summary:
  //    To create and initialize a frame buffer cache.
  //  
  //  Parameters:
  //    callBacks  - The callbacks to GLIntercept.
  //
  //  Returns:
  //    If a interface could be created and init, it is returned. 
  //    Else NULL is returned.
  //
  static GLFrameBufferCache * CreateInstance(InterceptPluginCallbacks *callBacks);

  //@
  //  Summary:
  //    To cache the frame buffer data before a render call.
  //  
  void CacheBuffersPre();

  //@
  //  Summary:
  //    To cache the frame buffer data after a render call.
  //  
  void CacheBuffersPost();

  //@
  //  Summary:
  //    To get the frame buffer data.
  //  
  //  Returns:
  //    a reference to the fraem buffer data is returned.
  //
  inline const vector<FrameBufferData> & GetFrameBufferData() const;

protected:


  InterceptPluginCallbacks *gliCallBacks;  // The callback interface into GLIntercept
  const GLCoreDriver       *GLV;           // The core OpenGL driver
  GLint numDrawBuffers;                    // The number of color buffers that can be supported at once

  GLint viewSize[4];                       // The view dimensions of the frame buffers 
  vector<FrameBufferData> frameData;       // The array of saved frame buffer data

  //@
  //  Summary:
  //    Constructor
  //
  //  Parameters:
  //    gliCallBacks  - The callbacks to GLIntercept.
  //
  GLFrameBufferCache(InterceptPluginCallbacks *callBacks);

  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~GLFrameBufferCache();

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
  //    To get the current frame buffer data to fill out the "frameData" 
  //    buffer data.
  //
  //  Parameters:
  //    isPostData  - If this call is filling out post-render data.
  //
  void GetCurrentBufferData(bool isPostData);

private:

  bool extensionPBO;                              // If the PBO extension is supported

  //Extension entry points
  void (GLAPIENTRY *iglBindBuffer) (GLenum target, GLuint buffer); //PBO buffer setting entry point

};

///////////////////////////////////////////////////////////////////////////////
//
inline const vector<FrameBufferData> & GLFrameBufferCache::GetFrameBufferData() const
{
  return frameData;
}

#endif // __GL_FRAME_BUFFER_CACHE_H_
