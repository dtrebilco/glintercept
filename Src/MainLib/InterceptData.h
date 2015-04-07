/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_DATA_H_
#define __INTERCEPT_DATA_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"
#include "FunctionTable.h"
#include "ReferenceCount.h"

#include <string>
#include <vector>

using namespace std;


class GLDriver;


//@
//  Summary:
//    This class intercepts OpenGL based calls and prepares them for
//    data logging. (this acts as a base class for data loggers)
//  
class InterceptData : public ReferenceCount
{
public:

  //@
  //  Summary:
  //    Constructor 
  //  
  //  Parameters:
  //    driver  - The driver used to make queries on the OpenGL state.
  //
	InterceptData(GLDriver *driver);

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
  virtual void LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args) =0;

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
  virtual void LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal) =0;
  
  //@
  //  Summary:
  //    To enable/disable data saving. Ensure data save path is set before
  //    enabling.
  //  
  //  Parameters:
  //    flag - Flag to enable/disable data saving.
  //
  inline virtual void SetDataSaving(bool flag);

  //@
  //  Summary:
  //    To get the data saving state.
  //  
  //  Returns:
  //    The state of data saving is returned.
  //
  inline bool GetDataSaving();

  //@
  //  Summary:
  //    To set the path where data is saved (including trailing seperator)
  //  
  //  Parameters:
  //    newPath - The new path to save data to.
  //
  virtual void SetDataSavingPath(const string &newPath);

  //@
  //  Summary:
  //    To get the path where data is saved (including trailing seperator)
  //    (this will create the directory if it does not exist)
  //  
  //  Returns:
  //    The path where data is saved is returned.
  //
  const string & GetDataSavingPath();

  //@
  //  Summary:
  //    To save all the currently dirty data to disk.
  //  
  virtual void SaveAllDirtyData() =0;

  //@
  //  Summary:
  //    To set all the data that is currently being tracked, as 
  //    dirty. (ie. needs saving)
  //  
  virtual void SetAllDataDirty() =0;

protected:

  GLDriver         *driver;                       // The OpenGL driver

  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~InterceptData();

private:

  bool      dataSavingEnabled;                    // Flag to indicate if we are saving data
  string    dataSavePath;                         // The path to save data to (Including trailing seperator)
  bool      dataSavePathCreated;                  // Flag to indicate if the data saving path has been created

};

///////////////////////////////////////////////////////////////////////////////
//
inline void InterceptData::SetDataSaving(bool flag)
{
  dataSavingEnabled = flag;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptData::GetDataSaving()
{
  return dataSavingEnabled;
}




#endif // __INTERCEPT_DATA_H_
