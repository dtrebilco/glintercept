
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif // WX_PRECOMP

#include "wx/stedit/stedit.h"
#include "wx/stedit/steframe.h"
#include <wx/sstream.h>

#include "GLIShaderData.h"
#include "GLIShaders.h"
#include "GLIShaderDebug.h"
#include "..\..\SEInterfaceCommands.h"


//The HInstance of the app (defined by wxWidgets)
extern HINSTANCE wxhInstance;

//A index for command so all issued commands are unique
uint commandIndex = 1;

//Callback for the shader dialog
LRESULT PASCAL ShaderDialogMsg(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//The class name of the created message window
static char SDInterface_ClassName[] = "GLI_ShaderData_Win_Class";

//The single instance of the shader data dialog
GLIShaderData *dataInstance = NULL;


///////////////////////////////////////////////////////////////////////////////
//
GLIShaderData::GLIShaderData(wxSTEditorFrame *newSte):
ste(newSte),

receiveMessageWin(NULL),
gliWindow(NULL),

showShader(false),
breakShader(false),
currCommandID(0)
{

}

///////////////////////////////////////////////////////////////////////////////
//
GLIShaderData::~GLIShaderData()
{
  //Close the GLI connection
  CloseGLIConnection();
  
  //Set the global variable to NULL
  dataInstance = NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderData::Init(wxSTEditorFrame *newSte, long gliWindowHandle)
{
  //If already init, return false;
  if(dataInstance != NULL)
  {
    return false;
  }

  //Create the instance  
  dataInstance = new GLIShaderData(newSte);

  //Connect to the GLI window
  if(!dataInstance->OpenGLIConnection(gliWindowHandle))
  {
    delete dataInstance;
    dataInstance = NULL;
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::Shutdown()
{
  //Delete the instance
  if(dataInstance != NULL)
  {
    delete dataInstance;
  }
  dataInstance = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
GLIShaderData *GLIShaderData::GetInstance()
{
  return dataInstance;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::RefreshShaderList()
{
  //Get the new list from GLI
  SendGLIMessage(CT_ShaderList,"");
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::SetShowShader(uint currShaderUID, bool switchState)
{
  if(switchState != showShader)
  {
    wxString stringID;

    //Enable and disable any shader showing with GLIntercept
    if(switchState)
    {
      //Show the currently selected shader
      stringID.Printf(wxT("%u"), currShaderUID);
      SendGLIMessage(CT_ShowShader, stringID, false);
    }
    else
    {
      //Unselect any selected shader
      stringID = wxT("0");
      SendGLIMessage(CT_ShowShader, stringID, false);
    }

    //Assign the new state
    showShader = switchState;
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::SetBreakShader(uint currShaderUID, bool switchState)
{
  if(switchState != breakShader)
  {
    wxString stringID;

    //Enable and disable any shader "breaking" with GLIntercept
    if(switchState)
    {
      //Break on the currently selected shader when it is used
      stringID.Printf(wxT("%u"), currShaderUID);
      SendGLIMessage(CT_BreakShader, stringID, false);
    }
    else
    {
      //Unselect any selected shader
      stringID = wxT("0");
      SendGLIMessage(CT_BreakShader, stringID, false);
    }

    //Assign the new state
    breakShader = switchState;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::SetSelectedShader(uint newShaderUID)
{
  wxString stringID;

  //If "showing" the selected shader
  if(showShader)
  {
    stringID.Printf(wxT("%u"), newShaderUID);
    SendGLIMessage(CT_ShowShader, stringID, false);
  }

  //If breaking on the selected shader
  if(breakShader)
  {
    stringID.Printf(wxT("%u"), newShaderUID);
    SendGLIMessage(CT_BreakShader, stringID, false);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::OpenShader(uint currShaderUID, const wxString &openFileName)
{
  //Only request valid ID's
  if(currShaderUID != 0)
  {
    //Request the source for the shader
    wxString stringID;
    stringID.Printf(wxT("%u"), currShaderUID);
    if(!SendGLIMessage(CT_ShaderSource,stringID))
    {
      return;
    }

    //Assign the opening filename
    currFileName = openFileName;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::CompileShader(const wxString &fileName, const wxString &source)
{
  ste->OutputAppendString(wxT("Compiling...\n  "));
  ste->OutputAppendString(fileName);
  ste->OutputAppendString(wxT("..."));

  //Extract the shader ID and append it with the source 
  //  (gets the first number from the filename)
  wxString compileData;
  int findOffset = fileName.Find('_');
  if(findOffset >= 0)
  {
    compileData = wxString(fileName, findOffset);
  }
  else
  {
    compileData = "0";
  }

  compileData += wxString(":");
  compileData += source;

  //Pass to GLIntercept
  if(!SendGLIMessage(CT_CompileShader,compileData))
  {
    return;
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::SetDebugShader(bool debugFlag, const wxString &fileName)
{
  //Extract the shader ID
  wxString shaderUID;
  int findOffset = fileName.Find('_');
  if(findOffset >= 0)
  {
    shaderUID = wxString(fileName, findOffset);
  }
  else
  {
    //Not a valid ID, so just return
    return;
  }

  //Pass to GLIntercept
  if(debugFlag)
  {
    SendGLIMessage(CT_SetDebugShader,shaderUID, false);
  }
  else
  {
    SendGLIMessage(CT_UnsetDebugShader,shaderUID, false);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::RevertShader(uint currShaderUID, const wxString &revertFileName)
{
  //Only request valid ID's
  if(currShaderUID != 0)
  {
    //Close the file if open
    wxSTEditorNotebook * notebook = ste->GetEditorNotebook();
    if(notebook)
    {
      //Find and close the page
      int pageNum = notebook->FindEditorPageByFileName(revertFileName);
      if (pageNum != wxNOT_FOUND)
      {
        notebook->ClosePage(pageNum, false);
      }
    }    

    //Flag to GLI that the shader needs reverting
    wxString revertID;
    revertID.Printf(wxT("%u"), currShaderUID);
    SendGLIMessage(CT_RevertShader,revertID, false);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderData::OpenGLIConnection(long gliWindowHandle)
{
  //Check the initial handle
  if(gliWindowHandle == 0)
  {
    ste->OutputAppendString(wxT("== No GLIntercept connection ==\n"));
    return false;
  }

  //Create the class of the interface window
  WNDCLASS wndclass;
  wndclass.style = 0;
  wndclass.lpfnWndProc = ShaderDialogMsg;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = wxhInstance;
  wndclass.hIcon = 0;
  wndclass.hCursor = NULL;
  wndclass.hbrBackground = NULL;
  wndclass.lpszMenuName = 0;
  wndclass.lpszClassName = SDInterface_ClassName;
  if (!::RegisterClass(&wndclass))
  {
    ste->OutputAppendString(wxT("== Unable to register window class ==\n"));
    return false;
  }

  //Create the window
  receiveMessageWin = ::CreateWindow(
                                    SDInterface_ClassName,
                                    SDInterface_ClassName,
                                    0,
                                    0, 0, 0, 0,
                                    0,
                                    0,
                                    wxhInstance,
                                    0);
  if (!receiveMessageWin)
  {
    UnregisterClass(SDInterface_ClassName, wxhInstance);
    ste->OutputAppendString(wxT("== Unable to create connection window ==\n"));
    return false;
  }

  //Assign the GLIntercept window handle
  gliWindow = reinterpret_cast<HWND>(gliWindowHandle);

  //Send the init message
  if(!SendGLIMessage(CT_Init, wxT(""), false))
  {
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::CloseGLIConnection()
{
  //Flag to the GLI window, that this is closing down
  if(gliWindow != NULL && receiveMessageWin != NULL)
  {
    COPYDATASTRUCT destroyData;
    destroyData.dwData = CT_Shutdown;
    destroyData.cbData = 0;
    destroyData.lpData = NULL;

    //Send the message 
    ::SendMessage(gliWindow, WM_COPYDATA, reinterpret_cast<WPARAM>(receiveMessageWin), reinterpret_cast<LPARAM>(&destroyData));
    gliWindow = NULL;
  }

  //Destroy the window if necessary
  if(receiveMessageWin != NULL)
  {
    DestroyWindow(receiveMessageWin);

    //Unregister the class
    UnregisterClass(SDInterface_ClassName, wxhInstance);
    receiveMessageWin = NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderData::SendGLIMessage(uint cmdID, const wxString &message, bool gliResponse)
{
  //If no connection
  if(!gliWindow || !receiveMessageWin)
  {
    ste->OutputAppendString(wxT("No GLI connection"));
    return false;
  }

  //Check if a message is pending
  if(currCommandID != 0)
  {
    ste->OutputAppendString(wxT("Awaiting other GLI command\n"));
    return false;
  }

  //Append the UID to the command
  currCommandID = (commandIndex << 16) | cmdID;

  //Increment the command index so all commands are unique
  commandIndex++;

  //Create the structure to send to GLI
  COPYDATASTRUCT sendData;
  sendData.dwData = currCommandID;
  sendData.cbData = 0;
  sendData.lpData = NULL;

  //Append any message data
  // NOTE: This has to be length not size! (STL difference)
  if(message.length() > 0)
  {
    sendData.cbData = message.length();
    sendData.lpData = (char*)message.c_str();
  }

  //Send the message and time out if not responded to in 3 seconds
  DWORD result;
  if(!::SendMessageTimeout(gliWindow, WM_COPYDATA, 
                           reinterpret_cast<WPARAM>(receiveMessageWin), reinterpret_cast<LPARAM>(&sendData),
                           SMTO_NORMAL, 3000, &result))
  {
    ste->OutputAppendString(wxT("== Unable to contact the GLIntercept window ==\n"));
    
    //Reset the command ID if the window could not be contacted.
    currCommandID = 0;
    return false;
  }
  
  //If a GLIntercept response is expected.
  if(gliResponse)
  {
    //Setup the timer to abort the command if not processed in 5 seconds
    ::SetTimer(receiveMessageWin, currCommandID, 5000, (TIMERPROC) NULL);     
  }
  else
  {
    //If no response is expected, allow other commands to be sent
    currCommandID = 0;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
BOOL GLIShaderData::ProcessGLIMessage(HWND msgWnd, PCOPYDATASTRUCT data)
{
  //Return false if the window ID's do not match
  if(msgWnd != gliWindow)
  {
    return FALSE;
  }

  //A debug start command can arrive at any time, so catch it here
  if((CommandType)LOWORD(data->dwData) == CT_DebugShaderInit)
  {
    //Process the debug start
    ProcessDebugInit(data->cbData, data->lpData);
    return TRUE;
  }

  //Ignore command IDs not expected
  if(data->dwData != currCommandID)
  {
    ste->OutputAppendString(wxT("== Unexpected command from GLIntercept ==\n"));
    return FALSE;
  }

  //Kill the timer associated with the command
  ::KillTimer(receiveMessageWin, currCommandID);

  //Get the command data(if any)
  wxString dataStr;
  if(data->cbData > 0)
  {
    //Assign the data (note there does not have to be a terminating NULL)
    dataStr = wxString((char*)data->lpData, data->cbData);
  }

  //Reset the current command ID
  currCommandID = 0;

  //Process the commands from GLIntercept
  switch(LOWORD(data->dwData))
  {
    case(CT_ShaderList):
      ProcessShaderList(dataStr);
      break;

    case(CT_ShaderSource):
      ProcessShaderSource(dataStr);
      break;

    case(CT_CompileShader):
      ProcessShaderCompile(dataStr);
      break;

    default:
      ste->OutputAppendString(wxT("== Unknown command from GLIntercept ==\n"));
      break;
  };

  return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//
BOOL GLIShaderData::ProcessTimerMessage(uint timerID)
{
  //If the current command has timed out
  if(currCommandID == timerID)
  {
    //Reset the command
    currCommandID = 0; //DT_TODO:Should we be setting a time out here?
    ste->OutputAppendString(wxT("== Command timed out -GLIntercept unresponsive (hung OpenGL app?) ==\n"));
    return TRUE;
  }

  //Probably an old time out
  return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::ProcessShaderList(const wxString &dataStr)
{
  //Get the shader dialog
  GLIShaders * shaderDialog = ste->GetGLIShadersDialog();
  if(!shaderDialog)
  {
    return;
  }

  //Set the new shader data
  shaderDialog->SetShaderListData(dataStr);

}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::ProcessShaderSource(const wxString &dataStr)
{
  //Open using the current "file name"
  wxSTEditorNotebook * notebook = ste->GetEditorNotebook();
  if(notebook)
  {
    //Load in the string as a stream
    wxStringInputStream strStream(dataStr); 
    notebook->LoadStream(currFileName, strStream);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::ProcessShaderCompile(const wxString &dataStr)
{
  ste->OutputAppendString(dataStr);
}



///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::ProcessDebugInit(uint bufferSize, void *buffer)
{
  GLIShaderDebug * shaderDebug = ste->GetGLIShaderDebugDialog();

  //Check if debugging can start
  if(!shaderDebug ||
     !shaderDebug->DebugBegin(bufferSize, buffer))
  {
    ste->OutputAppendString("== Unable to switch to debug mode. Bad network data? ==\n");

    //DT_TODO: Flag to GLI that debugging failed

  }

}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::SetDebugEnd()
{
  // Flag that debugging is completed
  SendGLIMessage(CT_DebugShaderDone,"", false);
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderData::SetDebugPixel(const wxPoint & debugPixel)
{
  // Pack the debug point into a GLI message
  wxString debugPixelString;
  debugPixelString.Printf(wxT("%u:%u"), debugPixel.x, debugPixel.y);
  if(!SendGLIMessage(CT_DebugShaderPixel, debugPixelString))
  {
    return false;
  }

  return true;
}

//DT_TODO: Cache these messages and process them on event idle 
//  (helps prevent issues with calling GUI system - eg InsertEditorSplitter with nPage = 0 to insert in the front always?)
///////////////////////////////////////////////////////////////////////////////
//
LRESULT PASCAL ShaderDialogMsg(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
  //Process the message
  switch (iMessage) 
  { 
    //Handle the responses from GLIntercept for data
    case WM_COPYDATA:
    {
      if(GLIShaderData::GetInstance())
      {
        return GLIShaderData::GetInstance()->ProcessGLIMessage((HWND)wParam, (PCOPYDATASTRUCT)lParam);
      }
    }

    // Handle time outs waiting for GLIntercept
    case WM_TIMER:
    {
      if(GLIShaderData::GetInstance())
      {
        return GLIShaderData::GetInstance()->ProcessTimerMessage((uint)wParam);
      }
    }
  }

  return ::DefWindowProc(hWnd, iMessage, wParam, lParam);
}

