/*=============================================================================
  GLIConfig - Cofiguration for the OpenGL intercept/debugging tool
  Copyright (C) 2006  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/

#include <OSDefines.h>
#include <ConfigParser.h>

#define SYMBOL_MAINDIALOG_STYLE    wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX|wxDIALOG_NO_PARENT
#define SYMBOL_MAINDIALOG_TITLE    _("GLIntercept Config")
#define SYMBOL_MAINDIALOG_IDNAME   10016
#define SYMBOL_MAINDIALOG_SIZE     wxSize(420, 290)
#define SYMBOL_MAINDIALOG_POSITION wxDefaultPosition

#define ID_TEXTCTRL_TARGET_FILE  10019
#define ID_BUTTON_TARGET_FILE    10020
#define ID_COMBOBOX_PROFILE      10018
#define ID_SCROLLEDWINDOW1       10017
#define ID_BUTTON_CONFIG_OPTIONS 10008
#define ID_BUTTON5               10009
#define ID_BUTTON_INSTALL_CONFIG 10010

// DT_TODO: Class checks, cleanup

//@
//  Summary:
//    The main config dialog that selects the target exe and profile.
// 
class MainDialog: public wxDialog
{    
  DECLARE_EVENT_TABLE()

public:

  //@
  //  Summary:
  //    Constructor.
  //
  //  Parameters:
  //    parent  - The parent window (if any)
  //
  //    id      - The window ID of the dialog
  //
  //    caption - The caption to display on the dialog.
  //
  //    pos     - The position of the dialog.
  //
  //    size    - The size of the dialog.
  //
  //    style   - The style flags on the dialog.
  //
  MainDialog( wxWindow* parent = NULL, wxWindowID id = SYMBOL_MAINDIALOG_IDNAME, const wxString& caption = SYMBOL_MAINDIALOG_TITLE, const wxPoint& pos = SYMBOL_MAINDIALOG_POSITION, const wxSize& size = SYMBOL_MAINDIALOG_SIZE, long style = SYMBOL_MAINDIALOG_STYLE );

protected:

  wxTextCtrl          *targetFileTextCtrl;        // The target filename text control
  wxComboBox          *profileListComboBox;       // The list display of profiles
  wxTextCtrl          *profileDescText;           // The profile description display
  wxButton            *installConfigButton;       // The button pressed to install a config 
  wxScrolledWindow    *profileDescScrolledWindow; // The scrolled window housing the profile description

  vector<ConfigToken>  diskProfiles;              // Profiles stored on disk

  bool                 customProfileUsed;         // If the custom profile is used or not
  ConfigToken          customProfile;             // A custom profile not stored on disk

  //@
  //  Summary:
  //    To create the main dialog.
  //
  //  Parameters:
  //    parent  - The parent window (if any)
  //
  //    id      - The window ID of the dialog
  //
  //    caption - The caption to display on the dialog.
  //
  //    pos     - The position of the dialog.
  //
  //    size    - The size of the dialog.
  //
  //    style   - The style flags on the dialog.
  //
  //  Returns:
  //    If creation is successful, true is returned. Else false is returned.
  //
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_MAINDIALOG_IDNAME, const wxString& caption = SYMBOL_MAINDIALOG_TITLE, 
               const wxPoint& pos = SYMBOL_MAINDIALOG_POSITION, const wxSize& size = SYMBOL_MAINDIALOG_SIZE, long style = SYMBOL_MAINDIALOG_STYLE );

  //@
  //  Summary:
  //    To create the controls and sizers on the dialog.
  //
  void CreateControls();

  //@
  //  Summary:
  //    To read the preconfigured profiles on disk and store them internally.
  //
  //  Returns:
  //    If some profiles were read successfully, true is returned. Else false 
  //    is returned.
  //
  bool ReadDiskProfiles();

  //@
  //  Summary:
  //    To update the list of profiles in the combobox select to reflect the
  //    current data.
  //
  void UpdateProfileDisplay();

  //@
  //  Summary:
  //    To update the profile description display to reflect the currently
  //    selected profile.
  //
  void UpdateProfileDescription();

  //@
  //  Summary:
  //    To get the current selected profile.
  //
  //  Parameters:
  //    retProfile - The profile data to return.
  //
  //  Returns:
  //    If a profile is selected, true is returned and retProfile is set. 
  //    Else false is return.
  //
  bool GetCurrentProfile(ConfigToken &retProfile) const;

  //@
  //  Summary:
  //    To get the current selected profile as a saving string.
  //
  //  Parameters:
  //    copySystemLib - If the profile requires the system OpenGL library
  //                    copied. (Windows only)
  //
  //    retString - The return profile data as a string.
  //
  //  Returns:
  //    If a profile is selected and it could be converted to a string, true
  //    is returned and retString is set. Else false is return.
  //
  bool GetSaveProfileString(bool copySystemLib, wxString &retString) const;

  //@
  //  Summary:
  //    To get the child token of the passed name, if the token name does not
  //    exist, it is created before being returned.
  //
  //  Parameters:
  //    saveProfile - The profile to get the child from.
  //
  //    childName - The name of the child token to get.
  //
  //  Returns:
  //    A child token of the specified name is returned.
  //
  ConfigToken * GetCreateChildToken(ConfigToken &saveProfile, const string &childName) const;

  //@
  //  Summary:
  //    To update the passed token values and update any occurences of
  //    "searchStr" in the data values with "replaceStr".
  //
  //  Parameters:
  //    updateProfile - The profile update the values in.
  //
  //    searchStr - The string to replace in the data tokens.
  //
  //    replaceStr - The replacement string for the data tokens.
  //
  void UpdateTokenValues(ConfigToken & updateProfile, const string & searchStr, const string &replaceStr) const;

  // Event handling
  void OnClose(wxCloseEvent& event);
  void OnTargetFileEntry(wxCommandEvent& event);
  void OnButtonTargetFile(wxCommandEvent& event);
  void OnButtonConfigOptions(wxCommandEvent& event);
  void OnProfileSelect(wxCommandEvent &event);
  void OnInstallConfig(wxCommandEvent &event);

};

