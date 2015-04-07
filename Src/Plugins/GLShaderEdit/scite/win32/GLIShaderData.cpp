
#include "GLIShaderData.h"
#include "SciTE.h"
#include "Scintilla.h"
#include "GLIToolTip.h"
#include "GLIShaderDebug.h"
#include "..\..\SEInterfaceCommands.h"

#include <algorithm>

using namespace std;

// ***********************************
// *** IMP - When using SString - note that length() != size() (as it is in STL)
// ***********************************

//A index for command so all issued commands are unique
uint commandIndex = 1;

//Callback for the shader dialog
BOOL CALLBACK ShaderDialogMsg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//The single instance of the shader data dialog
GLIShaderData *dataInstance = NULL;


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
GLIShaderData::GLIShaderData(SciTEBase *newScite):
scite(newScite),
dialogWindow(NULL),
gliToolTips(NULL),
gliWindow(NULL),
showShader(false),
breakShader(false),
currCommandID(0),

columnSort(CS_FrameNum),
sortReverse(true)
{

}

///////////////////////////////////////////////////////////////////////////////
//
GLIShaderData::~GLIShaderData()
{
  //Let the parent window handle destroying the dialog window
  
  //Flag to the GLI window, that this is closing down
  if(gliWindow != NULL)
  {
    COPYDATASTRUCT destroyData;
    destroyData.dwData = CT_Shutdown;
    destroyData.cbData = 0;
    destroyData.lpData = NULL;

    //Send the message 
    ::SendMessage(gliWindow, WM_COPYDATA, reinterpret_cast<WPARAM>(dialogWindow), reinterpret_cast<LPARAM>(&destroyData));
    gliWindow = NULL;
  }

  if(gliToolTips)
  {
    delete gliToolTips;
    gliToolTips = NULL;
  }

  //Set the global variable to NULL
  dataInstance = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderData::Init(HINSTANCE hInstance, HWND parentWnd, SciTEBase *newScite)
{
  //If already init, return false;
  if(dataInstance != NULL)
  {
    return false;
  }

  //Create the shader debug dialog
  if(!GLIShaderDebug::Init(hInstance, parentWnd, newScite))
  {
    return false;
  }

  //Create the instance  
  dataInstance = new GLIShaderData(newScite);

  //Get the GLIntercept window
  char *gliWindowStr = newScite->Property("glintercept.hwnd");
  if(!gliWindowStr || !gliWindowStr[0])
  {
    newScite->OutputAppendString("== No GLIntercept connection ==\n");
    delete dataInstance;
    dataInstance = NULL;

    //Delete the window string
    if(gliWindowStr)
    {
      delete [] gliWindowStr;
      gliWindowStr = NULL;
    }
    return false;
  }

  //Get the window handle
  HWND testWnd = reinterpret_cast<HWND>(atoi(gliWindowStr));
  delete [] gliWindowStr;
  gliWindowStr = NULL;

  //Create the dialog window
  dataInstance->dialogWindow = ::CreateDialog(hInstance, MAKEINTRESOURCE(IDD_GLISHADERS), parentWnd, ShaderDialogMsg); 
  if(!dataInstance->dialogWindow)
  {
    delete dataInstance;
    dataInstance = NULL;
    return false;
  }

  //Init the communication with the plugin
  COPYDATASTRUCT initData;
  initData.dwData = CT_Init;
  initData.cbData = 0;
  initData.lpData = NULL;

  //Send the message and time out if not responded to in 3 seconds
  DWORD result;
  if(!::SendMessageTimeout(testWnd, WM_COPYDATA, 
                           reinterpret_cast<WPARAM>(dataInstance->dialogWindow), reinterpret_cast<LPARAM>(&initData),
                           SMTO_NORMAL, 3000, &result))
  {
    newScite->OutputAppendString("== Unable to init the GLIntercept window ==\n");
    delete dataInstance;
    dataInstance = NULL;
    return false;
  }
  dataInstance->gliWindow = testWnd;

  //Position window 
  HWND parent = GetParent(dataInstance->dialogWindow);
  if(parent)
  {
    RECT parentPos;
    RECT dialogSize;
    if(GetWindowRect(parent, &parentPos) &&
       GetWindowRect(dataInstance->dialogWindow, &dialogSize))
    {
      //Position the window to the right of the host window
      int newW = dialogSize.right  - dialogSize.left;
      int newH = dialogSize.bottom - dialogSize.top;

      int newX = parentPos.right - (newW/4);
      int newY = dialogSize.top;

      //Set the new window position
      SetWindowPos(dataInstance->dialogWindow, HWND_TOP,
                   newX, newY, newW, newH, 
                   SWP_SHOWWINDOW);
    }
  }

  //Show the window
  ShowWindow(dataInstance->dialogWindow, SW_SHOW);

  //Set the initial shader data
  dataInstance->RefreshShaderList();

  //Create the tooltips 
  dataInstance->gliToolTips = new GLIToolTip(dataInstance->dialogWindow, hInstance);

  return true;
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
  //Return if there is no window
  if(!dialogWindow)
  {
    return;
  }

  //Get the list box
  HWND listbox = GetDlgItem(dialogWindow, IDC_SHADERLIST);  
  if(!listbox)
  {
    return;
  }

  //Empty all existing data in the list
  ListView_DeleteAllItems(listbox);

  //Get the new list from GLI
  SendGLIMessage(CT_ShaderList,"");

}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::SetShowShader(bool switchState)
{
  if(switchState != showShader)
  {
    //Enable and disable any shader showing with GLIntercept
    if(switchState)
    {
      //Show the currently selected shader
      SString stringID(currShaderData.shaderUID);
      SendGLIMessage(CT_ShowShader, stringID, false);
    }
    else
    {
      //Unselect any selected shader
      SString stringID(0);
      SendGLIMessage(CT_ShowShader, stringID, false);
    }

    //Assign the new state
    showShader = switchState;
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::SetBreakShader(bool switchState)
{
  if(switchState != breakShader)
  {
    //Enable and disable any shader "breaking" with GLIntercept
    if(switchState)
    {
      //Break on the currently selected shader when it is used
      SString stringID(currShaderData.shaderUID);
      SendGLIMessage(CT_BreakShader, stringID, false);
    }
    else
    {
      //Unselect any selected shader
      SString stringID(0);
      SendGLIMessage(CT_BreakShader, stringID, false);
    }

    //Assign the new state
    breakShader = switchState;
  }

}



///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::SetSelectedShader(int rowNum)
{
  //Save the old ID
  uint oldShaderID = currShaderData.shaderUID;

  //If the row is valid, extract the list data
  if(rowNum >= 0)
  {
    //Get the list box
    HWND listBox = GetDlgItem(dialogWindow, IDC_SHADERLIST);  

    //Attempt to get the data
    if(!listBox || !GetListItem(rowNum, listBox ,currShaderData))
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
  if(oldShaderID != currShaderData.shaderUID)
  {
    SString stringID(currShaderData.shaderUID);

    //If "showing" the selected shader
    if(showShader)
    {
      SendGLIMessage(CT_ShowShader, stringID, false);
    }

    //If breaking on the selected shader
    if(breakShader)
    {
      SendGLIMessage(CT_BreakShader, stringID, false);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::SetColumnSort(uint columnNum)
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

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::OpenShader()
{
  //Only request valid ID's
  if(currShaderData.shaderUID != 0)
  {
    //Request the source for the shader
    SString stringID(currShaderData.shaderUID);
    if(!SendGLIMessage(CT_ShaderSource,stringID))
    {
      return;
    }

    //Set the current source retrieve string
    currFileName = stringID; 

    currFileName += "_";
    currFileName += SString(currShaderData.openGLID);

    currFileName += ".";
    currFileName += currShaderData.shaderType;
    
    //Make the filename lower case
    currFileName.lowercase();
  }


}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::CompileShader(const SString &fileName, const char *source)
{
  scite->OutputAppendString("Compiling...\n  ");
  scite->OutputAppendString(fileName.c_str());
  scite->OutputAppendString("...");

  //Extract the shader ID and append it with the source 
  //  (gets the first number from the filename)
  SString compileData;
  int findOffset = fileName.search("_");
  if(findOffset >= 0)
  {
    compileData = SString(fileName.c_str(), 0, findOffset);
  }
  else
  {
    compileData = "0";
  }

  compileData += SString(":");
  compileData += SString(source);

  //Pass to GLIntercept
  if(!SendGLIMessage(CT_CompileShader,compileData))
  {
    return;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::SetDebugShader(bool debugFlag, const SString &fileName)
{
  //Extract the shader ID
  SString shaderUID;
  int findOffset = fileName.search("_");
  if(findOffset >= 0)
  {
    shaderUID = SString(fileName.c_str(), 0, findOffset);
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
void GLIShaderData::RevertShader()
{
  //Only request valid ID's
  if(currShaderData.shaderUID != 0)
  {
    SString closeFile(currShaderData.shaderUID);

    closeFile += "_";
    closeFile += SString(currShaderData.openGLID);

    closeFile += ".";
    closeFile += currShaderData.shaderType;
    
    //Make the filename lower case
    closeFile.lowercase();

    //Close the file if open
    scite->CloseBuffer(closeFile.c_str());

    //Flag to GLI that the shader needs reverting
    SString revertID(currShaderData.shaderUID);
    SendGLIMessage(CT_RevertShader,revertID, false);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderData::SendGLIMessage(uint cmdID, const SString &message, bool gliResponse)
{
  //If no connection
  if(!gliWindow || !dialogWindow)
  {
    scite->OutputAppendString("No GLI connection");
    return false;
  }

  //Check if a message is pending
  if(currCommandID != 0)
  {
    scite->OutputAppendString("Awaiting other GLI command\n");
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
                           reinterpret_cast<WPARAM>(dialogWindow), reinterpret_cast<LPARAM>(&sendData),
                           SMTO_NORMAL, 3000, &result))
  {
    scite->OutputAppendString("== Unable to contact the GLIntercept window ==\n");
    
    //Reset the command ID if the window could not be contacted.
    currCommandID = 0;
    return false;
  }
  
  //If a GLIntercept response is expected.
  if(gliResponse)
  {
    //Setup the timer to abort the command if not processed in 5 seconds
    ::SetTimer(dialogWindow, currCommandID, 5000, (TIMERPROC) NULL);     
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
    scite->OutputAppendString("== Unexpected command from GLIntercept ==\n");
    return FALSE;
  }

  //Kill the timer associated with the command
  ::KillTimer(dialogWindow,currCommandID);

  //Get the command data(if any)
  SString dataStr;
  if(data->cbData > 0)
  {
    //Assign the data (note there does not have to be a terminating NULL)
    dataStr = SString((char*)data->lpData, 0, data->cbData);
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
      scite->OutputAppendString("== Unknown command from GLIntercept ==\n");
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
    scite->OutputAppendString("== Command timed out -GLIntercept unresponsive (hung OpenGL app?) ==\n");
    return TRUE;
  }

  //Probably an old time out
  return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::ProcessShaderList(const SString &dataStr)
{
  uint rowNum =0;

  //Extract each row of data
  int startPos = 0;
  int endPos   = dataStr.search("*");
  while (endPos > 0)
  {
    //Extract a single shaders data
    SString shaderData(dataStr.c_str(), startPos, endPos);

    //Add a shader row
    AddShaderRow(rowNum, shaderData);

    //Get the next start values
    startPos = endPos + 1;
    endPos   = dataStr.search("*",startPos);
  }

  //Reset the selected shader
  SetSelectedShader(-1);

  //Sort the data by the selected column
  SortList();
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::AddShaderRow(uint rowNum, const SString &shaderData)
{
  //Get the list box
  HWND listbox = GetDlgItem(dialogWindow, IDC_SHADERLIST);  
  if(!listbox)
  {
    return;
  }

  SString listData[4];
  SString processData = shaderData;

  //Extract each token from the shader
  for(uint i=0; i<4; i++)
  {
    int endPos = processData.search(":");
    if(endPos < 0)
    {
      scite->OutputAppendString("== Error extracting shaders ==\n");
      return;
    }

    //Extract the string data
    listData[i] = SString(processData.c_str(), 0, endPos);

    //Remove the string from the processing data
    processData.remove(0, endPos+1);
  }

  LVITEM addItem;

  //Set the attributes
  addItem.mask = LVIF_TEXT | LVIF_PARAM;
  
  //Set the data to display
  addItem.iItem = rowNum;          
  addItem.iSubItem = 0;       
  
  //Set the id tag data
  addItem.pszText = (char*)listData[0].c_str();
  addItem.lParam = listData[0].value();

  //Insert the row
  ListView_InsertItem(listbox, &addItem);

  //Insert the data for the columns
  for(uint c=1; c<4; c++) 
  {
    addItem.mask = LVIF_TEXT;
    addItem.iSubItem = c;
    addItem.pszText= (char*)listData[c].c_str();
    ListView_SetItem(listbox, &addItem);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::ProcessShaderSource(const SString &dataStr)
{
  //Open using the current "file name"
  scite->OpenBuffer(currFileName.c_str(), dataStr.c_str());
}


///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::ProcessShaderCompile(const SString &dataStr)
{
  scite->OutputAppendString(dataStr.c_str());
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::ProcessDebugInit(uint bufferSize, void *buffer)
{
  //Check if debugging can start
  if(!GLIShaderDebug::GetInstance() ||
     !GLIShaderDebug::GetInstance()->DebugBegin(bufferSize, buffer))
  {
    scite->OutputAppendString("== Unable to switch to debug mode. Bad network data? ==\n");

    //DT_TODO: Flag to GLI that debugging failed

  }

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
bool GLIShaderData::SortList()
{
  uint i;

  //If no sorting is performed
  if(columnSort == CS_Undefined)
  {
    return true;
  }

  //Get the list box
  HWND listBox = GetDlgItem(dialogWindow, IDC_SHADERLIST);  
  if(!listBox)
  {
    return false;
  }

  //Get the number of list items
  uint numListItems = ListView_GetItemCount(listBox);
  if(numListItems == 0)
  {
    return true;
  }

  //Create a array to hold the items
  ListItemData *listDataArray = new ListItemData[numListItems];

  //Extract the items from the array
  for(i=0; i<numListItems; i++)
  {
    if(!GetListItem(i, listBox, listDataArray[i]))
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
  
  //Empty all existing data in the list
  ListView_DeleteAllItems(listBox);

  //Put the data back into the array
  if(!sortReverse)
  {
    for(i=0; i<numListItems; i++)
    {
      AddListItem(i, listBox, listDataArray[i]);
    }
  }
  else
  {
    for(i=0; i<numListItems; i++)
    {
      AddListItem(i, listBox, listDataArray[numListItems-1-i]);
    }
  }

  //Delete the array
  delete [] listDataArray;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderData::AddListItem(uint rowNum, HWND listBox, const ListItemData &listData)
{
  SString listDataStr[4];
  listDataStr[0] = SString(listData.shaderUID);
  listDataStr[1] = SString(listData.openGLID);
  listDataStr[2] = listData.shaderType;
  listDataStr[3] = SString(listData.frameNum);

  LVITEM addItem;

  //Set the attributes
  addItem.mask = LVIF_TEXT | LVIF_PARAM;
  
  //Set the data to display
  addItem.iItem = rowNum;          
  addItem.iSubItem = 0;       
  
  //Set the id tag data
  addItem.pszText = (char*)listDataStr[0].c_str();
  addItem.lParam = listData.shaderUID;

  //Insert the row
  ListView_InsertItem(listBox, &addItem);

  //Insert the data for the columns
  for(uint c=1; c<4; c++) 
  {
    addItem.mask = LVIF_TEXT;
    addItem.iSubItem = c;
    addItem.pszText= (char*)listDataStr[c].c_str();
    ListView_SetItem(listBox, &addItem);
  }

}



///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderData::GetListItem(uint rowNum, HWND listBox, ListItemData &listData)
{
  char buffer[256+1];

  //Get the shader unique ID
  LVITEM getItem;
  getItem.mask = LVIF_PARAM;
  getItem.iItem = rowNum;          
  getItem.iSubItem = 0;
  getItem.lParam   = 0;
  if(!ListView_GetItem(listBox, &getItem))
  {
    return false;
  }
  listData.shaderUID = getItem.lParam;

  //Get the OpenGL ID
  buffer[0] = '\0';
  ListView_GetItemText(listBox, rowNum, 1, buffer, 256);
  buffer[256] = '\0';
  sscanf(buffer, "%u", &listData.openGLID);


  //Get the GL Type
  buffer[0] = '\0';
  ListView_GetItemText(listBox, rowNum, 2, buffer, 256);
  buffer[256] = '\0';
  listData.shaderType = buffer;


  //Get the frame number
  buffer[0] = '\0';
  ListView_GetItemText(listBox, rowNum, 3, buffer, 256);
  buffer[256] = '\0';
  sscanf(buffer, "%u", &listData.frameNum);

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
BOOL CALLBACK ShaderDialogMsg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  //Process the message
  switch (message) 
  { 
    //Handle the responses from GLIntercept for data
    case WM_COPYDATA:
    {
      if(GLIShaderData::GetInstance())
      {
        return GLIShaderData::GetInstance()->ProcessGLIMessage((HWND)wParam, (PCOPYDATASTRUCT)lParam);
      }
      break;
    }

    // Handle time outs waiting for GLIntercept
    case WM_TIMER:
    {
      if(GLIShaderData::GetInstance())
      {
        return GLIShaderData::GetInstance()->ProcessTimerMessage((uint)wParam);
      }
      break;
    }

    //Setup initial list controls
    case WM_INITDIALOG:
    {
      HWND listbox = GetDlgItem(hDlg, IDC_SHADERLIST);

      LVCOLUMN columnData;
      columnData.mask = LVCF_TEXT | LVCF_WIDTH;

      columnData.pszText = "Shader UID";
      columnData.cx = 70;
      ListView_InsertColumn(listbox, 0, &columnData);

      columnData.pszText = "OpenGL ID";
      columnData.cx = 70;
      ListView_InsertColumn(listbox, 1, &columnData);

      columnData.pszText = "Type";
      columnData.cx = 50;
      ListView_InsertColumn(listbox, 2, &columnData);

      columnData.pszText = "Frame Num";
      columnData.cx = 73;
      ListView_InsertColumn(listbox, 3, &columnData);

      //Select full rows at a time style
      SendMessage(listbox, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT); 

      return TRUE;
    }

    //Destroy the window
    case WM_DESTROY:
    {
      //Delete the data instance
      delete dataInstance;
      return 0;
    }

    //Process notifications
    case WM_NOTIFY:
    {
      LPNMHDR nData = (LPNMHDR)lParam;

      //If it is the shader list
      if(LOWORD(wParam) == IDC_SHADERLIST &&
         GLIShaderData::GetInstance())
      {
        //If it is a item change/activate
        if(nData->code == LVN_ITEMCHANGED ||
           nData->code == LVN_ITEMACTIVATE) //NM_CLICK/NM_DBLCLK/LVN_KEYDOWN
        {
          //Get the selected row
          int selectedItem = -1;
          selectedItem = ListView_GetNextItem(nData->hwndFrom, -1, LVNI_FOCUSED);
          if(selectedItem >= 0)
          {
            //Set the selected shader
            GLIShaderData::GetInstance()->SetSelectedShader(selectedItem);

            //Open the shader on activate
            if(nData->code == LVN_ITEMACTIVATE)
            {
              GLIShaderData::GetInstance()->OpenShader(); 
            }
          }
          return TRUE;
        }

        //If a column was clicked
        if(nData->code == LVN_COLUMNCLICK)
        {
          //Flag to the UI to re-order based on the column click
          LPNMLISTVIEW columnClick = (LPNMLISTVIEW)lParam;
          GLIShaderData::GetInstance()->SetColumnSort(columnClick->iSubItem);
          return TRUE;
        }
      }

      //Process tooltips
      if (nData->code == TTN_NEEDTEXT)
      { 
        int idCtrl = GetDlgCtrlID((HWND)nData->idFrom); 
        LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT) lParam; 

        switch (idCtrl)
        { 
          case ID_REFRESH: 
              lpttt->lpszText = "Refresh the shader list"; 
              return TRUE; 

          case IDC_SHADERLIST: 
              lpttt->lpszText = "List of shaders currently available"; 
              return TRUE; 

          case IDC_SHOWSHADER: 
              lpttt->lpszText = "Flash the selected shader in the render window"; 
              return TRUE; 

          case IDC_BREAKSHADER: 
              lpttt->lpszText = "Throw a debugger breakpoint when the selected shader is used. (If run from an IDE)"; 
              return TRUE; 

          case ID_OPENSHADER:
              lpttt->lpszText = "Open the selected shader for editing"; 
              return TRUE; 

          case ID_REVERTSHADER:
              lpttt->lpszText = "Revert the selected shader to undo any edits"; 
              return TRUE; 
        } 
      } 

      break;
    }

    //Process commands
    case WM_COMMAND:
    {
      switch(LOWORD(wParam))
      {
        case ID_REFRESH:
        {
          //Refresh the shader list
          if(HIWORD(wParam) == BN_CLICKED &&
             GLIShaderData::GetInstance())
          {
            GLIShaderData::GetInstance()->RefreshShaderList();
          }
          return TRUE;
        }
        case IDC_SHOWSHADER:
        {
          if(HIWORD(wParam) == BN_CLICKED &&
             GLIShaderData::GetInstance())
          {
            //Get the state of the check box
            bool switchState = false;
            if(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED)
            {
              switchState = true;
            }
            
            GLIShaderData::GetInstance()->SetShowShader(switchState);
          }
          return TRUE;
        }
        case IDC_BREAKSHADER:
        {
          if(HIWORD(wParam) == BN_CLICKED &&
             GLIShaderData::GetInstance())
          {
            //Get the state of the check box
            bool switchState = false;
            if(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED)
            {
              switchState = true;
            }
            
            GLIShaderData::GetInstance()->SetBreakShader(switchState);
          }
          return TRUE;
        }
        case ID_OPENSHADER:
        {
          if(HIWORD(wParam) == BN_CLICKED &&
             GLIShaderData::GetInstance())
          {
            GLIShaderData::GetInstance()->OpenShader(); 
          }
          return TRUE;
        }
        case ID_REVERTSHADER:
        {
          if(HIWORD(wParam) == BN_CLICKED &&
             GLIShaderData::GetInstance())
          {
            GLIShaderData::GetInstance()->RevertShader(); 
          }
          return TRUE;
        }
      }

      break;
    }


  }

  return FALSE;
}

