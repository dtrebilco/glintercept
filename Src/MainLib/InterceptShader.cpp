/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "InterceptShader.h"
#include "FileUtils.h"
#include "GLDriver.h"
#include <string.h>

//The real OpenGL driver
extern GLCoreDriver GLV;
extern WGLDriver    GLW;

USING_ERRORLOG


//Flags to indicate what action to take when we get an
//  shader function
enum ShaderFunctionAction
{
  SFA_UNKNOWN =0,

  SFA_PROGRAM_CREATE,
  SFA_PROGRAM_DELETE,
  SFA_PROGRAM_BIND,

  SFA_ARB_STR_READY,
};

enum ShaderFunctionType
{
  GL_GENPROGRAMS =0,
  GL_DELETEPROGRAMS,
  GL_BINDPROGRAM,

  //Loading of shader string
  GL_PROGRAM_STRING,
  GL_LOAD_PROGRAM_NV
};

// Structure to hold the name of functions that affect the shader state
struct ShaderLogData
{
  ShaderLogData(const char * funcName, ShaderFunctionType funcType, ShaderFunctionAction newAction);

  string               functionName;  //The name of the function
  ShaderFunctionAction action;        //What the function does and what action to take
  ShaderFunctionType   funcType;      //The type of the function (enum version of name)
  int                  functionIndex; //Index of the function (in the function table, -may be -1 if unknown)

};


//The array of built-in known functions
ShaderLogData knownShaderFunctions[] = 
{

  //General creation/deletion shaders
  //ShaderLogData("glGenPrograms"   ,GL_GENPROGRAMS   ,SFA_PROGRAM_CREATE),
  ShaderLogData("glGenProgramsARB",GL_GENPROGRAMS   ,SFA_PROGRAM_CREATE),
  ShaderLogData("glGenProgramsNV" ,GL_GENPROGRAMS   ,SFA_PROGRAM_CREATE),

  //ShaderLogData("glDeletePrograms"   ,GL_DELETEPROGRAMS,SFA_PROGRAM_DELETE),
  ShaderLogData("glDeleteProgramsARB",GL_DELETEPROGRAMS,SFA_PROGRAM_DELETE),
  ShaderLogData("glDeleteProgramsNV" ,GL_DELETEPROGRAMS,SFA_PROGRAM_DELETE),

  //ShaderLogData("glBindProgram"   ,GL_BINDPROGRAM   ,SFA_PROGRAM_BIND),
  ShaderLogData("glBindProgramARB",GL_BINDPROGRAM   ,SFA_PROGRAM_BIND),
  ShaderLogData("glBindProgramNV" ,GL_BINDPROGRAM   ,SFA_PROGRAM_BIND),

  //Loading of shader data
  //ShaderLogData("glProgramString"   ,GL_PROGRAM_STRING,SFA_ARB_STR_READY),
  ShaderLogData("glProgramStringARB",GL_PROGRAM_STRING,SFA_ARB_STR_READY),
  ShaderLogData("glLoadProgramNV"   ,GL_LOAD_PROGRAM_NV,SFA_ARB_STR_READY),

};



#define NUM_SHADER_LOG_FUNCTIONS sizeof(knownShaderFunctions)/sizeof(ShaderLogData)


///////////////////////////////////////////////////////////////////////////////
//
ShaderLogData::ShaderLogData(const char * funcName,ShaderFunctionType type, ShaderFunctionAction newAction):
functionName(funcName),
action(newAction),
funcType(type),
functionIndex(-1)
{
}


///////////////////////////////////////////////////////////////////////////////
//
InterceptShader::InterceptShader(GLDriver *ogldriver,FunctionTable * functionTable,const ConfigData &configData):
InterceptData(ogldriver),
shaderManager(),
shaderSaver(configData),
initialized(false),
initFailed(false),

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
iglGetProgramivARB(NULL)

{

  //Register the shader based functions with the function table
  for(uint i=0;i<NUM_SHADER_LOG_FUNCTIONS;i++)
  {
    //Set the flag
    functionTable->SetFunctionFlag(knownShaderFunctions[i].functionName,FDF_SHADER_LOG);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
InterceptShader::~InterceptShader()
{
  //Turn off shader saving to flush out-standing shaders
  if(GetDataSaving())
  {
    SetDataSaving(false);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptShader::Init()
{
  //Test if we can proceed
  if(!driver || !driver->GetInternalGLCallMode() || initFailed)
  {
    return false;
  }

  //Flag that init has been called
  initialized  = true;

  //Check extension support
  extensionARBVertexProgram   = driver->IsExtensionSupported("GL_ARB_vertex_program");
  extensionNVVertexProgram    = driver->IsExtensionSupported("GL_NV_vertex_program"); 
  extensionARBFragmentProgram = driver->IsExtensionSupported("GL_ARB_fragment_program"); 
  extensionNVFragmentProgram  = driver->IsExtensionSupported("GL_NV_fragment_program"); 


  //Init function pointers (Do OGL2.0 if ARB version becomes core)
  // (this can be done as these functions will only be called on valid programs)
  iglGetProgramivARB = (void (GLAPIENTRY *)(GLenum target, GLenum pname, GLint *params))GLW.glGetProcAddress("glGetProgramivARB");


  //Do not look up the NV version (takes different parameters)

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool InterceptShader::DriverAvailable()
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
void InterceptShader::SetDataSaving(bool flag)
{
  //Assign the flag by calling the base class
  InterceptData::SetDataSaving(flag);

  //If enabling shader writing, save all currently bound shaders
  if(flag && initialized && DriverAvailable())
  {
    //Get all the dirty shaders
    vector<ShaderData *> dirtyShaders;
    shaderManager.GetAllDirtyData(dirtyShaders);
     
    //Loop for all dirty shaders
    for(uint i=0;i<dirtyShaders.size();i++)
    {
      //If shader is bound and dirty write it out
      if(dirtyShaders[i]->GetGLID() !=0 && 
         GetActiveShaderID(dirtyShaders[i]->GetGLType()) == dirtyShaders[i]->GetGLID())
      {
        SaveShaderData(dirtyShaders[i]);
      }
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShader::SetDataSavingPath(const string &newPath)
{
  //Append the Shaders directory then pass to the base class
  string path = newPath + "Shaders" + FileUtils::dirSeparator;

  //Call base class to set
  InterceptData::SetDataSavingPath(path);
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShader::LogFunctionPre(const FunctionData *funcData,uint index, const FunctionArgs & args)
{
  //Return if GL calls cannot be made 
  if(!DriverAvailable())
  {
    return;
  }

  //If this is not an shader function, return now
  if(!funcData->IsFlagSet(FDF_SHADER_LOG))
  {
    return;
  }

  //Get the action required for the function
  int actionIndex = GetShaderActionIndex(funcData->GetName(),index);
  if(actionIndex == -1)
  {
    return;
  }
  ShaderFunctionAction action = knownShaderFunctions[actionIndex].action;

  //Get a copy of the arguments
  FunctionArgs newArgs(args);

  //Perform the action
  switch(action)
  {
    case(SFA_PROGRAM_CREATE) :
      CreateARBShadersPre(funcData,index,newArgs);
      break;
    case(SFA_PROGRAM_DELETE) :
      DeleteARBShaders(funcData,index,newArgs);
      break;
    case(SFA_PROGRAM_BIND) :
      BindARBShaderPre(funcData,index,newArgs);
      break;
    case(SFA_ARB_STR_READY) :
      SetARBShaderReadyPre(funcData,index,newArgs,knownShaderFunctions[actionIndex].funcType);
      break;

    default:
       LOGERR(("InterceptShader::LogFunctionPre - Unknown action"));
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShader::LogFunctionPost(const FunctionData *funcData,uint index, const FunctionRetValue & retVal)
{
  //Return if GL calls cannot be made
  if(!funcData->IsFlagSet(FDF_SHADER_LOG) || !DriverAvailable())
  {
    return;
  }

  //Get the action required for the function
  int actionIndex = GetShaderActionIndex(funcData->GetName(),index);
  if(actionIndex == -1)
  {
    return;
  }
  ShaderFunctionAction action = knownShaderFunctions[actionIndex].action;

  //Perform the action
  switch(action)
  {
    case(SFA_PROGRAM_CREATE) :
      CreateARBShadersPost(funcData,index);
      break;
    case(SFA_PROGRAM_DELETE) :
      break;
    case(SFA_PROGRAM_BIND) :
      break;
    case(SFA_ARB_STR_READY) :
      SetARBShaderReadyPost(funcData,index);
      break;

    default:
       LOGERR(("InterceptShader::LogFunctionPost - Unknown action"));
  }

}
  

///////////////////////////////////////////////////////////////////////////////
//
int InterceptShader::GetShaderActionIndex(const string &functionName, int funcIndex)
{
  uint i;

  //Check the list to find the index 
  for(i=0;i<NUM_SHADER_LOG_FUNCTIONS;i++)
  {
    //If the index is the same, we have mapped this function previously 
    if(knownShaderFunctions[i].functionIndex == funcIndex)
    {
      return i;
    }
  }

  //If this function has not been called previously, find the name and assign the index
  for(i=0;i<NUM_SHADER_LOG_FUNCTIONS;i++)
  {
    //If the name is equal, assign the index
    if(knownShaderFunctions[i].functionName == functionName)
    {
      knownShaderFunctions[i].functionIndex = funcIndex;
      return i;
    }

  }

  //If the function is not found, something has broken with the mapping/registery
  LOGERR(("InterceptShader::GetShaderActionIndex - Attempting to log unknown function %s",functionName.c_str()));

  return -1;
}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptShader::CreateARBShadersPre(const FunctionData *funcData,uint index, FunctionArgs & args)
{
  //Get the number of shader ID's created
  args.Get(inumNewShaders);

  //Get the shader array
  args.Get(inewShaderIDs);
}

///////////////////////////////////////////////////////////////////////////////
//
void InterceptShader::CreateARBShadersPost(const FunctionData *funcData,uint index)
{
  //Check the array
  if(inewShaderIDs == NULL || inumNewShaders < 0)
  {
    LOGERR(("InterceptShader::CreateARBShadersPost - Invalid shader ID array"));
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
void InterceptShader::BindARBShaderPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args)
{
  //Get the target type
  GLenum newType;
  args.Get(newType);

  //Get the shader id
  GLuint shaderID;
  args.Get(shaderID);

  //Check if the shader exists yet or not
  ShaderData * shaderData = shaderManager.GetData(shaderID);
  if(!shaderData)
  {
    //Add the shader data to the array
    shaderManager.AddData(shaderID);
  }
  else
  {
    //If the shader exists and is ready and dirty, save it out
    if(shaderData->IsReady() && shaderData->IsDirty())
    {
      SaveShaderData(shaderData);
    }

    //If the shader is ready, ensure it was bound to the correct target
    if(shaderData->IsReady() && newType != shaderData->GetGLType())
    {
      LOGERR(("InterceptShader::BindARBShaderPre - Binding shader %u to a different target?",shaderID)); 
    }
  }
}



///////////////////////////////////////////////////////////////////////////////
//
void InterceptShader::DeleteARBShaders(const FunctionData *funcData,uint index, FunctionArgs & args)
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
    //Attempt to get the shader ID and save the shader if it is dirty
    ShaderData * shaderData = shaderManager.GetData(shaderIDs[i]);
    if(shaderData && shaderData->IsReady() && shaderData->IsDirty())
    {
      //Save the dirty shader before it is deleted
      SaveShaderData(shaderData);
    }

    //Remove the shader ID
    shaderManager.RemoveData(shaderIDs[i]);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void InterceptShader::SetARBShaderReadyPre(const FunctionData *funcData,uint funcTableIndex, FunctionArgs & args,uint funcType)
{
  //Reset the flag
  validShaderReady = false;

  if(funcType == GL_PROGRAM_STRING)
  {
    //Get the target
    args.Get(ishaderTarget);

    //Get the current shader index for the target
    ishaderID = GetActiveShaderID(ishaderTarget);

    //Get the format
    GLenum shaderFormat;
    args.Get(shaderFormat);

    //Check the format
    if(shaderFormat != GL_PROGRAM_FORMAT_ASCII_ARB)
    {
      LOGERR(("InterceptShader::SetARBShaderReadyPre - Need to pass ascii string for logging: ShaderID = %u",ishaderID));
      return;
    }

    //Get the length
    args.Get(ishaderLength);

    //Get the string
    args.Get(ishaderSrc);

  }
  else if(funcType == GL_LOAD_PROGRAM_NV)
  {
    //Get the target
    args.Get(ishaderTarget);

    //Get the id
    args.Get(ishaderID);

    //NV state programs do not need a bind call so 
    //  check if the shader exists yet or not
    ShaderData * shaderData = shaderManager.GetData(ishaderID);
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
    LOGERR(("InterceptShader::SetARBShaderReadyPre - Unhandled program type"));
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
void InterceptShader::SetARBShaderReadyPost(const FunctionData *funcData,uint funcTableIndex)
{
  //Return if GL calls cannot be made
  if(!DriverAvailable() || !validShaderReady)
  {
    return;
  }

  //Note: Both ARB and NV programs call this code. (don't make extension specific calls)

  //Get the data for the shader
  ShaderData * shaderData = shaderManager.GetData(ishaderID);
  if(!shaderData)
  {
    LOGERR(("InterceptShader::SetARBShaderReadyPost - Unknown or invalid shader? ShaderID = %u",ishaderID));
    return;
  }

  //If the shader exists and is ready and dirty, save it out
  if(shaderData->IsReady() && shaderData->IsDirty())
  {
    SaveShaderData(shaderData);
  }

  //Check for type changes
  if(shaderData->IsReady() && ishaderTarget != shaderData->GetGLType())
  {
    LOGERR(("InterceptShader::SetARBShaderReadyPost - Assigning new shader type to ID = %u?",ishaderID));
  }

  //Init the shader data variables
  shaderData->SetReady(ishaderTarget);

  //Set the shader string
  char * shaderCharSrc = new char[ishaderLength+1];
  memcpy(shaderCharSrc,ishaderSrc,ishaderLength*sizeof(char));

  //Append a NULL character
  shaderCharSrc[ishaderLength] = '\0';

  //Assign the array to the source string 
  shaderData->GetShaderSource() = shaderCharSrc;

  //Delete the old array
  delete [] shaderCharSrc;

  //Save the shader data out
  SaveShaderData(shaderData);
  
  //Reset the flag
  validShaderReady = false;
}




///////////////////////////////////////////////////////////////////////////////
//
void InterceptShader::SaveAllDirtyData() 
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return;
  }

  vector<ShaderData *> dirtyShaders;

  //Get all the dirty shaders
  shaderManager.GetAllDirtyData(dirtyShaders);

  //Loop for all shaders
  for(uint i=0;i<dirtyShaders.size();i++)
  {
    //Save the shader
    SaveShaderData(dirtyShaders[i]);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
uint InterceptShader::GetActiveShaderID(GLenum oglTarget)
{
  int retVal = 0;

  switch(oglTarget)
  {
    //Note: GL_VERTEX_PROGRAM_NV is the same enum
    case(GL_VERTEX_PROGRAM_ARB) :
    case(GL_FRAGMENT_PROGRAM_ARB) :
      {
        //Check for ARB support
        if(iglGetProgramivARB != NULL)
        {
          iglGetProgramivARB(oglTarget, GL_PROGRAM_BINDING_ARB, &retVal);
        }
        //If no ARB support, fall back to old NV way
        else if(oglTarget == GL_VERTEX_PROGRAM_NV)
        {
          GLV.glGetIntegerv(GL_VERTEX_PROGRAM_BINDING_NV,&retVal);
        }
      }
      break;

    case(GL_FRAGMENT_PROGRAM_NV) :
      GLV.glGetIntegerv(GL_FRAGMENT_PROGRAM_BINDING_NV,&retVal);
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
void InterceptShader::GetBoundShaders(uint &vertexShaderID,uint &fragmentShaderID)
{
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
      LOGERR(("InterceptShader::GetBoundShaders - Two vertex progarms bound at once?"));
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
      LOGERR(("InterceptShader::GetBoundShaders - Two fragment progarms bound at once?"));
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
      LOGERR(("InterceptShader::GetBoundShaders - Two fragment progarms bound at once?"));
    }

    //Assign the return value
    fragmentShaderID = tmpID;
  }
}


///////////////////////////////////////////////////////////////////////////////
//
bool InterceptShader::GetShaderFileName(GLuint id,string &retShaderName)
{
  //Check if the shader exists yet or not
  ShaderData * shaderData = shaderManager.GetData(id);
  if(!shaderData)
  {
    LOGERR(("InterceptShader::GetShaderFileName - Unknown or invalid shader? ID = %u",id));
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
bool InterceptShader::SaveShaderData(ShaderData *shaderData) 
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
    LOGERR(("InterceptShader::SaveShaderData - Shader has not been uploaded yet"));
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
    LOGERR(("InterceptShader::SaveShaderData - Error saving shader"));
  }

  //Assign the return file name
  shaderData->GetShaderSaveFileName() = retFileName;

  //Flag that the shader is no longer dirty 
  //   (set this reguardless or weither shader saving was successful)
  shaderData->SetDirty(false);

  return true;
}
