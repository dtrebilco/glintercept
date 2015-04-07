#ifndef _GLISHADERDEBUG_H_
#define _GLISHADERDEBUG_H_

#include "wx/frame.h"
#include "wx/statusbr.h"
#include "wx/toolbar.h"
#include "wx/splitter.h"
#include "wx/notebook.h"

#include <vector>

typedef unsigned int uint;

#define ID_SHADERDEBUG 10000
#define SYMBOL_GLISHADERDEBUG_STYLE wxSYSTEM_MENU|wxRESIZE_BORDER|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxCLOSE_BOX|wxCAPTION|wxCLIP_CHILDREN
#define SYMBOL_GLISHADERDEBUG_TITLE _("Shader Debug")
#define SYMBOL_GLISHADERDEBUG_IDNAME ID_SHADERDEBUG
#define SYMBOL_GLISHADERDEBUG_SIZE wxSize(720, 735)
#define SYMBOL_GLISHADERDEBUG_MINSIZE wxSize(385, 310)
#define SYMBOL_GLISHADERDEBUG_POSITION wxDefaultPosition
#define ID_STATUSBAR 10001
#define ID_TOOLBAR 10003
#define ID_TOOL9 10045
#define ID_TOOL10 10046
#define ID_TEXTCTRL2 10047
#define ID_TEXTCTRL3 10048
#define ID_SPLITTERWINDOW3 10049
#define ID_PANEL7 10050
#define ID_NOTEBOOK3 10052
#define ID_PANEL8 10051
#define ID_TEXTCTRL4 10053
#define ID_BUTTON6 10054
#define ID_TEXTCTRL5 10055
#define ID_BUTTON_DEBUGPIXEL 10056

#define ID_TOOL_SELECT 10058
#define ID_TOOL_ZOOM   10059

#define ID_TOOL_RED   10060
#define ID_TOOL_GREEN 10061
#define ID_TOOL_BLUE  10062
#define ID_TOOL_ALPHA 10063
#define ID_TOOL_RGB   10067 

#define ID_TOOL_PRE  10064
#define ID_TOOL_POST 10065
#define ID_TOOL_DIFF 10066

#define ID_UNIFORMGRID 10067
#define ID_WATCHGRID 10068

#define ID_XPOS_TEXTCTRL 10069
#define ID_YPOS_TEXTCTRL 10070

#define ID_TOOL_INTSCALE 10071
#define ID_TOOL_NORMALIZE 10072
#define ID_TOOL_NEXTRENDER 10073


class wxSTEditorFrame;
class wxToolBar;
class GLIDebugVariableGrid;
class GLIBitmapView;
class GLIBitmapNotebook;

//The states the debugger can be in
enum GLIDebugState
{
  DBS_None,   //No debugging state
  DBS_Init,   //Debugging is init, but no pixel/vertex has been selected
  DBS_Pixel,  //A debugging pixel has been selected

};

//@
//  Summary:
//    This structure contains the data saved from the frame buffer.
//  
struct FrameBufferData
{
  //@
  //  Summary:
  //    The default constructor.
  //  
  FrameBufferData();

  int  bufferType;           // The type of buffer the data comes from (eg. the RGBA or depth component)
  int  drawBuffer;           // If a specific draw buffer is used (eg. multiple color attachments)

  uint bufferWidth;          // The width of the buffer
  uint bufferHeight;         // The height of the buffer
  uint numPixelValues;       // The number of float values in a single pixel 

  std::vector<float> preBuffer;   // The pre-render buffer
  std::vector<float> postBuffer;  // The post-render buffer
};
typedef std::vector<FrameBufferData> FrameBufferDataArray;

//Structure to hold data about the uniforms in the program
struct UniformData
{
  UniformData(); 

  wxString name;          // The name of the uniform
  uint type;              // The type of the uniform
  uint numTypeElements;   // The number of elements in the type 
  bool isFloatType;       // Flag indicating a float or int based type 

  std::vector<int>   intUniformData;   // The float uniform data (if a float type)
  std::vector<float> floatUniformData; // The int uniform data   (if a int type)
};
typedef std::vector<UniformData> UniformDataArray;




class GLIShaderDebug: public wxFrame
{    
  DECLARE_CLASS( GLIShaderDebug )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  GLIShaderDebug( wxSTEditorFrame *newSte, wxWindowID id = SYMBOL_GLISHADERDEBUG_IDNAME, const wxString& caption = SYMBOL_GLISHADERDEBUG_TITLE, const wxPoint& pos = SYMBOL_GLISHADERDEBUG_POSITION, const wxSize& size = SYMBOL_GLISHADERDEBUG_SIZE, long style = SYMBOL_GLISHADERDEBUG_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_GLISHADERDEBUG_IDNAME, const wxString& caption = SYMBOL_GLISHADERDEBUG_TITLE, const wxPoint& pos = SYMBOL_GLISHADERDEBUG_POSITION, const wxSize& size = SYMBOL_GLISHADERDEBUG_SIZE, long style = SYMBOL_GLISHADERDEBUG_STYLE );

  /// Creates the controls and sizers
  void CreateControls();


  //@
  //  Summary:
  //    To start debugging mode with the provided network data.
  //  
  //  Parameters:
  //    bufferSize  - The size of the provided buffer.
  //
  //    buffer      - The network buffer.
  //
  //  Returns:
  //    If a debug mode switch was successful, true is returned. Else false
  //    is returned.
  //
  bool DebugBegin(uint bufferSize, void *buffer);

  //@
  //  Summary:
  //    To end the debug mode.
  //  
  //  Returns:
  //    If debug mode has exited, true is returned. Else false
  //    is returned.
  //
  bool DebugEnd();

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  // Set the debug pixel location
  void SetDebugPixel(const wxPoint &newPixel);

  // Update the current window title
  void UpdateWindowTitle();

  // Update the current selected color data
  void UpdateSelectedColor();
  
protected:

  wxSTEditorFrame *ste;                           // Handle to the ste instance

  GLIDebugState debugState;                       // The state of the debugger
  
  uint          debugUID;                         // The current debug shader unique ID
  uint          debugGLID;                        // The current debug shader OpenGL ID
  wxString      debugSource;                      // The current debug shader source

  UniformDataArray      uniformValues;            // The uniform data array
  FrameBufferDataArray  frameBufferArray;         // The frame buffer data array

  wxToolBar                  *debugToolBar;       // The debug tool bar 
  GLIDebugVariableGrid       *uniformGrid;        // The property grid used to display uniforms
  GLIDebugVariableGrid       *watchGrid;          // The property grid used to display debug variables
  GLIBitmapNotebook          *bitmapNotebook;     // The notebook displaying the render buffers

  wxStaticText *selectionStaticText;              // The static text displayed for the current selection
  wxTextCtrl   *xPosTextCtrl;                     // The text control that handles X coordinate entry
  wxTextCtrl   *yPosTextCtrl;                     // The text control that handles Y coordinate entry  
  wxButton     *debugPixelButton;                 // The button that is pressed to debug a pixel
  wxBoxSizer   *mainPanelSizer;                   // The main panel sizer  

  void OnClose(wxCloseEvent& event);
  void OnTool(wxCommandEvent& event);
  void OnText(wxCommandEvent& event);
  void OnButtonPixelDebug(wxCommandEvent& event);

  //@
  //  Summary:
  //    To open (or bring to focus) the current shader that is being debugged 
  //    in the main scite window.
  //  
  void OpenDebugSource();

  //@
  //  Summary:
  //    To update the display of uniforms/watch values with the current data.
  //  
  void UpdateVariableDisplay();

  //@
  //  Summary:
  //    To sync the controls with the current state of the toolbar.
  //  
  void SyncToolBarState();

  //@
  //  Summary:
  //    To sync the x/y debug pixel location to the current state of the controls.
  //  
  void SyncSelectedPixel();

};

#endif // _GLISHADERDEBUG_H_
