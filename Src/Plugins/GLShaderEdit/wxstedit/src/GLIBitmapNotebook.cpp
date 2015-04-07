
#include <wx/wx.h>
#include "GLIBitmapNotebook.h"
#include "GLIBitmapView.h"

#include "..\\..\\..\\..\\MainLib\\GLDefines.h"

///////////////////////////////////////////////////////////////////////////////
//
BEGIN_EVENT_TABLE(GLIBitmapNotebook, wxNotebook)

  EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, GLIBitmapNotebook::OnPageChange) 

END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
//
GLIBitmapNotebook::GLIBitmapNotebook(GLIShaderDebug *newParentControl, 
                                     wxWindow* parent, wxWindowID id, 
                                     const wxPoint& pos, const wxSize& size,
                                     long style, const wxString& name):
wxNotebook(parent, id, pos, size, style, name),
parentControl(newParentControl),
preferedBufferTypeID(0),
preferedDrawBufferID(0),
viewScaleMultiply(1),
viewScaleDivide(1),
selectPos(-1, -1),

toolState(TS_Select),
imageFlags(0),
imageState(IS_PostImage),
imageNormalized(false),
valueTextFormatInts(false)
{

  /* // Debug
  for(uint i=0; i<3; i++)
  {
    GLIBitmapView * bitmapView = new GLIBitmapView(this);
    bitmapView->SetDisplayImage(wxImage("C:\\Projects\\GLIntercept\\Research\\test.bmp"));
    AddPage(bitmapView, "test", true); 
    bitmapViews.push_back(bitmapView);
  }*/
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::AddBitmapPage(const FrameBufferData & newData)
{
  // Create the bitmap views
  GLIBitmapView * bitmapView = new GLIBitmapView(this);

  // Create the name for the bitmap
  wxString displayName = GetBufferDisplayName(newData);

  // Load the frame buffer data into the display
  if(!bitmapView->LoadBitmapData(newData))
  {
    displayName = displayName + wxT("-Error");
  }

  // Add the bitmap views to the notebook
  if(bitmapViews.size() == 0)
  {
    AddPage(bitmapView, displayName, true); 

    // If the preferred buffer type is not set, set it now
    if(preferedBufferTypeID == 0 &&
       preferedDrawBufferID == 0)
    {
      bitmapView->GetGLBufferType(preferedBufferTypeID, preferedDrawBufferID);         
    }

    // Check if the buffer data is valid size
    viewScaleMultiply = 1;
    viewScaleDivide   = 1;
    if(newData.bufferWidth > 0 && newData.bufferHeight > 0)
    {
      // Calculate the zoom factor
      wxSize scrollSize = bitmapView->GetSize();

      // Get the scale size in both directions
      float scaleX = (float)scrollSize.x / (float)newData.bufferWidth;
      float scaleY = (float)scrollSize.y / (float)newData.bufferHeight;

      // Get the minimum scale
      float minScale = scaleY;
      if(scaleX < scaleY)
      {
        minScale = scaleX;
      }

      // Calculate the scale ratio
      if(minScale > 1.0f)
      {
        viewScaleMultiply = (uint)minScale;
        viewScaleDivide   = 1;

        if(viewScaleMultiply < 1)
        {
          viewScaleMultiply  = 1;
        }
        if(viewScaleMultiply > MAX_ZOOM_IN_VALUE)
        {
          viewScaleMultiply = MAX_ZOOM_IN_VALUE;
        }
      }
      else if(minScale > 0.0f)
      {
        viewScaleMultiply = 1;
        viewScaleDivide   = (uint)ceil(1.0f/minScale);

        if(viewScaleDivide < 1)
        {
          viewScaleDivide  = 1;
        }
        if(viewScaleDivide > MAX_ZOOM_OUT_VALUE)
        {
          viewScaleDivide = MAX_ZOOM_OUT_VALUE;
        }
      }
    }

    // Set the scale values in the bitmap
    if(!bitmapView->SetZoomScale(viewScaleMultiply, viewScaleDivide))
    {
      // If the scale could not be set, just use a 1:1 scale
      viewScaleMultiply = 1;
      viewScaleDivide   = 1;
      bitmapView->SetZoomScale(viewScaleMultiply, viewScaleDivide);
    }
  }
  else
  {
    // Add the new page
    AddPage(bitmapView, displayName, false); 

    // Set the zoom scale
    bitmapView->SetZoomScale(viewScaleMultiply, viewScaleDivide);
  }

  // Set the cursor state
  UpdateCursorDisplay(bitmapView);

  // Add the view to the internal tracking array
  bitmapViews.push_back(bitmapView);

  // If the buffer added is the preferred buffer
  // (This is so the same buffer is kept displayed between multiple render calls)
  if(newData.bufferType == preferedBufferTypeID && 
     newData.drawBuffer == preferedDrawBufferID)
  {
    SetSelection(bitmapViews.size() - 1);
  }

  // Update the window title for new zoom factor and image
  parentControl->UpdateWindowTitle();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::ResetAllPages()
{
  // Reset the scale
  viewScaleMultiply = 1;
  viewScaleDivide   = 1;

  // Delete all attached pages
  DeleteAllPages();

  // Clear the stored array
  bitmapViews.clear();

  // Reset the selection point
  SetSelectedPosition(wxPoint(-1, -1));

  // Update the window title for new zoom factor
  parentControl->UpdateWindowTitle();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::OnPageChange(wxNotebookEvent &event)
{
  // Ensure our window ID and a valid page was selected previously
  if(event.GetId() == GetId())
  {
    int newPage = event.GetSelection();
    int oldPage = event.GetOldSelection();

    // Get if the selected pages are valid
    if(newPage >= 0 && newPage < bitmapViews.size() &&
       oldPage >= 0 && oldPage < bitmapViews.size())
    {
      int scrollStartX = 0;
      int scrollStartY = 0;
      int oldScrollPPUX = 1;
      int oldScrollPPUY = 1;
      int newScrollPPUX = 1;
      int newScrollPPUY = 1;

      // Get the old scroll position
      bitmapViews[oldPage]->GetViewStart(&scrollStartX, &scrollStartY);
      bitmapViews[oldPage]->GetScrollPixelsPerUnit(&oldScrollPPUX, &oldScrollPPUY);

      // Get the new scroll pixels per unit (probably the same as the old one)
      bitmapViews[newPage]->GetScrollPixelsPerUnit(&newScrollPPUX, &newScrollPPUY);

      // Ensure a valid setting of the scroll area
      if(newScrollPPUX != 0 && newScrollPPUY != 0)
      {
        // Set the new scroll position
        bitmapViews[newPage]->Scroll(scrollStartX*oldScrollPPUX/newScrollPPUX,
                                     scrollStartY*oldScrollPPUY/newScrollPPUY);
      }
      else
      {
        bitmapViews[newPage]->Scroll(0, 0);
      }

      // Get the current GL buffer type displayed
      bitmapViews[newPage]->GetGLBufferType(preferedBufferTypeID, preferedDrawBufferID);

      // Update the window title the new image
      parentControl->UpdateWindowTitle();
    }
  }

  // Allow others to process
  event.Skip();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::OnBitmapLeftClick(const wxPoint &bitmapPos)
{
  // Handle based on the current tool
  switch(toolState)
  {
    // Set the new selected position
    case(TS_Select):
      parentControl->SetDebugPixel(bitmapPos);
      break;

    case(TS_Zoom):
      ZoomBitmap(true, bitmapPos);
      break;
  };
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::OnBitmapRightClick(const wxPoint &bitmapPos)
{
  // Handle based on the current tool
  switch(toolState)
  {
    case(TS_Zoom):
      ZoomBitmap(false, bitmapPos);
      break;
  };
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::ZoomBitmap(bool zoomIn, const wxPoint &bitmapPos)
{
  // Get the current notebook selection
  int currSelection = GetSelection();
  if(currSelection < 0 || currSelection >= bitmapViews.size())
  {
    return;
  }

  // Update the view scale
  if(zoomIn)
  {
    if(viewScaleDivide > 1)
    {
      viewScaleDivide--;
    }
    else if(viewScaleMultiply < MAX_ZOOM_IN_VALUE)
    {
      viewScaleMultiply++;
    }
  }
  else
  {
    // Update the view scale
    if(viewScaleMultiply > 1)
    {
      viewScaleMultiply--;
    }
    else if(viewScaleDivide < MAX_ZOOM_OUT_VALUE)
    {
      viewScaleDivide++;
    }
  }

  // Set the zoom scale on all the windows
  for(uint i=0; i<bitmapViews.size(); i++)
  {
    bitmapViews[i]->SetZoomScale(viewScaleMultiply, viewScaleDivide);
  }

  // Center the current screen on the zoom point
  bitmapViews[currSelection]->CenterDisplay(bitmapPos);

  // Update the window title for new zoom factor
  parentControl->UpdateWindowTitle();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::SetToolState(ToolState newState)
{
  if(toolState != newState)
  {
    // Assign the new state
    toolState = newState;

    // Update all the child view cursors
    for(uint i=0; i<bitmapViews.size(); i++)
    {
      UpdateCursorDisplay(bitmapViews[i]);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::SetImageDisplay(uint flags, ImageState newState, bool newNormalize)
{
  // If any if the image display has changed
  if(imageFlags != flags ||
     imageState != newState ||
     imageNormalized != newNormalize)
  {
    imageFlags = flags;
    imageState = newState;
    imageNormalized = newNormalize;

    // Update all the image display
    for(uint i=0; i<bitmapViews.size(); i++)
    {
      bitmapViews[i]->UpdateBitmapData();
    }

    // Update the display of the selected bitmap 
    // (pre/post bitmap display may have changed)
    parentControl->UpdateSelectedColor();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
wxSize GLIBitmapNotebook::GetImageSize() const
{
  // If there is a current valid image
  int currSelection = GetSelection();
  if(currSelection >= 0 && currSelection < bitmapViews.size())
  {
    return bitmapViews[currSelection]->GetImageSize();
  }

  return wxSize(0,0); 
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::UpdateCursorDisplay(wxWindow *updateWindow) const
{
  if(!updateWindow)
  {
    return;
  }

  // Assign the cursor based on the tool state
  switch(toolState)
  {
    case(TS_Select):
      updateWindow->SetCursor(wxCursor(wxCURSOR_CROSS));
      break;
    case(TS_Zoom):
      updateWindow->SetCursor(wxCursor(wxCURSOR_MAGNIFIER));
      break;
  };

}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapNotebook::SetSelectedPosition(const wxPoint &newPos)
{
  // If the new position is the same, abort now
  if(newPos == selectPos)
  {
    return;
  }

  // Empty the selected position
  selectPos = wxPoint(-1, -1);

  // Get the current bitmap display
  int currSelection = GetSelection();
  if(currSelection >= 0 && currSelection < bitmapViews.size())
  {
    // If the bitmap position is valid
    wxSize imageSize = bitmapViews[currSelection]->GetImageSize();
    if(newPos.x >= 0 && newPos.x < imageSize.GetWidth() &&
       newPos.y >= 0 && newPos.y < imageSize.GetHeight())
    {
      // Assign the position
      selectPos = newPos;
    }

    // Refresh the active window
    bitmapViews[currSelection]->Refresh();
  }

  // Update the display
  parentControl->UpdateSelectedColor();
}

///////////////////////////////////////////////////////////////////////////////
//
wxString GLIBitmapNotebook::GetBitmapValueString() const
{
  wxString retString;

  // Loop and get the data for all bitmaps at that point
  for(uint i=0; i<bitmapViews.size(); i++)
  {
    // Get the bitmap data for the page
    if(i < GetPageCount())
    {
      retString += GetPageText(i) + wxT(" = ") + bitmapViews[i]->GetBitmapValueString(selectPos);
    }

    // Add a newline
    if(i != bitmapViews.size()-1)
    {
      retString += wxT("\n");
    }
  }
 
  return retString;
}


///////////////////////////////////////////////////////////////////////////////
//
wxString GLIBitmapNotebook::GetBufferDisplayName(const FrameBufferData &bufferData) const
{
  wxString retString(wxT("Unknown"));
  
  // Determine the type
  if(bufferData.bufferType == GL_RGBA)
  {
    retString = wxT("Color Buffer - ");
    
    wxString drawBufferStr;

    // Attempt to find the color buffer type
    switch(bufferData.drawBuffer)
    {
      case(GL_FRONT):
        drawBufferStr = wxT("Front");
        break;
      case(GL_BACK):
        drawBufferStr = wxT("Back");
        break;
      case(GL_LEFT):
        drawBufferStr = wxT("Left");
        break;
      case(GL_RIGHT):
        drawBufferStr = wxT("Right");
        break;
      case(GL_FRONT_AND_BACK):
        drawBufferStr = wxT("Front and Back");
        break;

      case(GL_FRONT_LEFT):
        drawBufferStr = wxT("Front Left");
        break;
      case(GL_FRONT_RIGHT):
        drawBufferStr = wxT("Front Right");
        break;
      case(GL_BACK_LEFT):
        drawBufferStr = wxT("Back Left");
        break;
      case(GL_BACK_RIGHT):
        drawBufferStr = wxT("Back Right");
        break;

      case(GL_AUX0):
      case(GL_AUX1):
      case(GL_AUX2):
      case(GL_AUX3):
        drawBufferStr.Printf(wxT("AUX %u"), bufferData.drawBuffer - GL_AUX0);
        break;

      case(GL_COLOR_ATTACHMENT0_EXT): 
      case(GL_COLOR_ATTACHMENT1_EXT): 
      case(GL_COLOR_ATTACHMENT2_EXT): 
      case(GL_COLOR_ATTACHMENT3_EXT): 
      case(GL_COLOR_ATTACHMENT4_EXT): 
      case(GL_COLOR_ATTACHMENT5_EXT): 
      case(GL_COLOR_ATTACHMENT6_EXT): 
      case(GL_COLOR_ATTACHMENT7_EXT): 
      case(GL_COLOR_ATTACHMENT8_EXT): 
      case(GL_COLOR_ATTACHMENT9_EXT): 
      case(GL_COLOR_ATTACHMENT10_EXT): 
      case(GL_COLOR_ATTACHMENT11_EXT): 
      case(GL_COLOR_ATTACHMENT12_EXT): 
      case(GL_COLOR_ATTACHMENT13_EXT): 
      case(GL_COLOR_ATTACHMENT14_EXT): 
      case(GL_COLOR_ATTACHMENT15_EXT):
        drawBufferStr.Printf(wxT("ColorAttach %u"), bufferData.drawBuffer - GL_COLOR_ATTACHMENT0_EXT);        
        break;

      default:
        drawBufferStr.Printf(wxT("Unknown 0x%x"), bufferData.drawBuffer);
        break;
    };

    retString += drawBufferStr;
  }
  else if(bufferData.bufferType == GL_DEPTH_COMPONENT)
  {
    retString = wxT("Depth Buffer");
  }

  return retString;
}
