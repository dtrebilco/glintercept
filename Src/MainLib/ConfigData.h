/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __CONFIG_DATA_H_
#define __CONFIG_DATA_H_

#include "GLInterceptConfig.h"
#include "GLDefines.h"

#include <string>
#include <vector>

using namespace std;

class ConfigParser;
class ConfigToken;

//@
//  Summary:
//    This structure holds all the configuration data used by the GLDriver
//    and associated classes.
//  
class ConfigData
{
public:

  //@
  //  Summary:
  //    Constructor. Inits all config data to default values.
  //  
  ConfigData();

  //@
  //  Summary:
  //    To read the config data values from a configuration file.
  //  
  void ReadConfigData();

  bool logEnabled;                                // Flag to indicate if logging is enabled
  bool logXMLFormat;                              // If true use XML to log, else use plain text.
  bool logFlush;                                  // If true, the logger is flushed after each function name is written (text file only)

  bool logMaxFrameLoggingEnabled;                 // If a maximum number of frames to log has been specified
  uint logMaxNumLogFrames;                        // The maximum number of frames to log (if logMaxFrameLoggingEnabled is true)

  string logXSLFile;                              // The XSL file to use when XML logging is enabled
  string logXSLBaseDir;                           // The base directory where the XSL file is located

  bool errorGetOpenGLChecks;                      // Flag to indicate if errors are checked for
  bool errorThreadChecking;                       // Flag to indicate if thread checking is performed.
  bool errorBreakOnError;                         // Flag to indicate if to break on an error
  bool errorLogOnError;                           // Flag to indicate if log OpenGL errors
  bool errorExtendedLogError;                     // Flag to indicate if extended data about an error is reported
  bool errorDebuggerErrorLog;                     // Flag to indicate if the error log is mirrored to the debugger

  bool logPerFrame;                               // Flag to indicate if we log per-frame or by the entire application
  bool logOneFrameOnly;                           // Flag to indicate if per-frame logging will only get one frame at a time
  vector<uint> logFrameKeys;                      // The key codes used to enable per-frame logging

  string logPath;                                 // The path to write the log files (including trailing seperator)
  string logName;                                 // The name of the log to write out (without extension)
  string functionDataFileName;                    // The name of the file/path to find the function config data
  string openGLFileName;                          // The full path to where the "real" opengl driver is

  bool imageLogEnabled;                           // Flag to indicate if the image log is enabled
  bool imageRenderCallStateLog;                   // Flag to indicate if the image state is recorded on render calls
  bool imageSaveIcon;                             // Save a icon version of the images 
  uint imageIconSize;                             // The size of the icon if saving icons
  string imageIconFormat;                         // The image format of the image icons

  bool imageSavePNG;                              // Save the images in PNG format
  bool imageSaveTGA;                              // Save the images in TGA format
  bool imageSaveJPG;                              // Save the images in JPG format
  bool imageFlipXAxis;                            // Flip the images on the X axis before writting out
  bool imageCubeMapTile;                          // Flag to indicate if cube maps are tiled together or saved as six images 

  bool imageSave1D;                               // Flag to indicate if 1D textures are saved
  bool imageSave2D;                               // Flag to indicate if 2D textures are saved (includes rect images)
  bool imageSave3D;                               // Flag to indicate if 3D textures are saved
  bool imageSaveCube;                             // Flag to indicate if Cube textures are saved
  bool imageSavePBufferTex;                       // Flag to indicate if textures that are bound to p-buffers are saved

  bool shaderLogEnabled;                          // Flag to indicate if the shader log is enabled
  bool shaderRenderCallStateLog;                  // Flag to indicate if the shader state is recorded on render calls
  bool shaderAttachLogState;                      // Flag to indicate if the shader log data is to be written.
  bool shaderValidatePreRender;                   // Flag to indicate if the shader is validated before each render.
  bool shaderLogUniformsPreRender;                // Flag to indicate if the shader is to log the uniforms before each render.

  bool displayListLogEnabled;                     // Flag to indicate if the display list log is enabled


  bool frameLogEnabled;                           // Flag to indicate if the frame log is enabled
  vector<string> frameAdditionalRenderCalls;      // Additional functions for which to dump framebuffer

  string frameImageFormat;                        // The format to save frame images in.
  bool framePreColorSave;                         // Save pre-color frame images
  bool framePostColorSave;                        // Save post-color frame images
  bool frameDiffColorSave;                        // Save diff-color frame images

  bool framePreDepthSave;                         // Save pre-depth frame images
  bool framePostDepthSave;                        // Save post-depth frame images
  bool frameDiffDepthSave;                        // Save diff-depth frame images

  bool framePreStencilSave;                       // Save pre-stencil frame images
  bool framePostStencilSave;                      // Save post-stencil frame images
  bool frameDiffStencilSave;                      // Save diff-stencil frame images
  vector<uint> frameStencilColors;                // The stencil color values to use when logging a stancil frame

  bool frameIconSave;                             // If icon images of the frame buffer are saved
  uint frameIconSize;                             // The save size of the frame buffer icon images
  string frameIconImageFormat;                    // The image format of the frame buffer icon images

  bool frameMovieEnabled;                         // If frame movies are enabled
  uint frameMovieWidth;                           // The frame movie width
  uint frameMovieHeight;                          // The frame movie height
  uint frameMovieRate;                            // The frame movie rate
  vector<string> frameMovieCodecs;                // The fraem movie codecs for compression

  bool timerLogEnabled;                           // Flag to indicate if the timer log is enabled
  uint timerLogCutOff;                            // The cutoff value for the timer log.


  string pluginBasePath;                          // The root path to the plugin directory
  
  //Structure of config data
  struct PluginData
  {
    string pluginName;                            // The name of the plugin to load 
    string pluginDLLName;                         // The name of the plugin dll to load 
    string pluginConfigData;                      // The configuration data for the plugin
  };
  
  vector<PluginData> pluginDataArray;             // The names of the dlls to load

protected:

  //@
  //  Summary:
  //    To read the function log config property values.
  //  
  void ReadFunctionLogConfigData(ConfigParser &parser);

  //@
  //  Summary:
  //    To read the per-frame config property values.
  //  
  void ReadPerFrameConfigData(ConfigParser &parser);

  //@
  //  Summary:
  //    To read the input files config property values.
  //  
  void ReadInputFilesConfigData(ConfigParser &parser);

  //@
  //  Summary:
  //    To read the error checking config property values.
  //  
  void ReadErrorCheckingConfigData(ConfigParser &parser);

  //@
  //  Summary:
  //    To read the image config property values.
  //  
  void ReadImageConfigData(ConfigParser &parser);

  //@
  //  Summary:
  //    To read the shader config property values.
  //  
  void ReadShaderConfigData(ConfigParser &parser);

  //@
  //  Summary:
  //    To read the display list config property values.
  //  
  void ReadDisplayListConfigData(ConfigParser &parser);

  //@
  //  Summary:
  //    To read the frame buffer logger config property values.
  //  
  void ReadFrameConfigData(ConfigParser &parser);

  //@
  //  Summary:
  //    To read the pre/post/diff options from a frame logger option.
  //  
  //  Parameters:
  //    frameTestToken  - The frame logger option for the token to test.
  //
  //    preToken - The returned pre value.
  //
  //    postToken - The returned post value.
  //
  //    diffToken - The returned diff value.
  //
  void ReadFramePrePostOptions(const ConfigToken *frameTestToken, bool &preToken, bool &postToken, bool &diffToken) const;

  //@
  //  Summary:
  //    To read the timer logger config property values.
  //  
  void ReadTimerConfigData(ConfigParser &parser);

  //@
  //  Summary:
  //    To read the plugin data config property values.
  //  
  void ReadPluginData(ConfigParser &parser);

  //@
  //  Summary:
  //    To get a valid image format from the passed token and
  //    set it in the passed string. If no valid format is retrieved,
  //    the return string is unchanged.
  //  
  //  Parameters:
  //    testToken  - The token to extract the image format from.
  //
  //    retValue - The string to return the value in.
  //
  void GetImageFormat(const ConfigToken *testToken, string &retValue);

};



#endif // __CONFIG_DATA_H_
