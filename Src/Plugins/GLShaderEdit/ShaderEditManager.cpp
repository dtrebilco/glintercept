#include "ShaderEditManager.h"

#include <ConfigParser.h>
#include <CommonErrorLog.h>
#include <InputUtils.h>

#include "SEContext.h"
#include "SEInterface.h"

USING_ERRORLOG

//The call back ids
enum CallBackIDs
{
  CBI_None =0,
  CBI_glEnd = GENERAL_CALLBACK_INDEX
};

//Path to the dll
extern string dllPath;


///////////////////////////////////////////////////////////////////////////////
//
ShaderEditManager::ShaderEditManager(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),
GLV(callBacks->GetCoreGLFunctions()),
inputUtils(callBacks->GetInputUtils()),
currContext(NULL),
shaderDebugGLSL(NULL),

showShaderUID(0),
showShaderType(USID_Undefined),
showShaderStage(0),
showShaderApplied(false),

breakShaderUID(0),
breakShaderType(USID_Undefined),

oldLogicOpEnable(0),
oldLogicOpMode(0)
{
  //LOGERR(("ShaderEdit Plugin Created"));
  
  //Init the show shader timer
  showShaderTimer.StartTimer(); 

  //Register all the callbacks
  gliCallBacks->RegisterGLFunction("glEnd");

  //Register the user ids for the callbacks
  gliCallBacks->SetFunctionID("glEnd",CBI_glEnd);

  //Parse the config file
  ConfigParser fileParser;
  if(fileParser.Parse(dllPath + "config.ini"))
  {
    ProcessConfigData(&fileParser);
    fileParser.LogUnusedTokens(); 
  }

  //Parse the config string
  ConfigParser stringParser;
  if(stringParser.ParseString(gliCallBacks->GetConfigString()))
  {
    ProcessConfigData(&stringParser);
    stringParser.LogUnusedTokens(); 
  }
}

///////////////////////////////////////////////////////////////////////////////
//
ShaderEditManager::~ShaderEditManager()
{
  //Destroy any created interface
  SEInterface::DestroyInstance();

  //Loop and destroy all contexts
  currContext = NULL;
  for(uint i=0; i<contextArray.size(); i++)
  {
    LOGERR(("ShaderEditManager - Outstanding context %x on shutdown",contextArray[i]->GetRCHandle()));
    delete contextArray[i];
  }
  contextArray.clear();

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::Destroy()
{
  //LOGERR(("ShaderEdit Plugin Destroyed"));
  
  //Destroy this plugin
  delete this;
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::ProcessConfigData(const ConfigParser *parser)
{
  const ConfigToken *testToken;

  //Get the token for the shader editor startup
  testToken = parser->GetToken("ShaderEditorKeys");
  if(testToken)
  {
    //Clear any existing key codes
    enableKeys.clear();

    //Loop for the number of values in the token
    for(uint i=0;i<testToken->GetNumValues();i++)
    {
      string value;
      testToken->Get(value,i);

      //Get the key code of the string
      uint newValue = InputUtils::GetKeyCode(value);
      if(newValue != 0)
      {
        //Add the value to the array
        enableKeys.push_back(newValue);
      }
    }       
  }

}



///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::GLFunctionPre(uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args )
{
  //Ignore pre-glEnd() calls
  if(updateID != CBI_glEnd)
  {
    //Pass all registered called to the context 
    if(currContext)
    {
      currContext->LogFunctionPre(updateID,funcName,funcIndex, args);
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //Handle post glBegin/glEnd blocks
  if(updateID == CBI_glEnd)
  {
    ProcessPostRender();
  }

  //Pass all registered calls to the context 
  else if(currContext)
  {
    currContext->LogFunctionPost(updateID,funcName,funcIndex, retVal);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditManager::StartShaderDebug(UniqueShaderIDRef boundShader, GLRenderCallCache *renderCache, GLFrameBufferCache *frameCache)
{
  //Return now if no render cache or shader editor
  if(!boundShader ||
     renderCache == NULL ||
     frameCache  == NULL ||
     SEInterface::GetInstance() == NULL)
  {
    LOGERR(("ShaderEditManager::StartShaderDebug - Unable to start the shader debugger"));
    return false;
  }

  //Clear out any existing cached commands
  SEInterface::GetInstance()->ProcessCachedMessages();
  if(!SEInterface::GetInstance())
  {
    //The interface can be destroyed while processing messages
    return false;
  }

  //Sanity check that there does not already exist a debug interface
  if(shaderDebugGLSL != NULL)
  {
    LOGERR(("ShaderEditManager::StartShaderDebug - Creating a debug interface while one exists?"));
    
    //Release the existing interface
    shaderDebugGLSL->ReleaseReference();
    shaderDebugGLSL = NULL;
  }

  //Get the program ID and source
  GLuint debugShaderID  = boundShader->GetShaderOpenGLID();
  string debugShaderSrc = boundShader->GetShaderSource();
  
  //If there is a override program, use that instead
  if(boundShader->GetSubstituteShader())
  {
    debugShaderID  = boundShader->GetSubstituteShader()->GetSubstituteProgramID();
    debugShaderSrc = boundShader->GetOverrideShaderSource();
  }

  //Create a shader debugger
  shaderDebugGLSL = ShaderDebugGLSL::CreateInstance(gliCallBacks, debugShaderID, debugShaderSrc, renderCache, frameCache);
  if(!shaderDebugGLSL)
  {
    LOGERR(("ShaderEditManager::StartShaderDebug - Unable to create GLSL shader debugger"));
    return false;
  }

  //Flag to the editor interface that debugging has started
  // (NOTE: The OpenGL ID provided is of the origional program, not an override)
  if(!SEInterface::GetInstance()->SendStartDebugging(boundShader->GetUniqueIndex(), boundShader->GetShaderOpenGLID(), shaderDebugGLSL))
  {
    LOGERR(("ShaderEditManager::StartShaderDebug - Unable to contact editor"));
    return false;
  }
   
  //Loop and process debug commands until no-longer debugging (or interface dies)
  while(SEInterface::GetInstance() && shaderDebugGLSL)
  {
    //Process editor commands
    // Note that when processing commands this calls is re-entrant in the calls that get made
    SEInterface::GetInstance()->ProcessCachedMessages();

    //Let other threads/processes have some time while waiting for debug messages
    ::Sleep(50);
  }

  //Destroy the shader debugger if still active (The interface may have died)
  if(shaderDebugGLSL)
  {
    shaderDebugGLSL->ReleaseReference();
    shaderDebugGLSL = NULL;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditManager::SetPixelDebug(uint xPos, uint yPos, uint pixelOverdraw, string &retLog)
{
  // Ensure debugging is active
  if(!shaderDebugGLSL)
  {
    LOGERR(("ShaderEditManager::SetPixelDebug - No debugger interface to debug on?"));
    retLog += "No debugger interface to debug pixel on";
    return false;
  }

  // Pass to the shader debugger
  return shaderDebugGLSL->SetPixelDebug(xPos, yPos, pixelOverdraw, retLog);
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::EndShaderDebug()
{
  //Release the shader debug interface
  if(shaderDebugGLSL)
  {
    shaderDebugGLSL->ReleaseReference();
    shaderDebugGLSL = NULL;
  }
  else
  {
    LOGERR(("ShaderEditManager::EndShaderDebug - No debug interface to release?"));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::GLRenderPre(const char *funcName, uint funcIndex, const FunctionArgs & args)
{
  //If OpenGL call can be made
  if(gliCallBacks->GetGLInternalCallState())
  {
    //If there is a context, pass the render call to it
    if(currContext)
    {
      currContext->OnRenderPre();

      //If the shader UID is not zero, "show" the shader
      if(showShaderUID != 0 &&
         currContext->IsShaderActive(showShaderUID, showShaderType))
      {
        //Apply the "show" state
        showShaderApplied = ApplyShowShader();
      }

      //If the break shader UID is not zero, "break-on" the shader
      if(breakShaderUID != 0 &&
         currContext->IsShaderActive(breakShaderUID, breakShaderType))
      {
        //Break now (DT_TODO: Do this in a generic way)
        if(IsDebuggerPresent() == TRUE)
        {
          DebugBreak();
        }
      }

    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //If OpenGL call can be made
  if(gliCallBacks->GetGLInternalCallState())
  {
    //If there is a context, pass the render call to it
    // (Note: This will never be called for glBegin-post as OpenGL
    //        calls cannot be made in Begin/End Blocks -see above registeration of glEnd)
    ProcessPostRender();
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::ProcessPostRender()
{
  //If there is a context, pass the end render call to it
  if(currContext && gliCallBacks->GetGLInternalCallState())
  {
    currContext->OnRenderPost();

    //Reset the "show" shader
    if(showShaderApplied)
    {
      //Flag that the shader is not shown
      showShaderApplied = false;

      //Reset the "show" state
      ApplyShowShader(true);
    }

    //Get the current shader and render cache
    GLRenderCallCache  *renderCache = currContext->GetRenderCallCache();
    GLFrameBufferCache *frameCache  = currContext->GetFrameBufferCache();
    const UniqueShaderIDRef &currGLSLShader = currContext->GetBoundGLSLShader();

    //Start shader debugging if a shader breakpoint is set on the current shader
    if(renderCache && frameCache && currGLSLShader && currGLSLShader->GetShaderDebugged())
    {
      //Note that this call will not return until debugging is complete
      if(!StartShaderDebug(currGLSLShader, renderCache, frameCache))
      {
        LOGERR(("ShaderEditManager::GLFunctionPost - Unable to start the shader debugger"));
        //DT_TODO: Log a message to the external app if running
      }
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //Check if the enable key state is down
  if(!SEInterface::GetInstance() && inputUtils->IsAllKeyDown(enableKeys))
  {
    //Attempt to create the instance
    SEInterface::CreateInstance(this, gliCallBacks);
  }

  //If the instance exists, flag that end of frame has occured by processing messages
  if(SEInterface::GetInstance())
  {
    SEInterface::GetInstance()->ProcessCachedMessages();
  }

  if(currContext)
  {
    //Update the bound shaders frame numbers
    currContext->SetShaderFrameNum(gliCallBacks->GetFrameNumber());
  }

  //If there is a shader being show
  if(showShaderUID != 0)
  {
    //Increment the show stage if necessary
    float elapsedTime = (float)((double)showShaderTimer.GetTimeDiff() * 1.0e-06);
    if(elapsedTime > 0.5f)
    {
      showShaderStage = (showShaderStage + 1) % 4;
      
      //Reset the timer
      showShaderTimer.StartTimer();
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::OnGLContextCreate(HGLRC rcHandle)
{
  //Check that there cannot be duplicates
  if(FindContext(rcHandle) != NULL)
  {
    LOGERR(("ShaderEditManager::OnGLContextCreate - Adding existing context %x?",rcHandle));
    return;
  }

  //Create the new context and add it to the list
  SEContext * newContext = new SEContext(rcHandle);
  contextArray.push_back(newContext);
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle)
{
  //Get the context handles
  SEContext * srcContext = FindContext(srcHandle);
  SEContext * dstContext = FindContext(dstHandle);

  //Check that the contexts exists
  if(srcContext == NULL || dstContext == NULL)
  {
    LOGERR(("ShaderEditManager::OnGLContextShareLists - Source and destination contexts not found"));
    return;
  }

  //Share the source lists/shaders with the destination context
  if(!srcContext->ShareLists(dstContext))
  {
    LOGERR(("ShaderEditManager::OnGLContextShareLists - Error sharing lists"));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle)
{
  //If setting a NULL context, just assign and return
  if(newRCHandle == NULL)
  {
    currContext = NULL;
    return;
  }

  //Set the new context handle
  SEContext * setContext = FindContext(newRCHandle);
  if(!setContext)
  {
    LOGERR(("ShaderEditManager::OnGLContextSet - Error finding context %x",newRCHandle));        
  }
  currContext = setContext;
  
  //Init. the context if not currently init
  if(currContext)
  {
    if(!currContext->IsInit())
    {
      currContext->Init(gliCallBacks);
    }

#ifdef GLI_BUILD_WINDOWS
    //Set the last used device context for this render context
    currContext->SetDCHandle(gliCallBacks->GetWGLFunctions()->wglGetCurrentDC());
#endif // GLI_BUILD_WINDOWS

    //Update the bound shaders frame numbers
    currContext->SetShaderFrameNum(gliCallBacks->GetFrameNumber());
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditManager::OnGLContextDelete(HGLRC rcHandle)
{
  //Loop for all contexts
  for(uint i=0; i<contextArray.size(); i++)
  {
    SEContext *delContext = contextArray[i]; 

    //Test if the handles match
    if(delContext->GetRCHandle() == rcHandle)
    {
      //If deleting the current context
      if(delContext == currContext)
      {
        currContext = NULL;
      }
      
      //Delete the context
      delete delContext;
      
      //Erase from the array
      contextArray.erase(contextArray.begin() + i);
      return;
    }
  }

  //Context not found?
  LOGERR(("ShaderEditManager::OnGLContextDelete - Context handle %x not found",rcHandle));
}


///////////////////////////////////////////////////////////////////////////////
//
SEContext *ShaderEditManager::FindContext(HGLRC handle)
{
  //Loop for all contexts
  for(uint i=0; i<contextArray.size(); i++)
  {
    //Test for a match
    if(contextArray[i]->GetRCHandle() == handle)
    {
      return contextArray[i];
    }
  }

  return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditManager::CompileShader(uint shaderID, const string &shaderSrc, string &retLog)
{
  //Do not allow shader compiles while debugging
  if(shaderDebugGLSL != NULL)
  {
    retLog += "Unable to compile shaders while debugging a shader";
    return false;
  }

  //Attempt to find the index
  const UniqueShaderID *foundShader = UniqueShaderID::FindUniqueShaderID(shaderID); 
  if(!foundShader)
  {
    retLog += "Unable to find shader ID - Shader no longer exists?";
    return false;
  }

  //Attempt to compile with the current context
  if(currContext && currContext->FindShader(foundShader))
  {
    return currContext->CompileShader(foundShader,shaderSrc, retLog);
  }

  //Note: This needs testing but difficult to find a test case
  //      as most programs use shared lists.

#ifdef GLI_BUILD_WINDOWS
  // DT_TODO: Update this for linux

  //Attempt to find in all other contexts
  for(uint i=0; i<contextArray.size(); i++)
  {
    SEContext * testContext = contextArray[i]; 

    //If it is in this context, switch to this context to compile
    if(testContext->FindShader(foundShader))
    {
      const WGLDriver * GLW = gliCallBacks->GetWGLFunctions();

      //Get the old context data
      HDC   oldDCHandle = GLW->wglGetCurrentDC();
      HGLRC oldRCHandle = GLW->wglGetCurrentContext();

      //Activate the context with OpenGL
      // NOTE: This will cause problems if in future GLIntercept supports
      //       multiple rendering contexts being active at once. 
      //      (ie. this context could already be active on a another thread)
      //       This also assumes that the last device context used is still valid.
      if(GLW->wglMakeCurrent(testContext->GetDCHandle(), testContext->GetRCHandle()))
      {
        bool retValue = testContext->CompileShader(foundShader,shaderSrc, retLog);

        //Reset the context
        GLW->wglMakeCurrent(oldDCHandle, oldRCHandle);
        return retValue;
      }
      else
      {
        LOGERR(("ShaderEditManager::CompileShader - Unable to set render context"));
        return false;
      }
    }
  }

#endif // GLI_BUILD_WINDOWS

  retLog += "Unable to find shader in an available context";
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditManager::ApplyShowShader(bool revert)
{
  if(!revert)
  {
    //Note: Think about perhaps using the blend mode instead of logic op
    //      (so float render targets will work) Ensure to take into account the seperate
    //      blend modes (color and alpha) when saving the current state
    GLenum setLogicOp = GL_SET;
    if(showShaderStage % 2 == 0)
    {
      setLogicOp = GL_COPY_INVERTED;
    }
    else 
    {
      //Have to set this in case the app is already using 
      //  GL_COPY_INVERTED to render
      setLogicOp = GL_COPY;
    }

    //Enable logic ops
    GLV->glGetIntegerv(GL_COLOR_LOGIC_OP,&oldLogicOpEnable);
    if(oldLogicOpEnable != GL_TRUE)
    {
      GLV->glEnable(GL_COLOR_LOGIC_OP);
    }

    //Set the logic mode to output pure white
    GLV->glGetIntegerv(GL_LOGIC_OP_MODE,&oldLogicOpMode);
    GLV->glLogicOp(setLogicOp);

    return true;
  }
  else
  {
    //Reset logic ops
    GLV->glLogicOp(oldLogicOpMode);
    if(oldLogicOpEnable != GL_TRUE)
    {
      GLV->glDisable(GL_COLOR_LOGIC_OP);
    }

    return true;
  }
}






