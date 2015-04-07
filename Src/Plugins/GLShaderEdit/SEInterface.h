/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SE_INTERFACE_H_
#define __SE_INTERFACE_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include "ReferenceCount.h"
#include "ShaderManagerASM.h"

#include <string>
#include <vector>

using namespace std;

class ShaderEditManager;
class ShaderDebugGLSL;

//@
//  Summary:
//    This class acts as an interface to an external editor of shaders.
//    The singleton instance will create the external process on initialization
//    and the singleton will be destroyed when the external editor is 
//    destroyed. (ie. so check for existance before using)
//  
class SEInterface
{
public:

  //@
  //  Summary:
  //    To create and initialize the interface that is used to communicate with
  //    an external editor.
  //  
  //  Parameters:
  //    manager    - The manager that uses this instance.
  //
  //    callBacks  - The callbacks to GLIntercept.
  //
  //  Returns:
  //    If a interface could be created and init, it is returned. 
  //    Else NULL is returned.
  //
  static SEInterface * CreateInstance(ShaderEditManager *manager, InterceptPluginCallbacks *callBacks);

  //@
  //  Summary:
  //    To retrieve the external editor interface.
  //  
  //  Returns:
  //    If a interface has been init with CreateInstance (and not been destroyed) it is returned. 
  //    Else NULL is returned.
  //
  static SEInterface * GetInstance();

  //@
  //  Summary:
  //    To destroy the singleton instance when the manager becomes invalid.
  //  
  static void DestroyInstance();

  //@
  //  Summary:
  //    To process any editor messages. Typically called on the end of the
  //    current frame.
  //
  //  Note: The interface may be destroyed at the end of this call if 
  //        the editor has been destroyed.  
  //  
  void ProcessCachedMessages();

  //@
  //  Summary:
  //    To flag to the shader editor that a shader has entered debug mode.
  //  
  //  Parameters:
  //    debugShaderUID - The unique ID of the shader being debugged.
  //
  //    origShaderID   - The ID of the origional OpenGL program. 
  //                     (even if an override program is in use)
  //
  //    shaderDebug    - The shader debug interface to extract the data from.
  //
  //  Returns:
  //    If the data was sent successfully, true is returned. Else false is
  //    returned.
  //
  bool SendStartDebugging(uint debugShaderUID, uint origShaderID, ShaderDebugGLSL * shaderDebug);

protected:

  static SEInterface *seiInstance;                // The singleton instance.

  ShaderEditManager        *manager;              // The manager of the interface
  InterceptPluginCallbacks *gliCallbacks;         // The callbacks to glIntercept  

  HWND                      messageWin;           // The created message window
  HANDLE                    editProcessHandle;    // The created process handle

  HWND                      editorWin;            // The handle to the editor window

  //@
  //  Summary:
  //    Constructor 
  //  
  //  Parameters:
  //    manager    - The manager that uses this instance.
  //
  //    gliCallBacks  - The callbacks to GLIntercept.
  //
	SEInterface(ShaderEditManager *manager, InterceptPluginCallbacks *gliCallBacks);

  //@
  //  Summary:
  //    Destructor.
  // 
  virtual ~SEInterface();

  //@
  //  Summary:
  //    To initialize the interface and start the external shader editor.
  // 
  bool Init();

  //@
  //  Summary:
  //    To send a shader list of all current shaders to the specified window.
  // 
  //  Parameters:
  //    cmdUID     - The message id.
  //
  void SendShaderList(uint cmdUID);

  //@
  //  Summary:
  //    To send the shader source to the specified window.
  // 
  //  Parameters:
  //    cmdUID     - The message id.
  //
  //    shaderID   - The ID of the shader to send the source for.
  //
  void SendShaderSource(uint cmdUID, const string &shaderID);

  //@
  //  Summary:
  //    To compile the passed shader source and send the status back
  //    to the passed window.
  // 
  //  Parameters:
  //    cmdUID     - The message id.
  //
  //    shaderID   - The ID of the shader to compile and the shader source.
  //
  void CompileShaderSource(uint cmdUID, const string &shaderData);

  //@
  //  Summary:
  //    To flag that the passed shader ID should be visiably "shown" when
  //    rendering.
  // 
  //  Parameters:
  //    shaderID    - The shader ID in string format to "show".
  //
  void ShowShader(const string &shaderID);

  //@
  //  Summary:
  //    To flag that the passed shader ID should be "broken-on" when
  //    rendering.
  // 
  //  Parameters:
  //    shaderID    - The shader ID in string format to "break-on".
  //
  void BreakShader(const string &shaderID);

  //@
  //  Summary:
  //    To revert any changes to the passed shader ID.
  // 
  //  Parameters:
  //    shaderID    - The shader ID in string format to revert.
  //
  void RevertShader(const string &shaderID);

  //@
  //  Summary:
  //    To set a shader as being debugged or not.
  // 
  //  Parameters:
  //    flag        - The flag indicating if the shader is being set or 
  //                  unset for debugging.
  //
  //    shaderID    - The shader ID in string format to debug.
  //
  void SetDebugShader(bool flag, const string &shaderID);

  //@
  //  Summary:
  //    To start debugging the passed pixel location.
  // 
  //  Parameters:
  //    debugPixelStr - The pixel location to debug (in string form).
  //
  void SetPixelDebug(const string &debugPixelStr);

  //@
  //  Summary:
  //    To send a windows message. (and time out if not sent)
  // 
  //  Parameters:
  //    sendWnd    - The window to send the message to.
  //
  //    cmdUID     - The message id.
  //
  //    data       - The message data.
  //
  //    sendDataSize   - The size of the data to send.
  //
  //    sendDataBuffer - The data to send.
  //
  //  Returns:
  //    If the message was sent, true is returned. Else false is returned.
  //
  bool SendWinMessage(HWND sendWnd, uint cmdUID, const string &data) const;
  bool SendWinMessage(HWND sendWnd, uint cmdUID, int sendDataSize, const void * sendDataBuffer) const;

};


#endif // __SE_INTERFACE_H_
