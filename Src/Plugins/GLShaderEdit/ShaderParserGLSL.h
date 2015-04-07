/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/

#ifndef __SHADER_PARSER_GLSL_H__
#define __SHADER_PARSER_GLSL_H__

#include "../../MainLib/InterceptPluginInterface.h"
#include <string>
#include <vector>

using namespace std;

// DT_TODO: Comments/ class checks

//@
//  Summary:
//    A basic GLSL parser to parse and extract valid line statements
//    and remove comments.
// 
class ShaderParserGLSL
{
public:
	ShaderParserGLSL();
	virtual ~ShaderParserGLSL();

  //@
  //  Summary:
  //    To parse the passed shader string and extract valid lines and
  //    the entry point.
  //  
  //  Parameters:
  //    shaderType - The shader type (vertex/fragment).
  //
  //    sourceString - The shader source.
  //
  //  Returns:
  //    If the shader was parsed successfully, 
  //    true is returned. Else false is returned.
  //
  bool Init(GLenum shaderType, const string &sourceString);

  //@
  //  Summary:
  //    To get if the parser has been init with valid data.
  //
  //  Returns:
  //    If the parser has been successfully init, true is returned. Else false
  //    is returned.
  //
  inline bool IsValid() const;

protected:

  struct ShaderLineData
  {
    vector<string> tokenArray; // The array of raw tokens for the shader line
    string         sourceLine; // The shader source line
  };
  
  // The line data for a breakpoint position
  struct BreakLineData
  {
    uint lineNumber;      // The line number of a possible break position
    uint linePosition;    // The line position of a possible break position 

    uint breakLineNumber; // The line number that triggers the break point position

    uint scopeIndex;      // What scope the break belongs to
  };

  // Data structure that contains scope information
  struct ScopeData
  {
    // DT_TODO: Add a constructor
    string name;        // The name of the scope

    uint lineEntry;     // The line number of the scope entry
    uint positionEntry; // The line position of the scope entry

    uint lineExit;      // The line number of the scope exit 
    uint positionExit;  // The line position of the scope exit
  };

  bool           isValid;            // Flag indicating if the parser has been initialized with valid data
  vector<string> shaderSourceLines;  // The array of each line of source (with comments stripped)

  vector<BreakLineData> shaderBreakLines; // The array of shader breakpoint lines
  vector<ScopeData>     shaderScopes;     // The array of shader scope positions

  uint scopeCount;     // The scope count of "{}" brackets
  uint bracketCount;   // The scope count of "()" brackets

  //@
  //  Summary:
  //    To process the control characters in the passed string.
  //  
  //  Parameters:
  //    line - The line to process.
  //
  //    lineNumber - The line number.
  //
  //  Returns:
  //    If the line cound be processed, true is returned. Else false
  //    is returned.
  //
  bool ProcessControlCharacters(const string & line, uint lineNumber);

  //@
  //  Summary:
  //    To remove the comments from a line
  //  
  //  Parameters:
  //    line - The line to remove comments from.
  //
  //    inComment - Flag indicating if currently inside a comment block.
  //
  static void RemoveComments(string & line, bool &inComment);

};

///////////////////////////////////////////////////////////////////////////////
//
inline bool ShaderParserGLSL::IsValid() const
{
  return isValid;
}

#endif //__SHADER_PARSER_GLSL_H__
