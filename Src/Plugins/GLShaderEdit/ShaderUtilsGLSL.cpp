/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2005  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "MiscUtils.h"

#include "ShaderUtilsGLSL.h"
#include <CommonErrorLog.h>

USING_ERRORLOG

// Helper mapping structure
struct ShaderTypeMapping
{
  ShaderTypeMapping(GLuint a_shaderType, const char * a_shaderTypeName)
    : shaderType(a_shaderType)
    , shaderTypeName(a_shaderTypeName)
  {
  }

  GLuint shaderType;
  const char * shaderTypeName;
};

// Mapping of known shader types
static ShaderTypeMapping s_shaderTypeMappings[] =
{
  ShaderTypeMapping(GL_VERTEX_SHADER,   "Vertex"),
  ShaderTypeMapping(GL_GEOMETRY_SHADER, "Geometry"),
  ShaderTypeMapping(GL_FRAGMENT_SHADER, "Fragment"),
};
#define NUM_SHADER_TYPE_MAP (sizeof(s_shaderTypeMappings) / sizeof(ShaderTypeMapping))


///////////////////////////////////////////////////////////////////////////////
//
UniformData::UniformData():

type(0),
size(0),

numTypeElements(0),
baseFormat(GL_FLOAT),

#ifdef  GLI_ATI_UNIFORM_GLSL_BUG

isATIUniformOverride(false),

#endif  //GLI_ATI_UNIFORM_GLSL_BUG

remapIndex(0)
{

}

///////////////////////////////////////////////////////////////////////////////
//
ShaderUtilsGLSL::ShaderUtilsGLSL(InterceptPluginCallbacks *callBacks):
gliCallBacks(callBacks),

iglGetProgramiv(NULL),

iglGetActiveUniform(NULL),
iglGetUniformLocation(NULL),
iglGetUniformfv(NULL),
iglGetUniformiv(NULL),
iglGetUniformuiv(NULL),

iglUniform1fv(NULL),
iglUniform2fv(NULL),
iglUniform3fv(NULL),
iglUniform4fv(NULL),
iglUniformMatrix2fv(NULL),
iglUniformMatrix3fv(NULL),
iglUniformMatrix4fv(NULL),
iglUniform1iv(NULL),
iglUniform2iv(NULL),
iglUniform3iv(NULL),
iglUniform4iv(NULL),
iglUniform1uiv(NULL),
iglUniform2uiv(NULL),
iglUniform3uiv(NULL),
iglUniform4uiv(NULL),

iglUniformMatrix2x3fv(NULL),
iglUniformMatrix2x4fv(NULL),
iglUniformMatrix3x2fv(NULL),
iglUniformMatrix3x4fv(NULL),
iglUniformMatrix4x2fv(NULL),
iglUniformMatrix4x3fv(NULL)

{
}

///////////////////////////////////////////////////////////////////////////////
//
ShaderUtilsGLSL::~ShaderUtilsGLSL()
{
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderUtilsGLSL::Init()
{
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

  GL_FUNC_LOOKUP(glGetProgramiv);
  GL_FUNC_LOOKUP(glGetActiveUniform);
  GL_FUNC_LOOKUP(glGetUniformLocation);
  GL_FUNC_LOOKUP(glGetUniformfv);
  GL_FUNC_LOOKUP(glGetUniformiv);

  GL_FUNC_LOOKUP(glUniform1fv);
  GL_FUNC_LOOKUP(glUniform2fv);
  GL_FUNC_LOOKUP(glUniform3fv);
  GL_FUNC_LOOKUP(glUniform4fv);
  GL_FUNC_LOOKUP(glUniformMatrix2fv);
  GL_FUNC_LOOKUP(glUniformMatrix3fv);
  GL_FUNC_LOOKUP(glUniformMatrix4fv);
  GL_FUNC_LOOKUP(glUniform1iv);
  GL_FUNC_LOOKUP(glUniform2iv);
  GL_FUNC_LOOKUP(glUniform3iv);
  GL_FUNC_LOOKUP(glUniform4iv);

  if(gliCallBacks->GetGLVersion() >= 2.1)
  {
    GL_FUNC_LOOKUP(glUniformMatrix2x3fv);
    GL_FUNC_LOOKUP(glUniformMatrix2x4fv);
    GL_FUNC_LOOKUP(glUniformMatrix3x2fv);
    GL_FUNC_LOOKUP(glUniformMatrix3x4fv);
    GL_FUNC_LOOKUP(glUniformMatrix4x2fv);
    GL_FUNC_LOOKUP(glUniformMatrix4x3fv);
  }

  if(gliCallBacks->GetGLVersion() >= 3.0)
  {
    GL_FUNC_LOOKUP(glGetUniformuiv);
    GL_FUNC_LOOKUP(glUniform1uiv);
    GL_FUNC_LOOKUP(glUniform2uiv);
    GL_FUNC_LOOKUP(glUniform3uiv);
    GL_FUNC_LOOKUP(glUniform4uiv);
  }

#undef GL_FUNC_LOOKUP

  return true;
}


//DT_TODO: Test this method with array sizes and ints etc...
///////////////////////////////////////////////////////////////////////////////
//
void ShaderUtilsGLSL::GetUniformValues(GLuint programHandle, UniformData &uniformData) const
{
  //Check that OpenGL calls can be made 
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //Sanity check the uniform size value
  if(uniformData.size > uniformData.indexData.size() ||
     uniformData.numTypeElements < 1)
  {
    LOGERR(("GetUniformValues - Invalid uniform size value"));
    return;
  } 

  //If the data is float based
  if(uniformData.baseFormat == GL_FLOAT)
  {
    //Create the storage array to be of the correct size
    if(uniformData.floatUniformData.size() != (uniformData.size * uniformData.numTypeElements))
    {
      uniformData.floatUniformData.resize(uniformData.size * uniformData.numTypeElements);
    }
    GLfloat *copyData = &uniformData.floatUniformData[0];

    //Loop for the size of the type
    for(uint s=0; s<uniformData.size; s++)
    {
      //Get the old uniform data
      iglGetUniformfv(programHandle, uniformData.indexData[s], copyData);

      //Update the location pointer
      copyData += uniformData.numTypeElements;
    }
  }
  else if(uniformData.baseFormat == GL_INT)
  {
    //Create the storage array to be of the correct size
    if(uniformData.intUniformData.size() != (uniformData.size * uniformData.numTypeElements))
    {
      uniformData.intUniformData.resize(uniformData.size * uniformData.numTypeElements);
    }
    GLint *copyData = &uniformData.intUniformData[0];

    //Loop for the size of the type
    for(uint s=0; s<uniformData.size; s++)
    {
      //Get the old uniform data
      iglGetUniformiv(programHandle, uniformData.indexData[s], copyData);

      //Update the location pointer
      copyData += uniformData.numTypeElements;
    }
  }
  else if(uniformData.baseFormat == GL_UNSIGNED_INT && iglGetUniformuiv)
  {
    //Create the storage array to be of the correct size
    if(uniformData.uintUniformData.size() != (uniformData.size * uniformData.numTypeElements))
    {
      uniformData.uintUniformData.resize(uniformData.size * uniformData.numTypeElements);
    }
    GLuint *copyData = &uniformData.uintUniformData[0];

    //Loop for the size of the type
    for(uint s=0; s<uniformData.size; s++)
    {
      //Get the old uniform data
      iglGetUniformuiv(programHandle, uniformData.indexData[s], copyData);

      //Update the location pointer
      copyData += uniformData.numTypeElements;
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderUtilsGLSL::GetUniformData(GLuint programHandle, UniformDataArray &retData) const
{
  //Empty the return array
  retData.clear();  

  //Get if OpenGL calls can be made
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return false;
  }

  //Get the number of uniforms for the program
  GLint numUniforms = 0;
  iglGetProgramiv(programHandle, GL_ACTIVE_UNIFORMS, &numUniforms);
  if(numUniforms <= 0)
  {
    return true;
  }

  //Get the max uniform string size
  GLint maxUniformSize = 0;
  iglGetProgramiv(programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformSize);
  if(maxUniformSize <= 0)
  {
    return false;
  }

  //Allocate the array to get the uniform strings
  GLchar * readUniformName = new GLchar[maxUniformSize+1];

  //Loop for the number of uniforms in the program
  for(uint i=0;i<(uint)numUniforms;i++)
  {
    GLint  typeSize=0;
    GLenum type;
    GLsizei lengthWritten=0;
    string  newUniformName;

    //Call GetActiveUniform to get the name,type and size of the type
    iglGetActiveUniform(programHandle,i,maxUniformSize,&lengthWritten,&typeSize,&type,readUniformName);

    if(lengthWritten > 0)
    {
      //Convert to a string
      newUniformName = (char*)readUniformName;


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


      //Ignore built in gl types
      if(newUniformName.find("gl_") != 0 && !ignoreUniform)
      {
        //Loop for the array size for the type
        vector<uint> indexDataArray;
        for(GLint s=0; s<typeSize; s++)
        {
          string testUniformName = newUniformName;
          if(s > 0)
          {
            //If it is an array type, get the index of each component
            string strBuffer;             
            StringPrintF(strBuffer, "[%u]",s);
            testUniformName += strBuffer;
          }

          //Get the location of the uniform
          GLint uniLocation = iglGetUniformLocation(programHandle,(const GLchar*)testUniformName.c_str());
          if(uniLocation < 0)
          {
            LOGERR(("GetUniformData - Uniform %s does not exist?",testUniformName.c_str()));

            //Clean up the uniform name
            delete [] readUniformName;
            return false;
          }

          //Add to the index data array
          indexDataArray.push_back(uniLocation);
        }

        UniformData newData;
        newData.indexData  = indexDataArray;
        newData.remapIndex = 0;

        newData.name = newUniformName;
        newData.size = typeSize;
        newData.type = type;

        //Check that the type is known
        if(!GetTypeData(newData.type, newData.numTypeElements, newData.baseFormat))
        {
          LOGERR(("GetUniformData - %s - Uniform is not a known type: 0x%x\n", newData.name.c_str(), newData.type));

          //Clean up the uniform name
          delete [] readUniformName;
          return false;
        }

        //Add the new data to the return array
        retData.push_back(newData);
      }
    }

  }

  //Clean up the uniform name
  delete [] readUniformName;

  return true;

}


///////////////////////////////////////////////////////////////////////////////
//
bool ShaderUtilsGLSL::GetTypeData(GLenum type, uint &numElements, GLuint &baseFormat)
{
  numElements = 0;
  baseFormat = GL_FLOAT;

  switch(type)
  {
    case(GL_FLOAT):
      numElements = 1;
      baseFormat = GL_FLOAT;
      break;

    case(GL_FLOAT_VEC2):
      numElements = 2;
      baseFormat = GL_FLOAT;
      break;

    case(GL_FLOAT_VEC3):
      numElements = 3;
      baseFormat = GL_FLOAT;
      break;

    case(GL_FLOAT_VEC4):
      numElements = 4; 
      baseFormat = GL_FLOAT;
      break;


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
    case(GL_INT):
      numElements = 1;
      baseFormat = GL_INT;
      break;

    case(GL_INT_VEC2):
      numElements = 2;
      baseFormat = GL_INT;
      break;

    case(GL_INT_VEC3):
      numElements = 3;
      baseFormat = GL_INT;
      break;

    case(GL_INT_VEC4):
      numElements = 4;
      baseFormat = GL_INT;
      break;


    case(GL_UNSIGNED_INT):
      numElements = 1;
      baseFormat = GL_UNSIGNED_INT;
      break;

    case(GL_UNSIGNED_INT_VEC2):
      numElements = 2;
      baseFormat = GL_UNSIGNED_INT;
      break;

    case(GL_UNSIGNED_INT_VEC3):
      numElements = 3;
      baseFormat = GL_UNSIGNED_INT;
      break;

    case(GL_UNSIGNED_INT_VEC4):
      numElements = 4;
      baseFormat = GL_UNSIGNED_INT;
      break;


    case(GL_BOOL)  :
      numElements = 1;
      baseFormat = GL_INT;
      break;

    case(GL_BOOL_VEC2):
      numElements = 2;
      baseFormat = GL_INT;
      break;

    case(GL_BOOL_VEC3):
      numElements = 3;
      baseFormat = GL_INT;
      break;

    case(GL_BOOL_VEC4):
      numElements = 4;
      baseFormat = GL_INT;
      break;


    case(GL_FLOAT_MAT2):
      numElements = 4;
      baseFormat = GL_FLOAT;
      break;

    case(GL_FLOAT_MAT3):
      numElements = 9;
      baseFormat = GL_FLOAT;
      break;

    case(GL_FLOAT_MAT4):
      numElements = 16;
      baseFormat = GL_FLOAT;
      break;

    case(GL_FLOAT_MAT2x3):
    case(GL_FLOAT_MAT3x2):
      numElements = 6;
      baseFormat = GL_FLOAT;
      break;

    case(GL_FLOAT_MAT2x4):
    case(GL_FLOAT_MAT4x2):
      numElements = 8;
      baseFormat = GL_FLOAT;
      break;

    case(GL_FLOAT_MAT3x4):
    case(GL_FLOAT_MAT4x3):
      numElements = 12;
      baseFormat = GL_FLOAT;
      break;

    default:
      return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderUtilsGLSL::LoadUniformDataFloat(GLuint loadPos, GLenum type, GLuint count, const GLfloat *buffer) const
{
  switch(type)
  {
    case(GL_FLOAT):
      iglUniform1fv(loadPos, count, buffer);
      break;

    case(GL_FLOAT_VEC2):
      iglUniform2fv(loadPos, count, buffer);
      break;

    case(GL_FLOAT_VEC3):
      iglUniform3fv(loadPos, count, buffer);
      break;

    case(GL_FLOAT_VEC4):
      iglUniform4fv(loadPos, count, buffer);
      break;

    case(GL_FLOAT_MAT2):
      iglUniformMatrix2fv(loadPos, count, GL_FALSE, buffer);
      break;

    case(GL_FLOAT_MAT3):
      iglUniformMatrix3fv(loadPos, count, GL_FALSE, buffer);
      break;

    case(GL_FLOAT_MAT4):
      iglUniformMatrix4fv(loadPos, count, GL_FALSE, buffer);
      break;

    case(GL_FLOAT_MAT2x3):
      if(iglUniformMatrix2x3fv)
      {
        iglUniformMatrix2x3fv(loadPos, count, GL_FALSE, buffer);
      }
      break;

    case(GL_FLOAT_MAT2x4):
      if(iglUniformMatrix2x4fv)
      {
        iglUniformMatrix2x4fv(loadPos, count, GL_FALSE, buffer);
      }
      break;

    case(GL_FLOAT_MAT3x2):
      if(iglUniformMatrix3x2fv)
      {
        iglUniformMatrix3x2fv(loadPos, count, GL_FALSE, buffer);
      }
      break;

    case(GL_FLOAT_MAT3x4):
      if(iglUniformMatrix3x4fv)
      {
        iglUniformMatrix3x4fv(loadPos, count, GL_FALSE, buffer);
      }
      break;

    case(GL_FLOAT_MAT4x2):
      if(iglUniformMatrix4x2fv)
      {
        iglUniformMatrix4x2fv(loadPos, count, GL_FALSE, buffer);
      }
      break;

    case(GL_FLOAT_MAT4x3):
      if(iglUniformMatrix4x3fv)
      {
        iglUniformMatrix4x3fv(loadPos, count, GL_FALSE, buffer);
      }
      break;

    default:
      LOGERR(("ShaderUtilsGLSL::LoadUniformDataFloat - Invalid type 0x%x",type));
      return;
  }

}



///////////////////////////////////////////////////////////////////////////////
//
void ShaderUtilsGLSL::LoadUniformDataInt(GLuint loadPos, GLenum type, GLuint count, const GLint *buffer) const
{
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
    case(GL_INT)       :
    case(GL_BOOL)  :
      iglUniform1iv(loadPos, count, buffer);
      break;

    case(GL_INT_VEC2):
    case(GL_BOOL_VEC2):
      iglUniform2iv(loadPos, count, buffer);
      break;

    case(GL_INT_VEC3):
    case(GL_BOOL_VEC3):
      iglUniform3iv(loadPos, count, buffer);
      break;

    case(GL_INT_VEC4):
    case(GL_BOOL_VEC4):
      iglUniform4iv(loadPos, count, buffer);
      break;

    default:
      LOGERR(("ShaderUtilsGLSL::LoadUniformDataInt - Invalid type 0x%x",type));
      return;

  }

}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderUtilsGLSL::LoadUniformDataUInt(GLuint loadPos, GLenum type, GLuint count, const GLuint *buffer) const
{
  if(!iglUniform1uiv ||
     !iglUniform2uiv ||
     !iglUniform3uiv ||
     !iglUniform4uiv)
  {
    LOGERR(("ShaderUtilsGLSL::LoadUniformDataUInt - Unsigned int interface not loaded for type 0x%x",type));
    return;
  }

  switch(type)
  {
    case(GL_UNSIGNED_INT) :
      iglUniform1uiv(loadPos, count, buffer);
      break;

    case(GL_UNSIGNED_INT_VEC2):
      iglUniform2uiv(loadPos, count, buffer);
      break;

    case(GL_UNSIGNED_INT_VEC3):
      iglUniform3uiv(loadPos, count, buffer);
      break;

    case(GL_UNSIGNED_INT_VEC4):
      iglUniform4uiv(loadPos, count, buffer);
      break;

    default:
      LOGERR(("ShaderUtilsGLSL::LoadUniformDataUInt - Invalid type 0x%x",type));
      return;

  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderUtilsGLSL::FindShaderToken(const string &shaderSource, uint startIndex, string & retShaderToken, GLuint & retGLType)
{
  retGLType = 0;
  retShaderToken.clear();

  //Loop for all characters in the string
  for(uint i = startIndex; i < shaderSource.size(); i++)
  {
    retShaderToken += shaderSource[i];

    // Abort when reaching the end marker
    if(shaderSource[i] == ']')
    {
      break;
    }
  }

  // Match the token against known strings
  for(uint i = 0; i < NUM_SHADER_TYPE_MAP; i++)
  {
    string testStr = "[" + string(s_shaderTypeMappings[i].shaderTypeName) + " Shader]";
    if(testStr == retShaderToken)
    {
      retGLType = s_shaderTypeMappings[i].shaderType;
      return true;
    }
  }

  // Else, match the unknown type
  if(sscanf(retShaderToken.c_str(),"[Type = 0x%X Shader]", &retGLType) == 1)
  {
    return true;
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
int ShaderUtilsGLSL::GetShaderTypeIndex(GLuint typeID)
{
  // Loop for a known type
  for(uint i = 0; i < NUM_SHADER_TYPE_MAP; i++)
  {
    if(s_shaderTypeMappings[i].shaderType == typeID)
    {
      return i;
    }
  }

  return -1;
}

///////////////////////////////////////////////////////////////////////////////
//
void ShaderUtilsGLSL::AppendShaderStrings(const vector<ShaderStrData> & srcStrings, string & retString)
{
  // Append the strings based on shader types - vertex shader, geometry shader, fragment shader
  string knownShaderTypes[NUM_SHADER_TYPE_MAP];
  string unknownShaderTypes;

  // Loop for all the shader strings
  for(uint i = 0; i < srcStrings.size(); i++)
  {
    // Get the type string
    int typeIndex = GetShaderTypeIndex(srcStrings[i].shaderType);
    if(typeIndex >= 0)
    {
      knownShaderTypes[typeIndex] += "\n[" + string(s_shaderTypeMappings[typeIndex].shaderTypeName) + " Shader]\n";
      knownShaderTypes[typeIndex] += srcStrings[i].shaderStr;
    }
    else
    {
      // Format the unknown type
      string typeStr;
      StringPrintF(typeStr, "\n[Type = 0x%X Shader]\n", srcStrings[i].shaderType);

      unknownShaderTypes += typeStr;
      unknownShaderTypes += srcStrings[i].shaderStr;
    }
  }

  // Append all strings to the return string
  for(uint i = 0; i < NUM_SHADER_TYPE_MAP; i++)
  {
    retString += knownShaderTypes[i];
  }
  retString += unknownShaderTypes;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ShaderUtilsGLSL::ExtractShaderStrings(const string &shaderSource, vector<ShaderStrData> &retSources)
{
  uint lineStart = 0;
  uint shaderStringStart = 0;
  GLuint shaderGLType = 0;
 
  string newLineStr;
  string shaderStartLine;

  //Loop for all characters in the string
  for(uint i = 0; i < shaderSource.size(); i++)
  {
    //Check if the starting character is a open bracket
    if(shaderSource[i] == '[' && lineStart == i)
    {
      //Test for the start token
      string shaderToken;
      GLuint glType;
      if(FindShaderToken(shaderSource, i, shaderToken, glType))
      {
        // If an shader type has just ended
        if(shaderGLType != 0)
        {
          // Assign the shader code string
          string newShaderCode;
          newShaderCode.assign(shaderSource, shaderStringStart, i - shaderStringStart);

          // Set the return array data
          retSources.push_back(ShaderStrData(shaderGLType, shaderStartLine + newShaderCode));
        }
 
        // Save the new start position and type
        shaderStringStart = i + (uint)shaderToken.size();
        shaderGLType = glType;

        //Set what line the shader was found on
        shaderStartLine = newLineStr;
      }
    }

    //If the character is equal to the new line character,
    // The next character must be on the new line
    if(shaderSource[i] == '\r' || shaderSource[i] == '\n')
    {
      lineStart = i+1;
    }

    //Count the new lines (so that each returned string has the same line offset as the main string)
    if(shaderSource[i] == '\n')
    {
      newLineStr += '\n';
    }
  }

  //If the string was not found, return false
  if(shaderGLType == 0)
  {
    return false;
  }

  // Assign the last string
  {
    string newShaderCode;
    newShaderCode.assign(shaderSource, shaderStringStart, shaderSource.size() - shaderStringStart);

    // Set the return array data
    retSources.push_back(ShaderStrData(shaderGLType, shaderStartLine + newShaderCode));
  }

  return true;
}





