#ifndef __EXTENSION_OVERRIDE_H_
#define __EXTENSION_OVERRIDE_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include <string>
#include <vector>

using namespace std;

class ConfigParser; 

class ExtensionOverride : public InterceptPluginInterface
{
public:
  ExtensionOverride(InterceptPluginCallbacks *callBacks);
  virtual ~ExtensionOverride();

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
  inline virtual void GLIAPI GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

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
  inline virtual void GLIAPI GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args );

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
  inline virtual void GLIAPI GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

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
 inline virtual void GLIAPI GLRenderPre (const char *funcName, uint funcIndex, const FunctionArgs & args );

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
  inline virtual void GLIAPI GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    Called when a OpenGL error occurs.
  //  
  //  Parameters:
  //    funcData - The data of the function that cause the error.
  //
  //    index    - A index to the function (In the function table).
  //
  inline virtual void GLIAPI OnGLError(const char *funcName, uint funcIndex);

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
  inline virtual void GLIAPI OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle);


  //@
  //  Summary:
  //    Called when this plugin is to be destroyed. The plugin should delete 
  //    itself. (Note: If a plugin needs to shutdown, always request deletion
  //    via InterceptPluginCallbacks::DestroyPlugin which will call this method
  //    on the next update. Do not destroy the plugin by other means.)
  //  
  virtual void GLIAPI Destroy();


  //Public data (Public so stand alone functions can easily access)

  const GLCoreDriver       *GLV;                  //The core OpenGL driver
  
#ifdef GLI_BUILD_WINDOWS  
  const char * (GLAPIENTRY *wglGetExtensionsStringARB) (HDC hdc); //The ARB WGL entry point
  const char * (GLAPIENTRY *wglGetExtensionsStringEXT) (void);    //The EXT WGL entry point
#endif //GLI_BUILD_WINDOWS  

  const GLubyte * (GLAPIENTRY *iglGetStringi)(GLenum name, GLuint index); // glGetStringi for extenstion queries on OpenGL 3.0+

  /// Utility class to store context data
  class Context
  {
  public:

    inline Context(HGLRC rcHandle)
    : m_glRCHandle(rcHandle)
    , m_isInit(false)
    , m_extensionsOverride(false)
    {}

    inline bool IsInit() { return m_isInit; }
    inline HGLRC GetRCHandle() const { return m_glRCHandle; }

    bool Init(InterceptPluginCallbacks *gliCallBacks);

    bool  m_extensionsOverride;        //!< Flag to indicate if the extensions over-ride string has been set
    string  m_extensionsString;        //!< The replacement extensions string
    vector<string> m_extensionsArray;  //!< The replacement extensions in array form 

  protected:

    HGLRC m_glRCHandle;  //!< The openGL rendering context handle
    bool  m_isInit;      //!< Flag if the context has been init
  };

  bool    vendorOverride;                         //Flag to indicate if the vendor over-ride string has been set
  string  vendorString;                           //The replacement vendor string

  bool    rendererOverride;                       //Flag to indicate if the renderer over-ride string has been set
  string  rendererString;                         //The replacement renderer string

  bool    versionOverride;                        //Flag to indicate if the version over-ride string has been set
  string  versionString;                          //The replacement version string
  GLint   versionMajorGLNum;                      //The replacement version (major number) 
  GLint   versionMinorGLNum;                      //The replacement version (minor number) 

  bool    shaderVersionOverride;                  //Flag to indicate if the GLSL shaderVersion over-ride string has been set
  string  shaderVersionString;                    //The replacement GLSL shaderVersion string

  string  replaceExtensionsString;                //The replacement extensions string
  vector<string> addExtensions;                   //List of extensions to add to the extension string
  vector<string> removeExtensions;                //List of extensions to remove from the extension string 

  bool    wglExtensionsOverride;                  //Flag to indicate if the WGL extensions over-ride string has been set
  string  wglExtensionsString;                    //The replacement WGL extensions string

  vector<string> wglAddExtensions;                //List of extensions to add to the WGL extension string
  vector<string> wglRemoveExtensions;             //List of extensions to remove from the WGL extension string 

  InterceptPluginCallbacks *gliCallBacks;         //The callback interface into GLIntercept

  Context         * m_currContext;                // The current OpenGL context
  vector<Context *> m_contextArray;               // The array of all the OpenGL contexts

protected:

  bool    initFlag;                               //Flag indicating if initialization was successful.
  bool    exposeStringMarkerEXT;                  //Expose the string marker extension

  //@
  //  Summary:
  //    Process the passed config parser to load the configuration data.
  //  
  void ProcessConfigData(const ConfigParser *parser);

  //@
  //  Summary:
  //    Process a single string token.
  //  
  void ProcessString(const ConfigParser *parser, const string &searchStr, bool &enable, string &setString) const;

  //@
  //  Summary:
  //    Process a extensions from the parser.
  //  
  void ProcessExtensionList(const ConfigParser *parser, 
                            const string &extName, string &extString,
                            const string &extAddName, vector<string> &addList,
                            const string &extRemoveName, vector<string> &removeList) const;

  //@
  //  Summary:
  //    Convert a string version of extensions into a list of flat extensions
  //  
  static void GetExtensionsList(const string &extString, vector<string> &retExtList);

  //@
  //  Summary:
  //    Format the passed extension string by adding/ removing the specified extension lists.
  //  
  static void FormatExtensionString(string &extString, const vector<string> &addList, const vector<string> &removeList);

  //@
  //  Summary:
  //    Find the conext point for the supplied handle
  //  
  Context *FindContext(HGLRC handle);

};


//Unused entry points

///////////////////////////////////////////////////////////////////////////////
//
inline void ExtensionOverride::GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args )
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ExtensionOverride::GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void ExtensionOverride::GLRenderPre (const char *funcName, uint funcIndex, const FunctionArgs & args)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void ExtensionOverride::GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void ExtensionOverride::OnGLError(const char *funcName, uint funcIndex)
{

}

///////////////////////////////////////////////////////////////////////////////
//
inline void ExtensionOverride::OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle)
{

}



#endif // __EXTENSION_OVERRIDE_H_
