/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2004  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __FC_VIEW_CAM_H_
#define __FC_VIEW_CAM_H_

#include "../../MainLib/InterceptPluginInterface.h"
#include <string>
#include <vector>

#include "FCConfigData.h"

using namespace std;

// Some pre-OGL3.0 defines
#define GL_ALPHA_TEST                           0x0BC0

#define GL_MATRIX_MODE                          0x0BA0
#define GL_MODELVIEW                            0x1700
#define GL_PROJECTION                           0x1701

#define GL_MODELVIEW_MATRIX                     0x0BA6
#define GL_PROJECTION_MATRIX                    0x0BA7

#define GL_POLYGON_MODE                         0x0B40
#define GL_LIGHTING                             0x0B50
#define GL_LIGHT0                               0x4000
#define GL_POSITION                             0x1203
#define GL_SPOT_DIRECTION                       0x1204

#define GL_QUADS                                0x0007


//@
//  Summary:
//    This class enables (attempts) a visualization of the view frustum.
//  
class FCViewCam
{
public:

  //@
  //  Summary:
  //    Constructor. Inits all config data to default values.
  //
  //  Parameters:
  //    gliCallBacks - The callback class for this plugin.
  //
  FCViewCam(InterceptPluginCallbacks *gliCallBacks);

  //@
  //  Summary:
  //    Destructor.
  //
  virtual ~FCViewCam();

  //@
  //  Summary:
  //    To init and test for required OpenGL extension support.
  //    Must be called when OpenGL calls are valid or Init will fail.
  //
  //  Parameters:
  //    configData - The config data to init with.
  //
  void Init(const FCConfigData &configData);


  //@
  //  Summary:
  //    To render the view frustum to the current screen.
  //    The altered model view matrix must be set in the OpenGL state 
  //    before this method is called.
  //
  void RenderViewFrustum();

protected:

  bool   initData;                                //Flag inidicating if init has been called successfully
  bool   extensionVertexProgram;                  //Flag inidicating if ARB/NV vertex programs are supported
  bool   extensionARBGLSLObjects;                 //Flag inidicating if ARB GLSL shader objects are supported
  float  oglVersion;                              //The current OpenGL version 


  InterceptPluginCallbacks *gliCallBacks;         //The callback interface into GLIntercept
  const GLCoreDriver       *GLV;                  //The core OpenGL driver
  
  GLdouble projMatrix[16];                        //The last projection matrix the points were rendered for
  GLdouble frustumPoints[8][4];                   //The points that make up the view frustum for the current projection matrix
  GLenum   setLogicOp;                            //The logic operation to set before rendering 
  
  void     (GLAPIENTRY *iglUseProgram) (GLuint program); //Use GLSL program
  GLhandle (GLAPIENTRY *iglGetHandleARB) (GLenum pname); //Get current ARB GLSL object/program
  void     (GLAPIENTRY *iglUseProgramObjectARB) (GLhandle program); //Use ARB GLSL program

  //@
  //  Summary:
  //    To update the view frustum rendre points by retrieving the current
  //    projection matrix and un-projecting through it.
  //
  void UpdateFrustumPoints();

  //@
  //  Summary:
  //    To render the view frustum with lines. No states are changed.
  //
  void RenderFrustumLines();

  //@
  //  Summary:
  //    To render the view frustum with quads. No states are changed.
  //
  void RenderFrustumSolid();

};



#endif // __FC_VIEW_CAM_H_
