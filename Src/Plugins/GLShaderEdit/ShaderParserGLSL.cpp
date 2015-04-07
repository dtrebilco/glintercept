/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "ShaderParserGLSL.h"
#include <stdio.h>

#include <CommonErrorLog.h>

// The pre-processor includes
extern "C" {

int ScanFromString(char *s);
int yylex_CPP(char* buf, int maxSize);

int InitPreprocessor(void);
int FinalizePreprocessor(void);

}

// The maximum size a token can be
#define MAX_TOKEN_SIZE 1024

USING_ERRORLOG

// Helper struct of replacement tokens
struct ReplaceToken
{
  ReplaceToken(const char * replaceValue, const char *newValue);
  
  string replaceToken;
  string newTokenValue;
};

// Array of tokens to replace
ReplaceToken replaceTokenArray[] =
{
  ReplaceToken("gl_FragColor", "gli_debug_FragColor"),
  ReplaceToken("gl_FragData", "gli_debug_FragColor"),
};
#define NUM_REPLACE_TOKENS (sizeof(replaceTokenArray)/sizeof(ReplaceToken))


//@
//  Summary:
//    Helper class to interface with a C-based pre-processor.
//  
class ExtractTokens
{
public:

  //@
  //  Summary:
  //    Constructor
  //  
  ExtractTokens();

  //@
  //  Summary:
  //    Destructor, resets the pre-processor.
  //  
  virtual ~ExtractTokens();

  //@
  //  Summary:
  //    To initialize the pre-processor.
  //  
  //  Returns:
  //    If the pre-processor could be init, true is returned. 
  //    Else false is returned.
  //
  bool Init(); 

  //@
  //  Summary:
  //    To process a line of input string.
  //  
  //  Parameters:
  //    processStr - The string to process.
  //
  //    newTokens - The new tokens from that have been parsed.
  //
  //  Returns:
  //    If the string was processed successfully, true is returned. 
  //    Else false is returned.
  //
  bool ProcessLine(const string &processStr, vector<string> & newTokens);

  uint lineNumber;      // The current line number
  uint stringNumber;    // The current string number 

  string storedString;  // A temp stored string

  bool parserFailure;   // Flag indicating if the parser has failed

  uint parsedGLSLVersion; // The GLSL version parsed from the shader
  string headerStrings;   // Header string containing all the #pragmas/#extensions

  static ExtractTokens * currentProcessor;  // Pointer to the current token processor

};

ExtractTokens * ExtractTokens::currentProcessor = NULL; // Static pointer to current processor

///////////////////////////////////////////////////////////////////////////////
//
ShaderParserGLSL::ShaderParserGLSL():
isValid(false),
scopeCount(0),
bracketCount(0)
{

}

///////////////////////////////////////////////////////////////////////////////
//
ShaderParserGLSL::~ShaderParserGLSL()
{

}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderParserGLSL::Init(GLenum shaderType, const string &sourceString)
{
  //Init the variables
  bool inComment = false;
  string::size_type stringStart =0;
  string::size_type stringEnd   =0;

  // Clear any existing shader data
  isValid = false;
  shaderSourceLines.clear();
  shaderBreakLines.clear();
  shaderScopes.clear();
  scopeCount = 0;
  bracketCount = 0;

  // Create the token extractor
  ExtractTokens tokenExtractor;
  if(!tokenExtractor.Init())
  {
    LOGERR(("ShaderParserGLSL::Init - Could not init token extractor"));
    return false;
  }

  // The tokens extracted from the source file
  vector<ShaderLineData> extractedTokens;

  //Get a line from the input stream
  do
  {
    //Get the next line of input
    string inputString;
    stringEnd = sourceString.find("\n",stringStart);
    if(stringEnd != string::npos)
    {
      //Get the sub string
      inputString.assign(sourceString,stringStart,stringEnd - stringStart);
      stringStart = stringEnd +1;
    }
    else
    {
      //Take the remainder of the string
      inputString.assign(sourceString,stringStart, sourceString.length() - stringStart);
      stringStart = sourceString.length();
    }

    //Remove comments from the line
    RemoveComments(inputString, inComment);

    // Add a end of line character for the pre-processor
    inputString += "\n";

    // Extract the tokens from the string
    ShaderLineData newLineData;
    if(!tokenExtractor.ProcessLine(inputString, newLineData.tokenArray))
    {
      LOGERR(("ShaderParserGLSL::Init - Error extracting tokens"));
      return false;
    }

    // Add the new tokens to the process array
    extractedTokens.push_back(newLineData);
     
  }while(stringEnd != string::npos);

  //Check that all the section areas are completed
  if(inComment)
  {
    LOGERR(("ShaderParserGLSL::Init - Comment section not completed by end of string"));
    return false;
  }

  // Add in all closing braces for control statements

  /*  Loop and process all tokens

    // Process each character and if a ';' , '{' or '}', find the breakpoint lines  
    if(!ProcessControlCharacters(inputString, shaderSourceLines.size()))
    {
      return false;
    }

    // If in the global scope find the "main" token

    // Keep track of all the outer scopes (call stack)

    // Add the token data to the internal array
    shaderSourceLines.push_back(inputString);
  */

  // If bracket count does not match - abort
  if(scopeCount != 0 || bracketCount != 0)
  {
    LOGERR(("ShaderParserGLSL::Init - Bracket scopes do not end. Missing } or ) ?"));
    return false;
  }

  // If main function not found, abort

  // Flag that init was successful
  isValid = true;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderParserGLSL::ProcessControlCharacters(const string & line, uint lineNumber)
{

  // Find the function scopes. } { in the outer scope


  // Loop for all characters
  for(uint i=0; i<line.length(); i++)
  {
    // Update the bracket count
    if(line[i] == '(')
    {
      // Increment the bracket count
      bracketCount++;
    }
    else if(line[i] == ')')
    {
      // Decrement the count
      if(bracketCount == 0)
      {
        LOGERR(("ProcessControlCharacters - No matching '(' for ')'"));
        return false; 
      }
      bracketCount--;
    }
    
    // Only process break-point type positions outside "()" brackets
    if(bracketCount == 0)
    {
      // If it is a control character, process it
      switch(line[i])
      {
        case('{'):
          {
            // Increment the scope count
            scopeCount++;

            // If a new outer scope, add the new scope position
            if(scopeCount == 1)
            {
              // Set the scope start position
              ScopeData newScope;
              newScope.lineEntry     = lineNumber;
              newScope.positionEntry = i;
              shaderScopes.push_back(newScope);
            }

            // Update the current process point
            break;
          }

        case('}'):
          {
            // Decrement the scope count
            if(scopeCount == 0 || shaderScopes.size() == 0)
            {
              LOGERR(("ProcessControlCharacters - No matching '{' for '}'"));
              return false; 
            }
            scopeCount--;

            // If closing an outer scope, save the position
            if(scopeCount == 0)
            {
              // Set the scope end position
              shaderScopes[shaderScopes.size() - 1].lineExit = lineNumber;
              shaderScopes[shaderScopes.size() - 1].positionExit = i;
            }

            // Update the current process point

            break;
          }

        case(';'):
          {
            // If there is a current scope and process point, add a break position
            if(scopeCount > 0 && shaderScopes.size() > 0 /*&&  */)
            {

            }

            // Update the current process point

            break;
          }
      }
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderParserGLSL::RemoveComments(string & line, bool &inComment)
{
  string::size_type commentPos  = string::npos;
  string::size_type commentPos2 = string::npos;

  //If in a comment, check for ending "*/"
  if(inComment)
  {
    commentPos = line.find("*/");

    //If the ending string has been found 
    if(commentPos != string::npos)
    {
      //Remove the all the comments
      line.erase(0,commentPos+2);
      inComment = false;
    }
    else
    {
      //Just return an empty string
      line = ""; 
      return;
    }
  }

  // DT_TODO: Update for nested comments (eg /*blah // blah2*/ )

  //Check for "//"
  commentPos = line.find("//");

  //If the ending string has been found 
  if(commentPos != string::npos)
  {
    //Remove the all the comments from the comment on
    line.erase(commentPos,string::npos);
  }

  //Loop and check for "/*"
  commentPos = line.find("/*");
  while(commentPos != string::npos)
  {
    //Attempt to find an ending comment
    commentPos2 = line.find("*/",commentPos);
    
    //If there is no ending comment, erase the rest of the string
    if(commentPos2 == string::npos)
    {
      line.erase(commentPos,line.length() - commentPos); 
      inComment = true;
      return;
    }

    //Add a single space to the line so value/* */s are not merged together
    line[commentPos2+1] = ' ';

    //Erase the comment (the last / was replaced by a space)
    line.erase(commentPos,(commentPos2+1)-commentPos);   

    //Get the next comment position
    commentPos = line.find("/*");
  }
}


// DT_TODO: Fill out the preprocessor functionality
// DT_TODO: Ensure to pre-define all extension strings - both in parser and final string
//    static char *PreprocessorBuiltinString = "#define GL_ARB_texture_rectangle 1\n"
//                                             "#define GL_3DL_array_objects 1\n";

///////////////////////////////////////////////////////////////////////////////
//
ReplaceToken::ReplaceToken(const char * replaceValue, const char *newValue) :
replaceToken(replaceValue),
newTokenValue(newValue)
{
}

///////////////////////////////////////////////////////////////////////////////
//
ExtractTokens::ExtractTokens():
lineNumber(0),
stringNumber(0),

parserFailure(false),
parsedGLSLVersion(110)
{
}

///////////////////////////////////////////////////////////////////////////////
//
ExtractTokens::~ExtractTokens()
{
  // Release the static lock if it is held
  if(currentProcessor == this)
  {
    // Reset the pre-processor
    FinalizePreprocessor();

    // Set the current processor as empty
    currentProcessor = NULL;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
bool ExtractTokens::Init()
{
  // Check if there is a current processor
  if(currentProcessor != NULL)
  {
    return false;
  }

  // DT_TODO: Process all the initial pre-process strings

  // Init the pre-processor 
  InitPreprocessor();

  // Assign as the current processor. (not thread safe)
  currentProcessor = this;
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ExtractTokens::ProcessLine(const string &processStr, vector<string> & newTokens)
{
  if(currentProcessor != this)
  {
    return false;
  }
  
  // Reset the parser flag
  newTokens.clear();
  parserFailure = false;

  // Set the line scan string
  char * localSrcString = new char[processStr.length() + 1];
  strcpy(localSrcString, processStr.c_str());
  ScanFromString(localSrcString);

  // Extract each token, one by one
  char tokenBuffer[MAX_TOKEN_SIZE + 1];
  while (yylex_CPP(tokenBuffer, MAX_TOKEN_SIZE) > 0)
  {
    string newToken(tokenBuffer);

    // Perform token replacement
    for(uint i=0; i<NUM_REPLACE_TOKENS; i++)
    {
      // If matching a replace token value, assign the new token
      if(newToken == replaceTokenArray[i].replaceToken)
      {
        newToken = replaceTokenArray[i].newTokenValue;
        break;
      }
    }

    // Add the new token
    newTokens.push_back(newToken);
  }

  // Release the parse memory
  delete [] localSrcString;

  // If the parser failed
  if(parserFailure)
  {
    return false;
  }

  return true;
}


extern "C" {

///////////////////////////////////////////////////////////////////////////////
//
void CPPDebugLogMsg(const char *msg)
{
  // Ignore debug messages
}

///////////////////////////////////////////////////////////////////////////////
//
void CPPWarningToInfoLog(const char *msg)
{
  // Ignore warnings
}

///////////////////////////////////////////////////////////////////////////////
//
void CPPShInfoLogMsg(const char *msg)
{
  // Abort on error
  LOGERR(("Preprocessor parser failure: %s", msg));
  if(ExtractTokens::currentProcessor)
  {
    ExtractTokens::currentProcessor->parserFailure = true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void CPPErrorToInfoLog(char *msg)
{
  // Abort on error
  LOGERR(("Preprocessor parser failure: %s", msg));
  if(ExtractTokens::currentProcessor)
  {
    ExtractTokens::currentProcessor->parserFailure = true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void SetLineNumber(int newLineNumber)
{
  // Set the new line number
  if(ExtractTokens::currentProcessor)
  {
    ExtractTokens::currentProcessor->lineNumber = newLineNumber;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void SetStringNumber(int newStringNumber)
{
  // Set the new string number
  if(ExtractTokens::currentProcessor)
  {
    ExtractTokens::currentProcessor->stringNumber = newStringNumber;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
int GetStringNumber(void)
{
  // Retrieve the string number
  if(ExtractTokens::currentProcessor)
  {
    return ExtractTokens::currentProcessor->stringNumber;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
int GetLineNumber(void)
{
  // Retrieve the line number
  if(ExtractTokens::currentProcessor)
  {
    return ExtractTokens::currentProcessor->lineNumber;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
int GetGLSLVersion(void)
{
  // Return the GLSL version that is being used for compiling
  if(ExtractTokens::currentProcessor)
  {
    return ExtractTokens::currentProcessor->parsedGLSLVersion;
  }

  return 110;
}

///////////////////////////////////////////////////////////////////////////////
//
void IncLineNumber(void)
{
  if(ExtractTokens::currentProcessor)
  {
    ExtractTokens::currentProcessor->lineNumber++;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void DecLineNumber(void)
{
  if(ExtractTokens::currentProcessor)
  {
    ExtractTokens::currentProcessor->lineNumber--;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void SetParseShaderVersion(int version)
{
  // Assign the version number
  if(ExtractTokens::currentProcessor)
  {
    ExtractTokens::currentProcessor->parsedGLSLVersion = version;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void HandlePragma(const char **tokens, int numTokens)
{  
  // Abort if no processor
  if(!ExtractTokens::currentProcessor)
  {
    return;
  }

  // Re-construct the pragma
  string addString = "#pragma";
  
  // Loop and add each token
  for(int i=0; i<numTokens; i++)
  {
    addString += " ";
    addString += tokens[i];
  }

  addString += "\n";

  // Add to the header strings
  ExtractTokens::currentProcessor->headerStrings += addString;
}

///////////////////////////////////////////////////////////////////////////////
//
void updateExtensionBehavior(const char* extName, const char* behavior)
{
  // Abort if no processor
  if(!ExtractTokens::currentProcessor)
  {
    return;
  }

  // Re-construct the pragma
  string addString = "#extension";
  addString += " "; 
  addString += extName; 
  addString += " : "; 
  addString += behavior; 
  addString += "\n";

  // Add to the header strings
  ExtractTokens::currentProcessor->headerStrings += addString;
}

///////////////////////////////////////////////////////////////////////////////
//
void StoreStr(char *addString)
{
  // Append the string
  if(ExtractTokens::currentProcessor)
  {
    ExtractTokens::currentProcessor->storedString += string(" ") + addString;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
const char* GetStrfromTStr(void)
{
  // Get the string
  if(ExtractTokens::currentProcessor)
  {
    return ExtractTokens::currentProcessor->storedString.c_str();
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////
//
void ResetTString(void)
{
  // Reset the processing string
  if(ExtractTokens::currentProcessor)
  {
    ExtractTokens::currentProcessor->storedString = "";
  }
}


}  // extern "C"
