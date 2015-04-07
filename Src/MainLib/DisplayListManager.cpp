/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "DisplayListManager.h"
#include "GLDriver.h"


USING_ERRORLOG

//Set the class name for the display list manager
template <>
char *DisplayListManager::className = "DisplayList";

///////////////////////////////////////////////////////////////////////////////
//
DisplayListData::DisplayListData(uint glID):
InterceptDataType(glID),
saveFileName(""),
displayListSource(""),
isRenderCallList(false)
{
}


///////////////////////////////////////////////////////////////////////////////
//
bool DisplayListData::GetUniqueFileName(string &retString) const
{
  //Set the initial display list name
  retString = "DisplayList_";

  //Add the display list ID
  string bufString;
  StringPrintF(bufString,"%04u_",id);
  retString = retString + bufString;
  
  //Add the save count
  static uint saveCount=0;
  saveCount++;
  StringPrintF(bufString,"%04u",saveCount);
  retString = retString + bufString;

  return true;
}



