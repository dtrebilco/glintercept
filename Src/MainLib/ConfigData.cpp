/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/

#include "ConfigData.h"

#include <ConfigParser.h>
#include <FileUtils.h>
#include <InputUtils.h>

extern string dllPath;

USING_ERRORLOG


///////////////////////////////////////////////////////////////////////////////
//
ConfigData::ConfigData():
logEnabled(false),
logXMLFormat(false),
logFlush(false),

logMaxFrameLoggingEnabled(false),
logMaxNumLogFrames(0),

logXSLFile(""),
logXSLBaseDir(""),

errorGetOpenGLChecks(false),
errorThreadChecking(false),
errorBreakOnError(true),
errorLogOnError(true),
errorExtendedLogError(false),
errorDebuggerErrorLog(true),

logPerFrame(false),
logOneFrameOnly(true),

logPath(""),
logName("gliInterceptLog"),
functionDataFileName("gliIncludes.h"),

imageLogEnabled(false),
imageRenderCallStateLog(true),
imageSaveIcon(true),
imageIconSize(32),
imageIconFormat("jpg"),

imageSavePNG(true),
imageSaveTGA(false),
imageSaveJPG(false),
imageFlipXAxis(false),
imageCubeMapTile(true),

imageSave1D(true),
imageSave2D(true),
imageSave3D(true),
imageSaveCube(true),
imageSavePBufferTex(true),

shaderLogEnabled(false),
shaderRenderCallStateLog(true),
shaderAttachLogState(true),
shaderValidatePreRender(false),
shaderLogUniformsPreRender(false),

displayListLogEnabled(false),

frameLogEnabled(false),
frameImageFormat("jpg"),
framePreColorSave(false),
framePostColorSave(false),
frameDiffColorSave(false),

framePreDepthSave(false),
framePostDepthSave(false),
frameDiffDepthSave(false),

framePreStencilSave(false),
framePostStencilSave(false),
frameDiffStencilSave(false),

frameIconSave(false),
frameIconSize(40),
frameIconImageFormat("jpg"),

frameMovieEnabled(false),
frameMovieWidth(640),
frameMovieHeight(480),
frameMovieRate(15),


timerLogEnabled(false),
timerLogCutOff(1),

pluginBasePath("")
{

#ifdef GLI_BUILD_WINDOWS

  //Construct the path to the OpenGL filename
  char DefaultGLLibName[MAX_PATH];

  //Get the default library name
  GetSystemDirectory(DefaultGLLibName, MAX_PATH);

  //Assign the name
  openGLFileName = string(DefaultGLLibName) + FileUtils::dirSeparator + string("opengl32.dll");
  
#endif //GLI_BUILD_WINDOWS  

#ifdef GLI_BUILD_LINUX

  //On Linux, the SGI ABI spec states there is only one default path for OpenGL
  // (http://oss.sgi.com/projects/ogl-sample/ABI/)
  openGLFileName = "/usr/lib/libGL.so";

#endif //GLI_BUILD_LINUX

}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadConfigData()
{
  //Parse the config files
  ConfigParser parser;
  
  //If parsing was successful, add the data to the function table
  if(!parser.Parse(dllPath + "gliConfig.ini"))
  {
    LOGERR(("ConfigData::ReadConfigData - Unable to read config data file - %sgliConfig.ini",dllPath.c_str()));
    return;
  }

  // Read the main function log options
  ReadFunctionLogConfigData(parser);

  // Read the pre-frame config data
  ReadPerFrameConfigData(parser);

  // Read the input file names
  ReadInputFilesConfigData(parser);

  // Read the error checking options
  ReadErrorCheckingConfigData(parser);

  //Read in the image data
  ReadImageConfigData(parser);

  //Read the shader data
  ReadShaderConfigData(parser);

  //Read the display list data
  ReadDisplayListConfigData(parser);
  
  //Read the frame log data
  ReadFrameConfigData(parser);

  //Read the timer options
  ReadTimerConfigData(parser);

  //Read plugin data
  ReadPluginData(parser);

  //Log all unused tokens
  parser.LogUnusedTokens();
}


///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadFunctionLogConfigData(ConfigParser &parser)
{
  // Get if logging the OpenGL function calls
  const ConfigToken *functionLogToken = parser.GetToken("FunctionLog");
  if(!functionLogToken)
  {
    return;
  }

  //Get if the log is enabled
  const ConfigToken *testToken = functionLogToken->GetChildToken("LogEnabled");
  if(testToken)
  {
    testToken->Get(logEnabled);
  }

  //Get if the log is flushed
  testToken = functionLogToken->GetChildToken("LogFlush");
  if(testToken)
  {
    testToken->Get(logFlush);
  }

  //Get list of additional render calls
  testToken = functionLogToken->GetChildToken("AdditionalRenderCalls");
  if(testToken)
  {
    //Loop for all values
    for(uint i=0; i < testToken->GetNumValues(); i++)
    {
      //Get each function name and add to the array
      string newValue;
      testToken->Get(newValue,i);
      frameAdditionalRenderCalls.push_back(newValue);
    }
  }

  //Get if a maximum number of logging frames has been specified
  testToken = functionLogToken->GetChildToken("LogMaxNumFrames");
  if(testToken)
  {
    logMaxFrameLoggingEnabled = true;
    testToken->Get(logMaxNumLogFrames);
  }

  //Get the log path
  testToken = functionLogToken->GetChildToken("LogPath");
  if(testToken)
  {
    testToken->Get(logPath);

    if(logPath.size() == 0)
    {
      //Assign the dll path as the log path
      logPath = dllPath;
    }
    //If no trailing slash is provided, add one
    //  (Perhaps check that it is a valid path?)
    else if(logPath[logPath.size()-1] != '\\' &&
            logPath[logPath.size()-1] != '/')
    {
      logPath = logPath + FileUtils::dirSeparator;
    }
  }
  else
  {
    //Assign the dll path as the log path
    logPath = dllPath;
  }

  //Get the log file name
  testToken = functionLogToken->GetChildToken("LogFileName");
  if(testToken)
  { 
    testToken->Get(logName);
  }

  //Get if the format is XML or not
  testToken = functionLogToken->GetChildToken("LogFormat");
  if(testToken)
  { 
    //Get an test the string for XML format
    string value;
    if(testToken->Get(value))
    {
      if(value == "XML" || value == "xml")
      {
        logXMLFormat = true;
      }
    }
  }

  //Get the XML format data
  const ConfigToken *xmlFormatToken = functionLogToken->GetChildToken("XMLFormat");
  if(xmlFormatToken)
  {
    //Get the XSL file
    testToken = xmlFormatToken->GetChildToken("XSLFile");
    if(testToken)
    { 
      testToken->Get(logXSLFile);
    }

    //Get the base directory
    testToken = xmlFormatToken->GetChildToken("BaseDir");
    if(testToken)
    { 
      testToken->Get(logXSLBaseDir);

      //Add a trailing seperator
      if(logXSLBaseDir.size() > 0 &&
         logXSLBaseDir[logXSLBaseDir.size()-1] != '\\' &&
         logXSLBaseDir[logXSLBaseDir.size()-1] != '/')
      {
        logXSLBaseDir = logXSLBaseDir + FileUtils::dirSeparator;
      }
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadPerFrameConfigData(ConfigParser &parser)
{
  //Get if the logging is per-frame or not
  const ConfigToken *perFrameToken = parser.GetToken("LogPerFrame");
  if(!perFrameToken)
  {
    return;
  }

  //Get if per frame logging is enabled
  const ConfigToken *testToken = perFrameToken->GetChildToken("Enabled");
  if(testToken)
  {
    testToken->Get(logPerFrame);
  }

  //Get if per-frame logging is forced to one frame at a time
  testToken = perFrameToken->GetChildToken("OneFrameOnly");
  if(testToken)
  {
    testToken->Get(logOneFrameOnly);
  }

  //Get the key codes
  testToken = perFrameToken->GetChildToken("FrameStartKeys");
  if(testToken)
  {
    //Loop for the number of values in the token
    for(uint i=0;i<testToken->GetNumValues();i++)
    {
      string value;
      testToken->Get(value,i);

      //Get the key code of the string
      uint newValue = InputUtils::GetKeyCode(value);
      if(newValue != 0)
      {
        //Add the value to the array
        logFrameKeys.push_back(newValue);
      }
    }       
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadInputFilesConfigData(ConfigParser &parser)
{
  // Get the inoput files section
  const ConfigToken *inputFilesToken = parser.GetToken("InputFiles");
  if(!inputFilesToken)
  {
    return;
  }

  //Get the OpenGL function defines
  const ConfigToken *testToken = inputFilesToken->GetChildToken("GLFunctionDefines");
  if(testToken)
  { 
    testToken->Get(functionDataFileName);
  }

  //Get the OpenGL system library
  testToken = inputFilesToken->GetChildToken("GLSystemLib");
  if(testToken)
  { 
    testToken->Get(openGLFileName);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadErrorCheckingConfigData(ConfigParser &parser)
{
  // Get the error checking section
  const ConfigToken *errorCheckToken = parser.GetToken("ErrorChecking");
  if(!errorCheckToken)
  {
    return;
  }

  //Determine if we issue error check calls
  const ConfigToken *testToken = errorCheckToken->GetChildToken("GLErrorChecking");
  if(testToken)
  {
    testToken->Get(errorGetOpenGLChecks);
  }

  //Determine if thread checking is performed
  testToken = errorCheckToken->GetChildToken("ThreadChecking");
  if(testToken)
  {
    testToken->Get(errorThreadChecking);
  }

  //Determine if we break on errors
  testToken = errorCheckToken->GetChildToken("BreakOnError");
  if(testToken)
  {
    testToken->Get(errorBreakOnError);
  }

  //Determine if we log on OpenGL error
  testToken = errorCheckToken->GetChildToken("LogOnError");
  if(testToken)
  {
    testToken->Get(errorLogOnError);
  }

  //Get if there is extened error log reporting
  testToken = errorCheckToken->GetChildToken("ExtendedErrorLog");
  if(testToken)
  {
    testToken->Get(errorExtendedLogError);
  }

  //Determine if we mirror the error log to the debug window
  testToken = errorCheckToken->GetChildToken("DebuggerErrorLog");
  if(testToken)
  {
    testToken->Get(errorDebuggerErrorLog);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadImageConfigData(ConfigParser &parser)
{
  //Get if the outer log section
  const ConfigToken * imgToken = parser.GetToken("ImageLog");
  if(!imgToken)
  {
    return;
  }
  const ConfigToken *imgTestToken;

  //Get if the log is enabled
  imgTestToken = imgToken->GetChildToken("LogEnabled");
  if(imgTestToken)
  { 
    imgTestToken->Get(imageLogEnabled);
  }

  //Get if we log the state on render calls 
  imgTestToken = imgToken->GetChildToken("RenderCallStateLog");
  if(imgTestToken)
  { 
    imgTestToken->Get(imageRenderCallStateLog);
  }

  //Get if icon image saving is enabled
  const ConfigToken *imgIconToken = imgToken->GetChildToken("ImageIcon");
  if(imgIconToken)
  {
    //Get if icon saving is enabled
    imgTestToken = imgIconToken->GetChildToken("Enabled");
    if(imgTestToken)
    {
      imgTestToken->Get(imageSaveIcon);
    }

    //Get the size of the icon image
    imgTestToken = imgIconToken->GetChildToken("Size");
    if(imgTestToken)
    {
      imgTestToken->Get(imageIconSize);
    }

    //Get the format of the icon file
    imgTestToken = imgIconToken->GetChildToken("SaveFormat");
    imageIconFormat = "jpg";
    if(imgTestToken)
    {
      GetImageFormat(imgTestToken, imageIconFormat);
    }
  }

  //Get the save formats
  imgTestToken = imgToken->GetChildToken("SaveFormats");
  if(imgTestToken)
  { 
    //Reset all save formats
    imageSavePNG  = false;
    imageSaveTGA  = false;
    imageSaveJPG  = false;

    //Loop for the number of values in the token
    for(uint i=0;i<imgTestToken->GetNumValues();i++)
    {
      string value;
      imgTestToken->Get(value,i);

      if(value == "PNG")
      {
        imageSavePNG = true;
      }
      else if(value == "TGA")
      {
        imageSaveTGA = true;
      }
      else if(value == "JPG")
      {
        imageSaveJPG = true;
      }
      else
      {
        LOGERR(("ConfigData::ReadImageConfigData - Unknown texture save format %s",value.c_str()));
      }
    }
  }

  //Get the flip X axis 
  imgTestToken = imgToken->GetChildToken("FlipXAxis");
  if(imgTestToken)
  { 
    imgTestToken->Get(imageFlipXAxis);
  }

  //Get the cube map tile property 
  imgTestToken = imgToken->GetChildToken("TileCubeMaps");
  if(imgTestToken)
  { 
    imgTestToken->Get(imageCubeMapTile);
  }

  //Get the GL texture saveing formats
  imgTestToken = imgToken->GetChildToken("SaveGLTypes");
  if(imgTestToken)
  { 
    //Reset all save formats
    imageSave1D  = false;
    imageSave2D  = false;
    imageSave3D  = false;
    imageSaveCube= false;

    //Loop for the number of values in the token
    for(uint i=0;i<imgTestToken->GetNumValues();i++)
    {
      string value;
      imgTestToken->Get(value,i);

      if(value == "1D")
      {
        imageSave1D = true;
      }
      else if(value == "2D")
      {
        imageSave2D = true;
      }
      else if(value == "3D")
      {
        imageSave3D = true;
      }
      else if(value == "CUBE")
      {
        imageSaveCube = true;
      }
      else
      {
        LOGERR(("ConfigData::ReadImageConfigData - Unknown GL texture format %s",value.c_str()));
      }
    }
  }

  //Get the p-buffer save property
  imgTestToken = imgToken->GetChildToken("SavePbufferTex");
  if(imgTestToken)
  { 
    imgTestToken->Get(imageSavePBufferTex);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadShaderConfigData(ConfigParser &parser)
{
  //Get the outer log section
  const ConfigToken * shaderToken = parser.GetToken("ShaderLog");
  if(!shaderToken)
  {
    return;
  }
  const ConfigToken *shaderTestToken;

  //Get if the log is enabled
  shaderTestToken = shaderToken->GetChildToken("LogEnabled");
  if(shaderTestToken)
  { 
    shaderTestToken->Get(shaderLogEnabled);
  }

  //Get if we log the state on render calls 
  shaderTestToken = shaderToken->GetChildToken("RenderCallStateLog");
  if(shaderTestToken)
  { 
    shaderTestToken->Get(shaderRenderCallStateLog);
  }

  //Get if we append log info 
  shaderTestToken = shaderToken->GetChildToken("AttachLogState");
  if(shaderTestToken)
  { 
    shaderTestToken->Get(shaderAttachLogState);
  }

  //Get if we validate the shaders
  shaderTestToken = shaderToken->GetChildToken("ValidatePreRender");
  if(shaderTestToken)
  { 
    shaderTestToken->Get(shaderValidatePreRender);
  }

  //Get if we log uniforms
  shaderTestToken = shaderToken->GetChildToken("UniformLogPreRender");
  if(shaderTestToken)
  { 
    shaderTestToken->Get(shaderLogUniformsPreRender);
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadDisplayListConfigData(ConfigParser &parser)
{
  //Get the outer log section
  const ConfigToken * listToken = parser.GetToken("DisplayListLog");
  if(!listToken)
  {
    return;
  }
  const ConfigToken *listTestToken;

  //Get if the log is enabled
  listTestToken = listToken->GetChildToken("LogEnabled");
  if(listTestToken)
  { 
    listTestToken->Get(displayListLogEnabled);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadFrameConfigData(ConfigParser &parser)
{
  //Get if the outer log section
  const ConfigToken * frameLogToken = parser.GetToken("FrameLog");
  if(!frameLogToken)
  {
    return;
  }
  
  const ConfigToken *frameTestToken;

  //Get if the log is enabled
  frameTestToken = frameLogToken->GetChildToken("LogEnabled");
  if(frameTestToken)
  { 
    frameTestToken->Get(frameLogEnabled);
  }

  //Get the save format
  frameImageFormat = "jpg";
  frameTestToken = frameLogToken->GetChildToken("SaveFormat");
  if(frameTestToken)
  { 
    GetImageFormat(frameTestToken, frameImageFormat);
  }

  // Get fullscreen flag
  frameTestToken = frameLogToken->GetChildToken("FullScreen");
  if (frameTestToken)
  {
      frameTestToken->Get(frameFullScreen);
  }

  //Get if icon frame buffer saving is enabled
  const ConfigToken *frameIconToken = frameLogToken->GetChildToken("FrameIcon");
  if(frameIconToken)
  {
    //Get if icon saving is enabled
    frameTestToken = frameIconToken->GetChildToken("Enabled");
    if(frameTestToken)
    {
      frameTestToken->Get(frameIconSave);
    }

    //Get the size of the icon image
    frameTestToken = frameIconToken->GetChildToken("Size");
    if(frameTestToken)
    {
      frameTestToken->Get(frameIconSize);
    }

    frameTestToken = frameIconToken->GetChildToken("SaveFormat");
    if(frameTestToken)
    {
      GetImageFormat(frameTestToken, frameIconImageFormat);
    }

  }

  //Load the frame movie token data
  const ConfigToken *frameMovieToken = frameLogToken->GetChildToken("FrameMovie");
  if(frameMovieToken)
  {
    //Get if movie saving is enabled
    frameTestToken = frameMovieToken->GetChildToken("Enabled");
    if(frameTestToken)
    {
      frameTestToken->Get(frameMovieEnabled);
    }

    //Get the width/height movie
    frameTestToken = frameMovieToken->GetChildToken("Size");
    if(frameTestToken)
    {
      if(frameTestToken->GetNumValues() != 2)
      {
        LOGERR(("FrameMovie - Config - Need two width/height values for size"));  
      }
      else
      {
        frameTestToken->Get(frameMovieWidth,0);
        frameTestToken->Get(frameMovieHeight,1);
      }
    }

    //Get the frame rate of the movie
    frameTestToken = frameMovieToken->GetChildToken("FrameRate");
    if(frameTestToken)
    {
      frameTestToken->Get(frameMovieRate);
    }

    //Get the compression
    frameTestToken = frameMovieToken->GetChildToken("Compression");
    if(frameTestToken)
    {
      //Loop for all values
      for(uint i=0; i<frameTestToken->GetNumValues(); i++)
      {
        //Get each codec and add to the array
        string newValue;
        frameTestToken->Get(newValue, i);
        frameMovieCodecs.push_back(newValue);
      }
    }

    //Add a no compression value if none are specified
    if(frameMovieCodecs.size() == 0)
    {
      frameMovieCodecs.push_back("none");
    }
  }


  //Get the pre/post color options
  frameTestToken = frameLogToken->GetChildToken("ColorBufferLog");
  if(frameTestToken)
  { 
    ReadFramePrePostOptions(frameTestToken,framePreColorSave,framePostColorSave, frameDiffColorSave);
  }


  //Get the pre/post depth options
  frameTestToken = frameLogToken->GetChildToken("DepthBufferLog");
  if(frameTestToken)
  { 
    ReadFramePrePostOptions(frameTestToken,framePreDepthSave,framePostDepthSave, frameDiffDepthSave);
  }

  //Get the pre/post stencil options
  frameTestToken = frameLogToken->GetChildToken("StencilBufferLog");
  if(frameTestToken)
  { 
    ReadFramePrePostOptions(frameTestToken,framePreStencilSave,framePostStencilSave, frameDiffStencilSave);
  }

  frameTestToken = frameLogToken->GetChildToken("StencilColors");
  if(frameTestToken)
  {
    //Test for the correct number of colors
    if(frameTestToken->GetNumValues() % 2 == 1)
    {
      LOGERR(("ConfigData::ReadFrameConfigData - Uneven number of stencil colors"));
    }
    else
    {
      int currValue =-1;
      int retIndex;
      uint retColor;

      //Loop for all the value pairs
      for(uint i=0;i<frameTestToken->GetNumValues();i+=2)
      {
        //Get the index/color pair
        if(!frameTestToken->Get(retIndex,i) ||
           !frameTestToken->Get(retColor,i+1))
        {
          LOGERR(("ConfigData::ReadFrameConfigData - Error retrieving stencil color value/index"));
          frameStencilColors.empty();
          break;
        }

        //Check that the index data is sorted
        if(retIndex <= currValue)
        {
          LOGERR(("ConfigData::ReadFrameConfigData - Unsorted array of stencil colors"));
          frameStencilColors.empty();
          break;
        }
        
        //Check bounds
        if(retIndex > 255)
        {
          LOGERR(("ConfigData::ReadFrameConfigData - Stencil index is too large: %d",retIndex));
          frameStencilColors.empty();
          break;
        }
        currValue = retIndex;

        //Add the pair to the return array
        frameStencilColors.push_back(retIndex);
        frameStencilColors.push_back(retColor);
      }
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadFramePrePostOptions(const ConfigToken *frameTestToken, bool &preToken, bool &postToken, bool &diffToken) const
{
  preToken = false;
  postToken= false;
  diffToken= false;

  //Loop for the number of values in the token
  for(uint i=0;i<frameTestToken->GetNumValues();i++)
  {
    string value;
    frameTestToken->Get(value,i);

    if(value == "pre")
    {
      preToken = true;
    }
    else if(value == "post")
    {
      postToken = true;
    }
    else if(value == "diff")
    {
      diffToken = true;
    }
    else
    {
      LOGERR(("ConfigData::ReadFramePrePostOptions - Unknown frame save option %s",value.c_str()));
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadTimerConfigData(ConfigParser &parser)
{
  //Get if the outer log section
  const ConfigToken * timerToken = parser.GetToken("TimerLog");
  if(!timerToken)
  {
    return;
  }
  const ConfigToken *timerTestToken;

  //Get if the log is enabled
  timerTestToken = timerToken->GetChildToken("LogEnabled");
  if(timerTestToken)
  { 
    timerTestToken->Get(timerLogEnabled);
  }

  //Get if we log the state on render calls 
  timerTestToken = timerToken->GetChildToken("LogCutoff");
  if(timerTestToken)
  { 
    timerTestToken->Get(timerLogCutOff);
  }


}


///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::ReadPluginData(ConfigParser &parser)
{
  //Get if the outer log section exists
  const ConfigToken * pluginDataToken = parser.GetToken("PluginData");
  if(!pluginDataToken)
  {
    return;
  }
  const ConfigToken *pluginTestToken;

  //Get the plugin base directory
  pluginTestToken = pluginDataToken->GetChildToken("BaseDir");
  if(pluginTestToken)
  { 
    pluginTestToken->Get(pluginBasePath);

    //Add a directory seperator
    pluginBasePath = pluginBasePath + FileUtils::dirSeparator;
  }

  //Get the plugin token
  pluginTestToken = pluginDataToken->GetChildToken("Plugins");
  if(pluginTestToken)
  { 
    //Loop for all children
    for(uint i=0;i<pluginTestToken->GetNumChildren();i++)
    {
      //Get the token and check that it has a value
      const ConfigToken *pToken = pluginTestToken->GetChildToken(i);
      if(pToken->GetNumValues() != 1)
      {
        LOGERR(("ConfigData::ReadPluginData - Error in PluginData:Plugins:%s -Expected one dll name value",pToken->GetName().c_str()));
        return;
      }

      //Fill out the plugin data
      PluginData newData;
      newData.pluginName = pToken->GetName();
      pToken->Get(newData.pluginDLLName,0);
      
      //Compile all child config data for the token
      for(uint childNum =0;childNum < pToken->GetNumChildren(); childNum++)
      {
        //Convert each child back to raw config string data
        string retString;
        if(parser.GenerateConfigString(pToken->GetChildToken(childNum),retString))
        {
          newData.pluginConfigData += retString;
        }
      }

      //Add the plugin data to the array
      pluginDataArray.push_back(newData);
    }
  }
 

}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigData::GetImageFormat(const ConfigToken *testToken, string &retValue)
{
  //Only process valid tokens
  if(!testToken)
  {
    return;
  }
  
  string value;
  testToken->Get(value);

  if(value == "PNG")
  {
    retValue = "png";
  }
  else if(value == "TGA")
  {
    retValue = "tga";
  }
  else if(value == "JPG")
  {
    retValue = "jpg";
  }
  else
  {
    LOGERR(("ConfigData::GetImageFormat - Unknown image save format %s",value.c_str()));
  }

}



