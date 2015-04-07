/*=============================================================================
  GLIntercept - OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/
#ifndef __SE_INTERFACE_COMMANDS_H_
#define __SE_INTERFACE_COMMANDS_H_

//The types of commands that can be passed between the shader editor plugin and UI
enum CommandType
{
  CT_None = 0,           // No command
  CT_Init,               // Init target window command 
  CT_Ping,               // Is active "ping" command

  CT_ShaderList,         // Request shader list
  CT_ShaderSource,       // Request shader source
  CT_CompileShader,      // Compile shader 
  CT_ShowShader,         // Request that the passed shader be "shown" visually
  CT_Shutdown,           // Shutdown message handling 
  CT_RevertShader,       // Revert shader
  CT_BreakShader,        // Break on shader usage

  CT_SetDebugShader,     // To flag a shader for debugging 
  CT_UnsetDebugShader,   // To un-flag a shader for debugging 

  CT_DebugShaderInit,    // When debugging on a shader starts (sends uniform data, screen image, possibly failure)
  CT_DebugShaderPixel,   // Passed to set the pixel to be debugged
  CT_DebugShaderLine,    // Set the line to be debugged and the return variables
  CT_DebugShaderDone,    // When debugging on the current shader is done
  
  CT_Max                 // Maximum number of commands

};


#endif // __SE_INTERFACE_COMMANDS_H_
