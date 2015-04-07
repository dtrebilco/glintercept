#ifndef __LOG_PLUGIN_H_
#define __LOG_PLUGIN_H_

#include "../MainLib/InterceptPluginInterface.h"

class ConfigParser; 

typedef void (GLAPIENTRY *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

class DebugContext : public InterceptPluginInterface
{
public:
	DebugContext(InterceptPluginCallbacks *callBacks);
	virtual ~DebugContext();

  //@
  //  Summary:
  //    Called when an OpenGL function that has been registered 
  //    (via RegisterGLFunction) is about to be made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void GLIAPI GLFunctionPre (uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args );

  //@
  //  Summary:
  //    Called when an OpenGL function that has been registered 
  //    (via RegisterGLFunction) has been made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return vlaue of the function (if any).
  //
  virtual void GLIAPI GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    Called when the OpenGL "frame end" call is about to be made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void GLIAPI GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args );

  //@
  //  Summary:
  //    Called after OpenGL "frame end" call has been made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void GLIAPI GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    Called when a OpenGL "render" call is about to be made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void GLIAPI GLRenderPre (const char *funcName, uint funcIndex, const FunctionArgs & args );

  //@
  //  Summary:
  //    Called when a OpenGL "render" call has been made.
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value of the function. (If any).
  //
  virtual void GLIAPI GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    Called when a OpenGL error occurs.
  //  
  //  Parameters:
  //    funcData - The data of the function that cause the error.
  //
  //    index    - A index to the function (In the function table).
  //
  virtual void GLIAPI OnGLError(const char *funcName, uint funcIndex);

  //@
  //  Summary:
  //    Called when a OpenGL context is created.
  //  
  //  Parameters:
  //    rcHandle - The new OpenGL context.
  //
  virtual void GLIAPI OnGLContextCreate(HGLRC rcHandle);

  //@
  //  Summary:
  //    Called when a OpenGL context is deleted.
  //  
  //  Parameters:
  //    rcHandle - The OpenGL context that is deleted.
  //
  virtual void GLIAPI OnGLContextDelete(HGLRC rcHandle);

  //@
  //  Summary:
  //    Called when a OpenGL context is assigned (set).
  //  
  //  Parameters:
  //    oldRCHandle - The old (previous) OpenGL context.
  //
  //    newRCHandle - The new OpenGL context.
  //
  virtual void GLIAPI OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle);

  //@
  //  Summary:
  //    Called when a OpenGL context share lists.
  //  
  //  Parameters:
  //    srcHandle - The context constaining the lists.
  //
  //    dstHandle - The context to now share the lists.
  //
  virtual void GLIAPI OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle);


  //@
  //  Summary:
  //    Called when this plugin is to be destroyed. The plugin should delete 
  //    itself. (Note: If a plugin needs to shutdown, always request deletion
  //    via InterceptPluginCallbacks::DestroyPlugin which will call this method
  //    on the next update. Do not destroy the plugin by other means.)
  //  
  virtual void GLIAPI Destroy();

  HGLRC (GLAPIENTRY *iwglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext, const int *attribList); // The context creation function to override
  bool    m_forceDebugMode;   // If forcing the debug mode in context creation
  bool    m_logToFunctionLog; // If logging to the function log
  bool    m_logToErrorLog;    // If logging to the error log
  bool    m_breakOnMessage;   // If issuing a debugger breakpoint on debug messages

  void (GLAPIENTRY *iglDebugMessageControl) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled);
  void (GLAPIENTRY *iglDebugMessageCallback) (GLDEBUGPROC callback, GLvoid* userParam);

  InterceptPluginCallbacks *gliCallBacks;

  // Helper struct to store a message control rule
  struct MessageControlRule
  {
    MessageControlRule()
    : m_source(GL_DONT_CARE)
    , m_type(GL_DONT_CARE)
    , m_severity(GL_DONT_CARE)
    , m_enabled(GL_TRUE)
    {
    }

    GLenum m_source;
    GLenum m_type;
    GLenum m_severity;
    GLboolean m_enabled;

    vector<GLuint> m_ids;
  };

  vector<MessageControlRule> m_controlRules; //!< The array of message control rules

  /// Utility class to store context data
  class Context
  {
  public:

    inline Context(HGLRC rcHandle)
    : m_glRCHandle(rcHandle)
    , m_isInit(false)
    , m_contextDebugOutput(false)
    , m_appCallback(NULL)
    , m_appUserParam(NULL)
    {}

    inline bool IsInit() { return m_isInit; }
    inline HGLRC GetRCHandle() const { return m_glRCHandle; }

    bool Init(InterceptPluginCallbacks *gliCallBacks);

    GLDEBUGPROC m_appCallback;  //!< The application specified callback 
    GLvoid*     m_appUserParam; //!< The application user parameter
  protected:

    HGLRC   m_glRCHandle;        //!< The openGL rendering context handle

    bool m_isInit;             //!< Flag if the context has been init
    bool m_contextDebugOutput; //!< Flag if this context supports debug output

  };

  Context         * m_currContext;                // The current OpenGL context
  vector<Context *> m_contextArray;               // The array of all the OpenGL contexts

protected:

  //@
  //  Summary:
  //    Process the passed config parser to load the configuration data.
  //  
  void ProcessConfigData(const ConfigParser *parser);

  //@
  //  Summary:
  //    Find the conext point for the supplied handle
  //  
  Context *FindContext(HGLRC handle);

};

#endif // __LOG_PLUGIN_H_
