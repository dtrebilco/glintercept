
//Turn off truncation warnings
#pragma warning(disable: 4786)

#include "SciTE.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

//Manually include the used files 
// (as the main sciTE win file cannot be included with MFC support on)
#include "Platform.h"
#include "PropSet.h"
#include "Accessor.h"
#include "WindowAccessor.h"
#include "Scintilla.h"
#include "Extender.h"
#include "FilePath.h"
#include "SciTEBase.h"

#include "PropTree\\StdAfx.h"
#include "PropTree\\PropTree.h"
#include "PropTree\\PropTreeItemGLI.h"
#include "GLIShaderDebug.h"

#include "..\\..\\..\\..\\Common\\NetworkUtils.h"
#include "..\\..\\..\\..\\MainLib\\GLDefines.h"

//Callback for the shader debug dialog
BOOL CALLBACK ShaderDebugMsg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//The single instance of the shader debug interface
GLIShaderDebug *debugInstance = NULL;

///////////////////////////////////////////////////////////////////////////////
//
FrameBufferData::FrameBufferData():
bufferType(0),
drawBuffer(GL_NONE),
bufferWidth(0),
bufferHeight(0),
numPixelValues(0)
{
}

///////////////////////////////////////////////////////////////////////////////
//
GLIShaderDebug::UniformData::UniformData():
name(""),
type(0),
numTypeElements(0),
isFloatType(false)
{

}

///////////////////////////////////////////////////////////////////////////////
//
GLIShaderDebug::GLIShaderDebug(SciTEBase *newScite):
scite(newScite),
dialogWindow(NULL),
propTree(NULL),

debugState(DBS_None),

debugUID(0),
debugGLID(0),
debugSource("")
{
  //Create the property tree
  propTree = new CPropTree();

  //Reserve some frame buffer data space to avoid expensive resizes
  frameBufferArray.reserve(20);
}

///////////////////////////////////////////////////////////////////////////////
//
GLIShaderDebug::~GLIShaderDebug()
{
  //Let the parent window handle destroying the dialog window

  //Set the global variable to NULL
  debugInstance = NULL;

  //Delete the property control
  if(propTree)
  {
    //Must destroy the window first
    propTree->DestroyWindow();
    delete propTree;
    propTree = NULL;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderDebug::Init(HINSTANCE hInstance, HWND parentWnd, SciTEBase *newScite)
{
  //If already init, return false;
  if(debugInstance != NULL)
  {
    return false;
  }

  //Assign the MFC instance handle
  afxCurrentInstanceHandle = hInstance;
  AfxSetResourceHandle(hInstance);

  //Create the instance  
  debugInstance = new GLIShaderDebug(newScite);

  //Create the dialog window
  debugInstance->dialogWindow = ::CreateDialog(hInstance, MAKEINTRESOURCE(IDD_GLISHADERDEBUG), parentWnd, ShaderDebugMsg); 
  if(!debugInstance->dialogWindow)
  {
    newScite->OutputAppendString("== Unable to init the GLIntercept shader debug window ==\n");
    delete debugInstance;
    debugInstance = NULL;
    return false;
  }

  //Position window 
  HWND parent = GetParent(debugInstance->dialogWindow);
  if(parent)
  {
    RECT parentPos;
    RECT dialogSize;
    if(GetWindowRect(parent, &parentPos) &&
       GetWindowRect(debugInstance->dialogWindow, &dialogSize))
    {
      //Position the window to the right of the host window
      int newW = dialogSize.right  - dialogSize.left;
      int newH = dialogSize.bottom - dialogSize.top;

      int newX = parentPos.right - (newW/4) + 50;
      int newY = parentPos.top   + 160;

      //Set the new window position (Don't activate it)
      SetWindowPos(debugInstance->dialogWindow, HWND_TOP,
                   newX, newY, newW, newH, 
                   SWP_NOACTIVATE);
    }
  }

  //Show the window
  //ShowWindow(debugInstance->dialogWindow, SW_SHOW);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
GLIShaderDebug *GLIShaderDebug::GetInstance()
{
  return debugInstance;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::InitPropertyControl(HWND initDialogWindow)
{
  //Init the dialog window variable
  dialogWindow = initDialogWindow;

  //Retrieve the area of the shader values control
  HWND renderAreaWnd = GetDlgItem(dialogWindow, IDC_SHADERVALUES);
  if(renderAreaWnd == NULL)
  {
    return;
  }

  RECT  clientArea;
  GetWindowRect(renderAreaWnd, &clientArea);

  POINT clientPoint = {clientArea.left, clientArea.top};  
  ScreenToClient(dialogWindow, &clientPoint);

  //Create the client area rect for the properties
  RECT newClientArea;
  newClientArea.left = clientPoint.x;
  newClientArea.top  = clientPoint.y; 
  newClientArea.right  = clientPoint.x + (clientArea.right  - clientArea.left);
  newClientArea.bottom = clientPoint.y + (clientArea.bottom - clientArea.top );

  //Create CPropTree control
  propTree->Create(WS_CHILD|WS_VISIBLE|PTS_NOTIFY, newClientArea, dialogWindow, IDC_SHADERPROPERTY);
  propTree->ShowInfoText(FALSE);

  //Set the initial display of watches/uniforms
  UpdateVariableDisplay();
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderDebug::DebugBegin(uint bufferSize, void *buffer)
{
  uint i;

  //Check the debug state
  if(debugState != DBS_None)
  {
    scite->OutputAppendString("== Unable to switch to debug mode. Already debugging? ==\n");
    return false;
  }

  //Check the input data
  if(bufferSize == 0 || buffer == NULL)
  {
    return false;
  }

  //Clear all existing uniforms/frame buffer data
  uniformValues.clear();
  frameBufferArray.clear();

  //Read the input data
  NetworkBufferReceive bufferData(bufferSize, buffer);

  //Get the debug shader UID/GL ID  
  if(!bufferData.Get(debugUID) ||
     !bufferData.Get(debugGLID))
  {
    return false;
  }

  //Get the shader source
  if(!bufferData.Get(debugSource))
  {
    return false;
  }

  //Get the number of uniforms
  uint numUniforms = 0;
  if(!bufferData.Get(numUniforms))
  {
    return false;
  }

  //Loop and get each uniform 
  for(i=0; i<numUniforms; i++)
  {
    UniformData newUniform;
    uint isFloat;

    //Get the main uniform data
    if(!bufferData.Get(newUniform.name) ||
       !bufferData.Get(newUniform.type) ||
       !bufferData.Get(newUniform.numTypeElements) ||
       !bufferData.Get(isFloat))
    {
      return false;
    }

    //Assign if it is a float or not
    if(isFloat > 0)
    {
      newUniform.isFloatType = true;
      if(!bufferData.Get(newUniform.floatUniformData))
      {
        return false;
      }

      //Return now if there is an uneven number of variables
      if(newUniform.floatUniformData.size() % newUniform.numTypeElements != 0)
      { 
        return false;
      }
    }
    else
    {
      newUniform.isFloatType = false;
      if(!bufferData.Get(newUniform.intUniformData))
      {
        return false;
      }

      //Return now if there is an uneven number of variables
      if(newUniform.intUniformData.size() % newUniform.numTypeElements != 0)
      { 
        return false;
      }
    }

    //Add the uniform data
    uniformValues.push_back(newUniform);
  }

  //Get the number of frame buffers
  uint numFrameBuffers = 0;
  if(!bufferData.Get(numFrameBuffers))
  {
    return false;
  }

  //Loop and get each frame buffer
  for(i=0; i<numFrameBuffers; i++)
  {
    //Add a new item
    frameBufferArray.push_back(FrameBufferData());

    //Get a reference to it (faster than copying all this buffer data twice)
    FrameBufferData &newFrameBuffer = frameBufferArray.back();

    //Get the main frame buffer data
    if(!bufferData.Get(newFrameBuffer.bufferType) ||
       !bufferData.Get(newFrameBuffer.drawBuffer) ||
       !bufferData.Get(newFrameBuffer.bufferWidth) ||
       !bufferData.Get(newFrameBuffer.bufferHeight) ||
       !bufferData.Get(newFrameBuffer.numPixelValues) ||
       !bufferData.Get(newFrameBuffer.preBuffer) ||
       !bufferData.Get(newFrameBuffer.postBuffer))
    {
      frameBufferArray.pop_back();
      return false;
    }

    //Check the size of the data
    if(newFrameBuffer.preBuffer.size() != newFrameBuffer.postBuffer.size() ||
       newFrameBuffer.preBuffer.size() != (newFrameBuffer.bufferWidth * 
                                           newFrameBuffer.bufferHeight * 
                                           newFrameBuffer.numPixelValues))
    {
      scite->OutputAppendString("== Invalid frame buffer data ==\n");
      frameBufferArray.pop_back();
      return false;
    }
  }


  //Check for any remaining network data
  if(bufferData.GetBufferSizeLeft() > 0)
  {
    scite->OutputAppendString("== Extra debug start data found in network buffer ==\n");
  }

  //Flag that debug mode has started
  debugState = DBS_Init;

  //Update the watch/uniform display
  UpdateVariableDisplay();

  //Show the window
  ShowWindow(dialogWindow, SW_SHOW);

  //Ensure the source that is being debugged is open
  OpenDebugSource();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLIShaderDebug::DebugEnd()
{
  //Check the debug state
  if(debugState == DBS_None)
  {
    scite->OutputAppendString("== Cannot exit debug mode when not debugging ==\n");
    return false;
  }


  //Flag that debug mode has ended


  //Hide the window
  ShowWindow(dialogWindow, SW_HIDE);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::OpenDebugSource()
{
  //Create a filename from the source ID's
  SString debugFileName(debugUID);
  debugFileName += "_";
  debugFileName += SString(debugGLID);

  //Can only currently debug GLSL shaders
  debugFileName += ".glsl";
  
  //Make the filename lower case
  debugFileName.lowercase();

  //Open using the current "file name"
  scite->OpenBuffer(debugFileName.c_str(), debugSource.c_str());
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::UpdateVariableDisplay()
{
  //Delete all existing items
  propTree->DeleteAllItems();

  // Create the watch variables root item
  CPropTreeItem* pRoot;
  pRoot = propTree->InsertItem(new CPropTreeItem());
  pRoot->SetLabelText(_T("Watch values"));
  pRoot->Expand(); 

  //Loop for all watch values and add them

  //Add a empty watch value to enable a new one to be added
  CPropTreeItem* pItem;
  pItem = propTree->InsertItem(new CPropTreeItem(), pRoot);
  pItem->SetLabelText(_T(""));
  pItem->SetInfoText(_T(""));
  
  //Create the uniforms root item
  pRoot = propTree->InsertItem(new CPropTreeItem());
  pRoot->SetLabelText(_T("Uniforms"));
  pRoot->Expand(); 

  //Loop for all uniforms and add values of the appropiate type
  for(uint i=0; i<uniformValues.size(); i++)
  { 
    AddVariableDisplay(pRoot, uniformValues[i]);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::AddVariableDisplay(CPropTreeItem* pRoot, const UniformData &uniformValue)
{
  //Create the initial static item
  CPropTreeItemGLI* varRoot;
  varRoot = (CPropTreeItemGLI*)propTree->InsertItem(new CPropTreeItemGLI(), pRoot);

  //Assign the name
  varRoot->SetLabelText(uniformValue.name.c_str());

  //Check the data
  if(uniformValue.isFloatType)
  {
    //Invalid data?
    if(uniformValue.floatUniformData.size() % uniformValue.numTypeElements != 0)
    {
      varRoot->SetItemValue("Invalid Data");
      return;
    }

    //Determine if the color block is rendered
    bool renderColor = false;
    if(uniformValue.numTypeElements == 3 ||
       uniformValue.numTypeElements == 4)
    {
      renderColor = true;
    }

    //Determine if it is a matrix type
    bool isMatrix = false;
    if(uniformValue.type == GL_FLOAT_MAT2 ||
       uniformValue.type == GL_FLOAT_MAT3 ||
       uniformValue.type == GL_FLOAT_MAT4)
    {
      isMatrix = true;
    }

    //Get the array size
    uint arraySize = uniformValue.floatUniformData.size() / uniformValue.numTypeElements;
    if(arraySize == 0)
    {
      varRoot->SetItemValue("<empty>");
      return;
    }

    //If there is more than one array item, nest them
    if(arraySize > 1)
    {
      //Update the main label name
      CString labelName;
      labelName.Format("%s[%u]", uniformValue.name.c_str(), arraySize); 
      varRoot->SetLabelText(labelName);

      //Loop an add each item as a sub item
      for(uint i=0; i<arraySize; i++)
      {
        CPropTreeItemGLI* arrayItem;
        arrayItem = (CPropTreeItemGLI*)propTree->InsertItem(new CPropTreeItemGLI(), varRoot);
        
        //Set the name
        labelName.Format("%s[%u]", uniformValue.name.c_str(), i); 
        arrayItem->SetLabelText(labelName);

        //Add the description
        if(isMatrix)
        {
          AddVariableMatrixDisplay(arrayItem, uniformValue, i*uniformValue.numTypeElements);
        }
        else
        {
          arrayItem->SetItemValue(uniformValue.numTypeElements, &uniformValue.floatUniformData[i*uniformValue.numTypeElements], renderColor);
        }
      }
    }
    else
    {
      //Handle matrices seperately
      if(isMatrix)
      {
        AddVariableMatrixDisplay(varRoot, uniformValue, 0);
      }
      else
      {
        varRoot->SetItemValue(uniformValue.floatUniformData.size(), &uniformValue.floatUniformData[0], renderColor);
      }
    }
  }
  else
  {
    //Invalid data?
    if(uniformValue.intUniformData.size() % uniformValue.numTypeElements != 0)
    {
      varRoot->SetItemValue("Invalid Data");
      return;
    }

    //Determine if it is a boolean type
    bool isBool = false;
    if(uniformValue.type == GL_BOOL ||
       uniformValue.type == GL_BOOL_VEC2 ||
       uniformValue.type == GL_BOOL_VEC3 ||
       uniformValue.type == GL_BOOL_VEC4 )
    {
      isBool = true;
    }

    //Get the array size
    uint arraySize = uniformValue.intUniformData.size() / uniformValue.numTypeElements;
    if(arraySize == 0)
    {
      varRoot->SetItemValue("<empty>");
      return;
    }

    //If there is more than one array item, nest them
    if(arraySize > 1)
    {
      //Update the main label name
      CString labelName;
      labelName.Format("%s[%u]", uniformValue.name.c_str(), arraySize); 
      varRoot->SetLabelText(labelName);

      //Loop an add each item as a sub item
      for(uint i=0; i<arraySize; i++)
      {
        CPropTreeItemGLI* arrayItem;
        arrayItem = (CPropTreeItemGLI*)propTree->InsertItem(new CPropTreeItemGLI(), varRoot);
        
        //Set the name
        labelName.Format("%s[%u]", uniformValue.name.c_str(), i); 
        arrayItem->SetLabelText(labelName);

        //Add the description
        arrayItem->SetItemValue(uniformValue.numTypeElements, &uniformValue.intUniformData[i*uniformValue.numTypeElements], isBool);
      }
    }
    else
    {
      varRoot->SetItemValue(uniformValue.intUniformData.size(), &uniformValue.intUniformData[0], isBool);
    }

  }

}

///////////////////////////////////////////////////////////////////////////////
//
void GLIShaderDebug::AddVariableMatrixDisplay(CPropTreeItemGLI* pRoot, const UniformData &uniformValue, uint arrayOffset)
{
  uint numRows =0;
  uint numElemPerRow =0;

  //Check the offsets
  if((arrayOffset + uniformValue.numTypeElements) > uniformValue.floatUniformData.size())
  {
    pRoot->SetItemValue("Error in offsets");
    return;
  }

  //Get the number of rows and elements per row
  switch(uniformValue.numTypeElements)
  {
    case(4):
      numRows = 2;
      numElemPerRow = 2;
      break;
    case(9):
      numRows = 3;
      numElemPerRow = 3;
      break;
    case(16):
      numRows = 4;
      numElemPerRow = 4;
      break;
    default:
      return;
  };
  
  //Update the label name
  CString labelName;
  labelName.Format("%s [%ux%u]", pRoot->GetLabelText(), numRows, numElemPerRow); 
  pRoot->SetLabelText(labelName);

  //Add the initial row to the first description
  pRoot->SetItemValue(numElemPerRow, &uniformValue.floatUniformData[arrayOffset], false);

  //Add the additional rows 
  for(uint i=1; i<numRows; i++)
  {
    CPropTreeItemGLI* arrayItem;
    arrayItem = (CPropTreeItemGLI*)propTree->InsertItem(new CPropTreeItemGLI(), pRoot);

    //Assign the item value for each additional row
    arrayItem->SetItemValue(numElemPerRow, &uniformValue.floatUniformData[arrayOffset + (numElemPerRow * i)], false);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
BOOL CALLBACK ShaderDebugMsg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  //Process the message
  switch (message) 
  { 
    //Setup initial list controls
    case WM_INITDIALOG:
    {
      //Initialize the property control
      if(GLIShaderDebug::GetInstance() != NULL)
      {
        GLIShaderDebug::GetInstance()->InitPropertyControl(hDlg);
      }

      /*
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
      */
      return TRUE;
    }

    //Hide the dialog when closed
    case WM_CLOSE:
    {
      ShowWindow(hDlg, SW_HIDE);
      break;
    }

    //Destroy the window
    case WM_DESTROY:
    {
      //Delete the data instance
      delete debugInstance;
      return 0;
    }

    //Process notifications
    case WM_NOTIFY:
    {
      LPNMHDR nData = (LPNMHDR)lParam;

      /*
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
      */

      break;
    }

    //Process commands
    case WM_COMMAND:
    {
      break;
    }


  }

  return FALSE;
}

