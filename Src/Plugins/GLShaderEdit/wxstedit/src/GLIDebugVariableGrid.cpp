
#include <wx/wx.h>
#include <wx/renderer.h>
#include <wx/clipbrd.h>
#include <wx/dnd.h>

#include "GLIDebugVariableGrid.h"
#include "..\\..\\..\\..\\Common\\MiscUtils.h"

//The grey colour used in rendering the grid
const wxColour gridGrey(225,224,229);

//The number of pixels to shift the grid display of item values over
#define CELL_RENDER_OFFSET 10

//The maximum number of values that can be watched at once
#define MAX_NUM_WATCH_VALUES 10

//The column indices of the data stored
#define NAME_COLUMN_INDEX  0
#define VALUE_COLUMN_INDEX 1
#define TYPE_COLUMN_INDEX  2

//Array of the different GLSL types
const wxString GLSLTypesStr[] =
{
  _T("float"),
  _T("vec2"),
  _T("vec3"),
  _T("vec4"),

  _T("int"),
  _T("ivec2"),
  _T("ivec3"),
  _T("ivec4"),

  _T("bool"),
  _T("bvec2"),
  _T("bvec3"),
  _T("bvec4"),

  _T("mat2"),
  _T("mat3"),
  _T("mat4"),

  _T("mat2x3"),
  _T("mat2x4"),

  _T("mat3x2"),
  _T("mat3x4"),

  _T("mat4x2"),
  _T("mat4x3"),

  _T("sampler1D"),
  _T("sampler2D"),
  _T("sampler3D"),
  _T("samplerCUBE"),

  _T("sampler1DShadow"),
  _T("sampler2DShadow"),
  _T("sampler2DRect"),
  _T("sampler2DRectShadow"),

};

//Array of types mapped to the OpenGL value
struct GLSLTypeMap
{
  GLSLTypeMap(GLenum type, bool isTypeFloat):
  GLSLType(type),
  isFloat(isTypeFloat)   
  {
  }

  GLenum GLSLType;  //The GLSL type
  bool   isFloat;   //If the type is a float type
};

const GLSLTypeMap GLSLTypesEnum[WXSIZEOF(GLSLTypesStr)] =
{
  GLSLTypeMap(GL_FLOAT,true),
  GLSLTypeMap(GL_FLOAT_VEC2,true),
  GLSLTypeMap(GL_FLOAT_VEC3,true),
  GLSLTypeMap(GL_FLOAT_VEC4,true),

  GLSLTypeMap(GL_INT,false),
  GLSLTypeMap(GL_INT_VEC2,false),
  GLSLTypeMap(GL_INT_VEC3,false),
  GLSLTypeMap(GL_INT_VEC4,false),

  GLSLTypeMap(GL_BOOL,false),
  GLSLTypeMap(GL_BOOL_VEC2,false),
  GLSLTypeMap(GL_BOOL_VEC3,false),
  GLSLTypeMap(GL_BOOL_VEC4,false),

  GLSLTypeMap(GL_FLOAT_MAT2,true),
  GLSLTypeMap(GL_FLOAT_MAT3,true),
  GLSLTypeMap(GL_FLOAT_MAT4,true),

  GLSLTypeMap(GL_FLOAT_MAT2x3,true),
  GLSLTypeMap(GL_FLOAT_MAT2x4,true),

  GLSLTypeMap(GL_FLOAT_MAT3x2,true),
  GLSLTypeMap(GL_FLOAT_MAT3x4,true),

  GLSLTypeMap(GL_FLOAT_MAT4x2,true),
  GLSLTypeMap(GL_FLOAT_MAT4x3,true),

  GLSLTypeMap(GL_SAMPLER_1D,false),
  GLSLTypeMap(GL_SAMPLER_2D,false),
  GLSLTypeMap(GL_SAMPLER_3D,false),
  GLSLTypeMap(GL_SAMPLER_CUBE,false),
  GLSLTypeMap(GL_SAMPLER_1D_SHADOW,false),
  GLSLTypeMap(GL_SAMPLER_2D_SHADOW,false),
  GLSLTypeMap(GL_SAMPLER_2D_RECT_ARB,false),
  GLSLTypeMap(GL_SAMPLER_2D_RECT_SHADOW_ARB,false),
};

CASSERT(WXSIZEOF(GLSLTypesEnum) == WXSIZEOF(GLSLTypesStr), GLSL_Arrays_not_equal);

///////////////////////////////////////////////////////////////////////////////
//
BEGIN_EVENT_TABLE(GLIDebugVariableGrid, wxGrid)

  EVT_KEY_DOWN(GLIDebugVariableGrid::OnKeyDown)

  EVT_GRID_CELL_LEFT_CLICK(GLIDebugVariableGrid::OnCellLeftClick)
  EVT_GRID_CELL_LEFT_DCLICK(GLIDebugVariableGrid::OnCellDLeftClick)
  
  EVT_GRID_SELECT_CELL(GLIDebugVariableGrid::OnCellSelect)
  EVT_GRID_CELL_CHANGE(GLIDebugVariableGrid::OnCellChange)

END_EVENT_TABLE()

//@
//  Summary:
//    This class handles the text drop events (from drag and drop)
//  
class GridDnDText : public wxDropTarget
{
public:

  //@
  //  Summary:
  //    Constructor.
  //  
  //  Parameters:
  //    newOwner - The owning control.
  //
  GridDnDText(GLIDebugVariableGrid *newOwner);

  //@
  //  Summary:
  //    The method that gets called when a drop event occurs.
  //  
  //  Parameters:
  //    x - The x coordinate of the drop.
  //
  //    y - The y coordinate of the drop.
  //
  //    def - The text being sent.
  //
  virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def);

protected:
  GLIDebugVariableGrid *owner;                    //The owning control which gets a callback to add a new watch value

  DECLARE_NO_COPY_CLASS(GridDnDText);
};

///////////////////////////////////////////////////////////////////////////////
//
GridDnDText::GridDnDText(GLIDebugVariableGrid *newOwner):
owner(newOwner)
{
  SetDataObject(new wxTextDataObject);
}

///////////////////////////////////////////////////////////////////////////////
//
wxDragResult GridDnDText::OnData(wxCoord x, wxCoord y, wxDragResult def)
{
  //If there is no data, return now
  if (!GetData() || !owner)
  {
    return wxDragNone;
  }

  //Process the drop
  wxTextDataObject *dobj = (wxTextDataObject *)m_dataObject;
  if(owner->OnDnDText(dobj->GetText()))
  {
    return wxDragCopy;
  }

  return wxDragNone;
}


//@
//  Summary:
//    This class performs grid cell rendering of the string data that is
//    offset by a fixed amount in the x direction.
//  
class OffsetStringGridCellRenderer : public wxGridCellStringRenderer
{
public:

  //@
  //  Summary:
  //    Constructor.
  //  
  //  Parameters:
  //    offset - The offset distance in pixels.
  //
  OffsetStringGridCellRenderer(int offset);

  //@
  //  Summary:
  //    Overriden method to calculate the best render size.
  //  
  virtual wxSize GetBestSize(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, int row, int col);

  //@
  //  Summary:
  //    Overriden method to draw the cell grid with the text offset.
  //  
  virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected);

protected:

  int  offsetX;                                   //The offset value in the x direction
};

///////////////////////////////////////////////////////////////////////////////
//
OffsetStringGridCellRenderer::OffsetStringGridCellRenderer(int newOffset):
offsetX(newOffset)
{

}

///////////////////////////////////////////////////////////////////////////////
//
wxSize OffsetStringGridCellRenderer::GetBestSize(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, int row, int col)
{
  //Add the width of the color block to the render size
  wxSize retSize = wxGridCellStringRenderer::GetBestSize(grid, attr, dc, row, col);
  retSize.SetWidth(retSize.GetWidth() + offsetX); 
  return retSize;
}

///////////////////////////////////////////////////////////////////////////////
//
void OffsetStringGridCellRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected)
{
  //Call super base class to clear the full drawing area
  wxGridCellRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);

  //Call base class with offset rectangle
  wxRect textRect(rect);
  textRect.x     += offsetX;
  textRect.width -= offsetX;
  wxGridCellStringRenderer::Draw(grid, attr, dc, textRect, row, col, isSelected);   
}


//@
//  Summary:
//    This class performs grid cell rendering of a color box next to the 
//    text string.
//  
class ColourGridCellRenderer : public OffsetStringGridCellRenderer
{
public:

  //@
  //  Summary:
  //    Constructor.
  //  
  //  Parameters:
  //    renderColor - The render color to display when rendering the grid cell.
  //
  ColourGridCellRenderer(wxColor renderColor);

  //@
  //  Summary:
  //    Overriden method to draw the cell grid with the color indicator.
  //  
  virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected);

protected:

  wxColor renderColour;                           //The box render colour 
};


///////////////////////////////////////////////////////////////////////////////
//
ColourGridCellRenderer::ColourGridCellRenderer(wxColor newColor):
OffsetStringGridCellRenderer(30),
renderColour(newColor)
{

}


///////////////////////////////////////////////////////////////////////////////
//
void ColourGridCellRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected)
{
  //Call base class to clear the full drawing area and render the string
  OffsetStringGridCellRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);

  //Get the old state
  wxBrush oldBrush = dc.GetBrush();
  wxPen   oldPen   = dc.GetPen();

  //Set the new state
  dc.SetBrush(wxBrush(renderColour));
  dc.SetPen(wxPen(*wxBLACK));

  //Render the color box
  wxRect renderRect = wxRect(rect.x + 5,rect.y + 1, 17, 14).Intersect(rect);
  dc.DrawRectangle(renderRect);

  //Restore the old state
  dc.SetBrush(oldBrush);
  dc.SetPen(oldPen);
}

//@
//  Summary:
//    This class performs grid cell rendering of a expand/contractbox to the 
//    text string.
//  
class ExpandGridCellRenderer : public OffsetStringGridCellRenderer
{
public:

  //@
  //  Summary:
  //    Constructor.
  //  
  //  Parameters:
  //    textOffset - The text offset render value.
  //
  //    isExpand - The flag indicating if the box is expanded or contracted.
  //
  ExpandGridCellRenderer(uint textOffset, bool isExpand);

  //@
  //  Summary:
  //    Overriden method to draw the cell grid with the expand box.
  //  
  virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected);

protected:

  bool isExpand;                                  //Flag indicating if it is a expand or contract box
};

///////////////////////////////////////////////////////////////////////////////
//
ExpandGridCellRenderer::ExpandGridCellRenderer(uint textOffset, bool newIsExpand):
OffsetStringGridCellRenderer(textOffset),
isExpand(newIsExpand)
{

}


///////////////////////////////////////////////////////////////////////////////
//
void ExpandGridCellRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected)
{
  //Call base class to clear the full drawing area and render the string
  OffsetStringGridCellRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);

  //Render the expand control based on the state
  wxRect drawRect(rect.x+1, rect.y+4, 9, 9);
  if(isExpand)
  {
    wxRendererNative::Get().DrawTreeItemButton(&grid, dc, drawRect, wxCONTROL_EXPANDED);
  }
  else
  {
    wxRendererNative::Get().DrawTreeItemButton(&grid, dc, drawRect, 0);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
GLIDebugVariableGrid::GLIDebugVariableGrid(wxWindow *parent,
                                           wxWindowID id,
                                           uint displayFlags,
                                           const wxPoint& pos,
                                           const wxSize& size,
                                           long style,
                                           const wxString& name):
wxGrid(parent, id, pos, size, style, name),
gridFlags(displayFlags),
internalCellEditCounter(0)
{
  //Create a drag target
  SetDropTarget(new GridDnDText(this));

  //Create the grid of 3 columns
  CreateGrid(0, 3, wxGrid::wxGridSelectRows);

  //Set the label values
  SetColLabelValue(NAME_COLUMN_INDEX,  wxT("Name"));
  SetColLabelValue(VALUE_COLUMN_INDEX, wxT("Value"));
  SetColLabelValue(TYPE_COLUMN_INDEX,  wxT("Type"));
  SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_TOP);
  SetColLabelSize(17);

  //Set the column sizes
  SetColSize(NAME_COLUMN_INDEX,  125);
  SetColSize(VALUE_COLUMN_INDEX, 200);
  SetColSize(TYPE_COLUMN_INDEX,  115);

  //Turn off row labels
  SetRowLabelSize(0);

  //Turn off cell overflowing
  SetDefaultCellOverflow(false);

  //Setup default colours
  SetDefaultCellBackgroundColour(*wxWHITE);
  SetDefaultCellTextColour      (*wxBLACK);
  SetGridLineColour       (gridGrey);
  SetLabelBackgroundColour(gridGrey);
  SetLabelTextColour      (*wxBLACK);
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::OnKeyDown(wxKeyEvent &keyEvent)
{
  //If in edit mode, allow all keys through
  if(IsCellEditControlEnabled())
  {
    keyEvent.Skip();
    return;
  }

  //Don't allow the enter key to switch between rows
  if(keyEvent.m_keyCode == WXK_RETURN)
  {
    //Toggle the expandable rows if necessary
    if(ToggleRowExpansion(GetGridCursorRow()))
    {
      return;
    }

    //If the cell is able to be edited, start editing
    if(!IsCurrentCellReadOnly())
    {
      EnableCellEditControl();
    }

    return;
  }

  //Delete watch values on the delete key
  if(keyEvent.m_keyCode == WXK_DELETE)
  {
    int currRow    = GetGridCursorRow();
    int currColumn = GetGridCursorCol();

    //Check that the current row is in range
    if(currRow >= 0 && currRow < rowTypeDataArray.size() &&
       rowTypeDataArray[currRow].type == RT_WatchValue)
    {
      //Can only delete old stored watch values, not in progress editing ones
      uint watchIndex = rowTypeDataArray[currRow].indexValue;
      if(watchIndex < watchValuesArray.size())
      {
        //Delete the watch value from the array
        watchValuesArray.erase(watchValuesArray.begin() + watchIndex);

        //Refresh the grid
        RefreshGrid();

        //Reset the selected cell
        SetCurrentCell(currRow, currColumn);
        return;
      }
    }
  }

  //Handle Ctrl-C to copy the contents of a cell to the clipboard
  if(keyEvent.m_keyCode == 'C' && keyEvent.m_controlDown)
  {
    if (wxTheClipboard->Open())
    {
      //Add the data this is in the current cell
      wxTheClipboard->SetData( new wxTextDataObject(GetCellValue(GetGridCursorRow(), GetGridCursorCol())) );
      wxTheClipboard->Close();
    }
  }

  //Process all other keys
  keyEvent.Skip();
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::OnCellLeftClick(wxGridEvent &gridEvent)
{
  //If the current row is expandable
  int currRow    = gridEvent.GetRow();
  int currColumn = gridEvent.GetCol();

  //Get the click area in cell space
  wxPoint clickPoint = gridEvent.GetPosition();
  wxRect  cellArea   = CellToRect(currRow, currColumn);

  //If clicking on (or near) the +/- 
  if(currColumn == NAME_COLUMN_INDEX && (clickPoint.x - cellArea.x) < CELL_RENDER_OFFSET)
  {
    if(ToggleRowExpansion(currRow))
    {
      return;
    }
  }
  
  //Allow other windows to process the event
  gridEvent.Skip(); 
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::OnCellDLeftClick(wxGridEvent &gridEvent)
{
  //If the current row is expandable
  int currRow    = gridEvent.GetRow();
  int currColumn = gridEvent.GetCol();

  //If double clicking on on a expandable row
  if(currColumn == NAME_COLUMN_INDEX)
  {
    if(ToggleRowExpansion(currRow))
    {
      return;
    }
  }
  
  //Allow other windows to process the event
  gridEvent.Skip(); 
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::OnCellSelect(wxGridEvent &gridEvent)
{
  //Select the row when a cell has been selected
  SelectRow(gridEvent.GetRow());

  //Allow other processing
  gridEvent.Skip();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::OnCellChange(wxGridEvent &gridEvent)
{
  int currRow    = gridEvent.GetRow();
  int currColumn = gridEvent.GetCol();

  //Get if the cell data is a watch value 
  // (and not currently doing a manual cell update)
  if(internalCellEditCounter == 0 &&
     currRow >= 0 && currRow < rowTypeDataArray.size() &&
     rowTypeDataArray[currRow].type == RT_WatchValue)
  {
    UniformData newWatchData;

    //Get the string value
    newWatchData.name = GetCellValue(currRow, NAME_COLUMN_INDEX);

    //Get the type data
    newWatchData.type = GL_FLOAT;
    newWatchData.isFloatType = true;
    GetGLSLType(GetCellValue(currRow, TYPE_COLUMN_INDEX), newWatchData.type, newWatchData.isFloatType); 

    //Test if the watch value index is for a new watch or, editing an old one
    uint watchIndex = rowTypeDataArray[currRow].indexValue;
    if(watchIndex >= watchValuesArray.size())
    {
      //Add the new watch if there is room (should always be) and the name has been set
      if(currColumn == NAME_COLUMN_INDEX && 
         newWatchData.name.length() > 0 &&
         watchValuesArray.size() < MAX_NUM_WATCH_VALUES)
      {
        watchValuesArray.push_back(newWatchData);

        //Refresh the grid
        RefreshGrid();

        //Reset the selected cell
        SetCurrentCell(currRow, currColumn);
      }
    }
    else
    {
      //If the name is valid
      if(newWatchData.name.length() > 0)
      {
        //Override the watch data
        watchValuesArray[watchIndex] = newWatchData;
      }
      else
      {
        //Delete the watch value from the array
        watchValuesArray.erase(watchValuesArray.begin() + watchIndex);

        //Refresh the grid
        RefreshGrid();

        //Reset the selected cell
        SetCurrentCell(currRow, currColumn);
      }
    }

    return;
  }

  //The event was not processed
  gridEvent.Skip();
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIDebugVariableGrid::OnDnDText(const wxString &newWatchName)
{
  //If currently editing, return now
  if(IsCellEditControlEnabled())
  {
    return false;
  }

  //If showing watch values or there is already too many watch variables
  if(!(gridFlags & DF_ShowWatchValues) || 
     watchValuesArray.size() >= MAX_NUM_WATCH_VALUES)
  {
    return false;
  }

  //Add the value
  UniformData newWatchData;
  newWatchData.name = newWatchName;
  newWatchData.type = GL_FLOAT;
  newWatchData.isFloatType = true;

  watchValuesArray.push_back(newWatchData);

  //Refresh the display
  int currRow    = GetGridCursorRow();
  int currColumn = GetGridCursorCol();

  //Update the grid
  RefreshGrid();
  
  //Reset the selected cell
  SetCurrentCell(currRow, currColumn);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIDebugVariableGrid::ToggleRowExpansion(int currRow)
{
  //Test if the passed row is within range
  if(currRow >= 0 && currRow < rowTypeDataArray.size())
  {
    //If the value is a uniform that has more that one value
    if(rowTypeDataArray[currRow].type == RT_Uniform)
    {
      //Get the uniform index
      uint uniformIndex = rowTypeDataArray[currRow].indexValue;
      if(uniformIndex < uniformDataArray.size() &&
         uniformDataArray[uniformIndex].arrayCount > 1)
      {
        //Toggle the array show value if necessary
        uniformDataArray[uniformIndex].isExpanded = !uniformDataArray[uniformIndex].isExpanded;

        //Refresh the grid
        RefreshGrid();

        //Reset the selected row
        SetCurrentCell(currRow, NAME_COLUMN_INDEX);
        return true;
      }
    }
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::SetUniformData(const UniformDataArray &newData)
{
  //Clear existing array
  uniformDataArray.clear();

  //Loop for all values
  for(uint i=0; i<newData.size(); i++)
  {
    UniformRowData newUniformRowData;

    //Assign the uniform data
    newUniformRowData.name = newData[i].name;
    newUniformRowData.type = newData[i].type;
    newUniformRowData.numTypeElements  = newData[i].numTypeElements;
    newUniformRowData.isFloatType      = newData[i].isFloatType;

    newUniformRowData.intUniformData   = newData[i].intUniformData;
    newUniformRowData.floatUniformData = newData[i].floatUniformData;

    //Calculate the array size
    if(newUniformRowData.isFloatType)
    {
      //Ensure the array values are correct
      if(newUniformRowData.floatUniformData.size() % newUniformRowData.numTypeElements != 0)
      {
        newUniformRowData.arrayCount = 0;
      }
      else
      {
        newUniformRowData.arrayCount = newUniformRowData.floatUniformData.size() / newUniformRowData.numTypeElements; 
      }
    }
    else
    {
      //Ensure the array values are correct
      if(newUniformRowData.intUniformData.size() % newUniformRowData.numTypeElements != 0)
      {
        newUniformRowData.arrayCount = 0;
      }
      else
      {
        newUniformRowData.arrayCount = newUniformRowData.intUniformData.size() / newUniformRowData.numTypeElements; 
      }
    }

    //Add to the array
    uniformDataArray.push_back(newUniformRowData);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::RefreshGrid()
{
  //Flag that we are updating the cells internally (so update callbacks do not get called)
  SetInternalEditMode(true);

  //Begin batch to not redraw during changes
  BeginBatch();

  //Clear any existing data
  ClearGrid();
  rowTypeDataArray.clear();

  //Delete all rows
  if(GetNumberRows() > 0)
  {
    DeleteRows(0, GetNumberRows());
  }

  //Refresh the watch values
  RefreshWatchValues();

  //Refresh the uniform data
  RefreshUniformValues();

  //End Batch to update the changes to the grid 
  EndBatch();

  //Reset the cell update counter
  SetInternalEditMode(false);  
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::RefreshWatchValues()
{
  // If not showing/adding watch values, return now
  if(!(gridFlags & DF_ShowWatchValues))
  {
    return;
  }

  //Add the header
  if(gridFlags & DF_ShowHeaders)
  {
    int headerIndex = AddRowType(RT_Header, 0);
    SetCellValue(headerIndex, NAME_COLUMN_INDEX, wxT("Watch values"));
  }

  //Loop and add all the watch values
  for(uint i=0; i<watchValuesArray.size(); i++)
  {
    AddWatchDisplay(watchValuesArray[i], i);
  }

  //Add a empty watch value to add a new value
  if(watchValuesArray.size() < MAX_NUM_WATCH_VALUES)
  {
    int newIndex = AddRowType(RT_WatchValue, watchValuesArray.size());  
    SetTypeSelectionBox(newIndex, TYPE_COLUMN_INDEX, GL_FLOAT);

    //Select the starting cell
    SetCurrentCell(newIndex, NAME_COLUMN_INDEX);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::RefreshUniformValues()
{
  // If not showing/adding uniform values, return now
  if(!(gridFlags & DF_ShowUniforms))
  {
    return;
  }

  //Add the header
  if(gridFlags & DF_ShowHeaders)
  {
    int headerIndex = AddRowType(RT_Header, 1);  
    SetCellValue(headerIndex, NAME_COLUMN_INDEX, wxT("Uniforms"));
  }

  //Loop for all uniforms and add values of the appropiate type
  for(uint i=0; i<uniformDataArray.size(); i++)
  { 
    AddUniformDisplay(uniformDataArray[i], i);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
wxString GLIDebugVariableGrid::GetGLSLStringType(GLenum type)
{
  //Loop for all enum types
  for(uint i=0; i<WXSIZEOF(GLSLTypesEnum); i++)
  {
    //If the type is found, return the string
    if(GLSLTypesEnum[i].GLSLType == type)
    {
      return GLSLTypesStr[i];
    }
  }

  //Else just return an unknown string
  return wxT("<unknown>");
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIDebugVariableGrid::GetGLSLType(const wxString &testString, GLenum &retType, bool &retIsFloat)
{
  //Loop for all enum types
  for(uint i=0; i<WXSIZEOF(GLSLTypesStr); i++)
  {
    //If the type is found, return the string
    if(GLSLTypesStr[i] == testString)
    {
      retType    = GLSLTypesEnum[i].GLSLType;
      retIsFloat = GLSLTypesEnum[i].isFloat;
      return true;
    }
  }

  return false;
}



///////////////////////////////////////////////////////////////////////////////
//
int GLIDebugVariableGrid::AddRowType(RowType addType, uint typeIndex)
{
  //Add the row and get the index
  AppendRows(1);

  //Add to the type array
  rowTypeDataArray.push_back(RowTypeData(addType, typeIndex));

  //Check that the array sizes match
  wxASSERT(rowTypeDataArray.size() == GetNumberRows());

  //Set the default properties for the different types
  int addIndex = GetNumberRows() - 1;
  switch(addType)
  {
    case(RT_Header):
      SetCellSize(addIndex, NAME_COLUMN_INDEX, 1, 3);
      SetReadOnly(addIndex, NAME_COLUMN_INDEX);
      SetCellBackgroundColour(addIndex, NAME_COLUMN_INDEX, gridGrey);
      SetCellFont(addIndex, NAME_COLUMN_INDEX, GetLabelFont());
      break;

    case(RT_WatchValue):
      SetCellRenderer(addIndex, NAME_COLUMN_INDEX, new OffsetStringGridCellRenderer(CELL_RENDER_OFFSET));  
      SetReadOnly(addIndex, VALUE_COLUMN_INDEX);
      break;

    case(RT_Uniform):
      SetReadOnly(addIndex, NAME_COLUMN_INDEX);
      SetReadOnly(addIndex, VALUE_COLUMN_INDEX);
      SetReadOnly(addIndex, TYPE_COLUMN_INDEX);

      SetCellRenderer(addIndex, NAME_COLUMN_INDEX, new OffsetStringGridCellRenderer(CELL_RENDER_OFFSET));  
      break;

    case(RT_UniformArray):
      SetReadOnly(addIndex, NAME_COLUMN_INDEX);
      SetReadOnly(addIndex, VALUE_COLUMN_INDEX);
      SetReadOnly(addIndex, TYPE_COLUMN_INDEX);

      SetCellRenderer(addIndex, NAME_COLUMN_INDEX, new OffsetStringGridCellRenderer(CELL_RENDER_OFFSET*2));  
      break;

  };


  //Return the index
  return addIndex;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::SetTypeSelectionBox(int row, int column, GLenum GLSLType)
{
  //Find the index to set
  uint assignIndex =0;
  for(uint i=0; i<WXSIZEOF(GLSLTypesEnum); i++)
  {
    //If the type is found, return the string
    if(GLSLTypesEnum[i].GLSLType == GLSLType)
    {
      assignIndex = i;
    }
  }

  //Add a combo box to select among the different GLSL types
  SetCellEditor(row, column, new wxGridCellChoiceEditor(WXSIZEOF(GLSLTypesStr), GLSLTypesStr));
  SetCellValue(row, column, GLSLTypesStr[assignIndex]);
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::AddUniformDisplay(const UniformRowData &uniformValue, uint uniformOffset)
{
  //Create the initial static item  
  int uniformRowIndex = AddRowType(RT_Uniform, uniformOffset);  

  //Assign the name
  SetCellValue(uniformRowIndex, NAME_COLUMN_INDEX, uniformValue.name);

  //Assign the type
  SetCellValue(uniformRowIndex, TYPE_COLUMN_INDEX, GetGLSLStringType(uniformValue.type));

  //Get the array size
  if(uniformValue.arrayCount == 0)
  {
    SetCellValue(uniformRowIndex, VALUE_COLUMN_INDEX, wxT("<empty>"));
    return;
  }

  //If there is more than one array item, nest them
  if(uniformValue.arrayCount > 1)
  {
    //Update the main label name
    wxString labelName;
    labelName.Printf("%s[%u]", uniformValue.name.c_str(), uniformValue.arrayCount); 
    SetCellValue(uniformRowIndex, NAME_COLUMN_INDEX, labelName);
    SetCellRenderer(uniformRowIndex, NAME_COLUMN_INDEX, new ExpandGridCellRenderer(CELL_RENDER_OFFSET, uniformValue.isExpanded));
    SetCellValue(uniformRowIndex, VALUE_COLUMN_INDEX, wxT("...."));

    //If the array is expanded
    if(uniformValue.isExpanded)
    {
      //Loop an add each item as a sub item
      for(uint i=0; i<uniformValue.arrayCount; i++)
      {
        int arrayRowIndex = AddRowType(RT_UniformArray, i);  

        //Set the name
        labelName.Printf("%s[%u]", uniformValue.name.c_str(), i); 
        SetCellValue(arrayRowIndex, NAME_COLUMN_INDEX, labelName);
        SetCellValue(arrayRowIndex, TYPE_COLUMN_INDEX, GetGLSLStringType(uniformValue.type));

        //Add the description
        if(uniformValue.isFloatType)
        {
          SetItemValue(arrayRowIndex, uniformValue.type, uniformValue.numTypeElements, &uniformValue.floatUniformData[i*uniformValue.numTypeElements]);
        }
        else
        {
          SetItemValue(arrayRowIndex, uniformValue.type, uniformValue.numTypeElements, &uniformValue.intUniformData[i*uniformValue.numTypeElements]);
        }
      }
    }
  }
  else
  {
    //Add the single value
    if(uniformValue.isFloatType)
    {
      SetItemValue(uniformRowIndex, uniformValue.type, uniformValue.floatUniformData.size(), &uniformValue.floatUniformData[0]);
    }
    else
    {
      SetItemValue(uniformRowIndex, uniformValue.type, uniformValue.intUniformData.size(), &uniformValue.intUniformData[0]);
    }
  }
 
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::AddWatchDisplay(const UniformData &watchValue, uint watchOffset)
{
  //Create the initial static item  
  int watchRowIndex = AddRowType(RT_WatchValue, watchOffset);  

  //Assign the name
  SetCellValue(watchRowIndex, NAME_COLUMN_INDEX, watchValue.name);

  //Assign the type from the drop down
  SetTypeSelectionBox(watchRowIndex, TYPE_COLUMN_INDEX, watchValue.type);

  //Check the data
  if(watchValue.isFloatType)
  {
    //Get if the data is valid
    if(watchValue.numTypeElements == 0 ||
       watchValue.floatUniformData.size() != watchValue.numTypeElements)
    {
      SetCellValue(watchRowIndex, VALUE_COLUMN_INDEX, wxT("<unknown>"));
      return;
    }

    //Add the single value
    SetItemValue(watchRowIndex, watchValue.type, watchValue.floatUniformData.size(), &watchValue.floatUniformData[0]);
  }
  else
  {
    //Get if the data is valid
    if(watchValue.numTypeElements == 0 ||
       watchValue.intUniformData.size() != watchValue.numTypeElements)
    {
      SetCellValue(watchRowIndex, VALUE_COLUMN_INDEX, wxT("<unknown>"));
      return;
    }

    //Add the single value
    SetItemValue(watchRowIndex, watchValue.type, watchValue.intUniformData.size(), &watchValue.intUniformData[0]);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
unsigned char FloatClamp(float value)
{
  //Get the float value in the 0..255 range
  int retValue = (int)(value * 255.0f);

  //Clamp the float value between 0..255
  if(retValue < 0)
  {
    return 0;
  }
  if(retValue > 255)
  {
    return 255;
  }

  return (unsigned char)retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::SetItemValue(int rowIndex, GLenum glType, unsigned int intCount, const int *intData)
{
  wxString setText;

  //Determine if it is a boolean type
  bool isBool = false;
  if(glType == GL_BOOL ||
     glType == GL_BOOL_VEC2 ||
     glType == GL_BOOL_VEC3 ||
     glType == GL_BOOL_VEC4 )
  {
    isBool = true;
  }

  //Add an opening brace
  if(intCount > 1)
  {
    setText += "("; 
  }

  //Copy the data
  for(unsigned int i=0; i<intCount; i++)
  {
    wxString newStr;

    //If it is a boolean value
    if(isBool)
    {
      if(intData[i] > 0)
      {
        newStr = "true";
      }
      else
      {
        newStr = "false";
      }
    }
    else
    {
      newStr.Printf(wxT("%d"), intData[i]);
    }
    
    //Append to the string
    setText += newStr;

    //Add a comma if necessary
    if(i != intCount-1)
    {
      setText += ", "; 
    }
  }

  //Add a closing brace
  if(intCount > 1)
  {
    setText += ")"; 
  }

  //Assign the data to the row
  SetCellValue(rowIndex, VALUE_COLUMN_INDEX, setText);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIDebugVariableGrid::SetItemValue(int rowIndex, GLenum glType, unsigned int floatCount, const float *floatData)
{
  wxString setText;

  //Determine if the color block is rendered
  bool renderColor = false;
  if(floatCount == 3 ||
     floatCount == 4)
  {
    renderColor = true;
  }

  //Determine if it is a matrix type
  bool isMatrix = false;
  uint numMatrixElemPerRow = 1;
  if(glType == GL_FLOAT_MAT2 || 
     glType == GL_FLOAT_MAT3 ||
     glType == GL_FLOAT_MAT4 ||
     glType == GL_FLOAT_MAT2x3 ||
     glType == GL_FLOAT_MAT2x4 ||
     glType == GL_FLOAT_MAT3x2 ||
     glType == GL_FLOAT_MAT3x4 ||
     glType == GL_FLOAT_MAT4x2 ||
     glType == GL_FLOAT_MAT4x3)
  {
    isMatrix = true;
    renderColor = false;

    //Get the number of matrix rows
    switch(glType)
    {
      case(GL_FLOAT_MAT2):
      case(GL_FLOAT_MAT2x3):
      case(GL_FLOAT_MAT2x4):
        numMatrixElemPerRow = 2;
        break;
      case(GL_FLOAT_MAT3):
      case(GL_FLOAT_MAT3x2):
      case(GL_FLOAT_MAT3x4):
        numMatrixElemPerRow = 3;
        break;
      case(GL_FLOAT_MAT4):
      case(GL_FLOAT_MAT4x2):
      case(GL_FLOAT_MAT4x3):
        numMatrixElemPerRow = 4;
        break;
    };

    switch(glType)
    {
      case(GL_FLOAT_MAT2):
      case(GL_FLOAT_MAT3x2):
      case(GL_FLOAT_MAT4x2):
        SetRowSize(rowIndex, GetRowSize(rowIndex) * 2);
        break;
      case(GL_FLOAT_MAT3):
      case(GL_FLOAT_MAT2x3):
      case(GL_FLOAT_MAT4x3):

        SetRowSize(rowIndex, GetRowSize(rowIndex) * 3);
        break;
      case(GL_FLOAT_MAT4):
      case(GL_FLOAT_MAT2x4):
      case(GL_FLOAT_MAT3x4):
        SetRowSize(rowIndex, GetRowSize(rowIndex) * 4);
        break;
    };
  }

  //Add an opening brace
  if(floatCount > 1)
  {
    setText += "("; 
  }

  //Copy the data
  for(unsigned int i=0; i<floatCount; i++)
  {
    wxString newStr;
    newStr.Printf(wxT("%g"), floatData[i]); // TODO: Transpose
    
    //Ensure there is a trailing .0
    if(newStr.Find('.') < 0 &&
       newStr.Find('e') < 0)
    {
      newStr += ".0";
    }

    //Append to the string
    setText += newStr;

    //Add a comma if necessary
    if(i != floatCount-1)
    {
      setText += ", "; 
    }

    //If this is the end of a matrix row, add a newline
    if(isMatrix && 
       (i != floatCount-1) && 
       ((i+1) % numMatrixElemPerRow == 0))
    {
      setText += "\n ";
    }
  }

  //Add a closing brace
  if(floatCount > 1)
  {
    setText += ")"; 
  }


  //If rendering a color
  if(renderColor)
  {
    //Assign a color box render to the value
    wxColor assignColour = wxColor(FloatClamp(floatData[0]), 
                                   FloatClamp(floatData[1]),
                                   FloatClamp(floatData[2]));

    SetCellRenderer(rowIndex, VALUE_COLUMN_INDEX, new ColourGridCellRenderer(assignColour));
  }

  //Assign the data to the row
  SetCellValue(rowIndex, VALUE_COLUMN_INDEX, setText);
}
