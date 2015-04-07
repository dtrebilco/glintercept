
#ifndef __GLI_DEBUG_VARIABLE_GRID_H_
#define __GLI_DEBUG_VARIABLE_GRID_H_

#include "wx/listctrl.h"
#include "wx/grid.h"
#include "GLIShaderDebug.h"
#include "..\\..\\..\\..\\MainLib\\GLDefines.h"

#include <vector>

//@
//  Summary:
//    This provides a grid display of GLSL uniform values and "watch" values.
//  
class GLIDebugVariableGrid: public wxGrid
{    
  DECLARE_EVENT_TABLE()

public:

  // The listing of different display properties
  enum DisplayFlays
  {
    DF_ShowHeaders     = (1 << 0),
    DF_ShowUniforms    = (1 << 1),
    DF_ShowWatchValues = (1 << 2),
  };

  //@
  //  Summary:
  //    Constructor. Sets up grid defaults and creates the display columns.
  //  
  GLIDebugVariableGrid(wxWindow *parent,
                       wxWindowID id,
                       uint displayFlags = DF_ShowHeaders | DF_ShowUniforms | DF_ShowWatchValues, 
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = wxWANTS_CHARS,
                       const wxString& name = wxPanelNameStr);

  //@
  //  Summary:
  //    To set the array of uniform data to display. This overrides all 
  //    existing data. Call RefreshGrid to display the new data.
  //  
  //  Parameters:
  //    newData - The new uniform data to set.
  //
  void SetUniformData(const UniformDataArray &newData);

  //@
  //  Summary:
  //    To refresh the grid display, updating any changes in uniform
  //    or watch values.
  //  
  void RefreshGrid();

  //@
  //  Summary:
  //    This method handles the dropping of text from a drag and drop event.
  //  
  //  Parameters:
  //    newWatchName - The new value to be added to the watch values array.
  //
  //  Returns:
  //    On successful add, true is returned. Else false is returned.
  //
  bool OnDnDText(const wxString &newWatchName);

protected:

  //List of the different types of data that can be stored
  enum RowType
  {
    RT_Header =0,   //Header value
    RT_WatchValue,  //Watch variable
    RT_Uniform,     //Uniform value
    RT_UniformArray,//Uniform array value
  };

  //Storage of the row data
  struct RowTypeData
  {
    inline RowTypeData(RowType newType, uint newIndex);

    RowType type;          //The type value of the row
    uint    indexValue;    //The index value of the element (if any)
  };
  typedef std::vector<RowTypeData> RowTypeDataArray;

  //Structure to hold data about the uniforms in the program
  struct UniformRowData : public UniformData
  {
    inline UniformRowData(); 

    uint arrayCount;       //The array count of the uniform
    bool isExpanded;       //If the array is visually expanded or not 
  };
  typedef std::vector<UniformRowData> UniformRowDataArray;

  uint                 gridFlags;                 // The flags that indicate what is available on this grid

  RowTypeDataArray     rowTypeDataArray;          //The array of different row types
  UniformDataArray     watchValuesArray;          //The array of values that are currently being watched
  UniformRowDataArray  uniformDataArray;          //The display uniform row data

  uint                 internalCellEditCounter;   //A counter that indicates when cells are being edited internally


  //@
  //  Summary:
  //    This method is to be called to flag when cell grid data is being 
  //    changed internally, so cell change callbacks do not occur.
  //  
  //  Parameters:
  //    flag - Flag indicating if this is data change start or end.
  //
  inline void SetInternalEditMode(bool flag);

  //@
  //  Summary:
  //    Callback events to handle changes on the grid.
  //  
  void OnKeyDown(wxKeyEvent &keyEvent);
  void OnCellSelect(wxGridEvent &gridEvent);
  void OnCellLeftClick(wxGridEvent &gridEvent);
  void OnCellDLeftClick(wxGridEvent &gridEvent);
  void OnCellChange(wxGridEvent &gridEvent);

  //@
  //  Summary:
  //    To toggle if a specified row shoudl be displayed expanded or 
  //    contracted. (eg. arrays of values)
  //  
  //  Parameters:
  //    currRow - The row to expand/contract.
  //
  //  Returns:
  //    The the indicated row can expand/contract, true is returned
  //    and the display is updated. Else false is returned.
  //
  bool ToggleRowExpansion(int currRow);

  //@
  //  Summary:
  //    To add all the "watch values" to the current grid.
  //  
  void RefreshWatchValues();

  //@
  //  Summary:
  //    To add all the uniform values to the current grid.
  //  
  void RefreshUniformValues();

  //@
  //  Summary:
  //    To add the row to the grid of the specified type.
  //  
  //  Parameters:
  //    addType - The type of row to add.
  //
  //    typeIndex - The index to associate with the row.
  //
  //  Returns:
  //    The index in the grid of the added row is returned.
  //
  int AddRowType(RowType addType, uint typeIndex);

  //@
  //  Summary:
  //    To add a single uniform value to the current grid.
  //  
  //  Parameters:
  //    uniformValue - The uniform data to add.
  //
  //    uniformOffset - The uniform number.
  //
  void AddUniformDisplay(const UniformRowData &uniformValue, uint uniformOffset);

  //@
  //  Summary:
  //    To add a single watch value to the current grid.
  //  
  //  Parameters:
  //    watchValue - The watch data to add.
  //
  //    watchOffset - The watch number.
  //
  void AddWatchDisplay(const UniformData &watchValue, uint watchOffset);

  //@
  //  Summary:
  //    To set the uniform/watch data in a grid row.
  //  
  //  Parameters:
  //    rowIndex - The row to set the data in.
  //
  //    glType   - The type of the data.
  //
  //    intCount - The number of integer values.
  //
  //    intData  - The integer data to add.
  //
  //    floatCount - The number of float values.
  //
  //    floatData  - The float data to add.
  //
  void SetItemValue(int rowIndex, GLenum glType, unsigned int intCount, const int *intData);
  void SetItemValue(int rowIndex, GLenum glType, unsigned int floatCount, const float *floatData);

  //@
  //  Summary:
  //    To convert a GLSL type to a string value.
  //  
  //  Parameters:
  //    type - The type to convert.
  //
  //  Returns:
  //     The string value of the type is returned.
  //
  wxString GetGLSLStringType(GLenum type);

  //@
  //  Summary:
  //    To convert a string to a GLSL type.
  //  
  //  Parameters:
  //    testString - The string to convert.
  //
  //    retType    - The return type on success.
  //
  //    retIsFloat - If the return type is a float type.
  //
  //  Returns:
  //    If the data could be retrieved, true is returned. 
  //    Else false is returned.
  //
  bool GetGLSLType(const wxString &testString, GLenum &retType, bool &retIsFloat);

  //@
  //  Summary:
  //    To add a GLSL type dropdown selection box in the specified cell.
  //  
  //  Parameters:
  //    row    - The row of the cell to add the box.
  //
  //    column - The column of the cell to add the box.
  //
  //    GLSLType - The GLSL type to set as the default value.
  //
  void SetTypeSelectionBox(int row, int column, GLenum GLSLType);

};

///////////////////////////////////////////////////////////////////////////////
//
inline GLIDebugVariableGrid::RowTypeData::RowTypeData(RowType newType, uint newIndex):
type(newType),
indexValue(newIndex) 
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline GLIDebugVariableGrid::UniformRowData::UniformRowData():
arrayCount(1),
isExpanded(false) 
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline void GLIDebugVariableGrid::SetInternalEditMode(bool flag)
{
  //Update the edit flag counter
  if(flag)
  {
    internalCellEditCounter++;
  }
  else if(internalCellEditCounter > 0)
  {
    internalCellEditCounter--;
  }
}


#endif // __GLI_DEBUG_VARIABLE_GRID_H_

