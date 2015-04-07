/*=============================================================================
  GLIConfig - Cofiguration for the OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "MainDialog.h"
#include "ConfigOptionsDialog.h"
#include "GLIcon16.xpm"


BEGIN_EVENT_TABLE( MainDialog, wxDialog )

  EVT_CLOSE(MainDialog::OnClose) 

  EVT_COMBOBOX(ID_COMBOBOX_PROFILE, MainDialog::OnProfileSelect) 

  EVT_TEXT(ID_TEXTCTRL_TARGET_FILE, MainDialog::OnTargetFileEntry)

  EVT_BUTTON(ID_BUTTON_TARGET_FILE, MainDialog::OnButtonTargetFile)
  EVT_BUTTON(ID_BUTTON_CONFIG_OPTIONS, MainDialog::OnButtonConfigOptions)
  EVT_BUTTON(ID_BUTTON_INSTALL_CONFIG, MainDialog::OnInstallConfig)

END_EVENT_TABLE()


///////////////////////////////////////////////////////////////////////////////
//
MainDialog::MainDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ):
  targetFileTextCtrl(NULL),
  profileListComboBox(NULL),
  profileDescText(NULL),
  installConfigButton(NULL),
  profileDescScrolledWindow(NULL),
  customProfileUsed(false)
{
  Create(parent, id, caption, pos, size, style);
}

///////////////////////////////////////////////////////////////////////////////
//
bool MainDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  // Set the style and create the controls
  SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  Centre();

  // Set the dialog's icon
  SetIcon(wxIcon(GLIcon16_xpm));

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void MainDialog::CreateControls()
{    
  // DT_TODO: Add tooltips

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(itemBoxSizer2);
  
  // Restrict the min size of the dialog
  itemBoxSizer2->SetMinSize(SYMBOL_MAINDIALOG_SIZE);
  SetMinSize(SYMBOL_MAINDIALOG_SIZE);

  wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
  itemFlexGridSizer3->AddGrowableCol(1);
  itemBoxSizer2->Add(itemFlexGridSizer3, 0, wxGROW|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxStaticText* itemStaticText4 = new wxStaticText( this, wxID_STATIC, _("Target Application"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer3->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

  wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
  itemFlexGridSizer3->Add(itemBoxSizer5, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

  targetFileTextCtrl = new wxTextCtrl( this, ID_TEXTCTRL_TARGET_FILE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer5->Add(targetFileTextCtrl, 1, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

  wxButton* targetFileButton = new wxButton( this, ID_BUTTON_TARGET_FILE, _("..."), wxDefaultPosition, wxSize(25, -1), 0 );
  itemBoxSizer5->Add(targetFileButton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

  wxStaticText* itemStaticText8 = new wxStaticText( this, wxID_STATIC, _("GLIntercept Profile"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer3->Add(itemStaticText8, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

  wxString* itemComboBox9Strings = NULL;
  profileListComboBox = new wxComboBox( this, ID_COMBOBOX_PROFILE, _T(""), wxDefaultPosition, wxDefaultSize, 0, itemComboBox9Strings, wxCB_READONLY );
  itemFlexGridSizer3->Add(profileListComboBox, 1, wxGROW|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

  wxStaticBox* itemStaticBoxSizer10Static = new wxStaticBox(this, wxID_ANY, _("Profile Description"));
  wxStaticBoxSizer* itemStaticBoxSizer10 = new wxStaticBoxSizer(itemStaticBoxSizer10Static, wxHORIZONTAL);
  itemBoxSizer2->Add(itemStaticBoxSizer10, 1, wxGROW|wxALL, 5);

  profileDescScrolledWindow = new wxScrolledWindow( this, ID_SCROLLEDWINDOW1, wxDefaultPosition, wxDefaultSize, wxVSCROLL );
  itemStaticBoxSizer10->Add(profileDescScrolledWindow, 1, wxGROW, 0);
  profileDescScrolledWindow->SetScrollbars(1, 1, 0, 0);

  wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxVERTICAL);
  profileDescScrolledWindow->SetSizer(itemBoxSizer12);

  profileDescText = new wxTextCtrl( profileDescScrolledWindow, wxID_STATIC, _(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE );
  itemBoxSizer12->Add(profileDescText, 1, wxALIGN_TOP|wxEXPAND|wxALL|wxADJUST_MINSIZE, 0);

  wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer14, 0, wxGROW|wxALL, 5);

  wxButton* itemButton15 = new wxButton( this, ID_BUTTON_CONFIG_OPTIONS, _("Advanced Options"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer14->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer14->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxButton* itemButton17 = new wxButton( this, ID_BUTTON5, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer14->Add(itemButton17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  // Add the install config button as initially greyed out
  installConfigButton = new wxButton( this, ID_BUTTON_INSTALL_CONFIG, _("Install Config"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer14->Add(installConfigButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  installConfigButton->Enable(false);

  // Read in the profiles
  ReadDiskProfiles();

  // Fill out the combo box listing
  UpdateProfileDisplay();

  // Select the first profile
  profileListComboBox->SetSelection(0);

  // Update the description
  UpdateProfileDescription();
}

///////////////////////////////////////////////////////////////////////////////
//
void MainDialog::UpdateProfileDisplay()
{
  // Clear out the valuse in the profile display
  profileListComboBox->Clear();

  // Loop for all stored profiles and assign a name based on the short description
  for(uint i=0; i<diskProfiles.size(); i++)
  {
    string profileName = "* Unknown *";
      
    // Attempt to get the profile name
    const ConfigToken * profileNameToken = diskProfiles[i].GetChildToken(string("ProfileName"));
    if(profileNameToken && profileNameToken->GetNumValues() > 0)
    {
      profileNameToken->Get(profileName);
    }

    // Add to the combo box
    profileListComboBox->Append(wxString(profileName.c_str()));
  }

  // Test if the custom profile is active
  if(customProfileUsed)
  {
    profileListComboBox->Append(wxString(wxT("Custom")));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void MainDialog::UpdateProfileDescription()
{
  // Get the selection
  int selectIndex = profileListComboBox->GetSelection();
  if(selectIndex == wxNOT_FOUND)
  {
    return;
  }

  string profileDescString = "* Unknown *";

  // If the select object is a disk profile
  if(selectIndex >=0 && selectIndex < diskProfiles.size())
  {
    // Attempt to get the profile description
    const ConfigToken * profileDescToken = diskProfiles[selectIndex].GetChildToken(string("ProfileDescription"));
    if(profileDescToken && profileDescToken->GetNumValues() > 0)
    {
      profileDescToken->Get(profileDescString);
    }
  }

  // If the last index, must be the custom control
  if(customProfileUsed && selectIndex == diskProfiles.size() )
  {
    profileDescString = "Custom configured profile";
  }

  // Assign the new description string
  profileDescText->SetLabel(wxString(profileDescString.c_str()));

  // Update the sizers to match the new text layout
  if(profileDescScrolledWindow && profileDescScrolledWindow->GetSizer())
  {
    // Fit to the new sizer size
    profileDescScrolledWindow->GetSizer()->Fit(profileDescScrolledWindow);
    
    // Layout the main dialog
    Layout();
  }
}

///////////////////////////////////////////////////////////////////////////////
//
bool MainDialog::ReadDiskProfiles()
{
  wxStandardPaths stdPaths;
  wxFileName dirProcess;

  // Get the main profile directory
  dirProcess.AssignDir(stdPaths.GetDataDir());
  dirProcess.AppendDir("Profiles");
  wxString profileDir = dirProcess.GetPath();

  // Test if the directory exists
  if(!wxDir::Exists(profileDir))
  {
    wxLogWarning("Unable to find profiles directory %s", profileDir.c_str());
    return false;
  }

  // Get the profile filenames
  wxArrayString profileFileNameArray;
  if(wxDir::GetAllFiles(profileDir, &profileFileNameArray, wxString(wxT("*.xni")), wxDIR_FILES) == 0)
  {
    wxLogWarning("No profiles in directory %s", profileDir.c_str());
    return false;
  }

  // Loop for all profiles
  diskProfiles.clear();
  for(uint i=0; i<profileFileNameArray.GetCount(); i++)
  {
    // Load the profile data
    ConfigParser parserLoader;
    if(parserLoader.Parse(string(profileFileNameArray[i].c_str())))
    {
      // Add to the profile array
      diskProfiles.push_back(*parserLoader.GetRootToken());
    }
  } 

  // Should also parse the user data directory
  //wxStandardPaths::GetUserDataDir

  // Check that some profiles were valid
  if(diskProfiles.size() <= 0)
  {
    wxLogWarning("No valid profiles loaded in directory %s", profileDir.c_str());
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void MainDialog::OnClose(wxCloseEvent& WXUNUSED(event))
{
  Destroy();
}

///////////////////////////////////////////////////////////////////////////////
//
void MainDialog::OnButtonTargetFile(wxCommandEvent& WXUNUSED(event))
{
  // Get the path of any currently selected file
  wxString oldDirectory = "";

  // Get the current file (if any) and set as the default
  wxFileName oldFileName(targetFileTextCtrl->GetValue());
  if(oldFileName.FileExists())
  {
    oldDirectory = oldFileName.GetPath();
  }

  // Open the file dialog
  wxFileDialog fileDialog(this, _T("Select the target OpenGL application"), oldDirectory, _T(""), _T("Executables (*.exe)|*.exe|Any (*.*)|*.*"), wxOPEN | wxFILE_MUST_EXIST );
  if (fileDialog.ShowModal() != wxID_OK)
  {
    return;
  }

  // If the selected string is empty, or the file does not exist, abort.
  if (fileDialog.GetPath().IsEmpty() || 
      !wxFileExists(fileDialog.GetPath()))
  {
    return;
  }
  
  // Set the new text in the text edit control
  targetFileTextCtrl->SetValue(fileDialog.GetPath());
}

///////////////////////////////////////////////////////////////////////////////
//
void MainDialog::OnTargetFileEntry(wxCommandEvent& event)
{
  // Grey out the install button based on the text values
  if(targetFileTextCtrl->GetValue().length() > 0)
  {
    installConfigButton->Enable(true);
  }
  else
  {
    installConfigButton->Enable(false);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void MainDialog::OnButtonConfigOptions(wxCommandEvent& WXUNUSED(event))
{
  ConfigOptionsDialog configOptionsDialog(this);

  // Get the current profile data
  ConfigToken currProfile;
  if(!GetCurrentProfile(currProfile))
  {
    wxLogError("No GLIntercept profile selected");
    return;
  }

  // Parse the config file and set the current profile data
  if(configOptionsDialog.ParseConfigOptions(currProfile))
  {
    // Display the config dialog in modal format
    configOptionsDialog.ShowModal();

    // If a UI change needs to occur
    if(configOptionsDialog.IsConfigSaved() || 
       configOptionsDialog.IsCustomConfigSelected())
    {
      // Save the existing selected profile name
      wxString oldProfileName = profileListComboBox->GetValue();
      
      // Re-load the config list if a new config was saved
      if(configOptionsDialog.IsConfigSaved())
      {
        // Read in the profiles
        ReadDiskProfiles();
      }

      // If a change in the config was selected, make the current config a "custom" config
      if(configOptionsDialog.IsCustomConfigSelected())
      {
        customProfileUsed = false;

        // Read in the custom profile
        ConfigToken newConfigData;
        if(configOptionsDialog.GetDisplayConfigData(newConfigData))
        {
          // Assign the new custom profile
          customProfileUsed = true;
          customProfile = newConfigData;
        }
      }

      // Update the profile display
      UpdateProfileDisplay();

      // If the custom profile is selected, select the last entry
      if(configOptionsDialog.IsCustomConfigSelected() && profileListComboBox->GetCount() > 0)
      {
        // Re-select the existing profile (or the custom one)
        profileListComboBox->SetSelection(profileListComboBox->GetCount() - 1);
      }
      else
      {
        // Select the old profile name
        profileListComboBox->SetValue(oldProfileName);
      }

      // Update the description
      UpdateProfileDescription();
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
void MainDialog::OnProfileSelect(wxCommandEvent &event)
{
  UpdateProfileDescription();
}

///////////////////////////////////////////////////////////////////////////////
//
bool MainDialog::GetCurrentProfile(ConfigToken &retProfile) const
{
  // Get the maximum profile index
  uint maxProfileIndex = diskProfiles.size() - 1;
  if(customProfileUsed)
  {
    maxProfileIndex++;
  }

  // Get the current active profile
  int profileSelectIndex = profileListComboBox->GetSelection();
  if(profileSelectIndex == wxNOT_FOUND ||
     profileSelectIndex < 0 ||
     profileSelectIndex > maxProfileIndex)
  {
    return false;
  }

  // Get a copy of the config data
  if(profileSelectIndex < diskProfiles.size())
  {
    retProfile = diskProfiles[profileSelectIndex];
  }
  if(customProfileUsed && profileSelectIndex == diskProfiles.size())
  {
    retProfile = customProfile;
  }

  return true;
}

// DT_TODO: Test all these error conditions

#ifdef GLI_BUILD_WINDOWS
///////////////////////////////////////////////////////////////////////////////
//
void MainDialog::OnInstallConfig(wxCommandEvent &event)
{
  wxStandardPaths stdPaths;

  // Get if the user has supplied a target application
  if(targetFileTextCtrl->GetValue().length() == 0)
  {
    wxLogError("Please select a target application first");
    return;
  }

  // DT_TODO: Accept a directory as a target?
  // Get the target filename and directory
  wxFileName targetFileName(targetFileTextCtrl->GetValue());
  wxString   targetDirectory = targetFileName.GetPath();
  if(!targetFileName.FileExists() || !wxDir::Exists(targetDirectory))
  {
    wxLogError("Unable to find target application %s", targetFileName.GetFullPath().c_str());
    return;
  }

  // Detect if there exists a appname.local file (eg. Google Earth)
  bool copySystemLib = false;
  wxFileName appLocalFile(targetFileTextCtrl->GetValue() + wxT(".local"));
  if(appLocalFile.FileExists())
  {
    wxMessageDialog copySystemLibDialog(this, 
                wxT("The target application is using a *.exe.local file.\n"
                    "GLIntercept needs to make a copy of the system OpenGL32.dll to the target directory and set the config system lib option.\n"
                    "Proceed?"), wxT("Copy system OpenGL?"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if(copySystemLibDialog.ShowModal() == wxID_YES)
    {
      copySystemLib = true;
    }
  }

  // Get the save config string
  wxString saveConfigString;
  if(!GetSaveProfileString(copySystemLib, saveConfigString))
  {
    wxLogError("Unable to generate GLIntercept profile file data");
    return;
  }

  // Calculate the source copy files
  wxFileName srcCopyOpenGL;
  srcCopyOpenGL.AssignDir(stdPaths.GetDataDir());
  srcCopyOpenGL.AppendDir("MainLib");
  srcCopyOpenGL.SetFullName(wxT("opengl32.dll"));

  // Check that the source files exist
  if(!srcCopyOpenGL.FileExists())
  { 
    wxLogError("Unable to copy opengl32.dll (%s) - file does not exist", srcCopyOpenGL.GetFullPath().c_str());     
    return;
  }

  // Calculate the destination files to copy
  wxFileName destCopyOpenGL(targetFileName);
  wxFileName destCopyConfig(targetFileName);

  destCopyOpenGL.SetFullName(wxT("opengl32.dll"));
  destCopyConfig.SetFullName(wxT("gliConfig.ini"));

  // Check to see if any of the writing files already exist
  // DT_TODO: Clean this up, looks messy when displayed
  wxString overwriteFilesMessage;
  if(destCopyOpenGL.FileExists())
  {
    overwriteFilesMessage += wxT("\n  openGL32.dll");
  }
  if(destCopyConfig.FileExists())
  {
    overwriteFilesMessage += wxT("\n  gliConfig.ini");
  }
  if(overwriteFilesMessage.length() > 0)
  {
    // Fill out the complete question
    overwriteFilesMessage = wxString(wxT("The following files already exist. Overwrite them?")) + overwriteFilesMessage;

    // Display the question dialog
    wxMessageDialog overwriteDialog(this, overwriteFilesMessage, wxT("Overwrite existing files?"), wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    if(overwriteDialog.ShowModal() == wxID_NO)
    {
      return;
    }
  }

  // Write the config file
  wxFile configWrite;
  if(!configWrite.Open(destCopyConfig.GetFullPath().c_str(), wxFile::write))
  {
    wxLogError("Unable to open config file (%s)", destCopyConfig.GetFullPath().c_str());     
    return;
  }
  if(!configWrite.Write(saveConfigString))
  {
    wxLogError("Unable to write to config file (%s)", destCopyConfig.GetFullPath().c_str());     
    return;
  }

  // Copy the GLIntercept opengl32.dll
  if(!::wxCopyFile(srcCopyOpenGL.GetFullPath(), destCopyOpenGL.GetFullPath()))
  {
    wxLogError("Unable to copy GLIntercept's opengl32.dll (%s)", srcCopyOpenGL.GetFullPath().c_str());     
    return;
  }

  //DT_TODO: If necessary, copy the system opengl32.dll to the target dir as opengl32.orig.dll


  //DT_TODO: Log a message : The following files have been successfully copied to ...

}
#endif // GLI_BUILD_WINDOWS

 
///////////////////////////////////////////////////////////////////////////////
//
bool MainDialog::GetSaveProfileString(bool copySystemLib, wxString &retString) const
{
  wxStandardPaths stdPaths;

  // Get the current selected profile
  ConfigToken saveProfile;
  if(!GetCurrentProfile(saveProfile))
  {
    wxLogError("No GLIntercept profile selected");
    return false;
  }

  // Get the profile name (if any)
  string profileName = "*Unknown*";
  {
    // Get the profile name if it exists
    const ConfigToken * profileNameToken = saveProfile.GetChildToken("ProfileName");
    if(profileNameToken)
    {
      profileNameToken->Get(profileName);
    }

    // Remove the profile/description tokens
    saveProfile.RemoveChild("ProfileName");
    saveProfile.RemoveChild("ProfileDescription");
  }

  // If copying the OpenGL system library
  if(copySystemLib)
  {
    // Get the input files token (or create it)
    ConfigToken * sourceInputFilesToken = GetCreateChildToken(saveProfile, "InputFiles");
    if(sourceInputFilesToken)
    {
      // Get the system lib token (or create it)
      ConfigToken * syslibToken = GetCreateChildToken(*sourceInputFilesToken, "GLSystemLib");
      if(syslibToken)
      {
        // Set the new override system lib name
        vector<string> newSystemLibName;
        newSystemLibName.push_back("opengl32.orig.dll");

        // Override the existing library name
        syslibToken->SetValueArray(newSystemLibName);
      }
    }
  }
 

  // Replace any string that start with %InstallPath% with the current path
  UpdateTokenValues(saveProfile, "%InstallPath%", stdPaths.GetDataDir().c_str());

  // Generate the config string from the profile
  string stlConfigString;
  for(uint childNum =0;childNum < saveProfile.GetNumChildren(); childNum++)
  {
    //Convert each child back to raw config string data
    const ConfigToken * childData = saveProfile.GetChildToken(childNum);
    if(childData)
    {
      string retString;
      if(ConfigParser::GenerateConfigString(childData, retString))
      {
        stlConfigString += retString;
      }
      else
      {
        wxLogError("Unable to generate GLIntercept configuration file data");
        return false;
      }
    }
  }

  // Add a header comment
  retString.Printf("/*\n*  GLIntercept config file generated by GLIConfig.\n*  Configuration based on profile \"%s\".\n*/\n\n", profileName.c_str());
  retString += stlConfigString.c_str();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
ConfigToken * MainDialog::GetCreateChildToken(ConfigToken &saveProfile, const string &childName) const
{
  // Attempt to get the token name
  ConfigToken * findToken = saveProfile.GetChildToken(childName);
  
  // If the new token does not exist, create it
  if(findToken == NULL)
  {
    // Add the new token
    ConfigToken addToken(childName);
    saveProfile.AddChild(addToken);

    // Get the new child pointer
    findToken = saveProfile.GetChildToken(childName);
  }

  return findToken;
}

///////////////////////////////////////////////////////////////////////////////
//
void MainDialog::UpdateTokenValues(ConfigToken & updateProfile, const string & searchStr, const string &replaceStr) const
{
  // Get the value array in the token
  uint i;
  vector<string> tokenValues;
  updateProfile.GetArray(updateProfile.GetNumValues(), tokenValues);

  // Loop for all values in the token
  for(i=0; i<tokenValues.size(); i++)
  {
    // Loop and find the token values
    string::size_type findPos = tokenValues[i].find(searchStr);
    while(findPos != string::npos)
    {
      // Replace the string
      tokenValues[i].replace(findPos, searchStr.size(), replaceStr);
      
      // Attempt to find more matches
      findPos = tokenValues[i].find(searchStr, findPos + replaceStr.size());
    }
  }

  // Replace the stored token values
  updateProfile.SetValueArray(tokenValues);

  // Loop for all children
  for(i=0; i<updateProfile.GetNumChildren(); i++) 
  {
    // Recurse
    UpdateTokenValues(*updateProfile.GetChildToken(i), searchStr, replaceStr);
  }
}