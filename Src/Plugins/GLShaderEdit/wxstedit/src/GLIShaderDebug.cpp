
#include <wx/wxprec.h>
#include <wx/stedit/stedit.h>
#include <wx/sstream.h>

#include "GLIShaderData.h"
#include "GLIShaderDebug.h"
#include "GLIDebugVariableGrid.h"
#include "GLIBitmapView.h"
#include "GLIBitmapNotebook.h"

#include "..\\..\\..\\..\\Common\\NetworkUtils.h"
#include "..\\..\\..\\..\\MainLib\\GLDefines.h"

#include "..\\art\\DebugDialog.xpm"

IMPLEMENT_CLASS( GLIShaderDebug, wxFrame )


BEGIN_EVENT_TABLE( GLIShaderDebug, wxFrame )

  EVT_CLOSE(GLIShaderDebug::OnClose)
  EVT_TOOL(wxID_ANY, GLIShaderDebug::OnTool) 

  EVT_TEXT(wxID_ANY, GLIShaderDebug::OnText)

  EVT_BUTTON(ID_BUTTON_DEBUGPIXEL, GLIShaderDebug::OnButtonPixelDebug)

END_EVENT_TABLE()


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
UniformData::UniformData():
name(""),
type(0),
numTypeElements(0),
isFloatType(false)
{

}

///////////////////////////////////////////////////////////////////////////////
//
GLIShaderDebug::GLIShaderDebug(wxSTEditorFrame *newSte, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ):
ste(newSte),

debugToolBar(NULL),
uniformGrid(NULL),
watchGrid(NULL),
bitmapNotebook(NULL),

selectionStaticText(NULL),
xPosTextCtrl(NULL),
yPosTextCtrl(NULL),
debugPixelButton(NULL),
mainPanelSizer(NULL),

debugState(DBS_None),
debugUID(0),
debugGLID(0),
debugSource("")
{
  // Create the frame's controls
  Create( newSte, id, caption, pos, size, style );

  // Set the minimum size of the dialog
  SetMinSize(SYMBOL_GLISHADERDEBUG_MINSIZE);
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderDebug::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  wxFrame::Create( parent, id, caption, pos, size, style );

  // Create the controls
  CreateControls();
  Centre();

  // Set the frame's icons
  wxBitmap bmp(DebugDialog_xpm);
  wxIcon icon;
  icon.CopyFromBitmap(bmp);
  wxIconBundle iconBundle(icon);

  SetIcons(iconBundle);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::CreateControls()
{    
  GLIShaderDebug* itemFrame1 = this;

  /*wxStatusBar* itemStatusBar2 = new wxStatusBar( itemFrame1, ID_STATUSBAR, wxST_SIZEGRIP|wxNO_BORDER );
  itemStatusBar2->SetFieldsCount(2);
  itemStatusBar2->SetStatusText(_("Field One"), 0);
  itemStatusBar2->SetStatusText(_("Field Two"), 1);
  int itemStatusBar2Widths[2];
  itemStatusBar2Widths[0] = 200;
  itemStatusBar2Widths[1] = -1;
  itemStatusBar2->SetStatusWidths(2, itemStatusBar2Widths);
  itemFrame1->SetStatusBar(itemStatusBar2); //*/

  // Setup the tool bar
  debugToolBar = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR );
  debugToolBar->SetToolBitmapSize(wxSize(16, 16));

  wxBitmap emptyBitmap;  
  debugToolBar->AddTool(ID_TOOL_SELECT, _T("Select Pixel"), STE_ARTBMP(wxART_STEDIT_SELECTION), emptyBitmap, wxITEM_RADIO, _T("Select debug pixel"), wxEmptyString);
  debugToolBar->AddTool(ID_TOOL_ZOOM, _T("Zoom"), STE_ARTBMP(wxART_STEDIT_ZOOM), emptyBitmap, wxITEM_RADIO, _T("Left mouse Zoom-in, Right mose zoom-out"), wxEmptyString);

  debugToolBar->AddSeparator();

  debugToolBar->AddTool(ID_TOOL_RGB, _T("RGB"), STE_ARTBMP(wxART_STEDIT_RGB), emptyBitmap, wxITEM_CHECK, _T("Toggle red/green/blue channel display"), wxEmptyString);
  debugToolBar->AddTool(ID_TOOL_RED, _T("Red"), STE_ARTBMP(wxART_STEDIT_RED), emptyBitmap, wxITEM_CHECK, _T("Toggle red channel display"), wxEmptyString);
  debugToolBar->AddTool(ID_TOOL_GREEN, _T("Green"), STE_ARTBMP(wxART_STEDIT_GREEN), emptyBitmap, wxITEM_CHECK, _T("Toggle green channel display"), wxEmptyString);
  debugToolBar->AddTool(ID_TOOL_BLUE, _T("Blue"), STE_ARTBMP(wxART_STEDIT_BLUE), emptyBitmap, wxITEM_CHECK, _T("Toggle blue channel display"), wxEmptyString);
  debugToolBar->AddTool(ID_TOOL_ALPHA, _T("Alpha"), STE_ARTBMP(wxART_STEDIT_ALPHA), emptyBitmap, wxITEM_CHECK, _T("Toggle alpha channel display"), wxEmptyString);

  debugToolBar->AddSeparator();

  debugToolBar->AddTool(ID_TOOL_PRE, _T("Pre"), STE_ARTBMP(wxART_STEDIT_PREIMAGE), emptyBitmap, wxITEM_RADIO, _T("Display frame buffer data \"Pre\" render call"), wxEmptyString);
  debugToolBar->AddTool(ID_TOOL_POST, _T("Post"), STE_ARTBMP(wxART_STEDIT_POSTIMAGE), emptyBitmap, wxITEM_RADIO, _T("Display frame buffer data \"Post\" render call"), wxEmptyString);
  debugToolBar->AddTool(ID_TOOL_DIFF, _T("Diff"), STE_ARTBMP(wxART_STEDIT_DIFFIMAGE), emptyBitmap, wxITEM_RADIO, _T("Display difference in Pre/Post frame buffer"), wxEmptyString);

  debugToolBar->AddSeparator();

  debugToolBar->AddTool(ID_TOOL_INTSCALE, _T("IntScale"), STE_ARTBMP(wxART_STEDIT_INTSCALE), emptyBitmap, wxITEM_CHECK, _T("Toggle value display as integers or floats"), wxEmptyString);
  debugToolBar->AddTool(ID_TOOL_NORMALIZE, _T("Normalize"), STE_ARTBMP(wxART_STEDIT_NORMALIZE), emptyBitmap, wxITEM_CHECK, _T("Scale the image value display so min=0.0 and max=1.0"), wxEmptyString);

  debugToolBar->AddSeparator();

  debugToolBar->AddTool(ID_TOOL_NEXTRENDER, _T("NextRender"), STE_ARTBMP(wxART_STEDIT_NEXTRENDER), emptyBitmap, wxITEM_NORMAL, _T("Skip to next debug render call"), wxEmptyString);


  // Set initial toolbar state
  debugToolBar->ToggleTool(ID_TOOL_SELECT, true);

  debugToolBar->ToggleTool(ID_TOOL_RGB, true);
  debugToolBar->ToggleTool(ID_TOOL_RED, true);
  debugToolBar->ToggleTool(ID_TOOL_GREEN, true);
  debugToolBar->ToggleTool(ID_TOOL_BLUE, true);

  debugToolBar->ToggleTool(ID_TOOL_POST, true);

  debugToolBar->EnableTool(ID_TOOL_NEXTRENDER, false);
/*
  wxStaticText* itemStaticText13 = new wxStaticText( itemToolBar3, wxID_STATIC, _("Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  itemToolBar3->AddControl(itemStaticText13);
  wxSize scaleTextSize = itemStaticText13->GetSize();

  wxTextCtrl* itemTextCtrl14 = new wxTextCtrl( itemToolBar3, ID_TEXTCTRL2, _T(""), wxDefaultPosition, wxSize(40, -1), 0 );
  itemToolBar3->AddControl(itemTextCtrl14);

  wxStaticText* itemStaticText15 = new wxStaticText( itemToolBar3, wxID_STATIC, _("Bias"), wxDefaultPosition, wxSize(25,-1), 0 );
  wxSize biasTextSize = itemStaticText13->GetSize();
  itemToolBar3->AddControl(itemStaticText15);
  
  wxTextCtrl* itemTextCtrl16 = new wxTextCtrl( itemToolBar3, ID_TEXTCTRL3, _T(""), wxPoint(400, 0), wxSize(40, -1), 0 );
  itemToolBar3->AddControl(itemTextCtrl16);
*/
  // Realize and set the tool bar
  debugToolBar->Realize();
  itemFrame1->SetToolBar(debugToolBar);

  // Create the main panel
  wxPanel* mainPanel = new wxPanel( itemFrame1, ID_PANEL7, wxDefaultPosition, wxSize(420, 275), wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
  mainPanelSizer = new wxBoxSizer(wxVERTICAL);
  mainPanel->SetSizer(mainPanelSizer);

  // Create the bitmap notebook
  bitmapNotebook = new GLIBitmapNotebook(this, mainPanel, ID_NOTEBOOK3, wxDefaultPosition, wxDefaultSize, wxNB_TOP );
  mainPanelSizer->Add(bitmapNotebook, 1, wxGROW|wxLEFT, 0);

  // Add the bottom editing controls
  wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
  mainPanelSizer->Add(itemBoxSizer21, 0, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer21->Add(itemBoxSizer22, 1, wxGROW|wxALL, 5);

  selectionStaticText = new wxStaticText(mainPanel, wxID_STATIC, _("Value (1.0, 1.0, 1.0, 1.0)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer22->Add(selectionStaticText, 1, wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 0);

  wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer21->Add(itemBoxSizer24, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticText* itemStaticText25 = new wxStaticText( mainPanel, wxID_STATIC, _("X ="), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer24->Add(itemStaticText25, 0, wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 5);

  xPosTextCtrl = new wxTextCtrl( mainPanel, ID_XPOS_TEXTCTRL, _T(""), wxDefaultPosition, wxSize(50, -1), 0, wxTextValidator(wxFILTER_NUMERIC));
  xPosTextCtrl->SetMaxLength(6);
  itemBoxSizer24->Add(xPosTextCtrl, 0, wxALIGN_CENTER_VERTICAL, 5);

  wxStaticText* itemStaticText27 = new wxStaticText( mainPanel, wxID_STATIC, _("Y ="), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer24->Add(itemStaticText27, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxADJUST_MINSIZE, 5);

  yPosTextCtrl = new wxTextCtrl( mainPanel, ID_YPOS_TEXTCTRL, _T(""), wxDefaultPosition, wxSize(50, -1), 0, wxTextValidator(wxFILTER_NUMERIC));
  yPosTextCtrl->SetMaxLength(6);
  itemBoxSizer24->Add(yPosTextCtrl, 0, wxALIGN_CENTER_VERTICAL, 5);

  debugPixelButton = new wxButton( mainPanel, ID_BUTTON_DEBUGPIXEL, _("Debug Pixel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer24->Add(debugPixelButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);


  //Add the uniform/watch value grid
  wxNotebook * sideNoteBook = ste->GetSideNotebook();
  if(sideNoteBook)
  {
    // Add the uniform display
    uniformGrid = new GLIDebugVariableGrid(sideNoteBook, ID_UNIFORMGRID, GLIDebugVariableGrid::DF_ShowUniforms,
                                            wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
    sideNoteBook->AddPage(uniformGrid, wxT("Uniforms"));
    uniformGrid->RefreshGrid();

    // Add the watch display
    watchGrid = new GLIDebugVariableGrid(sideNoteBook, ID_WATCHGRID, GLIDebugVariableGrid::DF_ShowWatchValues,
                                            wxDefaultPosition, wxDefaultSize, wxNO_BORDER);
    sideNoteBook->AddPage(watchGrid, wxT("Watch Values"));
    watchGrid->RefreshGrid();
  }

  //Set the initial display of watches/uniforms
  UpdateVariableDisplay();
  
  // Set the initial display values
  UpdateWindowTitle();
  UpdateSelectedColor();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::UpdateWindowTitle()
{
  wxString newTitle = SYMBOL_GLISHADERDEBUG_TITLE;

  // If initialized
  if(debugState != DBS_None && bitmapNotebook)
  {
    // Get the current image size
    wxSize imageSize = bitmapNotebook->GetImageSize();

    // If the width/height are of valid size
    if(imageSize.GetWidth() > 0 && imageSize.GetWidth() > 0)
    {
      // Get the current zoom factor
      uint zoomMultiply;
      uint zoomDivide;
      bitmapNotebook->GetZoomScale(zoomMultiply, zoomDivide); 

      // Format the new title    
      newTitle.Printf(wxT("%s %dx%d [%u:%u]"), SYMBOL_GLISHADERDEBUG_TITLE, imageSize.GetWidth(), imageSize.GetHeight(), zoomMultiply, zoomDivide); 
    }
  }
  else
  {
    newTitle.Printf(wxT("%s - Waiting for debug render call"), SYMBOL_GLISHADERDEBUG_TITLE); 
  }

  // Set the new title
  SetTitle(newTitle);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::UpdateSelectedColor()
{
  // If there is no controls, abort
  if(!bitmapNotebook || !selectionStaticText)
  {
    return;
  }

  // Get the selected position display string
  wxString displayString = bitmapNotebook->GetBitmapValueString();

  // Set the text on the text display
  selectionStaticText->SetLabel(wxString(wxT("Debug Pixel Values:\n")) + displayString);
 
  // Update the layout to handle the new text
  if(mainPanelSizer)
  {
    mainPanelSizer->Layout();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::SetDebugPixel(const wxPoint &newPixel)
{
  // If the debug controls exist
  if(!bitmapNotebook || !xPosTextCtrl || !yPosTextCtrl)
  {
    return;
  }

  // Set the x location
  wxString xPos;
  xPos << newPixel.x;
  xPosTextCtrl->SetValue(xPos);

  // Set the y location
  wxString yPos;
  yPos << newPixel.y;
  yPosTextCtrl->SetValue(yPos);
  
  // The text update callback handles setting the real stored value
}

///////////////////////////////////////////////////////////////////////////////
//
wxBitmap GLIShaderDebug::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin GLIShaderDebug bitmap retrieval
/*
    wxUnusedVar(name);
    if (name == _T("undo.xpm"))
    {
        wxBitmap bitmap(undo_xpm);
        return bitmap;
    }
    else if (name == _T("redo.xpm"))
    {
        wxBitmap bitmap(redo_xpm);
        return bitmap;
    }
    else if (name == _T("copy.xpm"))
    {
        wxBitmap bitmap(copy_xpm);
        return bitmap;
    }
    else if (name == _T("cut.xpm"))
    {
        wxBitmap bitmap(cut_xpm);
        return bitmap;
    }
    else if (name == _T("paste.xpm"))
    {
        wxBitmap bitmap(paste_xpm);
        return bitmap;
    }*/
    return wxNullBitmap;
////@end GLIShaderDebug bitmap retrieval
}

///////////////////////////////////////////////////////////////////////////////
//
wxIcon GLIShaderDebug::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin GLIShaderDebug icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end GLIShaderDebug icon retrieval
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::OnClose(wxCloseEvent& event)
{
  //If the close event can be vetoed
  if(event.CanVeto())
  {
    //Veto window destruction
    event.Veto();

    // If debugging a render call, ask if the current debug session should be ended
    if(debugState != DBS_None)
    {
      wxMessageDialog closeDialog(this, wxT("Stop debugging current render call?"), wxT("Render call debug shutdown"), wxYES_NO | wxCANCEL);
      int closeAction = closeDialog.ShowModal();

      // If cancelling, return now
      if(closeAction == wxID_CANCEL)
      {
        return;
      }
      else if(closeAction == wxID_YES)
      {
        // If debugging on current render call is stopped
        DebugEnd();
      }
    }

    //Simply hide the window
    Show(false);
    return;
  }

  //Let the window be destroyed
  Destroy();

  //Set the pointer in the parent to NULL
  ste->OnShaderDebugDestroy();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::OnTool(wxCommandEvent& event)
{
  // If skipping to the next render
  if(event.GetId() == ID_TOOL_NEXTRENDER)
  {
    DebugEnd();
    return;
  }

  // Handle the RGB toggle
  if(event.GetId() == ID_TOOL_RGB)
  {
    // Set/unset the RGB components
    bool setState = debugToolBar->GetToolState(ID_TOOL_RGB);
    debugToolBar->ToggleTool(ID_TOOL_RED, setState);
    debugToolBar->ToggleTool(ID_TOOL_GREEN, setState);
    debugToolBar->ToggleTool(ID_TOOL_BLUE, setState);
  }

  // Sync the state
  SyncToolBarState();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::OnText(wxCommandEvent& event)
{
  // Abort if not the x/y text controls
  if(event.GetId() != ID_XPOS_TEXTCTRL &&
     event.GetId() != ID_YPOS_TEXTCTRL)
  {
    return;
  }

  // Sync the current state of the controls with the selected pixel value
  SyncSelectedPixel();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::OnButtonPixelDebug(wxCommandEvent& event)
{
  // If no debugging, return now
  if(!GLIShaderData::GetInstance() || !bitmapNotebook)
  {
    return;
  }

  // Make sure the state is correct
  if(debugState == DBS_None)
  {
    ste->OutputAppendString("== Unable to debug a pixel until a render call occurs ==\n");
    return;
  }
  
  // Get the debug point
  wxPoint debugPoint = bitmapNotebook->GetBitmapSelectPoint();
  if(debugPoint.x < 0 || debugPoint.y < 0)
  {
    ste->OutputAppendString("== Invalid debug pixel location ==\n");
    return;
  }

  // Start debugging on the pixel
  if(!GLIShaderData::GetInstance()->SetDebugPixel(debugPoint))
  {
    ste->OutputAppendString("== Unable to start debugging the pixel ==\n");
    return;
  }

  // Assign the new state
  debugState = DBS_Pixel;

  // Display a dialog until GLIntercept responds?
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderDebug::DebugBegin(uint bufferSize, void *buffer)
{
  uint i;

  //Check the debug state
  if(debugState != DBS_None)
  {
    ste->OutputAppendString("== Unable to switch to debug mode. Already debugging? ==\n");
    return false;
  }

  //Check the input data
  if(bufferSize == 0 || buffer == NULL)
  {
    return false;
  }

  //Clear all existing uniforms/frame buffer data
  uniformValues.clear();
  frameBufferArray.clear();

  //Read the input data
  NetworkBufferReceive bufferData(bufferSize, buffer);

  //Get the debug shader UID/GL ID  
  if(!bufferData.Get(debugUID) ||
     !bufferData.Get(debugGLID))
  {
    return false;
  }

  //Get the shader source
  string networkString;
  if(!bufferData.Get(networkString))
  {
    return false;
  }
  debugSource = networkString.c_str();

  //Get the number of uniforms
  uint numUniforms = 0;
  if(!bufferData.Get(numUniforms))
  {
    return false;
  }

  //Loop and get each uniform 
  for(i=0; i<numUniforms; i++)
  {
    UniformData newUniform;
    uint isFloat;

    //Get the main uniform data
    if(!bufferData.Get(networkString) ||
       !bufferData.Get(newUniform.type) ||
       !bufferData.Get(newUniform.numTypeElements) ||
       !bufferData.Get(isFloat))
    {
      return false;
    }
    newUniform.name = networkString.c_str();

    //Assign if it is a float or not
    if(isFloat > 0)
    {
      newUniform.isFloatType = true;
      if(!bufferData.Get(newUniform.floatUniformData))
      {
        return false;
      }

      //Return now if there is an uneven number of variables
      if(newUniform.floatUniformData.size() % newUniform.numTypeElements != 0)
      { 
        return false;
      }
    }
    else
    {
      newUniform.isFloatType = false;
      if(!bufferData.Get(newUniform.intUniformData))
      {
        return false;
      }

      //Return now if there is an uneven number of variables
      if(newUniform.intUniformData.size() % newUniform.numTypeElements != 0)
      { 
        return false;
      }
    }

    //Add the uniform data
    uniformValues.push_back(newUniform);
  }

  //Get the number of frame buffers
  uint numFrameBuffers = 0;
  if(!bufferData.Get(numFrameBuffers))
  {
    return false;
  }

  //Loop and get each frame buffer
  for(i=0; i<numFrameBuffers; i++)
  {
    //Add a new item
    frameBufferArray.push_back(FrameBufferData());

    //Get a reference to it (faster than copying all this buffer data twice)
    FrameBufferData &newFrameBuffer = frameBufferArray.back();

    //Get the main frame buffer data
    if(!bufferData.Get(newFrameBuffer.bufferType) ||
       !bufferData.Get(newFrameBuffer.drawBuffer) ||
       !bufferData.Get(newFrameBuffer.bufferWidth) ||
       !bufferData.Get(newFrameBuffer.bufferHeight) ||
       !bufferData.Get(newFrameBuffer.numPixelValues) ||
       !bufferData.Get(newFrameBuffer.preBuffer) ||
       !bufferData.Get(newFrameBuffer.postBuffer))
    {
      frameBufferArray.pop_back();
      return false;
    }

    //Check the size of the data
    if(newFrameBuffer.preBuffer.size() != newFrameBuffer.postBuffer.size() ||
       newFrameBuffer.preBuffer.size() != (newFrameBuffer.bufferWidth * 
                                           newFrameBuffer.bufferHeight * 
                                           newFrameBuffer.numPixelValues))
    {
      ste->OutputAppendString("== Invalid frame buffer data ==\n");
      frameBufferArray.pop_back();
      return false;
    }
  }

  //Check for any remaining network data
  if(bufferData.GetBufferSizeLeft() > 0)
  {
    ste->OutputAppendString("== Extra debug start data found in network buffer ==\n");
  }

  //Flag that debug mode has started
  debugState = DBS_Init;

  //Update the watch/uniform display
  UpdateVariableDisplay();

  // Hide the notebook while updating
  bitmapNotebook->Hide();

  //Show the initial bitmaps
  bitmapNotebook->ResetAllPages();
  SyncToolBarState();
  for(i=0; i<frameBufferArray.size(); i++)
  {
    bitmapNotebook->AddBitmapPage(frameBufferArray[i]);
  }

  // Show the notebook
  bitmapNotebook->Show(true);

  // Enable the "next render" button
  debugToolBar->EnableTool(ID_TOOL_NEXTRENDER, true);

  // Sync the selected pixel positions with the current dialog display
  SyncSelectedPixel();

  // Ensure the current selected pixel display is updated
  UpdateSelectedColor();

  //Ensure the source that is being debugged is open
  OpenDebugSource();

  // Show the uniform page selection
  ste->ShowSideNotebookWindow(uniformGrid);

  //Show the window
  Show(true);
  SetFocus();
  RequestUserAttention();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderDebug::DebugEnd()
{
  //Check the debug state
  if(debugState == DBS_None)
  {
    ste->OutputAppendString("== Cannot exit debug mode when not debugging ==\n");
    return false;
  }

  // Reset the state
  debugState = DBS_None;

  // Reset the bitmap notebook
  if(bitmapNotebook)
  {
    bitmapNotebook->ResetAllPages();
  }

  // Sync the pixel selection
  SyncSelectedPixel();

  // Disable the "next render" button
  debugToolBar->EnableTool(ID_TOOL_NEXTRENDER, false);

  // Reset the uniform display?

  // Flag to GLIntercept that debugging is done
  if(GLIShaderData::GetInstance())
  {
    GLIShaderData::GetInstance()->SetDebugEnd();
  }

  // Hide the window?
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::OpenDebugSource()
{
  //Create a filename from the source ID's
  wxString debugFileName;
  debugFileName.Printf(wxT("%u_%u."), debugUID, debugGLID);

  //Can only currently debug GLSL shaders
  debugFileName += "glsl";
  
  //Make the filename lower case
  debugFileName.MakeLower();

  //Open using the current "file name"
  wxSTEditorNotebook * notebook = ste->GetEditorNotebook();
  if(notebook)
  {
    //Load in the string as a stream
    wxStringInputStream strStream(debugSource); 
    notebook->LoadStream(debugFileName, strStream);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::UpdateVariableDisplay()
{
  // Refresh the uniform grid
  if(uniformGrid)
  {
    uniformGrid->SetUniformData(uniformValues);
    uniformGrid->RefreshGrid();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::SyncSelectedPixel()
{
  // If the debug controls exist
  if(!debugPixelButton || !bitmapNotebook || !xPosTextCtrl || !yPosTextCtrl)
  {
    return;
  }

  // Get the x/y text strings
  wxPoint setPoint(-1, -1);
  wxString xPosStr = xPosTextCtrl->GetValue();
  wxString yPosStr = yPosTextCtrl->GetValue();

  // If x and y are both valid numbers
  if(xPosStr.IsNumber() && yPosStr.IsNumber())
  {
    // Get the number version of the string and assign the point
    long xPosLong;
    long yPosLong;
    if(xPosStr.ToLong(&xPosLong) &&
       yPosStr.ToLong(&yPosLong))
    {
      setPoint.x = xPosLong;
      setPoint.y = yPosLong;
    }
  }

  // Set the new selection point
  bitmapNotebook->SetSelectedPosition(setPoint);

  // Get if the position is valid
  wxPoint testPoint = bitmapNotebook->GetBitmapSelectPoint();

  // Enable/Disable the button based on the status
  if(testPoint.x < 0 || testPoint.y < 0)
  {
    debugPixelButton->Enable(false);
  }
  else
  {
    debugPixelButton->Enable(true);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::SyncToolBarState()
{
  if(!debugToolBar)
  {
    return;
  }

  // Get the tool state
  GLIBitmapNotebook::ToolState toolState = GLIBitmapNotebook::TS_Select;
  if(debugToolBar->GetToolState(ID_TOOL_ZOOM))
  {
    toolState = GLIBitmapNotebook::TS_Zoom;
  }

  // Get the display flags
  uint imageFlags = 0;
  if(debugToolBar->GetToolState(ID_TOOL_RED))
  {
    imageFlags |= GLIBitmapNotebook::IF_RED;
  }
  if(debugToolBar->GetToolState(ID_TOOL_GREEN))
  {
    imageFlags |= GLIBitmapNotebook::IF_GREEN;
  }
  if(debugToolBar->GetToolState(ID_TOOL_BLUE))
  {
    imageFlags |= GLIBitmapNotebook::IF_BLUE;
  }
  if(debugToolBar->GetToolState(ID_TOOL_ALPHA))
  {
    imageFlags |= GLIBitmapNotebook::IF_ALPHA;
  }

  // Update the RGB toggle
  if((imageFlags & GLIBitmapNotebook::IF_RGB) == GLIBitmapNotebook::IF_RGB)
  {
    debugToolBar->ToggleTool(ID_TOOL_RGB, true);
  }
  else
  {
    debugToolBar->ToggleTool(ID_TOOL_RGB, false);
  }

  // Get the display image type
  GLIBitmapNotebook::ImageState imageState = GLIBitmapNotebook::IS_PreImage;
  if(debugToolBar->GetToolState(ID_TOOL_POST))
  {
    imageState = GLIBitmapNotebook::IS_PostImage;
  }
  else if(debugToolBar->GetToolState(ID_TOOL_DIFF))
  {
    imageState = GLIBitmapNotebook::IS_DiffImage;
  }

  // Get if displaying ints or floats
  bool intScaleDisplay = debugToolBar->GetToolState(ID_TOOL_INTSCALE);
  
  // Get if the image display should be "normalized"
  bool imageNormalized = debugToolBar->GetToolState(ID_TOOL_NORMALIZE);

  // Set the states
  bitmapNotebook->SetToolState(toolState);
  bitmapNotebook->SetImageDisplay(imageFlags, imageState, imageNormalized);
  
  // Set if displying ints or floats
  bitmapNotebook->SetValueTextFormat(intScaleDisplay);
}


