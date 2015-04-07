/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __GL_RENDER_CALL_CACHE_H_
#define __GL_RENDER_CALL_CACHE_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include <ReferenceCount.h>

class GLDriver;


//@
//  Summary:
//    This class holds a previous OpenGL render call and can re-issue the
//    render call on demand.
//  
class GLRenderCallCache : public ReferenceCount
{
public:

  //@
  //  Summary:
  //    To create and initialize a render call cache.
  //  
  //  Parameters:
  //    callBacks  - The callbacks to GLIntercept.
  //
  //  Returns:
  //    If a interface could be created and init, it is returned. 
  //    Else NULL is returned.
  //
  static GLRenderCallCache * CreateInstance(InterceptPluginCallbacks *callBacks);

  //@
  //  Summary:
  //    To cache a OpenGL render call.
  //  
  //  Parameters:
  //    updateID  - The render call registered ID.
  //
  //    funcName  - The render call name.
  //
  //    args      - The arguments passed to the render call.
  //
  //  Returns:
  //    If the render call can be cached, true is returned. Else false is
  //    returned.
  //
  bool SetRenderCall(uint updateID, const char *funcName, const FunctionArgs & args);

  //@
  //  Summary:
  //    To issue a cached render call. If no render call is cached, no call
  //    is made.
  //  
  void RenderCall() const;

  //@
  //  Summary:
  //    To determine if there is an existing render call cached.
  //  
  //  Returns:
  //    If a call is cached, true is returned. Else false is returned.
  //
  inline bool IsRenderCallSet() const;

  //@
  //  Summary:
  //    To clear an existing render call.
  //  
  inline void ClearRenderCall();

protected:

  InterceptPluginCallbacks *gliCallBacks;  // The callback interface into GLIntercept
  const GLCoreDriver       *GLV;           // The core OpenGL driver

  bool   isRenderCallSet;                  // Flag indicating if the render call is set
  uint   currRenderType;                   // The current render type

  //@
  //  Summary:
  //    Constructor
  //
  //  Parameters:
  //    gliCallBacks  - The callbacks to GLIntercept.
  //
  GLRenderCallCache(InterceptPluginCallbacks *callBacks);

  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~GLRenderCallCache();

  //@
  //  Summary:
  //    To initialize constants and arrays etc.
  //  
  //  Returns:
  //    If initialization was successful, true is returned. Else false is returned.
  //
  bool Init();

private:

  //Extension entry points
  void (GLAPIENTRY *iglDrawRangeElements) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
  void (GLAPIENTRY *iglMultiDrawArrays) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
  void (GLAPIENTRY *iglMultiDrawElements) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount);

  //Cache render variables
  GLenum  iMode;
  GLenum  iType;

  GLint   iFirst;

  GLuint  iStart;
  GLuint  iEnd;

  GLsizei iCount;
  GLsizei iPrimcount;

  const GLvoid  *iPtrIndices;
  const GLvoid **iPtrPtrIndices;
  const GLint   *iPtrFirst;
  const GLsizei *iPtrCount; 
  
};


///////////////////////////////////////////////////////////////////////////////
//
inline bool GLRenderCallCache::IsRenderCallSet() const
{
  return isRenderCallSet;
}


///////////////////////////////////////////////////////////////////////////////
//
inline void GLRenderCallCache::ClearRenderCall()
{
  isRenderCallSet = false;
  currRenderType  = 0;
}



#endif // __GL_RENDER_CALL_CACHE_H_
