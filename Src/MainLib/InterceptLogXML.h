/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __INTERCEPT_LOG_XML_H_
#define __INTERCEPT_LOG_XML_H_

#include "GLInterceptConfig.h"
#include <stdio.h>
#include <string>
#include "InterceptLogText.h"
#include "InterceptShaderGLSL.h"

using namespace std;

class InterceptLogXML : public InterceptLogText 
{
public:

  //@
  //  Summary:
  //    Constructor to set up the log file. The default loggig is to stdout.
  //  
  //  Parameters:
  //    fileName - A file to open and use for logging.
  //
  //    functionTable - The table of all functions that are known/are being logged.
  //
  //    configData    - The configuration options for logging.
  //
  //    xslFileName   - The xsl file name used to format this XML (if any - can be empty).
  //
  InterceptLogXML(const string &fileName,FunctionTable * functionTable,const ConfigData &configData,
                  const string &xslFileName);

  //@
  //  Summary:
  //    Destructor, completes the log.
  //  
	virtual ~InterceptLogXML();

  //@
  //  Summary:
  //    To initialise the loog for writing. (ie. writes headers etc)
  //
  //  Returns:
  //    True is returned on init success, false if not.
  //
  virtual bool Init();

  //@
  //  Summary:
  //    To log the passed function and function data 
  //    (Before the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    args     - The arguments of the function.
  //
  virtual void LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args);

  //@
  //  Summary:
  //    To perform any post-call logging of a function.
  //    (After the actual function is called)
  //  
  //  Parameters:
  //    funcData - The data of the function logged.
  //
  //    index    - A index to the function logged (In the function table).
  //
  //    retVal   - The return value (if any).
  //
  virtual void LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal);

  //@
  //  Summary:
  //    To log that a GL error has occured.
  //  
  //  Parameters:
  //    errorCode - The GL error code to log.
  //
  virtual void LogFunctionError(uint errorCode);

  //@
  //  Summary:
  //    To log that the current function has completed. 
  //    (ie. All data for the previous function has been processed)
  //  
  virtual void LogFunctionComplete();

protected:

  uint   preFunctionCalled;                       // Counter indicating the "depth" of the pre-function log (GL calls can be made inside of other GL calls)
  bool   frameRenderCallLog;                      // Flag to indicate if frame buffer logging is performed.
  string xslFileName;                             // The file name of the XSL file used to format this file (if any)

  //@
  //  Summary:
  //    If the type of the parameter is custom (ie. non-standard OpenGL)
  //    calling this method will attempt to handle the type.
  //  
  //  Parameters:
  //    data  - The parameter.
  //
  //    isPointer - Flag to indicate if the value is a pointer
  //
  //    paramData  - A pointer to the current parameter data being processed.
  //
  //    retString  - If the data is handled, the data for the type is returned in the string.
  //
  //  Returns:
  //    If the parameter was converted, true is returned and the data is stored in retString.
  //    Else false is returned and retString is unchanged.
  //
  virtual bool ConvertCustomParam(const ParamValue & data, bool isPointer,const ParameterData *paramData,string &retString);

  //@
  //  Summary:
  //    To add special tags for special parameter typs to the XML output.
  //    (ie image/shader tags)
  //  
  //  Parameters:
  //    data  - The parameter.
  //
  //    isPointer - Flag to indicate if the value is a pointer
  //
  //    paramData  - A pointer to the current parameter data being processed.
  //
  void AddParamaterTags(const ParamValue &data, bool isPointer,const ParameterData *paramData);

  //@
  //  Summary:
  //    To add a image tag using the passed texture ID.
  //  
  //  Parameters:
  //    texID  - The texture ID to add a tag for.
  //
  void AddImageTag(uint texID);

  //@
  //  Summary:
  //    To add a GLSL shader tag using the passed shader ID.
  //  
  //  Parameters:
  //    shaderID  - The shader ID to add a tag for.
  //
  //    shaderType- The type of the shader (program/shader/unknown ARB).
  //
  void AddShaderTagGLSL(uint shaderID,InterceptShaderGLSL::GLSLRequestType shaderType);

  //@
  //  Summary:
  //    To add a ARB program shader tag using the passed shader ID.
  //  
  //  Parameters:
  //    shaderID  - The shader ID to add a tag for.
  //
  //    type      - The type of the shader (can be empty for unknown)
  //
  void AddShaderTagARB(uint shaderID,const string &type);

  //@
  //  Summary:
  //    To add a shader tag using the passed shader ID.
  //  
  //  Parameters:
  //    shaderID  - The shader ID to add a tag for.
  //
  //    type      - The type of the shader (can be empty for unknown)
  //
  //    shaderName- The filename of the shader. (May be converted to XML format)
  //
  void AddShaderTag(uint shaderID,const string &type, string &shaderName);

  //@
  //  Summary:
  //    To add a frame buffer tag using the passed tag ID.
  //  
  //  Parameters:
  //    tagID  - The ID of the tag.
  //
  //    filename - The filename of the tag. (if empty no tag will be written).
  //
  //    fileNameIcon - The icon for the filename
  //
  void AddFrameTag(const string &tagID, const string &filename, const string &fileNameIcon);

  //@
  //  Summary:
  //    To add a display list tag using the passed ID.
  //  
  //  Parameters:
  //    listID  - The display list ID to add a tag for.
  //
  void AddDisplayListTag(uint listID);

  //@
  //  Summary:
  //    To convert the passed string to be valid for including in a XML 
  //    attribute data section.
  //  
  //  Parameters:
  //    convertStr  - The string to convert.
  //
  void ConvertStringXML(string &convertStr);

  //@
  //  Summary:
  //    To convert the passed path/filename string to be valid for including in a XML 
  //    attribute data section. (also if possible, makes the image relative to this log's path)
  //  
  //  Parameters:
  //    convertFileName  - The fileName string to convert.
  //
  void ConvertFileNameXML(string &convertFileName);

};

#endif // __INTERCEPT_LOG_XML_H_
