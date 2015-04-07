/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "ShaderEditGLSL.h"
#include "MiscUtils.h"
#include "SubstituteShaderGLSL.h"
#include "SEContext.h"
#include <CommonErrorLog.h>

USING_ERRORLOG

//The call back ids
enum CallBackIDs
{
  CBI_glCreateShaderObject = SHADER_GLSL_CALLBACK_INDEX,
  CBI_glCreateProgramObject,

  CBI_glDeleteObject,
  CBI_glDeleteShader,
  CBI_glDeleteProgram,

  CBI_glShaderSource,
  CBI_glCompileShader,

  CBI_glAttachObject,
  CBI_glDetachObject,

  CBI_glLinkProgram,

  CBI_glUseProgramObject


#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

  ,CBI_glUniform1i
  ,CBI_glUniform1iv

#endif  //GLI_ATI_UNIFORM_GLSL_BUG

};

///////////////////////////////////////////////////////////////////////////////
//
ShaderEditGLSL::AttributeData::AttributeData():
size(0),
type(0),
isGLPrefix(false)
{
}


///////////////////////////////////////////////////////////////////////////////
//
ShaderEditGLSL::ShaderEditGLSL(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),
GLV(callBacks->GetCoreGLFunctions()),

//Initernal variable init
iObjectType(0),
iHandle(0),
iHandleDetach(0),

iglGetProgramiv(NULL),
iglGetShaderiv(NULL),
iglValidateProgram(NULL),
iglGetProgramInfoLog(NULL),
iglGetShaderInfoLog(NULL),
iglGetActiveUniform(NULL),
iglGetUniformLocation(NULL),
iglCreateProgram(NULL),
iglCreateShader(NULL),
iglDeleteProgram(NULL),
iglDeleteShader(NULL),
iglShaderSource(NULL),
iglCompileShader(NULL),
iglLinkProgram(NULL),
iglAttachShader(NULL),
iglDetachShader(NULL),
iglBindAttribLocation(NULL),
iglGetActiveAttrib(NULL),
iglGetAttribLocation(NULL)
{

}

///////////////////////////////////////////////////////////////////////////////
//
ShaderEditGLSL::~ShaderEditGLSL()
{
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditGLSL::Init()
{
  //Return if we cannot make OpenGL calls
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return false;
  }

  //Test extension support
  float glVersion = gliCallBacks->GetGLVersion();
  if(glVersion < 2.0f)
  {
    LOGERR(("ShaderEditGLSL::Init - Need OpenGL2.0 implementation to use the GLSL shader editor"));
    return false;
  }
 

  void **loadFunc;

  //Helper define for function lookups

#define GL_FUNC_LOOKUP(string)                           \
  loadFunc  = (void**)(&i##string);                      \
  *loadFunc = gliCallBacks->GetGLFunction(#string); \
  if(*loadFunc == NULL)                                  \
  {                                                      \
    LOGERR(("Function %s unable to be mapped",#string)); \
    return false;                                        \
  }                                                      

  //Init function pointers
  GL_FUNC_LOOKUP(glGetProgramiv);
  GL_FUNC_LOOKUP(glGetShaderiv);
  GL_FUNC_LOOKUP(glValidateProgram);
  GL_FUNC_LOOKUP(glGetProgramInfoLog);
  GL_FUNC_LOOKUP(glGetShaderInfoLog);
  GL_FUNC_LOOKUP(glGetActiveUniform);
  GL_FUNC_LOOKUP(glGetUniformLocation);
  GL_FUNC_LOOKUP(glCreateProgram);
  GL_FUNC_LOOKUP(glCreateShader);
  GL_FUNC_LOOKUP(glDeleteProgram);
  GL_FUNC_LOOKUP(glDeleteShader);
  GL_FUNC_LOOKUP(glShaderSource);
  GL_FUNC_LOOKUP(glCompileShader);
  GL_FUNC_LOOKUP(glLinkProgram);
  GL_FUNC_LOOKUP(glAttachShader);
  GL_FUNC_LOOKUP(glDetachShader);
  GL_FUNC_LOOKUP(glBindAttribLocation);
  GL_FUNC_LOOKUP(glGetActiveAttrib);
  GL_FUNC_LOOKUP(glGetAttribLocation);

#undef GL_FUNC_LOOKUP

 

#define FUNC_REGISTER(function, token)        \
  gliCallBacks->RegisterGLFunction(function); \
  gliCallBacks->SetFunctionID(function, token); 

  FUNC_REGISTER("glCreateShader",CBI_glCreateShaderObject);
  FUNC_REGISTER("glCreateShaderObjectARB",CBI_glCreateShaderObject);

  FUNC_REGISTER("glCreateProgram",CBI_glCreateProgramObject);
  FUNC_REGISTER("glCreateProgramObjectARB",CBI_glCreateProgramObject);

  FUNC_REGISTER("glDeleteProgram",CBI_glDeleteProgram);
  FUNC_REGISTER("glDeleteShader",CBI_glDeleteShader);
  FUNC_REGISTER("glDeleteObjectARB",CBI_glDeleteObject);

  FUNC_REGISTER("glShaderSource",CBI_glShaderSource);
  FUNC_REGISTER("glShaderSourceARB",CBI_glShaderSource);
  
  FUNC_REGISTER("glCompileShader",CBI_glCompileShader);
  FUNC_REGISTER("glCompileShaderARB",CBI_glCompileShader);

  FUNC_REGISTER("glAttachShader",CBI_glAttachObject);
  FUNC_REGISTER("glAttachObjectARB",CBI_glAttachObject);
  
  FUNC_REGISTER("glDetachShader",CBI_glDetachObject);  
  FUNC_REGISTER("glDetachObjectARB",CBI_glDetachObject);

  FUNC_REGISTER("glLinkProgram",CBI_glLinkProgram);
  FUNC_REGISTER("glLinkProgramARB",CBI_glLinkProgram);

  FUNC_REGISTER("glUseProgram",CBI_glUseProgramObject);
  FUNC_REGISTER("glUseProgramObjectARB",CBI_glUseProgramObject);


#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

  FUNC_REGISTER("glUniform1i",CBI_glUniform1i);
  FUNC_REGISTER("glUniform1iARB",CBI_glUniform1i);

  FUNC_REGISTER("glUniform1iv",CBI_glUniform1iv);
  FUNC_REGISTER("glUniform1ivARB",CBI_glUniform1iv);

#endif  //GLI_ATI_UNIFORM_GLSL_BUG


#undef FUNC_REGISTER


  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
ShaderEditGLSL *ShaderEditGLSL::CreateInstance(InterceptPluginCallbacks *callBacks)
{
  //Create the new shader edit
  ShaderEditGLSL * newShaderEdit = new ShaderEditGLSL(callBacks);

  //Attempt to init
  if(!newShaderEdit->Init())
  {
    LOGERR(("ShaderEditGLSL::CreateInstance - Unable to init new GLSL shader editor"));
    newShaderEdit->ReleaseReference();
    return NULL;
  }

  return newShaderEdit;
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditGLSL::DriverAvailable() const
{
  return gliCallBacks->GetGLInternalCallState();
}


///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::LogFunctionPre(uint updateID, const char *funcName, uint funcIndex, const FunctionArgs & args, SEContext * currContext)
{
  //Return if GL calls cannot be made 
  if(!DriverAvailable() || currContext == NULL)
  {
    return;
  }

  //Create a access copy of the arguments
  FunctionArgs newArgs(args);

  //Perform the action
  switch(updateID)
  {
    case(CBI_glCreateShaderObject) :
    case(CBI_glCreateProgramObject) :
      CreateObjectPre(updateID,newArgs);
      break;

    case(CBI_glDeleteObject) :
    case(CBI_glDeleteShader) :
    case(CBI_glDeleteProgram):
      DeleteObjectPre(newArgs);
      break;

    case(CBI_glShaderSource) :
      ShaderSourcePre(newArgs);
      break;
    case(CBI_glCompileShader) :
      break;

    case(CBI_glAttachObject) :
      AttachObjectPre(newArgs);
      break;
    case(CBI_glDetachObject) :
      DetachObjectPre(newArgs);
      break;

    case(CBI_glLinkProgram) :
      LinkProgramPre(newArgs);
      break;
    case(CBI_glUseProgramObject) :
      UseProgramPre(newArgs,currContext);
      break;
    
#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

    case(CBI_glUniform1i) :
    case(CBI_glUniform1iv) :
      Uniform1iPre(newArgs, currContext, updateID);
      break;

#endif  //GLI_ATI_UNIFORM_GLSL_BUG

    default:
       LOGERR(("ShaderEditGLSL::LogFunctionPre - Unknown update ID"));
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::LogFunctionPost(uint updateID, const char *funcName, uint funcIndex, const FunctionRetValue & retVal, SEContext * currContext)
{
  //Return if GL calls cannot be made 
  if(!DriverAvailable() || currContext == NULL)
  {
    return;
  }

  //Perform the action
  switch(updateID)
  {
    case(CBI_glCreateShaderObject) :
    case(CBI_glCreateProgramObject) :
      CreateObjectPost(retVal);
      break;

    case(CBI_glDeleteObject) :
    case(CBI_glDeleteShader) :
    case(CBI_glDeleteProgram):
      DeleteObjectPost(updateID);
      break;

    case(CBI_glShaderSource) :
      break;
    case(CBI_glCompileShader) :
      break;

    case(CBI_glAttachObject) :
      break;
    case(CBI_glDetachObject) :
      DetachObjectPost();
      break;

    case(CBI_glLinkProgram) :
      LinkProgramPost();
      break;
    case(CBI_glUseProgramObject) :
      break;

#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

    case(CBI_glUniform1i) :
    case(CBI_glUniform1iv) :
      break;

#endif  //GLI_ATI_UNIFORM_GLSL_BUG

    default:
       LOGERR(("ShaderEditGLSL::LogFunctionPost - Unknown updateID"));
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::CreateObjectPre(uint updateID, FunctionArgs & args)
{
  //If the function is a create shader object, get the shader type
  if(updateID == CBI_glCreateShaderObject)
  {
    //Get the number of shader ID's created
    args.Get(iObjectType);
  }
  else if(updateID == CBI_glCreateProgramObject)
  {
    //If it is a program object, just assign the type
    iObjectType = GL_PROGRAM_OBJECT_ARB;
  }
  else
  {
    //Unknown function type
    LOGERR(("ShaderEditGLSL::CreateObjectPre - Unknown function?"));
    return;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::CreateObjectPost(const FunctionRetValue & retVal)
{

  //Check the type
  if(iObjectType != GL_PROGRAM_OBJECT_ARB &&
     iObjectType != GL_VERTEX_SHADER  && 
     iObjectType != GL_GEOMETRY_SHADER  && 
     iObjectType != GL_FRAGMENT_SHADER)
  {
    LOGERR(("ShaderEditGLSL::CreateObjectPost - Unknown object type 0x%x",iObjectType));
    return;
  }

  //Get the returned handle
  GLuint newHandle;
  retVal.Get(newHandle);

  //Add as a shader program or shader object 
  if(iObjectType == GL_PROGRAM_OBJECT_ARB)
  {
    //Check for an existing object
    ShaderDataGLSL * shaderData = shaderProgramManager.GetData(newHandle);
    if(shaderData)
    {
      LOGERR(("ShaderEditGLSL::CreateObjectPost - Existing handle ID %u",newHandle));
      return;
    }

    //Create a new shader program
    shaderProgramManager.AddData(newHandle);

    //Get a pointer to the new object
    shaderData = shaderProgramManager.GetData(newHandle);
    if(!shaderData)
    {
      LOGERR(("ShaderEditGLSL::CreateObjectPost - Unable to create shader ID %u",newHandle));
      return;
    }

    //Assign the object type (assigned as ready where there is some data to save)
    shaderData->SetGLType(iObjectType);
  }
  else
  {
    //Check for an existing object
    ShaderDataGLSL * shaderData = shaderObjectManager.GetData(newHandle);
    if(shaderData)
    {
      LOGERR(("ShaderEditGLSL::CreateObjectPost - Existing handle ID %u",newHandle));
      return;
    }

    //Create a new shader object
    shaderObjectManager.AddData(newHandle);

    //Get a pointer to the new object
    shaderData = shaderObjectManager.GetData(newHandle);
    if(!shaderData)
    {
      LOGERR(("ShaderEditGLSL::CreateObjectPost - Unable to create shader ID %u",newHandle));
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
void ShaderEditGLSL::DeleteObjectPre(FunctionArgs & args)
{
  //Get the handle
  args.Get(iHandle);
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::DeleteObjectPost(uint updateID)
{
  //Get the handle and save the data if necessary
  ShaderDataGLSL * shaderData = NULL;
  if(updateID == CBI_glDeleteProgram)
  {
    shaderData = shaderProgramManager.GetData(iHandle);
  }
  else if(updateID == CBI_glDeleteShader)
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
    LOGERR(("ShaderEditGLSL::DeleteObjectPost - Unable to find shader ID %u",iHandle));
    return;
  }

  //If it is a program object, detach all references
  // NOTE: This is not technically accurate as the program objects are not deleted fully until
  //       they are no longer bound in any context, however, keeping tack of program binding states
  //       for all contexts will be difficult for this rare case, so it is not done.
  if(shaderData->GetGLType() == GL_PROGRAM_OBJECT_ARB)
  {

    //Flag to any possible substitute to delete itself (not technically accurate but safer)
    SubShaderGLSLRef subProgram = shaderData->GetUniqueID()->GetSubstituteShader();
    if(subProgram)
    {
      subProgram->DestroyProgram();
    }

    //Get a copy of the attached objects
    vector<GLuint> attachArray = shaderData->GetAttachmentArray();

    //Delete the program object
    shaderProgramManager.RemoveData(iHandle);
    shaderData = NULL;

    //Loop for all attachments
    for(uint i=0;i<attachArray.size();i++)
    {
      ShaderDataGLSL * attachData = shaderObjectManager.GetData(attachArray[i]);
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
        LOGERR(("ShaderEditGLSL::DeleteObjectPre - Unknown attachment %u",attachArray[i]));
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
void ShaderEditGLSL::ShaderSourcePre(FunctionArgs & args)
{
  //Get the handle
  GLuint srcHandle;
  args.Get(srcHandle);

  //Get number of strings
  GLsizei numStrings;
  args.Get(numStrings);

  //Get the string array
  void * dummyRead;
  args.Get(dummyRead);
  GLchar **strArray = (GLchar **)dummyRead; 

  //Get the string length array
  const GLint * strLengthArray;
  args.Get(strLengthArray);

  //Lookup the shader
  ShaderDataGLSL * shaderData = shaderObjectManager.GetData(srcHandle);
  if(!shaderData || shaderData->GetGLType() == GL_PROGRAM_OBJECT_ARB ||
      shaderData->GetUserDeleted())
  {
    LOGERR(("ShaderEditGLSL::ShaderSourcePre - Unable to find shader ID %u",srcHandle));
    return;
  }

  //Check data
  if(numStrings <= 0 || strArray == NULL)
  {
    LOGERR(("ShaderEditGLSL::ShaderSourcePre - Bad data passed as source to shader ID %u",srcHandle));
    return;
  }

  string shaderSrc;

  //Loop for the number of strings
  uint i = 0;
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
      //Append the string using the count
      shaderSrc.append((char*)strArray[i], strLengthArray[i]);
    }
  }

  //Remove any "return" characters from the text
  string shaderFormat=shaderSrc;
  shaderSrc = "";
  shaderSrc.reserve(shaderFormat.size());
  for(i=0; i<shaderFormat.size(); i++)
  {
    if(shaderFormat[i] != '\r' && 
       shaderFormat[i] != EOF && 
       shaderFormat[i] != '\0')
    {
      shaderSrc += shaderFormat[i];
    }
  }

  //Assign the shader source
  shaderData->SetShaderSource(shaderSrc);

  //Set the shader as ready and dirty
  shaderData->SetReady();
}


///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::AttachObjectPre(FunctionArgs & args)
{
  //Get the program
  GLuint programHandle;
  args.Get(programHandle);

  //Get the shader
  GLuint shaderHandle;
  args.Get(shaderHandle);

  //Look for the program and the shader
  ShaderDataGLSL * programData = shaderProgramManager.GetData(programHandle);
  ShaderDataGLSL * shaderData  = shaderObjectManager.GetData(shaderHandle);
  if(!shaderData || !programData || shaderData == programData ||
      programData->GetGLType() != GL_PROGRAM_OBJECT_ARB ||
      shaderData->GetGLType()  == GL_PROGRAM_OBJECT_ARB ||
      shaderData->GetUserDeleted())
  {
    LOGERR(("ShaderEditGLSL::AttachObjectPre - Invalid ID's %u,%u",programHandle,shaderHandle));
    return;
  }

  //Add the handle to each other
  programData->AttachObject(shaderHandle);
  shaderData->AttachObject(programHandle);
}


///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::DetachObjectPre(FunctionArgs & args)
{
  //Get the program
  args.Get(iHandle);

  //Get the shader
  args.Get(iHandleDetach);
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::DetachObjectPost()
{
  //Assign the working handles
  GLuint programHandle = iHandle;
  GLuint shaderHandle  = iHandleDetach;

  //Test for zero handles
  if(programHandle == 0 || shaderHandle == 0)
  {
    LOGERR(("ShaderEditGLSL::DetachObjectPost - Invalid ID's %u,%u",programHandle,shaderHandle));
    return;
  }

  //Look for the program and the shader
  // (Note: You can detach a user deleted object)
  ShaderDataGLSL * programData = shaderProgramManager.GetData(programHandle);
  ShaderDataGLSL * shaderData  = shaderObjectManager.GetData(shaderHandle);
  if(!shaderData || !programData || shaderData == programData ||
      programData->GetGLType() != GL_PROGRAM_OBJECT_ARB ||
      shaderData->GetGLType()  == GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("ShaderEditGLSL::DetachObjectPost - Invalid ID's %u,%u",programHandle,shaderHandle));
    return;
  }

  //Remove the handle from each other
  if(!programData->DetachObject(shaderHandle))
  {
    LOGERR(("ShaderEditGLSL::DetachObjectPost - Error detaching shader from program %u,%u",programHandle,shaderHandle));
  }
  if(!shaderData->DetachObject(programHandle))
  {
    LOGERR(("ShaderEditGLSL::DetachObjectPost - Error detaching program from shader %u,%u",programHandle,shaderHandle));
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
void ShaderEditGLSL::LinkProgramPre(FunctionArgs & args)
{
  //Get the program
  GLuint programHandle;
  args.Get(programHandle);

  //Look for the program 
  ShaderDataGLSL * programData = shaderProgramManager.GetData(programHandle);
  if(!programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("ShaderEditGLSL::LinkProgramPre - Invalid ID %u",programHandle));
    return;
  }
  
  // On successful link, objects can be attached /detached etc without affecting the
  //  internal program -so save the source strings at the time on linking

  //Get the array of associated shaders
  const vector<GLuint> & attachedObjects = programData->GetAttachmentArray();

  //Loop for all attachments
  vector<ShaderUtilsGLSL::ShaderStrData> shaderStrArray;
  for(uint i=0;i<attachedObjects.size();i++)
  {
    //Get the attachment
    const ShaderDataGLSL * attachData = shaderObjectManager.GetData(attachedObjects[i]);
    if(!attachData)
    {
      LOGERR(("ShaderEditGLSL::LinkProgramPre - Unable to find shader ID %u",attachedObjects[i]));
    }
    else
    {
      // Append a comment indicating the shader ID to the string
      string idString;
      StringPrintF(idString,"// Shader OpenGL ID %u\n", attachedObjects[i]);

      //Save the shaders 
      ShaderUtilsGLSL::ShaderStrData newData(attachData->GetGLType(), idString + attachData->GetUniqueID()->GetShaderSource());
      shaderStrArray.push_back(newData);
    }
  }

  //Assign the program source
  string programSource;
  ShaderUtilsGLSL::AppendShaderStrings(shaderStrArray, programSource);

  //This will reset any substitute programs
  programData->SetShaderSource(programSource);
          
  //Assign the handle to use in the Post Linking version
  iHandle = programHandle;
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::LinkProgramPost()
{
  //Look for the program 
  ShaderDataGLSL * programData = shaderProgramManager.GetData(iHandle);
  if(iHandle == 0 || !programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("ShaderEditGLSL::LinkProgramPost - Invalid ID %u",iHandle));
    return;
  }

  //Get the link status to determine if the program is "ready"
  GLint linkStatus;
  iglGetProgramiv(programData->GetGLID(), GL_LINK_STATUS, &linkStatus);
  if(linkStatus)
  {
    //Flag the program object as ready
    programData->SetReady();
  }
  else
  {
    LOGERR(("ShaderEditGLSL::LinkProgramPost - Shader program %u failed to link",iHandle));
  }

  //Reset the handle
  iHandle = 0;
}


///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::UseProgramPre(FunctionArgs & args, SEContext * currContext)
{
  //Get the program
  GLuint programHandle;
  args.Get(programHandle);
  if(programHandle == 0)
  {
    //Set the unique ID to NULL in the the current context
    currContext->SetBoundGLSLShader(UniqueShaderIDRef());
    return;
  }

  //Look for the program 
  ShaderDataGLSL * programData = shaderProgramManager.GetData(programHandle);
  if(!programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("ShaderEditGLSL::UseProgramPre - Invalid ID %u",programHandle));
    return;
  }

  //Set the bound shader
  currContext->SetBoundGLSLShader(programData->GetUniqueID());

  //Update the frame number
  programData->GetUniqueID()->SetLastFrameUsed(gliCallBacks->GetFrameNumber());

}


#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::Uniform1iPre(FunctionArgs & args, SEContext * currContext, uint updateID)
{
  //Get the initial location
  GLint location;
  args.Get(location);

  //Return now if the uniform location is negative
  if(location < 0)
  {
    return;
  }

  //Get the active shader
  const UniqueShaderIDRef & boundShader = currContext->GetBoundGLSLShader();
  if(!boundShader)
  {
    LOGERR(("Uniform1iPre - Calling glUniform when a shader is not bound?"));
    return;
  }

  //Get the internal program pointer
  ShaderDataGLSL * programData = shaderProgramManager.GetData(boundShader->GetShaderOpenGLID());
  if(!programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("ShaderEditGLSL::Uniform1iPre - Invalid ID %u",boundShader->GetShaderOpenGLID()));
    return;
  }

  //Get the data
  if(updateID == CBI_glUniform1i)
  {
    GLint uniformData;
    args.Get(uniformData);

    //Assign in the override
    programData->uniformBugManager->SetUniformData(location, uniformData);
  }
  else if(updateID == CBI_glUniform1i)
  {
    GLsizei numUniforms;
    args.Get(numUniforms);

    const GLint * uniformArray;
    args.Get(uniformArray);

    //If there are some uniforms in the array
    if(numUniforms > 0)
    {
      //Only store the first value - most samplers (which is what this hack is for) are only a single value.
      programData->uniformBugManager->SetUniformData(location, uniformArray[0]); 
    }
  }

}

#endif  //GLI_ATI_UNIFORM_GLSL_BUG



///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::GetBoundProgram(GLuint &programID)
{
  //Set initial return values
  programID  = 0;

  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return;
  }

  //Return the current program
  GLint retData = 0;
  GLV->glGetIntegerv(GL_CURRENT_PROGRAM, &retData);
  programID = retData;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditGLSL::FindShader(const UniqueShaderID *findShader)
{
  //Attempt to find the GL id
  ShaderDataGLSL * programData = shaderProgramManager.GetData(findShader->GetShaderOpenGLID());

  //If the GL ID is found in the manager and the unique IDs match,
  // the shader is found (Note that the same GL ID can exist in multiple contexts)
  if(programData && 
     programData->GetUniqueID()->GetUniqueIndex() == findShader->GetUniqueIndex())
  {
    return true;
  }

  //Search in the shader array
  ShaderDataGLSL * shaderData = shaderObjectManager.GetData(findShader->GetShaderOpenGLID());

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
bool ShaderEditGLSL::AddSubstituteShader(GLuint programID, const string &shaderSource, string &retLog)
{
  //Compile the new program
  GLuint newProgram = 0;
  if(!CompileProgram(programID, shaderSource, newProgram, retLog))
  {
    return false;
  }

  //Get the existing shader
  ShaderDataGLSL * programData = shaderProgramManager.GetData(programID);
  if(programID == 0 || !programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("ShaderEditGLSL::AddSubstituteShader - Invalid ID %u",programID));
    retLog += "==Invalid OpenGL shader ID==\n";
    return false;
  }

  //Create a substitute shader
  SubstituteShaderGLSL * newSub = new SubstituteShaderGLSL(newProgram,gliCallBacks);
  SubShaderGLSLRef newSubRef;
  newSubRef.Attach(newSub);


#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

  //Assign the uniform bug manager to the new substitution
  newSubRef->uniformBugManager = programData->uniformBugManager;

#endif  //GLI_ATI_UNIFORM_GLSL_BUG


  //Init the substitute
  string initLog;
  if(!newSubRef->Init(programID, initLog))
  {
    retLog += "\n==Unable to init substitute program.==\n";
    retLog += initLog;
    return false;
  }

  //Append any init data
  retLog += initLog;

  //Destroy any existing substitute shader 
  //  (not done in destructor as it can only be performed when the context is valid)
  if(programData->GetUniqueID()->GetSubstituteShader())
  {
    programData->GetUniqueID()->GetSubstituteShader()->DestroyProgram();
  }

  //Set the substitute in the program data
  programData->GetUniqueID()->SetSubstituteShader(newSubRef);

  //Set the alternate shader source
  programData->GetUniqueID()->SetOverrideShaderSource(shaderSource);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderEditGLSL::CompileProgram(GLuint programID, const string &shaderSource, GLuint &newProgram, string &retLog)
{
  //Assign the initial return program ID
  newProgram = 0;

  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    retLog += "==OpenGL driver not available to compile.==\n";
    return false;
  }

  //Get the existing shader
  ShaderDataGLSL * programData = shaderProgramManager.GetData(programID);
  if(programID == 0 || !programData || programData->GetGLType() != GL_PROGRAM_OBJECT_ARB)
  {
    LOGERR(("ShaderEditGLSL::CompileProgram - Invalid ID %u",programID));
    retLog += "==Invalid OpenGL shader ID==\n";
    return false;
  }

  //Check that the existing program is linked
  GLint checkLinkStatus = 0;
  iglGetProgramiv(programID, GL_LINK_STATUS, &checkLinkStatus);
  if(checkLinkStatus == 0)
  {
    LOGERR(("ShaderEditGLSL::CompileProgram - Program not linked. ID %u",programID));
    retLog += "==Invalid OpenGL shader ID==\n";
    return false;
  }

  // Extract all the shader strings from the source program
  vector<ShaderUtilsGLSL::ShaderStrData> shaderStrings;
  if(!ShaderUtilsGLSL::ExtractShaderStrings(shaderSource, shaderStrings))
  {
    LOGERR(("ShaderEditGLSL::CompileProgram - Unable to extract shader strings. ID %u",programID));
    retLog += "==Unable to access shader string data in program string (no [] tags?)==\n";
    return false;
  }

  // Loop and compile all the shader strings
  bool vertexShaderUsed = false;
  vector<GLuint> shadersIDs;
  for(uint i = 0; i < shaderStrings.size(); i++)
  {
    GLuint newShaderID = CompileShader(shaderStrings[i].shaderType, shaderStrings[i].shaderStr, retLog);
    if(newShaderID == 0)
    {
      //Delete the previously compiled shaders
      for (uint sid=0; sid < shadersIDs.size(); sid++)
      {
        iglDeleteShader(shadersIDs[sid]);
      }

      retLog += "\n==Error compiling shader.==\n";
      return false;
    }

    // Save the shader IDs
    shadersIDs.push_back(newShaderID);

    // Flag if vertex shaders are used
    if(shaderStrings[i].shaderType == GL_VERTEX_SHADER)
    {
      vertexShaderUsed = true;
    }
  }

  //If all shaders are empty, return false
  if(shadersIDs.size() == 0)
  {
    retLog += "\n==No vertex/fragment shaders to compile.==\n";
    return false;
  }

  //Create the shader program 
  newProgram = iglCreateProgram();

  //Attach the shaders  
  for(uint i = 0; i < shadersIDs.size(); i++)
  {
    iglAttachShader(newProgram, shadersIDs[i]); 
  }

  // If vertex shaders are used
  if(vertexShaderUsed)
  {
    //Copy vertex shader attributes from the existing program
    AttribLocationCopy(programID, newProgram);
  }

  //Link
  iglLinkProgram(newProgram);

  //Delete the shaders 
  // (NOTE: ATI has problems with detaching - seems to remove the compiled shader from the program)
  for(uint i = 0; i < shadersIDs.size(); i++)
  {
    iglDeleteShader(shadersIDs[i]); 
  }
  shadersIDs.clear();

  //Append the link log
  string linkLog;
  GetLogData(newProgram, linkLog, true);
  if(linkLog.size() > 0)
  {
    retLog += "Link Info Log:\n";
    retLog += linkLog;
    retLog += "\n";
  }
  
  //Abort on link fail
  GLint linkStatus = 0;
  iglGetProgramiv(newProgram, GL_LINK_STATUS, &linkStatus);
  if(linkStatus == 0)
  {
    //Delete the program
    iglDeleteProgram(newProgram); 
    newProgram = 0;

    retLog += "\n==Unable to link shaders.==\n";
    return false;
  }

  //Append to return log any missing attributes
  string attribDiff;
  GetAttribDifferences(programID, newProgram, attribDiff);
  if(attribDiff.size() > 0)
  {
    retLog += "New program vertex attribute mismatchs:\n\n";
    retLog += attribDiff;
    retLog += "\n";
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
GLuint ShaderEditGLSL::CompileShader(GLenum shaderType, const string &shaderSource, string &retLog) const
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return 0;
  }

  //Create the new shader
  GLuint newShader = iglCreateShader(shaderType);
  if(newShader == 0)
  {
    return 0;
  }

  //Load in the shader source
  const GLchar * src = (const GLchar *)shaderSource.c_str();
  iglShaderSource(newShader, 1, &src, NULL);

  //Compile the shader
  iglCompileShader(newShader);

  //Attach any info log
  string compileLog;
  GetLogData(newShader, compileLog, false);
  if(compileLog.size() > 0)
  {
    if(shaderType == GL_VERTEX_SHADER)
    {
      retLog += "Vertex shader info log:\n";
    }
    if(shaderType == GL_GEOMETRY_SHADER)
    {
      retLog += "Geometry shader info log:\n";
    }
    if(shaderType == GL_FRAGMENT_SHADER)
    {
      retLog += "Fragment shader info log:\n";
    }

    retLog += compileLog;
    retLog += "\n";
  }

  //Check the compile status
  GLint compileStatus;
  iglGetShaderiv(newShader, GL_COMPILE_STATUS, &compileStatus);
  if(compileStatus == 0)
  {
    //Delete the shader
    iglDeleteShader(newShader); 
    return 0; 
  }

  return newShader;
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::AttribLocationCopy(GLuint oldProgram, GLuint newProgram) const
{
  //Get the attributes
  vector<AttributeData> oldAttribArray;
  GetAttributeData(oldProgram, oldAttribArray);

  //Loop for all attributes
  for(uint i=0; i<oldAttribArray.size(); i++)
  {
    //If the name does not start with "gl_"
    if(!oldAttribArray[i].isGLPrefix)
    {
      //Get the location in the old shader
      GLint attribLocation = iglGetAttribLocation(oldProgram, (const GLchar *)oldAttribArray[i].name.c_str());

      //Set the location in the new shader 
      if(attribLocation >= 0)
      {
        iglBindAttribLocation(newProgram, attribLocation, (const GLchar *)oldAttribArray[i].name.c_str());
      }
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::GetAttribDifferences(GLuint oldProgram, GLuint newProgram, string &attribDiff) const
{
  string buffer;

  //Get the old attributes
  vector<AttributeData> oldAttribArray;
  GetAttributeData(oldProgram, oldAttribArray);

  //Get the new attributes
  vector<AttributeData> newAttribArray;
  GetAttributeData(newProgram, newAttribArray);

  //Loop for all old attributes
  for(uint i=0; i<oldAttribArray.size(); i++)
  {
    bool attribFound = false;

    //Attempt to find the attribute in the new array
    for(uint i2=0; i2<newAttribArray.size(); i2++)
    {
      //If the names are equal, compare and break
      if(oldAttribArray[i].name == newAttribArray[i2].name)
      {
        //Only log non-GL functions
        if(!oldAttribArray[i].isGLPrefix)
        {
          //See if there are any differences
          if(oldAttribArray[i].type != newAttribArray[i2].type ||
             oldAttribArray[i].size != newAttribArray[i2].size)
          {
            //Get string versions of the types
            string oldTypeStr = GetAttributeType(oldAttribArray[i].type);
            string newTypeStr = GetAttributeType(newAttribArray[i2].type);

            //Print the type differences
            StringPrintF(buffer, " %s - Attribute is different.\n     -Old type/size: %s,%d\n     -New type/size: %s,%d\n",
                                   oldAttribArray[i].name.c_str(), 
                                   oldTypeStr.c_str(), oldAttribArray[i].size,
                                   newTypeStr.c_str(), newAttribArray[i2].size);
            attribDiff += buffer;
          }
        }

        //Remove from the array
        newAttribArray.erase(newAttribArray.begin() + i2);

        //Flag that the attribute was found
        attribFound = true;
        break;
      }
    }

    //If not found, log
    if(!attribFound)
    {
      StringPrintF(buffer, " %s - Attribute not found in new program\n", oldAttribArray[i].name.c_str());
      attribDiff += buffer;
    }
  }

  //Log all new attributes
  for(uint n=0; n<newAttribArray.size(); n++)
  {
    StringPrintF(buffer, " %s - Attribute in new program -not in old program\n", newAttribArray[n].name.c_str());
    attribDiff += buffer;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::GetAttributeData(GLuint programID, vector<AttributeData> &vertexAttribData) const
{
  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return;
  }

  //Empty the array
  vertexAttribData.clear();

  //Get the number of attributes in the source program
  GLint numAttrib=0;
  iglGetProgramiv(programID,GL_ACTIVE_ATTRIBUTES, &numAttrib);
  if(numAttrib <= 0)
  {
    return;
  }
  
  //Get the maximum attribute length
  GLint maxAttribLength=0;
  iglGetProgramiv(programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribLength);
  if(maxAttribLength <= 0)
  {
    return;
  }

  //Allocate the array to get the attribute strings
  GLchar * attributeName = new GLchar[maxAttribLength+1];

  //Loop for all attributes
  for(uint i=0; i<(uint)numAttrib; i++)
  {
    //Get the attribute data from the old program
    GLint  attribSize;
    GLenum attribType;
    GLint  attribStrLength;
    iglGetActiveAttrib(programID, i, maxAttribLength, &attribStrLength, &attribSize, &attribType, attributeName);

    //Assign the attribute data
    AttributeData newAttribData;
    newAttribData.size = attribSize; 
    newAttribData.type = attribType;      
    newAttribData.name = (const char*)attributeName;

    //Determine if it is a reserved in built function
    if(newAttribData.name.find("gl_") == 0)
    {
      newAttribData.isGLPrefix = true;
    }
    else
    {
      newAttribData.isGLPrefix = false;
    }

    //Add to the return array
    vertexAttribData.push_back(newAttribData);
  }

  //Clean up attribute name
  delete [] attributeName;

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderEditGLSL::GetLogData(GLuint shaderObject, string & retString, bool isProgram) const
{
  retString ="";

  //Return if GL calls cannot be made
  if(!DriverAvailable())
  {
    return;
  }

  //Get the info log size
  GLint logSize=0;
  if(isProgram)
  {
    iglGetProgramiv(shaderObject, GL_INFO_LOG_LENGTH, &logSize);
  }
  else
  {
    iglGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logSize);
  }

  //Get the info log (only get logs of length greater than 1)
  if(logSize > 1)
  {
    //Allocate a array to hold the log
    GLchar * infoLog = new GLchar[logSize+1];
    infoLog[0] = '\0';

    //Get the info log
    if(isProgram)
    {
      iglGetProgramInfoLog(shaderObject, logSize, NULL, infoLog); 
    }
    else
    {
      iglGetShaderInfoLog(shaderObject, logSize, NULL, infoLog); 
    }
    
    //Not necessary, but just to be sure append a NULL character
    infoLog[logSize] = '\0';
    
    //Append to the string
    retString += (char*)infoLog;
    
    //Free info log array
    delete [] infoLog;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
string ShaderEditGLSL::GetAttributeType(GLenum attribEnum) const
{
  string retString;

  switch(attribEnum)
  {
    case(GL_FLOAT):
      retString = "float";
      break;

    case(GL_FLOAT_VEC2):
      retString = "vec2";
      break;

    case(GL_FLOAT_VEC3):
      retString = "vec3";
      break;

    case(GL_FLOAT_VEC4):
      retString = "vec4";
      break;


    case(GL_INT):
      retString = "int";
      break;

    case(GL_INT_VEC2):
      retString = "ivec2";
      break;

    case(GL_INT_VEC3):
      retString = "ivec3";
      break;

    case(GL_INT_VEC4):
      retString = "ivec4";
      break;


    case(GL_UNSIGNED_INT):
      retString = "uint";
      break;

    case(GL_UNSIGNED_INT_VEC2):
      retString = "uvec2";
      break;

    case(GL_UNSIGNED_INT_VEC3):
      retString = "uvec3";
      break;

    case(GL_UNSIGNED_INT_VEC4):
      retString = "uvec4";
      break;


    case(GL_BOOL)  :
      retString = "boolean";
      break;

    case(GL_BOOL_VEC2):
      retString = "bvec2";
      break;

    case(GL_BOOL_VEC3):
      retString = "bvec3";
      break;

    case(GL_BOOL_VEC4):
      retString = "bvec4";
      break;


   case(GL_FLOAT_MAT2):
      retString = "mat2";
      break;

    case(GL_FLOAT_MAT3):
      retString = "mat3";
      break;

    case(GL_FLOAT_MAT4):
      retString = "mat4";
      break;


    case(GL_FLOAT_MAT2x3):
      retString = "mat2x3";
      break;

    case(GL_FLOAT_MAT2x4):
      retString = "mat2x4";
      break;

    case(GL_FLOAT_MAT3x2):
      retString = "mat3x2";
      break;

    case(GL_FLOAT_MAT3x4):
      retString = "mat3x4";
      break;

    case(GL_FLOAT_MAT4x2):
      retString = "mat4x2";
      break;

    case(GL_FLOAT_MAT4x3):
      retString = "mat4x3";
      break;

    default:
      StringPrintF(retString,"0x%x", attribEnum);
      break;
  }

  return retString;
}







