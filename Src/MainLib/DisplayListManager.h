/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __DISPLAY_LIST_MANAGER_H_
#define __DISPLAY_LIST_MANAGER_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"
#include "InterceptDataManager.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;


//@
//  Summary:
//    Structure of data about a OpenGL display list
//  
class DisplayListData : public InterceptDataType
{
public:

  //@
  //  Summary:
  //    Constructor, inits all data to default values.
  //
  //  Parameters:
  //    glID  - The unique OpenGL ID of this list.
  // 
  DisplayListData(uint glID);

  //@
  //  Summary:
  //    To flag the display list as ready and sets that the display list can now be saved.
  //    (This call also flags the display list as dirty)
  //  
  inline void SetReady();

  //@
  //  Summary:
  //    To set if the display list is dirty or not. This flag should be set when the 
  //    OpenGL display list changes and un-set when the display list is written out.
  //
  //  Parameters:
  //    flag  - Flag to indicate if this OpenGL display list is now dirty or not.
  //
  inline void SetDirty(bool flag);

  //@
  //  Summary:
  //    To get a unique display list filename (including type,display list ID 
  //    and savecount) without any extension. 
  //  
  //  Parameters:
  //    retString  - The string to return the unique name in.
  //
  //  Returns:
  //    If a unique string could be generated, true is returned. 
  //    Else false is returned.
  //
  bool GetUniqueFileName(string &retString) const;

  //@
  //  Summary:
  //    To get a reference to the filename that was used to save the 
  //    display list data out previously.
  //  
  //  Returns:
  //    A reference to the filename of saved display list data is returned.
  //
  inline string & GetDisplayListSaveFileName();


  //@
  //  Summary:
  //    To get the display lists' command string.
  //  
  //  Returns:
  //    The string containing the display lists' commands is returned.
  //
  inline string & GetDisplayListSource();
  inline const string & GetDisplayListSource() const;

  //@
  //  Summary:
  //    To set this list as a render call list.
  //  
  //  Parameters:
  //    flag  - The flag to set this as a render call list.
  //
  inline void SetIsRenderCallList(bool flag);

  //@
  //  Summary:
  //    To get if this list is a render call list.
  //  
  //  Returns:
  //    If this list performs a render, true is returned. Else false is returned.
  //
  inline bool GetIsRenderCallList() const;


private:

  string saveFileName; // File name that was used when the display list was last saved.

  string displayListSource; // The source(command) string of the display list
  
  bool   isRenderCallList;  // Flag to indicate if this is a list that performs a render

};

//@
//  Summary:
//    This class maintains a list of display list data and controls 
//    adding/deleting and retrival of display list data.
//  
typedef InterceptDataManager<DisplayListData> DisplayListManager;



///////////////////////////////////////////////////////////////////////////////
//
inline void DisplayListData::SetReady()
{
  ready  = true;

  //Set as dirty
  SetDirty(true);
}

///////////////////////////////////////////////////////////////////////////////
//
inline void DisplayListData::SetDirty(bool flag)
{
  //Chcek if the flag is different
  if(flag != dirty && ready)
  {
    //Assign the dirty flag
    dirty = flag;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
inline string & DisplayListData::GetDisplayListSaveFileName()
{
  return saveFileName;
}

///////////////////////////////////////////////////////////////////////////////
//
inline string & DisplayListData::GetDisplayListSource()
{
  return displayListSource;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const string & DisplayListData::GetDisplayListSource() const
{
  return displayListSource;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void DisplayListData::SetIsRenderCallList(bool flag)
{
  isRenderCallList = flag;
}


///////////////////////////////////////////////////////////////////////////////
//
inline bool DisplayListData::GetIsRenderCallList() const
{
  return isRenderCallList;
}

#endif // __DISPLAY_LIST_MANAGER_H_
