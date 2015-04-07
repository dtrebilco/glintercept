/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptDisplayList.h"
#include "FileUtils.h"
#include "GLDriver.h"

//The real OpenGL driver
extern GLCoreDriver GLV;
extern WGLDriver    GLW;

USING_ERRORLOG


//Flags to indicate what action to take when we get an
//  list function
enum ListFunctionAction
{
  LFA_UNKNOWN =0,

  LFA_LIST_CREATE,
  LFA_LIST_DELETE,
  LFA_LIST_CALL,
  LFA_WGL_USE_FONT,
};

enum ListFunctionType
{
  GL_NEW_LIST =0,
  GL_END_LIST,
  GL_DELETE_LIST,
  GL_CALL_LIST,
  GL_CALL_LISTS,

#ifdef GLI_BUILD_WINDOWS

  WGL_USE_FONT_BITMAPS_A,
  WGL_USE_FONT_BITMAPS_W,

  WGL_USE_FONT_OUTLINES_A,
  WGL_USE_FONT_OUTLINES_W,

#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

  XGL_USE_X_FONT,

#endif //GLI_BUILD_LINUX

};

// Structure to hold the name of functions that affect the list state
struct ListLogData
{
  ListLogData(const char * funcName, ListFunctionType funcType, ListFunctionAction newAction);

  string               functionName;  //The name of the function
  ListFunctionAction   action;        //What the function does and what action to take
  ListFunctionType     funcType;      //The type of the function (enum version of name)
  int                  functionIndex; //Index of the function (in the function table, -may be -1 if unknown)

};


//The array of built-in known functions
ListLogData knownListFunctions[] = 
{

  //General creation/deletion lists
  ListLogData("glNewList",GL_NEW_LIST       ,LFA_LIST_CREATE),
  ListLogData("glEndList",GL_END_LIST       ,LFA_LIST_CREATE),
  ListLogData("glDeleteLists",GL_DELETE_LIST,LFA_LIST_DELETE),
  ListLogData("glCallList" ,GL_CALL_LIST    ,LFA_LIST_CALL),
  ListLogData("glCallLists",GL_CALL_LISTS   ,LFA_LIST_CALL),

#ifdef GLI_BUILD_WINDOWS

  ListLogData("wglUseFontBitmapsA" ,WGL_USE_FONT_BITMAPS_A, LFA_WGL_USE_FONT),
  ListLogData("wglUseFontBitmapsW" ,WGL_USE_FONT_BITMAPS_W, LFA_WGL_USE_FONT),

  ListLogData("wglUseFontOutlinesA" ,WGL_USE_FONT_OUTLINES_A, LFA_WGL_USE_FONT),
  ListLogData("wglUseFontOutlinesW" ,WGL_USE_FONT_OUTLINES_W, LFA_WGL_USE_FONT),

#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

  ListLogData("glXUseXFont" ,XGL_USE_X_FONT, LFA_WGL_USE_FONT),

#endif //GLI_BUILD_LINUX

};



#define NUM_LIST_LOG_FUNCTIONS sizeof(knownListFunctions)/sizeof(ListLogData)


///////////////////////////////////////////////////////////////////////////////
//
ListLogData::ListLogData(const char * funcName,ListFunctionType type, ListFunctionAction newAction):
functionName(funcName),
action(newAction),
funcType(type),
functionIndex(-1)
{
}


///////////////////////////////////////////////////////////////////////////////
//
InterceptDisplayList::InterceptDisplayList(GLDriver *ogldriver,FunctionTable * functionTable,const ConfigData &configData):
InterceptData(ogldriver),
displayListManager(),
displayListSaver(configData),
initialized(false),
initFailed(false),
listFunctionLog(functionTable),

listCompiling(false),
listRenderCall(false),
iListID(0),
iListString("")
{

  //Register the display list based functions with the function table
  for(uint i=0;i<NUM_LIST_LOG_FUNCTIONS;i++)
  {
    //Set the flag
    functionTable->SetFunctionFlag(knownListFunctions[i].functionName,FDF_DLIST_LOG);
  }

  //Init the list function log
  listFunctionLog.Init();

}

///////////////////////////////////////////////////////////////////////////////
//
InterceptDisplayList::~InterceptDisplayList()
{
  //Turn off list saving
  if(GetDataSaving())
  {
    SetDataSaving(false);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptDisplayList::Init()
{
  //Test if we can proceed
  if(!driver || /*!driver->GetInternalGLCallMode() ||*/ initFailed)
  {
    return false;
  }

  //Flag that init has been called
  initialized  = true;

  // Fail init for OpenGL version 3.0+ pure context?

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptDisplayList::DriverAvailable()
{
  //Get the state of the driver
  if(!driver || initFailed)
  {
    return false;
  }

  //Test if we have been init
  if(!initialized)
  {
    if(!Init())
    {
      initFailed = true;
      return false;
    }
  }
  
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptDisplayList::SetDataSaving(bool flag)
{
  //Assign the flag by calling the base class
  InterceptData::SetDataSaving(flag);
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptDisplayList::SetDataSavingPath(const string &newPath)
{
  //Append the DisplayLists directory then pass to the base class
  string path = newPath + "DisplayLists" + FileUtils::dirSeparator;

  //Call base class to set
  InterceptData::SetDataSavingPath(path);
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptDisplayList::LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{
  //Return if the driver is not available
  if(!DriverAvailable())
  {
    return;
  }

  //If we are in a display list, log the commands
  if(listCompiling)
  {
    //Append the commands to the string
    string functionString;
    listFunctionLog.GetFunctionString(funcData,index,args,functionString);
    
    //Adjust the string capacity size if the string is getting full (speed optimize)
    if(iListString.capacity() - iListString.size() < 200)
    {
      iListString.reserve(iListString.capacity()*2);
      //LOGERR(("Capacity %u Size %u",iListString.capacity(),iListString.size())); 
    }

    //Append the function name to the string
    iListString += functionString + "\n";

    //Test if a render call was made (Note: Possible issue if a display list calls 
    //  a display list that does a render - could fix by tracking what display lists 
    //  are called by a display list and checking them when a render call test is performed.
    //  This is complicated (have to check for recursion etc) and will only implement upon request.)
    if(funcData->IsFlagSet(FDF_RENDER_FUNC))
    {
      listRenderCall = true;
    }
  }

  //If this is not an display list function, return now
  if(!funcData->IsFlagSet(FDF_DLIST_LOG))
  {
    return;
  }

  //Get the action required for the function
  int actionIndex = GetListActionIndex(funcData->GetName(),index);
  if(actionIndex == -1)
  {
    return;
  }
  ListFunctionAction action = knownListFunctions[actionIndex].action;

  //Copy the argument list
  FunctionArgs newArgs(args);

  //Perform the action
  switch(action)
  {
    case(LFA_LIST_CREATE) :
      CreateListPre(actionIndex,funcData,index,newArgs);
      break;
    case(LFA_LIST_DELETE) :
      DeleteLists(funcData,index,newArgs);
      break;
    case(LFA_LIST_CALL) :
      CallLists(actionIndex,funcData,index,newArgs);
      break;
    case(LFA_WGL_USE_FONT) :
      WGLUseFont(actionIndex,funcData,index,newArgs);
      break;

    default:
       LOGERR(("InterceptDisplayList::LogFunctionPre - Unknown action"));
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptDisplayList::LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{
  //Test if driver is not available
  if(!funcData->IsFlagSet(FDF_DLIST_LOG) || !DriverAvailable())
  {
    return;
  }

  //Get the action required for the function
  int actionIndex = GetListActionIndex(funcData->GetName(),index);
  if(actionIndex == -1)
  {
    return;
  }
  ListFunctionAction action = knownListFunctions[actionIndex].action;

  //Perform the action
  switch(action)
  {
    case(LFA_LIST_CREATE) :
      break;
    case(LFA_LIST_DELETE) :
      break;
    case(LFA_LIST_CALL) :
      break; 
    case(LFA_WGL_USE_FONT) :
      break; 
    default:
       LOGERR(("InterceptDisplayList::LogFunctionPost - Unknown action"));
  }

}
  
///////////////////////////////////////////////////////////////////////////////
//
bool InterceptDisplayList::IsRenderListFunction(const FunctionData *funcData, uint funcTableIndex, const FunctionArgs & args)
{
  //If this is not a display list function, return now
  if(!funcData->IsFlagSet(FDF_DLIST_LOG))
  {
    return false;
  }

  //Return if the driver is not available
  if(!DriverAvailable())
  {
    return false;
  }

  //Get the "action" for the function
  int actionIndex = GetListActionIndex(funcData->GetName(),funcTableIndex);
  if(actionIndex == -1)
  {
    return false;
  }  

  ListFunctionType funcType = knownListFunctions[actionIndex].funcType;

  //Copy the arguments
  FunctionArgs newArgs(args);

  //Check if it is a single call
  if(funcType == GL_CALL_LIST)
  {
    //Get the list ID
    GLuint listID;
    newArgs.Get(listID);
    
    //Find the list
    DisplayListData * listData = displayListManager.GetData(listID);
    if(listData && listData->GetIsRenderCallList())
    {
      return true;
    }
  }

  //If it is a multi-call
  if(funcType == GL_CALL_LISTS)
  {
    //Get the number of lists
    GLsizei listNum;    
    newArgs.Get(listNum);

    //Get the type of the numbers
    GLenum listType;
    newArgs.Get(listType);

    //Get a pointer to the array
    GLvoid *listPtr;
    newArgs.Get(listPtr);

    //Get the list base
    GLint listBase;
    GLV.glGetIntegerv(GL_LIST_BASE,&listBase);

    //Check the passed data
    if(listPtr == NULL || listNum < 0)
    {
      return false;
    }

    //Loop and check each disply list if necessary
    for(uint i=0;i<(uint)listNum;i++)
    {
      //Get the next list in the array
      GLuint listID = listBase + GetNextListID(listType,&listPtr);

      //Find the list
      DisplayListData * listData = displayListManager.GetData(listID);
      if(listData && listData->GetIsRenderCallList())
      {
        return true;
      }
    }
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
int InterceptDisplayList::GetListActionIndex(const string &functionName, int funcIndex)
{
  uint i;

  //Check the list to find the index 
  for(i=0;i<NUM_LIST_LOG_FUNCTIONS;i++)
  {
    //If the index is the same, we have mapped this function previously 
    if(knownListFunctions[i].functionIndex == funcIndex)
    {
      return i;
    }
  }

  //If this function has not been called previously, find the name and assign the index
  for(i=0;i<NUM_LIST_LOG_FUNCTIONS;i++)
  {
    //If the name is equal, assign the index
    if(knownListFunctions[i].functionName == functionName)
    {
      knownListFunctions[i].functionIndex = funcIndex;
      return i;
    }

  }

  //If the function is not found, something has broken with the mapping/registery
  LOGERR(("InterceptDisplayList::GetListActionIndex - Attempting to log unknown function %s",functionName.c_str()));

  return -1;
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptDisplayList::CreateListPre(int actionIndex,const FunctionData *funcData,uint index, FunctionArgs & args)
{
  //Copy the function argument marker
  FunctionArgs origArgs(args);

  //Check if we are creating a new list
  if(knownListFunctions[actionIndex].funcType == GL_NEW_LIST)
  {
    //Check that we are not in a list
    if(listCompiling)
    {
      LOGERR(("InterceptDisplayList::CreateListPre - Attempt to create a new list while compiling list %u",iListID));
      return;
    }

    //Flag that we are in a list
    listCompiling = true;

    //Get the id of the list
    args.Get(iListID);

    ///Reset the string with the glNewList function call
    string functionString;
    listFunctionLog.GetFunctionString(funcData,index,origArgs,functionString);
    iListString = functionString + "\n";

    //Reset the render call flag
    listRenderCall = false;
  }
  else if(knownListFunctions[actionIndex].funcType == GL_END_LIST)
  {
    //Check that we are in a list
    if(!listCompiling)
    {
      LOGERR(("InterceptDisplayList::CreateListPre - Attempt to end a list while not compiling a list"));
      return;
    }

    //Reset the flag
    listCompiling = false;

    //Assign the string to the display list
    DisplayListData * listData = displayListManager.GetData(iListID);

    //If the list does not exist, add it
    if(!listData)
    {
      //Add the list
      displayListManager.AddData(iListID);

      //Get the pointer to it
      listData = displayListManager.GetData(iListID);
      if(!listData)
      {
        LOGERR(("InterceptDisplayList::CreateListPre - Unable to add new list?"));
        return;
      }
    }
    else
    {
      //Save the old list if it was dirty
      if(listData->IsReady() && listData->IsDirty())
      {
        SaveDisplayListData(listData);
      }
    }

    //Flag the list as ready
    listData->SetReady();
    listData->SetDirty(true);

    //Assign the new string of commands
    listData->GetDisplayListSource() = iListString;

    //Empty the string
    iListString = "";

    //Assign if a render call was made in the list
    listData->SetIsRenderCallList(listRenderCall);

    //Reset the render call flag
    listRenderCall = false;

    //Save the list out
    SaveDisplayListData(listData);
  }
  else
  {
    LOGERR(("InterceptDisplayList::CreateListPre - Attempting to log unknown function %s",funcData->GetName().c_str()));
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptDisplayList::DeleteLists(const FunctionData *funcData,uint index, FunctionArgs & args)
{
  //Get the first list number
  GLuint startList;
  args.Get(startList);

  //Get the range of lists to delete
  GLsizei range;
  args.Get(range);

  //Loop for all the lists
  for(uint i=startList;i<(startList+range);i++)
  {
    //Attempt to get the list ID and save the list if it is dirty
    DisplayListData *listData = displayListManager.GetData(i);
    if(listData && listData->IsReady() && listData->IsDirty())
    {
      //Save the dirty display list before it is deleted
      SaveDisplayListData(listData);
    }

    //Remove the display list ID
    displayListManager.RemoveData(i);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptDisplayList::CallLists(int actionIndex,const FunctionData *funcData,uint index, FunctionArgs & args)
{
  //If we are not saving lists, return now
  if(!GetDataSaving())
  {
    return;
  }

  //Check if it is a single call
  if(knownListFunctions[actionIndex].funcType == GL_CALL_LIST)
  {
    //Get the list ID
    GLuint listID;
    args.Get(listID);
    
    //Find the list
    DisplayListData * listData = displayListManager.GetData(listID);
    if(listData)
    {
      //Save the list if necessary
      if(listData->IsReady() && listData->IsDirty())
      { 
        SaveDisplayListData(listData); 
      }
    }
    else
    {
      LOGERR(("InterceptDisplayList::CallLists - Calling non-existant list? %u",listID));
    }
  }

  //If it is a multi-call
  if(knownListFunctions[actionIndex].funcType == GL_CALL_LISTS)
  {
    //Get the number of lists
    GLsizei listNum;
    args.Get(listNum);

    //Get the type of the numbers
    GLenum listType;
    args.Get(listType);

    //Get a pointer to the array
    GLvoid *listPtr;
    args.Get(listPtr);
    
    //Get the list base
    GLint listBase;
    GLV.glGetIntegerv(GL_LIST_BASE,&listBase);

    //Check the passed data
    if(listPtr == NULL || listNum < 0)
    {
      LOGERR(("InterceptDisplayList::CallLists - Invalid call data"));
      return;
    }

    //Loop and save each disply list if necessary
    for(uint i=0;i<(uint)listNum;i++)
    {
      //Get the next list in the array
      GLuint listID = listBase + GetNextListID(listType,&listPtr);

      //Find the list
      DisplayListData * listData = displayListManager.GetData(listID);
      if(listData)
      {
        //Save the list if necessary
        if(listData->IsReady() && listData->IsDirty())
        { 
          SaveDisplayListData(listData); 
        }
      }
      else
      {
        LOGERR(("InterceptDisplayList::CallLists - Calling non-existant list? %u",listID));
      }
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptDisplayList::WGLUseFont(int actionIndex,const FunctionData *funcData,uint index, FunctionArgs & args)
{
  GLint listCreateOffset=0;
  GLint listCreateCount =0;

  GLuint dummyOffset = 0;

  //Switch based on the function type
  // May need to update args retrieval for 64-bit
  switch(knownListFunctions[actionIndex].funcType)
  {

#ifdef GLI_BUILD_WINDOWS

    case(WGL_USE_FONT_BITMAPS_A) :
    case(WGL_USE_FONT_BITMAPS_W) :
    {
      void * dummyPtr = NULL;
      
      //Get the starting HDC and glyph
      args.Get(dummyPtr);
      args.Get(dummyOffset);
      
      //Get the display list count and base offset
      args.Get(listCreateCount);
      args.Get(listCreateOffset);
      break;
    }
    
    case(WGL_USE_FONT_OUTLINES_A) :
    case(WGL_USE_FONT_OUTLINES_W) :
    {
      void * dummyPtr = NULL;
          	
      //Get the starting HDC and glyph
      args.Get(dummyPtr);
      args.Get(dummyOffset);
      
      //Get the display list count and base offset
      args.Get(listCreateCount);
      args.Get(listCreateOffset);
      break;
    }
    
#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

    //DT_TODO: Test this Linux support
    case(XGL_USE_X_FONT) :
      
      args.Get(dummyOffset);
      args.Get(dummyOffset);

      //Get the display list count and base offset
      args.Get(listCreateCount);
      args.Get(listCreateOffset);
      break;

#endif //GLI_BUILD_LINUX

    default:
       LOGERR(("InterceptDisplayList::WGLUseFont - Attempting to log unknown function %s",funcData->GetName().c_str()));
       break;
  }

  //Test the offset limit
  if(listCreateOffset < 0)
  {
    LOGERR(("InterceptDisplayList::WGLUseFont - Attempting to create lists at a negative offset? %d",listCreateOffset));
    return;
  }

  //Loop for all the lists to create
  for(uint i=0; i<(uint)listCreateCount; i++)
  {
    //Get the new list index
    uint newListID = listCreateOffset + i;

    //Attempt to get the display list
    DisplayListData * listData = displayListManager.GetData(newListID);

    //If the list does not exist, add it
    if(!listData)
    {
      //Add the list
      displayListManager.AddData(newListID);

      //Get the pointer to it
      listData = displayListManager.GetData(newListID);
      if(!listData)
      {
        LOGERR(("InterceptDisplayList::WGLUseFont - Unable to add new list?"));
        return;
      }
    }
    else
    {
      //Save the old list if it was dirty
      if(listData->IsReady() && listData->IsDirty())
      {
        SaveDisplayListData(listData);
      }
    }

    //Flag the list as ready
    listData->SetReady();
    listData->SetDirty(true);

    //Assign the text for the display list
    listData->GetDisplayListSource() = "WGL/GLX text render";

    //Assign that these lists are render calls
    listData->SetIsRenderCallList(true);

    //Save the list out
    SaveDisplayListData(listData);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
GLuint InterceptDisplayList::GetNextListID(GLenum listType,GLvoid **listArray)
{
  GLuint retValue=0;

  //Extract the list ID based on the type
  switch(listType)
  {
    case(GL_BYTE) : 
      retValue = *((GLbyte*)*listArray);
      *listArray = ((GLbyte*)*listArray)+1;
      break;

    case(GL_UNSIGNED_BYTE) : 
      retValue = *((GLubyte*)*listArray);
      *listArray = ((GLubyte*)*listArray)+1;
      break;

    case(GL_SHORT) : 
      retValue = *((GLshort*)*listArray);
      *listArray = ((GLshort*)*listArray)+1;
      break;

    case(GL_UNSIGNED_SHORT) : 
      retValue = *((GLushort*)*listArray);
      *listArray = ((GLushort*)*listArray)+1;
      break;

    case(GL_INT) : 
      retValue = *((GLint*)*listArray);
      *listArray = ((GLint*)*listArray)+1;
      break;

    case(GL_UNSIGNED_INT) : 
      retValue = *((GLuint*)*listArray);
      *listArray = ((GLuint*)*listArray)+1;
      break;

    case(GL_FLOAT) : 
      retValue = (GLuint)(*((float*)*listArray));
      *listArray = ((float*)*listArray)+1;
      break;

    case(GL_2_BYTES) : 
      retValue  = ((GLubyte*)*listArray)[0] * 256;
      retValue += ((GLubyte*)*listArray)[1];
      *listArray = ((GLubyte*)*listArray)+2;
      break;

    case(GL_3_BYTES) : 
      retValue  = ((GLubyte*)*listArray)[0] * 65536;
      retValue += ((GLubyte*)*listArray)[1] * 256;
      retValue += ((GLubyte*)*listArray)[2];
      *listArray = ((GLubyte*)*listArray)+3;
      break;

    case(GL_4_BYTES) : 
      retValue  = ((GLubyte*)*listArray)[0] * 16777216;
      retValue += ((GLubyte*)*listArray)[1] * 65536;
      retValue += ((GLubyte*)*listArray)[2] * 256;
      retValue += ((GLubyte*)*listArray)[3];
      *listArray = ((GLubyte*)*listArray)+4;
      break;

    default:
      LOGERR(("InterceptDisplayList::GetNextListID - Invalid list type? %u",listType));
      return 0;
  }

  return retValue;
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptDisplayList::SaveAllDirtyData() 
{
  //Return if the driver is not available
  if(!DriverAvailable())
  {
    return;
  }

  vector<DisplayListData *> dirtyLists;

  //Get all the dirty lists
  displayListManager.GetAllDirtyData(dirtyLists);

  //Loop for all lists
  for(uint i=0;i<dirtyLists.size();i++)
  {
    //Save the list
    SaveDisplayListData(dirtyLists[i]);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptDisplayList::GetDisplayListFileName(GLuint id,string &retListName)
{
  //Check if the display list exists yet or not
  DisplayListData * displayListData = displayListManager.GetData(id);
  if(!displayListData)
  {
    LOGERR(("InterceptDisplayList::GetDisplayListFileName - Unknown or invalid display list? ID = %u",id));
    return false;
  }
  
  //If the display list is ready, save it
  if(displayListData->IsReady())
  {
    //Save the display list if dirty
    if(displayListData->IsDirty())
    {
      //If there is an error in saving, return now
      if(!SaveDisplayListData(displayListData))
      {
        return false;
      }
    }
  
    //Copy the file names
    retListName = displayListData->GetDisplayListSaveFileName();
    return true;
  }

  return false;

}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptDisplayList::SaveDisplayListData(DisplayListData *listData) 
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return false;
  }

  //If we are not saving lists, return now
  if(!GetDataSaving())
  {
    return false;
  }

  //Ensure that the list is ready
  if(!listData->IsReady())
  {
    LOGERR(("InterceptDisplayList::SaveDisplayListData - Display list is not ready yet"));
    return false;
  }

  //Get the file base name
  string listName;
  if(!listData->GetUniqueFileName(listName))
  {
    return false;
  }

  //Append the lists' directory name
  listName = GetDataSavingPath() + listName;

  //Write the display list out
  string retFileName;
  if(!displayListSaver.SaveDisplayList(listName,listData,retFileName))
  {
    LOGERR(("InterceptDisplayList::SaveDisplayListData - Error saving display list"));
  }

  //Assign the return file name
  listData->GetDisplayListSaveFileName() = retFileName;

  //Flag that the list is no longer dirty 
  //   (set this reguardless or weither list saving was successful)
  listData->SetDirty(false);

  return true;
}
