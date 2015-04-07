/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_FRAME_H_
#define __INTERCEPT_FRAME_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"
#include "InterceptData.h"
#include "ConfigData.h"
#include <corona.h>

#include <string>
#include <vector>

#define IF_NUM_STENCIL_COLORS 256

//The maximum number of color draw buffers that can be supported
#define MAX_COLOR_BUFFERS     16

using namespace std;


class GLDriver;
class MovieMaker;

//Enumeration of the types of buffers that can be saved
enum FrameInterceptDataTypes
{
  FIDT_PRE_FRAME  =0,    // Buffer is saved before the frame
  FIDT_POST_FRAME =1,    // Buffer is saved after the frame
  FIDT_DIFF_FRAME =2,    // The diff of the pre/post buffers is saved
  FIDT_MAX        =3     // Max number of types
};


//@
//  Summary:
//    This structure holds the filnames of files saved by InterceptFrame
//  
struct FrameInterceptFileNames
{

  //@
  //  Summary:
  //    Constructor 
  //  
  FrameInterceptFileNames();

  //@
  //  Summary:
  //    Resets all filenames to the empty string
  //  
  void Reset();

  //@
  //  Summary:
  //    Copy operator. (Needed as multi-dimensional arrays are used)
  //    (Works around a GCC 3.4.2 bug - fixed in 3.4.4)
  //   
  FrameInterceptFileNames & operator=(const FrameInterceptFileNames & src);

  uint   numColorBuffers;                   // The number of color buffers in use

  string colorBufNames[MAX_COLOR_BUFFERS][FIDT_MAX]; // The file name of the "pre/post/diff" images from the color buffer
  string depthBufNames[FIDT_MAX];           // The file name of the "pre/post/diff" images from the z buffer
  string stencilBufNames[FIDT_MAX];         // The file name of the "pre/post/diff" images from the stencil buffer

  string colorBufNamesIcon[MAX_COLOR_BUFFERS][FIDT_MAX]; // The file name of the "pre/post/diff" icon images from the color buffer
  string depthBufNamesIcon[FIDT_MAX];       // The file name of the "pre/post/diff" icon images from the z buffer
  string stencilBufNamesIcon[FIDT_MAX];     // The file name of the "pre/post/diff" icon images from the stencil buffer
  
  string movieFileName;                     // The file name of the frame movie

};



//@
//  Summary:
//    This class intercepts OpenGL render calls and saves the frame buffer, 
//    into pre/post/diff images
//  
class InterceptFrame : public InterceptData
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
  //    configData    - The configuration options for frame interception.
  //
	InterceptFrame(GLDriver *driver,FunctionTable * functionTable,const ConfigData &configData);

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
  //    To enable/disable frame saving. Ensure frame path is set before
  //    enabling.
  //  
  //  Parameters:
  //    flag - Flag to enable/disable frame saving.
  //
  virtual void SetDataSaving(bool flag);

  //@
  //  Summary:
  //    To set the path where frame images are saved (including trailing seperator)
  //  
  //  Parameters:
  //    newPath - The new path to save frame images to.
  //
  virtual void SetDataSavingPath(const string &newPath);

  //@
  //  Summary:
  //    To get the last images saved by the previous render call. If a render 
  //    call was just made (and LogFunctionPre has been called), the file 
  //    names returned will be for the images to be saved. If the image name
  //    is empty, no image for that option is being saved.
  //  
  //  Parameters:
  //    fileNames  - The return structure of file name.
  //
  //  Returns:
  //    If frame saving is enabled, the file names and true are returned. 
  //    Else false is returned.
  //
  inline bool GetFrameFileNames(FrameInterceptFileNames &retFileNames);


  //@
  //  Summary:
  //    To save all the currently dirty data to disk.
  //  
  inline virtual void SaveAllDirtyData();

  //@
  //  Summary:
  //    To set all the data that is currently being tracked, as 
  //    dirty. (ie. needs saving)
  //  
  inline virtual void SetAllDataDirty();

protected:

  bool      initialized;                          // Flag to indicate if we have been initialized
  bool      initFailed;                           // Flag to indicate if initialization failed

  bool      extensionDrawBuffers;                 // Flag indicating if draw buffers is supported
  bool      extensionPBO;                         // Flag if the PBO extension is supported 
  bool      extensionFBO;                         // Flag if the FBO extension is supported (ARB or core version)

  GLint     numDrawBuffers;                       // The number of draw buffers in use

  string    imageExtension;                       // The extension of the file format to savine images in (ie jpg/tga/png)  
  bool      inBeginEndSection;                    // Flag to indicate if the current frame is from a begin/end section
  bool      isRenderFunction;                     // Flag set in the pre call to flags to the post call that the function is a render call

  bool      saveIconFiles;                        // If icon versions of the frame buffers are saved 
  uint      iconSize;                             // The size of the icon if saving icons
  string    iconExtension;                        // The extension of the icon files

  FrameInterceptFileNames saveFileNames;          // The structure of files to use when saving

  udword    stencilColorLookup[IF_NUM_STENCIL_COLORS]; // Lookup stencil color values

  void (GLAPIENTRY *iglBindBuffer) (GLenum target, GLuint buffer); //PBO buffer setting entry point
  void (GLAPIENTRY *iglGetFramebufferAttachmentParameteriv) (GLenum target, GLenum attachment, GLenum pname, GLint *params); //OGL3.0/ FBO Entry point
  void (GLAPIENTRY *iglBindFramebuffer) (GLenum target, GLuint framebuffer);


  //Structure to store the results of the frame data
  struct SaveFrameData
  {
    //@
    //  Summary:
    //    Constructor
    //
    SaveFrameData();
    SaveFrameData(GLenum bufType, bool savePre,bool savePost,bool saveDiff);

    //@
    //  Summary:
    //    Destructor, destroys any allocated buffers
    //  
    virtual ~SaveFrameData();

    //@
    //  Summary:
    //    Resets the buffers to the initial state (ie. deletes any allocations)
    //  
    void ResetBuffers();

    bool savePre;                                 // Save pre frame images
    bool savePost;                                // Save post frame images
    bool saveDiff;                                // Save diff frame images

    corona::Image *preData;                       // The "pre" render buffer data
    corona::Image *postData;                      // The "post" render buffer data
    corona::Image *diffData;                      // The diff data

    GLenum bufType;                               // The type of buffer being read
    uint   bufferCounter;                         // The buffer counter (for multiple buffers of the same type)
  };

  SaveFrameData  colorSaveData[MAX_COLOR_BUFFERS];// The pre/post/diff color frame data
  SaveFrameData  depthSaveData;                   // The pre/post/diff depth frame data
  SaveFrameData  stencilSaveData;                 // The pre/post/diff stencil frame data


  bool           movieSaveEnabled;                // If saving movies of the frame buffer images is enabled
  bool           movieInitAttempt;                // If the movie has been attempted to be initialized this frame
  MovieMaker    *currFrameMovie;                  // The current movie frame.
  uint           movieWidth;                      // The width to render each frame buffer in the movie
  uint           movieHeight;                     // The height to render each frame buffer in the movie
  uint           movieFrameRate;                  // The frame rate of the movie
  vector<string> movieCodecs;                     // The codecs for saving the movie

  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~InterceptFrame();

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
  //    To get the "pre" buffer data for the passed buffer type and data.
  //  
  //  Parameters:
  //    bufferData - The structure to hold the results of the buffer retrieval
  //                 and what buffer to retrieve.
  //
  //    saveNames  - The pre/post/diff file names to be assigned when saving data out.
  //
  //    saveNamesIcon - The pre/post/diff icon file names to be assigned when saving data out.
  //
  //    saveCount  - The unique save count value that increments every buffer retrieval is attempted.
  //
  void GetBufferDataPre(SaveFrameData *bufferData, string saveNames[FIDT_MAX], string saveNamesIcon[FIDT_MAX], uint saveCount);

  //@
  //  Summary:
  //    To get the "post" buffer data for the passed buffer type and data.
  //  
  //  Parameters:
  //    bufferData - The structure to hold the results of the buffer retrieval
  //                 and what buffer to retrieve.
  //
  //    saveNames  - The pre/post/diff file names to be assigned when saving data out.
  //
  //    saveNamesIcon - The pre/post/diff icon file names to be assigned when saving data out.
  //
  void GetBufferDataPost(SaveFrameData *bufferData, string saveNames[FIDT_MAX], string saveNamesIcon[FIDT_MAX]) const;

  //@
  //  Summary:
  //    To get the OpenGL frame buffer for the specified option.
  //    If retrieval was successful, a image the size of the current
  //    viewport is returned. (Ensure the buffer type is readable before 
  //    calling with IsBufferReadable)
  //  
  //  Parameters:
  //    bufType  - The buffer type to get. (ie GL_RGBA, GL_DEPTH_COMPONENT)
  //
  //    bufferCount - The index of the buffer type (ie there can be multiple
  //                  color buffers)
  //
  //  Returns:
  //    If the frame buffer for the specific type could be retrieved,
  //    it is returned. Else NULL is returned.
  //
  corona::Image *GetBuffer(GLenum bufType, uint bufferCount) const;

  //@
  //  Summary:
  //    To get if the OpenGL frame buffer of the passed type is readable by
  //    GetBuffer.
  //  
  //  Parameters:
  //    bufType  - The buffer type to get. (ie GL_RGBA, GL_DEPTH_COMPONENT)
  //
  //    bufferCount - The index of the buffer type (ie there can be multiple
  //                  color buffers)
  //
  //  Returns:
  //    If the frame buffer type is readable, true is returned. Else false 
  //    is returned.
  //
  bool IsBufferReadable(GLenum bufType, uint bufferCount) const;

  //@
  //  Summary:
  //    To get save the passed image buffer to the specified filename.
  //  
  //  Parameters:
  //    fileName  - The file name (and extension) to save the buffer to.
  //
  //    fileNameIcon  -The file name (and extension) to save a icon version
  //                   of the buffer to. (if set)
  //
  //    saveBuffer  - The image buffer to write to the file name.
  //
  //  Returns:
  //    If saving was successful, true is returned. Else false is returned. 
  //
  bool SaveBuffer(const string &fileName, const string &fileNameIcon, corona::Image * saveBuffer) const; 

  //@
  //  Summary:
  //    To get the difference between the passed image buffers.
  //  
  //  Parameters:
  //    src1  - The first compare buffer.
  //
  //    src2  - The second compare buffer.
  //
  //  Returns:
  //    If a difference image can be calculated, a new image containing 
  //    the differences is returned. Else NULL is returned.
  //
  corona::Image *CalculateImageDiff(corona::Image * src1,corona::Image * src2) const;

  //@
  //  Summary:
  //    To initialize the movie frame.
  //  
  //  Returns:
  //    True is returned on success, false on failure.
  //
  bool InitMovieFrame();

  //@
  //  Summary:
  //    To add the current frame buffer data to the current frame movie.(if any)
  //  
  bool AddMovieFrame();

  //@
  //  Summary:
  //    To delete and reset all current frame image buffers.
  //  
  void ResetBuffers();

  //@
  //  Summary:
  //    To copy a image move frame data into a destination buffer. 
  //    This also performs the bit shift into the ABGR format and flips the
  //    image. (required by the movie maker).
  //  
  //  Parameters:
  //    dstBuffer  - The destination buffer.
  //
  //    dstStride  - The stride of the destination buffer.
  //
  //    srcBuffer  - The source buffer image.
  //
  //    newWidth   - The new width of the source buffer image.
  //
  //    newHeight  - The new height of the source buffer image.
  //
  //  Returns:
  //    If the image was copied, true is returned. Else false is returned.
  //
  bool CopyMovieFrameImage(udword *dstBuffer, uint dstStride, corona::Image *srcBuffer, uint newWidth, uint newHeight) const;

};


///////////////////////////////////////////////////////////////////////////////
//
inline bool InterceptFrame::GetFrameFileNames(FrameInterceptFileNames &fileNames)
{
  //Only return true if we are frame saving
  if(GetDataSaving())
  {
    fileNames = saveFileNames;
    return true;
  }
  
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptFrame::SaveAllDirtyData()
{
}


///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptFrame::SetAllDataDirty()
{
}

#endif // __INTERCEPT_FRAME_H_
