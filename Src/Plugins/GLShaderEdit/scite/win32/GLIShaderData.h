
#ifndef __GLI_SHADER_DATA_H_
#define __GLI_SHADER_DATA_H_

#include "SciTEWin.h"

typedef unsigned int uint;

class SciTEBase;
class GLIToolTip;

//Structure to hold data for a single list item
struct ListItemData
{
  ListItemData();
  uint    shaderUID;     // The shader unique ID
  uint    openGLID;      // The OpenGL shader ID
  SString shaderType;    // The Type of shader
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


//@
//  Summary:
//    This class displays the shader listing dialog and handles communication
//    with the GLIntercet application.
//  
class GLIShaderData
{
public:

  //@
  //  Summary:
  //    To initialize the main window etc.
  //  
  //  Returns:
  //    If initialization was successful, true is returned. Else false is returned.
  //
  static bool Init(HINSTANCE hInstance, HWND parentWnd, SciTEBase *scite);

  //@
  //  Summary:
  //    To get the shader data instance (if any).
  //  
  //  Returns:
  //    If a instance is available it is returned.
  //
  static GLIShaderData *GetInstance();

  //@
  //  Summary:
  //    Destructor, only to be called internally
  //  
  virtual ~GLIShaderData();

  //@
  //  Summary:
  //    Re-load all the shader ids from GLIntercept
  //  
  void RefreshShaderList();

  //@
  //  Summary:
  //    To set if selected shaders are shown or not.
  //  
  //  Parameters:
  //    switchState - The switch state to toggle showing shaders.
  //
  void SetShowShader(bool switchState);

  //@
  //  Summary:
  //    To set if selected shaders are have a debugger breakpoint thrown on
  //    them when used.
  //  
  //  Parameters:
  //    switchState - The switch state to toggle "breaking on" shaders.
  //
  void SetBreakShader(bool switchState);

  //@
  //  Summary:
  //    To set the selected shader ID
  //  
  //  Parameters:
  //    rowNum - The row number that contains the selected shader.
  //
  void SetSelectedShader(int rowNum);

  //@
  //  Summary:
  //    To set the column that the list is sorted on.
  // 
  //  Parameters:
  //    columnNum - The new column to sort by.
  //
  void SetColumnSort(uint columnNum);

  //@
  //  Summary:
  //    To open the currently selected shader in SciTE
  //  
  void OpenShader();

  //@
  //  Summary:
  //    To compile the passed source with GLIntercept. The type
  //    of compile is determined by the filename.
  //  
  //  Parameters:
  //    fileName - The name of the source string passed. 
  //               The shader ID will be extracted from the name.
  //
  //    source   - The source data of the shader.
  //
  void CompileShader(const SString &fileName, const char *source);

  //@
  //  Summary:
  //    To revert the currently selected shader.
  //  
  void RevertShader();

  //@
  //  Summary:
  //    To set the passed shader filename to be debugged/not-debugged.
  //  
  //  Parameters:
  //    debugFlag - The flag indicating if the shader is being debugged
  //                or not debugged. 
  //
  //    fileName  - The name of the shader. 
  //
  void SetDebugShader(bool debugFlag, const SString &fileName);

  //@
  //  Summary:
  //    To process a GLIntercept response message.
  //  
  //  Parameters:
  //    msgWnd - The ID of the window that sent the message.
  //
  //    data - The data sent.
  //
  BOOL ProcessGLIMessage(HWND msgWnd, PCOPYDATASTRUCT data);

  //@
  //  Summary:
  //    To process a timer message.
  //  
  //  Parameters:
  //    timerID - The ID of the timer.
  //
  BOOL ProcessTimerMessage(uint timerID);

protected:

  SciTEBase *scite;                               // Handle to the scite instance

  HWND         dialogWindow;                      // The handle to the dialog window
  HWND         gliWindow;                         // The handle to the GLIntercept window
  GLIToolTip  *gliToolTips;                       // The class handeling the tooltips for the dialog

  bool         showShader;                        // Flag indicating if the selected shader is visualized on the screen
  bool         breakShader;                       // Flag indicating if the selected shader has a debugger breakpoint thrown on it
  ListItemData currShaderData;                    // The currently selected shader data

  uint         currCommandID;                     // The current command that we are waiting for GLINtercept to respond to. 
                                                  //  Is 0 for no current command
  SString      currFileName;                      // The current filename (if any) associated with the command.


  ColumnSortOrder columnSort;                     // Enum indicating the sort order of the list box display
  bool            sortReverse;                    // Flag indicating if the column sort order is reveresed

  //@
  //  Summary:
  //    Constructor
  //  
  GLIShaderData(SciTEBase *scite);

  //@
  //  Summary:
  //    To send a message to the GLIntercept window. This will also
  //    install a callback to catch time outs if GLIntercept stops responding.
  //  
  //  Parameters:
  //    cmdID - The ID of the command to send.
  //
  //    message - The command data.
  //
  //    gliResponse - Flag if sending this message expects a response 
  //                  from GLIntercept. 
  //
  bool SendGLIMessage(uint cmdID, const SString &message, bool gliResponse = true);

  //@
  //  Summary:
  //    To process the result of a command send back from GLIntercept.
  //  
  //  Parameters:
  //    dataStr - The returned data from the command.
  //
  //    bufferSize - The size of the buffer.
  //
  //    buffer - The buffer sent from GLIntercept.
  //
  void ProcessShaderList(const SString &dataStr);
  void ProcessShaderSource(const SString &dataStr);
  void ProcessShaderCompile(const SString &dataStr);
  void ProcessDebugInit(uint bufferSize, void *buffer);

  //@
  //  Summary:
  //    To add a row to the display.
  //  
  //  Parameters:
  //    rowNum  - The number of the row to add.
  // 
  //    shaderData - The shader data to add to the row 
  //                 (seperated by colons ":").
  //
  void AddShaderRow(uint rowNum, const SString &shaderData);

  //@
  //  Summary:
  //    To add the data for a single list item to the dialog box.
  //  
  //  Parameters:
  //    rowNum  - The number of the row to add.
  // 
  //    listBox - The list box to add to.
  //
  //    listData - The data to add.
  //
  //  Returns:
  //    Returns true on success, false on failure.
  //
  void AddListItem(uint rowNum, HWND listBox, const ListItemData &listData);

  //@
  //  Summary:
  //    To get the data for a single list item from the dialog box.
  //  
  //  Parameters:
  //    rowNum  - The number of the row to get.
  // 
  //    listBox - The list box containing the data.
  //
  //    listData - The returned list box data
  //
  //  Returns:
  //    Returns true on success, false on failure.
  //
  bool GetListItem(uint rowNum, HWND listBox, ListItemData &listData);


  //@
  //  Summary:
  //    To sort the list view using teh current sorting options.
  //  
  //  Returns:
  //    Returns true on success, false on failure.
  //
  bool SortList();

};



#endif // __GLI_SHADER_DATA_H_
