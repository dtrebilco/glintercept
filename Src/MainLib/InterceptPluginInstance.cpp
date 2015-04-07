/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptPluginInstance.h"
#include "InterceptPluginManager.h"
#include "InterceptPluginDLLInstance.h"
#include "GLDriver.h"
#include <stdarg.h>
#include <string.h>

//The real OpenGL driver
extern GLCoreDriver GLV;
extern WGLDriver    GLW;

USING_ERRORLOG

//A dummy interface - assigned when the plugin has to be unloaded but the
//  interface cannot be destroyed.
class DummyPluginInterface : public InterceptPluginInterface
{
public:
  DummyPluginInterface(){};
  virtual void GLIAPI GLFunctionPre (uint userIndex, const char *funcName, uint funcIndex, const FunctionArgs & args ) {};
  virtual void GLIAPI GLFunctionPost(uint userIndex, const char *funcName, uint funcIndex, const FunctionRetValue & retVal) {};
  virtual void GLIAPI GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args )  {};
  virtual void GLIAPI GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal) {};
  virtual void GLIAPI GLRenderPre (const char *funcName, uint funcIndex, const FunctionArgs & args )   {};
  virtual void GLIAPI GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)   {};
  virtual void GLIAPI OnGLError(const char *funcName, uint funcIndex) {};
  virtual void GLIAPI OnGLContextCreate(HGLRC rcHandle) {};
  virtual void GLIAPI OnGLContextDelete(HGLRC rcHandle) {};
  virtual void GLIAPI OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle)    {};
  virtual void GLIAPI OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle) {};
  virtual void GLIAPI Destroy()
  {
    delete this;
  }
};



///////////////////////////////////////////////////////////////////////////////
//
InterceptPluginInstance::InterceptPluginInstance(InterceptPluginManager *manager, GLDriver *ogldriver,InterceptPluginDLLInstance *newDLLInstance):
driver(ogldriver),
pluginManager(manager),
contextFunctionCalls(true),
hasRegisteredFunctions(false),
gliPlugin(NULL),
dllInstance(newDLLInstance)
{
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptPluginInstance::~InterceptPluginInstance()
{
  //Destroy the plugin
  if(gliPlugin)
  {
    //Ensure destroy is not called twice
    InterceptPluginInterface *destroyPlugin = gliPlugin;
    gliPlugin = NULL; 

    destroyPlugin->Destroy();
  }

  //Destroy the plugin loader (needs to be done after the plugin is destroyed)
  if(dllInstance)
  {
    dllInstance->ReleaseReference();    
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginInstance::Init(const char *pluginName, const string &newConfigString)
{
  //Assign the configuration string
  configString = newConfigString;

  //Init the plugin
  gliPlugin = dllInstance->CreateLogPlugin(pluginName, this);
  if(gliPlugin == NULL)
  {
    LOGERR(("InterceptPluginInstance::Init - Unable to init plugin %s in %s",pluginName,dllInstance->GetDLLFileName().c_str()));
    return false;
  }

  //Do not do anything here that the plugin needs 
  //  (do before init so plugin can access in constructor)

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
const char * InterceptPluginInstance::GetConfigString()
{
  return configString.c_str();
}

///////////////////////////////////////////////////////////////////////////////
//
float InterceptPluginInstance::GetGLVersion()
{
  return driver->GetOpenGLVersion();
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginInstance::IsGLExtensionSupported(const char *extensionName)
{
  return driver->IsExtensionSupported(extensionName);
}


///////////////////////////////////////////////////////////////////////////////
//
void *InterceptPluginInstance::GetGLFunction(const char *functionName)
{
  //Ensure OpenGL calls can be made
  if(driver->GetInternalGLCallMode())
  {
    return GLW.glGetProcAddress(functionName);
  }
 
  return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginInstance::RegisterGLFunction(const char *functionName)
{
  pluginManager->RegisterGLFunction(this,functionName);
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginInstance::UnRegisterGLFunction(const char *functionName)
{
  pluginManager->UnRegisterGLFunction(this,functionName);
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginInstance::SetFunctionID(const char *functionName, uint newID)
{
  return pluginManager->SetFunctionID(this,functionName,newID);
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginInstance::SetContextFunctionCalls(bool enable)
{
  //Assign the flag
  contextFunctionCalls = enable;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginInstance::GetGLArgString(uint funcIndex, const FunctionArgs & args, uint strLength, char *retString)
{
  return pluginManager->GetGLArgString(funcIndex, args, strLength, retString);
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginInstance::GetGLReturnString(uint funcIndex, const FunctionRetValue & retVal, uint strLength, char *retString)
{
  return pluginManager->GetGLReturnString(funcIndex, retVal, strLength, retString);
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginInstance::GetGLInternalCallState()
{
  return driver->GetInternalGLCallMode();
}


///////////////////////////////////////////////////////////////////////////////
//
uint InterceptPluginInstance::GetFrameNumber()
{
  return driver->GetFrameNumber();
}


///////////////////////////////////////////////////////////////////////////////
//
uintptr_t InterceptPluginInstance::GetThreadID()
{
  return GetActiveThreadID();
}


///////////////////////////////////////////////////////////////////////////////
//
HGLRC InterceptPluginInstance::GetGLContextID()
{
  GLContext *currContext = driver->GetCurrentContext();

  //If there is a current context
  if(currContext)
  {
    return currContext->GetRCHandle();
  }

  return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
void GLI_CDECL PluginLogErrorMessage(const char *errorMessage,...)
{
  //Global function to handle plugin error messages
  if(gliLog)
  {
    //Get the variable list
    va_list marker;
    va_start(marker,errorMessage);     
  
    //Call the error log
    gliLog->LogErrorArgs(errorMessage,marker);

    va_end( marker );              
  };
}

///////////////////////////////////////////////////////////////////////////////
//
LOGERRPROC InterceptPluginInstance::GetLogErrorFunction()
{
  return PluginLogErrorMessage;
}


///////////////////////////////////////////////////////////////////////////////
//
const GLCoreDriver * InterceptPluginInstance::GetCoreGLFunctions()
{
  return &GLV;
}

///////////////////////////////////////////////////////////////////////////////
//
const WGLDriver * InterceptPluginInstance::GetWGLFunctions()
{
  return &GLW;
}

///////////////////////////////////////////////////////////////////////////////
//
const InputUtils * InterceptPluginInstance::GetInputUtils()
{
  return driver->GetInputUtils();
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginInstance::AddOverrideFunction(const char * funcName, void * overrideFunctionPtr, void *origFuncPtr)
{
  //Flag to not unload the DLL until shutdown
  hasRegisteredFunctions = true;

  return driver->AddOverrideFunction(funcName, overrideFunctionPtr, origFuncPtr);
}


///////////////////////////////////////////////////////////////////////////////
//
LoggerMode InterceptPluginInstance::GetLoggerMode() const
{
  return driver->GetLoggerMode();
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginInstance::GetLoggerPath(uint strLength, char *retString)
{
  //If no logging is on, return now
  if(driver->GetLoggerMode() == LM_None)
  {
    return false;
  }

  //Get the current log path (and check that there is enough write space)
  string currentPath = driver->GetLoggerPath();
  if(currentPath.length()+1 > strLength)
  {
    return false;
  }

  //Assign the string
  strncpy(retString, currentPath.c_str(), currentPath.length());
  retString[currentPath.length()] = '\0';

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginInstance::GetLogResourceFilename(LogResourceType type, uint typeID, uint strLength, char *retString)
{
  //If no logging is on, return now
  if(driver->GetLoggerMode() == LM_None)
  {
    return false;
  }

  //Ensure there is a current context
  GLContext *currContext = driver->GetCurrentContext();
  if(!currContext)
  {
    return false;
  }

  //Get the string based on the type
  string resourceFilename;
  switch(type)
  {
    case(LRT_Texture):
      {
        ImageSaveFiles imageFileNames;

        //Get the filenames for the image
        if(currContext->GetTextureFileNames(typeID, imageFileNames) &&
           imageFileNames.imageFileNames.size() > 0)
        {
          //Just use the primary filename
          resourceFilename = imageFileNames.imageFileNames[0];
        }
      }
      break;

    case(LRT_ShaderASM):
      currContext->GetShaderFileName(typeID, resourceFilename);
      break;

    case(LRT_ShaderGLSL): 
      currContext->GetShaderGLSLFileName(typeID, InterceptShaderGLSL::GLSLRT_GLSL_Shader, resourceFilename);
      break;
    case(LRT_ProgramGLSL):
      currContext->GetShaderGLSLFileName(typeID, InterceptShaderGLSL::GLSLRT_GLSL_Program, resourceFilename);
      break;


    case(LRT_DisplayList):
      currContext->GetDisplayListFileName(typeID, resourceFilename);
      break;

    case(LRT_ColorBuffer):
      {
        FrameInterceptFileNames frameFileNames;
 
        //Get all the frame filenames 
        if(currContext->GetFrameFileNames(frameFileNames))
        {
          //Just return the "post" filename for now (add new enums if needed)
          if(typeID < frameFileNames.numColorBuffers)
          {
            resourceFilename = frameFileNames.colorBufNames[typeID][FIDT_POST_FRAME];
          }
        }
      }
      break;
    case(LRT_StencilBuffer):
      {
        FrameInterceptFileNames frameFileNames;
 
        //Get all the frame filenames 
        if(currContext->GetFrameFileNames(frameFileNames))
        {
          //Just return the "post" filename for now (add new enums if needed)
          resourceFilename = frameFileNames.stencilBufNames[FIDT_POST_FRAME];
        }
      }
      break;
    case(LRT_DepthBuffer):
      {
        FrameInterceptFileNames frameFileNames;
 
        //Get all the frame filenames 
        if(currContext->GetFrameFileNames(frameFileNames))
        {
          //Just return the "post" filename for now (add new enums if needed)
          resourceFilename = frameFileNames.depthBufNames[FIDT_POST_FRAME];
        }
      }
      break;
  }

  //Strip the path
  string filePath = driver->GetLoggerPath();
  if(resourceFilename.find(filePath,0) == 0)
  {
    //Strip the path
    resourceFilename.erase(0,filePath.size());
  }

  //If there is no resource, (or there is no room to store it) return
  if(resourceFilename.length() == 0 || 
     resourceFilename.length()+1 > strLength)
  {
    return false;
  }

  //Assign the string
  strncpy(retString, resourceFilename.c_str(), resourceFilename.length());
  retString[resourceFilename.length()] = '\0';

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginInstance::AddLoggerString(const char *addString)
{
  driver->AddLoggerString(addString);
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptPluginInstance::DestroyPlugin()
{
  //Delete the interface
  if(gliPlugin)
  {
    //Ensure destroy is not called twice
    InterceptPluginInterface *destroyPlugin = gliPlugin;
    
    //Assign a dummy interface
    gliPlugin = new DummyPluginInterface; 

    //Destroy the old plugin
    destroyPlugin->Destroy();
  }

  //Only fully delete the DLL if there are no registered functions contained within it
  if(!hasRegisteredFunctions)
  {
    //Flag to the manager to remove this plugin. 
    //  (This will happen in the future as it is likely this method is 
    //   called during the plugin array iteration)
    return pluginManager->FlagPluginRemoval(this);
  }
   
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptPluginInstance::OnDLLUnload(InterceptPluginDLLInstance *deleteDLLInstance)
{
  //Return now if the instance is not "this" plugins' dll
  if(deleteDLLInstance != dllInstance)
  {
    return;
  }

  //Delete the interface
  if(gliPlugin)
  {
    //Ensure destroy is not called twice
    InterceptPluginInterface *destroyPlugin = gliPlugin;

    //Assign a dummy interface
    gliPlugin = new DummyPluginInterface; 
   
    //Destroy the old plugin
    destroyPlugin->Destroy();
  }

  //Flag for removal
  pluginManager->FlagPluginRemoval(this);
}
