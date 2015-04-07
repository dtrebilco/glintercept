
#include <wx/wx.h>
#include "GLIBitmapView.h"
#include "GLIBitmapNotebook.h"

#define SCROLL_RATE 10

// The hash bitmap 
static char *hashBitmapXPM[] = {
// columns rows colors chars-per-pixel 
"6 6 2 1",
"  c #C0C0C0",
". c gray100",
// pixels 
" .... ",
". .. .",
"..  ..",
"..  ..",
". .. .",
" .... "
};

static char *targetBitmapXPM[] = {
/* columns rows colors chars-per-pixel */
"32 32 3 1",
"  c black",
". c None",
"X c gray100",
/* pixels */
"................................",
"................................",
"................................",
"................................",
".............     ..............",
"..........           ...........",
".........    .....    ..........",
".......    .........    ........",
".......  ....XXXXX....  ........",
"......  ...XX.....XX...  .......",
".....   ..X.........X..   ......",
".....  ..X...........X..  ......",
".....  ..X...........X..  ......",
"....  ..X...... ......X..  .....",
"....  ..X.....X X.....X..  .....",
"....  ..X....  .  ....X..  .....",
"....  ..X.....X X.....X..  .....",
"....  ..X...... ......X..  .....",
".....  ..X...........X..  ......",
".....  ..X...........X..  ......",
".....   ..X.........X..   ......",
"......  ...XX.....XX...  .......",
".......  ....XXXXX....  ........",
".......    .........    ........",
".........    .....    ..........",
"..........           ...........",
".............     ..............",
"................................",
"................................",
"................................",
"................................",
"................................"
};
#define TARGETBITMAP_CENTERX 15
#define TARGETBITMAP_CENTERY 15

///////////////////////////////////////////////////////////////////////////////
//
BEGIN_EVENT_TABLE(GLIBitmapView, wxScrolledWindow)

  EVT_PAINT  (GLIBitmapView::OnPaint)

  EVT_MOTION (GLIBitmapView::OnMouseMove) 

  EVT_LEFT_DOWN  (GLIBitmapView::OnLeftDown)
  EVT_RIGHT_DOWN  (GLIBitmapView::OnRightDown)

  EVT_LEFT_DCLICK (GLIBitmapView::OnLeftDClick)
  EVT_RIGHT_DCLICK (GLIBitmapView::OnRightDClick)

END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
//
GLIBitmapView::GLIBitmapView(GLIBitmapNotebook *parent):
wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                 wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE),

parentControl(parent),
scaleMultiply(1),
scaleDivide(1)
{
  // Assign the initial scaling values for normalization
  framePreNormalize[0] = 0.0f;
  framePreNormalize[1] = 1.0f;  
  framePostNormalize[0] = 0.0f;
  framePostNormalize[1] = 1.0f;

  // Load the hash pixmap used for rendering the non-image areas
  // (could make this a static shared parameter)
  hashBitmap = wxBitmap(hashBitmapXPM);

  // Load the target display bitmap
  targetBitmap = wxBitmap(targetBitmapXPM);

  // Set that the background does not need to be drawn
  // (prevents scrolling artifacts)
  SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIBitmapView::LoadBitmapData(const FrameBufferData & newData)
{
  //Test for valid data
  if((newData.bufferWidth * newData.bufferHeight * newData.numPixelValues) != newData.postBuffer.size() ||
     (newData.bufferWidth * newData.bufferHeight * newData.numPixelValues) != newData.preBuffer.size())
  {
    return false;
  }

  // Check that there is some size
  if((newData.bufferWidth * newData.bufferHeight * newData.numPixelValues) <= 0)
  {
    return false;
  }
   
  // If it is an invalid number of elements per pixel
  if(newData.numPixelValues != 1 && newData.numPixelValues != 4)
  {
    return false;
  }

  // Assign the internal frame data
  frameData = newData;

  //Loop and get the normalize values value for both pre/post images
  CalculateNormalizeValues(newData.preBuffer, framePreNormalize);
  CalculateNormalizeValues(newData.postBuffer, framePostNormalize);

  // Update the display image
  return UpdateBitmapData();  
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::CalculateNormalizeValues(const std::vector<float> &calcValues, float resultValues[2]) const
{
  // Assign the initial resturn values
  resultValues[0] = 0.0f;
  resultValues[1] = 1.0f;

  // Return now if there are o values to test
  if(calcValues.size() == 0)
  {
    return;
  }

  // Get the initial min/max
  float minValue = calcValues[0];
  float maxValue = calcValues[0];

  // Loop for all other values in the buffer
  for(uint i=1; i < calcValues.size(); i++)
  {
    // Test for min/max and assign
    if(calcValues[i] < minValue)
    {
      minValue = calcValues[i];
    }
    if(calcValues[i] > maxValue)
    {
      maxValue = calcValues[i];
    }
  }

  // Calculate the scale and bias values
  if((maxValue - minValue) > 0.0000001)
  {
    resultValues[0] = minValue;
    resultValues[1] = 1.0f / (maxValue - minValue);
  }
  else
  {
    resultValues[0] = 0.0f;
    resultValues[1] = 1.0f / maxValue;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIBitmapView::UpdateBitmapData()
{
  // Check that there is some size
  if((frameData.bufferWidth * frameData.bufferHeight * frameData.numPixelValues) <= 0)
  {
    return false;
  }
   
  // If it is an invalid number of elements per pixel
  if(frameData.numPixelValues != 1 && frameData.numPixelValues != 4)
  {
    return false;
  }

  // Create the image to load the data into
  displayImage = wxImage(frameData.bufferWidth, frameData.bufferHeight);

  // Get the display format
  uint imageFlags = parentControl->GetImageFlags();
  GLIBitmapNotebook::ImageState imageState = parentControl->GetImageState();
  bool imageNormalized = parentControl->GetImageNormalized();
  bool pixelDiff = false;

  // Get a pointer to the internal data array
  unsigned char *convertBitmap = displayImage.GetData();
  for(uint y=0; y<frameData.bufferHeight; y++)
  {
    for(uint x=0; x<frameData.bufferWidth; x++)
    {
      //DT_TODO: clean up and optimize
      uint offsetDst = ((y * frameData.bufferWidth) + x) * 3;
      uint offsetSrc = (((frameData.bufferHeight - y - 1) * frameData.bufferWidth) + x) * frameData.numPixelValues;      

      //Convert to dst format
      switch(frameData.numPixelValues)
      {
        case(4):
          SetBitmapColor4f(&convertBitmap[offsetDst], &frameData.preBuffer[offsetSrc], &frameData.postBuffer[offsetSrc], imageFlags, imageState, imageNormalized, pixelDiff);
          break;

        case(1):
          SetBitmapColor1f(&convertBitmap[offsetDst], &frameData.preBuffer[offsetSrc], &frameData.postBuffer[offsetSrc], imageFlags, imageState, imageNormalized, pixelDiff);
          break;
      }
    }
  }

  // If all the pixels were the same pre/post and displaying a "diff"
  if(imageState == GLIBitmapNotebook::IS_DiffImage && !pixelDiff)
  {
    // Set entire image to red
    displayImage.SetRGB(wxRect(0,0, displayImage.GetWidth(), displayImage.GetHeight()), 255, 0, 0);
  }

  //Test set an image
  //displayImage = wxImage("C:\\Projects\\GLIntercept\\Research\\test.bmp");

  // Save the existing scroll position
  int oldScrollX = 0;
  int oldScrollY = 0;
  GetViewStart(&oldScrollX, &oldScrollY);

  // Recaluate the display
  RecalculateDisplay();

  // Restore the scroll position
  Scroll(oldScrollX, oldScrollY);

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::SetBitmapColor1f(unsigned char * dstColor, float * srcPreColor, float * srcPostColor,
                                     uint imageFlags, GLIBitmapNotebook::ImageState imageState, bool imageNormalized, bool &pixelDiff)
{
  float preColor[4];
  float postColor[4];

  // Assign the pre-post colors
  preColor[0] = srcPreColor[0];
  preColor[1] = srcPreColor[0];
  preColor[2] = srcPreColor[0];
  preColor[3] = 1.0f;

  postColor[0] = srcPostColor[0];
  postColor[1] = srcPostColor[0];
  postColor[2] = srcPostColor[0];
  postColor[3] = 1.0f;

  // Set to ignore specific color flags
  imageFlags = (GLIBitmapNotebook::IF_RED | GLIBitmapNotebook::IF_GREEN | GLIBitmapNotebook::IF_BLUE);

  // Call the 4 color version
  SetBitmapColor4f(dstColor, preColor, postColor, imageFlags, imageState, imageNormalized, pixelDiff);
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::SetBitmapColor4f(unsigned char * dstColor, float * srcPreColor, float * srcPostColor,
                                     uint imageFlags, GLIBitmapNotebook::ImageState imageState, bool imageNormalized, bool &pixelDiff)
{
  float processColor[4];

  // Get the initial processing data
  if(imageState == GLIBitmapNotebook::IS_PreImage)
  {
    processColor[0] = srcPreColor[0];
    processColor[1] = srcPreColor[1];
    processColor[2] = srcPreColor[2];
    processColor[3] = srcPreColor[3];

    // If the image is normalized
    if(imageNormalized)
    {
      processColor[0] = (processColor[0] - framePreNormalize[0]) * framePreNormalize[1];
      processColor[1] = (processColor[1] - framePreNormalize[0]) * framePreNormalize[1];
      processColor[2] = (processColor[2] - framePreNormalize[0]) * framePreNormalize[1];
      processColor[3] = (processColor[3] - framePreNormalize[0]) * framePreNormalize[1];
    }
  }
  else
  {
    processColor[0] = srcPostColor[0];
    processColor[1] = srcPostColor[1];
    processColor[2] = srcPostColor[2];
    processColor[3] = srcPostColor[3];

    // If the image is normalized
    if(imageNormalized)
    {
      processColor[0] = (processColor[0] - framePostNormalize[0]) * framePostNormalize[1];
      processColor[1] = (processColor[1] - framePostNormalize[0]) * framePostNormalize[1];
      processColor[2] = (processColor[2] - framePostNormalize[0]) * framePostNormalize[1];
      processColor[3] = (processColor[3] - framePostNormalize[0]) * framePostNormalize[1];
    }
  }

  // Process the different channels
  if(imageFlags == GLIBitmapNotebook::IF_RED)
  {
    processColor[0] = processColor[0];
    processColor[1] = processColor[0];
    processColor[2] = processColor[0];
  }
  else if(imageFlags == GLIBitmapNotebook::IF_GREEN)
  {
    processColor[0] = processColor[1];
    processColor[1] = processColor[1];
    processColor[2] = processColor[1];
  }
  else if(imageFlags == GLIBitmapNotebook::IF_BLUE)
  {
    processColor[0] = processColor[2];
    processColor[1] = processColor[2];
    processColor[2] = processColor[2];
  }
  else if(imageFlags == GLIBitmapNotebook::IF_ALPHA)
  {
    processColor[0] = processColor[3];
    processColor[1] = processColor[3];
    processColor[2] = processColor[3];
  }
  else
  {
    // Must be a combination of flags
    if(!(imageFlags & GLIBitmapNotebook::IF_RED))
    {
      processColor[0] = 0.0f;
    }
    if(!(imageFlags & GLIBitmapNotebook::IF_GREEN))
    {
      processColor[1] = 0.0f;
    }
    if(!(imageFlags & GLIBitmapNotebook::IF_BLUE))
    {
      processColor[2] = 0.0f;
    }

    // If there is alpha, blend as if on a red background 
    if(imageFlags & GLIBitmapNotebook::IF_ALPHA)
    {
      processColor[0] = (processColor[0] * processColor[3]) + (1.0f - processColor[3]);
      processColor[1] = (processColor[1] * processColor[3]);
      processColor[2] = (processColor[2] * processColor[3]);
    }
  }

  // If the image is a "diff"
  if(imageState == GLIBitmapNotebook::IS_DiffImage)
  {
    // Assign a green color on no-image change
    if(srcPreColor[0] == srcPostColor[0] &&
       srcPreColor[1] == srcPostColor[1] &&
       srcPreColor[2] == srcPostColor[2] &&
       srcPreColor[3] == srcPostColor[3])
    {
      processColor[0] = 0.0f;
      processColor[1] = 1.0f;
      processColor[2] = 0.0f;
    }
    else
    {
      // Flag that this pixel was different
      pixelDiff = true;
    }
  }

  // DT_TODO: Handle large values? what about negatives?
  // Assign the destination color
  dstColor[0] = (unsigned char)(255.0f * processColor[0]); 
  dstColor[1] = (unsigned char)(255.0f * processColor[1]); 
  dstColor[2] = (unsigned char)(255.0f * processColor[2]); 
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::SetDisplayImage(const wxImage &newImage)
{
  // Assign the new image
  displayImage = newImage;

  // Recaluate the display
  RecalculateDisplay();
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIBitmapView::SetZoomScale(uint newScaleMultiply, uint newScaleDivide)
{
  // Ensure one if the values is 1
  if(newScaleMultiply != 1 && newScaleDivide != 1)
  {
    return false;
  }

  // Do not allow zero
  if(newScaleMultiply == 0 || newScaleDivide == 0)
  {
    return false;
  }

  // Check that the scale is not too great
  if(newScaleMultiply > 1 && newScaleMultiply > MAX_ZOOM_IN_VALUE)
  {
    return false;
  }
  if(newScaleDivide > 1 && newScaleDivide > MAX_ZOOM_OUT_VALUE)
  {
    return false;
  }

  // Assign the new scale values
  scaleMultiply = newScaleMultiply;
  scaleDivide   = newScaleDivide;

  // Recalculate the display for the new scale
  RecalculateDisplay();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::RecalculateDisplay()
{
  // Calculate the new scaled display image 
  if(displayImage.Ok())
  {
    // Calculate the new virtual width/height
    uint newWidth  = displayImage.GetWidth()  * scaleMultiply / scaleDivide;
    uint newHeight = displayImage.GetHeight() * scaleMultiply / scaleDivide;

    // Clamp the image dimensions
    if(newWidth == 0)
    {
      newWidth = 1;
    }
    if(newHeight == 0)
    {
      newHeight = 1;
    }

    // If scaling down, manually scale the image 
    // (There are artifacts using the device context to do the scaling during rendering)
    if(scaleDivide > 1)
    {
      cacheBitmap = wxBitmap(displayImage.Scale(newWidth, newHeight));
    }
    else
    {
      cacheBitmap = wxBitmap(displayImage);
    }

    // Set the new window virtual size
    uint scrollX = newWidth  / SCROLL_RATE;
    uint scrollY = newHeight / SCROLL_RATE;

    //Check that there is enough display room
    if((scrollX * SCROLL_RATE) < newWidth)
    {
      scrollX++;
    }
    if((scrollY * SCROLL_RATE) < newHeight)
    {
      scrollY++;
    }

    SetScrollbars(SCROLL_RATE, SCROLL_RATE, scrollX, scrollY);
  }
  else
  {
    // Set empty data
    cacheBitmap = wxBitmap();
    SetScrollbars( 1, 1, 1, 1);
  }

  // Force a window re-paint
  Refresh();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::OnPaint(wxPaintEvent &event)
{
  wxPaintDC dc(this);
  PrepareDC(dc);

  // If the cache bitmap is not set, return now
  if(!cacheBitmap.Ok())
  {
    return;
  }

  // If the window is greater than the image size, fill the remaining area
  int windowWidth  = 0;
  int windowHeight = 0;
  int scrollStartX = 0;
  int scrollStartY = 0;
  int scrollPPUX = 0;
  int scrollPPUY = 0;
  GetScrollPixelsPerUnit(&scrollPPUX, &scrollPPUY);
  GetViewStart(&scrollStartX, &scrollStartY);
  GetClientSize(&windowWidth, &windowHeight);

  // Add the current scroll offset to the total window area
  windowWidth  += scrollStartX * scrollPPUX; 
  windowHeight += scrollStartY * scrollPPUY; 

  uint imageWidth  = displayImage.GetWidth()  * scaleMultiply / scaleDivide;
  uint imageHeight = displayImage.GetHeight() * scaleMultiply / scaleDivide;

  // If there is extra width area
  if(windowWidth > imageWidth)
  {
    // Draw the filling rectangle hash area
    dc.SetPen(wxPen(*wxWHITE, 1, wxTRANSPARENT));
    dc.SetBrush(wxBrush(hashBitmap));
    dc.DrawRectangle(imageWidth, 0, windowWidth - imageWidth, imageHeight);
  }

  // If there is is extra height area
  if(windowHeight > imageHeight)
  {
    // Draw the filling rectangle hash area
    dc.SetPen(wxPen(*wxWHITE, 1, wxTRANSPARENT));
    dc.SetBrush(wxBrush(hashBitmap));

    // Calculate the draw width
    uint fillWidth = imageWidth;
    if(fillWidth < windowWidth)
    { 
      fillWidth = windowWidth;
    }

    dc.DrawRectangle(0, imageHeight, fillWidth, windowHeight - imageHeight);
  }

  //If scaling the image up. (down scaling is done manually)
  if(scaleMultiply > 1)
  {
    dc.SetUserScale((float)scaleMultiply, (float)scaleMultiply);
  }

  // Draw the bitmap
  dc.DrawBitmap(cacheBitmap, 0, 0, false);

  // Reset scaling
  if(scaleMultiply > 1)
  {
    dc.SetUserScale(1.0, 1.0);
  }

  // If there is a selection point
  wxPoint selectPoint = parentControl->GetBitmapSelectPoint();
  if(selectPoint.x >=0 && selectPoint.y >= 0)
  {
    // Transform the point into virtual bitmap space
    selectPoint.x = (selectPoint.x * scaleMultiply) / scaleDivide;
    selectPoint.y = (selectPoint.y * scaleMultiply) / scaleDivide;

    // Offset by the bitmap center point 
    // (when zooming in the pizels will be large so position in the center of a pixel by using scaleMultiply)
    selectPoint.x = selectPoint.x - TARGETBITMAP_CENTERX + (scaleMultiply / 2); 
    selectPoint.y = selectPoint.y - TARGETBITMAP_CENTERY + (scaleMultiply / 2); 

    // Render the bitmap at the position
    dc.DrawBitmap(targetBitmap, selectPoint.x, selectPoint.y, true);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::OnMouseMove(wxMouseEvent &event)
{
  // Get the (unclamped) bitmap move position
  wxPoint bitmapPos = ConvertWindowToBitmap(wxPoint(event.m_x, event.m_y), false);

  // If the unclamped position is not on the  bitmap, return now
  if(bitmapPos.x < 0 || bitmapPos.x >= displayImage.GetWidth() ||
     bitmapPos.y < 0 || bitmapPos.y >= displayImage.GetHeight())
  {
    // Set an empty tooltip
    SetToolTip(wxT(""));

    event.Skip();
    return;
  }

  // Update the tooltip
  SetToolTip(GetBitmapToolTip(bitmapPos));

  // Skip the event
  event.Skip();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::OnLeftDown(wxMouseEvent &event)
{
  // Get the bitmap click position
  wxPoint bitmapPos = ConvertWindowToBitmap(wxPoint(event.m_x, event.m_y));

  // Call the parent class
  parentControl->OnBitmapLeftClick(bitmapPos);

  // Skip the event
  event.Skip();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::OnRightDown(wxMouseEvent &event)
{
  // Get the bitmap click position
  wxPoint bitmapPos = ConvertWindowToBitmap(wxPoint(event.m_x, event.m_y));

  // Call the parent class
  parentControl->OnBitmapRightClick(bitmapPos);

  // Skip the event
  event.Skip();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::OnLeftDClick(wxMouseEvent &event)
{
  // Get the bitmap click position
  wxPoint bitmapPos = ConvertWindowToBitmap(wxPoint(event.m_x, event.m_y));

  // Call the parent class twice
  parentControl->OnBitmapLeftClick(bitmapPos);
  parentControl->OnBitmapLeftClick(bitmapPos);

  // Skip the event
  event.Skip();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::OnRightDClick(wxMouseEvent &event)
{
  // Get the bitmap click position
  wxPoint bitmapPos = ConvertWindowToBitmap(wxPoint(event.m_x, event.m_y));

  // Call the parent class twice
  parentControl->OnBitmapRightClick(bitmapPos);
  parentControl->OnBitmapRightClick(bitmapPos);

  // Skip the event
  event.Skip();
}

///////////////////////////////////////////////////////////////////////////////
//
wxPoint GLIBitmapView::ConvertWindowToBitmap(const wxPoint &winPoint, bool clampValue)
{
  wxPoint retPoint(0,0);
  
  // Calculate the initail unscrolled position
  CalcUnscrolledPosition(winPoint.x, winPoint.y, &retPoint.x, &retPoint.y);

  // Take into account scaling
  retPoint.x = (retPoint.x * scaleDivide) / scaleMultiply;
  retPoint.y = (retPoint.y * scaleDivide) / scaleMultiply;

  // Clamp to bitmap extents if required
  if(clampValue)
  {
    if(retPoint.x >= displayImage.GetWidth())
    {
      retPoint.x = displayImage.GetWidth()-1;
    }
    if(retPoint.y >= displayImage.GetHeight())
    {
      retPoint.y = displayImage.GetHeight()-1;
    }

    // Clamp to zero
    if(retPoint.x < 0)
    {
      retPoint.x = 0; 
    }
    if(retPoint.y < 0)
    {
      retPoint.y = 0; 
    }
  }

  return retPoint;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIBitmapView::CenterDisplay(const wxPoint &bitmapPoint)
{
  wxPoint centerPoint(bitmapPoint);  

  // Apply the scale and bias to the point
  centerPoint.x = (centerPoint.x * scaleMultiply) / scaleDivide;
  centerPoint.y = (centerPoint.y * scaleMultiply) / scaleDivide;

  // Subtract half the current window size
  int windowWidth  = 0;
  int windowHeight = 0;
  GetClientSize(&windowWidth, &windowHeight);

  centerPoint.x -= windowWidth / 2;
  centerPoint.y -= windowHeight / 2;
  
  // Clamp to zero
  if(centerPoint.x < 0)
  {
    centerPoint.x = 0; 
  }
  if(centerPoint.y < 0)
  {
    centerPoint.y = 0; 
  }

  // Get the point scroll increments
  int scrollPPUX = 0;
  int scrollPPUY = 0;
  GetScrollPixelsPerUnit(&scrollPPUX, &scrollPPUY);

  // Apply the scroll scale
  if(scrollPPUX > 0 && scrollPPUY > 0)
  {
    Scroll(centerPoint.x / scrollPPUX, centerPoint.y / scrollPPUY);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
wxString GLIBitmapView::GetBitmapValueString(const wxPoint& bitmapPoint) const
{
  wxString retValue(wxT("..."));

  // Check if the point is valid
  if(bitmapPoint.x >= 0 && bitmapPoint.x < frameData.bufferWidth &&
     bitmapPoint.y >= 0 && bitmapPoint.y < frameData.bufferHeight)
  {
    uint offsetSrc = (((frameData.bufferHeight - bitmapPoint.y - 1) * frameData.bufferWidth) + bitmapPoint.x) * frameData.numPixelValues;      

    // Get if the display is pre/post image
    const float *bufferData;
    GLIBitmapNotebook::ImageState imageState = parentControl->GetImageState();
    if(imageState == GLIBitmapNotebook::IS_PreImage)
    {
      bufferData = &frameData.preBuffer[offsetSrc];
    }
    else
    {
      bufferData = &frameData.postBuffer[offsetSrc];
    }

    if(frameData.numPixelValues == 4)
    {
      retValue = wxString(wxT("(")) + FormatFloat(bufferData[0]) + wxT(", ") +
                                      FormatFloat(bufferData[1]) + wxT(", ") +
                                      FormatFloat(bufferData[2]) + wxT(", ") +
                                      FormatFloat(bufferData[3]) + wxT(")");
    }
    else if(frameData.numPixelValues == 1)
    {
      retValue = FormatFloat(bufferData[0]);
    }
  }
 
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
wxString GLIBitmapView::GetBitmapToolTip(const wxPoint& bitmapPoint) const
{
  wxString retValue(wxT("..."));

  // Check if the point is valid
  if(bitmapPoint.x >= 0 && bitmapPoint.x < frameData.bufferWidth &&
     bitmapPoint.y >= 0 && bitmapPoint.y < frameData.bufferHeight)
  {
    uint offsetSrc = (((frameData.bufferHeight - bitmapPoint.y - 1) * frameData.bufferWidth) + bitmapPoint.x) * frameData.numPixelValues;           

    // Get if the display is pre/post image
    const float *bufferData;
    GLIBitmapNotebook::ImageState imageState = parentControl->GetImageState();
    if(imageState == GLIBitmapNotebook::IS_PreImage)
    {
      bufferData = &frameData.preBuffer[offsetSrc];
    }
    else
    {
      bufferData = &frameData.postBuffer[offsetSrc];
    }

    // Format the tooltip
    retValue.Printf("XY=(%d,%d)", bitmapPoint.x, bitmapPoint.y);
    if(frameData.numPixelValues == 4)
    {
      retValue += wxString(wxT("\nR=")) + FormatFloat(bufferData[0]) + 
                           wxT("\nG=")  + FormatFloat(bufferData[1]) + 
                           wxT("\nB=")  + FormatFloat(bufferData[2]) + 
                           wxT("\nA=")  + FormatFloat(bufferData[3]) ;
    }
    else if(frameData.numPixelValues == 1)
    {
      retValue += wxString(wxT("\nV=")) + FormatFloat(bufferData[0]); 
    }
  }

  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
wxString GLIBitmapView::FormatFloat(float value) const
{
  // If formatting as integers
  if(parentControl->GetValueTextFormat())
  {
    return wxString::Format("%d", (int)(value * 255.0f));
  }
  else
  {
    wxString newStr;
    newStr.Printf(wxT("%g"), value);
    
    //Ensure there is a trailing .0
    if(newStr.Find('.') < 0 &&
       newStr.Find('e') < 0)
    {
      newStr += ".0";
    }
    return newStr;
  }
}

