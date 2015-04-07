#include "DebugContext.h"

#include <ErrorLog.h>
#include <ConfigParser.h>
#include <CommonErrorLog.h>

#define WGL_CONTEXT_FLAGS_ARB     0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001

//The object used in the override functions
const DebugContext * g_contextData = NULL;

//Path to the dll
extern string dllPath;

USING_ERRORLOG


struct StrLookup
{
  StrLookup(GLenum a_value, const char * a_string)
  : m_value(a_value)
  , m_string(a_string)
  {}

  GLenum m_value; // !< The OpenGL value
  const char * m_string; //!< The string version of the value
};

#define STATIC_ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

static StrLookup s_debugSourceLookup[] =
{
  StrLookup(GL_DONT_CARE, "Dont Care"),

  StrLookup(GL_DEBUG_SOURCE_API,             "OpenGL"),
  StrLookup(GL_DEBUG_SOURCE_WINDOW_SYSTEM,   "Windows"),
  StrLookup(GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler"),
  StrLookup(GL_DEBUG_SOURCE_THIRD_PARTY,     "Third Party"),
  StrLookup(GL_DEBUG_SOURCE_APPLICATION,     "Application"),
  StrLookup(GL_DEBUG_SOURCE_OTHER,           "Other"),
};

static StrLookup s_debugTypeLookup[] =
{
  StrLookup(GL_DONT_CARE, "Dont Care"),

  StrLookup(GL_DEBUG_TYPE_ERROR,               "Error"),
  StrLookup(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated behavior"),
  StrLookup(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,  "Undefined behavior"),
  StrLookup(GL_DEBUG_TYPE_PORTABILITY,         "Portability"),
  StrLookup(GL_DEBUG_TYPE_PERFORMANCE,         "Performance"),
  StrLookup(GL_DEBUG_TYPE_OTHER,               "Other"),

  StrLookup(GL_DEBUG_TYPE_MARKER,              "Marker"),
  StrLookup(GL_DEBUG_TYPE_PUSH_GROUP,          "Push Group"),
  StrLookup(GL_DEBUG_TYPE_POP_GROUP,           "Pop Group"),
};

static StrLookup s_debugSeverityLookup[] =
{
  StrLookup(GL_DONT_CARE, "Dont Care"),

  StrLookup(GL_DEBUG_SEVERITY_HIGH,        "High"),
  StrLookup(GL_DEBUG_SEVERITY_MEDIUM,      "Medium"),
  StrLookup(GL_DEBUG_SEVERITY_LOW,         "Low"),
  StrLookup(GL_DEBUG_SEVERITY_NOTIFICATION,"Notification"), 

};

///////////////////////////////////////////////////////////////////////////////
//
static bool LookupDebugStr(const GLenum a_value, StrLookup * a_lookUpArray, uint a_lookupLength, const char *& a_retStr)
{
  for(uint i = 0; i < a_lookupLength; i++)
  {
    if(a_lookUpArray[i].m_value == a_value)
    {
      a_retStr = a_lookUpArray[i].m_string;
      return true;
    }
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
static bool LookupDebugValue(const char * a_str, StrLookup * a_lookUpArray, uint a_lookupLength, GLenum & a_retValue)
{
  for(uint i = 0; i < a_lookupLength; i++)
  {
    if(strcmp(a_str, a_lookUpArray[i].m_string) == 0)
    {
      a_retValue = a_lookUpArray[i].m_value;
      return true;
    }
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
void ProcessXMLString(const string &processStr, string & retString)
{
  //Return now if there are no invalid characters
  if(processStr.find_first_of('<') == string::npos &&
     processStr.find_first_of('&') == string::npos)
  {
    retString = processStr;
    return;
  }

  //If there are invalid characters, loop and replace them
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
}

///////////////////////////////////////////////////////////////////////////////
//
HGLRC GLAPIENTRY Custom_wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int *attribList)
{
  HGLRC retVal = NULL;

  //If there is a class to access
  if(g_contextData && 
     g_contextData->iwglCreateContextAttribsARB)
  {
    // If there is an attribute list, create a new one
    vector<int> newAttribList;
    if(g_contextData->m_forceDebugMode)
    {
      bool debugModeSet = false;

      // Loop for all attributes
      while(attribList != NULL && 
            *attribList != 0)
      {
        // Update the context flags if necessary
        if(*attribList == WGL_CONTEXT_FLAGS_ARB)
        {
          newAttribList.push_back(attribList[0]);
          newAttribList.push_back(attribList[1] | WGL_CONTEXT_DEBUG_BIT_ARB);
          debugModeSet = true;
        }
        else
        {
          // Copy other attributes
          newAttribList.push_back(attribList[0]);
          newAttribList.push_back(attribList[1]);
        }
        attribList+=2;
      }

      // If no context flags were set, add the debug flags manually
      if(!debugModeSet)
      {
        newAttribList.push_back(WGL_CONTEXT_FLAGS_ARB);
        newAttribList.push_back(WGL_CONTEXT_DEBUG_BIT_ARB);
      }

      // Add a terminating 0
      newAttribList.push_back(0);

      // Asssign a pointer to the new list
      attribList = &newAttribList[0];
    }

    //Call the real OpenGL function
    retVal = g_contextData->iwglCreateContextAttribsARB(hDC, hShareContext, attribList);
  }

  return retVal;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY GLDebugOutput(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,GLvoid* userParam)
{
  if(!g_contextData) 
  {
    return;
  }

  //Create a string from the input
  const string strDebug((const char*)message, length);

  // Get the debug source
  const char * debugSrc = "Unknown Source";
  LookupDebugStr(source, s_debugSourceLookup, STATIC_ARRAY_SIZE(s_debugSourceLookup), debugSrc);

  const char * debugType = "Unknown Type";
  LookupDebugStr(type, s_debugTypeLookup, STATIC_ARRAY_SIZE(s_debugTypeLookup), debugType);

  const char * debugSev = "Unknown Severity";
  LookupDebugStr(severity, s_debugSeverityLookup, STATIC_ARRAY_SIZE(s_debugSeverityLookup), debugSev);

  // If logging to the function log
  if(g_contextData->m_logToFunctionLog)
  {
    //Get the current logger mode
    LoggerMode currMode = g_contextData->gliCallBacks->GetLoggerMode();

    //Log to any current loggers
    if(currMode == LM_Text_Logging)
    {
      string logString; 
      StringPrintF(logString, "\n %s: %s(%s) %u: %s", debugSrc, debugType, debugSev, id, strDebug.c_str());

      g_contextData->gliCallBacks->AddLoggerString(logString.c_str());
    }
    else if(currMode == LM_XML_Logging)
    {
      //Strip any invalid characters ( < and & characters)
      string strDebugXMLSafe;
      ProcessXMLString(strDebug, strDebugXMLSafe);

      string logString; 
      StringPrintF(logString, "<DEBUG>%s: %s(%s) %u: %s</DEBUG>", debugSrc, debugType, debugSev, id, strDebugXMLSafe.c_str());

      //Add to the log
      g_contextData->gliCallBacks->AddLoggerString(logString.c_str());
    }
  }

  // If logging to the error log
  if(g_contextData->m_logToErrorLog)
  {
    LOGERR(("%s: %s(%s) %u: %s", debugSrc, debugType, debugSev, id, strDebug.c_str()));
  }

  // Call any other registered call back on the context
  if(g_contextData->m_currContext &&
     g_contextData->m_currContext->m_appCallback)
  {
    g_contextData->m_currContext->m_appCallback(source, type, id, severity, length, message, g_contextData->m_currContext->m_appUserParam);
  }

  // Issue a debug breakpoint if necessary
  if(g_contextData->m_breakOnMessage)
  {
    ErrorLog::IssueDebuggerBreak();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY Custom_glDebugMessageControl(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)
{
  // If there is no context or there are rules, ignore this call
  if(!g_contextData ||
     !g_contextData->iglDebugMessageControl ||
     g_contextData->m_controlRules.size() > 0) 
  {
    return;
  }

  // Call the real OpenGL with the parameters
  g_contextData->iglDebugMessageControl(source, type, severity, count, ids, enabled);
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY Custom_glDebugMessageCallback(GLDEBUGPROC callback, GLvoid* userParam)
{
  if(!g_contextData ||
     !g_contextData->m_currContext)
  {
    return;
  }

  // Store the application callback
  g_contextData->m_currContext->m_appCallback = callback;
  g_contextData->m_currContext->m_appUserParam = userParam;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY Custom_glGetPointerv(GLenum pname, void** params)
{
  if(!g_contextData ||
     !g_contextData->m_currContext ||
     !params)
  {
    return;
  }
  
  // Access the parameters that are overridden
  if(pname == GL_DEBUG_CALLBACK_FUNCTION)
  {
    *params = g_contextData->m_currContext->m_appCallback;
  }
  else if(pname == GL_DEBUG_CALLBACK_USER_PARAM)
  {
    *params = g_contextData->m_currContext->m_appUserParam;    
  }
  else
  {
    // Not a type that is intercepted
    g_contextData->gliCallBacks->GetCoreGLFunctions()->glGetPointerv(pname, params);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLAPIENTRY Custom_glDisable(GLenum cap)
{
  // If not the disable for debug or synchronous output, allow the call
  if(cap != GL_DEBUG_OUTPUT &&
     cap != GL_DEBUG_OUTPUT_SYNCHRONOUS &&
     g_contextData)
  {
    g_contextData->gliCallBacks->GetCoreGLFunctions()->glDisable(cap);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
DebugContext::DebugContext(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),
iwglCreateContextAttribsARB(NULL),
m_forceDebugMode(true),
m_logToFunctionLog(true),
m_logToErrorLog(true),
m_breakOnMessage(false),
iglDebugMessageControl(NULL),
iglDebugMessageCallback(NULL)
{
  gliCallBacks->SetContextFunctionCalls(true);

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

  //Assign the static class
  if(g_contextData == NULL)
  {
    g_contextData = this;
  }
  else
  {
    LOGERR(("DebugContext - Cannot register twice"));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
DebugContext::~DebugContext()
{

}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::ProcessConfigData(const ConfigParser *parser)
{
  //Get if the string marker extension is exposed
  const ConfigToken *stringMarkerToken = parser->GetToken("ForceDebugMode");
  if(stringMarkerToken)
  {
    stringMarkerToken->Get(m_forceDebugMode);
  }

  // Get if using synchronous debug mode
  //const ConfigToken *syncMarkerToken = parser->GetToken("ForceSyncDebug");
  //if(syncMarkerToken)
  //{
  //  syncMarkerToken->Get(m_synchronousDebug);
  //}

  // Get the function log token
  const ConfigToken *funcLogMarkerToken = parser->GetToken("LogToFunctionLog");
  if(funcLogMarkerToken)
  {
    funcLogMarkerToken->Get(m_logToFunctionLog);
  }
  const ConfigToken *errorLogMarkerToken = parser->GetToken("LogToErrorLog");
  if(errorLogMarkerToken)
  {
    errorLogMarkerToken->Get(m_logToErrorLog);
  }

  // Get if breaking into the debugger
  const ConfigToken *debuggerBreakpointMarkerToken = parser->GetToken("BreakOnMessage");
  if(debuggerBreakpointMarkerToken)
  {
    debuggerBreakpointMarkerToken->Get(m_breakOnMessage);
  }

  // Process the rule strings
  const ConfigToken *rulesToken = parser->GetToken("MessageControl");
  if(rulesToken)
  {
    // If there are new rules, clear any old rules
    if(rulesToken->GetNumChildren() > 0)
    {
      m_controlRules.clear();
    }

    // Loop for each rule
    for(uint i = 0; i < rulesToken->GetNumChildren(); i++)
    {
      const ConfigToken *ruleToken = rulesToken->GetChildToken(i);
      if(ruleToken->GetNumValues() != 4)
      {
        LOGERR(("Invalid number of values in Rule %s", ruleToken->GetName().c_str()));
        return;
      }
      MessageControlRule newRule;

      // Get the value strings
      string sourceValue;
      string typeValue;
      string severityValue;
      if(!ruleToken->Get(sourceValue, 0) ||
         !ruleToken->Get(typeValue, 1) ||
         !ruleToken->Get(severityValue, 2))
      {
        LOGERR(("Invalid values in Rule %s", ruleToken->GetName().c_str()));
        return;
      }
     
      // Decode the source value
      if(!LookupDebugValue(sourceValue.c_str(),   s_debugSourceLookup,   STATIC_ARRAY_SIZE(s_debugSourceLookup),   newRule.m_source) ||
         !LookupDebugValue(typeValue.c_str(),     s_debugTypeLookup,     STATIC_ARRAY_SIZE(s_debugTypeLookup),     newRule.m_type) ||
         !LookupDebugValue(severityValue.c_str(), s_debugSeverityLookup, STATIC_ARRAY_SIZE(s_debugSeverityLookup), newRule.m_severity))
      {
        LOGERR(("Unable to lookup enum values in Rule %s", ruleToken->GetName().c_str()));
        return;
      }
 
      // Get if enabled
      {
        bool enabled = false;
        if(!ruleToken->Get(enabled, 3))
        {
          LOGERR(("Invalid enabled flag in Rule %s", ruleToken->GetName().c_str()));
          return;
        }
        newRule.m_enabled = GL_FALSE;
        if(enabled)
        {
          newRule.m_enabled = GL_TRUE;
        }
      }

      // Get the array of IDs
      const ConfigToken *idsToken = ruleToken->GetChildToken("ids");
      if(idsToken)
      {
        if(!idsToken->GetArray(idsToken->GetNumValues(), newRule.m_ids))
        {
          LOGERR(("Unable to get ids in Rule %s", ruleToken->GetName().c_str()));
          return;
        }
      }

      // Check the OpenGL types for valid values
      if(newRule.m_ids.size() > 0)
      {
        if(newRule.m_source == GL_DONT_CARE ||
           newRule.m_type == GL_DONT_CARE ||
           newRule.m_severity != GL_DONT_CARE)
        {
          LOGERR(("Invalid rule when specifying ids - %s (check OpenGL specs)", ruleToken->GetName().c_str()));
          return;
        }
      }

      // Add the new rule to the array
      m_controlRules.push_back(newRule);
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::GLFunctionPre (uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args )
{
}


///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::GLFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::GLFrameEndPre(const char *funcName, uint funcIndex, const FunctionArgs & args )
{
}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::GLRenderPre (const char *funcName, uint funcIndex, const FunctionArgs & args)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::GLRenderPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::OnGLError(const char *funcName, uint funcIndex)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::OnGLContextCreate(HGLRC rcHandle)
{
  //Check that there cannot be duplicates
  if(FindContext(rcHandle) != NULL)
  {
    LOGERR(("DebugContext::OnGLContextCreate - Adding existing context %x?",rcHandle));
    return;
  }

  //Create the new context and add it to the list
  Context * newContext = new Context(rcHandle);
  m_contextArray.push_back(newContext);
}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::OnGLContextDelete(HGLRC rcHandle)
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
  LOGERR(("DebugContext::OnGLContextDelete - Context handle %x not found",rcHandle));
}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::OnGLContextSet(HGLRC oldRCHandle, HGLRC newRCHandle)
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
    LOGERR(("DebugContext::OnGLContextSet - Error finding context %x",newRCHandle));        
  }
  m_currContext = setContext;
  
  // Check if OpenGL calls can be made 
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  // Always try for the context creation function pointer
  if(m_forceDebugMode &&
     !iwglCreateContextAttribsARB)
  {
    iwglCreateContextAttribsARB = (HGLRC (GLAPIENTRY *)(HDC hDC, HGLRC hShareContext, const int *attribList))gliCallBacks->GetWGLFunctions()->glGetProcAddress("wglCreateContextAttribsARB");

    // Register the override
    if(iwglCreateContextAttribsARB)
    {
      gliCallBacks->AddOverrideFunction("wglCreateContextAttribsARB", (void*)Custom_wglCreateContextAttribsARB, iwglCreateContextAttribsARB);
    }
  }

  // Register the debug handles if not already done
  // http://www.opengl.org/registry/specs/ARB/debug_output.txt
  // http://www.opengl.org/registry/specs/KHR/debug.txt
  if(iglDebugMessageControl == NULL ||
     iglDebugMessageCallback == NULL)
  {
    if(gliCallBacks->GetGLVersion() >= 4.3f ||
       gliCallBacks->IsGLExtensionSupported("GL_ARB_debug_output") ||
       gliCallBacks->IsGLExtensionSupported("GL_KHR_debug"))
    {
      iglDebugMessageControl = (void (GLAPIENTRY *)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled))gliCallBacks->GetWGLFunctions()->glGetProcAddress("glDebugMessageControl");
      if(!iglDebugMessageControl)
      {
        iglDebugMessageControl = (void (GLAPIENTRY *)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled))gliCallBacks->GetWGLFunctions()->glGetProcAddress("glDebugMessageControlARB");
      }

      iglDebugMessageCallback = (void (GLAPIENTRY *)(GLDEBUGPROC callback, GLvoid* userParam))gliCallBacks->GetWGLFunctions()->glGetProcAddress("glDebugMessageCallback");
      if(!iglDebugMessageCallback)
      {
        iglDebugMessageCallback = (void (GLAPIENTRY *)(GLDEBUGPROC callback, GLvoid* userParam))gliCallBacks->GetWGLFunctions()->glGetProcAddress("glDebugMessageCallbackARB");
      }

      gliCallBacks->AddOverrideFunction("glDebugMessageCallback", Custom_glDebugMessageCallback, iglDebugMessageCallback);
      gliCallBacks->AddOverrideFunction("glDebugMessageCallbackARB", Custom_glDebugMessageCallback, gliCallBacks->GetWGLFunctions()->glGetProcAddress("glDebugMessageCallbackARB"));

      gliCallBacks->AddOverrideFunction("glDebugMessageControl", Custom_glDebugMessageControl, iglDebugMessageControl);
      gliCallBacks->AddOverrideFunction("glDebugMessageControlARB", Custom_glDebugMessageControl, gliCallBacks->GetWGLFunctions()->glGetProcAddress("glDebugMessageControlARB"));

      gliCallBacks->AddOverrideFunction("glGetPointerv", Custom_glGetPointerv, gliCallBacks->GetCoreGLFunctions()->glGetPointerv);

      // Override glDisable to catch GL_DEBUG_OUTPUT_SYNCHRONOUS
      gliCallBacks->AddOverrideFunction("glDisable", Custom_glDisable, gliCallBacks->GetCoreGLFunctions()->glDisable);
    }
  }

  //Init. the context if not currently init
  if(m_currContext)
  {
    if(!m_currContext->IsInit())
    {
      m_currContext->Init(gliCallBacks);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::OnGLContextShareLists(HGLRC srcHandle, HGLRC dstHandle)
{

}

///////////////////////////////////////////////////////////////////////////////
//
void DebugContext::Destroy()
{
  //Don't do this:
  //gliCallBacks->DestroyPlugin();

  //Destroy this plugin
  delete this;
}

///////////////////////////////////////////////////////////////////////////////
//
DebugContext::Context *DebugContext::FindContext(HGLRC handle)
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
bool DebugContext::Context::Init(InterceptPluginCallbacks *gliCallBacks)
{
  if(m_isInit)
  {
    return true;
  }

  if(!gliCallBacks->GetGLInternalCallState() ||
     !g_contextData)
  {
    return false;
  }

  // Get if the debug out extension is supported
  m_contextDebugOutput = gliCallBacks->GetGLVersion() >= 4.3f ||
                         gliCallBacks->IsGLExtensionSupported("GL_ARB_debug_output") ||
                         gliCallBacks->IsGLExtensionSupported("GL_KHR_debug");
  if(m_contextDebugOutput)
  {
    // Register the callback (still do this outside the m_synchronousDebug option?)
    if(g_contextData->iglDebugMessageCallback)
    {
      g_contextData->iglDebugMessageCallback(GLDebugOutput, NULL);
    }

    if(gliCallBacks->GetGLVersion() >= 4.3f ||
       gliCallBacks->IsGLExtensionSupported("GL_KHR_debug"))
    {
      gliCallBacks->GetCoreGLFunctions()->glEnable(GL_DEBUG_OUTPUT);
    }

    // Force sync mode - glDisable is intercepted to ensure the application does not change the setting.
    gliCallBacks->GetCoreGLFunctions()->glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    // Set all the rules on the context
    if(g_contextData->iglDebugMessageControl)
    {
      for(uint i = 0; i < g_contextData->m_controlRules.size(); i++)
      {
        const MessageControlRule & rule = g_contextData->m_controlRules[i];

        // Get a pointer to the id array (if it exists)
        const GLuint *idArray = NULL;
        if(rule.m_ids.size() > 0)
        {
          idArray = &rule.m_ids[0];
        }

        g_contextData->iglDebugMessageControl(rule.m_source, rule.m_type, rule.m_severity, (GLsizei)rule.m_ids.size(), idArray, rule.m_enabled);
      }
    }
  }

  // Flag that now init
  m_isInit = true;
  return true;
}

