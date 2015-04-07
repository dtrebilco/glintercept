/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "ShaderEditASM.h"
#include "SEContext.h"
#include <MiscUtils.h>
#include <CommonErrorLog.h>

#define GL_PROGRAM_ERROR_STRING_ARB     0x8874
#define GL_PROGRAM_ERROR_POSITION_ARB   0x864B
#define GL_PROGRAM_ERROR_POSITION_NV    0x864B

USING_ERRORLOG

//The call back ids
enum CallBackIDs
{
  CBI_glGenPrograms = SHADER_ASM_CALLBACK_INDEX,
  CBI_glDeletePrograms,
  CBI_glBindProgram,
  CBI_glProgramString,
  CBI_glLoadProgram
};

///////////////////////////////////////////////////////////////////////////////
//
ShaderEditASM::ShaderEditASM(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),
GLV(callBacks->GetCoreGLFunctions()),

extensionARBVertexProgram(false),
extensionNVVertexProgram(false),
extensionARBFragmentProgram(false),
extensionNVFragmentProgram(false),

//Initernal variable init
inumNewShaders(0),
inewShaderIDs(NULL),
ishaderTarget(0),
ishaderID(0),
ishaderLength(0),
ishaderSrc(0),

validShaderReady(false),

iglGetProgramiv(NULL),
iglGenProgramsARB(NULL),
iglBindProgramARB(NULL),
iglProgramStringARB(NULL),
iglDeleteProgramsARB(NULL),

iglGenProgramsNV(NULL),
iglBindProgramNV(NULL),
iglLoadProgramNV(NULL),
iglDeleteProgramsNV(NULL)

{

}

///////////////////////////////////////////////////////////////////////////////
//
ShaderEditASM::~ShaderEditASM()
{

}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditASM::Init()
{
  //Return if we cannot make OpenGL calls
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return false;
  }

  //Check extension support
  extensionARBVertexProgram   = gliCallBacks->IsGLExtensionSupported("GL_ARB_vertex_program");
  extensionNVVertexProgram    = gliCallBacks->IsGLExtensionSupported("GL_NV_vertex_program"); 
  extensionARBFragmentProgram = gliCallBacks->IsGLExtensionSupported("GL_ARB_fragment_program"); 
  extensionNVFragmentProgram  = gliCallBacks->IsGLExtensionSupported("GL_NV_fragment_program"); 

  //Check that at least some ASM shaders are supported
  if(!extensionARBVertexProgram   && !extensionNVVertexProgram &&
     !extensionARBFragmentProgram && !extensionNVFragmentProgram)
  {
    LOGERR(("ShaderEditASM::Init - No asm OpenGL extension support."));
    return false;
  }

  //Init function pointers
  // (this can be done as these functions will only be called on valid programs)
  iglGetProgramiv = (void (GLAPIENTRY *)(GLenum target, GLenum pname, GLint *params))gliCallBacks->GetGLFunction("glGetProgramivARB");  
  if((extensionARBVertexProgram || extensionARBFragmentProgram) && 
    iglGetProgramiv == NULL)
  {
    LOGERR(("ShaderEditASM::Init - Unable to get ARB shader function entry point."));
    return false;
  }

  void **loadFunc;

  //Helper define for function lookups
#define GL_FUNC_LOOKUP(string)                           \
  loadFunc  = (void**)(&i##string);                      \
  *loadFunc = gliCallBacks->GetGLFunction(#string);      \
  if(*loadFunc == NULL)                                  \
  {                                                      \
    LOGERR(("Function %s unable to be mapped",#string)); \
    return false;                                        \
  }

  //Load the ARB extension entry points
  if(extensionARBVertexProgram || extensionARBFragmentProgram)
  {
    GL_FUNC_LOOKUP(glGenProgramsARB);
    GL_FUNC_LOOKUP(glBindProgramARB);
    GL_FUNC_LOOKUP(glProgramStringARB);
    GL_FUNC_LOOKUP(glDeleteProgramsARB);
  }

  //Load the NV extension entry points
  if(extensionNVVertexProgram || extensionNVFragmentProgram)
  {
    GL_FUNC_LOOKUP(glGenProgramsNV);
    GL_FUNC_LOOKUP(glBindProgramNV);
    GL_FUNC_LOOKUP(glLoadProgramNV);
    GL_FUNC_LOOKUP(glDeleteProgramsNV);
  }

#undef GL_FUNC_LOOKUP

  //Register the shader based functions
  gliCallBacks->RegisterGLFunction("glGenProgramsARB");
  gliCallBacks->RegisterGLFunction("glGenProgramsNV");
  gliCallBacks->SetFunctionID("glGenProgramsARB",CBI_glGenPrograms);
  gliCallBacks->SetFunctionID("glGenProgramsNV", CBI_glGenPrograms);

  gliCallBacks->RegisterGLFunction("glDeleteProgramsARB");
  gliCallBacks->RegisterGLFunction("glDeleteProgramsNV");
  gliCallBacks->SetFunctionID("glDeleteProgramsARB",CBI_glDeletePrograms);
  gliCallBacks->SetFunctionID("glDeleteProgramsNV", CBI_glDeletePrograms);

  gliCallBacks->RegisterGLFunction("glBindProgramARB");
  gliCallBacks->RegisterGLFunction("glBindProgramNV");
  gliCallBacks->SetFunctionID("glBindProgramARB",CBI_glBindProgram);
  gliCallBacks->SetFunctionID("glBindProgramNV", CBI_glBindProgram);

  gliCallBacks->RegisterGLFunction("glProgramStringARB");
  gliCallBacks->SetFunctionID("glProgramStringARB",CBI_glProgramString);

  gliCallBacks->RegisterGLFunction("glLoadProgramNV");
  gliCallBacks->SetFunctionID("glLoadProgramNV", CBI_glLoadProgram);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
ShaderEditASM *ShaderEditASM::CreateInstance(InterceptPluginCallbacks *callBacks)
{
  //Create the new shader edit
  ShaderEditASM * newShaderEdit = new ShaderEditASM(callBacks);

  //Attempt to init
  if(!newShaderEdit->Init())
  {
    LOGERR(("ShaderEditASM::CreateInstance - Unable to init new ASM shader editor"));
    newShaderEdit->ReleaseReference();
    return NULL;
  }

  return newShaderEdit;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditASM::DriverAvailable()
{
  return gliCallBacks->GetGLInternalCallState();
}


///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::LogFunctionPre(uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args)
{
  //Return if GL calls cannot be made 
  if(!DriverAvailable())
  {
    return;
  }

  //Create a access copy of the arguments
  FunctionArgs newArgs(args);

  //Perform the action
  switch(updateID)
  {
    case(CBI_glGenPrograms) :
      CreateARBShadersPre(newArgs);
      break;

    case(CBI_glDeletePrograms) :
      DeleteARBShaders(newArgs);
      break;

    case(CBI_glBindProgram) :
      BindARBShaderPre(newArgs);
      break;

    case(CBI_glProgramString) :
    case(CBI_glLoadProgram) :
      SetARBShaderReadyPre(updateID,newArgs);
      break;

    default:
       LOGERR(("ShaderEditASM::LogFunctionPre - Unknown update ID"));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::LogFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal)
{
  //Perform the action
  switch(updateID)
  {
    case(CBI_glGenPrograms) :
      CreateARBShadersPost();
      break;

    case(CBI_glDeletePrograms) :
      break;

    case(CBI_glBindProgram) :
      break;

    case(CBI_glProgramString) :
    case(CBI_glLoadProgram) :
      SetARBShaderReadyPost(updateID);
      break;

    default:
       LOGERR(("ShaderEditASM::LogFunctionPost - Unknown update ID"));
  }
}
  

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::CreateARBShadersPre(FunctionArgs & args)
{
  //Get the number of shader ID's created
  args.Get(inumNewShaders);

  //Get the shader array
  args.Get(inewShaderIDs);

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::CreateARBShadersPost()
{
  //Check the array
  if(inewShaderIDs == NULL || inumNewShaders < 0)
  {
    LOGERR(("ShaderEditASM::CreateARBShadersPost - Invalid shader ID array"));
    return;
  }
  
  //Loop for the returned shader ID's
  for(uint i=0;i<(uint)inumNewShaders;i++)
  {
    shaderManager.AddData(inewShaderIDs[i]);
  }

  //Reset the arrays
  inumNewShaders = 0;
  inewShaderIDs  = NULL;

}


///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::BindARBShaderPre(FunctionArgs & args)
{
  //Get the target type
  GLenum newType;
  args.Get(newType);

  //Get the shader id
  GLuint shaderID;
  args.Get(shaderID);

  //Check if the shader exists yet or not
  ShaderDataASM * shaderData = shaderManager.GetData(shaderID);
  if(!shaderData)
  {
    //Add the shader data to the array
    shaderManager.AddData(shaderID);

    //Retrieve the pointer
    shaderData = shaderManager.GetData(shaderID);
    if(shaderData)
    {
      //Update the frame number
      shaderData->GetUniqueID()->SetLastFrameUsed(gliCallBacks->GetFrameNumber());
    }
  }
  else
  {
    //Update the frame number
    shaderData->GetUniqueID()->SetLastFrameUsed(gliCallBacks->GetFrameNumber());

    //If the shader is ready, ensure it was bound to the correct target
    if(shaderData->IsReady() && newType != shaderData->GetGLType())
    {
      LOGERR(("ShaderEditASM::BindARBShaderPre - Binding shader %u to a different target?",shaderID)); 
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::DeleteARBShaders(FunctionArgs & args)
{
  //Get the count
  GLsizei numShaders;
  args.Get(numShaders);

  //Get the shader array
  const GLuint *shaderIDs;
  args.Get(shaderIDs);

  //Catch bad values
  if(numShaders <= 0)
  {
    return;  
  }

  //Loop for all the shaders
  for(uint i=0;i<(uint)numShaders;i++)
  {
    //Remove the shader ID
    shaderManager.RemoveData(shaderIDs[i]);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::SetARBShaderReadyPre(uint updateID, FunctionArgs & args)
{
  //Reset the flag
  validShaderReady = false;

  if(updateID == CBI_glProgramString)
  {
    //Get the target
    args.Get(ishaderTarget);

    //Return false for unknown shader types
    if(ishaderTarget != GL_VERTEX_PROGRAM_ARB &&
       ishaderTarget != GL_FRAGMENT_PROGRAM_ARB)
    {
      LOGERR(("ShaderEditASM::SetARBShaderReadyPre - Unknown/unsupported shader type: 0x%x",ishaderTarget));
      return;
    }

    //Get the current shader index for the target
    ishaderID = GetActiveShaderID(ishaderTarget);

    //Get the format
    GLenum shaderFormat;
    args.Get(shaderFormat);

    //Check the format
    if(shaderFormat != GL_PROGRAM_FORMAT_ASCII_ARB)
    {
      LOGERR(("ShaderEditASM::SetARBShaderReadyPre - Need to pass ascii string for editing : ShaderID = %u",ishaderID));
      return;
    }

    //Get the length
    args.Get(ishaderLength);

    //Get the string
    args.Get(ishaderSrc);

  }
  else if(updateID == CBI_glLoadProgram)
  {
    //Get the target
    args.Get(ishaderTarget);

    //Return false for unknown shader types
    if(ishaderTarget != GL_VERTEX_PROGRAM_NV &&
       ishaderTarget != GL_FRAGMENT_PROGRAM_NV)
    {
      LOGERR(("ShaderEditASM::SetARBShaderReadyPre - Unknown/unsupported NV shader type: 0x%x",ishaderTarget));
      return;
    }

    //Get the id
    args.Get(ishaderID);

    //NV state programs do not need a bind call so 
    //  check if the shader exists yet or not
    ShaderDataASM * shaderData = shaderManager.GetData(ishaderID);
    if(!shaderData)
    {
      //Add the shader data to the array
      shaderManager.AddData(ishaderID);
    }

    //Get the length
    args.Get(ishaderLength);

    //Get the string
    args.Get(ishaderSrc);

  }
  else
  {
    LOGERR(("ShaderEditASM::SetARBShaderReadyPre - Unhandled program type"));
    return;
  }

  //Flag success
  if(ishaderLength > 0 && ishaderSrc != NULL && ishaderID != 0)
  {
    validShaderReady = true;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::SetARBShaderReadyPost(uint updateID)
{
  //Return if GL calls cannot be made
  if(!DriverAvailable() || !validShaderReady)
  {
    return;
  }

  //Note: Both ARB and NV programs call this code. (don't make extension specific calls)

  //Get the data for the shader
  ShaderDataASM * shaderData = shaderManager.GetData(ishaderID);
  if(!shaderData)
  {
    LOGERR(("ShaderEditASM::SetARBShaderReadyPost - Unknown or invalid shader? ShaderID = %u",ishaderID));
    return;
  }

  //Check for type changes
  if(shaderData->IsReady() && ishaderTarget != shaderData->GetGLType())
  {
    LOGERR(("ShaderEditASM::SetARBShaderReadyPost - Assigning new shader type to ID = %u?",ishaderID));
  }

  //Init the shader data variables
  shaderData->SetReady(ishaderTarget);

  //Assign the type of program
  switch(ishaderTarget)
  {
    case(GL_VERTEX_PROGRAM_ARB):

      //See what interface the program was loaded through
      if(updateID == CBI_glProgramString)
      {
        shaderData->GetUniqueID()->SetShaderType(USID_ARB_VP);
      }
      else
      {
        shaderData->GetUniqueID()->SetShaderType(USID_NV_VP);
      }
      break;

    case(GL_FRAGMENT_PROGRAM_ARB):
      shaderData->GetUniqueID()->SetShaderType(USID_ARB_FP);
      break;

    case(GL_FRAGMENT_PROGRAM_NV):
      shaderData->GetUniqueID()->SetShaderType(USID_NV_FP);
      break;
  }


  //Set the shader string
  char * shaderCharSrc = new char[ishaderLength+1];
  memcpy(shaderCharSrc,ishaderSrc,ishaderLength*sizeof(char));

  //Append a NULL character
  shaderCharSrc[ishaderLength] = '\0';

  //Note: If the shader source is updated, what happens to contexts that already have the shader bound?

  //Assign the array to the source string 
  shaderData->SetShaderSource(shaderCharSrc);

  //Delete the old array
  delete [] shaderCharSrc;
 
  //Reset the flag
  validShaderReady = false;
}



///////////////////////////////////////////////////////////////////////////////
//
uint ShaderEditASM::GetActiveShaderID(GLenum oglTarget)
{
  int retVal = 0;

  switch(oglTarget)
  {
    //Note: GL_VERTEX_PROGRAM_NV is the same enum
    case(GL_VERTEX_PROGRAM_ARB) :
    case(GL_FRAGMENT_PROGRAM_ARB) :
      {
        //Check for ARB support
        if(iglGetProgramiv != NULL)
        {
          iglGetProgramiv(oglTarget, GL_PROGRAM_BINDING_ARB, &retVal);
        }
        //If no ARB support, fall back to old NV way
        else if(oglTarget == GL_VERTEX_PROGRAM_NV)
        {
          GLV->glGetIntegerv(GL_VERTEX_PROGRAM_BINDING_NV,&retVal);
        }
      }
      break;

    case(GL_FRAGMENT_PROGRAM_NV) :
      GLV->glGetIntegerv(GL_FRAGMENT_PROGRAM_BINDING_NV,&retVal);
      break;

    case(GL_VERTEX_STATE_PROGRAM_NV) :
      //Not sure how to handle state programs
      break;

    default:
      LOGERR(("Unknown shader target %x ",oglTarget));
  }
  return retVal;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditASM::FindShader(const UniqueShaderID *findShader)
{
  //Attempt to find the GL id
  ShaderDataASM * shaderData = shaderManager.GetData(findShader->GetShaderOpenGLID());

  //If the GL ID is found in the manager and the unique IDs match,
  // the shader is found (Note that the same GL ID can exist in multiple contexts)
  if(shaderData && 
     shaderData->GetUniqueID()->GetUniqueIndex() == findShader->GetUniqueIndex())
  {
    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::SetShaderFrameNum(uint newFrameNum)
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return;
  }

  //Get what shaders are bound
  //If ARB/NV vertex program support, get the bound shader
  if(extensionARBVertexProgram || extensionNVVertexProgram)
  {
    uint tmpID = GetActiveShaderID(GL_VERTEX_PROGRAM_ARB);
    if(tmpID != 0)
    {
      //Attempt to find the shader and set the new frame number
      ShaderDataASM * shaderData = shaderManager.GetData(tmpID);
      if(shaderData)
      {
        shaderData->GetUniqueID()->SetLastFrameUsed(newFrameNum);
      }
    }
  }

  //If ARB fragment program support, get the bound shader
  if(extensionARBFragmentProgram)
  {
    uint tmpID = GetActiveShaderID(GL_FRAGMENT_PROGRAM_ARB);
    if(tmpID != 0)
    {
      //Attempt to find the shader and set the new frame number
      ShaderDataASM * shaderData = shaderManager.GetData(tmpID);
      if(shaderData)
      {
        shaderData->GetUniqueID()->SetLastFrameUsed(newFrameNum);
      }
    }
  }

  //If NV fragment program support, get the bound shader
  if(extensionNVFragmentProgram)
  {
    uint tmpID = GetActiveShaderID(GL_FRAGMENT_PROGRAM_NV);
    if(tmpID != 0)
    {
      //Attempt to find the shader and set the new frame number
      ShaderDataASM * shaderData = shaderManager.GetData(tmpID);
      if(shaderData)
      {
        shaderData->GetUniqueID()->SetLastFrameUsed(newFrameNum);
      }
    }
  }


}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditASM::IsShaderActive(uint shaderUID, USIDType shaderType)
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return false;
  }

  GLenum oglShaderType = 0;

  //Check extension support for all types
  if((shaderType == USID_ARB_VP && extensionARBVertexProgram) ||
     (shaderType == USID_NV_VP && extensionNVVertexProgram))
  {
    oglShaderType = GL_VERTEX_PROGRAM_ARB;
  }
  if(shaderType == USID_ARB_FP && extensionARBFragmentProgram)
  {
    oglShaderType = GL_FRAGMENT_PROGRAM_ARB;
  }
  if(shaderType == USID_NV_FP && extensionNVFragmentProgram)
  {
    oglShaderType = GL_FRAGMENT_PROGRAM_NV;
  }

  //If a OpenGL type was set
  if(oglShaderType != 0)
  {
    //Get the bound program ID
    GLuint boundID = GetActiveShaderID(oglShaderType);

    //Check if the shader type is enabled
    if(boundID != 0 && GLV->glIsEnabled(oglShaderType))
    {
      //Get the shader data and compare the unique index
      ShaderDataASM * shaderData = shaderManager.GetData(boundID);
      if(shaderData && shaderData->GetUniqueID()->GetUniqueIndex() == shaderUID)
      {
        return true;
      }
    }
  }


  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditASM::AddSubstituteProgram(GLuint programID, const string &programSource, string &retLog)
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    retLog += "==OpenGL driver not available to compile.==\n";
    return false;
  }

  //Check if the program exists yet or not
  ShaderDataASM * shaderData = shaderManager.GetData(programID);
  if(!shaderData)
  {
    //Add the shader data to the array
    LOGERR(("ShaderEditASM::AddSubstituteProgram - Invalid ID %u",programID));
    retLog += "==Invalid OpenGL program ID==\n";
    return false;
  }

  //Get the old bound program ID
  GLuint oldBoundID = GetActiveShaderID(shaderData->GetGLType());

  //Clear any error code
  GLenum errorCode = GLV->glGetError();
  if(errorCode != GL_NO_ERROR)
  {
    retLog += "==OpenGL error pre-compile - fix application==\n";     
  }
  
  USIDType shaderType     = shaderData->GetUniqueID()->GetShaderType();
  GLenum   oglProgramType = shaderData->GetGLType();

  //Check extension support 
  if(shaderType == USID_ARB_VP && !extensionARBVertexProgram)
  {
    retLog += "==ARB vertex program not supported==\n";
    return false;
  }
  if(shaderType == USID_ARB_FP && !extensionARBFragmentProgram)
  {
    retLog += "==ARB fragment program not supported==\n";
    return false;
  }
  if(shaderType == USID_NV_VP && !extensionNVVertexProgram)
  {
    retLog += "==NV vertex program not supported==\n";
    return false;
  }
  if(shaderType == USID_NV_FP && !extensionNVFragmentProgram)
  {
    retLog += "==NV fragment program not supported==\n";
    return false;
  }

  //Compile based on the program type
  if(shaderType == USID_ARB_VP ||
     shaderType == USID_ARB_FP)
  {
    //ARB compile
    //Create a new program ID to test the compile
    GLuint newProgramID;
    iglGenProgramsARB(1, &newProgramID);

    //Bind the test program and load the source
    iglBindProgramARB(oglProgramType, newProgramID);
    iglProgramStringARB(oglProgramType, GL_PROGRAM_FORMAT_ASCII_ARB,
                        (GLsizei)programSource.size(), programSource.c_str());

    //Get any log info
    string infoStr = (const char*)GLV->glGetString(GL_PROGRAM_ERROR_STRING_ARB);
    if(infoStr.size() > 0)
    {
      retLog += "Program compile info\n";
      retLog += infoStr;
      retLog += "\n";
    }
    
    //Get any error position
    GLint errorPos;
    GLV->glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorPos);
    if(errorPos >= 0)
    {
      FormatCompileErrorLocation(programSource, errorPos, retLog);
    }

    //Get any error codes 
    errorCode = GLV->glGetError();
    if(errorCode != GL_NO_ERROR)
    {
      retLog += "==Program compile failure==\n";
      iglDeleteProgramsARB(1, &newProgramID);
      return false;
    }

    //Delete the test load program
    iglDeleteProgramsARB(1, &newProgramID);
    newProgramID = 0;

    //Compile the real program
    iglBindProgramARB(oglProgramType, shaderData->GetGLID());
    iglProgramStringARB(oglProgramType, GL_PROGRAM_FORMAT_ASCII_ARB,
                        (GLsizei)programSource.size(), programSource.c_str());

    //Get any error codes 
    errorCode = GLV->glGetError();
    if(errorCode != GL_NO_ERROR)
    {
      retLog += "==Program test compiled OK, failed substitute compile ==\n";
      return false;
    }

    //Re-bind the old program ID
    iglBindProgramARB(oglProgramType, oldBoundID);
  }

  //Compile the NV versions
  else if(shaderType == USID_NV_VP ||
          shaderType == USID_NV_FP)
  {
    //NV compile
    //Create a new program ID to test the compile
    GLuint newProgramID;
    iglGenProgramsNV(1, &newProgramID);

    //Load the source into a test program
    iglLoadProgramNV(oglProgramType, newProgramID, (GLsizei)programSource.size(), (const GLubyte*)programSource.c_str());

    //Get any error position
    GLint errorPos;
    GLV->glGetIntegerv(GL_PROGRAM_ERROR_POSITION_NV, &errorPos);
    if(errorPos >= 0)
    {
      FormatCompileErrorLocation(programSource, errorPos, retLog);
    }

    //Get any error codes 
    errorCode = GLV->glGetError();
    if(errorCode != GL_NO_ERROR)
    {
      retLog += "==Program compile failure==\n";
      iglDeleteProgramsNV(1, &newProgramID);
      return false;
    }

    //Delete the test load program
    iglDeleteProgramsNV(1, &newProgramID);
    newProgramID = 0;

    //Compile the real program
    iglLoadProgramNV(oglProgramType, shaderData->GetGLID(), (GLsizei)programSource.size(), (const GLubyte*)programSource.c_str());

    //Get any error codes 
    errorCode = GLV->glGetError();
    if(errorCode != GL_NO_ERROR)
    {
      retLog += "==Program test compiled OK, failed substitute compile ==\n";
      return false;
    }

  }
  else
  {
    retLog += "==Unknown program type==\n";
    return false;
  }

  //Set the alternate shader source
  shaderData->GetUniqueID()->SetOverrideShaderSource(programSource);
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::FormatCompileErrorLocation(const string &programSource, GLint errorPos, string &retLog)
{
  //Get the number of lines
  uint lineNum = 1;
  uint charNum = 1;
  for(GLint i=0; i<errorPos && i<(int)programSource.size(); i++)
  {
    //Count the number of new lines
    if(programSource[i] == '\n')
    {
      lineNum++;
      charNum = 1;
    }
    else
    {
      charNum++;
    }
  }

  //Copy five characters near the error
  string errorData(programSource,errorPos,5);

  //Append the error string data
  string buffer;
  StringPrintF(buffer,"line %u, column %u: Compile error near >%s< char (%d)\n", lineNum, charNum, errorData.c_str(), errorPos);
  retLog += buffer;

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditASM::GetBoundShaders(uint &vertexShaderID,uint &fragmentShaderID)
{
/*
  //Set initial return values
  vertexShaderID  = 0;
  fragmentShaderID = 0;

  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return;
  }

  //If ARB/NV vertex program support, get the bound shader
  if( (extensionARBVertexProgram || extensionNVVertexProgram) &&
       GLV.glIsEnabled(GL_VERTEX_PROGRAM_ARB))
  {
    uint tmpID = GetActiveShaderID(GL_VERTEX_PROGRAM_ARB);

    //Check the return ID
    if(vertexShaderID != 0 && tmpID != vertexShaderID)
    {
      LOGERR(("ShaderEditASM::GetBoundShaders - Two vertex progarms bound at once?"));
    }

    //Assign the return value
    vertexShaderID = tmpID;
  }


  //If ARB fragment program support, get the bound shader
  if(extensionARBFragmentProgram && GLV.glIsEnabled(GL_FRAGMENT_PROGRAM_ARB))
  {
    uint tmpID = GetActiveShaderID(GL_FRAGMENT_PROGRAM_ARB);

    //Check the return ID
    if(fragmentShaderID != 0 && tmpID != fragmentShaderID)
    {
      LOGERR(("ShaderEditASM::GetBoundShaders - Two fragment progarms bound at once?"));
    }

    //Assign the return value
    fragmentShaderID = tmpID;
  }


  //If NV fragment program support, get the bound shader
  if(extensionNVFragmentProgram && GLV.glIsEnabled(GL_FRAGMENT_PROGRAM_NV))
  {
    uint tmpID = GetActiveShaderID(GL_FRAGMENT_PROGRAM_NV);

    //Check the return ID
    if(fragmentShaderID != 0 && tmpID != fragmentShaderID)
    {
      LOGERR(("ShaderEditASM::GetBoundShaders - Two fragment progarms bound at once?"));
    }

    //Assign the return value
    fragmentShaderID = tmpID;
  }
*/
}



