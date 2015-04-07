/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __FUNCTION_PARSER_H__
#define __FUNCTION_PARSER_H__

#include "GLInterceptConfig.h"

#include "EnumData.h"
#include "ParameterData.h"
#include "FunctionData.h"


#include <string>
#include <vector>

using namespace std;


//@
//  Summary:
//    A very crude function parser to read function definitions.
//  
class FunctionParser  
{
public:

  //Constructor/Destructor
  FunctionParser();
  virtual ~FunctionParser();

  //@
  //  Summary:
  //    To parse the passed filename and extract all functions/enum etc.
  //    (This method may parse other files)
  //  
  //  Parameters:
  //    fileName - The filename to open.
  //
  //  Returns:
  //    If the file (and all sub-files) was parsed successfully, 
  //    true is returned. Else false is returned.
  //
  bool Parse(const string & fileName);

  //@
  //  Summary:
  //    To add this parser's data to the passed arrays.
  //  
  //  Parameters:
  //    funcArray - The function array to add to.
  //
  //    enumArray - The enum array to add to.
  //
  //  Returns:
  //    If this parser's data conflicts with the current data in the passed 
  //    arrays (ie already existing data) false is returned. Else true is
  //    returned.
  //
  bool GetFunctionData(FunctionDataArray & funcArray,EnumDataArray & enumArray);


  //@
  //  Summary:
  //    To dump the parsed data (Code generation method).
  //  
  //  Parameters:
  //    fileName - The file name to write the data to.
  //
  //void WriteFunctionData(const char *fileName) const;

protected:

  FunctionDataArray  functionArray;               // The array to hold all the parsed data
  EnumDataArray      enumDataArray;               // The array of enum data (used by the functions)

  bool   inComment;                               // Flag to indicate if the current lines we are parsing are in comment
  bool   inFunction;                              // Flag to indicate if the current lines we are parsing are in a function
  bool   inEnum;                                  // Flag to indicate if the current lines we are parsing are in a enumaration

  string currEnumName;                            // The current name of the enum we are parsing 
  string currFunctionName;                        // The current name of the function we are parsing   

  EnumData * currEnum;                            // The current enum data store


  uint   lineCounter;                             // The current line number
  string fileName;                                // The current file

  static vector<string> parsedDefines;            //Array to ensure we do not parse the same file twice


  //Structure to hold the parsed tokens
  struct ParseToken
  {
    string name;                                  // Name of the token
    bool   optional;                              // Flag to indicate if the token is optional

  };
  typedef vector<ParseToken> TokenArray;


  //@
  //  Summary:
  //    To parse a single line
  //  
  //  Parameters:
  //    line - The line to parse and extract data from.
  //
  //  Returns:
  //    Returns true if the line is valid, false if not.
  //
  bool ParseLine(string line);

  //@
  //  Summary:
  //    To remove the comments from a line
  //  
  //  Parameters:
  //    line - The line to remove comments from.
  //
  void RemoveComments(string & line);

  //@
  //  Summary:
  //    To remove the white space and new line characters from a line.
  //  
  //  Parameters:
  //    line - The line edit.
  //
  void RemoveWhiteSpace(string & line);

  //@
  //  Summary:
  //    To check the line for invalid data
  //  
  //  Parameters:
  //    line - The line check.
  //
  //  Returns:
  //    Returns true if the line is valid, false if not.
  //
  bool CheckString(const string & line);

  //@
  //  Summary:
  //    To check if the line is a #include line
  //  
  //  Parameters:
  //    line - The line check.
  //
  //  Returns:
  //    Returns true if the line is a #include, false if not.
  //
  bool IsIncludeFile(const string & line) const;

  //@
  //  Summary:
  //    To check if the line is a enumeration line
  //  
  //  Parameters:
  //    line - The line check.
  //
  //  Returns:
  //    Returns true if the line is a enumeration, false if not.
  //
  bool IsEnum(const string & line) const;

  //@
  //  Summary:
  //    To check if the line is a function line
  //  
  //  Parameters:
  //    line - The line check.
  //
  //  Returns:
  //    Returns true if the line is a function, false if not.
  //
  bool IsFunction(const string & line) const;

  //@
  //  Summary:
  //    To parse a include file
  //  
  //  Parameters:
  //    line - The line to extract the include file from.
  //
  //  Returns:
  //    Returns true if the include was parsed successfully, false if not.
  //
  bool ParseIncludeFile(const string & line);

  //@
  //  Summary:
  //    To parse a enum 
  //  
  //  Parameters:
  //    line - The line to extract the enum data from.
  //
  //  Returns:
  //    Returns true if the enum data was parsed successfully, false if not.
  //
  bool ParseEnum(const string & line);

  //@
  //  Summary:
  //    To parse a function
  //  
  //  Parameters:
  //    line - The line to extract the function data from.
  //
  //  Returns:
  //    Returns true if the function data was parsed successfully, false if not.
  //
  bool ParseFunction(const string & line);

  //@
  //  Summary:
  //    To get an extract all the function tokens from the passed line.
  //  
  //  Parameters:
  //    line - The line to extract the tokens from.
  //
  //    array - The array which returns all the tokens.
  //
  //  Returns:
  //    Returns true if the function tokens are valid, false if not.
  //
  bool GetAllFunctionTokens(string &line,TokenArray & array);

  //@
  //  Summary:
  //    To get an extract a single function token from the passed line.
  //    (Assumes all duplicate spaces are stripped)
  //  
  //  Parameters:
  //    line - The line to extract the token from.
  //
  //    token - The returned token.
  //
  //    optional - Flag to indicate if the token was optional. (bracketed '[]')
  //
  //  Returns:
  //    Returns true if a function token could bve extracted, false if not.
  //
  bool GetFunctionToken(string &line,string &token,bool &optional);

};

#endif // __FUNCTION_PARSER_H__
