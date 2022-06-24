/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "GLDriver.h"
#include "FunctionTable.h"
#include "ExtensionFunction.h"
#include "FunctionParser.h"
#include "InterceptLogXML.h"
#include <FileUtils.h>
#include "InterceptPluginManager.h"
#include <string.h>

USING_ERRORLOG

//The function table lookup
FunctionTable * functionTable=NULL;

//The path to the dll
extern string dllPath;

//The real OpenGL driver
extern GLCoreDriver GLV;
extern WGLDriver    GLW;

#ifdef GLI_BUILD_WINDOWS

//Handle to the DLL instance
extern HANDLE       dllHandle;

#endif //GLI_BUILD_WINDOWS


//@
//  Summary:
//    Perform the function lookup of core OpenGL functions.
//  
//  Parameters:
//    openGLLib - The library to load the OpenGL functions from.
//
//  Returns:
//    True is returned on success, false is otherwise
//
bool MapCoreOpenGLFunctions(DLLLoader &openGLLib);

//@
//  Summary:
//    Perform the function lookup of windows GL functions.
//  
//  Parameters:
//    openGLLib - The library to load the OpenGL functions from.
//
//  Returns:
//    True is returned on success, false is otherwise
//
bool MapWGLFunctions(DLLLoader &openGLLib);


///////////////////////////////////////////////////////////////////////////////
//
void GLDriver::InitFunctionTable()
{
  //Test if we are already init.
  if(functionTable)
  { 
    return;
  }

  //Create the function table
  functionTable = new FunctionTable;

}

///////////////////////////////////////////////////////////////////////////////
//
GLDriver::GLDriver():
isInit(false),
frameNumber(0),
loggingStartFrameNum(0),

currLogDir(""),
loggingEnabled(true),
threadCheckingEnabled(false),
functionCallDepth(0),

extensionFunction(NULL),
interceptLog(NULL),
errorDataLog(NULL),

errorFuncString(""),

glContext(NULL),
pluginManager(NULL),

internalCallModeCount(0),
glBeginEndState(false),
glNewListState(false),

functionTime(0),
functionTimeEnabled(false)
{

}

///////////////////////////////////////////////////////////////////////////////
//
GLDriver::~GLDriver()
{
  //Destroy the plugin manager
  if(pluginManager)
  {
    delete pluginManager;
    pluginManager = NULL;
  }

  //Delete the log
  if(interceptLog)
  {
    delete interceptLog;
    interceptLog = NULL;
  }
  if(errorDataLog)
  {
    delete errorDataLog;
    errorDataLog = NULL;
  }

  //If there exists a current GL context on shutdown, something was wrong
  if(glContext != NULL)
  {
    LOGERR(("GLDriver - Shutdown - Current OpenGL context %p?",glContext->GetRCHandle()));
    glContext = NULL;
  }

  //Destroy any outstanding OpenGL contexts
  for(uint i=0;i<glContextArray.size();i++)
  {
    LOGERR(("GLDriver - Shutdown - Outstanding OpenGL context %p ?",glContextArray[i]->GetRCHandle()));

    //Delete the context
    delete glContextArray[i];
  }
  glContextArray.clear();

  //Shut the error logger down here
  if(gliLog)
  {
    //Turn of debugger logger for the ending tags
    gliLog->SetDebuggerLogEnabled(false);

    //Write a log ending tag
    gliLog->LogError("===================================================");
    gliLog->LogError("Log End.");

    //Delete the error log
    delete gliLog;
    gliLog = NULL;
  }


  //Delete the extension function loader
  if(extensionFunction)
  {
    delete extensionFunction;
    extensionFunction = NULL;
  }

  //Delete the function table if it was active
  if(functionTable)
  {
    delete functionTable;
    functionTable = NULL;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool GLDriver::Init()
{
  //Read in the configuration data
  configData.ReadConfigData();

  //Set if the logger mirrors to the debugger
  if(gliLog)
  {
    gliLog->SetDebuggerLogEnabled(configData.errorDebuggerErrorLog);
  }

  //Attempt to open the driver
  if(!openGLLib.Init(configData.openGLFileName.c_str()))
  {
    LOGERR(("GLDriver::Init - Unable to open OpenGL lib file %s",configData.openGLFileName.c_str())); 
    return false;
  }

#ifdef GLI_BUILD_WINDOWS

  //Check that we have not loaded our own DLL (Windows can sometimes force this)
  if(openGLLib.IsDLLHandleMatch(dllHandle))
  {
    LOGERR(("GLDriver::Init - OpenGL lib file points to GLIntercept lib - using <appname>.exe.local file?")); 
    return false;
  }

#endif //GLI_BUILD_WINDOWS

  //Do the core OpenGL function mapping
  if(!MapCoreOpenGLFunctions(openGLLib) || !MapWGLFunctions(openGLLib))
  {
    LOGERR(("GLDriver::Init - Unable to map core/windows OpenGL functions")); 
    return false;
  }

  //Create the function table if necessary
  if(!functionTable)
  { 
    InitFunctionTable();
  }


  //Create the extension function handler
  extensionFunction = new ExtensionFunction(functionTable);

  //Parse the include files
  FunctionParser    parser;
  FunctionDataArray tmpFuncArray;
  EnumDataArray     tmpEnumArray;

  //If parsing was successful, get the known function table data
  if(parser.Parse(configData.functionDataFileName.c_str()))
  {
    //Get the data from the parser (A double copy, slow -may need to speed up)
    parser.GetFunctionData(tmpFuncArray,tmpEnumArray);
  }

  //Init the function table with known data (even if there is no known data or parsing faild)
  functionTable->InitKnownFunctionTable(tmpFuncArray,tmpEnumArray);
  
  //Assign thread checking flag
  threadCheckingEnabled = configData.errorThreadChecking;

  //Assign the current logging directory
  currLogDir = configData.logPath;

  //Set if the any logging is enabled
  loggingEnabled = !configData.logPerFrame;

  //If we are not-perframe logging, create the initial logger here
  if(loggingEnabled && configData.logEnabled)
  {
    //Init the initial function log
    if(!InitFunctionLog())
    {
      return false;
    }
  }

  //Create the error data log
  errorDataLog =  new InterceptLog(functionTable);

  //Register the rendering functions.
  functionTable->SetFunctionFlag("glEvalMesh1",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glEvalMesh2",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glRectd",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glRectf",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glRecti",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glRects",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glRectdv",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glRectfv",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glRectiv",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glRectsv",FDF_RENDER_FUNC);

  functionTable->SetFunctionFlag("glBegin",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glDrawPixels",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glDrawArrays",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glDrawArraysEXT",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glDrawElements",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glDrawRangeElements",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glDrawRangeElementsEXT",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glMultiDrawArrays",FDF_RENDER_FUNC);  
  functionTable->SetFunctionFlag("glMultiDrawArraysEXT",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glMultiDrawElements",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glMultiDrawElementsEXT",FDF_RENDER_FUNC);

  functionTable->SetFunctionFlag("glDrawArraysInstancedARB",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawElementsInstancedARB",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawArraysInstancedEXT",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawElementsInstancedEXT",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawArraysInstanced",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glDrawElementsInstanced",FDF_RENDER_FUNC); 

  functionTable->SetFunctionFlag("glDrawElementsBaseVertex",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawRangeElementsBaseVertex",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawElementsInstancedBaseVertex",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glMultiDrawElementsBaseVertex",FDF_RENDER_FUNC); 

  functionTable->SetFunctionFlag("glDrawArraysIndirect",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawElementsIndirect",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawTransformFeedback",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawTransformFeedbackStream",FDF_RENDER_FUNC); 

  functionTable->SetFunctionFlag("glDrawArraysInstancedBaseInstance",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawElementsInstancedBaseInstance",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawElementsInstancedBaseVertexBaseInstance",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawTransformFeedbackInstanced",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawTransformFeedbackStreamInstanced",FDF_RENDER_FUNC); 

  functionTable->SetFunctionFlag("glDrawElementsBaseVertexEXT",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawRangeElementsBaseVertexEXT",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawElementsInstancedBaseVertexEXT",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glMultiDrawElementsBaseVertexEXT",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawArraysInstancedBaseInstanceEXT",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawElementsInstancedBaseInstanceEXT",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawElementsInstancedBaseVertexBaseInstanceEXT",FDF_RENDER_FUNC); 

  functionTable->SetFunctionFlag("glMultiDrawArraysIndirect",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glMultiDrawElementsIndirect",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glMultiDrawArraysIndirectEXT",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glMultiDrawElementsIndirectEXT",FDF_RENDER_FUNC); 

  functionTable->SetFunctionFlag("glMultiDrawArraysIndirectCountARB",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glMultiDrawElementsIndirectCountARB",FDF_RENDER_FUNC);

  functionTable->SetFunctionFlag("glDrawArraysInstancedNV",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glDrawElementsInstancedNV",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glDrawTransformFeedbackNV",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glMultiDrawArraysIndirectBindlessNV",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glMultiDrawElementsIndirectBindlessNV",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glMultiDrawArraysIndirectBindlessCountNV",FDF_RENDER_FUNC);
  functionTable->SetFunctionFlag("glMultiDrawElementsIndirectBindlessCountNV",FDF_RENDER_FUNC);

  functionTable->SetFunctionFlag("glMultiDrawArraysIndirectAMD",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glMultiDrawElementsIndirectAMD",FDF_RENDER_FUNC); 

  functionTable->SetFunctionFlag("glDrawElementArrayAPPLE",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawRangeElementArrayAPPLE",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glMultiDrawElementArrayAPPLE",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glMultiDrawRangeElementArrayAPPLE",FDF_RENDER_FUNC); 

  functionTable->SetFunctionFlag("glDrawElementsBaseVertexOES",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawRangeElementsBaseVertexOES",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glDrawElementsInstancedBaseVertexOES",FDF_RENDER_FUNC); 
  functionTable->SetFunctionFlag("glMultiDrawElementsBaseVertexOES",FDF_RENDER_FUNC); 

  //Set any other additionally configured render calls
  const vector<string>& renderCalls = configData.frameAdditionalRenderCalls;
  for(vector<string>::size_type i = 0; i < renderCalls.size(); i++)
  {
    functionTable->SetFunctionFlag(renderCalls[i], FDF_RENDER_FUNC);
  }

#ifdef GLI_BUILD_WINDOWS

  //Set the buffer swap call
  functionTable->SetFunctionFlag("wglSwapBuffers",FDF_FRAME_FUNC);
  functionTable->SetFunctionFlag("wglSwapLayerBuffers",FDF_FRAME_FUNC);
  functionTable->SetFunctionFlag("wglSwapMultipleBuffers",FDF_FRAME_FUNC);
  
#endif// GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX

  //Set the buffer swap call. Only one for GLX?
  functionTable->SetFunctionFlag("glXSwapBuffers",FDF_FRAME_FUNC);
  
#endif// GLI_BUILD_LINUX


  //Flag that we are now init
  isInit = true;

  //Create the plugin manager
  pluginManager = new InterceptPluginManager(this,functionTable);
  
  //Attempt to load the plugins
  if(!pluginManager->LoadPlugins(configData))
  {
    delete pluginManager;
    pluginManager = NULL;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLDriver::SetBeginEndState(bool mode)
{
  //Only set when there is a valid context
  if(glContext)
  {
    glBeginEndState = mode;

    //Enable/disable based on if we are enabling or disabling the mode
    SetInternalGLCallMode(!mode);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLDriver::SetNewListState(bool mode)
{
  //Only set inside a valid context
  if(glContext)
  {
    //Cannot enable/disable a list while in a glBegin/glEnd block
    if(glBeginEndState)
    {
      if(mode)
      {
        LOGERR(("GLDriver::SetNewListState - Enabling a list in a glBegin/glEnd block?"));
      }
      else
      {
        LOGERR(("GLDriver::SetNewListState - Ending a list while in a glBegin/glEnd block?"));
      }
    }
    else
    {
      glNewListState = mode;

      //Enable/disable based on if we are enabling or disabling the mode
      SetInternalGLCallMode(!mode);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLDriver::SetInternalGLCallMode(bool mode)
{
  if(mode)
  {
    //Check the value to catch bad calls
    if(internalCallModeCount >0)
    {
      internalCallModeCount--;
    }
    else
    {
      LOGERR(("GLDriver::SetInternalGLCallMode - INTERNAL ERROR -Inconsistent call mode"));
    }
  }
  else
  {
    internalCallModeCount++;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void * GLDriver::AddExtensionFunction(const string & funcName,void * functionPtr)
{
  //Check that we are init.
  if(!extensionFunction)
  {
    LOGERR(("GLDriver::AddExtensionFunction - No extension function interceptor for function %s",funcName.c_str())); 
    return functionPtr;
  }

  //Pass off to the extension function class
  return extensionFunction->AddFunction(funcName,functionPtr);
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLDriver::AddOverrideFunction(const string & funcName, void * overrideFunctionPtr, void *origFuncPtr)
{
  //Check that we are init.
  if(!extensionFunction)
  {
    LOGERR(("GLDriver::AddOverrideFunction - No extension function interceptor for function %s",funcName.c_str())); 
    return false;
  }

  //Pass off to the extension function class
  return extensionFunction->AddOverrideFunction(funcName,overrideFunctionPtr,origFuncPtr);
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLDriver::LogFunctionPre(uint index, const FunctionArgs & args)
{
  //Check the function table
  if(!functionTable)
  {
    LOGERR(("GLDriver::LogFunctionPre - FunctionTable is NULL")); 
    return false;
  }

  //Get the function data
  const FunctionData *funcData = functionTable->GetFunctionData(index);

  //Check the function data
  if(!funcData)
  {
    LOGERR(("GLDriver::LogFunctionPre - Function data for index %u is NULL",index)); 
    return false;
  }

  //If this is being called before init, indicates no context
  if(!isInit)
  {
    LOGERR(("Function %s is being called before context creation",funcData->GetName().c_str()));
    return false;
  }

  //Do a quick thread check (DT_TODO: Add future support to log OpenGL calls made outside a context)
  if(threadCheckingEnabled && glContext && glContext->GetContextThreadID() != GetActiveThreadID())
  {
    LOGERR(("Function %s is being called on a thread that does not have the main context",funcData->GetName().c_str()));

    //Break if requested
    if(configData.errorBreakOnError)
    {
      //Flag a break point
      ErrorLog::IssueDebuggerBreak();
    }

    return false;    
  }

  //If we have a context, perform context logging
  if(glContext && functionCallDepth == 0)
  {
    glContext->LogFunctionPre(funcData, index, args); 
  }

  //Pass the call to the plugin manager
  if(pluginManager)
  {
    pluginManager->LogFunctionPre(funcData,index,args);
  }

  //Log the function
  if(interceptLog)
  {
    interceptLog->LogFunctionPre(funcData, index, args);
  }

  //Perform pre-error log reporting
  if(configData.errorGetOpenGLChecks && functionCallDepth == 0)
  {
    FunctionErrorCheckPre(funcData, index, args);
  }

  //If this function is a end of frame function, process it
  //DT_TODO: ensure we are on the main frame an not a p-buffer?
  if(funcData->IsFlagSet(FDF_FRAME_FUNC))
  {
    //Flag to any plugins that the end of frame is about to occur
    if(pluginManager)
    {
      pluginManager->LogFrameEndPre(funcData,index,args);
    }
  }

  //If we time functions, set the start time
  if(functionTimeEnabled && functionCallDepth == 0)
  {
    functionTime = 0;
    functionTimer.StartTimer();
  }

  //Increment the function call depth
  functionCallDepth++;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLDriver::LogFunctionPost(uint index, const FunctionRetValue & returnVal)
{
  //Return now if not init (or there was no pre call)
  if(!isInit || functionCallDepth == 0)
  {
    return false;
  }

  //Check the function table
  if(!functionTable)
  {
    LOGERR(("GLDriver::LogFunctionPost - FunctionTable is NULL")); 
    return false;
  }

  //Get the function data
  const FunctionData *funcData = functionTable->GetFunctionData(index);

  //Check the function data
  if(!funcData)
  {
    LOGERR(("GLDriver::LogFunctionPost - Function data for index %u is NULL",index)); 
    return false;
  }

  //Do a quick thread check
  if(threadCheckingEnabled && glContext && glContext->GetContextThreadID() != GetActiveThreadID())
  {
    //Assume the "pre" call logged the error
    return false;    
  }

  //Decrement the function call depth
  functionCallDepth--;

  //If we time functions, get the final time 
  if(functionTimeEnabled && functionCallDepth == 0)
  {
    functionTime = functionTimer.GetTimeDiff();
    
    //Reset the enabled flag
    functionTimeEnabled = false;
  }

  //If we have a context, perform context logging
  if(glContext && functionCallDepth == 0)
  {
    glContext->LogFunctionPost(funcData, index, returnVal);
  }

  //Pass the call to the plugin manager
  if(pluginManager)
  {
    pluginManager->LogFunctionPost(funcData,index,returnVal);
  }

  //Log the function
  if(interceptLog)
  {
    interceptLog->LogFunctionPost(funcData, index, returnVal);
  }

  //Check if we can log internal calls and get error checks are requested
  if(configData.errorGetOpenGLChecks && functionCallDepth == 0)
  {
    FunctionErrorCheckPost(funcData,index);
  }

  //Flag that all processing on the current function is complete
  if(interceptLog)
  {
    interceptLog->LogFunctionComplete();
  }

  //If this function is a end of frame function, process it
  //DT_TODO: ensure we are on the main frame an not a p-buffer?
  if(funcData->IsFlagSet(FDF_FRAME_FUNC))
  {
    ProcessFrameEnd();

    //A post frame swap should only be made at the zero call depth
    if(functionCallDepth != 0)
    {
      LOGERR(("Pre/Post functions out of sync. Use -ThreadChecking=True to check for thread issues"));
    }

    //Flag to any plugins that the end of frame has occured
    if(pluginManager)
    {
      pluginManager->LogFrameEndPost(funcData,index,returnVal);
    }
  }

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
void GLDriver::FunctionErrorCheckPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{
  //If we are reporting extened error log data
  if(configData.errorLogOnError && errorDataLog && configData.errorExtendedLogError)
  {
    //Get all the parameters and the function data
    errorDataLog->GetFunctionString(funcData,index,args,errorFuncString);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLDriver::FunctionErrorCheckPost(const FunctionData *funcData,uint index)
{
  //Return if OpenGL calls cannot be made
  if(!GetInternalGLCallMode())
  {
    return;
  }

  // Abort if this is an OS function call (WGL/GLX etc)
  if(funcData->IsFlagSet(FDF_OS_FUNC))
  {
    return;
  }

  //Get Error code
  GLenum newError;
  newError = GLV.glGetError();

  //If there is an error log and cache it
  if(newError != GL_NO_ERROR)
  {
    //Log the error
    if(interceptLog)
    {
      interceptLog->LogFunctionError(newError);
    }

    //Flag to the plugin manager that an error has occured
    if(pluginManager)
    {
      pluginManager->OnGLError(funcData->GetName().c_str(),index);
    }

    //Log if requested
    if(configData.errorLogOnError && errorDataLog)
    {
      //Get the error value as a string
      string errorString;
      errorDataLog->GetErrorStringValue(newError,errorString);

      //Report the extened error data if available, else just report the function
      if(configData.errorExtendedLogError)
      {
        LOGERR(("GL ERROR - Function %s generated error %s",errorFuncString.c_str(),errorString.c_str()));
      }
      else
      {
        LOGERR(("GL ERROR - Function %s generated error %s",funcData->GetName().c_str(),errorString.c_str()));
      }
    }

    //Break if requested
    if(configData.errorBreakOnError)
    {
      //Flag a break point
      ErrorLog::IssueDebuggerBreak();
    }

    //If the cached error is no-error, assign this error as the new error
    if(glContext && glContext->GetCachedError() == GL_NO_ERROR)
    {
      glContext->SetCachedError(newError);
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
float GLDriver::GetOpenGLVersion()
{
  //Return now if not init / no context
  if(!glContext)
  {
    LOGERR(("GLDriver::GetOpenGLVersion - Unable to determine version number - Not initialized/no context")); 
    return 0.0f;
  }

  return glContext->GetOpenGLVersion();
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLDriver::IsExtensionSupported(const char *extension)
{
  //Return now if not init / no context
  if(!glContext)
  {
    LOGERR(("GLDriver::IsExtensionSupported - Unable to determine if a extension is supported - Not initialized/no context")); 
    return false;
  }

  return glContext->IsExtensionSupported(extension);
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLDriver::InitFunctionLog()
{
  //Delete the existing logger (if any)
  if(interceptLog)
  {
    delete interceptLog;
    interceptLog = NULL;
  }

  //Create the log path if necessary
  if(!FileUtils::CreateFullDirectory(currLogDir))
  {
    LOGERR(("GLDriver::InitFunctionLog - Unable to create directory %s ",currLogDir.c_str())); 

    //If could not create the new directory, just use the dll directory
    currLogDir = dllPath;
  }

  //Create the logger for function output
  if(configData.logXMLFormat)
  {
    //Attempt to copy the xsl file
    if(configData.logXSLFile.length() > 0)
    {
      //Attempt to copy the file
      if(FileUtils::CopyFile(configData.logXSLBaseDir + configData.logXSLFile ,currLogDir + configData.logXSLFile, true))
      {
        //Remove any properties on the newly copied file
        FileUtils::SetFileProperties(currLogDir + configData.logXSLFile,FileUtils::FPROP_NORMAL);

#ifdef GLI_BUILD_WINDOWS

        //Copy the "viewer" .hta file to allow IE users to view the logs without error messages
        FileUtils::CopyFile(configData.logXSLBaseDir + "IEViewLog.hta", currLogDir + "IEViewLog.hta", true);
        FileUtils::SetFileProperties(currLogDir + "IEViewLog.hta", FileUtils::FPROP_NORMAL);

#endif //GLI_BUILD_WINDOWS

        interceptLog = new InterceptLogXML((currLogDir + configData.logName+".xml").c_str(),functionTable,configData,configData.logXSLFile);  
      }
      else
      {
        //Don't use XSL if the file could not be copied
        LOGERR(("Failed to copy XSL file %s",(configData.logXSLBaseDir + configData.logXSLFile).c_str()));
        interceptLog = new InterceptLogXML((currLogDir + configData.logName+".xml").c_str(),functionTable,configData,"");  
      }
    }
    else
    {
      interceptLog = new InterceptLogXML((currLogDir + configData.logName+".xml").c_str(),functionTable,configData,"");
    }
  }
  else
  {
    interceptLog = new InterceptLogText((currLogDir + configData.logName+".txt").c_str(),functionTable,configData);
  }

  //Init the log
  if(!interceptLog->Init())
  {
    LOGERR(("GLDriver::InitFunctionLog - Unable to init the intercept log")); 

    //Delete the log on failure
    delete interceptLog;
    interceptLog = NULL;

    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLDriver::ProcessFrameEnd()
{
  
#ifdef GLI_BUILD_LINUX
  
  //Update the X-Windows keyboard state
  inputUtils.Update(GLW.glXGetCurrentDisplay());  
  
#endif //GLI_BUILD_LINUX
  
  //Increment the frame number
  frameNumber++;

  //Check if we log per-frame
  bool logStateChange =false;
  if(configData.logPerFrame)
  {
    static bool previousState=false;

    //Check if the key state has changed
    if(inputUtils.IsAllKeyDown(configData.logFrameKeys) != previousState)
    {
      previousState = !previousState;

      //Only toggle the logging on the key down
      if(previousState)
      {
        logStateChange = true;
      }
    }

    //If we are only logging one frame at a time
    if(configData.logOneFrameOnly)
    {
      //Disable the logger if it was enabled from a pervious frame
      if(loggingEnabled)
      {
        logStateChange = true;
      }
    }

    for (int i = 0; i < configData.logFrameStartIndices.size(); i++)
    {
      if (frameNumber == configData.logFrameStartIndices[i])
      {
        logStateChange = true;
      }
    }
  }

  //Check if the limit of how many frame can be logged has been reached.
  if(loggingEnabled && configData.logMaxFrameLoggingEnabled && 
     frameNumber - loggingStartFrameNum >= configData.logMaxNumLogFrames)
  {
    logStateChange = true;
  }


  //If a change in per-frame saving occured
  if(logStateChange)
  {
    //If the log is not currently enabled
    if(!loggingEnabled)
    {
      //Flag logging as enabled
      loggingEnabled = true;
      loggingStartFrameNum = frameNumber;

      //Assign the current logging directory
      string dirString;
      StringPrintF(dirString,"Frame_%06u",frameNumber);
      currLogDir = configData.logPath + dirString + FileUtils::dirSeparator;

      //Loop through all the contexts and set all data as dirty
      for(uint i=0;i<glContextArray.size();i++)
      {
        glContextArray[i]->SetLoggerDataDirty();
      }

      //Init the logger
      if(configData.logEnabled)
      {
        InitFunctionLog();
      }

      //Activate the context
      if(glContext)
      {
        glContext->ActivateLoggers(currLogDir);
      }
    }
    else
    {
      // Flag logging as disabled
      loggingEnabled = false;

      //Delete the existing logger (if any)
      if(interceptLog)
      {
        delete interceptLog;
        interceptLog = NULL;
      }

      //Suspend the context
      if(glContext)
      {
        glContext->SuspendLoggers();
      }
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool GLDriver::CreateOpenGLContext(HGLRC rcHandle)
{
  //Check the handle
  if(rcHandle == NULL)
  {
    LOGERR(("GLDriver::CreateOpenGLContext - Attempt to create a NULL context")); 
    return false;
  }

  //Attempt to find an existing context
  if(GetOpenGLContext(rcHandle) != NULL)
  {
    LOGERR(("GLDriver::CreateOpenGLContext - Attempt to create an existing context %p",rcHandle)); 
    return false;
  }

  //Create the new context
  GLContext *newContext = new GLContext(rcHandle,configData,this,functionTable);

  //Add the new context to the array
  glContextArray.push_back(newContext);

  //Flag the new context to the plugin manager
  if(pluginManager)
  {
    pluginManager->OnGLContextCreate(rcHandle);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLDriver::DeleteOpenGLContext(HGLRC rcHandle)
{
  //Attempt to get the context
  GLContext *delContext = GetOpenGLContext(rcHandle);
  if(!delContext)
  {
    LOGERR(("GLDriver::DeleteOpenGLContext - Attempt to delete unknown context %p",rcHandle)); 
    return false;
  }

  //Check if we are deleting the current context
  if(delContext == glContext)
  {
    LOGERR(("GLDriver::DeleteOpenGLContext - Deleting current context %p?",rcHandle)); 

    //Do not perform correct shutdown (as the GL context is not valid)
    glContext = NULL;
  }

  //Remove it from the array
  for(uint i=0;i<glContextArray.size();i++)
  {
    //If the handles match, remove the context
    if(glContextArray[i]->GetRCHandle() == rcHandle)
    {
      glContextArray.erase(glContextArray.begin() + i);
      break;
    }
  }

  //Delete it
  delete delContext;

  //Flag the deleted context to the plugin manager
  if(pluginManager)
  {
    pluginManager->OnGLContextDelete(rcHandle);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLDriver::SetOpenGLContext(HGLRC rcHandle)
{
  //Check that existing contexts are shut down before enabling the new context.
  if(glContext && rcHandle)
  {
    LOGERR(("GLDriver::SetOpenGLContext - Context should only go from NULL->valid and valid->NULL")); 
  }

  HGLRC oldContextHandle = NULL;

  //Shutdown existing context
  if(glContext)
  {
    //If attempting to have multiple rendering contexts active, flag an severe warning
    if(glContext->GetContextThreadID() != GetActiveThreadID())
    {
      LOGERR(("*WARNING* - Multiple rendering contexts cannot be active at once with the current GLIntercept")); 
      LOGERR(("*WARNING* - GLIntercept is not currently thread safe - crashes may occur")); 
    }

    //Check when the state change occurs
    if(internalCallModeCount > 0 || glBeginEndState || glNewListState)
    {
      LOGERR(("GLDriver::SetOpenGLContext - Changing context in the middle of a glBegin/glNewList block?")); 
    }

    //Disable logging
    glContext->SuspendLoggers();

    //Get the render context handle for the old context
    oldContextHandle = glContext->GetRCHandle();

    //Detach the context
    glContext->SetContextInActive();
    glContext = NULL;
  }

  //Attempt to get the context
  if(rcHandle != NULL)
  {
    GLContext *newContext = GetOpenGLContext(rcHandle);
    if(!newContext)
    {
      LOGERR(("GLDriver::SetOpenGLContext - Attempt to set unknown context %p?",rcHandle)); 
      return false;
    }

    //Assign the new context
    glContext = newContext;

    //Assign the thread ID
    glContext->SetContextActive(GetActiveThreadID());

    //If logging is enabled, activate the loggers
    if(loggingEnabled)
    {
      glContext->ActivateLoggers(currLogDir);
    }
  }

  //Flag the changed context to the plugin manager
  if(pluginManager)
  {
    pluginManager->OnGLContextSet(oldContextHandle,rcHandle);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLDriver::ShareOpenGLContext(HGLRC contextA, HGLRC contextB)
{
  GLContext * aContext = GetOpenGLContext(contextA);
  GLContext * bContext = GetOpenGLContext(contextB);

  //If both contexts were retrieved, assign the context data
  if(aContext && bContext)
  {
    //Share the internal list data
    if(!aContext->ShareLists(bContext))
    {
      LOGERR(("ShareOpenGLContext - Unable to share internal lists"));
    }

    //Flag to the plugin manager that the share has occured
    if(pluginManager)
    {
      pluginManager->OnGLContextShareLists(contextA,contextB);
    }
  }
  else
  {
    LOGERR(("ShareOpenGLContext - Unable to get internal contexts"));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
GLContext *GLDriver::GetOpenGLContext(HGLRC rcHandle) const
{
  //Loop for the entire vector
  for(uint i=0;i<glContextArray.size();i++)
  {
    //If the handles match, return the context
    if(glContextArray[i]->GetRCHandle() == rcHandle)
    {
      return glContextArray[i];
    }
  }

  return NULL;
}



