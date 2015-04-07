/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __DISPLAY_LIST_SAVE_MANAGER_H_
#define __DISPLAY_LIST_SAVE_MANAGER_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"

#include <string>
#include <vector>

using namespace std;

class GLDriver;
class ConfigData;


//@
//  Summary:
//    Class to handle the saving of display lists. 
//  
class DisplayListSaveManager
{
public:

  //@
  //  Summary:
  //    To set the initial saving options for the display list saver.
  //  
  //  Parameters:
  //    configData  - The configuration data to copy the saving options from.
  //
  DisplayListSaveManager(const ConfigData &configData);

  //@
  //  Summary:
  //    Destructor
  //  
  virtual ~DisplayListSaveManager();

  //@
  //  Summary:
  //    To save the passed display list data structure out to disk. 
  //  
  //  Parameters:
  //    fileName  - The base file name to save the display list out as. (without extension)
  //
  //    displayListData  - The display list data to save. 
  //
  //    retFileName - The return file name of the display list saved.
  //
  //  Returns:
  //    True is returned if the display list data was written successfully. 
  //    Otherwise false is returned.
  //
  bool SaveDisplayList(const string &fileName, const DisplayListData *displayListData,string &retFileName) const;


protected:
   
  //@
  //  Summary:
  //    To save a displayLists' source to disk.
  //  
  //  Parameters:
  //    fileName  - The base file name to save the displayList out as. (without extension)
  //
  //    displayListSrc  - The raw displayList source data to save. 
  //
  //    displayListLength - The length of the displayList source.
  //
  //    retFileName - The return file name of the displayList saved.
  //
  //  Returns:
  //    True is returned if the data was written successfully. 
  //    Otherwise false is returned.
  //
  bool WriteDisplayListFile(const string &fileName,const char * displayListSrc, uint displayListLength,string &retFileName) const;

};





#endif // __DISPLAY_LIST_SAVE_MANAGER_H_
