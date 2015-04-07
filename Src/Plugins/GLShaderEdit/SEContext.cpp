/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "SEContext.h"


///////////////////////////////////////////////////////////////////////////////
//
SEContext::SEContext(HGLRC rcHandle):
isInit(false),
glRCHandle(rcHandle),

#ifdef GLI_BUILD_WINDOWS
dcHandle(NULL),
#endif // GLI_BUILD_WINDOWS

shaderEditASM(NULL),
shaderEditGLSL(NULL),
renderCallCache(NULL),
frameBufferCache(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////
//
SEContext::~SEContext()
{
  //Delete the ASM shader edit
  if(shaderEditASM)
  {
    shaderEditASM->ReleaseReference();
  }

  //Delete the GLSL shader edit
  if(shaderEditGLSL)
  {
    shaderEditGLSL->ReleaseReference();
  }

  //Delete the debug caches
  if(renderCallCache)
  {
    renderCallCache->ReleaseReference();
  }
  if(frameBufferCache)
  {
    frameBufferCache->ReleaseReference();
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void SEContext::Init(InterceptPluginCallbacks *gliCallBacks)
{
  //If we can make GL calls and have not been initialized previously
  if(gliCallBacks->GetGLInternalCallState() && !isInit)
  {
    //Note: These managers may already be assigned if sharelists 
    //      was called previously

    //Determine if ASM shaders are supported
    if(!shaderEditASM)
    {
      shaderEditASM  = ShaderEditASM::CreateInstance(gliCallBacks);
    }

    //Determine if GLSL shaders are supported
    if(!shaderEditGLSL)
    {
      shaderEditGLSL = ShaderEditGLSL::CreateInstance(gliCallBacks); 
    }

    //Create a render call cache manager
    if(!renderCallCache)
    {
      renderCallCache = GLRenderCallCache::CreateInstance(gliCallBacks);
    }

    //Create a frame buffer cache
    if(!frameBufferCache)
    {
      frameBufferCache = GLFrameBufferCache::CreateInstance(gliCallBacks);
    }

    //Flag that we are initialized
    isInit = true;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void SEContext::LogFunctionPre(uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args)
{
  //Test for ASM  callbacks
  if(updateID >= SHADER_ASM_CALLBACK_INDEX &&
     updateID <= SHADER_ASM_CALLBACK_MAX)
  {
    //Do the ASM shader edit
    if(shaderEditASM)
    {
      shaderEditASM->LogFunctionPre(updateID,funcName,funcIndex,args);
    }
  }

  //Test for GLSL callbacks
  else if(updateID >= SHADER_GLSL_CALLBACK_INDEX &&
          updateID <= SHADER_GLSL_CALLBACK_MAX)
  {
    //Else do the GLSL shader log
    if(shaderEditGLSL)
    {
      shaderEditGLSL->LogFunctionPre(updateID,funcName,funcIndex,args, this);
    }
  }

  //Process cache any render calls
  else if(renderCallCache != NULL &&
          updateID >= RENDER_CALL_CALLBACK_INDEX &&
          updateID <= RENDER_CALL_CALLBACK_MAX)
  {
    // If debugging, store the render call
    if(boundGLSLShader && boundGLSLShader->GetShaderDebugged())
    {
      renderCallCache->SetRenderCall(updateID, funcName, args);
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void SEContext::LogFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //Test for ASM  callbacks
  if(updateID >= SHADER_ASM_CALLBACK_INDEX &&
     updateID <= SHADER_ASM_CALLBACK_MAX)
  {
    //Do the shader edit
    if(shaderEditASM)
    {
      shaderEditASM->LogFunctionPost(updateID,funcName,funcIndex,retVal);
    }
  }

  //Test for GLSL callbacks
  else if(updateID >= SHADER_GLSL_CALLBACK_INDEX &&
          updateID <= SHADER_GLSL_CALLBACK_MAX)
  {
    // Else do the GLSL shader edit
    if(shaderEditGLSL)
    {
      shaderEditGLSL->LogFunctionPost(updateID,funcName,funcIndex,retVal,this);
    }
  }

  //Clear any cached render calls
  else if(renderCallCache != NULL &&
          updateID >= RENDER_CALL_CALLBACK_INDEX &&
          updateID <= RENDER_CALL_CALLBACK_MAX)
  {
    //If a shader is bound
    if(boundGLSLShader)
    {
      //Clear any cached render calls
      renderCallCache->ClearRenderCall();
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool SEContext::ShareLists(SEContext *shareContext) const
{
  // Check the context
  if(shareContext == NULL)
  {
    return false;
  }
  if(shareContext == this)
  {
    return true;
  }

  //Remove old shaders from the incomming context
  if(shareContext->shaderEditASM)
  {
    shareContext->shaderEditASM->ReleaseReference();
    shareContext->shaderEditASM = NULL;
  }
  if(shareContext->shaderEditGLSL)
  {
    shareContext->shaderEditGLSL->ReleaseReference();
    shareContext->shaderEditGLSL = NULL;
  }

  //Assign our shader editors
  if(shaderEditASM)
  {
    shaderEditASM->AddReference();
    shareContext->shaderEditASM = shaderEditASM;
  }
  if(shaderEditGLSL)
  {
    shaderEditGLSL->AddReference();
    shareContext->shaderEditGLSL = shaderEditGLSL;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void SEContext::OnRenderPre()
{
  // If debugging 
  if(boundGLSLShader && boundGLSLShader->GetShaderDebugged())
  {
    //Cache the frame buffer data
    frameBufferCache->CacheBuffersPre();
  }

  //If there is a substitute GLSL shader set, bind it.
  if(boundGLSLShader && boundGLSLShader->GetSubstituteShader())
  {
    boundGLSLShader->GetSubstituteShader()->BindSubstitute();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void SEContext::OnRenderPost()
{
  //If there is a substitute shader set, un-bind it.
  if(boundGLSLShader && boundGLSLShader->GetSubstituteShader())
  {
    boundGLSLShader->GetSubstituteShader()->UnBindSubstitute();
  }

  // If debugging 
  if(boundGLSLShader && boundGLSLShader->GetShaderDebugged())
  {
    //Cache the frame buffer data
    frameBufferCache->CacheBuffersPost();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool SEContext::FindShader(const UniqueShaderID *findShader)
{
  //Pass to the shader editors/trackers
  if(shaderEditASM && shaderEditASM->FindShader(findShader))
  {
    return true;
  }

  if(shaderEditGLSL && shaderEditGLSL->FindShader(findShader))
  {
    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
bool SEContext::CompileShader(const UniqueShaderID *findShader, const string &shaderSrc, string &retLog)
{
  //Pass to the shader editors to compile
  if(shaderEditASM && shaderEditASM->FindShader(findShader))
  {
    return shaderEditASM->AddSubstituteProgram(findShader->GetShaderOpenGLID(), shaderSrc, retLog);
  }

  if(shaderEditGLSL && shaderEditGLSL->FindShader(findShader))
  {
    return shaderEditGLSL->AddSubstituteShader(findShader->GetShaderOpenGLID(), shaderSrc, retLog);
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
void SEContext::SetShaderFrameNum(uint newFrameNum)
{
  //If there is a GLSL shader bound, update it
  if(boundGLSLShader)
  {
    boundGLSLShader->SetLastFrameUsed(newFrameNum);
  }

  //If there is a ASM shader editor, pass the call for it to 
  //  update the bound shaders
  if(shaderEditASM)
  {
    shaderEditASM->SetShaderFrameNum(newFrameNum);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
bool SEContext::IsShaderActive(uint shaderUID, USIDType shaderType)
{
  //If the bound GLSL shader matches, return true
  if(shaderType == USID_GLSLProgram && boundGLSLShader &&
     boundGLSLShader->GetUniqueIndex() == shaderUID)
  {
    return true;
  }

  //Pass to the ASM shader editor
  if(shaderEditASM)
  {
    return shaderEditASM->IsShaderActive(shaderUID, shaderType);
  }

  return false;

}












