
#include "ExtensionOverride.h"

#include <ConfigParser.h>
#include <CommonErrorLog.h>


USING_ERRORLOG

//Path to the dll
extern string dllPath;

//The object used in the override functions
const ExtensionOverride *overrideData = NULL;


///////////////////////////////////////////////////////////////////////////////
//
ExtensionOverride::ExtensionOverride(InterceptPluginCallbacks *callBacks):
GLV(callBacks->GetCoreGLFunctions()),

#ifdef GLI_BUILD_WINDOWS  

wglGetExtensionsStringARB(NULL),
wglGetExtensionsStringEXT(NULL),

#endif //GLI_BUILD_WINDOWS  

iglGetStringi(NULL),

vendorOverride(false),
vendorString(""),

rendererOverride(false),
rendererString(""),

versionOverride(false),
versionString(""),
versionMajorGLNum(1),
versionMinorGLNum(0),

shaderVersionOverride(false),
shaderVersionString(""),

replaceExtensionsString(""),

wglExtensionsOverride(false),
wglExtensionsString(""),

gliCallBacks(callBacks),
initFlag(false),
exposeStringMarkerEXT(false)

{
  //LOGERR(("Ext Override Plugin Created"));

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

  // Add the string marker token if necessary
  if(exposeStringMarkerEXT)
  {
    //Add to the extension string if necessary
    bool foundExt = false;
    for(uint i = 0; i < addExtensions.size(); i++)
    {
      if(addExtensions[i] == "GL_GREMEDY_string_marker")
      {
        foundExt = true;
        break;
      }
    }
    if(!foundExt)
    {
      addExtensions.push_back("GL_GREMEDY_string_marker");
    }
  }

  //Assign the static class
  if(overrideData == NULL)
  {
    overrideData = this;
  }
  else
  {
    LOGERR(("ExtensionOverride - Cannot register twice"));
  }

}

///////////////////////////////////////////////////////////////////////////////
//
ExtensionOverride::~ExtensionOverride()
{
  //Un-set this class
  if(overrideData == this)
  {
    overrideData = NULL;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::Destroy()
{
  //LOGERR(("Ext Override Plugin Destroyed"));
  
  //Destroy this plugin
  delete this;
}

///////////////////////////////////////////////////////////////////////////////
//
const GLubyte* GLAPIENTRY Custom_glGetString( GLenum strEnum )
{
  const GLubyte * retVal = NULL;

  //If there is a class to access
  if(overrideData)
  {
    //Call the real OpenGL function first
    retVal = overrideData->GLV->glGetString(strEnum);

    //Handle the enum
    switch(strEnum)
    {
      //Replace vendor string
      case(GL_VENDOR) :
         if(overrideData->vendorOverride)
         {
           retVal = (const GLubyte *)overrideData->vendorString.c_str(); 
         }
         break;

      //Replace renderer string
      case(GL_RENDERER) :
         if(overrideData->rendererOverride)
         {
           retVal = (const GLubyte *)overrideData->rendererString.c_str(); 
         }
         break;

      //Replace version string
      case(GL_VERSION) :
         if(overrideData->versionOverride)
         {
           retVal = (const GLubyte *)overrideData->versionString.c_str(); 
         }
         break;

      //Replace GLSL version string
      case(GL_SHADING_LANGUAGE_VERSION) :
         if(overrideData->shaderVersionOverride)
         {
           retVal = (const GLubyte *)overrideData->shaderVersionString.c_str(); 
         }
         break;


      //Replace extensions string
      case(GL_EXTENSIONS) : 
         if(overrideData &&
            overrideData->m_currContext &&
            overrideData->m_currContext->m_extensionsOverride)
         {
           retVal = (const GLubyte *)overrideData->m_currContext->m_extensionsString.c_str(); 
         }
         break;

      default:
         //Unhandled string
         break;
    }
  }

  return retVal;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY Custom_glGetIntegerv(GLenum pname, GLint *params)
{
  //If there is a class to access
  if(overrideData)
  {
    //Call the real OpenGL function first
    overrideData->GLV->glGetIntegerv(pname, params);

    // If it is the number of extensions enum and there are parameters
    if(params)
    {
      switch(pname)
      {
        case(GL_NUM_EXTENSIONS) : 
          if(overrideData &&
             overrideData->m_currContext &&
             overrideData->m_currContext->m_extensionsOverride)
          {
            *params = (GLint)overrideData->m_currContext->m_extensionsArray.size();
          }
          break;

        case(GL_MAJOR_VERSION) : 
          if(overrideData->versionOverride)
          {
            *params = overrideData->versionMajorGLNum;
          }
          break;

        case(GL_MINOR_VERSION) : 
          if(overrideData->versionOverride)
          {
            *params = overrideData->versionMinorGLNum;
          }
          break;

        default:
           //Unhandled string
           break;
      }
    }

  }
}

///////////////////////////////////////////////////////////////////////////////
//
const GLubyte * GLAPIENTRY Custom_glGetStringi(GLenum name, GLuint index)
{
  const GLubyte * retVal = NULL;

  //If there is a class to access
  if(overrideData)
  {
    // If getting the extensions string
    if(name == GL_EXTENSIONS &&
       overrideData->m_currContext &&
       overrideData->m_currContext->m_extensionsOverride)
    {
      if(index < overrideData->m_currContext->m_extensionsArray.size())
      {
        retVal = (const GLubyte *)(overrideData->m_currContext->m_extensionsArray[index].c_str());
      }
      else
      {
        retVal = NULL;
      }
    }
    else
    {
      //Call the real OpenGL function (Note that the extension count may have been adjusted - so cannot call first)
      retVal = overrideData->iglGetStringi(name, index);
    }
  }

  return retVal;
}



#ifdef GLI_BUILD_WINDOWS  

///////////////////////////////////////////////////////////////////////////////
//
const char * GLAPIENTRY Custom_wglGetExtensionsStringARB(HDC hdc)
{
  const char * retVal = "";

  //If there is a class to access
  if(overrideData && overrideData->wglGetExtensionsStringARB)
  {
    //Call the real OpenGL call first
    retVal = overrideData->wglGetExtensionsStringARB(hdc);

    //Override the return string if necessary
    if(overrideData->wglExtensionsOverride)
    {
      retVal = overrideData->wglExtensionsString.c_str(); 
    }
  }

  return retVal;
}

///////////////////////////////////////////////////////////////////////////////
//
const char * GLAPIENTRY Custom_wglGetExtensionsStringEXT(void)
{
  const char * retVal = "";

  //If there is a class to access
  if(overrideData && overrideData->wglGetExtensionsStringEXT)
  {
    //Call the real OpenGL call first
    retVal = overrideData->wglGetExtensionsStringEXT();

    //Override the return string if necessary
    if(overrideData->wglExtensionsOverride)
    {
      retVal = overrideData->wglExtensionsString.c_str(); 
    }
  }

  return retVal;
}

#endif // GLI_BUILD_WINDOWS  

///////////////////////////////////////////////////////////////////////////////
//
void ProcessXMLString(string &processStr)
{
  //Return now if there are no invalid characters
  if(processStr.find_first_of('<') == string::npos &&
     processStr.find_first_of('&') == string::npos)
  {
    return;
  }

  //If there are invalid characters, loop and replace them
  string retString;
  retString.reserve(processStr.length()+5);

  //Loop for all characters
  for(uint i=0;i<processStr.length();i++)
  {
    //Replace all invalid XML characters
    if(processStr[i] == '<')
    {
      retString += "&lt;";  
    }
    else if(processStr[i] == '&')
    {
      retString += "&amp;";  
    }
    else
    {
      retString += processStr[i];  
    }
  }

  //Assign the return string
  processStr = retString;

}


///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY glStringMarkerGREMEDY(GLsizei len, const void *inputString)
{
  //Write to the log file if available
  if(overrideData) 
  {
    //Create a string from the input
    string strMarker;
    if(len > 0)
    {
      strMarker = string((const char*)inputString, len);
    }
    else if(len == 0)
    {
      strMarker = (const char*)inputString;
    }

    //Get the current logger mode
    LoggerMode currMode = overrideData->gliCallBacks->GetLoggerMode();

    //Log to any current loggers
    if(currMode == LM_Text_Logging)
    {
      overrideData->gliCallBacks->AddLoggerString(strMarker.c_str());
    }
    else if(currMode == LM_XML_Logging)
    {
      //Possibly parse for colors?

      //Strip any invalid characters ( < and & characters)
      ProcessXMLString(strMarker);

      //Add <TEXT> lables
      strMarker = string("<TEXT>") + strMarker + string("</TEXT>");
      
      //Add to the log
      overrideData->gliCallBacks->AddLoggerString(strMarker.c_str());
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::GLFunctionPre(uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args )
{
}


///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
}

///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::OnGLContextCreate(HGLRC rcHandle)
{
  //Check that there cannot be duplicates
  if(FindContext(rcHandle) != NULL)
  {
    LOGERR(("ExtensionOverride::OnGLContextCreate - Adding existing context %x?",rcHandle));
    return;
  }

  //Create the new context and add it to the list
  Context * newContext = new Context(rcHandle);
  m_contextArray.push_back(newContext);
}

///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::OnGLContextDelete(HGLRC rcHandle)
{
  //Loop for all contexts
  for(uint i=0; i<m_contextArray.size(); i++)
  {
    Context *delContext = m_contextArray[i]; 

    //Test if the handles match
    if(delContext->GetRCHandle() == rcHandle)
    {
      //If deleting the current context
      if(delContext == m_currContext)
      {
        m_currContext = NULL;
      }
      
      //Delete the context
      delete delContext;
      
      //Erase from the array
      m_contextArray.erase(m_contextArray.begin() + i);
      return;
    }
  }

  //Context not found?
  LOGERR(("ExtensionOverride::OnGLContextDelete - Context handle %x not found",rcHandle));
}

///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle)
{
  //If the new context is NULL or already init return now
  if(newRCHandle == NULL)
  { 
    // Set current context to NULL
    m_currContext = NULL;
    return;
  }

  //Set the new context handle
  Context * setContext = FindContext(newRCHandle);
  if(!setContext)
  {
    LOGERR(("ExtensionOverride::OnGLContextSet - Error finding context %x",newRCHandle));        
  }
  m_currContext = setContext;

  // Check if OpenGL calls can be made 
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  // If the glGetStringi extension entry point has not been gotten previously (not all contexts may have it)
  if(!iglGetStringi)
  {
    iglGetStringi = (const GLubyte * (GLAPIENTRY *)(GLenum name, GLuint index))gliCallBacks->GetWGLFunctions()->glGetProcAddress("glGetStringi");

    // Register the override
    gliCallBacks->AddOverrideFunction("glGetStringi", (void*)Custom_glGetStringi, iglGetStringi);
  }

  //Init. the context if not currently init
  if(m_currContext)
  {
    if(!m_currContext->IsInit())
    {
      m_currContext->Init(gliCallBacks);
    }
  }

  // If we have already been initialized, return
  if(initFlag)
  {
    return;
  }

  //Add the string marker extension if necessary
  if(exposeStringMarkerEXT)
  {
    gliCallBacks->AddOverrideFunction("glStringMarkerGREMEDY", (void*)glStringMarkerGREMEDY, NULL);
  }

  //Register all the callbacks
  gliCallBacks->AddOverrideFunction("glGetString", (void*)Custom_glGetString, (void*)GLV->glGetString);
  gliCallBacks->AddOverrideFunction("glGetIntegerv", (void*)Custom_glGetIntegerv, (void*)GLV->glGetIntegerv);

#ifdef GLI_BUILD_WINDOWS  

  //Attempt to get the wgl entry points
  void **funcLoader;
  funcLoader= (void**)&wglGetExtensionsStringARB;
  *funcLoader = gliCallBacks->GetWGLFunctions()->glGetProcAddress("wglGetExtensionsStringARB");

  funcLoader = (void**)&wglGetExtensionsStringEXT;
  *funcLoader = gliCallBacks->GetWGLFunctions()->glGetProcAddress("wglGetExtensionsStringEXT");

  //Register the overrides
  if(gliCallBacks->AddOverrideFunction("wglGetExtensionsStringARB", (void*)Custom_wglGetExtensionsStringARB, (void*)wglGetExtensionsStringARB) ||
     gliCallBacks->AddOverrideFunction("wglGetExtensionsStringEXT", (void*)Custom_wglGetExtensionsStringEXT, (void*)wglGetExtensionsStringEXT))
  {
    //If there are WGL extenisons to process
    if(wglAddExtensions.size() > 0 || wglRemoveExtensions.size() > 0)
    {
      //If the wgl extensions string is not set, get the string and add/remove the neccessary extensions
      if(!wglExtensionsOverride)
      {
        //Use the ARB version first 
        if(wglGetExtensionsStringARB)
        {
          //Note: Is getting the "current" HDC ok? 
          // Will future possible "multiple ICD" drivers cause issues?
          wglExtensionsString = wglGetExtensionsStringARB(gliCallBacks->GetWGLFunctions()->wglGetCurrentDC());
        }
        else if(wglGetExtensionsStringEXT)
        {
          //If found, copy the string
          wglExtensionsString = wglGetExtensionsStringEXT();
        }
        else
        {
          LOGERR(("WGL extension override - No WGL extension entry point?"));
          wglExtensionsString = "";
        }

        wglExtensionsOverride = true;
      }

      //Add/Remove the extensions from the string
      FormatExtensionString(wglExtensionsString,wglAddExtensions,wglRemoveExtensions);
    }
  }
  
#endif // GLI_BUILD_WINDOWS  

  //Flag that initialization was successful 
  initFlag = true;
}

///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::FormatExtensionString(string &extString, const vector<string> &addList, const vector<string> &removeList)
{
  //Extract all the extensions from the extension string
  vector<string> extArray;
  GetExtensionsList(extString,extArray);

  //Add extensions
  for(uint addNum=0; addNum < addList.size(); addNum++)
  {
    //Search the existing array for a match
    bool strMatch = false;
    for(uint i=0;i<extArray.size();i++)
    { 
      //Break if a match is found
      if(addList[addNum] == extArray[i])
      { 
        strMatch = true;
        break;
      }
    }

    //Add the extension
    if(!strMatch)
    {
      extArray.push_back(addList[addNum]);
    }
  }

  //Remove extensions
  for(uint removeNum=0; removeNum < removeList.size(); removeNum++)
  {
    //Search the existing array for a match
    for(uint i=0;i<extArray.size();i++)
    { 
      //Break if a match is found
      if(removeList[removeNum] == extArray[i])
      { 
        //Erase the extension
        extArray.erase(extArray.begin() + i);
        break;
      }
    }
  }

  //Reform the extension string
  extString = "";
  for(uint i=0;i<extArray.size();i++)
  {
    extString += extArray[i] + string(" ");
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::GetExtensionsList(const string &extString, vector<string> &retExtList)
{
  //Init variables
  string subString;
  int startOffset   = -1;
  int strCopyLength = 0;
  retExtList.clear();
  
  //Loop for the entire length of the string
  for(uint i=0;i<extString.length();i++)
  {
    //Check if not a space seperator
    if(isspace(extString[i]) == 0)
    {
      //Set the starting offset of the string
      if(startOffset == -1)
      {
        startOffset = i;
      }

      strCopyLength++;
    }
    else
    {
      //Extract the sub string
      if(startOffset != -1)
      {
        subString.assign(extString,startOffset,strCopyLength);
        retExtList.push_back(subString);
        //LOGERR(("%s",subString.c_str()));
      }

      //Reset string counters
      startOffset   = -1;
      strCopyLength = 0;
    }
  }

  //Extract any remainder string
  if(startOffset != -1)
  {
    subString.assign(extString,startOffset,strCopyLength);
    retExtList.push_back(subString);
    //LOGERR(("%s",subString.c_str()));
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::ProcessConfigData(const ConfigParser *parser)
{
  //Process all the straight string replacements
  ProcessString(parser,"VendorString",vendorOverride,vendorString);
  
  ProcessString(parser,"RendererString",rendererOverride,rendererString);
  
  ProcessString(parser,"VersionString",versionOverride,versionString);
  
  ProcessString(parser,"ShaderVersionString",shaderVersionOverride,shaderVersionString);

  // Get the major/minor number of any version override
  if(versionOverride)
  {
    GLint releaseNum=0;

    //Get the version numbers
    sscanf(versionString.c_str(), "%d.%d.%d", &versionMajorGLNum, &versionMinorGLNum, &releaseNum);
  }

  //Get if the string marker extension is exposed
  const ConfigToken *stringMarkerToken = parser->GetToken("EnableStringMarker");
  if(stringMarkerToken)
  {
    stringMarkerToken->Get(exposeStringMarkerEXT);
  }

  //Process core extensions
  ProcessExtensionList(parser, "ExtensionsString", replaceExtensionsString,
                               "AddExtensions",    addExtensions,
                               "RemoveExtensions", removeExtensions);

  //Process WGL extensions
  ProcessExtensionList(parser, "WGLExtensionsString", wglExtensionsString,
                               "WGLAddExtensions",    wglAddExtensions,
                               "WGLRemoveExtensions", wglRemoveExtensions);

  //Only enable for a valid string
  if(wglExtensionsString.length() > 0)
  {
    wglExtensionsOverride = true;
  }
  else
  {
    wglExtensionsOverride = false;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::ProcessString(const ConfigParser *parser, const string &searchStr, bool &enable, string &setString) const
{
  const ConfigToken *testToken;

  //Get the token for the string
  testToken = parser->GetToken(searchStr);
  if(testToken)
  {
    //Only enable for a valid string
    if(testToken->Get(setString) && setString.length() > 0)
    {
      enable = true;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ExtensionOverride::ProcessExtensionList(const ConfigParser *parser, 
                                             const string &extName, string &extString,
                                             const string &extAddName, vector<string> &addList,
                                             const string &extRemoveName, vector<string> &removeList) const
{

  //Get the extensions
  const ConfigToken *testToken;

  //Get the token for the extensions
  testToken = parser->GetToken(extName);
  if(testToken)
  {
    //Reset the extension string
    extString = "";

    //Loop for all values
    for(uint i=0;i<testToken->GetNumValues();i++)
    {
      //Append the child to the extension string
      string childString;
      if(testToken->Get(childString,i))
      {
        extString += childString + string(" "); 
      }
    }
  }

  //Process the add/remove extensions
  testToken = parser->GetToken(extAddName);
  if(testToken)
  {
    //Empty the array
    addList.clear();

    //Loop for all values
    for(uint i=0;i<testToken->GetNumValues();i++)
    {
      //Append the child
      string childString;
      if(testToken->Get(childString,i))
      {
        addList.push_back(childString); 
      }
    }
  }
  testToken = parser->GetToken(extRemoveName);
  if(testToken)
  {
    //Empty the array
    removeList.clear();

    //Loop for all values
    for(uint i=0;i<testToken->GetNumValues();i++)
    {
      //Append the child
      string childString;
      if(testToken->Get(childString,i))
      {
        removeList.push_back(childString); 
      }
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
ExtensionOverride::Context *ExtensionOverride::FindContext(HGLRC handle)
{
  //Loop for all contexts
  for(uint i=0; i<m_contextArray.size(); i++)
  {
    //Test for a match
    if(m_contextArray[i]->GetRCHandle() == handle)
    {
      return m_contextArray[i];
    }
  }

  return NULL;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ExtensionOverride::Context::Init(InterceptPluginCallbacks *gliCallBacks)
{
  if(m_isInit)
  {
    return true;
  }

  if(!gliCallBacks->GetGLInternalCallState() ||
     !overrideData)
  {
    return false;
  }

  //Only enable for a valid string
  if(overrideData->replaceExtensionsString.length() > 0)
  {
    m_extensionsString = overrideData->replaceExtensionsString;
    m_extensionsOverride = true;
  }
  else
  {
    m_extensionsOverride = false;
  }

  //If there are extensions to add/remove
  if(overrideData->addExtensions.size() > 0 || 
     overrideData->removeExtensions.size() > 0)
  {
    //If the extensions string is not set, get the string and add/remove the neccessary extensions
    if(!m_extensionsOverride)
    {
      //Get the real extensions string
      if(overrideData->iglGetStringi)
      {
        GLint numOfExtensions = 0;
        m_extensionsString = "";
        overrideData->GLV->glGetIntegerv(GL_NUM_EXTENSIONS, &numOfExtensions);
        for(GLint i = 0; i < numOfExtensions; i++)
        {
          m_extensionsString += (const char*)overrideData->iglGetStringi(GL_EXTENSIONS, i);
          m_extensionsString += " ";
        }
      }
      else
      {
        m_extensionsString = (const char*)overrideData->GLV->glGetString(GL_EXTENSIONS);
      }

      m_extensionsOverride = true;
    }

    //Add/Remove the extensions from the string
    ExtensionOverride::FormatExtensionString(m_extensionsString, overrideData->addExtensions, overrideData->removeExtensions);
  }

  // If extensions are being overridden, get the array version
  if(m_extensionsOverride)
  {
    ExtensionOverride::GetExtensionsList(m_extensionsString, m_extensionsArray);
  }

  // Flag that now init
  m_isInit = true;
  return true;
}

