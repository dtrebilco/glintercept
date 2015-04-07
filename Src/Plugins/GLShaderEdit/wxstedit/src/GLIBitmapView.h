
#ifndef __GLI_BITMAP_VIEW_H_
#define __GLI_BITMAP_VIEW_H_

#include <wx/wx.h>
#include <wx/image.h>
#include "GLIShaderDebug.h"
#include "GLIBitmapNotebook.h"

#define MAX_ZOOM_OUT_VALUE   10
#define MAX_ZOOM_IN_VALUE    20

class GLIBitmapNotebook;

//DT_TODO: Class/ comment checks

//@
//  Summary:
//    This class provides a scrollable view of a bitmap with colour scaling
//    options.
//  
class GLIBitmapView: public wxScrolledWindow
{    
  DECLARE_EVENT_TABLE()

public:

  //@
  //  Summary:
  //    Constructor.
  //  
  GLIBitmapView(GLIBitmapNotebook *parent);

  bool SetZoomScale(uint scaleMultiply, uint scaleDivide);
  inline void GetZoomScale(uint &retScaleMultiply, uint &retScaleDivide) const;

  bool LoadBitmapData(const FrameBufferData & newData);

  void SetDisplayImage(const wxImage &newImage);

  //@
  //  Summary:
  //    Call to update the main display bitmap when the framedata changes or
  //    the display format changes.
  //  
  bool UpdateBitmapData();

  // Center the display on the bitamp/image point
  void CenterDisplay(const wxPoint &bitmapPoint);

  // Get the image size of the current display image
  inline wxSize GetImageSize() const;

  // Get the display string for the passed bitmap point
  wxString GetBitmapValueString(const wxPoint& bitmapPoint) const;

  // Get the current buffer type data
  inline void GetGLBufferType(int &bufferTypeID, int drawBufferID) const;

protected:

  GLIBitmapNotebook *parentControl;               // The controlling parent control

  FrameBufferData frameData;                      // The display frame data

  float framePreNormalize[2];                     // The values used to normalize the pre image
  float framePostNormalize[2];                    // The values used to normalize the post image

  uint scaleMultiply;                             // The multiply part of the scale function
  uint scaleDivide;                               // The divide part of the scale function

  wxImage  displayImage;                          // The current full resolution display image
  wxBitmap cacheBitmap;                           // The cached scaled version of the bitmap image

  wxBitmap hashBitmap;                            // The bitmap displayed in areas where there is no image
  wxBitmap targetBitmap;                          // The bitmap displayed around the target debug pixel

  //@
  //  Summary:
  //    Callback events to handle changes.
  //  
  void OnPaint(wxPaintEvent &event);
  void OnMouseMove(wxMouseEvent &event);
  void OnLeftDown(wxMouseEvent &event);
  void OnRightDown(wxMouseEvent &event);
  void OnLeftDClick(wxMouseEvent &event);
  void OnRightDClick(wxMouseEvent &event);

  void RecalculateDisplay();

  //@
  //  Summary:
  //    Convert a point from window space to bitmap space
  //    clampValue - This value indicates if the returned value is clamped to the bitmap extents 
  wxPoint ConvertWindowToBitmap(const wxPoint &winPoint, bool clampValue = true);

  // Caclculate the values used when normalizing a image
  void CalculateNormalizeValues(const std::vector<float> &calcValues, float resultValues[2]) const;

  // Sets "pixelDiff" to true if the source pixel data is different (and in Diff image mode)
  void SetBitmapColor1f(unsigned char * dstColor, float * srcPreColor, float * srcPostColor,
                        uint imageFlags, GLIBitmapNotebook::ImageState imageState, bool imageNormalized, bool &pixelDiff);

  void SetBitmapColor4f(unsigned char * dstColor, float * srcPreColor, float * srcPostColor,
                        uint imageFlags, GLIBitmapNotebook::ImageState imageState, bool imageNormalized, bool &pixelDiff);

  // Get the tooltip for the specified bitmap value
  wxString GetBitmapToolTip(const wxPoint& bitmapPoint) const;

  // Convert the float into a string (using the current formatting rules)
  wxString FormatFloat(float value) const;
};


///////////////////////////////////////////////////////////////////////////////
//
inline void GLIBitmapView::GetZoomScale(uint &retScaleMultiply, uint &retScaleDivide) const
{
  retScaleMultiply = scaleMultiply;
  retScaleDivide   = scaleDivide;
}


///////////////////////////////////////////////////////////////////////////////
//
inline wxSize GLIBitmapView::GetImageSize() const
{
  return wxSize(displayImage.GetWidth(), displayImage.GetHeight());
}

///////////////////////////////////////////////////////////////////////////////
//
inline void GLIBitmapView::GetGLBufferType(int &bufferTypeID, int drawBufferID) const
{
  bufferTypeID = frameData.bufferType;
  drawBufferID = frameData.drawBuffer;
}


#endif // __GLI_BITMAP_VIEW_H_

