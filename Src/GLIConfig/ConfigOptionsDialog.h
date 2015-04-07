/*=============================================================================
  GLIConfig - Cofiguration for the OpenGL intercept/debugging tool
  Copyright (C) 2007  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/

#include <OSDefines.h>
#include <wx/propgrid/propgrid.h>

#include <string>
#include <vector>

// UI defines for dislogs/controls
#define SYMBOL_CONFIG_OPTIONS_DIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_CONFIG_OPTIONS_DIALOG_TITLE _("Config options")
#define SYMBOL_CONFIG_OPTIONS_DIALOG_IDNAME 10021
#define SYMBOL_CONFIG_OPTIONS_DIALOG_SIZE wxSize(525, 750)
#define SYMBOL_CONFIG_OPTIONS_DIALOG_POSITION wxDefaultPosition
#define ID_PROPGRID_CTRL     10022
#define ID_BUTTON_SAVEAS     10023
#define ID_BUTTON_CANCEL     10024
#define ID_BUTTON_DONE       10025

using namespace std;

class wxPropertyGrid;
class ConfigToken;
class wxFileName;

//@
//  Summary:
//    The config selection types
// 
enum ConfigType
{
  CT_String = 0,     // A single string
  CT_StringList,     // A string list

  CT_Boolean,        // Boolean value
  CT_Int,            // Integer value
  CT_UInt,           // Unsigned integer
  CT_Float,          // Floating point
  CT_Enum,           // Single enumeration 
  CT_EnumMulti,      // Multi-Select enumeration 

  CT_DirSelect,      // Directory selection
  CT_FileSelect,     // File selection
  CT_KeyCombination, // Key combination selection

  CT_MaxNumTypes
};

//@
//  Summary:
//    The data stored for a selectable config option.
// 
struct ConfigSelectValue
{
  ConfigSelectValue();

  string     typeName;   // The name of the config value
  ConfigType configType; // The config type

  vector<string> defaultValue;   // The default value(s) of the config
  string         commentString;  // The comment string for the value

  vector<string> enumValueTypes; // If this is a enum type, the listing of the valid enum values

  bool   useIgnoreValue;         // If this select value has an ignore value
  string ignoreIfValueEquals;    // The ignore value (useIgnoreValue must be true to be used) 

  bool   isValueDisplayed;       // If the value is to be displayed by default
};

//@
//  Summary:
//    A header value to group arrays of selectable values
// 
struct ConfigHeaderValue
{
  string headerName;                           // The name of the header

  vector<ConfigHeaderValue> childHeaderArray;  // Array of child sub-headers
  vector<ConfigSelectValue> childSelectArray;  // Array of child select values
};


//@
//  Summary:
//    A plugin value to group the config data in a plugin
// 
struct PluginConfigValue : public ConfigHeaderValue
{
  string pluginLoadString;                     // The plugin load string
};


//@
//  Summary:
//    This class provides a display dialog to edit the GLIntercept properties.
// 
class ConfigOptionsDialog: public wxDialog
{    
  DECLARE_EVENT_TABLE()

public:

  //@
  //  Summary:
  //    Constructor
  // 
  ConfigOptionsDialog(wxWindow* parent, wxWindowID id = SYMBOL_CONFIG_OPTIONS_DIALOG_IDNAME, const wxString& caption = SYMBOL_CONFIG_OPTIONS_DIALOG_TITLE, const wxPoint& pos = SYMBOL_CONFIG_OPTIONS_DIALOG_POSITION, const wxSize& size = SYMBOL_CONFIG_OPTIONS_DIALOG_SIZE, long style = SYMBOL_CONFIG_OPTIONS_DIALOG_STYLE);

  //@
  //  Summary:
  //    To set the passed profile as the displayed profile data
  // 
  //  Parameters:
  //    setProfile - The source profile data
  //
  //  Returns:
  //    If the options could be parsed and the dialog displayed, true is
  //    returned. Else false is returned.
  //
  bool ParseConfigOptions(const ConfigToken &setProfile);

  //@
  //  Summary:
  //    To get if a custom config has been selected.
  //    (If any profile data has been changed)
  // 
  //  Returns:
  //    If a custom config has been selected, true is returned. Else false
  //    is returned.
  //
  inline bool IsCustomConfigSelected() const;

  //@
  //  Summary:
  //    To get if a new config has been saved to disk
  //
  //  Returns:
  //    If a new profile has been saved, true is returned. Else false is
  //    returned.
  //
  inline bool IsConfigSaved() const;

  //@
  //  Summary:
  //    To get the currently displayed config data
  // 
  //  Parameters:
  //    newConfigData - The return config data structure.
  //
  //  Returns:
  //    If the config data could be retreived, true is
  //    returned. Else false is returned.
  //
  bool GetDisplayConfigData(ConfigToken & newConfigData) const;

protected:

  wxPropertyGridManager* propGrid;            // The property grid display

  ConfigHeaderValue         gridValues;       // The root grid values
  vector<PluginConfigValue> pluginGridValues; // The plugin grid values

  bool isCustomConfigSelected;                // If a new custom config has been selected 
  bool isConfigSaved;                         // If a new config has been saved 

  //@
  //  Summary:
  //    Dialog creation.
  // 
  //  Returns:
  //    If the dialog has been init successfully, true is
  //    returned. Else false is returned.
  //
  bool Create(wxWindow* parent, wxWindowID id = SYMBOL_CONFIG_OPTIONS_DIALOG_IDNAME, const wxString& caption = SYMBOL_CONFIG_OPTIONS_DIALOG_TITLE, const wxPoint& pos = SYMBOL_CONFIG_OPTIONS_DIALOG_POSITION, const wxSize& size = SYMBOL_CONFIG_OPTIONS_DIALOG_SIZE, long style = SYMBOL_CONFIG_OPTIONS_DIALOG_STYLE);

  //@
  //  Summary:
  //    Creates the controls and sizers when creating the dialog.
  // 
  void CreateControls();

  //@
  //  Summary:
  //    Parse the supplied config file and add to the current gridValues.
  // 
  //  Parameters:
  //    parseFileName - The filename of the file to parse.
  //
  //    isPlugin      - If the filename is a plugin config file.
  //
  //  Returns:
  //    If the config file could be parsed, true is returned. Else false
  //    is returned.
  //
  bool ParseConfigFile(const wxFileName & parseFileName, bool isPlugin);

  //@
  //  Summary:
  //    Extract a plugin definition and add it to the pluginGridValues array.
  // 
  //  Parameters:
  //    addTokenData - The token containing the values to extract.
  //
  //  Returns:
  //    If the token data could be extracted, true is returned. Else false
  //    is returned.
  //
  bool ExtractConfigPlugins(const ConfigToken * addTokenData);

  //@
  //  Summary:
  //    Extract a header definition and add it to the passed headerValue.
  // 
  //  Parameters:
  //    headerValue  - The header value to add the token data to.
  //
  //    addTokenData - The token containing the values to extract.
  //
  //  Returns:
  //    If the token data could be extracted, true is returned. Else false
  //    is returned.
  //
  bool ExtractConfigHeaders(ConfigHeaderValue & headerValue, const ConfigToken * addTokenData) const;

  //@
  //  Summary:
  //    Extract a selection definition and add it to the passed selectValue.
  // 
  //  Parameters:
  //    selectValue - The select value structure to add the token data to.
  //
  //    addTokenData - The token containing the values to extract.
  //
  //  Returns:
  //    If the token data could be extracted, true is returned. Else false
  //    is returned.
  //
  bool ExtractConfigSelectValue(ConfigSelectValue & selectValue, const ConfigToken * addTokenData) const;

  //@
  //  Summary:
  //    Extract a selection value type and assign it to the passed selectValue.
  // 
  //  Parameters:
  //    selectValue - The select value structure to set the value type in.
  //
  //    valueTypeString - The value type string.
  //
  //  Returns:
  //    If the value string is valid, true is returned and the type is
  //    assigned. Else false is returned.
  //
  bool ExtractConfigValueType(ConfigSelectValue &selectValue, const string & valueTypeString) const;

  //@
  //  Summary:
  //    To set the profile data as the default values stored in the passed 
  //    headerToken structure. (called recursively to set child and plugin
  //    values) 
  // 
  //  Parameters:
  //    setProfile  - The source values to set.
  //
  //    headerToken - The token to set the values in.
  //
  //  Returns:
  //    If no errors occured during setting, true is returned. Else
  //    false is returned.
  //
  bool SetProfileData(const ConfigToken &setProfile, ConfigHeaderValue & headerToken);

  //@
  //  Summary:
  //    To add a new header value to the dialog property grid display. 
  // 
  //  Parameters:
  //    headerValue     - The new header value to set.
  //
  //    parentHeaderID  - The parent property grid value (if not root).
  //
  //    isRootHeader    - If this header is the root header.
  //
  //    parentNamePath  - The parent names seperated by a '.'.
  //
  //  Returns:
  //    If no errors occured during setting, true is returned. Else
  //    false is returned.
  //
  bool AddPropGridHeader(ConfigHeaderValue & headerValue, wxPGId parentHeaderID, bool isRootHeader, const string & parentNamePath);

  //@
  //  Summary:
  //    To add a new select value to the dialog property grid display. 
  // 
  //  Parameters:
  //    headerValue     - The new header select value to set.
  //
  //    parentHeaderID  - The parent property value ID.
  //
  //    parentNamePath  - The parent names seperated by a '.'.
  //
  //  Returns:
  //    If no errors occured during setting, true is returned. Else
  //    false is returned.
  //
  bool AddPropGridSelectValue(ConfigSelectValue & headerValue, wxPGId parentHeaderID, const string & parentNamePath);
  
  //@
  //  Summary:
  //    To get a grid property value as an array of strings.
  // 
  //  Parameters:
  //    selectValue   - The property select value.
  //
  //    selectPropID  - The property grid value ID.
  //
  //    retValues     - The return array of strings.
  //
  //  Returns:
  //    If the values could be retrieved, true is returned. Else
  //    false is returned.
  //
  bool GetPropGridSelectValue(const ConfigSelectValue & selectValue, wxPGId selectPropID, vector<string> &retValues) const;

  //@
  //  Summary:
  //    To recurse and extract the display config data in config token form.
  // 
  //  Parameters:
  //    newConfigData  - The return config data.
  //
  //    headerValue    - The source header value data to extract from the
  //                     property grid display.
  //
  //    parentNamePath - The path name to the parent token
  //
  //  Returns:
  //    If the values could be retrieved, true is returned. Else
  //    false is returned.
  //
  bool GetDisplayConfigData(ConfigToken & newConfigData, const ConfigHeaderValue & headerValue, const string & parentNamePath) const;

  // Callbacks
  void OnButtonSaveAs(wxCommandEvent& event);
  void OnButtonCancel(wxCommandEvent& event);
  void OnButtonDone(wxCommandEvent &event);
  void OnPropertyGridChange(wxPropertyGridEvent& event);

};

///////////////////////////////////////////////////////////////////////////////
//
inline bool ConfigOptionsDialog::IsCustomConfigSelected() const
{
  return isCustomConfigSelected;
}

///////////////////////////////////////////////////////////////////////////////
//
inline bool ConfigOptionsDialog::IsConfigSaved() const
{
  return isConfigSaved;
}
