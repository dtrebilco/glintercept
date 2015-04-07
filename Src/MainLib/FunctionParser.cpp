/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "FunctionParser.h"
#include "ExtensionFunctionWrapper.h"
#include <FileUtils.h>
#include <stdlib.h>

USING_ERRORLOG

vector<string> FunctionParser::parsedDefines;


///////////////////////////////////////////////////////////////////////////////
//
FunctionParser::FunctionParser():
inComment(false),
inFunction(false),
inEnum(false),
currEnum(NULL),
lineCounter(0)
{

}


///////////////////////////////////////////////////////////////////////////////
//
FunctionParser::~FunctionParser()
{
  if(currEnum)
  {
    delete currEnum;
  }

}

/*
///////////////////////////////////////////////////////////////////////////////
//
void FunctionParser::WriteFunctionData(const char *fileName) const
{

  //Open the file
  FILE * file = fopen(fileName,"wt");
  if(!file)
  {
    LOGERR(("FunctionParser::Parse - Unable to open file %s",fileName));
    return;
  }

  //Loop for all the current functions
  for(uint i=0; i<functionArray.size(); i++)
  {
    uint i2;

    //Determine if there is a return type
    bool hasReturnValue = true;
    if(functionArray[i].returnType.GetGLType()  == PT_void &&
       functionArray[i].returnType.pointerCount == 0       &&
       functionArray[i].returnType.length       == -1)
    {
      hasReturnValue = false; 
    }

    //Write the starting tag
    if(hasReturnValue)
    {
      fprintf(file, "  GL_FUNCTION_WRAPPER_RET(");
    }
    else
    {
      fprintf(file, "  GL_FUNCTION_WRAPPER(");
    }

    //Write the function name
    fprintf(file, "%s,(",functionArray[i].functionName.c_str());

    //Check that there is some parameters
    if(functionArray[i].parameterArray.size() == 0)
    {
      fprintf(file, "Function has no parameters");
      return;
    }

    //Loop for all arguments
    for(i2=0; i2<functionArray[i].parameterArray.size(); i2++)
    {
      const ParameterData &currParam = functionArray[i].parameterArray[i2];

      //Get the type as a string
      string typeString = ParameterData::GetTypeString(currParam.GetGLType());

      //Get the number of pointers (if any)
      string pointerStr = "";
      uint numPointers = currParam.pointerCount;
      if(currParam.length != -1)
      {
        numPointers++;
      }
      for(uint i3=0; i3<numPointers; i3++)
      {
        pointerStr += "*";
      }


      //"const" modifiers will have to be added manually
      fprintf(file, "%s %s%s",typeString.c_str(), pointerStr.c_str(), currParam.paramName.c_str());

      //Add a comma
      if(i2 != functionArray[i].parameterArray.size() -1)
      {
        fprintf(file, ", ");         
      }
    }


    fprintf(file, "), (");

    //Loop for all arguments to dump just the parameter names
    for(i2=0; i2<functionArray[i].parameterArray.size(); i2++)
    {
      const ParameterData &currParam = functionArray[i].parameterArray[i2];

      fprintf(file, "%s", currParam.paramName.c_str());

      //Add a comma
      if(i2 != functionArray[i].parameterArray.size() -1)
      {
        fprintf(file, ", ");         
      }
    }

    fprintf(file, "), ");

    //Print the starting argument
    // If there is no real first parameter, just use a "dummy"
    if(functionArray[i].parameterArray.size() == 1 &&
       functionArray[i].parameterArray[0].GetGLType()  == PT_void &&
       functionArray[i].parameterArray[0].pointerCount == 0       &&
       functionArray[i].parameterArray[0].length       == -1)
    {
      fprintf(file,"*DT_TODO*glcDummyValue");
    }
    else
    {
      fprintf(file,"%s",functionArray[i].parameterArray[0].paramName.c_str());
    }

    //Add the return type if necessary
    if(hasReturnValue)
    {
      string typeString = ParameterData::GetTypeString(functionArray[i].returnType.GetGLType());
      fprintf(file, ", %s", typeString.c_str());

      //Write the number of pointers for the type
      uint numPointers = functionArray[i].returnType.pointerCount;
      if(functionArray[i].returnType.length != -1)
      {
        numPointers++;
      }
      for(uint i3=0; i3<numPointers; i3++)
      {
        fprintf(file, "*");
      }

    }

    fprintf(file, ");\n");
  }


  //Close the file
  fclose(file);
}
//*/


///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::Parse(const string & fName)
{
  //Init the variables
  inComment =false;
  inFunction=false;
  inEnum    =false;

  //Assign file and line counters (Account for header)
  lineCounter=2;
  fileName = fName;

  //Open the file
  FILE * file = fopen(fileName.c_str(),"rt");
  if(!file)
  {
    LOGERR(("FunctionParser::Parse - Unable to open file %s",fileName.c_str()));
    return false;
  }

  //Get the initial line
  char inputString[1024];
  char defineName [1024];
  if(fgets(inputString,1024,file) != NULL)
  {
    int numScaned = sscanf(inputString,"#define %1023s",defineName);
    if(numScaned == 0 || numScaned == EOF)
    {
      LOGERR(("FunctionParser::Parse - Unable to read the define in the file %s",fileName.c_str()));
      fclose(file);
      return false;
    }
  }

  //Search for the existing string
  string defineStr = defineName;
  for(uint i=0;i<parsedDefines.size();i++)
  {
    //Check for duplicate defines
    if(parsedDefines[i] == defineStr)
    {
      LOGERR(("FunctionParser::Parse - In file  %s, define= %s already existed in another file",fileName.c_str(),defineName));
      fclose(file);
      return false;
    }
  }

  //Add the define to the array
  parsedDefines.push_back(defineStr);

  //Diagnostic
  //LOGERR(("Parsing file %s  Define= %s",fileName.c_str(),defineName));

  //Get a line from the input stream
  while(fgets(inputString,1024,file) != NULL)
  {
    //Parse the line - return false on an error
    if(!ParseLine(inputString))
    {
      LOGERR(("FunctionParser::Parse - Line error in file %s Line %u",fileName.c_str(),lineCounter));
      fclose(file);
      return false;
    }

    //Increment the line counter
    lineCounter++;
  }

  //Close the file
  fclose(file);

  //If still in a function or enum, return false
  if(inEnum || inFunction)
  {
    LOGERR(("FunctionParser::Parse - Still in function/enum at end of file %s ",fileName.c_str()));
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::GetFunctionData(FunctionDataArray & funcArray,EnumDataArray & enumArray)
{
  uint i,i2;

  //Add all our function elements to the passed array
  for(i=0;i<functionArray.size();i++)
  {
    //Check that this function name does not already exist
    for(i2=0;i2<funcArray.size();i2++)
    {
      if(funcArray[i2].GetName() == functionArray[i].GetName())
      {
        LOGERR(("FunctionParser::GetFunctionData - Function %s already exists",funcArray[i2].GetName().c_str()));
        return false;
      }
    }

    funcArray.push_back(functionArray[i]);
  }

  bool enumMerged = false;

  //Add all our enum elements to the passed array
  for(i=0;i<enumDataArray.size();i++)
  {
    //Re-set flag
    enumMerged = false;

    //Check that this enum name does not already exist
    for(i2=0;i2<enumArray.size();i2++)
    {
      //If the enum already exists
      if(enumArray[i2].GetName() == enumDataArray[i].GetName())
      {
        //Attempt to merge the enums
        if(!enumArray[i2].Merge(enumDataArray[i]))
        {
          LOGERR(("FunctionParser::GetFunctionData - Enum %s already exists and could not be merged",enumArray[i2].GetName().c_str()));
          return false;
        }
        else
        {
          enumMerged = true;
        }
      }
    }

    //Add the enum to the array if it was not merged
    if(!enumMerged)
    {
      enumArray.push_back(enumDataArray[i]);
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::ParseLine(string line)
{
  //Remove comments
  RemoveComments(line);

  //Check for an include file
  if(IsIncludeFile(line))
  {
    return ParseIncludeFile(line);
  }

  //Remove the excess white spaces
  RemoveWhiteSpace(line);

  //Abort now for an empty string
  if(line == "" || line == " ")
  {
    return true;
  }

  //Check the string for invalid characters
  if(!CheckString(line))
  {
    return false;
  }

  //Check for a enum
  if(!inFunction && IsEnum(line))
  {
    return ParseEnum(line);
  }

  //Check for a function
  if(IsFunction(line))
  {
    return ParseFunction(line);
  }


  LOGERR(("Unexpected data in file %s line %u. Type %s",fileName.c_str(),lineCounter,line.c_str()));

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
void FunctionParser::RemoveComments(string & line)
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
void FunctionParser::RemoveWhiteSpace(string & line)
{
  // Replace all tabs
  string::size_type tabPos = line.find("\t");
  while(tabPos != string::npos)
  {
    line[tabPos] = ' ';
    tabPos = line.find("\t");
  }

  //Erase all leading white space
  while(line.size() > 0 &&
        line[0] == ' ')
  {
    //Erase one of the spaces
    line.erase(0,1);
  }

  //Find duplicate white spaces
  string::size_type whitePos = line.find("  ");
  while(whitePos != string::npos)
  {
    //Erase one of the spaces
    line.erase(whitePos,1);

    //Get the next comment position
    whitePos = line.find("  ");
  }

  //Remove the return statement
  whitePos = line.find("\r");
  if(whitePos != string::npos)
  {
    line.erase(whitePos,1);
  }

  //Remove the newline statement
  whitePos = line.find("\n");
  if(whitePos != string::npos)
  {
    line.erase(whitePos,1);
  }

  //Erase all trailing white space
  while(line.size() > 0 &&
        line[line.size()-1] == ' ')
  {
    //Erase one of the spaces
    line.erase(line.size()-1,1);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::CheckString(const string & line)
{

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::IsIncludeFile(const string & line) const
{
  //Attempt to find an include directive
  string::size_type includePos = line.find("#include");
  if(includePos == string::npos)
  {
    return false;
  }

  //If we have a new include
  for(uint i=0;i<includePos;i++)
  {
    //Only allow white space
    if(line[i] != ' ')
    {
      LOGERR(("FunctionParser::GetIncludeFile - Extra data befor include directive. Line %u File %s",lineCounter,fileName.c_str()));
      return false;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::IsEnum(const string & line) const
{
  //If already in an enum, assume enum data
  if(inEnum)
  {
    return true;
  }

  //Check for the start of an enum
  char buffer[1000+1];
  if(sscanf(line.c_str(),"enum %1000s {",&buffer[0]) == 1)
  {
    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::IsFunction(const string & line) const
{
  //If already in a function, assume function data data
  if(inFunction)
  {
    return true;
  }

  //Just do a crude check for the braces
  string::size_type openBrace = line.find("(");
  string::size_type closeBrace = line.find(");");

  if(openBrace != string::npos && closeBrace != string::npos)
  {
    return true;
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::ParseIncludeFile(const string & line)
{
  //Get the file name
  string::size_type fileStart = line.find("\"",0);
  if(fileStart == string::npos)
  {
    return false;
  }
  string::size_type fileEnd   = line.find("\"",fileStart+1);
  if(fileEnd == string::npos)
  {
    return false;
  }

  string newFileName=line;

  //Erase and the end bit then the front bit
  newFileName.erase(fileEnd,newFileName.size()-fileEnd);
  newFileName.erase(0,fileStart + 1);

  //Convert \ or /  to the directory seperators
  for(uint i=0; i<newFileName.length(); i++)
  {
    //If equal to any directory seperator, assign the correct one
    if(newFileName[i] == '\\' || newFileName[i] == '/')
    {
      newFileName[i] = FileUtils::dirSeparator;
    }
  }

  //Add the existing path to the file name
  string::size_type pathEnd = fileName.rfind(FileUtils::dirSeparator);
  if(pathEnd != string::npos)
  {
    //Get the path
    string filePath = fileName;
    filePath.erase(pathEnd+1,filePath.size()-pathEnd-1);

    //Append the path
    newFileName = filePath + newFileName;
  }

  //Create a new parser
  FunctionParser newParser;

  //Parse the file
  if(!newParser.Parse(newFileName))
  {
    return false;
  }

  //If parsed successfully, get the function data
  if(!newParser.GetFunctionData(functionArray,enumDataArray))
  {
    LOGERR(("FunctionParser::Duplicate data in file - %s",newFileName.c_str()));
    return false;
  }

  return true;
}



///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::ParseEnum(const string & line)
{
  char buffer[1000+1];

  //Make a copy of the line
  string enumLine = line;

  //Check to see if we are currently processing an enum 
  if(!inEnum)
  {
    //Get the enum name
    if(sscanf(enumLine.c_str(),"enum %1000s {",&buffer[0]) != 1)
    {
      return false;
    }

    //Assign the name (remove bracket if necessary)
    currEnumName = buffer;
    string::size_type removeBracket =currEnumName.find("{");
    if(removeBracket != string::npos)
    {
      currEnumName.erase(removeBracket,1);
    }

    //Erase the enum flag
    int enumStart = (int)enumLine.find("{");
    enumLine.erase(0,enumStart+1);

    bool maskedEnum = false;
    //Check for a mask enum
    if(currEnumName.find("Mask_") == 0)
    {
      maskedEnum = true;
    }

    //Safety check
    if(currEnum != NULL)
    {
      LOGERR(("FunctionParser::ParseEnum - Starting a new enum in the middle of a current one? New enum %s",currEnumName.c_str()));
      return false;
    }

    //Get or create the new enum structure
    currEnum = new EnumData(currEnumName,maskedEnum);

    //Flag that we are now in an enum
    inEnum = true;
  }

  //Safety check
  if(currEnum == NULL)
  {
    LOGERR(("FunctionParser::ParseEnum - No current enum?"));
    return false;
  }

  //Loop and get a enum
  int enumValue = -1;
  while (sscanf(enumLine.c_str()," %1000s = 0x%x,",&buffer[0],&enumValue) == 2)
  {
    //Create the new value structure
    EnumValue newValue;
    newValue.name  = buffer;
    newValue.value = enumValue;

    //Add it
    currEnum->AddEnumValue(newValue);

    //Skip the read enum
    int skipIndex = (int)enumLine.find(",");
    if(skipIndex >= 0)
    {
      enumLine.erase(0,skipIndex+1);
      RemoveWhiteSpace(enumLine);
    }
    else
    {
      LOGERR(("FunctionParser::ParseEnum - Invalid line %s", enumLine.c_str()));
      return false;
    }
  }

  //Check if the enum finishes
  if(enumLine == "};")
  {
    //Re-set flag
    bool enumMerged = false;

    //Check that this enum name does not already exist
    for(uint i=0;i<enumDataArray.size();i++)
    {
      //If the enum already exists
      if(enumDataArray[i].GetName() == currEnum->GetName())
      {
        //Attempt to merge the enums
        if(!enumDataArray[i].Merge(*currEnum))
        {
          LOGERR(("FunctionParser::ParseEnum - Enum %s already exists and could not be merged",enumDataArray[i].GetName().c_str()));
          return false;
        }
        else
        {
          enumMerged = true;
        }
      }
    }

    //Add the enum to the array if it was not merged
    if(!enumMerged)
    {
      enumDataArray.push_back(*currEnum);
    }
    
    //Release and NULL out the enum data
    delete currEnum;
    currEnum = NULL;

    //Flag that we are now no longer in an enum
    inEnum = false;
  }
  else
  {
    // Check for extra data...
    if(enumLine != "")
    {
      LOGERR(("FunctionParser::ParseEnum - Unexpected data \"%s\"", enumLine.c_str()));
      return false;
    }
  }

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::ParseFunction(const string & line)
{
  //Flag that we are now in a function 
  inFunction = true;

  //Get the parameter part and the function name/ return type
  string parameterStr = line;
  string functionStr  = line;

  //Just do a crude check for the braces
  string::size_type openBrace = line.find("(");
  string::size_type closeBrace = line.find(");");

  if(openBrace == string::npos || closeBrace == string::npos)
  {
    return false;
  }

  //Strip all data from the parameters
  parameterStr.erase(closeBrace,parameterStr.size()-closeBrace);
  parameterStr.erase(0,openBrace+1);

  //Strip all data from the function name/return type
  functionStr.erase(openBrace,functionStr.size()-openBrace);


  TokenArray functionTokens;
  TokenArray parameterTokens;

  //Get all the function tokens
  if(!GetAllFunctionTokens(functionStr,functionTokens))
  {
    LOGERR(("FunctionParser::ParseFunction - Unable to get function tokens File %s Line %u",fileName.c_str(),lineCounter));
    return false;
  }

  //Get all the parameter tokens 
  if(parameterStr != "" && parameterStr!= " " && !GetAllFunctionTokens(parameterStr,parameterTokens))
  {
    LOGERR(("FunctionParser::ParseFunction - Unable to get parameter tokens File %s Line %u",fileName.c_str(),lineCounter));
    return false;
  }

  //Get the return type
  ParameterData retType;
  if(functionTokens.size() == 0 || !retType.AssignType(functionTokens[0].name))
  {
    LOGERR(("FunctionParser::ParseFunction - Could not determine return type. File %s Line %u",fileName.c_str(),lineCounter));
    return false;
  }
  functionTokens.erase(functionTokens.begin());

  //Check for a enum name
  if((retType.type == PT_enum || retType.type == PT_bitfield) && functionTokens.size() > 0 && functionTokens[0].optional == true)
  {
    //Assign the enum name
    retType.typeName = functionTokens[0].name;

    //Pop off the value
    functionTokens.erase(functionTokens.begin());
  }

  //Check for a pointer
  while(functionTokens.size() > 0 && functionTokens[0].name == "*")
  {
    //Assign as a pointer
    retType.pointerCount++;

    //Pop off the value
    functionTokens.erase(functionTokens.begin());
  }

  //Get the function name (should only be 1 value left)
  if(functionTokens.size() != 1)
  {
    LOGERR(("FunctionParser::ParseFunction - Extra/Insufficient function name data? File %s Line %u",fileName.c_str(),lineCounter));
    return false;
  }

  FunctionData newFunction(functionTokens[0].name);

  //Assign the return type
  newFunction.returnType = retType;

  // Check for return types that will require the wrapper ASM to be updated
  if( (retType.type == PT_float) ||
      (retType.type == PT_double)
#ifdef OS_ARCH_x86
      || (retType.type == PT_int64)
      || (retType.type == PT_uint64)
#endif // OS_ARCH_x86
    )
  {
    // Manually wrapped functions can have exotic return types
    if(!ExtensionFunctionWrapper::IsManualWrapFunction(newFunction.GetName()))
    {
      LOGERR(("FunctionParser::ParseFunction - Return type (%s) is unsupported in GLIntercept - update GLIntercept wrapper ASM to support floats/doubles/long types. File %s Line %u",retType.GetTypeString(retType.type).c_str(),fileName.c_str(),lineCounter));
      return false;
    }
  }

  //Get the parameters
  while(parameterTokens.size() > 0)
  {
    //Get the parameter type
    ParameterData newParam;
    if(!newParam.AssignType(parameterTokens[0].name))
    {
      LOGERR(("FunctionParser::ParseFunction - Could not determine parameter type. File %s Line %u",fileName.c_str(),lineCounter));
      return false;
    }
    parameterTokens.erase(parameterTokens.begin());

    //Check for a enum name
    if((newParam.type == PT_enum || newParam.type == PT_bitfield) && parameterTokens.size() > 0 && parameterTokens[0].optional == true)
    {
      //Assign the enum name
      newParam.typeName = parameterTokens[0].name;

      //Pop off the value
      parameterTokens.erase(parameterTokens.begin());
    }

    //Check for a pointer
    while(parameterTokens.size() > 0 && parameterTokens[0].name == "*")
    {
      //Assign as a pointer
      newParam.pointerCount++;

      //Pop off the value
      parameterTokens.erase(parameterTokens.begin());
    }

    //Get the name (if not void)
    if(newParam.type != PT_void || newParam.pointerCount > 0)
    {

      if(parameterTokens.size() == 0)
      {
        LOGERR(("FunctionParser::ParseFunction - No parameter name? File %s Line %u",fileName.c_str(),lineCounter));
        return false;
      }

      //Assign the name
      newParam.paramName = parameterTokens[0].name;

      //Pop off the value
      parameterTokens.erase(parameterTokens.begin());
    }

    //Get an array size
    if(parameterTokens.size() > 0 && parameterTokens[0].optional == true)
    {
      //Assign the enum name
      newParam.length = atoi(parameterTokens[0].name.c_str());

      //Check for zero length
      if(newParam.length == 0)
      {
        LOGERR(("FunctionParser::ParseFunction - Zero parameter array length? (or invalid) File %s Line %u",fileName.c_str(),lineCounter));
        return false;
      }

      //Pop off the value
      parameterTokens.erase(parameterTokens.begin());
    }

    //Check for a comma
    if(parameterTokens.size() > 0)
    {
      if(parameterTokens[0].name != ",")
      {
        LOGERR(("FunctionParser::ParseFunction - Expecting parameter comma. File %s Line %u",fileName.c_str(),lineCounter));
        return false;
      }
      //Pop off the value
      parameterTokens.erase(parameterTokens.begin());
    }

    //Add the parameter to the function array
    newFunction.parameterArray.push_back(newParam);
  }

  //Check for parameters
  if(newFunction.parameterArray.size() == 0)
  {
    LOGERR(("FunctionParser::ParseFunction - No function parameters? (Not even void?). File %s Line %u",fileName.c_str(),lineCounter));
    return false;
  }

  //Check if the function already exists
  for(uint i=0;i<functionArray.size();i++)
  {
    if(functionArray[i].GetName() == newFunction.GetName())
    {
      LOGERR(("FunctionParser::ParseFunction - Function %s already exists. File %s Line %u",functionArray[i].GetName().c_str(),fileName.c_str(),lineCounter));
      return false;
    }
  }

  //Add the function to the array
  functionArray.push_back(newFunction);

  //Flag that we are now no longer in a function 
  inFunction = false;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::GetFunctionToken(string &line,string &token,bool &optional)
{
  //Flag to indicate if we are in a bracket
  bool inBracket = false;

  //Assign the return flag
  optional = false;

  //Assing the token a empty string
  token = "";

  //Read all the standard characters until a '*' '[' ']' or ','
  while(line.size() > 0)
  {
    char currChar = line[0];

    //If the current character is a space, strip it and break
    if(currChar == ' ')
    {
      line.erase(0,1);

      //Break if there is a existing token
      if(token != "")
      {
        break;
      }
    }


    //Check for a open bracket
    else if(currChar == '[')
    {
      //Break if there is a existing token
      if(token != "")
      {
        break;
      }

      //If we are already in a bracket, Return a parse error
      if(inBracket)
      {
        LOGERR(("FunctionParser::GetFunctionToken - '[' encountered without a ']' File %s Line %u",fileName.c_str(),lineCounter));
        return false;
      }

      //Assign the flags
      optional  = true;
      inBracket = true;

      //Erase the character
      line.erase(0,1);
    }


    //Check for a close bracket
    else if(currChar == ']')
    {
      //If we are not already in a bracket, Return a parse error
      if(!inBracket)
      {
        LOGERR(("FunctionParser::GetFunctionToken - ']' encountered without a '[' File %s Line %u",fileName.c_str(),lineCounter));
        return false;
      }

      //Assign the flags
      inBracket = false;

      //Erase the character and break
      line.erase(0,1);
      break;
    }

    //Check for comma or star
    else if(currChar == ',' || currChar == '*')
    {
      //If the token line is empty, assign this as the token
      if(token == "")
      {
        token = currChar;
        line.erase(0,1);
        break;
      }
      else
      {
        //Else simply flag the end of the token
        break;
      }
    }

    //If a valid character or number, add it
    else if((currChar >= 'a' && currChar <= 'z') || 
            (currChar >= 'A' && currChar <= 'Z') || 
            (currChar >= '0' && currChar <= '9') ||
            (currChar == '_'))
    {
      //Add the character
      token = token + currChar;

      //Erase it from the string
      line.erase(0,1);
    }
    else
    {
      LOGERR(("FunctionParser::GetFunctionToken - Invalid character %c File %s Line %u",currChar,fileName.c_str(),lineCounter));
      return false;
    }
  }

  //If we are still in a bracket, Return a parse error
  if(inBracket)
  {
    LOGERR(("FunctionParser::GetFunctionToken - '[' encountered without a ']' File %s Line %u",fileName.c_str(),lineCounter));
    return false;
  }

  //Return false when no token was found
  if(token == "")
  {
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool FunctionParser::GetAllFunctionTokens(string &line,TokenArray & array)
{
  string token;
  bool   optional;

  //Get the function data
  while(GetFunctionToken(line,token,optional))
  {
    //If the token was a const, continue
    if(token == "const")
    {
      continue;
    }

    //Create the new token
    ParseToken newToken;
    newToken.name = token;
    newToken.optional = optional;

    //Add the token
    array.push_back(newToken);
  }


  //Return false on no tokens
  if(array.size() == 0)
  {
    return false;
  }

  return true;
}
