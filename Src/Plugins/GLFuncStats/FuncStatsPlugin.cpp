#include "FuncStatsPlugin.h"

#include <ConfigParser.h>
#include <CommonErrorLog.h>

#include <algorithm>

#define NUM_CALL_COUNT_SPACING 45

USING_ERRORLOG

//Path to the dll
extern string dllPath;

///////////////////////////////////////////////////////////////////////////////
//
FuncStatsPlugin::FunctionCallData::FunctionCallData():
funcCallCount(0)
{
}

///////////////////////////////////////////////////////////////////////////////
//
FuncStatsPlugin::FuncStatsPlugin(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),

functionsCountSort(false),
functionsNameSort(false),

numGLFunctionCalls(0),
numGLCallsFirstFrame(0)
{
  //Get callbacks for all functions
  gliCallBacks->RegisterGLFunction("*");

  //Get calls that are even outside contexts  
  gliCallBacks->SetContextFunctionCalls(false);

  //Parse the config file
  ConfigParser fileParser;
  if(fileParser.Parse(dllPath + "config.ini"))
  {
    ProcessConfigData(&fileParser);
    fileParser.LogUnusedTokens(); 
  }

  //Parse the config string
  ConfigParser stringParser;
  if(stringParser.ParseString(gliCallBacks->GetConfigString()))
  {
    ProcessConfigData(&stringParser);
    stringParser.LogUnusedTokens(); 
  }

}

///////////////////////////////////////////////////////////////////////////////
//
FuncStatsPlugin::~FuncStatsPlugin()
{

}

///////////////////////////////////////////////////////////////////////////////
//
void FuncStatsPlugin::ProcessConfigData(ConfigParser *parser)
{
  //Get if logging by call count
  const ConfigToken *testToken = parser->GetToken("LogCallCountSort");
  if(testToken)
  {
    testToken->Get(functionsCountSort);
  }

  //Get if logging by function name
  testToken = parser->GetToken("LogFunctionNameSort");
  if(testToken)
  {
    testToken->Get(functionsNameSort);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void FuncStatsPlugin::GLFunctionPre (uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args )
{
  //Loop and add array entries to ensure the index value is valid
  while(funcIndex >= functionDataArray.size())
  {
    functionDataArray.push_back(FunctionCallData());  
  }

  //If this function has not been called before
  if(functionDataArray[funcIndex].funcCallCount == 0)
  {
    //Assign the function name
    functionDataArray[funcIndex].functionName = funcName;
  }

  //Increment the call count for the function
  functionDataArray[funcIndex].funcCallCount++;
  
  //Increment the total call count
  numGLFunctionCalls++;
} 

///////////////////////////////////////////////////////////////////////////////
//
inline void FuncStatsPlugin::GLFrameEndPost(const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //If the first frame has just passed
  if(gliCallBacks->GetFrameNumber() == 1)
  {
    //Assign the number of function calls made in the first frame
    numGLCallsFirstFrame = numGLFunctionCalls;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void FuncStatsPlugin::Destroy()
{
  //Get the frame count
  uint frameNumber = gliCallBacks->GetFrameNumber();
  uint averageCalls = 0;

  //If two frames have passed, get the average excluding the first frame
  if(frameNumber > 1)
  {
    averageCalls = (uint)((double)(numGLFunctionCalls - numGLCallsFirstFrame) / (double)(frameNumber-1));
  }

  //Handle unlikely event of exceeding 2^32 number of calls (can happen if left running for hours)
  char numFuncCallsStr[100];
  
#ifdef GLI_BUILD_WINDOWS
  _i64toa(numGLFunctionCalls, numFuncCallsStr, 10);
#endif //GLI_BUILD_WINDOWS

#ifdef GLI_BUILD_LINUX
  //DT_TODO: Test this
  sprintf(numFuncCallsStr, "%lld", numGLFunctionCalls);
#endif //GLI_BUILD_LINUX

  //Dump the total call count and average per frame (excluding first frame of xxx calls)
  LOGERR(("\n======= OpenGL function call statistics =========="));
  LOGERR((" Total GL calls: %s", numFuncCallsStr));
  LOGERR((" Number of frames: %u  Average: %u calls/frame (excluding first frame count of %u)",frameNumber, averageCalls, (uint)numGLCallsFirstFrame));

  //If dumping by call number
  if(functionsCountSort)
  {
    //Sort the array based on function call count
    sort(functionDataArray.begin(), functionDataArray.end(), FunctionCallData::SortByCount);
    
    LOGERR(("\n======= OpenGL function calls by call count =========="));

    //Loop and dump the function data 
    for(uint i=0; i<functionDataArray.size(); i++)
    {
      //Only dump functions that have been called
      if(functionDataArray[i].funcCallCount > 0)
      {
        LOGERR(("%s %s %u",functionDataArray[i].functionName.c_str(), 
                           GetSpaces(functionDataArray[i].functionName).c_str(),
                           functionDataArray[i].funcCallCount)); 
      }
    }
  }

  //If dumping stats by function name
  if(functionsNameSort)
  {
    //Sort the array based on function name
    sort(functionDataArray.begin(), functionDataArray.end(), FunctionCallData::SortByName);
    
    LOGERR(("\n======= OpenGL function calls by name =========="));

    //Loop and dump the function data 
    for(uint i=0; i<functionDataArray.size(); i++)
    {
      //Only dump functions that have been called
      if(functionDataArray[i].funcCallCount > 0)
      {
        LOGERR(("%s %s %u",functionDataArray[i].functionName.c_str(), 
                           GetSpaces(functionDataArray[i].functionName).c_str(),
                           functionDataArray[i].funcCallCount)); 
      }
    }
  }

  //Destroy this plugin
  delete this;
}

///////////////////////////////////////////////////////////////////////////////
//
string FuncStatsPlugin::GetSpaces(const string &funcName)
{
  string retString;

  //IF the name is too long, return now
  if(funcName.length() >= NUM_CALL_COUNT_SPACING)
  {
    return retString;
  }

  //Calculate the required length
  uint numSpaces =  NUM_CALL_COUNT_SPACING - (uint)funcName.length();
  retString.reserve(numSpaces);

  //Add the spaces
  for(uint i=0; i<numSpaces; i++)
  {
    retString +=".";
  }

  return retString;
}

