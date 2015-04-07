/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptLogText.h"
#include "GLDriver.h"
#include <time.h>

USING_ERRORLOG

extern GLDriver glDriver;

///////////////////////////////////////////////////////////////////////////////
//
InterceptLogText::InterceptLogText(const string &fileName,FunctionTable * functionTable,const ConfigData &configData):
InterceptLog(functionTable),
logFileName(fileName),
logFile(NULL),
logFlush(configData.logFlush),
shaderRenderCallLog(configData.shaderRenderCallStateLog),
imageRenderCallLog(configData.imageRenderCallStateLog),
functionTimerEnabled(configData.timerLogEnabled),
functionTimerCutOff(configData.timerLogCutOff)
{
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptLogText::Init()
{
  //Write the log header
  struct tm *newtime;
  time_t     aclock;

  //Attempt to open the file
  logFile = fopen(logFileName.c_str(),"wt");

  //Prevent NULL file settings
  if(logFile == NULL)
  {
    LOGERR(("InterceptLog - Unable to open log file %s",logFileName.c_str()));
    return false;
  }

  //Get the current time
  time( &aclock );

  //Convert it
  newtime = localtime( &aclock );

  fprintf(logFile,
    "===============================================================================\n"
    " GLIntercept version %s Log generated on: %s \n"
    "===============================================================================\n",__GLI_BUILD_VER_STR,asctime(newtime));


  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptLogText::~InterceptLogText()
{
  //Close the log file
  if(logFile)
  {
    fclose(logFile);
    logFile = NULL;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogText::LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{

  //Check the function data
  if(!funcData)
  {
    LOGERR(("InterceptLogText::LogFunctionPre - Function data for index %u is NULL",index)); 
    return;
  }

  //Start the function on a newline
  fprintf(logFile,"\n");

  //Loop for the function call depth to flag sub-function calls
  for(uint i=0;i<glDriver.GetFunctionCallDepth();i++)
  {
    fprintf(logFile,"----->");
  }

  //Get the function data and args and write it out
  string functionString;
  GetFunctionString(funcData,index,args,functionString);
  fprintf(logFile,"%s",functionString.c_str());

  //Flush the log if necessary
  if(logFlush)
  {
    fflush(logFile);
  }

  //Flag to the driver if function timing is needed
  if(functionTimerEnabled) 
  {
    glDriver.SetFunctionTimer();
  }

  //If this is an rendering function (an there is a context)
  // (NOTE: This uses the origional "args" not a working copy)
  if(glDriver.GetCurrentContext() &&
     glDriver.GetCurrentContext()->IsRenderFuncion(funcData,index,args))
  {
    //If we log shaders on render calls, log them
    if(shaderRenderCallLog)
    {
      //Get the bound shaders
      uint vertexShader,fragmentShader;
      GLuint programGLSL;
      glDriver.GetCurrentContext()->GetBoundShaders(vertexShader,fragmentShader,programGLSL);

      //If a valid vertex shader, log it
      if(vertexShader != 0)
      {
        fprintf(logFile," VP=%u ",vertexShader);
      }

      if(fragmentShader != 0)
      {
        fprintf(logFile," FP=%u ",fragmentShader);
      }
      
      if(programGLSL != 0)
      {
        fprintf(logFile," GLSL=%u ",programGLSL);
      }
    }

    //If we log images on render calls, log them
    if(imageRenderCallLog)
    {
      //Get the bound textures
      BoundTextureArray boundTextures;
      glDriver.GetCurrentContext()->GetBoundTextures(boundTextures);

      //If there are any textures
      if(boundTextures.size() > 0)
      {
        fprintf(logFile," Textures[ ");
        
        //Loop for all textures and log the stage and texture number
        for(uint i=0;i<boundTextures.size();i++)
        {
          fprintf(logFile,"(%u,%u) ",boundTextures[i].texStage,boundTextures[i].texID);
        }

        fprintf(logFile,"] ");
      }
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogText::LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{

  //Check the function data
  if(!funcData)
  {
    LOGERR(("InterceptLogText::LogFunctionPost - Function data for index %u is NULL",index)); 
    return;
  }

  //Get the return parameter
  const ParameterData * returnData = &funcData->returnType;

  //Determine if we are processing pointers
  bool isPointer=false;
  if(returnData->pointerCount > 0 || returnData->length != -1)
  {
    isPointer=true;
  }

  //Check the return value
  if(isPointer ||
     returnData->type != PT_void)
  {
    //Look up the data
    ParamValue value;
    if(GetReturnValue(returnData->type, retVal, isPointer, value))
    {
      string out = ConvertParam(value, isPointer, returnData);

      //Log the result
      fprintf(logFile,"=%s ",out.c_str());
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogText::LogFunctionError(uint errorCode)
{
  //If the log is not writing or the error code is 0, return
  if(errorCode == 0x0)
  {
    return;
  }

  //Log that an error has occured
  fprintf(logFile," glGetError() = ");

  //Get the error string
  string errorString;
  GetErrorStringValue(errorCode,errorString);

  //Print the error value out to the log
  fprintf(logFile,"%s",errorString.c_str());

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogText::LogFunctionComplete()
{
  //Test if we log function timer values
  if(functionTimerEnabled)
  {
    //Get the function time
    uint funcTime = glDriver.GetFunctionTime();
    if(funcTime >= functionTimerCutOff)
    {
      //Print the function time if it is greater than the cutoff
      fprintf(logFile," Time= %uus",funcTime);
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogText::AddLogString(const char* addString)
{
  //Add the string to the log
  if(addString)
  {
    fputs(addString,logFile);
  }
}






