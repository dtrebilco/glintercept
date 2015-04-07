/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "MiscUtils.h"

#include "SubstituteShaderGLSL.h"
#include <CommonErrorLog.h>

USING_ERRORLOG


///////////////////////////////////////////////////////////////////////////////
//
SubstituteShaderGLSL::SubstituteShaderGLSL(GLuint newProgramID,InterceptPluginCallbacks *callBacks):
programID(newProgramID),
oldProgramID(0),
shaderUtils(callBacks),
gliCallBacks(callBacks),

iglUseProgram(NULL),
iglDeleteProgram(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////
//
SubstituteShaderGLSL::~SubstituteShaderGLSL()
{
  //Do not delete the substitute here.
  // (this will mean a shader leak if the main GLSL program leaks itself)
}

///////////////////////////////////////////////////////////////////////////////
//
bool SubstituteShaderGLSL::Init(GLuint oldProgram, string &initLog)
{
  //Check the program ID's
  if(oldProgram == 0 || programID == 0)
  {
    return false;
  }

  //Assign the old program ID
  oldProgramID = oldProgram;

  //Check if GL calls can be made
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return false;
  }

  //Only allow usage of the 2.0 interfaces
  if(gliCallBacks->GetGLVersion() < 2.0)
  {
    return false;
  }

  //Init the shader utils object
  if(!shaderUtils.Init())
  {
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

  GL_FUNC_LOOKUP(glUseProgram);
  GL_FUNC_LOOKUP(glDeleteProgram);

#undef GL_FUNC_LOOKUP

  //Generate the lookup table of values to copy
  string uniformLog;
  GenerateUniformRemapArray(uniformLog);
  if(uniformLog.size() > 0)
  {
    initLog += "New program uniform mismatchs:\n\n";
    initLog += uniformLog;
    initLog += "\n";
  }

  return true;
}


///////////////////////////////////////////////////////////////////////////////
//
void SubstituteShaderGLSL::DestroyProgram()
{
  //Delete the program id if possible
  if(programID != 0)
  {
    //If driver is available delete the program
    if(gliCallBacks->GetGLInternalCallState())
    {
      iglDeleteProgram(programID);
    }
    //Else log an error
    else
    {
      LOGERR(("Unable to delete substitute program %u",programID));
    }

    //Set the program ID to zero 
    programID = 0;
  }
  
}

///////////////////////////////////////////////////////////////////////////////
//
void SubstituteShaderGLSL::BindSubstitute()
{
  //Get if OpenGL calls can be made
  if(programID == 0 || !gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //Note: Perhaps make sure the current program is the oldProgram?

  //Bind the substitute program
  iglUseProgram(programID);

  //Copy the uniform data
  UniformDataCopy();

}

///////////////////////////////////////////////////////////////////////////////
//
void SubstituteShaderGLSL::UnBindSubstitute()
{
  //Get if OpenGL calls can be made
  if(programID == 0 || !gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //Reset to the origional program
  iglUseProgram(oldProgramID);

}

//DT_TODO: Test this with int and float arrays
///////////////////////////////////////////////////////////////////////////////
//
void SubstituteShaderGLSL::UniformDataCopy()
{
  //Check that OpenGL calls can be made 
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //Loop for all re-map types
  for(uint i=0; i<remapUniformArray.size(); i++)
  {
    UniformData &remapData = remapUniformArray[i];

#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

    //If this is a ATI override, get the data from the stored arrays
    if(remapData.isATIUniformOverride && uniformBugManager)
    {
      //If there is a value to re-map
      if(remapData.indexData.size() > 0)
      {
        //Even if this is an array of values, just load the first one.
        // (this bug is in relation to samplers which are typically single values)
        GLint loadInt = uniformBugManager->GetUniformData(remapData.indexData[0]);

        //Load the single override value
        shaderUtils.LoadUniformDataInt(remapData.remapIndex, remapData.type, 1, &loadInt);       
      }

      //Skip to the next uniform
      continue;
    }

#endif  //GLI_ATI_UNIFORM_GLSL_BUG

    //Get the uniform data from the old program
    shaderUtils.GetUniformValues(oldProgramID, remapData);

    //Load the data into the new program
    if(remapData.baseFormat == GL_FLOAT)
    {
      //Load the float uniform data
      if(remapData.floatUniformData.size() == (remapData.size * remapData.numTypeElements))
      {
        shaderUtils.LoadUniformDataFloat(remapData.remapIndex, remapData.type, 
                                         remapData.size, &remapData.floatUniformData[0]); 
      }
    }
    else if(remapData.baseFormat == GL_INT)
    {
      //Load the int uniform data
      if(remapData.intUniformData.size() == (remapData.size * remapData.numTypeElements))
      {
        shaderUtils.LoadUniformDataInt(remapData.remapIndex, remapData.type, 
                                       remapData.size, &remapData.intUniformData[0]); 
      }
    }
    else if(remapData.baseFormat == GL_UNSIGNED_INT)
    {
      //Load the uint uniform data
      if(remapData.uintUniformData.size() == (remapData.size * remapData.numTypeElements))
      {
        shaderUtils.LoadUniformDataUInt(remapData.remapIndex, remapData.type, 
                                        remapData.size, &remapData.uintUniformData[0]); 
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool SubstituteShaderGLSL::TestUniformDataCopy(UniformData &remapData, string &retLog) const
{
  string bufferStr;

  //Test for OpenGL errors during copy
  if(gliCallBacks->GetCoreGLFunctions()->glGetError() != GL_NO_ERROR)
  {
    StringPrintF(bufferStr,"Existing OpenGL error - fix application?\n");
    retLog += bufferStr;
  }

  //Check the size
  if(remapData.size > remapData.indexData.size())
  {
    StringPrintF(bufferStr,"%s - Invalid uniform size values\n",remapData.name.c_str());
    retLog += bufferStr;
    return false;
  } 

  //Get the uniform data from the old program
  //Note: This may crash on some implementations (eg ATI Cat 5.9) 
  //      if the new array is smaller than the old uniform data array.
  //      (ATI returns the complete array size on each glGetUniform call - which may 
  //       overwrite the end of the array if the array size has shrunk in the new program).
  shaderUtils.GetUniformValues(oldProgramID, remapData);

  //If a error occured during retrieval
  if(gliCallBacks->GetCoreGLFunctions()->glGetError() != GL_NO_ERROR)
  {
    StringPrintF(bufferStr," %s - OpenGL error on uniform copy - (broken drivers?)\n", remapData.name.c_str());
    retLog += bufferStr;
    return false;
  }

  return true;
}



///////////////////////////////////////////////////////////////////////////////
//
void SubstituteShaderGLSL::GenerateUniformRemapArray(string &initLog)
{
  uint i;
  string bufferStr;

  //Get the uniforms of the old program
  UniformDataArray oldUniformData;
  if(!shaderUtils.GetUniformData(oldProgramID, oldUniformData))
  {
    initLog += "Unble to get old program unform data\n";
    return;
  }

  //Get the uniforms of the new program
  UniformDataArray newUniformData;
  if(!shaderUtils.GetUniformData(programID, newUniformData))
  {
    initLog += "Unble to get new program unform data\n";
    return;
  }

  //Clear the mapping array
  remapUniformArray.clear();

  //Loop for all values in the old array
  for(i=0; i<oldUniformData.size(); i++)
  {
    bool foundFlag = false;
    const UniformData & oldData = oldUniformData[i];

    //Loop for all new values
    for(uint i2=0; i2<newUniformData.size(); i2++)
    {
      const UniformData & newData = newUniformData[i2];

      //Check for a name match
      if(oldData.name == newData.name)
      {
        foundFlag = true;

        //Check the types and abort if not equal 
        if(oldData.type != newData.type)
        {
          StringPrintF(bufferStr," %s - Uniforms types are different\n",oldData.name.c_str());
          initLog += bufferStr;
          break;
        }

        //Check the sizes 
        uint addDataTypeSize = oldData.size;
        if(oldData.size != newData.size)
        {
          StringPrintF(bufferStr," %s - Uniform sizes are different (%u != %u)\n",
                                    oldData.name.c_str(),oldData.size,newData.size);
          initLog += bufferStr;

          //Take the minimum of the two sizes
          if(newData.size < addDataTypeSize)
          {
            addDataTypeSize = newData.size;
          }
        }
        
        //Check the type size
        if(addDataTypeSize == 0)
        {
          StringPrintF(bufferStr," %s - Uniform size is zero?\n", oldData.name.c_str());
          initLog += bufferStr;
          break;
        }

        //Create a new entry in the mapping array
        UniformData addData;
        
        addData.indexData  = oldData.indexData;
        addData.remapIndex = newData.indexData[0];

        addData.name = newData.name;
        addData.size = addDataTypeSize;
        addData.type = newData.type;

        //Get the data about the type 
        if(!shaderUtils.GetTypeData(addData.type, addData.numTypeElements, addData.baseFormat))
        {
          StringPrintF(bufferStr," %s - Uniform is not a known type: 0x%x\n", addData.name.c_str(),addData.type);
          initLog += bufferStr;
          break;
        }

        //Test if the data can be copied without OpenGL errors (ATI bug)
        if(TestUniformDataCopy(addData, initLog))
        {
          //Add the data to the array
          remapUniformArray.push_back(addData);
        }

#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

        //If it is just a single variable (hopefully a sampler), use the ATI override
        else if(addData.baseFormat == GL_INT && addData.numTypeElements == 1)
        {
          initLog += string("Using ATI uniform override on ") + addData.name + string("\n");
          addData.isATIUniformOverride = true; 
          remapUniformArray.push_back(addData);
        }

#endif  //GLI_ATI_UNIFORM_GLSL_BUG


        break;
      }
    }

    //If not found
    if(!foundFlag)
    {
      StringPrintF(bufferStr," %s - Uniform not found in new program\n",
                                oldData.name.c_str());
      initLog += bufferStr;
    }
  }

  //Log all the new uniforms not in the old program
  for(i=0; i<newUniformData.size(); i++)
  {
    bool foundFlag = false;
    const UniformData & newData = newUniformData[i];

    //Loop for all old values
    for(uint i2=0; i2<oldUniformData.size(); i2++)
    {
      const UniformData & oldData = oldUniformData[i2];

      //Check for a name match
      if(oldData.name == newData.name)
      {
        foundFlag = true;
        break;
      }
    }

    //If not found
    if(!foundFlag)
    {
      StringPrintF(bufferStr," %s - Uniform in new program -not in old program\n",
                               newData.name.c_str());
      initLog += bufferStr;
    }
  }

}





