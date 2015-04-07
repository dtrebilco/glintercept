/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "GLRenderCallCache.h"
#include "SEContext.h"
#include <CommonErrorLog.h>

USING_ERRORLOG

//DT_TODO: Ensure to test each of these draw calls (new humus demo!)

//The call back ids
enum CallBackIDs
{
  CBI_glDrawArrays = RENDER_CALL_CALLBACK_INDEX,
  CBI_glDrawElements,

  CBI_glDrawRangeElements,
  CBI_glMultiDrawArrays,
  CBI_glMultiDrawElements,

};

///////////////////////////////////////////////////////////////////////////////
//
GLRenderCallCache::GLRenderCallCache(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),
GLV(callBacks->GetCoreGLFunctions()),

isRenderCallSet(false),
currRenderType(0),

iglDrawRangeElements(NULL),
iglMultiDrawArrays(NULL),
iglMultiDrawElements(NULL),

iMode(0),
iType(0),
iFirst(0),
iStart(0),
iEnd(0),
iCount(0),
iPrimcount(0),

iPtrIndices(NULL),
iPtrPtrIndices(NULL),
iPtrFirst(NULL),
iPtrCount(NULL) 
{

}

///////////////////////////////////////////////////////////////////////////////
//
GLRenderCallCache::~GLRenderCallCache()
{
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLRenderCallCache::Init()
{
  //Return if we cannot make OpenGL calls
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return false;
  }

  //Test extension support
  float glVersion = gliCallBacks->GetGLVersion();
  if(glVersion < 1.4f)
  {
    LOGERR(("GLRenderCallCache::Init - Need OpenGL 1.4 implementation to use render call cache"));
    return false;
  }
 

  void **loadFunc;

  //Helper define for function lookups

#define GL_FUNC_LOOKUP(string)                           \
  loadFunc  = (void**)(&i##string);                      \
  *loadFunc = gliCallBacks->GetGLFunction(#string);      \
  if(*loadFunc == NULL)                                  \
  {                                                      \
    LOGERR(("Function %s unable to be mapped",#string)); \
    return false;                                        \
  }                                                      

  //Init function pointers
  GL_FUNC_LOOKUP(glDrawRangeElements);
  GL_FUNC_LOOKUP(glMultiDrawArrays);
  GL_FUNC_LOOKUP(glMultiDrawElements);

#undef GL_FUNC_LOOKUP


#define FUNC_REGISTER(function, token)        \
  gliCallBacks->RegisterGLFunction(function); \
  gliCallBacks->SetFunctionID(function, token); 

  FUNC_REGISTER("glDrawArrays",  CBI_glDrawArrays);
  FUNC_REGISTER("glDrawElements",CBI_glDrawElements);

  FUNC_REGISTER("glDrawRangeElements",   CBI_glDrawRangeElements);
  FUNC_REGISTER("glDrawRangeElementsEXT",CBI_glDrawRangeElements);

  FUNC_REGISTER("glMultiDrawArrays",   CBI_glMultiDrawArrays);
  FUNC_REGISTER("glMultiDrawArraysEXT",CBI_glMultiDrawArrays);

  FUNC_REGISTER("glMultiDrawElements",   CBI_glMultiDrawElements);
  FUNC_REGISTER("glMultiDrawElementsEXT",CBI_glMultiDrawElements);
  
#undef FUNC_REGISTER

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
GLRenderCallCache *GLRenderCallCache::CreateInstance(InterceptPluginCallbacks *callBacks)
{
  //Create the new render cache
  GLRenderCallCache * newRenderCache = new GLRenderCallCache(callBacks);

  //Attempt to init
  if(!newRenderCache->Init())
  {
    LOGERR(("GLRenderCallCache::CreateInstance - Unable to init new render cache"));
    newRenderCache->ReleaseReference();
    return NULL;
  }

  return newRenderCache;
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLRenderCallCache::SetRenderCall(uint updateID, const char *funcName, const FunctionArgs & args)
{
  //Check if setting a new render call while one is "active"
  if(isRenderCallSet)
  {
    LOGERR(("GLRenderCallCache::SetRenderCall - Setting a render call while an existing call is active?")); 
    isRenderCallSet = false;
  }

  //Create a access copy of the arguments
  FunctionArgs newArgs(args);

  //Switch based on the render type
  switch(updateID)
  {
    case(CBI_glDrawArrays) :
      newArgs.Get(iMode);
      newArgs.Get(iFirst);
      newArgs.Get(iCount);
      break;

    case(CBI_glDrawElements) :
      newArgs.Get(iMode);
      newArgs.Get(iCount);
      newArgs.Get(iType);
      newArgs.Get(iPtrIndices);
      break;

    case(CBI_glDrawRangeElements) :
      newArgs.Get(iMode);
      newArgs.Get(iStart);
      newArgs.Get(iEnd);
      newArgs.Get(iCount);
      newArgs.Get(iType);
      newArgs.Get(iPtrIndices);
      break;

    case(CBI_glMultiDrawArrays) :
      newArgs.Get(iMode);
      newArgs.Get(iPtrFirst);
      newArgs.Get(iPtrCount);
      newArgs.Get(iPrimcount);
      break;

    case(CBI_glMultiDrawElements) :
      newArgs.Get(iMode);
      newArgs.Get(iPtrCount);
      newArgs.Get(iType);
      newArgs.Get(iPtrIndices);
      newArgs.Get(iPrimcount);

      //Manually assign the pointer to index arrays
      iPtrPtrIndices = (const GLvoid **)iPtrIndices;
      break;

    default:
      LOGERR(("GLRenderCallCache::SetRenderCall - Unknown render call %s",funcName));
      return false;
      break;
  }

  //Store the current render type
  currRenderType = updateID;
  
  //Flag that a rendre call is set
  isRenderCallSet = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLRenderCallCache::RenderCall() const
{
  //Return now if no render call is active
  if(!isRenderCallSet)
  {
    LOGERR(("GLRenderCallCache::RenderCall - Attempt to render when no render call is set!")); 
    return;
  }

  //Switch based on the render type
  switch(currRenderType)
  {
    case(CBI_glDrawArrays) :
      GLV->glDrawArrays(iMode, iFirst, iCount);
      break;

    case(CBI_glDrawElements) :
      GLV->glDrawElements(iMode, iCount, iType, iPtrIndices);
      break;

    case(CBI_glDrawRangeElements) :
      iglDrawRangeElements(iMode, iStart, iEnd, iCount, iType, iPtrIndices);
      break;

    case(CBI_glMultiDrawArrays) :
      iglMultiDrawArrays(iMode, iPtrFirst, iPtrCount, iPrimcount);
      break;

    case(CBI_glMultiDrawElements) :
      iglMultiDrawElements(iMode, iPtrCount, iType, iPtrPtrIndices, iPrimcount);
      break;

    default:
      LOGERR(("GLRenderCallCache::RenderCall - Unknown render call")); 
      break;
  }

}










