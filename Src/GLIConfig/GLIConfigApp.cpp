/*=============================================================================
  GLIConfig - Cofiguration for the OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/

#include "GLIConfigApp.h"
#include "MainDialog.h"

// Register main application class
IMPLEMENT_APP(GLIConfigApp)

///////////////////////////////////////////////////////////////////////////////
//
bool GLIConfigApp::OnInit()
{
  // Create the main dialog and display it
  MainDialog *dialog = new MainDialog();
  dialog->Show(true);
  return true;
}

