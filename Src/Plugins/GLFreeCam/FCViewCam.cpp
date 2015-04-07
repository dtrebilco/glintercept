/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#include "FCViewCam.h"

#include <CommonErrorLog.h>

USING_ERRORLOG

//External glu methods
void      transform_point(GLdouble out[4], const GLdouble m[16], const GLdouble in[4]);
GLboolean invert_matrix(const GLdouble * m, GLdouble * out);


///////////////////////////////////////////////////////////////////////////////
//
FCViewCam::FCViewCam(InterceptPluginCallbacks *callBacks):
initData(false),
extensionVertexProgram(false),
extensionARBGLSLObjects(false),
oglVersion(1.0),
gliCallBacks(callBacks),
GLV(callBacks->GetCoreGLFunctions()),
setLogicOp(GL_SET),
iglUseProgram(NULL),
iglGetHandleARB(NULL),
iglUseProgramObjectARB(NULL)
{
  //The projMatrix and frustumPoints are set in Init

}

///////////////////////////////////////////////////////////////////////////////
//
FCViewCam::~FCViewCam()
{
}

///////////////////////////////////////////////////////////////////////////////
//
void FCViewCam::Init(const FCConfigData &configData)
{
  //Return now if unable to make OpenGL calls
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //Set initial frustum points
  UpdateFrustumPoints();

  //Get if vertex programs are supported (ARB/NV use the same tag)
  if(gliCallBacks->IsGLExtensionSupported("GL_ARB_vertex_program") || 
     gliCallBacks->IsGLExtensionSupported("GL_NV_vertex_program"))
  {
    extensionVertexProgram = true;
  }

  //Get the OpenGL version
  oglVersion = gliCallBacks->GetGLVersion();

  //Get if shader objects are supported
  if(oglVersion >= 2.0f)
  {
    iglUseProgram = (void (GLAPIENTRY *) (GLuint handle))gliCallBacks->GetGLFunction("glUseProgram");
  }
  else if(gliCallBacks->IsGLExtensionSupported("GL_ARB_shader_objects"))
  {
    extensionARBGLSLObjects = true;

    //Get the entry points (favour OGL 2.0 over ARB)
    iglGetHandleARB = (GLhandle (GLAPIENTRY *) (GLenum pname))gliCallBacks->GetGLFunction("glGetHandleARB");
    iglUseProgramObjectARB = (void (GLAPIENTRY *) (GLhandle handle))gliCallBacks->GetGLFunction("glUseProgramObjectARB");
  }


  //Set the GL logic op to black if necessary (else will default to white)
  if(configData.viewFrustumColor == "black")
  {
    setLogicOp = GL_CLEAR;
  }

  //Flag that init was successful
  initData = true;
}

///////////////////////////////////////////////////////////////////////////////
//
void FCViewCam::RenderViewFrustum()
{
  //Return if not init
  if(!initData)
  {
    return;
  }

  //Return now if unable to make OpenGL calls
  if(!gliCallBacks->GetGLInternalCallState())
  {
    return;
  }

  //May want to compare the current matrix and then update?
  UpdateFrustumPoints();

  //Perhaps in future set up buffer writes/tests (depth etc)

  //Disable programs
  bool oldVertexProgramMode = false;
  if(extensionVertexProgram && 
     GLV->glIsEnabled(GL_VERTEX_PROGRAM_ARB))
  {
    oldVertexProgramMode = true;
    GLV->glDisable(GL_VERTEX_PROGRAM_ARB);
  }

  //Disable GLSL objects
  uint oldGLSLHandle = 0;
  if(oglVersion >= 2.0f && iglUseProgram)
  {
    GLint retData = 0;
    GLV->glGetIntegerv(GL_CURRENT_PROGRAM, &retData);
    oldGLSLHandle = retData;

    //Disable GLSL programs
    iglUseProgram(0);
  }
  else if(extensionARBGLSLObjects && iglGetHandleARB)
  {
    oldGLSLHandle = iglGetHandleARB(GL_PROGRAM_OBJECT_ARB);
    
    //Disable GLSL programs
    iglUseProgramObjectARB(0);
  }

  //Disable alpha test
  bool oldAlphaTestMode = false;
  if(GLV->glIsEnabled(GL_ALPHA_TEST))
  {
    oldAlphaTestMode = true;
    GLV->glDisable(GL_ALPHA_TEST);
  }

  //Enable logic ops
  GLint oldLogicOpEnable;
  GLV->glGetIntegerv(GL_COLOR_LOGIC_OP,&oldLogicOpEnable);
  if(oldLogicOpEnable != GL_TRUE)
  {
    GLV->glEnable(GL_COLOR_LOGIC_OP);
  }

  //Set the logic mode to output pure white
  GLint oldLogicOpMode;
  GLV->glGetIntegerv(GL_LOGIC_OP_MODE,&oldLogicOpMode);
  GLV->glLogicOp(setLogicOp);

  //Render the lines
  RenderFrustumLines();

  //Reset the logic op mode
  GLV->glLogicOp(oldLogicOpMode);

  //Reset logic ops
  if(oldLogicOpEnable != GL_TRUE)
  {
    GLV->glDisable(GL_COLOR_LOGIC_OP);
  }
  
  //Re-enable alpha test
  if(oldAlphaTestMode)
  {
    GLV->glEnable(GL_ALPHA_TEST);
  }

  //Re-enable GLSL
  if(oldGLSLHandle != 0)
  {
    //Re-enable using the same disable interface
    if(oglVersion >= 2.0f && iglUseProgram)
    {
      iglUseProgram(oldGLSLHandle);
    }
    else
    {
      iglUseProgramObjectARB(oldGLSLHandle);
    }
  }

  //Re-enable programs
  if(oldVertexProgramMode)
  {
    GLV->glEnable(GL_VERTEX_PROGRAM_ARB);
  }

  
/* Note: Initially tried with blending. Problems with two render passes and blending
          
  //Enabled blending
  GLint oldBlendEnable;
  GLV->glGetIntegerv(GL_BLEND,&oldBlendEnable);
  if(oldBlendEnable != GL_TRUE)
  {
    GLV->glEnable(GL_BLEND);
  }

  NOTE: Need to get/rest blend function/equation seperate 
  //Get the old blend function 
  GLint oldSrcBlendFunc;
  GLint oldDstBlendFunc;
  GLV->glGetIntegerv(GL_BLEND_SRC,&oldSrcBlendFunc);
  GLV->glGetIntegerv(GL_BLEND_DST,&oldDstBlendFunc);
  GLV->glBlendFunc(GL_ZERO, GL_CONSTANT_COLOR);

  //Get the old blend color
  GLfloat oldBlendColor[4];
  GLV->glGetFloatv(GL_BLEND_COLOR,&oldBlendColor[0]);
  iglBlendColor(1.0f,1.0f,0.0f,1.0f);

  //Get the old blend equation
  GLint oldBlendEquation;
  GLV->glGetIntegerv(GL_BLEND_EQUATION,&oldBlendEquation);
  if(oldBlendEquation != GL_FUNC_ADD)
  {
    iglBlendEquation(GL_FUNC_ADD);
  }

  RenderFrustumLines();

  //Reset the blend equation
  if(oldBlendEquation != GL_FUNC_ADD)
  {
    iglBlendEquation(oldBlendEquation);
  }

  //Reset the blend color
  iglBlendColor(oldBlendColor[0],oldBlendColor[1],oldBlendColor[2],oldBlendColor[3]);

  //Reset blending
  GLV->glBlendFunc(oldSrcBlendFunc, oldDstBlendFunc);
  if(oldBlendEnable != GL_TRUE)
  {
    GLV->glDisable(GL_BLEND);
  }
*/

}


///////////////////////////////////////////////////////////////////////////////
//
void FCViewCam::UpdateFrustumPoints()
{
  //Get the current projection matrix
  GLV->glGetDoublev(GL_PROJECTION_MATRIX,&projMatrix[0]);

  GLdouble winCoords[8][4] = 
  { {-1.0, -1.0, -1.0, 1.0},
    {-1.0,  1.0, -1.0, 1.0},
    { 1.0,  1.0, -1.0, 1.0},
    { 1.0, -1.0, -1.0, 1.0},
    
    {-1.0, -1.0,  1.0, 1.0},
    {-1.0,  1.0,  1.0, 1.0},
    { 1.0,  1.0,  1.0, 1.0},
    { 1.0, -1.0,  1.0, 1.0},
  };

  // Calculate inverse projection
  GLdouble inverseProj[16];
  if(!invert_matrix(projMatrix, inverseProj))
  {
    return;
  }

  //Get the frustum coordinates 
  // (need 4 coordinates as some perspective matrices result in negative w)
  for(uint i=0;i<8;i++)
  {
    //Transform the point
    transform_point(frustumPoints[i], inverseProj, winCoords[i]);
  }

}


///////////////////////////////////////////////////////////////////////////////
//
void FCViewCam::RenderFrustumLines()
{
  GLV->glBegin(GL_LINES);

  //Front
  GLV->glVertex4dv(frustumPoints[0]);
  GLV->glVertex4dv(frustumPoints[1]);

  GLV->glVertex4dv(frustumPoints[1]);
  GLV->glVertex4dv(frustumPoints[2]);

  GLV->glVertex4dv(frustumPoints[2]);
  GLV->glVertex4dv(frustumPoints[3]);

  GLV->glVertex4dv(frustumPoints[3]);
  GLV->glVertex4dv(frustumPoints[0]);

  //Back
  GLV->glVertex4dv(frustumPoints[4]);
  GLV->glVertex4dv(frustumPoints[5]);

  GLV->glVertex4dv(frustumPoints[5]);
  GLV->glVertex4dv(frustumPoints[6]);

  GLV->glVertex4dv(frustumPoints[6]);
  GLV->glVertex4dv(frustumPoints[7]);

  GLV->glVertex4dv(frustumPoints[7]);
  GLV->glVertex4dv(frustumPoints[4]);

  //Sides
  GLV->glVertex4dv(frustumPoints[0]);
  GLV->glVertex4dv(frustumPoints[4]);

  GLV->glVertex4dv(frustumPoints[3]);
  GLV->glVertex4dv(frustumPoints[7]);

  GLV->glVertex4dv(frustumPoints[1]);
  GLV->glVertex4dv(frustumPoints[5]);

  GLV->glVertex4dv(frustumPoints[2]);
  GLV->glVertex4dv(frustumPoints[6]);

  GLV->glEnd();

}

///////////////////////////////////////////////////////////////////////////////
//
void FCViewCam::RenderFrustumSolid()
{
  GLV->glBegin(GL_QUADS);

  //Front
  GLV->glVertex4dv(frustumPoints[0]);
  GLV->glVertex4dv(frustumPoints[1]);
  GLV->glVertex4dv(frustumPoints[2]);
  GLV->glVertex4dv(frustumPoints[3]);

  //Back
  GLV->glVertex4dv(frustumPoints[4]);
  GLV->glVertex4dv(frustumPoints[5]);
  GLV->glVertex4dv(frustumPoints[6]);
  GLV->glVertex4dv(frustumPoints[7]);

  //Sides
  GLV->glVertex4dv(frustumPoints[0]);
  GLV->glVertex4dv(frustumPoints[1]);
  GLV->glVertex4dv(frustumPoints[5]);
  GLV->glVertex4dv(frustumPoints[4]);

  GLV->glVertex4dv(frustumPoints[3]);
  GLV->glVertex4dv(frustumPoints[2]);
  GLV->glVertex4dv(frustumPoints[6]);
  GLV->glVertex4dv(frustumPoints[7]);

  GLV->glVertex4dv(frustumPoints[1]);
  GLV->glVertex4dv(frustumPoints[5]);
  GLV->glVertex4dv(frustumPoints[6]);
  GLV->glVertex4dv(frustumPoints[2]);

  GLV->glVertex4dv(frustumPoints[0]);
  GLV->glVertex4dv(frustumPoints[4]);
  GLV->glVertex4dv(frustumPoints[7]);
  GLV->glVertex4dv(frustumPoints[3]);

  GLV->glEnd();
}

