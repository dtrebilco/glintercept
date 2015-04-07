/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_DISPLAY_LIST_H_
#define __INTERCEPT_DISPLAY_LIST_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"
#include "DisplayListManager.h"
#include "DisplayListSaveManager.h"
#include "InterceptData.h"
#include "InterceptLog.h"

#include <string>
#include <vector>

using namespace std;


class GLDriver;

//@
//  Summary:
//    This class intercepts OpenGL display list based calls and prepares them for
//    logging.
//  
class InterceptDisplayList : public InterceptData
{
public:

  //@
  //  Summary:
  //    Constructor 
  //  
  //  Parameters:
  //    driver  - The driver used to make queries on the OpenGL state.
  //
  //    functionTable - The table of all functions that are known/are being logged.
  //
  //    configData    - The configuration options for display list interception.
  //
	InterceptDisplayList(GLDriver *driver,FunctionTable * functionTable,const ConfigData &configData);

  //@
  //  Summary:
  //    To log the passed function and function data 
  //    (Before the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args);

  //@
  //  Summary:
  //    To perform any post-call logging of a function.
  //    (After the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value (if any).
  //
  virtual void LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal);
  
  //@
  //  Summary:
  //    To enable/disable display list saving. Ensure display list path is set before
  //    enabling.
  //  
  //  Parameters:
  //    flag - Flag to enable/disable display list saving.
  //
  virtual void SetDataSaving(bool flag);

  //@
  //  Summary:
  //    To set the path where display list are saved (including trailing seperator)
  //  
  //  Parameters:
  //    newPath - The new path to save display list to.
  //
  virtual void SetDataSavingPath(const string &newPath);

  //@
  //  Summary:
  //    To save the passed display list data to disk. Lists are only saved if 
  //    display list saving is enabled. The state of the lists' dirty flag is
  //    re-set on successful saving. 
  //  
  //  Parameters:
  //    listData  - The display list data to save.
  //
  //  Returns:
  //    If saving was successful, true is returned. Else false is returned.
  //
  bool SaveDisplayListData(DisplayListData *listData);

  //@
  //  Summary:
  //    To save all the currently dirty lists to disk.
  //  
  virtual void SaveAllDirtyData();
  
  //@
  //  Summary:
  //    To set all the display list that are currently being tracked, as 
  //    dirty. (ie. needs saving)
  //  
  virtual inline void SetAllDataDirty();

  //@
  //  Summary:
  //    To get the saved display list file name for the passed list ID. If the 
  //    display list is dirty, an attempt to save the list is made. 
  //  
  //  Parameters:
  //    id  - The display list ID of the list to save.
  //
  //    retListName - The filename used to save the display list.
  //
  //  Returns:
  //    If saving/retrieval was successful, true is returned. Else false is returned.
  //
  bool GetDisplayListFileName(GLuint id,string &retListName);

  //@
  //  Summary:
  //    To determine if the passed function calls a list that performs a 
  //    render.
  //  
  //  Parameters:
  //    funcData  - The function data pointer to test.
  //
  //    funcTableIndex - The index of the passed function data in the function
  //                     table.
  //
  //    args     - The arguments of the function.
  //
  //  Returns:
  //    If the passed function data pointer calls a list (that performs a render), 
  //    true is returned. Else false is returned.
  //
  bool IsRenderListFunction(const FunctionData *funcData, uint funcTableIndex, const FunctionArgs & args);

protected:

  DisplayListManager     displayListManager;      // The manager of display lists
  DisplayListSaveManager displayListSaver;        // The manager of saving display lists to disk

  bool      initialized;                          // Flag to indicate if we have been initialized
  bool      initFailed;                           // Flag to indicate if initialization failed

  InterceptLog listFunctionLog;                   // A text logger to record the function calls

  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~InterceptDisplayList();

  //@
  //  Summary:
  //    To initialize constants and arrays etc.
  //  
  //  Returns:
  //    If initialization was successful, true is returned. Else false is returned.
  //
  bool Init();

  //@
  //  Summary:
  //    To determine if the driver is available and perform initialization if
  //    necessary.
  //  
  //  Returns:
  //    If the driver is available, true is returned. Else false is returned.
  //
  bool DriverAvailable();

  //@
  //  Summary:
  //    To get the action index (what action needs to be taken) for the 
  //    passed function name and index. (the index is assumed to remain the
  //    same for a given function name)
  //  
  //  Returns:
  //    If a action is valid for the index it is returned. Else -1 is returned.
  //
  int GetListActionIndex(const string &functionName, int funcIndex); 


private:

  bool   listCompiling;                           // Flag to indicate if a list is currently being compiled.
  bool   listRenderCall;                          // Flag to indicate that a render call was made from the list.
  GLuint iListID;                                 // The internal list ID of the list currently being compiled.
  string iListString;                             // The string containing all the commands used in compiling the current list.

  //Creates a display list
  void CreateListPre(int actionIndex,const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args);

  //Delete the specified range of lists
  void DeleteLists(const FunctionData *funcData,uint index, FunctionArgs & args);

  //Flags that a display list has been called
  void CallLists(int actionIndex,const FunctionData *funcData,uint index, FunctionArgs & args);
  
  //Flags that display lists have been created by a font building call
  void WGLUseFont(int actionIndex,const FunctionData *funcData,uint index, FunctionArgs & args);

  //@
  //  Summary:
  //    To get the next listID from the list array. 
  //  
  //  Parameters:
  //    listType  - The format of the ID's in the list array.
  //
  //    listArray - The list array to extract the list ID's from.
  //                (The list is incremented once the list ID is extracted)
  //
  //  Returns:
  //    A list ID is extracted from the "listArray" and returned.
  //
  GLuint GetNextListID(GLenum listType,GLvoid **listArray);

};


///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptDisplayList::SetAllDataDirty()
{
  displayListManager.SetAllDataDirty();
}

#endif // __INTERCEPT_DISPLAY_LIST_H_
