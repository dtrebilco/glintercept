/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptLogXML.h"
#include "GLDriver.h"
#include "FileUtils.h"
#include <time.h>

USING_ERRORLOG

extern GLDriver glDriver;


///////////////////////////////////////////////////////////////////////////////
//
InterceptLogXML::InterceptLogXML(const string &fileName,FunctionTable * functionTable,const ConfigData &configData, 
                                 const string &newXSLFileName):
InterceptLogText(fileName,functionTable,configData),
preFunctionCalled(0),
frameRenderCallLog(configData.frameLogEnabled),
xslFileName(newXSLFileName)
{

}



///////////////////////////////////////////////////////////////////////////////
//
InterceptLogXML::~InterceptLogXML()
{
  //If there is a log file open
  if(logFile)
  {
    //If the ending tags need writing
    while (preFunctionCalled > 0)
    {
      //Complete the function
      fprintf(logFile,"</FUNCTION>\n"); 
      preFunctionCalled--;
    }

    //Write the closing tags
    fprintf(logFile,"</FUNCTIONS>\n");
    fprintf(logFile,"</GLINTERCEPT>\n");
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptLogXML::Init()
{
  //Attempt to open the file
  logFile = fopen(logFileName.c_str(),"wt");

  //Prevent NULL file settings
  if(logFile == NULL)
  {
    LOGERR(("InterceptLogXML - Unable to open log file %s",logFileName.c_str()));
    return false;
  }

  //Write the XML version info
  fprintf(logFile,"<?xml version='1.0'?>\n");

  //Add the XSL file name if applicable
  if(xslFileName.size() > 0)
  {
    fprintf(logFile,"<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>",xslFileName.c_str());
  }

  //Write the header
  fprintf(logFile,"<GLINTERCEPT>\n");

  //Write the header data
  fprintf(logFile,"<HEADER>\n");

  //Write the version
  fprintf(logFile,"<VERSION>%s</VERSION>\n",__GLI_BUILD_VER_STR);

  //Write the time stamp
  struct tm *newtime;
  time_t     aclock;

  //Get the current time
  time( &aclock );

  //Convert it
  newtime = localtime( &aclock );
  fprintf(logFile,"<TIMESTAMP>%s</TIMESTAMP>\n",asctime(newtime));

  //Close the header
  fprintf(logFile,"</HEADER>\n");

  //Open the function data
  fprintf(logFile,"<FUNCTIONS>\n");

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{
  //Check the function data
  if(!funcData)
  {
    LOGERR(("InterceptLogXML::LogFunctionPre - Function data for index %u is NULL",index)); 
    return;
  }

  //Flag that the "pre" function has been called
  preFunctionCalled++;

  //Create a args processing object
  FunctionArgs newArgs(args);
   
  //Write the starting tag
  fprintf(logFile,"<FUNCTION>\n");

  //Write the name
  fprintf(logFile,"<NAME>%s</NAME>\n",funcData->GetName().c_str());

  //Loop for all the parameters
  for(uint i=0;i<funcData->parameterArray.size();i++)
  {
    //Get the parameter
    const ParameterData * paramData = &funcData->parameterArray[i];

    //Determine if we are processing pointers
    bool isPointer=false;
    if(paramData->pointerCount > 0 || paramData->length != -1)
    {
      isPointer=true;
    }

    //Get the value
    ParamValue value;
    if(!GetNextValue(paramData->GetGLType(),newArgs,isPointer, value))
    {
      break;
    }

    //Write the parameter tag
    fprintf(logFile,"<PARAM name=\"%s\" type=\"%s\">",paramData->paramName.c_str(),paramData->GetDisplayString().c_str());

    //Test if this is an array value
    if(paramData->length != -1)
    {
      bool isArrayOfPointers = false;

      //Test for an array of pointers
      if(paramData->pointerCount > 0)
      {
        isArrayOfPointers = true;
      }

      //Assign the array
      void * array =  value.pointerValue;

      //Loop and print the array
      for(uint i2=0;i2<(uint)paramData->length;i2++)
      {
        //Get the value from the array
        if(!GetNextArrayValue(paramData->GetGLType(),&array,isArrayOfPointers,value))
        {
          break;
        }

        //Add any custom tags 
        AddParamaterTags(value,isArrayOfPointers,paramData);

        //Convert and print the value
        string out=ConvertParam(value,isArrayOfPointers,paramData);
        fprintf(logFile,"<VALUE data=\"%s\"/>",out.c_str());
      }
      fprintf(logFile,"</PARAM>\n");
    }
    else
    {
      //Add any custom tags 
      AddParamaterTags(value,isPointer,paramData);

      //Just get the single value
      string out=ConvertParam(value,isPointer,paramData);
      fprintf(logFile,"<VALUE data=\"%s\"/></PARAM>",out.c_str());
    }

  }

  //Start a new line
  fprintf(logFile,"\n");

  //Flag to the driver if function timing is needed
  if(functionTimerEnabled) 
  {
    glDriver.SetFunctionTimer();
  }

  //If this is an rendering function
  // (NOTE: this uses the orogional "args" type, not the processing version)
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
        AddShaderTagARB(vertexShader,"VP");
      }

      if(fragmentShader != 0)
      {
        AddShaderTagARB(fragmentShader,"FP");
      }

      if(programGLSL != 0)
      {
        AddShaderTagGLSL(programGLSL,InterceptShaderGLSL::GLSLRT_GLSL_Program);
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
        //Loop for all textures and log the stage and texture number
        for(uint i=0;i<boundTextures.size();i++)
        {
          fprintf(logFile,"<TEXSTAGE number=\"%u\">",boundTextures[i].texStage);
          AddImageTag(boundTextures[i].texID);
          fprintf(logFile,"</TEXSTAGE>",boundTextures[i].texStage);
        }
      }
    }

    // Register frame buffer images if requested
    if(frameRenderCallLog)
    {
      //Get the frame file names
      FrameInterceptFileNames frameFileNames;
      if(glDriver.GetCurrentContext()->GetFrameFileNames(frameFileNames))
      {
        //Add all valid tags
        fprintf(logFile,"<FRAMEBUFFER>");

        //If there is some color frame buffer
        for(uint i=0; i<frameFileNames.numColorBuffers; i++) 
        {
          if(frameFileNames.colorBufNames[i][FIDT_PRE_FRAME ].size() > 0 ||
             frameFileNames.colorBufNames[i][FIDT_POST_FRAME].size() > 0 ||
             frameFileNames.colorBufNames[i][FIDT_DIFF_FRAME].size() > 0)
          {
            fprintf(logFile,"<COLORBUFFER>");
            AddFrameTag("PRE",frameFileNames.colorBufNames     [i][FIDT_PRE_FRAME],
                              frameFileNames.colorBufNamesIcon [i][FIDT_PRE_FRAME]);
            AddFrameTag("POST",frameFileNames.colorBufNames    [i][FIDT_POST_FRAME],
                               frameFileNames.colorBufNamesIcon[i][FIDT_POST_FRAME]);
            AddFrameTag("DIFF",frameFileNames.colorBufNames    [i][FIDT_DIFF_FRAME],
                               frameFileNames.colorBufNamesIcon[i][FIDT_DIFF_FRAME]);
            fprintf(logFile,"</COLORBUFFER>");
          }
        }

        //If there is some depth frame buffer
        if(frameFileNames.depthBufNames[FIDT_PRE_FRAME ].size() > 0 ||
           frameFileNames.depthBufNames[FIDT_POST_FRAME].size() > 0 ||
           frameFileNames.depthBufNames[FIDT_DIFF_FRAME].size() > 0)
        {
          fprintf(logFile,"<DEPTHBUFFER>");
          AddFrameTag("PRE",frameFileNames.depthBufNames     [FIDT_PRE_FRAME],
                            frameFileNames.depthBufNamesIcon [FIDT_PRE_FRAME]);
          AddFrameTag("POST",frameFileNames.depthBufNames    [FIDT_POST_FRAME],
                             frameFileNames.depthBufNamesIcon[FIDT_POST_FRAME]);   
          AddFrameTag("DIFF",frameFileNames.depthBufNames    [FIDT_DIFF_FRAME],
                             frameFileNames.depthBufNamesIcon[FIDT_DIFF_FRAME]);   
          fprintf(logFile,"</DEPTHBUFFER>");
        }

        //If there is some stencil frame buffer
        if(frameFileNames.stencilBufNames[FIDT_PRE_FRAME ].size() > 0 ||
           frameFileNames.stencilBufNames[FIDT_POST_FRAME].size() > 0 ||
           frameFileNames.stencilBufNames[FIDT_DIFF_FRAME].size() > 0)
        {
          fprintf(logFile,"<STENCILBUFFER>");
          AddFrameTag("PRE",frameFileNames.stencilBufNames     [FIDT_PRE_FRAME],
                            frameFileNames.stencilBufNamesIcon [FIDT_PRE_FRAME]);
          AddFrameTag("POST",frameFileNames.stencilBufNames    [FIDT_POST_FRAME],
                             frameFileNames.stencilBufNamesIcon[FIDT_POST_FRAME]);   
          AddFrameTag("DIFF",frameFileNames.stencilBufNames    [FIDT_DIFF_FRAME],
                             frameFileNames.stencilBufNamesIcon[FIDT_DIFF_FRAME]);   
          fprintf(logFile,"</STENCILBUFFER>");
        }


        //Add the movie tag if a name is available
        if(frameFileNames.movieFileName.length() > 0)
        {
          ConvertFileNameXML(frameFileNames.movieFileName);
          fprintf(logFile,"<FRAMEMOVIE video=\"%s\"/>\n",frameFileNames.movieFileName.c_str());       
        }

        fprintf(logFile,"</FRAMEBUFFER>");
      }
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{
  //Check the function data
  if(!funcData)
  {
    LOGERR(("InterceptLogXML::LogFunctionPost - Function data for index %u is NULL",index)); 
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
      fprintf(logFile,"<RETURN type=\"%s\">",returnData->GetDisplayString().c_str());
      fprintf(logFile,"<VALUE data=\"%s\"/></RETURN>\n",out.c_str());
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::LogFunctionError(uint errorCode)
{
  //If the log is not writing or the error code is 0, return
  if(errorCode == 0x0)
  {
    return;
  }

  //Log that an error has occured
  fprintf(logFile,"<ERROR>");

  //Get the error string
  string errorString;
  GetErrorStringValue(errorCode,errorString);

  //Log the result
  fprintf(logFile,"<VALUE data=\"%s\"/>",errorString.c_str());

  fprintf(logFile,"</ERROR>");

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::LogFunctionComplete()
{
  //If the pre function has not been called, return
  if(preFunctionCalled == 0)
  {
    return;
  }

  //Test if we log function timer values
  if(functionTimerEnabled)
  {
    //Get the function time
    uint funcTime = glDriver.GetFunctionTime();
    if(funcTime >= functionTimerCutOff)
    {
      //Log the function time if it is greater than the cutoff
      fprintf(logFile,"<FUNCTIME data=\"%u\"/>",funcTime);
    }
  }

  //Indicate that the post function has been called
  preFunctionCalled--;

  //Complete the function
  fprintf(logFile,"</FUNCTION>\n"); 
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptLogXML::ConvertCustomParam(const ParamValue &data, bool isPointer,const ParameterData *paramData,string &retString)
{
  //Call base class to convert the custom types
  if(InterceptLogText::ConvertCustomParam(data,isPointer,paramData,retString))
  {
    //Strip the returned string of inavlid characters 
    ConvertStringXML(retString);
    return true;
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::AddParamaterTags(const ParamValue &data, bool isPointer,const ParameterData *paramData)
{
  //Certain types need extra data added first (not actually handled here)
  if(!isPointer)
  {
    //Determine the type
    switch(paramData->type)
    {
      case(PT_image_index):
        {
          //Add the image tag
          AddImageTag(data.uintValue);
          break;
        }

      case(PT_shader_index):
        {
          //Get the associated shader 
          AddShaderTagARB(data.uintValue,"");
          break;
        }

      case(PT_GLSL_program):
        {
          //Get the associated shader 
          AddShaderTagGLSL(data.uintValue,InterceptShaderGLSL::GLSLRT_GLSL_Program);
          break;
        }

      case(PT_GLSL_shader):
        {
          //Get the associated shader 
          AddShaderTagGLSL(data.uintValue,InterceptShaderGLSL::GLSLRT_GLSL_Shader);
          break;
        }

      case(PT_GLSL_ARB_handle):
        {
          //Get the associated shader 
          AddShaderTagGLSL(data.uintValue,InterceptShaderGLSL::GLSLRT_GLSL_ARB);
          break;
        }

      case(PT_display_list):
        {
          //Get the associated display list
          AddDisplayListTag(data.uintValue);
          break;
        }
        
      default:
        break;   
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::AddImageTag(uint texID)
{
  //Get the file names for the texture
  ImageSaveFiles imageFiles;
  if(glDriver.GetCurrentContext() && 
     glDriver.GetCurrentContext()->GetTextureFileNames(texID,imageFiles))
  {
    //Write out an image tag
    fprintf(logFile,"<IMAGE id=\"%u\"",texID);

    //Write the file name
    if(imageFiles.imageFileNames.size() > 0)
    {
      //Format the image name and print it
      ConvertFileNameXML(imageFiles.imageFileNames[0]);
      fprintf(logFile," name=\"%s\" ",imageFiles.imageFileNames[0].c_str()); 
    }

    //Write the icon name
    if(imageFiles.iconName.length() > 0)
    {
      //Format the icon name and print it
      ConvertFileNameXML(imageFiles.iconName);
      fprintf(logFile," icon=\"%s\" ",imageFiles.iconName.c_str());
    }

    fprintf(logFile,"/>"); 

    //Additional images are saved without the icon
    for(uint i=1;i<imageFiles.imageFileNames.size();i++)
    {
      //Format the icon name and print it
      ConvertFileNameXML(imageFiles.imageFileNames[i]);
      fprintf(logFile,"<IMAGE name=\"%s\" />",imageFiles.imageFileNames[i].c_str()); 
    }
  }
  else
  {
    //Write out an image tag with only the ID
    fprintf(logFile,"<IMAGE id=\"%u\" />",texID);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::AddShaderTagGLSL(uint shaderID,InterceptShaderGLSL::GLSLRequestType shaderType)
{
  string shaderName;

  //Check for a current context
  if(glDriver.GetCurrentContext())
  {
    //If the data could not be retrieved, ensure the string is empty
    if(!glDriver.GetCurrentContext()->GetShaderGLSLFileName(shaderID,shaderType,shaderName))
    {
      shaderName = "";
    }
  }

  AddShaderTag(shaderID, "GLSL", shaderName);
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::AddShaderTagARB(uint shaderID,const string &type)
{
  string shaderName;
  
  //Check for a current context
  if(glDriver.GetCurrentContext())
  {
    //Get the standard ARB shader file 
    if(!glDriver.GetCurrentContext()->GetShaderFileName(shaderID,shaderName))
    {
      shaderName = "";
    }
  }

  AddShaderTag(shaderID, type, shaderName);      
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::AddShaderTag(uint shaderID, const string &type, string &shaderName)
{
  //If there is a shader name to write, write it out
  if(shaderName.length() > 0)
  {
    //Convert the file name 
    ConvertFileNameXML(shaderName);

    //Check for a known type
    if(type != "")
    {
      //Write out a shader tag
      fprintf(logFile,"<SHADER id=\"%u\" type=\"%s\" name=\"%s\"/>",shaderID,type.c_str(),shaderName.c_str());
    }
    else
    {
      //Write out a shader tag
      fprintf(logFile,"<SHADER id=\"%u\" name=\"%s\"/>",shaderID,shaderName.c_str());
    }
  }
  else
  {
    //Check for a known type
    if(type != "")
    {
      //Write out an shader tag with type and ID
      fprintf(logFile,"<SHADER id=\"%u\" type=\"%s\" />",shaderID,type.c_str());
    }
    else
    {
      //Write out an shader tag with only the ID
      fprintf(logFile,"<SHADER id=\"%u\" />",shaderID);
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::AddFrameTag(const string &tagID, const string &filename, const string &fileNameIcon)
{
  //Only proceed if the file name has a length
  if(filename.length() > 0)
  {
    //Convert the file name and write it out
    string convertName = filename;
    ConvertFileNameXML(convertName);

    //If saving an icon file
    if(fileNameIcon.length() > 0)
    {
      string convertNameIcon = fileNameIcon;
      ConvertFileNameXML(convertNameIcon);

      fprintf(logFile,"<%s icon=\"%s\" image=\"%s\" />",tagID.c_str(),convertNameIcon.c_str(), convertName.c_str());    
    }
    else
    {
      fprintf(logFile,"<%s image=\"%s\" />",tagID.c_str(),convertName.c_str());    
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::AddDisplayListTag(uint listID)
{
  //Get the display list filename
  string listName;
  if(glDriver.GetCurrentContext() &&
     glDriver.GetCurrentContext()->GetDisplayListFileName(listID,listName) &&
     listName.length() > 0)
  {
    //Convert the file name 
    ConvertFileNameXML(listName);

    //Write out a display list tag
    fprintf(logFile,"<DLIST id=\"%u\" name=\"%s\"/>",listID,listName.c_str());
  }
  else
  {
    //Write out an display list tag 
    fprintf(logFile,"<DLIST id=\"%u\" />",listID);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::ConvertStringXML(string &convertStr)
{
  string retString;
  retString.reserve(convertStr.length()+5);

  //Loop for all characters
  for(uint i=0;i<convertStr.length();i++)
  {
    //Replace all quotes with a XML quote tag
    if(convertStr[i] == '"')
    {
      retString+="&quot;";  
    }
    else
    {
      retString+=convertStr[i];  
    }
  }

  convertStr = retString;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptLogXML::ConvertFileNameXML(string &convertFileName)
{
  //Get the path of this string
  string::size_type pathEnd = logFileName.rfind(FileUtils::dirSeparator);
  if(pathEnd != string::npos)
  {
    //Get the path
    string filePath = logFileName;
    filePath.erase(pathEnd+1,filePath.size()-pathEnd-1);

    //If the entire path is at the start of this string
    if(convertFileName.find(filePath,0) == 0)
    {
      //Strip the path
      convertFileName.erase(0,filePath.size());
    }
  }

  //Convert all back slashes to forward slashes
  //  (some XML browsers do not like back slashes)
  for(uint i=0;i<convertFileName.size();i++)
  {
    if(convertFileName[i] == '\\')
    {
      convertFileName[i] = '/';
    }
  }

}

