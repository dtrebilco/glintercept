
#ifndef __GLI_SHADER_DATA_H_
#define __GLI_SHADER_DATA_H_


typedef unsigned int uint;

//DT_TODO: Cleanup comments, class checks

class wxSTEditorFrame;

//@
//  Summary:
//    This class displays the shader listing dialog and handles communication
//    with the GLIntercept application.
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
  static bool Init(wxSTEditorFrame *newSte, long gliWindowHandle);

  //@
  //  Summary:
  //    To shutdown the GLIShader data interface.
  //  
  static void Shutdown();

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
  //    Re-load all the shader ids from GLIntercept
  //  
  void RefreshShaderList();

  //@
  //  Summary:
  //    To set if selected shaders are shown or not.
  //  
  //  Parameters:
  //    currShaderUID - The current shader ID. (if any)
  //
  //    switchState - The switch state to toggle showing shaders.
  //
  void SetShowShader(uint currShaderUID, bool switchState);

  //@
  //  Summary:
  //    To set if selected shaders are have a debugger breakpoint thrown on
  //    them when used.
  //  
  //  Parameters:
  //    currShaderUID - The current shader ID. (if any)
  //
  //    switchState - The switch state to toggle "breaking on" shaders.
  //
  void SetBreakShader(uint currShaderUID, bool switchState);

  //@
  //  Summary:
  //    To set the selected shader ID
  //  
  //  Parameters:
  //    newShaderUID - The shader ID that is the new selected shader.
  //
  void SetSelectedShader(uint newShaderUID);

  //@
  //  Summary:
  //    To open the currently selected shader in the editor.
  //
  //  Parameters:
  //    currShaderUID - The current shader ID. 
  //
  //    openFileName - The filename of the shader to open.
  //
  void OpenShader(uint currShaderUID, const wxString &openFileName);

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
  void CompileShader(const wxString &fileName, const wxString &source);

  //@
  //  Summary:
  //    To revert the currently selected shader.
  //  
  //  Parameters:
  //    currShaderUID - The current shader ID. 
  //
  //    revertFileName - The filename of the shader to revert.
  //
  void RevertShader(uint currShaderUID, const wxString &revertFileName);

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
  void SetDebugShader(bool debugFlag, const wxString &fileName);

  //@
  //  Summary:
  //    To flag that shader debugging has completed.
  //  
  void SetDebugEnd();

  //@
  //  Summary:
  //    To set the debug pixel location.
  //  
  bool SetDebugPixel(const wxPoint & debugPixel);

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

  wxSTEditorFrame *ste;                           // Handle to the ste instance
  
  HWND         receiveMessageWin;                 // The handle to receive messages on
  HWND         gliWindow;                         // The handle to the GLIntercept window

  bool         showShader;                        // Flag indicating if the selected shader is visualized on the screen
  bool         breakShader;                       // Flag indicating if the selected shader has a debugger breakpoint thrown on it

  uint         currCommandID;                     // The current command that we are waiting for GLIntercept to respond to. 
                                                  //  Is 0 for no current command
  wxString     currFileName;                      // The current filename (if any) associated with the command.

  //@
  //  Summary:
  //    Constructor
  //  
  GLIShaderData(wxSTEditorFrame *ste);

  //@
  //  Summary:
  //    Destructor, only to be called internally
  //  
  virtual ~GLIShaderData();

  //@
  //  Summary:
  //    To open a connection to the controlling GLIntercept.
  //  
  //  Parameters:
  //    gliWindowHandle - The GLIntercept handle to connect to.
  //
  //  Returns:
  //    Returns true on connect success, false on failure.
  //
  bool OpenGLIConnection(long gliWindowHandle);

  //@
  //  Summary:
  //    To close the connection to GLIntercept.
  //  
  void CloseGLIConnection();

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
  //  Returns:
  //    Returns true on message success, false on failure.
  //
  bool SendGLIMessage(uint cmdID, const wxString &message, bool gliResponse = true);

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
  void ProcessShaderList(const wxString &dataStr);
  void ProcessShaderSource(const wxString &dataStr);
  void ProcessShaderCompile(const wxString &dataStr);
  void ProcessDebugInit(uint bufferSize, void *buffer);

};



#endif // __GLI_SHADER_DATA_H_
