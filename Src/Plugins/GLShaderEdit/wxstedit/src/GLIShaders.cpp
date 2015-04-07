

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "GLIShaders.h"
#include "GLIShaderData.h"
#include "wx/stedit/stedit.h"
#include "wx/stedit/steframe.h"

#include <algorithm>

using namespace std;


BEGIN_EVENT_TABLE( GLIShaders, wxDialog )

  EVT_CHECKBOX( ID_CHECKBOX_SHOWSHADER, GLIShaders::OnCheckboxShowShaderClick )
  EVT_CHECKBOX( ID_CHECKBOX_BREAKSHADER, GLIShaders::OnCheckboxBreakShaderClick )

  EVT_LIST_ITEM_ACTIVATED( ID_LISTCTRL, GLIShaders::OnListctrlItemActivated )
  EVT_LIST_ITEM_FOCUSED( ID_LISTCTRL, GLIShaders::OnListctrlItemFocused )
  EVT_LIST_COL_CLICK( ID_LISTCTRL, GLIShaders::OnListctrlColLeftClick )

  EVT_BUTTON( ID_BUTTON_OPENSHADER, GLIShaders::OnButtonOpenShaderClick )
  EVT_BUTTON( ID_BUTTON_REVERTSHADER, GLIShaders::OnButtonRevertShaderClick )
  EVT_BUTTON( ID_BUTTON_REFRESHSHADERS, GLIShaders::OnButtonRefreshShadersClick )

  EVT_MOVE  ( GLIShaders::OnMove ) 

END_EVENT_TABLE()


///////////////////////////////////////////////////////////////////////////////
//
ListItemData::ListItemData():
shaderUID(0),
openGLID(0),
frameNum(0)
{
}


///////////////////////////////////////////////////////////////////////////////
//
GLIShaders::GLIShaders(wxSTEditorFrame *newSte, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ):
ste(newSte),
shaderListCtrl(NULL),
positionOffset(0,0),

columnSort(CS_FrameNum),
sortReverse(true)
{
  Create(newSte, id, caption, pos, size, style);

  // Calculate the default position offset
  wxSize parentSize = ste->GetSize();
  wxSize dialogSize = SYMBOL_GLISHADERS_SIZE;

  positionOffset.x = parentSize.x  - (dialogSize.x/3);
  positionOffset.y = 60;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaders::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{

////@begin GLIShaders creation
  SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
  Centre();
////@end GLIShaders creation
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::CreateControls()
{    
////@begin GLIShaders content construction

  GLIShaders* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxRIGHT|wxTOP|wxBOTTOM, 2);

  wxCheckBox* itemCheckBox4 = new wxCheckBox( itemDialog1, ID_CHECKBOX_SHOWSHADER, _("Show shader usage"), wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox4->SetValue(false);
  itemCheckBox4->SetToolTip(_("Flash the selected shader in the render window"));
  itemBoxSizer3->Add(itemCheckBox4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  itemBoxSizer3->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxCheckBox* itemCheckBox6 = new wxCheckBox( itemDialog1, ID_CHECKBOX_BREAKSHADER, _("Break on shader usage"), wxDefaultPosition, wxDefaultSize, 0 );
  itemCheckBox6->SetValue(false);
  itemCheckBox6->SetToolTip(_("Throw a debugger breakpoint when the selected shader is used. (If run from an IDE)"));
  itemBoxSizer3->Add(itemCheckBox6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);

  shaderListCtrl = new wxListCtrl( itemDialog1, ID_LISTCTRL, wxPoint(0, 0), wxSize(265, 360), wxLC_REPORT );
  shaderListCtrl->SetToolTip(_("List of shaders currently available"));

  //Create the four columns of shader data
  wxListItem itemCol;
  itemCol.SetText(_("Shader UID"));
  itemCol.SetWidth(70);
  itemCol.SetAlign(wxLIST_FORMAT_LEFT);
  shaderListCtrl->InsertColumn(0, itemCol);

  itemCol.SetText(_("OpenGL ID"));
  itemCol.SetWidth(70);
  itemCol.SetAlign(wxLIST_FORMAT_LEFT);
  shaderListCtrl->InsertColumn(1, itemCol);

  itemCol.SetText(_("Type"));
  itemCol.SetWidth(46);
  itemCol.SetAlign(wxLIST_FORMAT_LEFT);
  shaderListCtrl->InsertColumn(2, itemCol);

  itemCol.SetText(_("Frame Num"));
  itemCol.SetWidth(73);
  itemCol.SetAlign(wxLIST_FORMAT_LEFT);
  shaderListCtrl->InsertColumn(3, itemCol);
  
  itemBoxSizer2->Add(shaderListCtrl, 1, wxGROW|wxALL, 5);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);

  wxButton* itemButton9 = new wxButton( itemDialog1, ID_BUTTON_OPENSHADER, _("Open"), wxDefaultPosition, wxDefaultSize, 0 );
  itemButton9->SetToolTip(_("Open the selected shader for editing"));
  itemBoxSizer8->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxButton* itemButton10 = new wxButton( itemDialog1, ID_BUTTON_REVERTSHADER, _("Revert"), wxDefaultPosition, wxDefaultSize, 0 );
  itemButton10->SetToolTip(_("Revert the selected shader to undo any edits"));
  itemBoxSizer8->Add(itemButton10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxButton* itemButton11 = new wxButton( itemDialog1, ID_BUTTON_REFRESHSHADERS, _("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
  itemButton11->SetToolTip(_("Refresh the shader list"));
  itemBoxSizer8->Add(itemButton11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end GLIShaders content construction
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::UpdateWindowPosition()
{
  // Get the now position relative to the parent
  wxPoint newPos = ste->GetPosition() + positionOffset;

  // Get the screen width and height
  int screenWidth  = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
  int screenHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

  // Get this dialog's size
  wxSize dialogSize = GetSize();

  // Clamp to the desktop dimensions
  if(newPos.x + dialogSize.x > screenWidth)
  {
    newPos.x = screenWidth - dialogSize.x;
  }
  if(newPos.y + dialogSize.y > screenHeight)
  {
    newPos.y = screenHeight - dialogSize.y;
  }
  if(newPos.x < 0)
  {
    newPos.x = 0;
  }
  if(newPos.y < 0)
  {
    newPos.y = 0;
  }

  // Move to the offset position position
  wxPoint oldPositionOffset = positionOffset;
  Move(newPos.x, newPos.y);

  // Restore the position offset after a manual move
  positionOffset = oldPositionOffset;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::OnMove(wxMoveEvent &event)
{
  // Get the new position relative to the main window
  positionOffset = GetPosition() - ste->GetPosition();

  event.Skip();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::OnCheckboxShowShaderClick( wxCommandEvent& event )
{
  //If the shader data exists, set the current shader "show shader" state
  if(GLIShaderData::GetInstance())
  {
    GLIShaderData::GetInstance()->SetShowShader(currShaderData.shaderUID, event.IsChecked());
  }
  else
  {
    event.Skip();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::OnCheckboxBreakShaderClick( wxCommandEvent& event )
{
  //If the shader data exists, set the current shader "break shader" state
  if(GLIShaderData::GetInstance())
  {
    GLIShaderData::GetInstance()->SetBreakShader(currShaderData.shaderUID, event.IsChecked());
  }
  else
  {
    event.Skip();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::OnListctrlItemActivated( wxListEvent& event )
{
  //Set as the active item
  SetSelectedShader(event.GetIndex());

  //Open the item
  OpenSelectedShader();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::OnListctrlItemFocused( wxListEvent& event )
{
  //Set the item as focused
  SetSelectedShader(event.GetIndex());
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::OnListctrlColLeftClick( wxListEvent& event )
{
  //Order based on the column clicked
  if(event.GetColumn() >= 0)
  {
    SetColumnSort(event.GetColumn());
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::OnButtonOpenShaderClick( wxCommandEvent& event )
{
  OpenSelectedShader();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::OnButtonRevertShaderClick( wxCommandEvent& event )
{
  RevertSelectedShader();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::OnButtonRefreshShadersClick( wxCommandEvent& event )
{
  //If the shader data exists, refresh the shaders
  if(GLIShaderData::GetInstance())
  {
    GLIShaderData::GetInstance()->RefreshShaderList();
  }
  else
  {
    event.Skip();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
wxBitmap GLIShaders::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
////@begin GLIShaders bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
////@end GLIShaders bitmap retrieval
}

///////////////////////////////////////////////////////////////////////////////
//
wxIcon GLIShaders::GetIconResource( const wxString& name )
{
  // Icon retrieval
////@begin GLIShaders icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
////@end GLIShaders icon retrieval
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::SetSelectedShader(int rowNum)
{
  //Save the old ID
  uint oldShaderID = currShaderData.shaderUID;

  //If the row is valid, extract the list data
  if(rowNum >= 0)
  {
    //Attempt to get the data
    if(!GetListItem(rowNum, currShaderData))
    {
      return;
    }
  }
  else
  {
    //Reset the list data
    currShaderData = ListItemData();
  }

  //Pass to GLIntercept if the shader has changed
  if(GLIShaderData::GetInstance() && oldShaderID != currShaderData.shaderUID)
  {
    GLIShaderData::GetInstance()->SetSelectedShader(currShaderData.shaderUID);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::OpenSelectedShader()
{
  //If the shader data exists 
  if(GLIShaderData::GetInstance() && currShaderData.shaderUID != 0)
  {
    wxString currFileName;
    
    //Set the current source retrieve string
    currFileName.Printf(wxT("%u_%u."), currShaderData.shaderUID, currShaderData.openGLID);
    currFileName += currShaderData.shaderType;
    
    //Make the filename lower case
    currFileName.MakeLower();

    //Request the shader from GLIntercept and open the result in the editor
    GLIShaderData::GetInstance()->OpenShader(currShaderData.shaderUID, currFileName);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::RevertSelectedShader()
{
  //If the shader data exists
  if(GLIShaderData::GetInstance() && currShaderData.shaderUID != 0)
  {
    wxString currFileName;
    
    //Set the current source retrieve string
    currFileName.Printf(wxT("%u_%u."), currShaderData.shaderUID, currShaderData.openGLID);
    currFileName += currShaderData.shaderType;
    
    //Make the filename lower case
    currFileName.MakeLower();

    //Revert the shader filename
    GLIShaderData::GetInstance()->RevertShader(currShaderData.shaderUID, currFileName);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::SetShaderListData( const wxString &dataStr)
{
  //If there is no control, return
  if(!shaderListCtrl)
  {
    return;
  }

  //Reset the selected shader
  SetSelectedShader(-1);

  //To speed up inserting, hide the control temporarily
  shaderListCtrl->Hide();

  //Clear all the rows
  shaderListCtrl->DeleteAllItems();

  //Extract each row of data
  int startPos = 0;
  int endPos   = dataStr.find('*');

  while (endPos > 0)
  {
    //Extract a single shaders data
    wxString shaderData = dataStr.SubString(startPos, endPos-1);

    //Add a shader row
    AddShaderRow(0, shaderData);

    //Get the next start values
    startPos = endPos + 1;
    endPos   = dataStr.find('*',startPos);
  }

  //Re-show the control
  shaderListCtrl->Show();

  //Sort the data by the selected column
  SortList();

}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::AddShaderRow(uint rowNum, const wxString &shaderData)
{
  //If there is no control, return
  if(!shaderListCtrl)
  {
    return;
  }

  wxString listData[4];
  wxString processData = shaderData;

  //Extract each token from the shader
  for(uint i=0; i<4; i++)
  {
    int endPos = processData.find(':');
    if(endPos < 0)
    {
      ste->OutputAppendString(wxT("== Error extracting shaders ==\n"));
      return;
    }

    //Extract the string data
    listData[i] = wxString(processData.c_str(), endPos);

    //Remove the string from the processing data
    processData.Remove(0, endPos+1);
  }

  //Get the shader ID
  unsigned long shaderUID = 0;
  if(!listData[0].ToULong(&shaderUID))
  {
    ste->OutputAppendString(wxT("== Error extracting shaders - Invalid Shader ID ==\n"));
    return;
  }

  //Insert the row
  if(shaderListCtrl->InsertItem(rowNum, listData[0]) == -1)
  {
    return;
  }

  //Assign the row ID
  shaderListCtrl->SetItemData(rowNum, shaderUID);

  //Assign the rest of the list data
  shaderListCtrl->SetItem(rowNum, 1, listData[1]);
  shaderListCtrl->SetItem(rowNum, 2, listData[2]);
  shaderListCtrl->SetItem(rowNum, 3, listData[3]);  

}


///////////////////////////////////////////////////////////////////////////////
//
bool CompareShaderUID(const ListItemData& a, const ListItemData& b)
{
  return (a.shaderUID < b.shaderUID);
}

///////////////////////////////////////////////////////////////////////////////
//
bool CompareOpenGLID(const ListItemData& a, const ListItemData& b)
{
  return (a.openGLID < b.openGLID);
}

///////////////////////////////////////////////////////////////////////////////
//
bool CompareShaderType(const ListItemData& a, const ListItemData& b)
{
  return (strcmp(a.shaderType.c_str(), b.shaderType.c_str()) < 0);
}

///////////////////////////////////////////////////////////////////////////////
//
bool CompareFrameNum(const ListItemData& a, const ListItemData& b)
{
  return (a.frameNum < b.frameNum);
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaders::SortList()
{
  uint i;

  //If no sorting is performed
  if(columnSort == CS_Undefined)
  {
    return true;
  }

  //Get the list box
  if(!shaderListCtrl)
  {
    return false;
  }

  //Get the number of list items
  uint numListItems = shaderListCtrl->GetItemCount();
  if(numListItems == 0)
  {
    return true;
  }

  //Create a array to hold the items
  ListItemData *listDataArray = new ListItemData[numListItems];

  //Extract the items from the array
  for(i=0; i<numListItems; i++)
  {
    if(!GetListItem(i, listDataArray[i]))
    {
      //Delete the array and return
      delete [] listDataArray;
      return false;
    }
  }

  //Sort the array
  switch(columnSort)
  {
    //Note that STL sort requires one element past the end of the array for the "end" of sort.
    case(CS_ShaderUID):
      sort(&listDataArray[0], &listDataArray[numListItems], CompareShaderUID); 
      break;
    case(CS_OpenGLID):
      sort(&listDataArray[0], &listDataArray[numListItems], CompareOpenGLID); 
      break;
    case(CS_ShaderType):
      sort(&listDataArray[0], &listDataArray[numListItems], CompareShaderType); 
      break;
    case(CS_FrameNum):
      sort(&listDataArray[0], &listDataArray[numListItems], CompareFrameNum); 
      break;
  }
  
  //To speed up inserting, hide the control temporarily
  shaderListCtrl->Hide();

  //Empty all existing data in the list
  shaderListCtrl->DeleteAllItems();

  //Put the data back into the array
  if(!sortReverse)
  {
    for(i=0; i<numListItems; i++)
    {
      AddListItem(i, listDataArray[i]);
    }
  }
  else
  {
    for(i=0; i<numListItems; i++)
    {
      AddListItem(i, listDataArray[numListItems-1-i]);
    }
  }

  //Re-show the control
  shaderListCtrl->Show();

  //Delete the array
  delete [] listDataArray;

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::AddListItem(uint rowNum, const ListItemData &listData)
{
  wxString listDataStr[4];
  listDataStr[0].Printf(wxT("%u"), listData.shaderUID);
  listDataStr[1].Printf(wxT("%u"), listData.openGLID);
  listDataStr[2] = listData.shaderType;
  listDataStr[3].Printf(wxT("%u"), listData.frameNum);

  //If there is no control, return
  if(!shaderListCtrl)
  {
    return;
  }

  //Insert the row
  if(shaderListCtrl->InsertItem(rowNum, listDataStr[0]) == -1)
  {
    return;
  }

  //Assign the row ID
  shaderListCtrl->SetItemData(rowNum, listData.shaderUID);

  //Assign the rest of the list data
  shaderListCtrl->SetItem(rowNum, 1, listDataStr[1]);
  shaderListCtrl->SetItem(rowNum, 2, listDataStr[2]);
  shaderListCtrl->SetItem(rowNum, 3, listDataStr[3]);  

}



///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaders::GetListItem(uint rowNum, ListItemData &listData) const
{
  unsigned long newLong = 0;
  ListItemData retData;

  //If there is no control, return
  if(!shaderListCtrl)
  {
    return false;
  }

  //Get the shader unique ID
  retData.shaderUID = shaderListCtrl->GetItemData(rowNum);

  //Get the OpenGL ID  
  wxListItem getItem;
  getItem.m_mask = wxLIST_MASK_TEXT;
  getItem.SetId(rowNum);
  getItem.SetColumn(1);
  if(!shaderListCtrl->GetItem(getItem) ||
     !getItem.GetText().ToULong(&newLong))
  {
    return false;
  }
  retData.openGLID = newLong;

  //Get the GL Type 
  getItem.SetColumn(2);
  if(!shaderListCtrl->GetItem(getItem))
  {
    return false;
  }
  retData.shaderType = getItem.GetText();

  //Get the frame number 
  getItem.SetColumn(3);
  if(!shaderListCtrl->GetItem(getItem) ||
     !getItem.GetText().ToULong(&newLong))
  {
    return false;
  }
  retData.frameNum = newLong;

  //Assign the return data on success
  listData = retData;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaders::SetColumnSort(uint columnNum)
{
  //Determine the column type
  ColumnSortOrder newOrder = CS_Undefined;
  switch(columnNum)
  {
    case(0):
      newOrder = CS_ShaderUID;
      break;
    case(1):
      newOrder = CS_OpenGLID;
      break;
    case(2):
      newOrder = CS_ShaderType;
      break;
    case(3):
      newOrder = CS_FrameNum;
      break;
  }
 
  //Test if the sort is already done on this column
  if(newOrder == columnSort)
  {
    //Toggle the sort reverse option
    sortReverse = !sortReverse;
  }
  else
  {
    //Assign the new column sort
    columnSort  = newOrder;
    sortReverse = false;
  }

  //Re-sort the list
  SortList();
}
