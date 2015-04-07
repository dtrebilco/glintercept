#include <OSDefines.h>
#include "ConfigParser.h"
#include <stdio.h>
#include <stdlib.h>

#include <CommonErrorLog.h>

USING_ERRORLOG

//Global parsing data
const string ignoreChars(" \r\n\t");
const string singleTokens("={}(),;");

///////////////////////////////////////////////////////////////////////////////
//
ConfigParser::RawToken::RawToken(const string &newValue,bool stringFlag):
value(newValue),
isString(stringFlag)
{
}

///////////////////////////////////////////////////////////////////////////////
//
ConfigParser::ConfigParser():
inComment(false),
lineCounter(0),
fileName("")
{

}

///////////////////////////////////////////////////////////////////////////////
//
ConfigParser::~ConfigParser()
{

}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigParser::Parse(const string &fName)
{
  //Init the variables
  inComment = false;

  //Assign file and line counters 
  lineCounter=1;
  fileName = fName;

  //Open the file
  FILE * file = fopen(fileName.c_str(),"rt");
  if(!file)
  {
    LOGERR(("ConfigParser::Parse - Unable to open file %s",fileName.c_str()));
    return false;
  }

  char inputString[1024];
  RawTokenArray rawTokens;

  //Get a line from the input stream
  while(fgets(inputString,1024,file) != NULL)
  {
    //Parse the line - return false on an error
    if(!ParseLine(inputString,rawTokens))
    {
      LOGERR(("ConfigParser::Parse - Line error in file %s Line %u",fileName.c_str(),lineCounter));
      fclose(file);
      return false;
    }

    //Increment the line counter
    lineCounter++;
  }

  //Close the file
  fclose(file);

  //Check that all the section areas are completed
  if(inComment)
  {
    LOGERR(("ConfigParser::Parse - Comment section not completed by end of file %s",fileName.c_str()));
    return false;
  }

  //Pass the raw tokens to a syntax checker and extract "real" tokens
  uint tokenStart =0;
  bool retValue = ExtractTokens(rawTokens,tokenStart,NULL);

  /* Debug
  if(retValue)
  {
    string testString;
    for(uint i=0;i<rootToken.GetNumChildren()-1;i++)
    {
      string tokenStr;
      GenerateConfigString(rootToken.GetChildToken(i),tokenStr);
      testString += tokenStr;
    }

    ConfigParser testParser;
    testParser.ParseString(testString);
    if(!rootToken.Compare(&testParser.rootToken))
    {
      LOGERR(("Compare does not match!"));
    }
    //LOGERR(("%s",testString.c_str()));
  }//*/
  return retValue;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigParser::ParseString(const string &configString)
{
  //Init the variables
  inComment = false;

  //Assign file and line counters 
  lineCounter=1;
  fileName = "<Supplied String>";
  string::size_type stringStart =0;
  string::size_type stringEnd   =0;

  RawTokenArray rawTokens;
  string inputString;

  //Get a line from the input stream
  do
  {
    //Get the next line of input
    stringEnd = configString.find("\n",stringStart);
    if(stringEnd != string::npos)
    {
      //Get the sub string
      inputString.assign(configString,stringStart,stringEnd - stringStart);
      stringStart = stringEnd +1;
    }
    else
    {
      //Take the remainder of the string
      inputString.assign(configString,stringStart, configString.length() - stringStart);
      stringStart = configString.length();
    }

    //Parse the line - return false on an error
    if(!ParseLine(inputString,rawTokens))
    {
      LOGERR(("ConfigParser::ParseString - Line error in string line %u",lineCounter));
      return false;
    }

    //Increment the line counter
    lineCounter++;

  }while(stringEnd != string::npos);

  //Check that all the section areas are completed
  if(inComment)
  {
    LOGERR(("ConfigParser::ParseString - Comment section not completed by end of string"));
    return false;
  }

  //Pass the raw tokens to a syntax checker and extract "real" tokens
  uint tokenStart =0;
  return ExtractTokens(rawTokens,tokenStart,NULL);
}


///////////////////////////////////////////////////////////////////////////////
//
bool ConfigParser::ParseLine(string line, RawTokenArray &rawTokens)
{
  //Remove comments
  RemoveComments(line);

  //Loop while there is still tokens to be parsed
  do
  {
    string rawToken;
    bool   isQuoteToken =false;

    //Get a token
    if(!GetRawToken(line,rawToken,isQuoteToken))
    {
      return false;
    }

    //Add if a valid quoted token or a regular token of some length
    if(rawToken.length() > 0 || isQuoteToken)
    {
      //Add the raw token to the array
      rawTokens.push_back(RawToken(rawToken,isQuoteToken));   
    }
  } while (line.length() > 0);

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
void ConfigParser::RemoveComments(string & line)
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

    //Erase the comment
    line.erase(commentPos,(commentPos2+2)-commentPos);   

    //Get the next comment position
    commentPos = line.find("/*");
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigParser::GetRawToken(string &line, string &retString, bool &quotedToken)
{
  //Reset flag
  quotedToken = false;

  //Skip any leading ignore characters
  string::size_type charOffset = line.find_first_not_of(ignoreChars.c_str());
  if(charOffset != string::npos)
  {
    line.erase(0,charOffset);
  }
  else
  {
    //If there are no characters, null out the line and return
    line = "";
  }
  
  //Return now if there exist no more valid characters
  if(line.length() == 0)
  {
    retString = "";
    return true;
  }

  //If the first character is a " search for a quoted string
  if(line[0] == '"')
  {
    string::size_type endQuote = line.find("\"",1);
    if(endQuote != string::npos)
    {
      //Extract the quote
      retString = line.substr(1,endQuote-1);

      //Erase from the line
      line.erase(0,endQuote+1);

      //Indicate that the value came from a quoted string
      quotedToken = true;
      return true;
    }
    else
    {
      LOGERR(("ConfigParser::GetRawToken - Cannot have quote in token value/multi line quote"));
      retString = "";
      return false;
    }
  }

  //Assign a test string for the first character
  string testString;
  testString.assign(line,0,1);

  //If the first character is a single token, extract and return it
  if(testString.find_first_of(singleTokens.c_str()) != string::npos)
  {
    //Erase the token
    line.erase(0,1);
    
    //Assign the return string and return
    retString = testString;
    return true;
  }
  
  //Extract a standard token
  string stdIgnore = ignoreChars + singleTokens;
  string::size_type charEnd = line.find_first_of(stdIgnore.c_str());

  if(charEnd != string::npos)
  {
    //Extract the token
    retString = line.substr(0,charEnd);

    //Erase from the line
    line.erase(0,charEnd);
  }
  else
  {
    //If the token proceeds to the end of the line, assign the whole line as the token.
    retString = line;
    line ="";
  }

  //Ensure there are no quote values
  string::size_type startQuote = retString.find("\"");
  if(startQuote != string::npos)
  {
    LOGERR(("ConfigParser::GetRawToken - Cannot have quote (\") in token value"));
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigParser::ExtractTokens(RawTokenArray &rawTokens, uint &tokenNum, ConfigToken *parent)
{
  ConfigToken *currToken=NULL;
  bool inbracket = false;
  bool assignment= false;

  //NOTE: May have to use "proper" parser recursion if this gets much more complex

  //Loop for all tokens
  for(;tokenNum<rawTokens.size();tokenNum++)
  {
    string & tokenString = rawTokens[tokenNum].value;
    int commandChar = (int)tokenString.find_first_of(singleTokens.c_str());
  
    //If this is a command character, process it 
    if(commandChar == 0  && tokenString.length() == 1 &&
       !rawTokens[tokenNum].isString)
    {

      if(tokenString == "{")
      {
        //Check that we are not in a bracket
        if(inbracket || !currToken || assignment)
        {
          LOGERR(("ConfigParser::ExtractTokens - Un-expected '{'"));
          return false;
        }

        //Recursive call
        tokenNum++;
        if(!ExtractTokens(rawTokens,tokenNum,currToken))
        {
          LOGERR(("ConfigParser::ExtractTokens - Error in child block"));
          return false;
        }
      }
      else if(tokenString == "}")
      {
        //Check that we are not in a bracket and that there was a '{'
        if(inbracket || !parent || assignment)
        {
          LOGERR(("ConfigParser::ExtractTokens - Un-expected '}'"));
          return false;
        }

        return true;
      }
      else if(tokenString == "(")
      {
        if(inbracket || !assignment || !currToken)
        {
          LOGERR(("ConfigParser::ExtractTokens - Un-expected '('"));
          return false;
        }

        //Set bracket flag
        inbracket = true;
      }
      else if(tokenString == ")")
      {
        //Check for missing opening bracket
        if(!inbracket || !assignment || !currToken)
        {
          LOGERR(("ConfigParser::ExtractTokens - Un-expected ')'"));
          return false;
        }

        //Un-set bracket flag
        inbracket = false;
        assignment= false;
      }
      else if(tokenString == ",")
      {
        //NOTE: does not check for excess comma's (ie double or extra trailing commas)
        //Check for missing previous child and in a bracket
        if(!inbracket || !currToken || currToken->GetNumValues() == 0)
        {
          LOGERR(("ConfigParser::ExtractTokens - Un-expected ','"));
          return false;
        }
      }
      else if(tokenString == "=")
      {
        //Check for current token
        if(!currToken || assignment || inbracket)
        {
          LOGERR(("ConfigParser::ExtractTokens - Un-expected '='"));
          return false;
        }

        assignment=true;
      }
      else if(tokenString == ";")
      {
        //Check for current token
        if(!currToken || assignment || inbracket)
        {
          LOGERR(("ConfigParser::ExtractTokens - Un-expected ';'"));
          return false;
        }
      
        //The token is just a seperator, so do nothing
      }
      else
      {
        LOGERR(("ConfigParser::ExtractTokens - Unknown command '%s'",tokenString.c_str()));
        return false;
      }
    }

    //Process the non-command token
    else
    {
      //Else just create a new config token 
      if(inbracket && currToken)
      {
        //Add a new value to the config token
        currToken->values.push_back(tokenString);
      }
      else if(assignment)
      {
        //Add a new value to the config token
        currToken->values.push_back(tokenString);

        //Flag that the assignment is complete
        assignment =false;
      }
      else
      {
        //Create a new token
        ConfigToken newToken;
        newToken.name = tokenString;

        //Either add to the parent or root
        ConfigToken * addToToken = &rootToken;
        if(parent)
        {
          addToToken = parent;
        }

        //Test if the child already exists
        ConfigToken * existingToken = const_cast<ConfigToken *>(addToToken->GetChildToken(tokenString));
        if(existingToken == NULL)
        {
          //Add the token
          addToToken->children.push_back(newToken);
          currToken = &addToToken->children.back();
        }
        else
        {
          //Cannot simply ignore as there may be children or ther values associated with the token
          LOGERR(("ConfigParser::ExtractTokens - Extra %s token -using last value",tokenString.c_str()));
          
          //Reset the token
          existingToken->Reset();

          //Assign as the current token
          existingToken->name = tokenString;
          currToken = existingToken;
        }
      }
    }
  }


  //If this is not the root token, and we have processed all the data
  //  there is a missing closing bracket
  if((parent != NULL || inbracket || assignment) && tokenNum>=rawTokens.size())
  {
    //Missing closing bracket
    LOGERR(("ConfigParser::ExtractTokens - Un-expected end of tokens (end of file missing a '}' or a ')'?"));
    return false;
  }
/*
  //Debug
  for(uint i=0;i<parsedTokens.size();i++)
  {
    LOGERR(("%s",parsedTokens[i].name.c_str()));
  }
*/
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
const ConfigToken * ConfigParser::GetToken(const string & name) const
{
  return rootToken.GetChildToken(name);
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigParser::LogUnusedTokens(const ConfigToken * startToken, string tokenString) const
{
  //If the start token is NULL, use the root token
  if(startToken == NULL)
  {
    startToken = &rootToken;
    tokenString = "Token ";
  }

  //Loop for all children
  for(uint i=0;i<startToken->GetNumChildren();i++)
  {
    //Check if the token has been accessed
    if(!startToken->children[i].tokenAccessed)
    {
      LOGERR(("%s->%s not accessed in the configuration file",tokenString.c_str(),startToken->children[i].name.c_str()));
    }
    else
    {
      //Recurse and check children of the token
      LogUnusedTokens(&startToken->children[i],tokenString + "->" + startToken->children[i].name);
    }
  }

}


///////////////////////////////////////////////////////////////////////////////
//
bool ConfigParser::GenerateConfigString(const ConfigToken * startToken, string &retString, uint spaceOffset)
{
  //Check for a valid token
  if(startToken == NULL)
  {
    return false;
  }
  
  //Reset the return string
  retString = "";

  //Append the name
  AddSpaces(retString,spaceOffset);
  string tokenName = startToken->GetName();
  if(!AddValueQuotes(tokenName))
  {
    return false;
  }
  retString += tokenName;

  //Add and values
  if(startToken->GetNumValues() > 0)
  {
    retString += " = ";

    //Add starting brackets for arrays of values
    if(startToken->GetNumValues() > 1)
    {
      retString += "("; 
    }
  
    //Loop for all the values
    for(uint i2=0;i2<startToken->GetNumValues();i2++)
    {
      //Get the value
      string tokenValue;
      startToken->Get(tokenValue, i2);

      //Validate and add the value
      if(!AddValueQuotes(tokenValue))
      {
        retString = "";
        return false;
      }
      retString += tokenValue; 

      //Add a seperating comma if not the last value
      if(i2 != startToken->GetNumValues()-1)
      {
        retString += ",";
      }
    }

    if(startToken->GetNumValues() > 1)
    {
      retString += ")"; 
    }
  }

  //Add a seperating colon
  retString += ";\n";

  //Append any children
  if(startToken->GetNumChildren() > 0)
  {
    AddSpaces(retString,spaceOffset); 
    retString += "{\n";

    //Loop for the number of children of this token
    for(uint i=0;i<startToken->GetNumChildren(); i++)
    {
      //Get the child
      const ConfigToken * child = startToken->GetChildToken(i);

      string childString;
      if(!GenerateConfigString(child,childString,spaceOffset+2))
      {
        retString = "";
        return false;
      }

      //Add the child string data
      retString += childString;
    }

    AddSpaces(retString,spaceOffset); 
    retString += "}\n";
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigParser::AddValueQuotes(string &value)
{
  string::size_type commentPos1 = value.find("//");
  string::size_type commentPos2 = value.find("/*");

  //If the ending string has been found 
  if(commentPos1 != string::npos ||
     commentPos2 != string::npos)
  {
    LOGERR(("ConfigParser::AddValueQuotes - Unable to have comment tokens in values"));
    return false;
  }

  //Disallow quotes
  string::size_type quoteChar  = value.find("\"");
  if(quoteChar != string::npos)
  {
    LOGERR(("ConfigParser::AddValueQuotes - Cannot have quotes in value"));
    return false;
  }

  //Add quotes to the value if necessary
  string stdIgnore = ignoreChars + singleTokens;
  string::size_type ignoreChar = value.find_first_of(stdIgnore.c_str());
  string::size_type singleChar = value.find_first_of(singleTokens.c_str());
  if(ignoreChar != string::npos ||
     singleChar != string::npos ||
     value.length() == 0)
  {
    value = string("\"") + value + string("\"");
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigParser::AddSpaces(string &spaceStr, uint count)
{
  //Add the specified number of spaces
  for(uint i=0;i<count;i++)
  {
    spaceStr +=" ";
  }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//   ConfigToken
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::Get(bool &retValue,uint valueOffset) const
{
  //If the token just exists by itself, return true
  if(GetNumValues() == 0 && valueOffset == 0)
  {
    retValue = true;
    return true;
  }
  if(valueOffset >= GetNumValues())
  {
    LOGERR(("ConfigToken::Get - Invalid offset %u",valueOffset));
    return false;
  }

  //Get a lower case version of the value 
  string cmpStr = values[valueOffset];
  for(uint i=0;i<cmpStr.length();i++)
  {
    cmpStr[i] = tolower(cmpStr[i]);
  }

  //If the value is assigne to true
  if(cmpStr == "true" || cmpStr == "on" || cmpStr == "+")
  {
    retValue = true;
    return true;
  }

  //If the value is assigne to off
  else if(cmpStr == "false" || cmpStr == "off" || cmpStr == "-")
  {
    retValue = false;
    return true;
  }

  //In-determinite value
  return false;
}



///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::Get(int &retValue,uint valueOffset) const
{
  //Check that there is a value
  if(GetNumValues() == 0)
  {
    return false;
  }
  if(valueOffset >= GetNumValues())
  {
    LOGERR(("ConfigToken::Get - Invalid offset %u",valueOffset));
    return false;
  }

  //Get the integer version of the string
  char *endValue;
  const char * str    = values[valueOffset].c_str();
  const char * endStr = str + (sizeof(char) * values[valueOffset].length());

  //Assume a base of 16 if the sequence starts with 0x 
  // (not using default 0 value as octal values may arise with a leading 0)
  uint numBase = 10;
  if(CheckHexString(values[valueOffset]))
  {
    numBase = 16;
  }

  //Get the value from the string
  long testValue = strtol(str,&endValue,numBase);
 
  //If there was some non-numeric values in the string report and return false
  if(endValue != endStr)
  {
    LOGERR(("ConfigToken::Get - Non integer values in config string %s",str));
    return false;
  }

  //Assign the return value
  retValue = testValue;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::Get(uint &retValue,uint valueOffset) const
{
  //Check that there is a value
  if(GetNumValues() == 0)
  {
    return false;
  }
  if(valueOffset >= GetNumValues())
  {
    LOGERR(("ConfigToken::Get - Invalid offset %u",valueOffset));
    return false;
  }

  //Get the integer version of the string
  char *endValue;
  const char * str    = values[valueOffset].c_str();
  const char * endStr = str + (sizeof(char) * values[valueOffset].length());

  //Check for negative
  if(values[valueOffset].length() > 0 && str[0] == '-')
  {
    LOGERR(("ConfigToken::Get - Negative integer values in config string %s",str));
    return false;
  }

  //Assume a base of 16 if the sequence starts with 0x 
  // (not using default 0 value as octal values may arise with a leading 0)
  uint numBase = 10;
  if(CheckHexString(values[valueOffset]))
  {
    numBase = 16;
  }

  //Get the value from the string
  unsigned long testValue = strtoul(str,&endValue,numBase);
 
  //If there was some non-numeric values in the string report and return false
  if(endValue != endStr)
  {
    LOGERR(("ConfigToken::Get - Non integer values in config string %s",str));
    return false;
  }

  //Assign the return value
  retValue = testValue;

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::Get(float &retValue,uint valueOffset) const
{
  //Check that there is a value
  if(GetNumValues() == 0)
  {
    return false;
  }
  if(valueOffset >= GetNumValues())
  {
    LOGERR(("ConfigToken::Get - Invalid offset %u",valueOffset));
    return false;
  }

  //Get the float version of the string
  char *endValue;
  const char * str    = values[valueOffset].c_str();
  const char * endStr = str + (sizeof(char) * values[valueOffset].length());
  double testValue = strtod(str,&endValue);
 
  //If there was some non-numeric values in the string report and return false
  if(endValue != endStr)
  {
    LOGERR(("ConfigToken::Get - Non float values in config string %s",str));
    return false;
  }

  //Assign the return value
  retValue = (float)testValue;

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::Get(string &retValue,uint valueOffset) const
{
  if(GetNumValues() == 0)
  {
    return false;
  }
  if(valueOffset >= GetNumValues())
  {
    LOGERR(("ConfigToken::Get - Invalid offset %u",valueOffset));
    return false;
  }

  //Just return the first value
  retValue = values[valueOffset];
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::GetArray(uint count,vector<bool> &retValue) const
{
  //Check the array size
  if(count > GetNumValues())
  {
    return false;
  }

  //Loop for the count
  vector<bool> tmpArray;
  for(uint i=0;i<count;i++)
  {
    //Get a single value
    bool value;
    if(Get(value,i))
    {
      tmpArray.push_back(value); 
    }
    else
    {
      return false;
    }
  }

  //Assign the array
  retValue = tmpArray;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::GetArray(uint count,vector<int> &retValue) const
{
  //Check the array size
  if(count > GetNumValues())
  {
    return false;
  }

  //Loop for the count
  vector<int> tmpArray;
  for(uint i=0;i<count;i++)
  {
    //Get a single value
    int value;
    if(Get(value,i))
    {
      tmpArray.push_back(value); 
    }
    else
    {
      return false;
    }
  }

  //Assign the array
  retValue = tmpArray;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::GetArray(uint count,vector<uint> &retValue) const
{
  //Check the array size
  if(count > GetNumValues())
  {
    return false;
  }

  //Loop for the count
  vector<uint> tmpArray;
  for(uint i=0;i<count;i++)
  {
    //Get a single value
    uint value;
    if(Get(value,i))
    {
      tmpArray.push_back(value); 
    }
    else
    {
      return false;
    }
  }

  //Assign the array
  retValue = tmpArray;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::GetArray(uint count,vector<float> &retValue) const
{
  //Check the array size
  if(count > GetNumValues())
  {
    return false;
  }

  //Loop for the count
  vector<float> tmpArray;
  for(uint i=0;i<count;i++)
  {
    //Get a single value
    float value;
    if(Get(value,i))
    {
      tmpArray.push_back(value); 
    }
    else
    {
      return false;
    }
  }

  //Assign the array
  retValue = tmpArray;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::GetArray(uint count,vector<string> &retValue) const
{
  //Check the array size
  if(count > GetNumValues())
  {
    return false;
  }

  //Loop for the count
  vector<string> tmpArray;
  for(uint i=0;i<count;i++)
  {
    //Get a single value
    string value;
    if(Get(value,i))
    {
      tmpArray.push_back(value); 
    }
    else
    {
      return false;
    }
  }

  //Assign the array
  retValue = tmpArray;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
const ConfigToken * ConfigToken::GetChildToken(const string & name) const
{
  //Loop and check all tokens
  for(uint i=0;i<children.size();i++)
  {
    //Check if the token value exists
    if(children[i].name == name)
    {
      //Flag that the child has been accessed
      children[i].tokenAccessed = true;

      //Return the child
      return &children[i];
    }
  }

  //Return NULL for no token
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
const ConfigToken * ConfigToken::GetChildToken(uint childNum) const
{
  //Check the array size and return the child
  if(childNum < children.size())
  {
    //Flag that the child has been accessed
    children[childNum].tokenAccessed = true;

    //Return the child
    return &children[childNum];
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
ConfigToken * ConfigToken::GetChildToken(const string & name)
{
  //Loop and check all tokens
  for(uint i=0;i<children.size();i++)
  {
    //Check if the token value exists
    if(children[i].name == name)
    {
      //Flag that the child has been accessed
      children[i].tokenAccessed = true;

      //Return the child
      return &children[i];
    }
  }

  //Return NULL for no token
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
ConfigToken * ConfigToken::GetChildToken(uint childNum)
{
  //Check the array size and return the child
  if(childNum < children.size())
  {
    //Flag that the child has been accessed
    children[childNum].tokenAccessed = true;

    //Return the child
    return &children[childNum];
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::CheckHexString(const string &strNum) const
{
  //Check the string length
  if(strNum.length() >= 2)
  {
    //Ignore a + or - 
    const char * checkStr = strNum.c_str();
    if(checkStr[0] == '+' || checkStr[0] == '-')
    {
      //If there is insufficient room left for the header, return now
      if(strNum.length() == 2)
      {
        return false;
      }

      //Increment the check position
      checkStr++;
    }
    
    //Check for the "0x"
    if(checkStr[0] == '0' && 
       (checkStr[1] == 'x' || checkStr[1] == 'X'))
    {
     return true;
    }
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::Compare(const ConfigToken *compareToken) const
{
  if(!compareToken)
  {
    return false;
  }

  //Compare the name/values count/children count
  if(GetName()        != compareToken->GetName() ||
     GetNumValues()   != compareToken->GetNumValues() ||
     GetNumChildren() != compareToken->GetNumChildren())
  {
    return false;
  }

  //Compare the values
  for(uint i=0;i<GetNumValues();i++)
  {
    //Compare each array value
    if(values[i] != compareToken->values[i])
    {
      return false;
    }
  }

  //Loop and compare the children
  for(uint childNum=0; childNum<GetNumChildren(); childNum++)
  { 
    //Recurse through all the children
    // (without setting the accessed flag)
    if(!children[childNum].Compare(&compareToken->children[childNum]))
    {
      return false;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigToken::AddChild(const ConfigToken &newChild)
{
  // Add the new child
  children.push_back(newChild);
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigToken::RemoveChild(const string &childString)
{
  //Loop and search for the child name
  for(uint i=0; i<GetNumChildren(); i++)
  {
    // If found, remove it
    if(children[i].GetName() == childString)
    {
      children.erase(children.begin() + i);
      return true;
    }
  }

  return false;
}

