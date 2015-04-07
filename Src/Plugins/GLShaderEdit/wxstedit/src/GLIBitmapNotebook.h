
#ifndef __GLI_BITMAP_NOTEBOOK_H_
#define __GLI_BITMAP_NOTEBOOK_H_

#include <wx/wx.h>
#include <wx/notebook.h>

#include "GLIShaderDebug.h"

class GLIBitmapView;

//DT_TODO: Class/ comment checks

//@
//  Summary:
//    This class provides a notebook of bitmap images.
//  
class GLIBitmapNotebook: public wxNotebook
{    
  DECLARE_EVENT_TABLE()

public:
  
  enum ToolState
  {
    TS_Select,  // Select a debug pixel tool
    TS_Zoom     // Zoom image tool
  };

  enum ImageFlags
  {
    IF_RED   = (1 << 0), // Display red channel
    IF_GREEN = (1 << 1), // Display green channel
    IF_BLUE  = (1 << 2), // Display blue channel
    IF_ALPHA = (1 << 3), // Display alpha channel

    IF_RGB = (IF_RED | IF_GREEN | IF_BLUE) // Combined red/green/blue 
  };

  enum ImageState
  {
    IS_PreImage,  // Display pre render image
    IS_PostImage, // Display post render image
    IS_DiffImage, // Display diff image
  };

  //@
  //  Summary:
  //    Constructor.
  //  
  GLIBitmapNotebook(GLIShaderDebug *parentControl,
                    wxWindow* parent, wxWindowID id, 
                    const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                    long style = 0, const wxString& name = wxNotebookNameStr);

  void AddBitmapPage(const FrameBufferData & newData);

  void ResetAllPages();

  // Get the zoom scale for displaying bitmaps
  inline void GetZoomScale(uint &retScaleMultiply, uint &retScaleDivide) const;
  
  // Get the current displayed image size
  wxSize GetImageSize() const;

  void SetToolState(ToolState newState);
  inline ToolState GetToolState();

  void SetImageDisplay(uint flags, ImageState newState, bool newNormalize);
  inline uint GetImageFlags() const;
  inline ImageState GetImageState() const;
  inline bool GetImageNormalized() const;

  void OnBitmapLeftClick(const wxPoint &bitmapPos);
  void OnBitmapRightClick(const wxPoint &bitmapPos);

  // Get the current bitmap selection coordinates. Negative if invalid
  inline wxPoint GetBitmapSelectPoint() const;

  // To set the new bitmap selection point
  void SetSelectedPosition(const wxPoint &newPos);

  // Get the current bitmap point data in a display string
  wxString GetBitmapValueString() const;

  // Get the display string for the currently selected point
  inline const wxString & GetSelectPointString() const;

  // Set if displying ints or floats
  inline void SetValueTextFormat(bool displayInts);

  // Get if displying ints or floats
  inline bool GetValueTextFormat() const;

protected:

  GLIShaderDebug             *parentControl;      // The controlling parent

  std::vector<GLIBitmapView*> bitmapViews;        // The array of bitmap views of the render buffers (contained within bitmapNotebook);

  int preferedBufferTypeID;                       // The prefered selection buffer ID
  int preferedDrawBufferID;                       // The prefered draw buffer ID

  uint viewScaleMultiply;                         // The multiply part of the bitmap scale function
  uint viewScaleDivide;                           // The divide part of the bitmap scale function

  wxPoint  selectPos;                             // The bitmap selection coordinates

  ToolState  toolState;                           // The current tool

  uint       imageFlags;                          // The render flags for the image display
  ImageState imageState;                          // The state of the current image to render
  bool       imageNormalized;                     // The flag indicating if the image display should be "normalized". 

  bool       valueTextFormatInts;                 // If displaying values in strings as ints or floats

  //@
  //  Summary:
  //    Callback events to handle changes.
  //  
  void OnPageChange(wxNotebookEvent &event);

  //@
  //  Summary:
  //    To calculate a display name for the passed buffer
  //  
  //  Parameters:
  //    bufferData  - The buffer to calculate a display name for.
  //
  //  Returns:
  //    A display name is returned.
  //
  wxString GetBufferDisplayName(const FrameBufferData &bufferData) const;

  //@
  //  Summary:
  //    To update the cursor display on the passed window to mat the current
  //    tool state.
  //  
  //  Parameters:
  //    updateWindow  - The window to set the cursor on.
  //
  void UpdateCursorDisplay(wxWindow *updateWindow) const;

  //@
  //  Summary:
  //    To zoom the contained bitmaps.
  //  
  //  Parameters:
  //    zoomIn - If zooming in or out.
  //
  //    bitmapPos - The bitmap point to focus the zoom on.
  //
  void ZoomBitmap(bool zoomIn, const wxPoint &bitmapPos);

};

///////////////////////////////////////////////////////////////////////////////
//
inline GLIBitmapNotebook::ToolState GLIBitmapNotebook::GetToolState()
{
  return toolState;
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint GLIBitmapNotebook::GetImageFlags() const
{
  return imageFlags;
}

///////////////////////////////////////////////////////////////////////////////
//
inline GLIBitmapNotebook::ImageState GLIBitmapNotebook::GetImageState() const
{
  return imageState;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool GLIBitmapNotebook::GetImageNormalized() const
{
  return imageNormalized;
}

///////////////////////////////////////////////////////////////////////////////
//
inline wxPoint GLIBitmapNotebook::GetBitmapSelectPoint() const
{
  return selectPos;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void GLIBitmapNotebook::GetZoomScale(uint &retScaleMultiply, uint &retScaleDivide) const
{
  retScaleMultiply = viewScaleMultiply;
  retScaleDivide   = viewScaleDivide;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::SetValueTextFormat(bool displayInts)
{
  // Check if the state changed
  if(valueTextFormatInts != displayInts)
  { 
    valueTextFormatInts = displayInts;

    // Update the selected color to the new display format
    parentControl->UpdateSelectedColor();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIBitmapNotebook::GetValueTextFormat() const
{
  return valueTextFormatInts;
}


#endif // __GLI_BITMAP_NOTEBOOK_H_

