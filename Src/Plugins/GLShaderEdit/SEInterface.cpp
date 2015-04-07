/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "SEInterface.h"
#include "ShaderEditManager.h"
#include "SEInterfaceCommands.h"
#include "GLFrameBufferCache.h"
#include <MiscUtils.h>
#include <NetworkUtils.h>
#include <CommonErrorLog.h>

USING_ERRORLOG

// The singleton instance.
SEInterface * SEInterface::seiInstance = NULL;

//DLL Instance
extern HINSTANCE dllInstance;

//Path to the dll
extern string dllPath;

//Prototype of the window callback
LRESULT PASCAL SEInterface_WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


struct CommandMsg
{
  CommandMsg();
  CommandType commandType; // The type of the command 
  uint        commandUID;  // The unique ID passed with the command
  string      commandData; // The raw command data

  HWND        commandWnd;  // The window that requested this command
};

static vector<CommandMsg>  cachedCommands; // Command that have been requested but not yet processed 

static char SEInterface_ClassName[] = "GLI_SEInterface_Win_Class";

///////////////////////////////////////////////////////////////////////////////
//
CommandMsg::CommandMsg():
commandType(CT_None),
commandUID(0),
commandWnd(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////
//
SEInterface::SEInterface(ShaderEditManager *newManager, InterceptPluginCallbacks *callBacks):
manager(newManager),
gliCallbacks(callBacks),
messageWin(NULL),
editProcessHandle(NULL),
editorWin(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////
//
SEInterface::~SEInterface()
{
  //Destroy the window if necessary
  if(messageWin)
  {
    if(!DestroyWindow(messageWin))
    {
      LOGERR(("~SEInterface - Error destroying message window"));
    }
    messageWin = NULL;

    if(!UnregisterClass(SEInterface_ClassName, dllInstance))
    {
      LOGERR(("~SEInterface - Error unregistering window class"));
    }
  }

  //Flag to no longer "show" the shader
  manager->SetShowShader(0,USID_Undefined); 

  //Flag to no longer "break-on" the shader
  manager->SetBreakShader(0,USID_Undefined); 

  //Remove any debugging flags from all shaders
  UniqueShaderID::ClearAllDebugFlags();

  //Close the process handle
  if(editProcessHandle)
  {
    CloseHandle(editProcessHandle);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool SEInterface::Init()
{
  //Clar any possible cached commands
  cachedCommands.clear();

  //Create the class of the interface window
  WNDCLASS wndclass;
  wndclass.style = 0;
  wndclass.lpfnWndProc = SEInterface_WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = dllInstance;
  wndclass.hIcon = 0;
  wndclass.hCursor = NULL;
  wndclass.hbrBackground = NULL;
  wndclass.lpszMenuName = 0;
  wndclass.lpszClassName = SEInterface_ClassName;
  if (!::RegisterClass(&wndclass))
  {
    LOGERR(("SEInterface::Init - unable to register window class"));
    return false;
  }

  //Create the window
  messageWin = ::CreateWindow(
                      SEInterface_ClassName,
                      SEInterface_ClassName,
                      0,
                      0, 0, 0, 0,
                      0,
                      0,
                      dllInstance,
                      0);
  if (!messageWin)
  {
    UnregisterClass(SEInterface_ClassName, dllInstance);
    LOGERR(("SEInterface::Init - Unable to create message window"));
    return false;
  }

  //Create a process startup structures
  STARTUPINFO si;
  PROCESS_INFORMATION processID;

  //Create the editor and show it
  memset(&si, 0, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  si.dwFlags = STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_SHOW;

  //Pass the message window handle to the editor
  string cmdLine;
  string exeName = dllPath + "GLISciTE.exe";
  // New tool command line:
  // StringPrintF(cmdLine, "%s -glintercepthwnd=%u", exeName.c_str(), messageWin); 
  StringPrintF(cmdLine, " -glintercept.hwnd=%u", messageWin); 

  //Create the new editor as a seperate process
  if(!::CreateProcess(exeName.c_str(),
                      (char*)cmdLine.c_str(),
                      NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP,
                      NULL, NULL, &si, &processID))
  {
    LOGERR(("SEInterface::Init - Unable to create editor process"));
    return false;
  }

  //Assign the process handle
  editProcessHandle = processID.hProcess;

  //Close thread handle. 
  CloseHandle(processID.hThread);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void SEInterface::ProcessCachedMessages()
{
  //Check that the instance is available
  if(!seiInstance || !messageWin)
  {
    return;
  }

  //Peek at any messages for our window 
  MSG processMsg;
  //DT_TODO: Debug and check that having a while loop here instead of an if will cause any problems....(lag? infinite loop?)
  //         (perehaps have a limit of 100 iterations?)
  while(PeekMessage(&processMsg, messageWin, NULL, NULL, PM_REMOVE))
  {
    DispatchMessage(&processMsg);
  }

  //Check if the window is still active
  if(cachedCommands.size() == 0)
  {
    //Get if the editor process has crashed
    DWORD exitCode;
    if(GetExitCodeProcess(editProcessHandle, &exitCode) &&
       exitCode != STILL_ACTIVE)
    {
      LOGERR(("SEInterface::ProcessCachedMessages - Crashed editor process?"));

      //Destroy this class
      DestroyInstance();
      return;
    }
  }

  //Process any commands
  for(uint i=0; i<cachedCommands.size(); i++)
  {
    const CommandMsg &currCmd = cachedCommands[i];

    //Process the init command first to get the editor window
    if(editorWin == NULL && currCmd.commandType == CT_Init)
    {
      //Assign the communication window
      editorWin = currCmd.commandWnd;
    }

    //Check that the command came from the known window
    if(editorWin != NULL && currCmd.commandWnd == editorWin)
    {
      switch(currCmd.commandType)
      {
        //Ignore commands
        case(CT_None):
        case(CT_Init):
        case(CT_Ping):
          break;

        //Get a listing of all the current shaders
        case(CT_ShaderList):
          SendShaderList(currCmd.commandUID);
          break;

        //Get the source of a particular shader
        case(CT_ShaderSource):
          SendShaderSource(currCmd.commandUID, currCmd.commandData);
          break;

        //Compile a shader
        case(CT_CompileShader):
          CompileShaderSource(currCmd.commandUID, currCmd.commandData);
          break;

        //Flag a shader as shown
        case(CT_ShowShader):
          ShowShader(currCmd.commandData);
          break;

        //Revert the shader
        case(CT_RevertShader):
          RevertShader(currCmd.commandData);
          break;

        //Flag to break on a shader
        case(CT_BreakShader):
          BreakShader(currCmd.commandData);
          break;

        //Set a shader as being debugged
        case(CT_SetDebugShader):
          SetDebugShader(true, currCmd.commandData);
          break;

        //Un-set a shader as being debugged
        case(CT_UnsetDebugShader):
          SetDebugShader(false, currCmd.commandData);
          break;

        //Shader Init should only be a outgoing command
        case(CT_DebugShaderInit):
          LOGERR(("SEInterface::ProcessCachedMessages - Shader Init Command?"));
          break;

        //Start debugging the sent pixel location
        case(CT_DebugShaderPixel):
          SetPixelDebug(currCmd.commandData);
          break;
        case(CT_DebugShaderLine):
          break;

        //Flag that shader debugging is complete
        case(CT_DebugShaderDone):
          manager->EndShaderDebug();
          break;

        //Destroy this message handler
        case(CT_Shutdown):
          //Remove all cached commands
          cachedCommands.clear();

          //Destroy this class
          DestroyInstance();
          return;
          break;

        default:
          LOGERR(("SEInterface::ProcessCachedMessages - Unknown command"));
      }
    }
    else
    {
      LOGERR(("SEInterface::ProcessCachedMessages - Received command from invalid window?"));
    }
  }

  //Remove all cached commands
  cachedCommands.clear();

}



///////////////////////////////////////////////////////////////////////////////
//
void SEInterface::SendShaderList(uint cmdUID)
{
  //Get the sahder list
  const vector<UniqueShaderID *> &shaderList = UniqueShaderID::GetUniqueShaderIDList();  

  string shaderListStr;
  shaderListStr.reserve(2000);

  string buffer;

  //Loop for all the shaders and append the data to the string
  for(uint i=0; i<shaderList.size(); i++)
  {
    const UniqueShaderID * currShader = shaderList[i];

    //Determine the shader type
    string strShaderType;
    switch(currShader->GetShaderType())
    {
      case(USID_GLSLProgram):
        strShaderType = "GLSL";
        break;

      case(USID_ARB_VP):
        strShaderType = "ARBVP";
        break;

      case(USID_ARB_FP):
        strShaderType = "ARBFP";
        break;

      case(USID_NV_VP):
        strShaderType = "NVVP";
        break;

      case(USID_NV_FP):
        strShaderType = "NVFP";
        break;

      default:
        //Ignore all other types
        break;
    };

    //Ony add if it was a valid type
    if(strShaderType.length() > 0)
    {
      StringPrintF(buffer,"%u:%u:%s:%u:*",
                    currShader->GetUniqueIndex(),currShader->GetShaderOpenGLID(),
                    strShaderType.c_str(), currShader->GetLastFrameUsed());
      shaderListStr += buffer;
    }
  }

  //Send the message
  SendWinMessage(editorWin, cmdUID, shaderListStr);
}

///////////////////////////////////////////////////////////////////////////////
//
void SEInterface::SendShaderSource(uint cmdUID, const string &strShaderID)
{
  string shaderSource;

  //Get the shader ID
  uint shaderUIDIndex = atoi(strShaderID.c_str());

  //Look up the string value
  const UniqueShaderID * shaderDataID = UniqueShaderID::FindUniqueShaderID(shaderUIDIndex);
  if(shaderDataID)
  {
    //Get any override source first
    shaderSource = shaderDataID->GetOverrideShaderSource();
    if(shaderSource.size() == 0)
    {
      //Retrieve the origional shader source
      shaderSource = shaderDataID->GetShaderSource();
      if(shaderSource.size() == 0)
      {
        shaderSource = "<No current source>";
      }
    }
  }
  else
  {
    //Flag that the shader source no longer exists
    shaderSource = "The specified shader ";
    shaderSource += strShaderID;
    shaderSource += " no longer exists in the program - refresh";
  }

  //Send the message
  SendWinMessage(editorWin, cmdUID, shaderSource);

}

///////////////////////////////////////////////////////////////////////////////
//
void SEInterface::CompileShaderSource(uint cmdUID, const string &shaderData)
{
  string shaderLog;
  string shaderSrc;
  string strShaderID;

  //Extract the first digits
  int endPos = (int)shaderData.find(":");
  if(endPos == string::npos)
  {
    //An error message
    SendWinMessage(editorWin, cmdUID, "CompileShaderSource - No shader ID");
    return;
  }

  //Extracte the ID and the shader source
  strShaderID = shaderData.substr(0, endPos);
  shaderSrc   = shaderData.substr(endPos+1, shaderData.length() - endPos - 1);

  //Get the shader ID
  uint shaderUIDIndex = atoi(strShaderID.c_str());

  //Look up the string value
  if(manager->CompileShader(shaderUIDIndex, shaderSrc, shaderLog))
  {
    shaderLog = string(" Success\n") + shaderLog;
  }
  else
  {
    shaderLog = string(" Error\n") + shaderLog;
  }

  //Send the message
  SendWinMessage(editorWin, cmdUID, shaderLog);
}

///////////////////////////////////////////////////////////////////////////////
//
void SEInterface::ShowShader(const string &shaderID)
{
  //Get the shader ID
  uint shaderUIDIndex = 0;
  if(shaderID.length() > 0)
  {
    shaderUIDIndex = atoi(shaderID.c_str());
  }

  //Look up the string value
  const UniqueShaderID * shaderDataID = NULL;
  if(shaderUIDIndex > 0)
  {
    shaderDataID = UniqueShaderID::FindUniqueShaderID(shaderUIDIndex);
  }

  //Flag to the manager the new ID
  if(shaderDataID)
  {
    manager->SetShowShader(shaderUIDIndex,shaderDataID->GetShaderType()); 
  }
  else
  {
    manager->SetShowShader(0,USID_Undefined); 
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void SEInterface::BreakShader(const string &shaderID)
{
  //Get the shader ID
  uint shaderUIDIndex = 0;
  if(shaderID.length() > 0)
  {
    shaderUIDIndex = atoi(shaderID.c_str());
  }

  //Look up the string value
  const UniqueShaderID * shaderDataID = NULL;
  if(shaderUIDIndex > 0)
  {
    shaderDataID = UniqueShaderID::FindUniqueShaderID(shaderUIDIndex);
  }

  //Flag to the manager the new ID
  if(shaderDataID)
  {
    manager->SetBreakShader(shaderUIDIndex,shaderDataID->GetShaderType()); 
  }
  else
  {
    manager->SetBreakShader(0,USID_Undefined); 
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void SEInterface::SetDebugShader(bool flag, const string &shaderID)
{
  //Get the shader ID
  uint shaderUIDIndex = 0;
  if(shaderID.length() > 0)
  {
    shaderUIDIndex = atoi(shaderID.c_str());
  }

  //Look up the string value
  UniqueShaderID * shaderDataID = NULL;
  if(shaderUIDIndex > 0)
  {
    shaderDataID = UniqueShaderID::FindUniqueShaderID(shaderUIDIndex);
  }

  //Set the shader debug flag
  if(shaderDataID)
  {
    shaderDataID->SetShaderDebugged(flag);
  }
  else
  {
    LOGERR(("SEInterface::SetDebugShader - Unable to find shader %u",shaderUIDIndex));  
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void SEInterface::RevertShader(const string &shaderID)
{
  //DT_TODO: Ensure to exit debugging mode before reverting a shader...

  //Get the shader ID
  uint shaderUIDIndex = 0;
  if(shaderID.length() > 0)
  {
    shaderUIDIndex = atoi(shaderID.c_str());
  }

  //Look up the string value
  UniqueShaderID * shaderDataID = NULL;
  if(shaderUIDIndex > 0)
  {
    shaderDataID = UniqueShaderID::FindUniqueShaderID(shaderUIDIndex);
  }
  
  //Return if the shader is not found
  if(!shaderDataID)
  {
    return;
  }

  //If the type is GLSL, simply detach the substitute
  if(shaderDataID->GetShaderType() == USID_GLSLProgram)
  {
    //Reset the override shader source
    shaderDataID->SetOverrideShaderSource("");

    //Assign a empty substitute
    shaderDataID->SetSubstituteShader(SubShaderGLSLRef());
    return;
  }

  //If a ASM type shader, compile it with the origional source
  string shaderSrc = shaderDataID->GetShaderSource();
  string shaderLog;

  //Look up the string value
  if(!manager->CompileShader(shaderUIDIndex, shaderSrc, shaderLog))
  {
    LOGERR(("SEInterface::RevertShader - Error compiling shader with origional source?"));
    return;
  }

  //Reset the override shader source
  shaderDataID->SetOverrideShaderSource("");
}


///////////////////////////////////////////////////////////////////////////////
//
bool SEInterface::SendStartDebugging(uint debugShaderUID, uint origShaderID, ShaderDebugGLSL * shaderDebug)
{
  // Update this code below if re-enabling shader debugging code
  return false;
/*
  uint i;
  NetworkBufferSend networkBuffer(30000);

  //Get the uniform data to send
  const UniformDataArray & sendUniformData = shaderDebug->GetUniformData();

  //Add the shader UID 
  networkBuffer.Add(debugShaderUID);

  //Add the OpenGL ID and shader source 
  // (NOTE: If a override program is in use, the source comes from the override. However, the
  //        OpenGL ID is still the ID of the origional program)
  networkBuffer.Add(origShaderID);
  networkBuffer.Add(shaderDebug->GetProgramSource());

  //Add all the uniform data to the buffer
  // (start with the counter for the number of uniforms)
  networkBuffer.Add(sendUniformData.size());
  for(i=0; i<sendUniformData.size(); i++)
  {
    //Add the name
    networkBuffer.Add(sendUniformData[i].name);
    
    //Add the type, size, numType elements, isFloatType 
    networkBuffer.Add(sendUniformData[i].type);
    networkBuffer.Add(sendUniformData[i].numTypeElements);
    networkBuffer.Add((uint)sendUniformData[i].isFloatType);

    //If float array
    if(sendUniformData[i].isFloatType)
    {
      //Add the number of floats
      networkBuffer.Add(sendUniformData[i].floatUniformData.size(), &sendUniformData[i].floatUniformData[0]);
    }
    else
    {
      //Add the number of ints
      networkBuffer.Add(sendUniformData[i].intUniformData.size(), &sendUniformData[i].intUniformData[0]);
    }
  }

  //Add the frame buffer data to send (can be very big!)
  const GLFrameBufferCache * frameBufferCache = shaderDebug->GetFrameBufferCache();
  if(frameBufferCache == NULL)
  {
    LOGERR(("SEInterface::SendStartDebugging - No frame buffer cache."));
    return false;
  }

  //Get the frame buffer data
  const vector<FrameBufferData> & frameData = frameBufferCache->GetFrameBufferData();
  
  //Add the number of frame buffers
  networkBuffer.Add((uint)frameData.size());
  
  //Loop and add each frame buffer
  for(i=0; i<frameData.size(); i++)
  {
    //Add the data about the buffer
    networkBuffer.Add(frameData[i].bufferType);
    networkBuffer.Add(frameData[i].drawBuffer);

    networkBuffer.Add(frameData[i].bufferWidth);
    networkBuffer.Add(frameData[i].bufferHeight);
    networkBuffer.Add(frameData[i].numPixelValues);

    //Add the pre/post buffer data
    networkBuffer.Add(frameData[i].preBuffer.size(),  &frameData[i].preBuffer[0]);
    networkBuffer.Add(frameData[i].postBuffer.size(), &frameData[i].postBuffer[0]);
  }

  //The send data should be a multiple of 4
  if(networkBuffer.GetBufferSize() % 4 != 0)
  {
    LOGERR(("SEInterface::SendStartDebugging - Error calculating the send data size."));
    return false;
  }

  //Check that there is a shader editor active and init (sanity check)
  if(editorWin == NULL)
  {
    LOGERR(("SEInterface::SendStartDebugging - No shader editor window?."));
    return false;
  }

  //Send the debug begin message
  SendWinMessage(editorWin, CT_DebugShaderInit, networkBuffer.GetBufferSize(), networkBuffer.GetBuffer());

  return true;
  */
}

///////////////////////////////////////////////////////////////////////////////
//
void SEInterface::SetPixelDebug(const string &debugPixelStr)
{
  string shaderDebugLog;

  // DT_TODO: Complete
  // Extract the debug position

  // Extract the overdraw position (does not have to exist)

  // If the over-draw position does not exist, determine the number of overdraws for the pixel.

     // If the pixel has overdraw, send to the dialog a list of the pixel values that overdraw


  // Start debugging the pixel
  if(!manager->SetPixelDebug(0, 0, 0, shaderDebugLog))
  {
    // Report to the debugger that the pixel could not be debugged
  }

  // On success, pass back a list of valid debug lines (with number of times hit) and  
  // the flow of the shader by line numbers

  // Send the debugger an error message on failure
}

///////////////////////////////////////////////////////////////////////////////
//
bool SEInterface::SendWinMessage(HWND sendWnd, uint cmdUID, const string &data) const
{
  //Point to the string if it has some data
  if(data.size() > 0)
  {
    return SendWinMessage(sendWnd, cmdUID, (int)data.size(), (void*)data.c_str()); 
  }
  else
  {
    return SendWinMessage(sendWnd, cmdUID, 0, NULL); 
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool SEInterface::SendWinMessage(HWND sendWnd, uint cmdUID, int sendDataSize, const void * sendDataBuffer) const
{
  COPYDATASTRUCT sendData;
  sendData.dwData = cmdUID;
  sendData.cbData = sendDataSize;
  sendData.lpData = (void*)sendDataBuffer;

  //Send the message and time out if not responded to in 3 seconds
  ULONG_PTR result;
  if(!SendMessageTimeout(sendWnd, WM_COPYDATA, 
                         reinterpret_cast<WPARAM>(messageWin), reinterpret_cast<LPARAM>(&sendData),
                         SMTO_ABORTIFHUNG | SMTO_NORMAL, 3000, &result))
  {
    LOGERR(("SEInterface::SendWinMessage - Error sending message"));
    return false;
  }

  return true;
}



///////////////////////////////////////////////////////////////////////////////
//
LRESULT PASCAL SEInterface_WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) 
{
  //Only handle copy data messages
  if (iMessage == WM_COPYDATA)
  {
    CommandMsg newCommand;
    
    //Extract the command type and UID
    PCOPYDATASTRUCT srcData = (PCOPYDATASTRUCT)lParam;
    newCommand.commandType = (CommandType)LOWORD(srcData->dwData);
    newCommand.commandUID  = (uint)srcData->dwData; // Possible 64 bit issues

    //Assign the calling windows' ID
    newCommand.commandWnd = (HWND)wParam;

    //Copy the command data
    if(srcData->cbData > 0 && srcData->lpData)
    {
      newCommand.commandData = string((const char*)srcData->lpData, srcData->cbData);
    }

    //Check the command type
    if(newCommand.commandType >= CT_Max || newCommand.commandType < 0)
    {
      LOGERR(("SEInterface_WndProc - Invalid command"));
      return FALSE;
    }

    //If there is a editor interface (mostly a sanity check)
    if(SEInterface::GetInstance())
    {
      //Add the command to the cached command array
      cachedCommands.push_back(newCommand);
    }

    return TRUE;
  }

  return ::DefWindowProc(hWnd, iMessage, wParam, lParam);
}




///////////////////////////////////////////////////////////////////////////////
//
SEInterface * SEInterface::CreateInstance(ShaderEditManager *manager, InterceptPluginCallbacks *callBacks)
{
  //If the instance already exists, return it
  if(seiInstance)
  {
    return seiInstance;  
  }

  //Create an init the new interface
  SEInterface * newInterface = new SEInterface(manager, callBacks);
  if(!newInterface->Init())
  {
    LOGERR(("SEInterface::CreateInstance - Error creating the shader editor interface"));
    return false;
  }

  //Assign the interface an return
  seiInstance = newInterface;

  return seiInstance;
}

///////////////////////////////////////////////////////////////////////////////
//
SEInterface * SEInterface::GetInstance()
{
  return seiInstance;
}


///////////////////////////////////////////////////////////////////////////////
//
void SEInterface::DestroyInstance()
{
  //Destroy if allocated
  if(seiInstance)
  {
    delete seiInstance;
    seiInstance = NULL;
  }
}





