/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "GLDriver.h"
#include "FileUtils.h"
#include <string.h>

USING_ERRORLOG

//The real OpenGL driver
extern GLCoreDriver GLV;
extern WGLDriver    GLW;

//The path to the dll
extern string dllPath;

///////////////////////////////////////////////////////////////////////////////
//
GLContext::GLContext(HGLRC rcHandle, const ConfigData &configData, GLDriver *glDriver, FunctionTable *functionTable):
driver(glDriver),

initialized(false),
iglGetStringi(NULL),

glRCHandle(rcHandle),
cachedError(GL_NO_ERROR),
contextThreadID(0),

interceptImage(NULL),
interceptShader(NULL),
interceptShaderGLSL(NULL),
interceptFrame(NULL),
interceptList(NULL)
{

  //Create the new intercept image
  if(configData.imageLogEnabled)
  {
    interceptImage = new InterceptImage(glDriver,functionTable,configData);
  }

  //Create the new intercept shader
  if(configData.shaderLogEnabled)
  {
    interceptShader = new InterceptShader(glDriver,functionTable,configData);

    //Due to the uniqeness of GLSL, these shaders have their own logger
    interceptShaderGLSL = new InterceptShaderGLSL(glDriver,functionTable,configData);
  }

  //Create a new intercept display list
  if(configData.displayListLogEnabled)
  {
    interceptList = new InterceptDisplayList(glDriver,functionTable,configData);
  }

  //Create the new intercept frame
  if(configData.frameLogEnabled)
  {
    interceptFrame = new InterceptFrame(glDriver,functionTable,configData);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
GLContext::~GLContext()
{
  //Delete the image log
  if(interceptImage)
  {
    interceptImage->ReleaseReference();
  }

  //Delete the shader log
  if(interceptShader)
  {
    interceptShader->ReleaseReference();
  }

  //Delete the GLSL shader log
  if(interceptShaderGLSL)
  {
    interceptShaderGLSL->ReleaseReference();
  }

  //Delete the frame logger
  if(interceptFrame)
  {
    interceptFrame->ReleaseReference();
  }

  //Delete the list logger
  if(interceptList)
  {
    interceptList->ReleaseReference();
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void GLContext::InitActiveContext()
{
  // Abort if already initialized, or unable to make OpenGL calls
  if(initialized || !driver->GetInternalGLCallMode())
  {
    return;
  }

  // Flag as initialized
  initialized = true;

  // Get the extension query interface for OpenGL 3.0+ (May return NULL)
  if(GetOpenGLVersion() >= 3.0f)
  {
    iglGetStringi = (const GLubyte * (GLAPIENTRY *)(GLenum name, GLuint index))GLW.glGetProcAddress("glGetStringi");
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLContext::LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{
  //Do frame log first
  if(interceptFrame)
  {
    interceptFrame->LogFunctionPre(funcData,index,args);
  }

  //Do the image log 
  if(interceptImage)
  {
    interceptImage->LogFunctionPre(funcData,index,args);
  }

  //Do the shader log
  if(interceptShader)
  {
    interceptShader->LogFunctionPre(funcData,index,args);
  }
  
  //Do the GLSL shader log
  if(interceptShaderGLSL)
  {
    interceptShaderGLSL->LogFunctionPre(funcData,index,args);
  }

  //Do the list log
  if(interceptList)
  {
    interceptList->LogFunctionPre(funcData,index,args);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void GLContext::LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{
  //Do frame log first
  if(interceptFrame)
  {
    interceptFrame->LogFunctionPost(funcData,index,retVal);
  }

  //Do the image log 
  if(interceptImage)
  {
    interceptImage->LogFunctionPost(funcData,index,retVal);
  }

  //Do the shader log
  if(interceptShader)
  {
    interceptShader->LogFunctionPost(funcData,index,retVal);
  }

  //Do the GLSL shader log
  if(interceptShaderGLSL)
  {
    interceptShaderGLSL->LogFunctionPost(funcData,index,retVal);
  }

  //Do the list log
  if(interceptList)
  {
    interceptList->LogFunctionPost(funcData,index,retVal);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
bool GLContext::ShareLists(GLContext *shareContext) const
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

  //Delete existing context data
  if(shareContext->interceptImage)
  {
    shareContext->interceptImage->ReleaseReference();
    shareContext->interceptImage = NULL;
  }
  if(shareContext->interceptShader)
  {
    shareContext->interceptShader->ReleaseReference();
    shareContext->interceptShader = NULL;
  }
  if(shareContext->interceptShaderGLSL)
  {
    shareContext->interceptShaderGLSL->ReleaseReference();
    shareContext->interceptShaderGLSL = NULL;
  }

  if(shareContext->interceptList)
  {
    shareContext->interceptList->ReleaseReference();
    shareContext->interceptList = NULL;
  }


  //Assign our lists
  if(interceptImage)
  {
    interceptImage->AddReference();
    shareContext->interceptImage = interceptImage;
  }
  if(interceptShader)
  {
    interceptShader->AddReference();
    shareContext->interceptShader = interceptShader;
  }
  if(interceptShaderGLSL)
  {
    interceptShaderGLSL->AddReference();
    shareContext->interceptShaderGLSL = interceptShaderGLSL;
  }

  if(interceptList)
  {
    interceptList->AddReference();
    shareContext->interceptList = interceptList;
  }


  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void GLContext::ActivateLoggers(const string &logPath)
{
  //DT_TODO: Simplify this method

  //Activate the image logger
  if(interceptImage)
  {
    //Set the image saving path
    interceptImage->SetDataSavingPath(logPath);

    //Activate the image saving
    interceptImage->SetDataSaving(true);
  }
  if(interceptShader)
  {
    //Set the shader saving path
    interceptShader->SetDataSavingPath(logPath);

    //Activate the shader saving
    interceptShader->SetDataSaving(true);
  }
  if(interceptShaderGLSL)
  {
    //Set the shader saving path
    interceptShaderGLSL->SetDataSavingPath(logPath);

    //Activate the shader saving
    interceptShaderGLSL->SetDataSaving(true);
  }


  if(interceptList)
  {
    //Set the display list saving path
    interceptList->SetDataSavingPath(logPath);

    //Activate the display list saving
    interceptList->SetDataSaving(true);
  }


  //Activate the frame logger
  if(interceptFrame)
  {
    //Set the frame saving path
    interceptFrame->SetDataSavingPath(logPath);

    //Activate the frame saving
    interceptFrame->SetDataSaving(true);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void GLContext::SuspendLoggers()
{
  if(interceptImage)
  {
    interceptImage->SetDataSaving(false);
  }
  if(interceptShader)
  {
    interceptShader->SetDataSaving(false);
  }
  if(interceptShaderGLSL)
  {
    interceptShaderGLSL->SetDataSaving(false);
  }

  if(interceptList)
  {
    interceptList->SetDataSaving(false);
  }
  if(interceptFrame)
  {
    interceptFrame->SetDataSaving(false);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void GLContext::SetLoggerDataDirty()
{
  //Set all images dirty
  if(interceptImage)
  {
    interceptImage->SetAllDataDirty();
  }

  //Set all shaders dirty
  if(interceptShader)
  {
    interceptShader->SetAllDataDirty();
  }
  if(interceptShaderGLSL)
  {
    interceptShaderGLSL->SetAllDataDirty();
  }

  //Set all lists dirty
  if(interceptList)
  {
    interceptList->SetAllDataDirty();
  }

}


///////////////////////////////////////////////////////////////////////////////
//
float GLContext::GetOpenGLVersion()
{
  //If GL calls cannot be made, flag a error and return
  if(!driver->GetInternalGLCallMode())
  {
    LOGERR(("GLContext::GetOpenGLVersion - Unable to determine version number")); 
    return 0.0f;
  }
  
  //Get the version string
  char * versionString = (char*)GLV.glGetString(GL_VERSION);
  if(!versionString || strlen(versionString) == 0)
  {
    LOGERR(("GLContext::GetOpenGLVersion - Unable to determine version number")); 
    return 0.0f;
  }

  uint majorNum  =1;
  uint minorNum  =0;
  uint releaseNum=0;

  //Get the version number
  sscanf(versionString,"%u.%u.%u",&majorNum,&minorNum,&releaseNum);

  //Minor sanity checks
  if(minorNum > 9)
  {
    minorNum = 9;
  }
  if(releaseNum > 9)
  {
    releaseNum = 9;
  }

  //Merge the versions into one float string
  return ((float)majorNum) + (((float)minorNum) / 10.0f) + (((float)releaseNum) / 100.0f);
}

///////////////////////////////////////////////////////////////////////////////
//
bool GLContext::IsExtensionSupported(const char *extension)
{
  //If GL calls cannot be made, flag a error and return
  if(!driver->GetInternalGLCallMode())
  {
    LOGERR(("GLContext::IsExtensionSupported - Unable to determine if a extension is supported")); 
    return false;
  }

  if(extension == NULL)
  {
    LOGERR(("GLContext::IsExtensionSupported - Invalid extension")); 
    return false;
  }

  // Extension names should not have spaces.
  GLubyte *testSpace = (GLubyte *) strchr(extension, ' ');
  if (testSpace || *extension == '\0')
  {
    LOGERR(("GLContext::IsExtensionSupported - Invalid extension")); 
    return false;
  }

  // If the new extension checking method exists (OpenGL 3.0+)
  if(iglGetStringi)
  {
    // Get the extension count
    GLint numExts = 0;
    GLV.glGetIntegerv(GL_NUM_EXTENSIONS, &numExts);

    // Loop and compare each extension
    for (int i = 0; i < numExts; i++)
    {
      const char *testExt = (const char*)iglGetStringi(GL_EXTENSIONS, i);
      if (testExt && strcmp(extension, testExt) == 0)
      {
        return true;
      }
    }
  }
  else
  {
    //Get the extensions (Old pre-OpenGL 3.0 way)
    const GLubyte *extensionString = GLV.glGetString(GL_EXTENSIONS);
    if(!extensionString)
    {
      return false;
    }

    //Loop and check for the extension string
    const GLubyte *start = extensionString;
    for (;;)
    {
      //Search for the extension
      GLubyte *where = (GLubyte *) strstr((const char *) start, extension);
      if (!where)
      {
        break;
      }

      //Get where the extension should start and end to make sure we don't get a sub-string
      GLubyte *terminator = where + strlen(extension);
      if (where == start || *(where - 1) == ' ')
      {
        if (*terminator == ' ' || *terminator == '\0')
        {
          return true;
        }
      }
      start = terminator;
    }
  }

  return false;
}


