/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptShaderGLSL.h"
#include "FileUtils.h"
#include "GLDriver.h"

//The real OpenGL driver
extern GLCoreDriver GLV;
extern WGLDriver    GLW;

USING_ERRORLOG


//Flags to indicate what action to take when we get an
//  shader function
enum ShaderGLSLFunctionAction
{
  GLSLA_UNKNOWN =0,
  
  GLSLA_OBJECT_CREATE,
  GLSLA_OBJECT_DELETE,

  GLSLA_SHADER_SOURCE,
  GLSLA_SHADER_COMPILE,

  GLSLA_OBJECT_ATTACH,
  GLSLA_OBJECT_DETACH,

  GLSLA_LINK_PROGRAM,
  GLSLA_USE_PROGRAM,

  GLSLA_VALIDATE_PROGRAM,

};

enum ShaderGLSLFunctionType
{
  GLI_CREATE_SHADER_OBJECT =0,
  GLI_CREATE_PROGRAM_OBJECT,

  GLI_DELETE_PROGRAM,
  GLI_DELETE_SHADER,
  GLI_DELETE_OBJECT,

  GLI_SHADER_SOURCE,

  GLI_COMPILE_SHADER,
  
  GLI_ATTACH_OBJECT,
  GLI_DETACH_OBJECT,

  GLI_LINK_PROGRAM,

  GLI_USE_PROGRAM_OBJECT,

  GLI_VALADATE_PROGRAM,
};

// Structure to hold the name of functions that affect the shader state
struct ShaderGLSLLogData
{
  ShaderGLSLLogData(const char * funcName, ShaderGLSLFunctionType funcType, ShaderGLSLFunctionAction newAction);

  string                   functionName;  //The name of the function
  ShaderGLSLFunctionAction action;        //What the function does and what action to take
  ShaderGLSLFunctionType   funcType;      //The type of the function (enum version of name)
  int                      functionIndex; //Index of the function (in the function table, -may be -1 if unknown)

};


//The array of built-in known functions
ShaderGLSLLogData knownShaderFunctionsGLSL[] = 
{

  //General creation/deletion shaders
  ShaderGLSLLogData("glCreateShader"         ,GLI_CREATE_SHADER_OBJECT,GLSLA_OBJECT_CREATE),
  ShaderGLSLLogData("glCreateShaderObjectARB",GLI_CREATE_SHADER_OBJECT,GLSLA_OBJECT_CREATE),

  ShaderGLSLLogData("glCreateProgram"         ,GLI_CREATE_PROGRAM_OBJECT,GLSLA_OBJECT_CREATE),
  ShaderGLSLLogData("glCreateProgramObjectARB",GLI_CREATE_PROGRAM_OBJECT,GLSLA_OBJECT_CREATE),

  ShaderGLSLLogData("glDeleteProgram"   ,GLI_DELETE_PROGRAM,GLSLA_OBJECT_DELETE),
  ShaderGLSLLogData("glDeleteShader"    ,GLI_DELETE_SHADER, GLSLA_OBJECT_DELETE),
  ShaderGLSLLogData("glDeleteObjectARB" ,GLI_DELETE_OBJECT, GLSLA_OBJECT_DELETE),

  ShaderGLSLLogData("glShaderSource"    ,GLI_SHADER_SOURCE,GLSLA_SHADER_SOURCE),
  ShaderGLSLLogData("glShaderSourceARB" ,GLI_SHADER_SOURCE,GLSLA_SHADER_SOURCE),

  ShaderGLSLLogData("glCompileShader"   ,GLI_COMPILE_SHADER,GLSLA_SHADER_COMPILE),
  ShaderGLSLLogData("glCompileShaderARB",GLI_COMPILE_SHADER,GLSLA_SHADER_COMPILE),

  ShaderGLSLLogData("glAttachShader"    ,GLI_ATTACH_OBJECT,GLSLA_OBJECT_ATTACH),  
  ShaderGLSLLogData("glAttachObjectARB" ,GLI_ATTACH_OBJECT,GLSLA_OBJECT_ATTACH),  

  ShaderGLSLLogData("glDetachShader"    ,GLI_DETACH_OBJECT,GLSLA_OBJECT_DETACH),  
  ShaderGLSLLogData("glDetachObjectARB" ,GLI_DETACH_OBJECT,GLSLA_OBJECT_DETACH),  

  ShaderGLSLLogData("glLinkProgram"     ,GLI_LINK_PROGRAM,GLSLA_LINK_PROGRAM),  
  ShaderGLSLLogData("glLinkProgramARB"  ,GLI_LINK_PROGRAM,GLSLA_LINK_PROGRAM),  

  ShaderGLSLLogData("glUseProgram"         ,GLI_USE_PROGRAM_OBJECT,GLSLA_USE_PROGRAM),  
  ShaderGLSLLogData("glUseProgramObjectARB",GLI_USE_PROGRAM_OBJECT,GLSLA_USE_PROGRAM),  

  ShaderGLSLLogData("glValidateProgram"   ,GLI_VALADATE_PROGRAM,GLSLA_VALIDATE_PROGRAM),  
  ShaderGLSLLogData("glValidateProgramARB",GLI_VALADATE_PROGRAM,GLSLA_VALIDATE_PROGRAM),  

};



#define NUM_SHADER_LOG_FUNCTIONS sizeof(knownShaderFunctionsGLSL)/sizeof(ShaderGLSLLogData)


///////////////////////////////////////////////////////////////////////////////
//
ShaderGLSLLogData::ShaderGLSLLogData(const char * funcName,ShaderGLSLFunctionType type, ShaderGLSLFunctionAction newAction):
functionName(funcName),
action(newAction),
funcType(type),
functionIndex(-1)
{
}


///////////////////////////////////////////////////////////////////////////////
//
InterceptShaderGLSL::InterceptShaderGLSL(GLDriver *ogldriver,FunctionTable * functionTable,const ConfigData &configData):
InterceptData(ogldriver),
shaderObjectManager(),
shaderProgramManager(),
shaderSaver(configData),
recordInfoLog(configData.shaderAttachLogState),
validatePreRender(configData.shaderValidatePreRender),
dumpUniformsPreRender(configData.shaderLogUniformsPreRender),

initialized(false),
initFailed(false),

//Initernal variable init
iObjectType(0),
iHandle(0),
iHandleDetach(0),

iglGetProgramiv(NULL),
iglGetShaderiv(NULL),

iglGetProgramInfoLog(NULL),
iglGetShaderInfoLog(NULL),

iglValidateProgram(NULL),
iglGetActiveUniform(NULL),
iglGetUniformLocation(NULL),
iglGetUniformfv(NULL),
iglGetUniformiv(NULL),
iglGetUniformuiv(NULL)
{

  //Compile time assert since a storage class is re-used
  CASSERT(sizeof(GLuint) == sizeof(GLhandle),GLSL_handles_not_the_same_size_as_unsigned_ints);

  //Register the shader based functions with the function table
  for(uint i=0;i<NUM_SHADER_LOG_FUNCTIONS;i++)
  {
    //Set the flag
    functionTable->SetFunctionFlag(knownShaderFunctionsGLSL[i].functionName,FDF_SHADER_GLSL_LOG);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptShaderGLSL::~InterceptShaderGLSL()
{
  //Turn off shader saving to flush out-standing shaders
  if(GetDataSaving())
  {
    SetDataSaving(false);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptShaderGLSL::Init()
{
  //Test if we can proceed
  if(!driver || !driver->GetInternalGLCallMode() || initFailed)
  {
    return false;
  }

  //Flag that init has been called
  initialized = true;

  //Check extension support
  // (only allow OpenGL 2.0 or better)
  if(driver->GetOpenGLVersion() < 2.0f)
  {
    LOGERR(("The GLSL logger needs OpenGL 2.0 or better to run"));
    return false;
  }

  //Init function pointers
  // (this can be done as these functions will only be called on valid programs)
  void **loadFunc;

#define GL_FUNC_LOOKUP(string)                           \
  loadFunc  = (void**)(&i##string);                      \
  *loadFunc = GLW.glGetProcAddress(#string); \
  if(*loadFunc == NULL)                                  \
  {                                                      \
    LOGERR(("Function %s unable to be mapped",#string)); \
    return false;                                        \
  }                                                      
  
  GL_FUNC_LOOKUP(glGetProgramiv);
  GL_FUNC_LOOKUP(glGetShaderiv);

  GL_FUNC_LOOKUP(glGetProgramInfoLog);
  GL_FUNC_LOOKUP(glGetShaderInfoLog);

  GL_FUNC_LOOKUP(glValidateProgram);
  GL_FUNC_LOOKUP(glGetActiveUniform);
  GL_FUNC_LOOKUP(glGetUniformLocation);

  GL_FUNC_LOOKUP(glGetUniformfv);
  GL_FUNC_LOOKUP(glGetUniformiv);

  // Get the OpenGL 3.0 entry points
  if(driver->GetOpenGLVersion() >= 3.0f)
  {
    GL_FUNC_LOOKUP(glGetUniformuiv);
  }

#undef GL_FUNC_LOOKUP

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptShaderGLSL::DriverAvailable()
{
  //Get the state of the driver
  if(!driver || !driver->GetInternalGLCallMode() || initFailed)
  {
    return false;
  }

  //Test if we have been init
  if(!initialized)
  {
    if(!Init())
    {
      initFailed = true;
      return false;
    }
  }
  
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::SetDataSaving(bool flag)
{
  //Assign the flag by calling the base class
  InterceptData::SetDataSaving(flag);

  //If enabling shader writing, save all currently bound shaders
  if(flag && initialized && DriverAvailable())
  {
    //Get the currently bound program 
    GLuint programID;
    GetBoundProgram(programID);

    //Look it up
    ShaderGLSLData * programData = shaderProgramManager.GetData(programID);
    if(programID == 0 || !programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
    {
      return;
    }

    //If the program is ready and dirty save it
    if(programData->IsReady() && programData->IsDirty())
    {
      SaveShaderData(programData);
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::SetDataSavingPath(const string &newPath)
{
  //Append the Shaders directory then pass to the base class
  string path = newPath + "Shaders" + FileUtils::dirSeparator;

  //Call base class to set
  InterceptData::SetDataSavingPath(path);
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{
  //Return if GL calls cannot be made 
  if(!DriverAvailable())
  {
    return;
  }

  //If this is a render function and we are doing validation/or dumping uinforms
  //  Update the data and save (if the log is currently saving)
  if(GetDataSaving() && (validatePreRender || dumpUniformsPreRender) && 
     driver->GetCurrentContext() && 
     driver->GetCurrentContext()->IsRenderFuncion(funcData,index,args))
  {
    UpdateLogPreRender(funcData);
  }

  //If this is not an GLSL shader function, return now
  if(!funcData->IsFlagSet(FDF_SHADER_GLSL_LOG))
  {
    return;
  }

  //Get the action required for the function
  int actionIndex = GetShaderActionIndex(funcData->GetName(),index);
  if(actionIndex == -1)
  {
    return;
  }
  ShaderGLSLFunctionAction action = knownShaderFunctionsGLSL[actionIndex].action;

  //Create a copy of the arguments
  FunctionArgs newArgs(args);

  //Perform the action
  switch(action)
  {
    case(GLSLA_OBJECT_CREATE) :
      CreateObjectPre(funcData,index,newArgs,knownShaderFunctionsGLSL[actionIndex].funcType);
      break;
    case(GLSLA_OBJECT_DELETE) :
      DeleteObjectPre(funcData,index,newArgs);
      break;

    case(GLSLA_SHADER_SOURCE) :
      ShaderSourcePre(funcData,index,newArgs);
      break;
    case(GLSLA_SHADER_COMPILE) :
      CompileShaderPre(funcData,index,newArgs);
      break;

    case(GLSLA_OBJECT_ATTACH) :
      AttachObjectPre(funcData,index,newArgs);
      break;
    case(GLSLA_OBJECT_DETACH) :
      DetachObjectPre(funcData,index,newArgs);
      break;

    case(GLSLA_LINK_PROGRAM) :
      LinkProgramPre(funcData,index,newArgs);
      break;
    case(GLSLA_USE_PROGRAM) :
      UseProgramPre(funcData,index,newArgs);
      break;

    case(GLSLA_VALIDATE_PROGRAM) :
      ValidateProgramPre(funcData,index,newArgs);
      break;
    
    default:
       LOGERR(("InterceptShader::LogFunctionPre - Unknown action"));
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{
  //Return if GL calls cannot be made
  if(!funcData->IsFlagSet(FDF_SHADER_GLSL_LOG) || !DriverAvailable())
  {
    return;
  }

  //Get the action required for the function
  int actionIndex = GetShaderActionIndex(funcData->GetName(),index);
  if(actionIndex == -1)
  {
    return;
  }
  ShaderGLSLFunctionAction action = knownShaderFunctionsGLSL[actionIndex].action;

  //Perform the action
  switch(action)
  {
    case(GLSLA_OBJECT_CREATE) :
      CreateObjectPost(funcData,index,retVal);
      break;
    case(GLSLA_OBJECT_DELETE) :
      DeleteObjectPost(funcData,index,retVal,knownShaderFunctionsGLSL[actionIndex].funcType);
      break;

    case(GLSLA_SHADER_SOURCE) :
      break;
    case(GLSLA_SHADER_COMPILE) :
      CompileShaderPost(funcData,index,retVal);
      break;

    case(GLSLA_OBJECT_ATTACH) :
      break;
    case(GLSLA_OBJECT_DETACH) :
      DetachObjectPost(funcData,index,retVal);
      break;

    case(GLSLA_LINK_PROGRAM) :
      LinkProgramPost(funcData,index,retVal);
      break;
    case(GLSLA_USE_PROGRAM) :
      break;

    case(GLSLA_VALIDATE_PROGRAM) :
      ValidateProgramPost(funcData,index,retVal);
      break;

    default:
       LOGERR(("InterceptShader::LogFunctionPost - Unknown action"));
  }

}
  

///////////////////////////////////////////////////////////////////////////////
//
int InterceptShaderGLSL::GetShaderActionIndex(const string &functionName, int funcIndex)
{
  uint i;

  //Check the list to find the index 
  for(i=0;i<NUM_SHADER_LOG_FUNCTIONS;i++)
  {
    //If the index is the same, we have mapped this function previously 
    if(knownShaderFunctionsGLSL[i].functionIndex == funcIndex)
    {
      return i;
    }
  }

  //If this function has not been called previously, find the name and assign the index
  for(i=0;i<NUM_SHADER_LOG_FUNCTIONS;i++)
  {
    //If the name is equal, assign the index
    if(knownShaderFunctionsGLSL[i].functionName == functionName)
    {
      knownShaderFunctionsGLSL[i].functionIndex = funcIndex;
      return i;
    }

  }

  //If the function is not found, something has broken with the mapping/registery
  LOGERR(("InterceptShaderGLSL::GetShaderActionIndex - Attempting to log unknown function %s",functionName.c_str()));

  return -1;
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::CreateObjectPre(const FunctionData *funcData,uint index, FunctionArgs & args,uint funcType)
{
  //If the function is a create shader object, get the shader type
  if(funcType == GLI_CREATE_SHADER_OBJECT)
  {
    //Get the type created
    args.Get(iObjectType);
  }
  else if(funcType == GLI_CREATE_PROGRAM_OBJECT)
  {
    //If it is a program object, just assign the type
    iObjectType = GL_PROGRAM_OBJECT_ARB;
  }
  else
  {
    //Unknown function type
    LOGERR(("InterceptShaderGLSL::CreateObjectPre - Unknown function?"));
    return;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::CreateObjectPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{
  //Check the type
  if(iObjectType != GL_PROGRAM_OBJECT_ARB &&
     iObjectType != GL_VERTEX_SHADER  &&
     iObjectType != GL_GEOMETRY_SHADER &&
     iObjectType != GL_FRAGMENT_SHADER)
  {
    LOGERR(("InterceptShaderGLSL::CreateObjectPost - Unknown object type 0x%x",iObjectType));
    return;
  }

  //Get the returned handle
  GLuint newHandle;
  retVal.Get(newHandle);

  //Add as a shader program or shader object 
  if(iObjectType == GL_PROGRAM_OBJECT_ARB)
  {
    //Check for an existing object
    ShaderGLSLData * shaderData = shaderProgramManager.GetData(newHandle);
    if(shaderData)
    {
      LOGERR(("InterceptShaderGLSL::CreateObjectPost - Existing handle ID %u",newHandle));
      return;
    }

    //Create a new shader program
    shaderProgramManager.AddData(newHandle);

    //Get a pointer to the new object
    shaderData = shaderProgramManager.GetData(newHandle);
    if(!shaderData)
    {
      LOGERR(("InterceptShaderGLSL::CreateObjectPost - Unable to create shader ID %u",newHandle));
      return;
    }

    //Assign the object type (assigned as ready where there is some data to save)
    shaderData->SetGLType(iObjectType);
  }
  else
  {
    //Check for an existing object
    ShaderGLSLData * shaderData = shaderObjectManager.GetData(newHandle);
    if(shaderData)
    {
      LOGERR(("InterceptShaderGLSL::CreateObjectPost - Existing handle ID %u",newHandle));
      return;
    }

    //Create a new shader object
    shaderObjectManager.AddData(newHandle);

    //Get a pointer to the new object
    shaderData = shaderObjectManager.GetData(newHandle);
    if(!shaderData)
    {
      LOGERR(("InterceptShaderGLSL::CreateObjectPost - Unable to create shader ID %u",newHandle));
      return;
    }

    //Assign the object type (assigned as ready where there is some data to save)
    shaderData->SetGLType(iObjectType);
  }

  //Reset the flag
  iObjectType = 0;

}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::DeleteObjectPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the handle
  args.Get(iHandle);

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::DeleteObjectPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal, uint funcType)
{
  //Get the handle and save the data if necessary
  ShaderGLSLData * shaderData = NULL;
  if(funcType == GLI_DELETE_PROGRAM)
  {
    shaderData = shaderProgramManager.GetData(iHandle);
  }
  else if(funcType == GLI_DELETE_SHADER)
  {
    shaderData = shaderObjectManager.GetData(iHandle);
  }
  else
  {
    //Search both arrays for the old ARB shader object case
    shaderData = shaderObjectManager.GetData(iHandle);
    if(!shaderData)
    {
      shaderData = shaderProgramManager.GetData(iHandle);
    }
  }
  if(!shaderData || iHandle == 0)
  {
    LOGERR(("InterceptShaderGLSL::DeleteObjectPost - Unable to find shader ID %u",iHandle));
    return;
  }

  //Save the data if it is ready
  if(shaderData->IsReady() && shaderData->IsDirty())
  {
    //Save the dirty shader before it is deleted
    SaveShaderData(shaderData);
  }

  //If it is a program object, detach all references
  // NOTE: This is not technically accurate as the program objects are not deleted fully until
  //       they are no longer bound in any context, however, keeping tack of program binding states
  //       for all contexts will be difficult for this rare case, so it is not done.
  if(shaderData->GetGLType() == GL_PROGRAM_OBJECT_ARB)
  {
    //Get a copy of the attached objects
    vector<GLuint> attachArray = shaderData->GetAttachmentArray();

    //Delete the program object
    shaderProgramManager.RemoveData(iHandle);
    shaderData = NULL;

    //Loop for all attachments
    for(uint i=0;i<attachArray.size();i++)
    {
      ShaderGLSLData * attachData = shaderObjectManager.GetData(attachArray[i]);
      if(attachData)
      {
        //Do a detach from the now deleted object
        attachData->DetachObject(iHandle);

        //If the attachment was previously (user) deleted and there are no remaining attachments,
        // Completely delete now
        if(attachData->GetUserDeleted() && attachData->GetAttachmentArray().size() == 0)
        {
          shaderObjectManager.RemoveData(attachArray[i]);
          attachData= NULL;
        }
      }
      else
      {
        LOGERR(("InterceptShaderGLSL::DeleteObjectPre - Unknown attachment %u",attachArray[i]));
      }
    }
  }
  else
  {
    //Delete now if the shader has no attachments
    if(shaderData->GetAttachmentArray().size() == 0)
    {
      shaderObjectManager.RemoveData(iHandle);
      shaderData = NULL;
    }
    else
    {
      //Else just flag as deleted (Cannot delete until all references are detached)
      shaderData->SetUserDeleted();
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::ShaderSourcePre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the handle
  GLuint srcHandle;
  args.Get(srcHandle);

  //Get number of strings
  GLsizei numStrings;
  args.Get(numStrings);

  //Get the string array
  void * dummyPtr;
  args.Get(dummyPtr);  
  GLchar **strArray = (GLchar **)dummyPtr;

  //Get the string length array
  const GLint * strLengthArray;
  args.Get(strLengthArray);

  //Lookup the shader
  ShaderGLSLData * shaderData = shaderObjectManager.GetData(srcHandle);
  if(!shaderData || shaderData->GetGLType() == GL_PROGRAM_OBJECT_ARB ||
      shaderData->GetUserDeleted())
  {
    LOGERR(("InterceptShaderGLSL::ShaderSourcePre - Unable to find shader ID %u",srcHandle));
    return;
  }

  //Check data
  if(numStrings <= 0 || strArray == NULL)
  {
    LOGERR(("InterceptShaderGLSL::ShaderSourcePre - Bad data passed as source to shader ID %u",srcHandle));
    return;
  }

  //Save the shader if dirty
  if(shaderData->IsReady() && shaderData->IsDirty())
  {
    //Save the dirty shader before it is overwitten
    SaveShaderData(shaderData);
  }

  string shaderSrc;

  //Loop for the number of strings
  uint i;
  for(i=0; i<(uint)numStrings; i++)
  {
    //If the string does have a NULL terminating character
    if(strLengthArray == NULL || strLengthArray[i] < 0)
    {
      //Append the string directly
      shaderSrc = shaderSrc + (char*)strArray[i];
    }
    else
    {
      //Append the atring using the count
      shaderSrc.append((char*)strArray[i], strLengthArray[i]);
    }
  }

  //Remove any "return" characters from the text
  // DT_TODO: Do this in a "format" option? (performed by the save manager?)
  string shaderFormat=shaderSrc;
  shaderSrc = "";
  shaderSrc.reserve(shaderFormat.size());
  for(i=0; i<shaderFormat.size(); i++)
  {
    if(shaderFormat[i] != '\r'&& 
       shaderFormat[i] != EOF && 
       shaderFormat[i] != '\0')
    {
      shaderSrc += shaderFormat[i];
    }
  }

  //Assign the shader source
  shaderData->GetShaderSource() = shaderSrc;

  //Set the shader as ready and dirty
  shaderData->SetReady();

  //Save the shader data if possible
  SaveShaderData(shaderData);
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::CompileShaderPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the handle
  args.Get(iHandle);
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::CompileShaderPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal)
{
  //Lookup the shader
  ShaderGLSLData * shaderData = shaderObjectManager.GetData(iHandle);
  if(!shaderData || shaderData->GetGLType() == GL_PROGRAM_OBJECT_ARB ||
     !shaderData->IsReady() || shaderData->GetUserDeleted())
  {
    LOGERR(("InterceptShaderGLSL::CompileShaderPost - Unable to find active shader ID %u",iHandle));
    return;
  }


  //Append log status
  if(recordInfoLog)
  {
    string logString;
    GetLogData(shaderData,logString);

    //Assign the program source
    shaderData->GetShaderLog() = logString;

    //Set as dirty
    shaderData->SetDirty(true);
  }

  //If the shader exists and is ready and dirty, save it out
  if(shaderData->IsReady() && shaderData->IsDirty())
  {
    SaveShaderData(shaderData);
  }

  //Reset the handle
  iHandle = 0;
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::AttachObjectPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the program
  GLuint programHandle;
  args.Get(programHandle);

  //Get the shader
  GLuint shaderHandle;
  args.Get(shaderHandle);

  //Look for the program and the shader
  ShaderGLSLData * programData = shaderProgramManager.GetData(programHandle);
  ShaderGLSLData * shaderData  = shaderObjectManager.GetData(shaderHandle);
  if(!shaderData || !programData || shaderData == programData ||
      programData->GetGLType() != GL_PROGRAM_OBJECT_ARB ||
      shaderData->GetGLType()  == GL_PROGRAM_OBJECT_ARB ||
      shaderData->GetUserDeleted())
  {
    LOGERR(("InterceptShaderGLSL::AttachObjectPre - Invalid ID's %u,%u",programHandle,shaderHandle));
    return;
  }

  //Add the handle to each other
  programData->AttachObject(shaderHandle);
  shaderData->AttachObject(programHandle);

  //Save the shader if dirty
  if(shaderData->IsReady() && shaderData->IsDirty())
  {
    //Save the dirty shader
    SaveShaderData(shaderData);
  }

  //Save the program if dirty
  if(programData->IsReady() && programData->IsDirty())
  {
    //Save the dirty program
    SaveShaderData(programData);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::DetachObjectPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the program
  args.Get(iHandle);

  //Get the shader
  args.Get(iHandleDetach);
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::DetachObjectPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal)
{
  //Assign the working handles
  GLuint programHandle = iHandle;
  GLuint shaderHandle  = iHandleDetach;

  //Test for zero handles
  if(programHandle == 0 || shaderHandle == 0)
  {
    LOGERR(("InterceptShaderGLSL::DetachObjectPost - Invalid ID's %u,%u",programHandle,shaderHandle));
    return;
  }

  //Look for the program and the shader
  // (Note: You can detach a user deleted object)
  ShaderGLSLData * programData = shaderProgramManager.GetData(programHandle);
  ShaderGLSLData * shaderData  = shaderObjectManager.GetData(shaderHandle);
  if(!shaderData || !programData || shaderData == programData ||
      programData->GetGLType() != GL_PROGRAM_OBJECT_ARB ||
      shaderData->GetGLType()  == GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("InterceptShaderGLSL::DetachObjectPost - Invalid ID's %u,%u",programHandle,shaderHandle));
    return;
  }

  //Remove the handle from each other
  if(!programData->DetachObject(shaderHandle))
  {
    LOGERR(("InterceptShaderGLSL::DetachObjectPost - Error detaching shader from program %u,%u",programHandle,shaderHandle));
  }
  if(!shaderData->DetachObject(programHandle))
  {
    LOGERR(("InterceptShaderGLSL::DetachObjectPost - Error detaching program from shader %u,%u",programHandle,shaderHandle));
  }

  //Save the shader if dirty
  if(shaderData->IsReady() && shaderData->IsDirty())
  {
    //Save the dirty shader
    SaveShaderData(shaderData);
  }

  //Save the program if dirty
  if(programData->IsReady() && programData->IsDirty())
  {
    //Save the dirty program
    SaveShaderData(programData);
  }

  //If the shader data is user deleted (and now has no references) delete it
  if(shaderData->GetUserDeleted() && shaderData->GetAttachmentArray().size() == 0)
  {
    //Remove
    shaderObjectManager.RemoveData(shaderData->GetGLID());

    //This makes the pointers invalid, so reset them
    shaderData  = NULL;
    programData = NULL;
  }

  //Reset the handles
  iHandle = 0;
  iHandleDetach = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::LinkProgramPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{

  //Get the program
  GLuint programHandle;
  args.Get(programHandle);

  //Look for the program 
  ShaderGLSLData * programData = shaderProgramManager.GetData(programHandle);
  if(!programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("InterceptShaderGLSL::LinkProgramPre - Invalid ID %u",programHandle));
    return;
  }
  
  // On successful link, objects can be attached /detached etc without affecting the
  //  internal program -so save the source strings at the time on linking
  string vertexString;
  string geometryString;
  string fragmentString;
  string bufString;

  //Get the array of associated shaders
  const vector<GLuint> & attachedObjects = programData->GetAttachmentArray();

  //Loop for all attachments
  for(uint i=0;i<attachedObjects.size();i++)
  {
    //Get the attachment
    const ShaderGLSLData * attachData = shaderObjectManager.GetData(attachedObjects[i]);
    if(!attachData)
    {
      LOGERR(("InterceptShaderGLSL::LinkProgramPre - Unable to find shader ID %u",attachedObjects[i]));
    }
    else
    {
      //Save the shaders based on the different types
      switch(attachData->GetGLType())
      {
        case(GL_VERTEX_SHADER):
          StringPrintF(bufString,"\n//======================================================\n"
                                   "//   Vertex Shader %u \n"
                                   "//======================================================\n",attachedObjects[i]);
          vertexString += bufString;
          vertexString += attachData->GetShaderLog();
          vertexString += attachData->GetShaderSource();
          break;

        case(GL_GEOMETRY_SHADER):
          StringPrintF(bufString,"\n//======================================================\n"
                                   "//   Geometry Shader %u \n"
                                   "//======================================================\n",attachedObjects[i]);
          geometryString += bufString;
          geometryString += attachData->GetShaderLog();
          geometryString += attachData->GetShaderSource();
          break;

        case(GL_FRAGMENT_SHADER):
          StringPrintF(bufString,"\n//======================================================\n"
                                   "//   Fragment Shader %u\n"
                                   "//======================================================\n",attachedObjects[i]);
          fragmentString += bufString;
          fragmentString += attachData->GetShaderLog();
          fragmentString += attachData->GetShaderSource();
          break;
        default:
          LOGERR(("InterceptShaderGLSL::LinkProgramPre - Unknown shader ID %u",attachedObjects[i]));
          break;
      }
    }
  }

  //Assign the program source
  programData->GetShaderSource() = vertexString + geometryString + fragmentString;

  //Flag the program object as ready and dirty
  programData->SetReady();
  programData->SetDirty(true);

  //Assign the handle to use in the Post Linking version
  iHandle = programHandle;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::LinkProgramPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal)
{
  //Look for the program 
  ShaderGLSLData * programData = shaderProgramManager.GetData(iHandle);
  if(iHandle == 0 || !programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("InterceptShaderGLSL::LinkProgramPost - Invalid ID %u",iHandle));
    return;
  }

  //Append log status
  if(recordInfoLog)
  {
    string logString;
    GetLogData(programData,logString);

    //Assign the program source
    programData->GetShaderLog() = logString;

    //Set as dirty
    programData->SetDirty(true);
  }

  //If the program exists and is ready and dirty, save it out
  if(programData->IsReady() && programData->IsDirty())
  {
    SaveShaderData(programData);
  }


  //Reset the handle
  iHandle = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::UseProgramPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the program
  GLuint programHandle;
  args.Get(programHandle);
  if(programHandle == 0)
  {
    return;
  }

  //Look for the program 
  ShaderGLSLData * programData = shaderProgramManager.GetData(programHandle);
  if(!programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("InterceptShaderGLSL::UseProgramPre - Invalid ID %u",programHandle));
    return;
  }

  //If the program exists and is ready and dirty, save it out
  if(programData->IsReady() && programData->IsDirty())
  {
    SaveShaderData(programData);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::ValidateProgramPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the program
  args.Get(iHandle); 

}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::ValidateProgramPost(const FunctionData *funcData,uint funcTableIndex, const FunctionRetValue & retVal)
{
  //Look for the program 
  ShaderGLSLData * programData = shaderProgramManager.GetData(iHandle);
  if(iHandle == 0 || !programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("InterceptShaderGLSL::ValidateProgramPost - Invalid ID %u",iHandle));
    return;
  }
  
  //If the program exists, is ready and we record log info, save the updated log data
  if(recordInfoLog && programData->IsReady())
  {
    //Append log status
    string logString;
    GetLogData(programData,logString);

    //Assign the program source
    programData->GetShaderLog() = logString;

    //Set as dirty
    programData->SetDirty(true);

    //Save it out
    SaveShaderData(programData);
  }

  //Reset the handle
  iHandle = 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::SaveAllDirtyData() 
{
  uint i;

  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return;
  }

  vector<ShaderGLSLData *> dirtyShaders;

  //Get all the dirty programs
  shaderProgramManager.GetAllDirtyData(dirtyShaders);

  //Loop for all shaders
  for(i=0;i<dirtyShaders.size();i++)
  {
    //Save the shader
    SaveShaderData(dirtyShaders[i]);
  }

  //Get all the dirty shader objects
  dirtyShaders.empty();
  shaderObjectManager.GetAllDirtyData(dirtyShaders);

  //Loop for all shaders
  for(i=0;i<dirtyShaders.size();i++)
  {
    //Save the shader
    SaveShaderData(dirtyShaders[i]);
  }


}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::GetBoundProgram(GLuint &programID)
{
  //Set initial return values
  programID  = 0;

  //Return if GL calls cannot be made
  // (This also checks that OpenGL 2.0 is supported)
  if(!DriverAvailable())
  {
    return;
  }

  //Return the current program
  GLint retData = 0;
  GLV.glGetIntegerv(GL_CURRENT_PROGRAM, &retData);
  programID = retData;
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::GetLogData(const ShaderGLSLData *shaderData, string & retString)
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    retString = "";
    return;
  }

  //Get the type of the object
  GLint logSize=0;
  switch(shaderData->GetGLType())
  {
    //If a program, get the link status
    case(GL_PROGRAM_OBJECT_ARB) :
    {
      GLint linkStatus;
      iglGetProgramiv(shaderData->GetGLID(), GL_LINK_STATUS, &linkStatus);
      if(linkStatus)
      {
        retString = "\n//== PROGRAM LINK STATUS = TRUE"; 
      }
      else
      {
        retString = "\n//== PROGRAM LINK STATUS = FALSE"; 
      }
      GLint validateStatus;
      iglGetProgramiv(shaderData->GetGLID(), GL_VALIDATE_STATUS, &validateStatus);
      if(validateStatus)
      {
        retString += "\n//== PROGRAM VALIDATE STATUS = TRUE\n"; 
      }
      else
      {
        retString += "\n//== PROGRAM VALIDATE STATUS = FALSE\n"; 
      }

      //Get the info log size
      iglGetProgramiv(shaderData->GetGLID(), GL_INFO_LOG_LENGTH, &logSize);

      break;
    }

    //If a shader, get the compile status
    case(GL_FRAGMENT_SHADER) :
    case(GL_VERTEX_SHADER) :
    case(GL_GEOMETRY_SHADER) :
    {
      GLint compileStatus;
      iglGetShaderiv(shaderData->GetGLID(), GL_COMPILE_STATUS, &compileStatus);
      if(compileStatus)
      {
        retString = "\n//== SHADER COMPILE STATUS = TRUE\n"; 
      }
      else
      {
        retString = "\n//== SHADER COMPILE STATUS = FALSE\n"; 
      }

      //Get the info log size
      iglGetShaderiv(shaderData->GetGLID(), GL_INFO_LOG_LENGTH, &logSize);
      break;
    }

    default:
      LOGERR(("InterceptShaderGLSL::GetLogData - Shader %u is of unknown type %u",shaderData->GetGLID(),shaderData->GetGLType()));
      return;
  }
  

  //Get the info log (only get logs of length greater than 1)
  if(logSize > 1)
  {
    //Allocate a array to hold the log
    GLchar * infoLog = new GLchar[logSize+1];
    infoLog[0] = '\0';

    //Get the info log
    if(shaderData->GetGLType() == GL_PROGRAM_OBJECT_ARB)
    {
      iglGetProgramInfoLog(shaderData->GetGLID(), logSize, NULL, infoLog); 
    }
    else
    {
      iglGetShaderInfoLog(shaderData->GetGLID(), logSize, NULL, infoLog); 
    }
    
    //Not necessary, but just to be sure append a NULL character
    infoLog[logSize] = '\0';

    //Append to the string
    retString += "/*== INFO LOG ==\n";
    retString += (char*)infoLog;
    retString += "  == INFO LOG END ==*/\n";

    //Free info log array
    delete [] infoLog;
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::UpdateLogPreRender(const FunctionData *funcData)
{
  //Return now if not validate/dump or not dumping info data
  if(!recordInfoLog || (!validatePreRender && !dumpUniformsPreRender) ||
     !DriverAvailable() || !GetDataSaving())
  {
    return;
  }

  //Get the current bound program
  GLuint programID;
  GetBoundProgram(programID);

  //Return if the program is not "ready" or is deleted
  ShaderGLSLData * programData = shaderProgramManager.GetData(programID);
  if(programID == 0 || !programData || 
     programData->GetGLType() != GL_PROGRAM_OBJECT_ARB || !programData->IsReady())
  {
    return;
  }
  
  //If we validate the program, perform it before the log is updated 
  if(validatePreRender && iglValidateProgram)
  {
    iglValidateProgram(programID);
  }

  //Get the new log
  string logString;
  GetLogData(programData,logString);

  //Assign the program source
  programData->GetShaderLog() = logString;

  //Get if we dump uniforms
  if(dumpUniformsPreRender)
  {
    //Get the number of uniforms
    GLint numUniforms = 0;
    iglGetProgramiv(programData->GetGLID(), GL_ACTIVE_UNIFORMS, &numUniforms);

    //If there are uniforms add a tag
    if(numUniforms > 0)
    {
      programData->GetShaderLog() += "\n//======================================================\n"
                                       "//  Program uniforms (at render call) \n"
                                       "//======================================================\n";
    }

    //Get the max uniform string size
    GLint maxUniformSize = 0;
    iglGetProgramiv(programData->GetGLID(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformSize);

    //If there are uniforms to get, get them
    if(numUniforms > 0 && maxUniformSize > 0)
    {
      //Allocate the array to get the uniform strings
      GLchar * uniformName = new GLchar[maxUniformSize+1];
      string   uniformData;
      string   newUniformName;

      //Loop for the number of uniforms in the program
      for(uint i=0;i<(uint)numUniforms;i++)
      {
        GLint  typeSize = 0;
        GLenum type = 0;
        GLsizei lengthWritten = 0;

        //Init the retrieve uniform name
        uniformName[0] = '\0';

        //Call GetActiveUniform to get the name,type and size of the type
        iglGetActiveUniform(programData->GetGLID(),i,maxUniformSize,&lengthWritten,&typeSize,&type,uniformName);
        if(lengthWritten > 0)
        {
          //Convert to a string
          newUniformName = (char*)uniformName;
          
          //Nvidia/ATI Hack ======================================
          // Nvidia (in 69.xx drivers) and ATI Cat 5.9 return a uniform for every element of arrays.
          // Only take note of the first one and ignore the rest
          bool ignoreUniform = false;
          if(newUniformName.length() > 3 &&
             newUniformName[newUniformName.length()-1] == ']')
          {
            //If the characters end with "[0]"
            if(newUniformName[newUniformName.length()-2] == '0' &&
               newUniformName[newUniformName.length()-3] == '[')
            {
              //Erase the last three characters
              newUniformName.erase(newUniformName.length()-3, 3);
              LOGERR(("Nvidia/ATI uniform array workaround for %s",newUniformName.c_str()));
            }
            else
            {
              //Don't process this uniform
              ignoreUniform = true;
            }
          }
          //End Hack ======================================

          if(newUniformName.find("gl_") != 0 && !ignoreUniform)
          {
            //Loop for the array length
            for(GLint s=0; s<typeSize; s++)
            {
              string testUniformName = newUniformName;

              //If it is an array type, get each element one by one.
              if(s > 0)
              {
                //If it is an array type, get the index of each component
                string strBuffer;             
                StringPrintF(strBuffer, "[%u]",s);
                testUniformName += strBuffer;
              }

              //Get the data associated with the uniform in string form
              GetUniformValue(programData->GetGLID(),typeSize,type,testUniformName,uniformData);

              //Append the data to the log
              programData->GetShaderLog() += string("//  ") + testUniformName + 
                                             string("  =  ") + uniformData + string("\n");
            }
          }
        }

      }

      //Clean up the uniform name
      delete [] uniformName;
    }
  }

  //Set as dirty
  programData->SetDirty(true);

  //Save the program
  SaveShaderData(programData);

}

// Helper struct to get uniform data (assumes all uniform types are the same size)
struct GLUniformData
{
  union
  {
    GLfloat m_floatData;
    GLint   m_intData;
    GLuint  m_uintData;
  };
};
CASSERT(sizeof(GLUniformData) == sizeof(GLfloat) &&
        sizeof(GLfloat) == sizeof(GLint) &&
        sizeof(GLint) == sizeof(GLuint), __TypesNotSameSize__);

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShaderGLSL::GetUniformValue(GLuint progID,GLuint typeSize,GLenum type,const string &uniformName,string &retString)
{
  //Determine if the functions are available
  if(!iglGetUniformfv || !iglGetUniformiv)
  {
    retString = "<unknown>";
    return;    
  }

  retString = "";

  //Call GetUniformLocation  
  GLint uniLocation = iglGetUniformLocation(progID,(const GLchar*)uniformName.c_str());
  if(uniLocation < 0)
  {
    //If the uniform is unknown, return a unknown string
    retString = "<unknown>";
    return;
  }

  bool isSamplerType = false;
  uint numElements = 1;

  //Determine the type and size of the data to get
  switch(type)
  {
    case(GL_SAMPLER_1D):
    case(GL_SAMPLER_2D):
    case(GL_SAMPLER_3D):
    case(GL_SAMPLER_CUBE):
    case(GL_SAMPLER_1D_SHADOW):
    case(GL_SAMPLER_2D_SHADOW):
    case(GL_SAMPLER_2D_RECT):
    case(GL_SAMPLER_2D_RECT_SHADOW):

    case(GL_SAMPLER_1D_ARRAY):
    case(GL_SAMPLER_2D_ARRAY):
    case(GL_SAMPLER_1D_ARRAY_SHADOW):
    case(GL_SAMPLER_2D_ARRAY_SHADOW):
    case(GL_SAMPLER_CUBE_SHADOW):

    case(GL_SAMPLER_BUFFER):

    case(GL_INT_SAMPLER_1D):
    case(GL_INT_SAMPLER_2D):
    case(GL_INT_SAMPLER_3D):
    case(GL_INT_SAMPLER_CUBE):
    case(GL_INT_SAMPLER_1D_ARRAY):
    case(GL_INT_SAMPLER_2D_ARRAY):
    case(GL_INT_SAMPLER_2D_RECT):
    case(GL_INT_SAMPLER_BUFFER):

    case(GL_UNSIGNED_INT_SAMPLER_1D):
    case(GL_UNSIGNED_INT_SAMPLER_2D):
    case(GL_UNSIGNED_INT_SAMPLER_3D):
    case(GL_UNSIGNED_INT_SAMPLER_CUBE):
    case(GL_UNSIGNED_INT_SAMPLER_1D_ARRAY):
    case(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY):
    case(GL_UNSIGNED_INT_SAMPLER_2D_RECT):
    case(GL_UNSIGNED_INT_SAMPLER_BUFFER):

      isSamplerType = true;
      numElements = 1;
      break;

    case(GL_FLOAT): 
    case(GL_INT):
    case(GL_UNSIGNED_INT):
    case(GL_BOOL):
      numElements = 1;
      break;

    case(GL_FLOAT_VEC2):
    case(GL_INT_VEC2):
    case(GL_UNSIGNED_INT_VEC2):
    case(GL_BOOL_VEC2):
      numElements = 2;
      break;

    case(GL_FLOAT_VEC3):
    case(GL_INT_VEC3):
    case(GL_UNSIGNED_INT_VEC3):
    case(GL_BOOL_VEC3):
      numElements = 3;
      break;

    case(GL_FLOAT_VEC4):
    case(GL_INT_VEC4):
    case(GL_UNSIGNED_INT_VEC4):
    case(GL_BOOL_VEC4):
      numElements = 4;
      break;

    case(GL_FLOAT_MAT2):
      numElements = 4;
      break;

    case(GL_FLOAT_MAT3):
      numElements = 9;
      break;

    case(GL_FLOAT_MAT4):
      numElements = 16;
      break;

    case(GL_FLOAT_MAT2x4):
    case(GL_FLOAT_MAT4x2):
      numElements = 8;
      break;

    case(GL_FLOAT_MAT3x2):
    case(GL_FLOAT_MAT2x3):
      numElements = 6;
      break;

    case(GL_FLOAT_MAT3x4):
    case(GL_FLOAT_MAT4x3):
      numElements = 12;
      break;

    default:
      //Only enable this when debugging, new types may be added in future
      //LOGERR(("InterceptShaderGLSL::GetUniformValue - Unknown uniform type 0x%x",type));
      StringPrintF(retString,"<unknown type = 0x%x>",type);
      return;
  }

  //Determine the base GL type (assume float)
  GLuint baseType = GL_FLOAT;
  if(type == GL_INT || type == GL_INT_VEC2 || type == GL_INT_VEC3 || type == GL_INT_VEC4 ||
     type == GL_BOOL || type == GL_BOOL_VEC2 || type == GL_BOOL_VEC3 || type == GL_BOOL_VEC4 ||
     isSamplerType)
  {
    baseType = GL_INT;
  }

  if(type == GL_UNSIGNED_INT || type == GL_UNSIGNED_INT_VEC2 || type == GL_UNSIGNED_INT_VEC3 || type == GL_UNSIGNED_INT_VEC4)
  {
    baseType = GL_UNSIGNED_INT;
  }

  //Get the total uniform data size
  uint uniformDataSize = numElements;
  if(uniformDataSize == 0)
  {
    LOGERR(("InterceptShaderGLSL::GetUniformValue - Uniform is of zero length?"));
    retString = "<unknown>";
    return;
  }

  //If the data is a float format, get the uniform via floats
  // (Note: The array size is multiplied by typeSize(arraySize) as when 
  //        retrieving arrays, some implementations (ATI Cat5.9) return all 
  //        the elements in the array on each call to glGetUniform - 
  //        not just a single element)
  vector<GLUniformData> uniformData;
  uniformData.resize(uniformDataSize*typeSize);

  //Call apropiate GetUniform
  if(baseType == GL_FLOAT)
  {
    iglGetUniformfv(progID,uniLocation,&uniformData[0].m_floatData);
  }
  else if(baseType == GL_INT)
  {
    iglGetUniformiv(progID,uniLocation,&uniformData[0].m_intData);
  }
  else if(baseType == GL_UNSIGNED_INT && iglGetUniformuiv) // Note that iglGetUniformuiv is in OpenGL 3.0
  {
    iglGetUniformuiv(progID,uniLocation,&uniformData[0].m_uintData);
  }
  else
  {
    retString = "<unknown>";
    return;
  }

  //Add a beginning brace
  if(numElements > 1)
  {
    retString += "(";
  }

  //Loop for the number of elements in the type
  string buffer;
  for(uint i2=0;i2<numElements;i2++)
  {
    //Add the data to the return buffer
    if(baseType == GL_FLOAT)
    {
      StringPrintF(buffer,"%f",uniformData[i2].m_floatData);
    }
    else if(baseType == GL_INT)
    {
      StringPrintF(buffer,"%d",uniformData[i2].m_intData);
    }
    else if(baseType == GL_UNSIGNED_INT)
    {
      StringPrintF(buffer,"%u",uniformData[i2].m_uintData);
    }

    retString += buffer;

    //If the uniform was a sampler type, get the texture ID if possible
    if(isSamplerType && baseType == GL_INT)
    {
      //Get the lookup type of texture
      GLenum textureType=0;
      switch(type)
      {
        case(GL_SAMPLER_1D): 
        case(GL_SAMPLER_1D_SHADOW):
        case(GL_INT_SAMPLER_1D):
        case(GL_UNSIGNED_INT_SAMPLER_1D):
          textureType = GL_TEXTURE_1D;
          break;

        case(GL_SAMPLER_2D):
        case(GL_SAMPLER_2D_SHADOW):
        case(GL_INT_SAMPLER_2D):
        case(GL_UNSIGNED_INT_SAMPLER_2D):
          textureType = GL_TEXTURE_2D;
          break;

        case(GL_SAMPLER_3D): 
        case(GL_INT_SAMPLER_3D):
        case(GL_UNSIGNED_INT_SAMPLER_3D):
          textureType = GL_TEXTURE_3D;
          break;

        case(GL_SAMPLER_CUBE):
        case(GL_SAMPLER_CUBE_SHADOW):
        case(GL_INT_SAMPLER_CUBE):
        case(GL_UNSIGNED_INT_SAMPLER_CUBE):
          textureType = GL_TEXTURE_CUBE_MAP;
          break;

        case(GL_SAMPLER_2D_RECT):
        case(GL_SAMPLER_2D_RECT_SHADOW):
        case(GL_INT_SAMPLER_2D_RECT):
        case(GL_UNSIGNED_INT_SAMPLER_2D_RECT):
          textureType = GL_TEXTURE_RECTANGLE;
          break;

        case(GL_SAMPLER_1D_ARRAY):
        case(GL_SAMPLER_1D_ARRAY_SHADOW):
        case(GL_INT_SAMPLER_1D_ARRAY):
        case(GL_UNSIGNED_INT_SAMPLER_1D_ARRAY):
          textureType = GL_TEXTURE_1D_ARRAY;
          break;

        case(GL_SAMPLER_2D_ARRAY):
        case(GL_SAMPLER_2D_ARRAY_SHADOW):
        case(GL_INT_SAMPLER_2D_ARRAY):
        case(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY):
          textureType = GL_TEXTURE_2D_ARRAY;
          break;

        case(GL_SAMPLER_BUFFER):
        case(GL_INT_SAMPLER_BUFFER):
        case(GL_UNSIGNED_INT_SAMPLER_BUFFER):
          textureType = GL_TEXTURE_BUFFER;
          break;
      }

      //Lookup the texture bound at that stage for that type
      uint retTexID;
      if(driver->GetCurrentContext() && textureType !=0 &&
         driver->GetCurrentContext()->GetBoundTexture(textureType,uniformData[i2].m_intData,retTexID))
      {
        if(retTexID == 0)
        {
          retString += "<No Tex ID>";
        }
        else
        {
          StringPrintF(buffer,"<TexID = %u>",retTexID);
          retString += buffer;
        }
      }
    }

    //Add a comma if there are more elements to come
    if(i2<numElements-1)
    {
      retString += ",";
    }
  }

  //Add a closing brace
  if(numElements > 1)
  {
    retString += ")";
  }
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptShaderGLSL::GetShaderFileName(GLuint id,GLSLRequestType shaderType,string &retShaderName)
{

  //Check if the shader exists yet or not
  ShaderGLSLData * shaderData = NULL;
  if(shaderType == GLSLRT_GLSL_Program)
  {
    shaderData = shaderProgramManager.GetData(id);
  }
  else if(shaderType == GLSLRT_GLSL_Shader)
  {
    shaderData = shaderObjectManager.GetData(id);
  }
  else
  {
    //Default ARB case gets it from both
    shaderData = shaderProgramManager.GetData(id);
    if(!shaderData)
    {
      shaderData = shaderObjectManager.GetData(id);
    }
  }

  if(!shaderData)
  {
    LOGERR(("InterceptShaderGLSL::GetShaderFileName - Unknown or invalid shader? ID = %u",id));
    return false;
  }
  
  //If the shader is ready, save it
  if(shaderData->IsReady())
  {
    //Save the shader if dirty
    if(shaderData->IsDirty())
    {
      //If there is an error in saving, return now
      if(!SaveShaderData(shaderData))
      {
        return false;
      }
    }
  
    //Copy the file names
    retShaderName = shaderData->GetShaderSaveFileName();
    return true;
  }

  return false;
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptShaderGLSL::SaveShaderData(ShaderGLSLData *shaderData) 
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return false;
  }

  //If we are not saving shaders, return now
  if(!GetDataSaving())
  {
    return false;
  }

  //Ensure that the shader is ready
  if(!shaderData->IsReady())
  {
    LOGERR(("InterceptShaderGLSL::SaveShaderData - Shader has not been uploaded yet"));
    return false;
  }

  //Get the file base name
  string shaderName;
  if(!shaderData->GetUniqueFileName(shaderName))
  {
    return false;
  }

  //Append the shader's directory name
  shaderName = GetDataSavingPath() + shaderName;

  //Write the shader out
  string retFileName;
  if(!shaderSaver.SaveShader(shaderName,shaderData,retFileName))
  {
    LOGERR(("InterceptShaderGLSL::SaveShaderData - Error saving shader"));
  }

  //Assign the return file name
  shaderData->GetShaderSaveFileName() = retFileName;

  //Flag that the shader is no longer dirty 
  //   (set this reguardless or weither shader saving was successful)
  shaderData->SetDirty(false);

  return true;
}
