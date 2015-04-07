#ifndef __CONFIG_PARSER_H__
#define __CONFIG_PARSER_H__

#include <string>
#include <vector>

using namespace std;
typedef unsigned int uint;

///////////////////////////////////////////////////////////////////////////////
//
//  Syntax is as follows:
//
//  -  //, /* and " cannot be used in token names or values
//
//  -  // and /* */ comments can be used. Note that these tokens take 
//     precidence over all tokens so you cannot have values with // or /* */ 
//     in them. 
//      
//  -  A token is simply a string of characters. (ie boolname) All tokens are case 
//     sensetitive.
//
//  -  The = operator is used to assign a value to a token (ie boolname = true)
//
//  -  All tokens and values are seperated by spaces, end of line or reserved 
//     characters (which are  "=(){},; ). To assign a token or value with a 
//     reserved character (except for the " character) enclose it with quotes
//     (ie boolname = "string value" or emptystring = "";)
//
//  -  A token can be assigned multiple values if you use brackets. 
//     (ie. boolname = (one, two,three) )
//
//  -  A token can have children using the curly brackets.
//     (ie boolname { sub1=a sub2=b }
//
//  -  The end of a token assignment section can be delimited by a semicolon.
//     (ie boolname { sub1=a;sub2=b; }
//
//  Example syntax:
//
//  //A comment
//  Toggle=on 
//
//  /* Another comment */
//  ToggleB = "Off / On"
//
//  Colors
//  {
//    ColA = (red,green,blue);
//    ColB = (0.5,0.6,0.7);
//  }
//
///////////////////////////////////////////////////////////////////////////////

//@
//  Summary:
//    A token container used in ConfigParser
// 
class ConfigToken
{
friend class ConfigParser;
public:

  //@
  //  Summary:
  //    Constructor, inits data values
  //  
  inline ConfigToken();

  //@
  //  Summary:
  //    Constructor that sets the name of this token.
  //  
  //  Parameters:
  //    newName - The new name for the token.
  //
  inline ConfigToken(const string & newName);

  //@
  //  Summary:
  //    To get the name of this token.
  //  
  //  Returns:
  //    The name of this token is returned.
  //
  inline const string & GetName() const;

  //@
  //  Summary:
  //    To get the number of values contained within the token.
  //  
  //  Returns:
  //    The number of values assigned to this token is returned.
  //
  inline uint GetNumValues() const;

  //@
  //  Summary:
  //    To get the number of children of this token.
  //  
  //  Returns:
  //    The number of children of this token is returned.
  //
  inline uint GetNumChildren() const;

  //@
  //  Summary:
  //    To get a value from the token.
  //
  //    (Note in the case of boolean values, it is true if:
  //       -No values exits (ie token by itself)
  //       -Values are "on" "true" "+"
  //     It is false if the valuse are "off", "false" or "-". Any other values 
  //     will cause no value to be set. (ie function returns false) 
  //  
  //  Parameters:
  //    retValue - The variable to return the retrieved value from. 
  //               If a value cannot be retrieved, the variable is not changed.
  //
  //    valueOffset - To get a value other than the first value from the token.
  //
  //  Returns:
  //    If a value can be retrieved, true is returned. Else false is returned.
  //
  bool Get(bool &retValue,uint valueOffset =0) const; 
  bool Get(int &retValue,uint valueOffset =0) const; 
  bool Get(uint &retValue,uint valueOffset =0) const; 
  bool Get(float &retValue,uint valueOffset =0) const; 
  bool Get(string &retValue,uint valueOffset =0) const; 

  //@
  //  Summary:
  //    To get an array of values from the token. (see the "Get" function)
  //  
  //  Parameters:
  //    count    - The number of values to retrieve.   
  //
  //    retValue - The variable to return the retrieved values from. 
  //               If a "count" values cannot be retrieved, the variable 
  //               array is not changed. The array is emptied before values are set.
  //               (ie existing data in the array is not kept if valid values are found)
  //
  //  Returns:
  //    If a "count" values can be retrieved, true is returned. Else false is returned.
  //
  bool GetArray(uint count,vector<bool> &retValue) const; 
  bool GetArray(uint count,vector<int> &retValue) const; 
  bool GetArray(uint count,vector<uint> &retValue) const; 
  bool GetArray(uint count,vector<float> &retValue) const; 
  bool GetArray(uint count,vector<string> &retValue) const; 

  //@
  //  Summary:
  //    To get a child token from this token.
  //  
  //  Parameters:
  //    name     - The name of the child token to get.
  //
  //    childNum - The index of the child token to get
  //
  //  Returns:
  //    If a child token of "name or "value" can be retrieved, a pointer to it 
  //    is returned. Else NULL is returned.
  //
  const ConfigToken * GetChildToken(const string & name) const;
  const ConfigToken * GetChildToken(uint childNum) const;
  ConfigToken * GetChildToken(const string & name);
  ConfigToken * GetChildToken(uint childNum);

  //@
  //  Summary:
  //    To compare this token with the passed token. All values and
  //    children are compared.
  //  
  //  Parameters:
  //    compareToken - The token to compare against this token.
  //
  //  Returns:
  //    If the tokens equal, true is returned. Else false is returned.
  //
  bool Compare(const ConfigToken *compareToken) const;

  // DT_TODO: Check the name of the token for uniqueness?
  //@
  //  Summary:
  //    To add a new child token of this token
  //  
  //  Parameters:
  //    newChild - The new token to add.
  //
  void AddChild(const ConfigToken &newChild);

  //@
  //  Summary:
  //    To remove the child with the specified name.
  //  
  //  Parameters:
  //    childString - The child string to remove.
  //
  //  Returns:
  //    Returns true if the child with the specified name is removed.
  //
  bool RemoveChild(const string &childString);

  //@
  //  Summary:
  //    To reset the token. (delete all children etc)
  //  
  inline void Reset();

  //@
  //  Summary:
  //    To set the values used in the token.
  //  
  //  Parameters:
  //    newValueArray - The new array of values.
  //
  inline void SetValueArray(const vector<string> &newValueArray);

protected:

  string name;                                    // Name of the token
  vector<string> values;                          // Values contained within the token

  vector<ConfigToken> children;                   // Children of the token 

  mutable bool  tokenAccessed;                    // Flag to indicate if the token has been accessed.
                                                  // (used to report un-used tokens)

  //@
  //  Summary:
  //    To determine if a passed string could be a hexadecimal based number. 
  //    (only starting characters checked)
  //  
  //  Parameters:
  //    strNum     - The string to check.
  //
  //  Returns:
  //    Returns true if the starting characters of the string indicate a
  //    hexadecimal based number.
  //
  bool CheckHexString(const string &strNum) const;
};


//@
//  Summary:
//    A very crude configuration parser. (Minimal error checking)
// 
class ConfigParser  
{
public:
	ConfigParser();
	virtual ~ConfigParser();

  //@
  //  Summary:
  //    To parse the passed filename and extract all the config data. 
  //  
  //  Parameters:
  //    fileName - The filename to open.
  //
  //  Returns:
  //    If the file was parsed successfully, 
  //    true is returned. Else false is returned.
  //
  bool Parse(const string &fileName);

  //@
  //  Summary:
  //    To parse the passed conmfig string and extract all the config data.
  //    (String should be in the same format as if it was comming from a file)
  //  
  //  Parameters:
  //    configString - The config string.
  //
  //  Returns:
  //    If the string was parsed successfully, 
  //    true is returned. Else false is returned.
  //
  bool ParseString(const string &configString);

  //@
  //  Summary:
  //    To get a token of the passed name.
  //  
  //  Parameters:
  //    name - The name of the token to get.
  //
  //  Returns:
  //    If a token of the passed name exists, the token is returned. 
  //    Else NULL is returned.
  //
  const ConfigToken * GetToken(const string & name) const;

  //@
  //  Summary:
  //    To get access to the root parse token.
  //  
  //  Returns:
  //    The root parse token is returned
  //
  inline const ConfigToken * GetRootToken() const;

  //@
  //  Summary:
  //    To report to the error logger all the tokens that have not been
  //    accessed. This can be called after the program has retrieved its'
  //    data to indicate to the user all unused tokens.
  //  
  //  Parameters:
  //    startToken - The token to start logging at. If NULL, the root token is
  //                 used.
  //
  //    tokenString - The string to use at the start of each log call.
  //
  void LogUnusedTokens(const ConfigToken * startToken = NULL, string tokenString=string("")) const;

  //@
  //  Summary:
  //    To genarate a string config data based on the input token. 
  //    (performs the reverse of parse)
  //  
  //  Parameters:
  //    startToken - The token to start generating the string config data at. 
  //                 All values and childern will be added to the string
  //
  //    retString  - The string to store the generated config data.
  //
  //    spaceOffset - The initial spacing offset. (children are 2 spaces indented)
  //
  //  Returns:
  //    If all tokens parsed and the string is generated successfully, 
  //    true is returned. Else false is returned.
  //
  static bool GenerateConfigString(const ConfigToken * startToken, string &retString, uint spaceOffset=0);

protected:

  bool   inComment;                               // Flag to indicate if the current lines we are parsing are in comment

  uint   lineCounter;                             // The current line number
  string fileName;                                // The current file

  ConfigToken rootToken;                          // The root token

  //Structure containing raw string data extracted
  struct RawToken
  {

    //@
    //  Summary:
    //    Constructor to init values.
    //  
    RawToken(const string &newValue,bool isString);

    string value;                                 // The value of the raw token
    bool   isString;                              // Flag indicating if the token is a literal string value or not 
                                                  // (ie. do not process as a command token)
  };
  typedef vector<RawToken> RawTokenArray; 


  //@
  //  Summary:
  //    To parse a single line
  //  
  //  Parameters:
  //    line - The line to parse and extract data from.
  //
  //    rawTokens - The list of raw tokens to to add tokens to.
  //
  //  Returns:
  //    Returns true if the line is valid, false if not.
  //
  bool ParseLine(string line, RawTokenArray &rawTokens);

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
  //    To extract a token from the passed line
  //  
  //  Parameters:
  //    line - The line to parse and extract a token from.
  //
  //    retString    - The raw token extracted (if any).
  //
  //    quotedToken  - Flag to indicate if the token comes frome a quoted
  //                   string.
  //
  //  Returns:
  //    Returns a token extracted from the "line" in "retString" (may be empty)
  //    If an error occurs, false is returned. Else true is returned
  //
  bool GetRawToken(string &line, string &retString, bool &quotedToken); 

  //@
  //  Summary:
  //    To extract the config tokens from an array of raw string tokens.
  //  
  //  Parameters:
  //    rawTokens - The raw array to extract tokens from.
  //
  //    tokenStart - The raw number token to start processing from.
  //
  //    parent    - The current parent token.
  //
  //  Returns:
  //    Returns true if all tokens were processed OK, false if not.
  //
  bool ExtractTokens(RawTokenArray &rawTokens,uint &tokenStart, ConfigToken *parent=NULL);

  //@
  //  Summary:
  //    To check if the passed string is a valid token value and add quotes
  //    if necessary. (for saving tokens to a string).
  //  
  //  Parameters:
  //    value - The value to check and quote if necessary.
  //
  //  Returns:
  //    Returns true if the token value was processed OK, false if not.
  //
  static bool AddValueQuotes(string &value);
  
  //@
  //  Summary:
  //    To add the specified number of spaces to the string
  //  
  //  Parameters:
  //    spaceStr - The string to add spaces to.
  //
  //    count    - The number of spaces to add.
  //
  static void AddSpaces(string &spaceStr, uint count);

};

///////////////////////////////////////////////////////////////////////////////
//
inline ConfigToken::ConfigToken():
  tokenAccessed(false)
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline ConfigToken::ConfigToken(const string & newName):
  name(newName),
  tokenAccessed(false)
{
}

///////////////////////////////////////////////////////////////////////////////
//
inline const string & ConfigToken::GetName() const
{
  return name;
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint ConfigToken::GetNumValues() const
{
  return (uint)values.size();
}

///////////////////////////////////////////////////////////////////////////////
//
inline uint ConfigToken::GetNumChildren() const
{
  return (uint)children.size();
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ConfigToken::Reset()
{
  //Delete all children and values
  name = "";
  values.clear();
  children.clear();

  //Flag the token as not accessed
  tokenAccessed = false;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void ConfigToken::SetValueArray(const vector<string> &newValueArray)
{
  // Copy over the new values
  values = newValueArray;
}

///////////////////////////////////////////////////////////////////////////////
//
inline const ConfigToken * ConfigParser::GetRootToken() const
{
  return &rootToken;
}

#endif 
