
#ifndef __GLI_SHADERS_H_
#define __GLI_SHADERS_H_

#include "wx/listctrl.h"

typedef unsigned int uint;

////@begin control identifiers
#define ID_GLISHADERS 10019 //DT_TODO: These numbers need updating/moving to be unique?
#define SYMBOL_GLISHADERS_STYLE wxCAPTION|wxRESIZE_BORDER|wxCLOSE_BOX|wxRAISED_BORDER
#define SYMBOL_GLISHADERS_TITLE _("GLI Shaders")
#define SYMBOL_GLISHADERS_IDNAME ID_GLISHADERS
#define SYMBOL_GLISHADERS_SIZE wxSize(400, 300)
#define SYMBOL_GLISHADERS_POSITION wxDefaultPosition
#define ID_CHECKBOX_SHOWSHADER 10020
#define ID_CHECKBOX_BREAKSHADER 10021
#define ID_LISTCTRL 10022
#define ID_BUTTON_OPENSHADER 10023
#define ID_BUTTON_REVERTSHADER 10024
#define ID_BUTTON_REFRESHSHADERS 10025

//DT_TODO: Cleanup comments, class checks

class wxSTEditorFrame;


//Structure to hold data for a single list item
struct ListItemData
{
  ListItemData();
  uint    shaderUID;     // The shader unique ID
  uint    openGLID;      // The OpenGL shader ID
  wxString shaderType;    // The Type of shader
  uint    frameNum;      // The last frame number the shader as used
};

//Enumeration of sorting orders
enum ColumnSortOrder
{
  CS_Undefined =0,       // No sort order
  CS_ShaderUID,          // Sort by shader unique ID
  CS_OpenGLID,           // Sort by OpenGL ID 
  CS_ShaderType,         // Sort by shader type
  CS_FrameNum            // Sort by the last frame the shader was used
};



class GLIShaders: public wxDialog
{    
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  GLIShaders( wxSTEditorFrame *newSte, wxWindowID id = SYMBOL_GLISHADERS_IDNAME, const wxString& caption = SYMBOL_GLISHADERS_TITLE, const wxPoint& pos = SYMBOL_GLISHADERS_POSITION, const wxSize& size = SYMBOL_GLISHADERS_SIZE, long style = SYMBOL_GLISHADERS_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_GLISHADERS_IDNAME, const wxString& caption = SYMBOL_GLISHADERS_TITLE, const wxPoint& pos = SYMBOL_GLISHADERS_POSITION, const wxSize& size = SYMBOL_GLISHADERS_SIZE, long style = SYMBOL_GLISHADERS_STYLE );

  /// Creates the controls and sizers
  void CreateControls();

  //@
  //  Summary:
  //    Update the window position relative to the main window
  //  
  void UpdateWindowPosition();

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  //To set the new shader list data
  void SetShaderListData( const wxString &dataStr);

protected:

  wxSTEditorFrame *ste;                           // Handle to the ste instance
  wxListCtrl      *shaderListCtrl;                // The control listing all the shaders 

  wxPoint         positionOffset;                 // The position of the dialog relative to the main window

  ListItemData    currShaderData;                 // The currently selected shader data

  ColumnSortOrder columnSort;                     // Enum indicating the sort order of the list box display
  bool            sortReverse;                    // Flag indicating if the column sort order is reveresed


  // Handle events
  void OnMove(wxMoveEvent &event);
  void OnCheckboxShowShaderClick( wxCommandEvent& event );
  void OnCheckboxBreakShaderClick( wxCommandEvent& event );

  void OnListctrlItemActivated( wxListEvent& event );
  void OnListctrlItemFocused( wxListEvent& event );
  void OnListctrlColLeftClick( wxListEvent& event );

  void OnButtonOpenShaderClick( wxCommandEvent& event );
  void OnButtonRevertShaderClick( wxCommandEvent& event );
  void OnButtonRefreshShadersClick( wxCommandEvent& event );

  //To add a shader data row
  void AddShaderRow(uint rowNum, const wxString &shaderData);
  
  bool SortList();
  void AddListItem(uint rowNum, const ListItemData &listData);
  bool GetListItem(uint rowNum, ListItemData &listData) const;

  void SetSelectedShader(int rowNum);
  void OpenSelectedShader();
  void RevertSelectedShader();

  //@
  //  Summary:
  //    To set the column that the list is sorted on.
  // 
  //  Parameters:
  //    columnNum - The new column to sort by.
  //
  void SetColumnSort(uint columnNum);

};

#endif // __GLI_SHADERS_H_