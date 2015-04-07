/*=============================================================================
  GLIConfig - Cofiguration for the OpenGL intercept/debugging tool
  Copyright (C) 2007  Damian Trebilco

  Licensed under the MIT license - See Docs\license.txt for details.
=============================================================================*/

#include "wx/wx.h"
#include "ConfigOptionsDialog.h"
#include <ConfigParser.h>

#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>
#include <wx/stdpaths.h>

// The name of the special case header for plugins
const string CONFIG_PLUGINS_HEADER_NAME = string("Plugins");
const string CONFIG_PLUGIN_ENABLE_NAME  = string("PluginEnabled");
const string CONFIG_PLUGIN_TOKEN_TRUE   = string("True");
const string CONFIG_PLUGIN_TOKEN_FALSE  = string("False");

// Callback dialog event table
BEGIN_EVENT_TABLE( ConfigOptionsDialog, wxDialog )

  EVT_BUTTON(ID_BUTTON_SAVEAS, ConfigOptionsDialog::OnButtonSaveAs)
  EVT_BUTTON(ID_BUTTON_CANCEL, ConfigOptionsDialog::OnButtonCancel)
  EVT_BUTTON(ID_BUTTON_DONE, ConfigOptionsDialog::OnButtonDone)

  EVT_PG_CHANGED(ID_PROPGRID_CTRL, ConfigOptionsDialog::OnPropertyGridChange)

END_EVENT_TABLE()

//@
//  Summary:
//    The config selection type mapping to strings
// 
struct ConfigTypeStrMap
{
  ConfigTypeStrMap(ConfigType newType, const string & newTypeString);

  ConfigType type;       // The value type
  string     typeString; // The string version of the type
};

// The mapping of value types to strings
ConfigTypeStrMap configTypeStrMap[CT_MaxNumTypes] =
{
  ConfigTypeStrMap(CT_String,    "String"),
  ConfigTypeStrMap(CT_StringList,"StringList"),

  ConfigTypeStrMap(CT_Boolean,   "Boolean"),     
  ConfigTypeStrMap(CT_Int,       "Int"),     
  ConfigTypeStrMap(CT_UInt,      "UInt"), 
  ConfigTypeStrMap(CT_Float,     "Float"),   

  ConfigTypeStrMap(CT_Enum,      "Enum"),          
  ConfigTypeStrMap(CT_EnumMulti, "EnumMulti"),          

  ConfigTypeStrMap(CT_DirSelect,      "DirSelect"),
  ConfigTypeStrMap(CT_FileSelect,     "FileSelect"),
  ConfigTypeStrMap(CT_KeyCombination, "KeyCombination"),
};

// Helper class to get the list of direct sub-directories. (Not sub-sub directories etc)
class DirListTraverse : public wxDirTraverser
{
public:
  DirListTraverse(wxArrayString& retDirfiles); 

  // When a file is traversed, ignore and continue
  virtual wxDirTraverseResult OnFile(const wxString& filename);

  // When a directory is traversed, store it and do not process any files or sub-directories
  virtual wxDirTraverseResult OnDir(const wxString& dirname);

private:

  wxArrayString& dirArray;  // The cached array of directories
};

///////////////////////////////////////////////////////////////////////////////
//
DirListTraverse::DirListTraverse(wxArrayString& retDirfiles) :
  dirArray(retDirfiles)
{
}

///////////////////////////////////////////////////////////////////////////////
//
wxDirTraverseResult DirListTraverse::OnFile(const wxString& WXUNUSED(filename))
{
  return wxDIR_CONTINUE;
}

///////////////////////////////////////////////////////////////////////////////
//
wxDirTraverseResult DirListTraverse::OnDir(const wxString& dirname)
{
  // Store the directory 
  dirArray.Add(dirname);

  // Ignore any sub-directories or files
  return wxDIR_IGNORE;
}

///////////////////////////////////////////////////////////////////////////////
//
ConfigTypeStrMap::ConfigTypeStrMap(ConfigType newType, const string & newTypeString) :
  type(newType),
  typeString(newTypeString)
{
}

///////////////////////////////////////////////////////////////////////////////
//
ConfigSelectValue::ConfigSelectValue():
  configType(CT_Boolean),
  useIgnoreValue(false),
  isValueDisplayed(false)
{
}

///////////////////////////////////////////////////////////////////////////////
//
ConfigOptionsDialog::ConfigOptionsDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) :
  propGrid(NULL),
  isCustomConfigSelected(false),
  isConfigSaved(false)
{
  Create(parent, id, caption, pos, size, style);
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create(parent, id, caption, pos, size, style);

  CreateControls();
  Centre();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigOptionsDialog::CreateControls()
{    
  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  this->SetSizer(itemBoxSizer2);

  // Construct wxPropertyGrid control (Look at additional styles for this control?)
  propGrid = new wxPropertyGridManager(this, ID_PROPGRID_CTRL, wxDefaultPosition, wxDefaultSize, wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE | wxPG_DESCRIPTION);
  itemBoxSizer2->Add(propGrid, 1, wxGROW|wxALL, 5);

  // Add a default page to the property grid manager
  propGrid->AddPage(wxT("Config Options"));

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

  // Create the option buttons
  wxButton* itemButton5 = new wxButton( this, ID_BUTTON_SAVEAS, _("Save as..."), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxButton* itemButton7 = new wxButton( this, ID_BUTTON_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  wxButton* itemButton8 = new wxButton( this, ID_BUTTON_DONE, _("Done"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::ParseConfigOptions(const ConfigToken &setProfile)
{
  wxStandardPaths stdPaths;

  // Reset any grid values
  gridValues.childHeaderArray.clear();
  gridValues.childSelectArray.clear();

  // Get the directory that contains the main options
  wxFileName mainConfigDef;
  mainConfigDef.AssignDir(stdPaths.GetDataDir());
  mainConfigDef.AppendDir("MainLib");
  mainConfigDef.SetFullName(wxT("gliConfig_Definition.ini"));

  // Parse the main config file
  if(!ParseConfigFile(mainConfigDef, false))
  {
    return false;
  }

  // Get the plugin directory
  wxFileName pluginDir;
  pluginDir.AssignDir(stdPaths.GetDataDir());
  pluginDir.AppendDir("Plugins");
  wxDir searchDir(pluginDir.GetFullPath());
  
  // Search for all directories under the "Plugins" directory
  wxArrayString foundDirs;
  DirListTraverse dirTraverse(foundDirs);
  searchDir.Traverse(dirTraverse);

  // Loop for all plugins directories and get any plugin options  
  for(uint i=0; i<foundDirs.size(); i++)
  {
    wxFileName pluginConfigDef;
    pluginConfigDef.AssignDir(foundDirs[i]);
    pluginConfigDef.SetFullName(wxT("config_Definition.ini"));

    // If the file exists
    if(pluginConfigDef.FileExists())
    {
      // Parse the main config file
      if(!ParseConfigFile(pluginConfigDef, true))
      {
        return false;
      }
    }
    else
    {
      wxLogWarning("Unable to find plugin config definition (%s) - file does not exist", pluginConfigDef.GetFullPath().c_str());
    }
  }

  // Set the defualts from the passed config options
  if(!SetProfileData(setProfile, gridValues))
  { 
    wxLogError("Unable to set selected config options");     
    return false;
  }
   
  // Lock and clear the property grid 
  propGrid->Freeze();

  // Add the options to the property grid
  string parentPath("");
  if(!AddPropGridHeader(gridValues, 0, true, parentPath))
  {
    propGrid->Thaw();
    return false;
  }
  
  // Release the property grid
  propGrid->Thaw();

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::ParseConfigFile(const wxFileName & parseFileName, bool isPlugin)
{
  // Check that the config definition file exists
  if(!parseFileName.FileExists())
  { 
    wxLogError("Unable to find config definition (%s) - file does not exist", parseFileName.GetFullPath().c_str());     
    return false;
  }

  // Parse the file
  ConfigParser parserLoader;
  if(!parserLoader.Parse(string(parseFileName.GetFullPath().c_str())))
  {
    wxLogError("Error in parsing (%s)", parseFileName.GetFullPath().c_str());     
    return false;
  }

  // If this is a plugin, add to the plugin array
  if(isPlugin)
  {
    if(!ExtractConfigPlugins(parserLoader.GetRootToken()))
    {
      return false;
    }
  }
  else
  {
    // Create the main config options from the file
    if(!ExtractConfigHeaders(gridValues, parserLoader.GetRootToken()))
    {
      return false;
    }
  }

  // Log any unused tokens
  parserLoader.LogUnusedTokens();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::AddPropGridHeader(ConfigHeaderValue & headerValue, wxPGId parentHeaderID, bool isRootHeader, const string & parentNamePath)
{
  uint i;

  // If this header is expanded or not
  bool expandHeader = false;

  // Add all the data values
  for(i=0; i<headerValue.childSelectArray.size(); i++)
  {
    // Add a select value (pass the header name down)
    if(!AddPropGridSelectValue(headerValue.childSelectArray[i], parentHeaderID, parentNamePath))
    {
      return false;
    }

    // Determine if this header is to be expanded
    if(headerValue.childSelectArray[i].isValueDisplayed)
    {  
      expandHeader = true;
    }
  }

  // Add all the header values
  for(i=0; i<headerValue.childHeaderArray.size(); i++)
  {
    // Add a new header to the parent control
    wxPGId newPID;
    if(isRootHeader)
    {
      // Add a root header
      newPID = propGrid->Append(wxPropertyCategory(headerValue.childHeaderArray[i].headerName.c_str()));
    }
    else
    {
      // Add a sub-header
      newPID = propGrid->AppendIn(parentHeaderID, wxPropertyCategory(headerValue.childHeaderArray[i].headerName.c_str()));
    }

    // Calculate the child path
    string childNamePath = parentNamePath + string(".") + headerValue.childHeaderArray[i].headerName;

    // Recurse
    if(!AddPropGridHeader(headerValue.childHeaderArray[i], newPID, false, childNamePath))
    {
      return false;
    }

    // If the child is expanded, expand this header
    if(propGrid->IsPropertyExpanded(newPID))
    {
      expandHeader = true;
    }
  }

  // If this is the plugins header
  if(headerValue.headerName == CONFIG_PLUGINS_HEADER_NAME)
  {
    // Loop for each plugin
    for(i=0; i<pluginGridValues.size(); i++)
    {
      // Add the plugin header section
      wxPGId newPID = propGrid->AppendIn(parentHeaderID, wxPropertyCategory(pluginGridValues[i].headerName.c_str()));

      // Calculate the child path
      string childNamePath = parentNamePath + string(".") + pluginGridValues[i].headerName;

      // Add a section for each plugin
      if(!AddPropGridHeader(pluginGridValues[i], newPID, false, childNamePath))
      {
        return false;
      }

      // If the child is expanded, expand this header
      if(propGrid->IsPropertyExpanded(newPID))
      {
        expandHeader = true;
      }
    }    
  }

  // If this is not the root
  if(!isRootHeader)
  {
    // If the header is to be expanded or not
    if(expandHeader)
    {
      propGrid->Expand(parentHeaderID);
    }
    else
    {
      propGrid->Collapse(parentHeaderID);
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::AddPropGridSelectValue(ConfigSelectValue & selectValue, wxPGId parentHeaderID, const string & parentNamePath)
{
  wxPGId addID = 0;

  // Calculate the child path
  string selectNamePath = parentNamePath + string(".") + selectValue.typeName;

  // Add a selection based on the type
  switch(selectValue.configType)
  {
    case(CT_String):
      {
        // Get the default value
        wxString defaultValue;
        if(selectValue.defaultValue.size() > 0)
        {
          defaultValue = selectValue.defaultValue[0].c_str();
        }

        // Add a string property
        addID = propGrid->AppendIn(parentHeaderID, wxStringProperty(selectValue.typeName.c_str(), selectNamePath.c_str(), defaultValue));
      }
      break;

    case(CT_KeyCombination):
    case(CT_StringList):
      {
        // Get the default values
        wxArrayString defaultValues;
        for(uint i=0; i<selectValue.defaultValue.size(); i++)
        {
          defaultValues.Add(selectValue.defaultValue[i].c_str());
        }

        // Add a string-list property
        addID = propGrid->AppendIn(parentHeaderID, wxArrayStringProperty(selectValue.typeName.c_str(), selectNamePath.c_str(), defaultValues));
      }
      break;

    case(CT_Boolean):
      {
        // Get the default value
        bool defaultValue = false;
        if(selectValue.defaultValue.size() > 0)
        {
          // If the default value is true
          if(selectValue.defaultValue[0] == CONFIG_PLUGIN_TOKEN_TRUE)
          {
            defaultValue = true;
          }
          else if(selectValue.defaultValue[0] == CONFIG_PLUGIN_TOKEN_FALSE)
          {
            defaultValue = false;
          }
          else
          {
            wxLogError("AddPropGridSelectValue - Unknown boolean value \"%s\"", selectValue.defaultValue[0].c_str());
            return false;
          }
        }

        // Add a boolean selection
        addID = propGrid->AppendIn(parentHeaderID, wxBoolProperty(selectValue.typeName.c_str(), selectNamePath.c_str(), defaultValue));
      }
      break;

    case(CT_Int):
      {
        // Get the default value
        int defaultValue = 0;
        if(selectValue.defaultValue.size() > 0)
        {
          defaultValue = atoi(selectValue.defaultValue[0].c_str());
        }

        // Add a int property
        addID = propGrid->AppendIn(parentHeaderID, wxIntProperty(selectValue.typeName.c_str(), selectNamePath.c_str(), defaultValue));
      }
      break;

    case(CT_UInt):
      {
        // Get the default value
        unsigned int defaultValue = 0;
        if(selectValue.defaultValue.size() > 0)
        {
          int testValue = atoi(selectValue.defaultValue[0].c_str());
          if(testValue >= 0)
          {
            defaultValue = testValue;
          }
        }

        // Add a uint property
        addID = propGrid->AppendIn(parentHeaderID, wxUIntProperty(selectValue.typeName.c_str(), selectNamePath.c_str(), defaultValue));
      }
      break;

    case(CT_Float):
      {
        // Get the default value
        float defaultValue = 0.0f;
        if(selectValue.defaultValue.size() > 0)
        {
          defaultValue = atof(selectValue.defaultValue[0].c_str());
        }

        // Add a float property
        addID = propGrid->AppendIn(parentHeaderID, wxFloatProperty(selectValue.typeName.c_str(), selectNamePath.c_str(), defaultValue));
      }
      break;

    case(CT_Enum):
      {
        // Get the default value
        wxArrayString enumValues;
        int defaultValue = 0;
        for(uint i=0; i<selectValue.enumValueTypes.size(); i++)
        {
          enumValues.Add(selectValue.enumValueTypes[i].c_str());

          // Check if the value is the default value
          if(selectValue.defaultValue.size() > 0 &&
             selectValue.defaultValue[0] == selectValue.enumValueTypes[i])
          {
            defaultValue = i;
          }
        }

        // Add a enum property
        addID = propGrid->AppendIn(parentHeaderID, wxEnumProperty(selectValue.typeName.c_str(), selectNamePath.c_str(), enumValues, defaultValue));
      }
      break;


    case(CT_EnumMulti):
      {
        // Get the default value
        wxArrayString enumValues;
        wxArrayInt defaultValues;
        for(uint i=0; i<selectValue.enumValueTypes.size(); i++)
        {
          enumValues.Add(selectValue.enumValueTypes[i].c_str());

          // Check if the value is the default value
          for(uint i2=0; i2<selectValue.defaultValue.size(); i2++)
          {
            // Compare the default value to the enum value
            if(selectValue.defaultValue[i2] == selectValue.enumValueTypes[i])
            {
              defaultValues.Add(i);
              break;
            }
          }
        }

        // Add a multiselect enum property
        addID = propGrid->AppendIn(parentHeaderID, wxMultiChoiceProperty(selectValue.typeName.c_str(), selectNamePath.c_str(), enumValues, defaultValues));
      }
      break;

    case(CT_DirSelect):
      {
        // Get the default value
        wxString defaultValue;
        if(selectValue.defaultValue.size() > 0)
        {
          defaultValue = selectValue.defaultValue[0].c_str();
        }

        // Add a directory select property
        addID = propGrid->AppendIn(parentHeaderID, wxDirProperty(selectValue.typeName.c_str(), selectNamePath.c_str(), defaultValue));
      }
      break;

    case(CT_FileSelect):
      {
        // Get the default value
        wxString defaultValue;
        if(selectValue.defaultValue.size() > 0)
        {
          defaultValue = selectValue.defaultValue[0].c_str();
        }

        // Add a file select property
        addID = propGrid->AppendIn(parentHeaderID, wxFileProperty(selectValue.typeName.c_str(), selectNamePath.c_str(), defaultValue));
      }
      break;

    default:
      wxLogError("AddPropGridSelectValue - Unknown type");
      return false;
  }

  // Assign the comment string
  wxString commentStr = selectValue.commentString.c_str();
  commentStr.Replace("\\n", "\n");
  propGrid->SetPropertyHelpString(addID, commentStr);

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::GetPropGridSelectValue(const ConfigSelectValue & selectValue, wxPGId selectPropID, vector<string> &retValues) const
{
  // Clear the return array
  retValues.clear();

  // Check the passed value
  if(!wxPGIdIsOk(selectPropID))
  {
    wxLogError("GetPropGridSelectValue - Invlaid property ID");
    return false;
  }

  // Get the value based on the type
  switch(selectValue.configType)
  {
    case(CT_Enum):
    case(CT_DirSelect):
    case(CT_FileSelect):
    case(CT_String):
      {
        // Get the string value
        wxString propValue = propGrid->GetPropertyValueAsString(selectPropID);
        retValues.push_back(string(propValue.c_str())); 
      }
      break;

    case(CT_EnumMulti):
      {
        // Get the string value
        wxString propValue = propGrid->GetPropertyValueAsString(selectPropID);
        
        // Break the return string up by " "
        string addString;
        bool   inAddString = false;
        for(uint i=0; i<propValue.size(); i++)
        {
          // If the string is the seperator
          if(propValue[i] == '"')
          {
            // If in some quotes, add it to the final array
            if(inAddString)
            {
              // Add the new string
              retValues.push_back(addString);
            }
            else
            {
              // Start a new string
              addString = "";
            }

            // Toggle if we are in or out of a string
            inAddString = !inAddString; 
          }
          else
          {
            addString += propValue[i];
          }
        }
      }
      break;

    case(CT_KeyCombination):
    case(CT_StringList):
      {
        // Get the list of strings and convert it to the return array
        const wxArrayString& propValueArray = propGrid->GetPropertyValueAsArrayString(selectPropID);
        for(uint i=0; i<propValueArray.size(); i++)
        {
          retValues.push_back(string(propValueArray[i].c_str())); 
        }
      }
      break;

    case(CT_Boolean):
      {
        // Get the boolean value
        if(propGrid->GetPropertyValueAsBool(selectPropID))
        {
          retValues.push_back(CONFIG_PLUGIN_TOKEN_TRUE);
        }
        else
        {
          retValues.push_back(CONFIG_PLUGIN_TOKEN_FALSE);
        }
      }
      break;

    case(CT_Int):
      {
        // Get the long value
        long propValue = propGrid->GetPropertyValueAsLong(selectPropID);
        
        // Convert it to a string and add it to the return array
        wxString propValueStr;
        propValueStr.Printf("%d",(int)propValue); 
        retValues.push_back(string(propValueStr.c_str()));
      }
      break;

    case(CT_UInt):
      {
        // Get the long value
        long propValue = propGrid->GetPropertyValueAsLong(selectPropID);
        
        // Convert it to a string and add it to the return array
        wxString propValueStr;
        propValueStr.Printf("%u",(uint)propValue); 
        retValues.push_back(string(propValueStr.c_str()));
      }
      break;

    case(CT_Float):
      {
        // Get the double value
        double propValue = propGrid->GetPropertyValueAsDouble(selectPropID);
        
        // Convert it to a string and add it to the return array
        wxString propValueStr;
        propValueStr.Printf("%f", propValue); 
        retValues.push_back(string(propValueStr.c_str()));
      }
      break;

    default:
      wxLogError("GetPropGridSelectValue - Unknown type");
      return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::ExtractConfigPlugins(const ConfigToken * addTokenData)
{
  if(!addTokenData)
  {
    wxLogError("ExtractConfigPlugins - NULL token data");
    return false;
  }

  // Loop for all config children
  for(uint i=0; i<addTokenData->GetNumChildren(); i++)
  {
    const ConfigToken * childToken = addTokenData->GetChildToken(i);

    // If the child has a value, create a new select value 
    if(childToken->GetNumValues() == 1)
    {
      // Else, assume a header and create a new header value
      PluginConfigValue newPluginValue;

      // Assign the name
      newPluginValue.headerName = childToken->GetName();
   
      // Assign the plugin config data
      childToken->Get(newPluginValue.pluginLoadString);
     
      // Extract sub headers and values
      if(!ExtractConfigHeaders(newPluginValue, childToken))
      {
        return false;
      }

      // Add the new plugin
      pluginGridValues.push_back(newPluginValue); 
    }
    else
    {
      wxLogError("ExtractConfigPlugins - Token (%s) is not a plugin?", childToken->GetName().c_str());
      return false;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::ExtractConfigHeaders(ConfigHeaderValue & headerValue, const ConfigToken * addTokenData) const
{
  if(!addTokenData)
  {
    wxLogError("ExtractConfigHeaders - NULL token data");
    return false;
  }

  // Loop for all config children
  for(uint i=0; i<addTokenData->GetNumChildren(); i++)
  {
    const ConfigToken * childToken = addTokenData->GetChildToken(i);

    // If the child has a value, create a new select value 
    if(childToken->GetNumValues() == 1)
    {
      ConfigSelectValue newSelectValue;

      // Assign the name
      newSelectValue.typeName = childToken->GetName();

      // Extract the value type
      string valueTypeString;
      childToken->Get(valueTypeString);
      if(!ExtractConfigValueType(newSelectValue, valueTypeString))
      {
        return false;
      }

      // Extract the value data
      if(!ExtractConfigSelectValue(newSelectValue, childToken))
      {
        return false;
      }

      // Add the new value
      headerValue.childSelectArray.push_back(newSelectValue);
    }
    else if(childToken->GetNumValues() == 0)
    {
      // Else, assume a header and create a new header value
      ConfigHeaderValue newHeaderValue;

      // Assign the name
      newHeaderValue.headerName = childToken->GetName();

      // Recurse
      if(!ExtractConfigHeaders(newHeaderValue, childToken))
      {
        return false;
      }

      // Add the new header
      headerValue.childHeaderArray.push_back(newHeaderValue); 
    }
    else
    {
      wxLogError("ExtractConfigHeaders - Token (%s) has invalid number of values", childToken->GetName().c_str());
      return false;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::ExtractConfigSelectValue(ConfigSelectValue & selectValue, const ConfigToken * addTokenData) const
{
  const ConfigToken * testTokenData = NULL;

  if(!addTokenData)
  {
    wxLogError("ExtractConfigSelectValue - NULL token data");
    return false;
  }

  // Get the default values
  testTokenData = addTokenData->GetChildToken("DefaultValue");
  if(testTokenData)
  {
    testTokenData->GetArray(testTokenData->GetNumValues(), selectValue.defaultValue);
  }

  // Get the comment string
  testTokenData = addTokenData->GetChildToken("Comment");
  if(testTokenData)
  {
    testTokenData->Get(selectValue.commentString);
  }

  // Get the enum values (if they exist)
  testTokenData = addTokenData->GetChildToken("EnumValueTypes");
  if(testTokenData)
  {
    testTokenData->GetArray(testTokenData->GetNumValues(), selectValue.enumValueTypes);
  }

  // Get if there is an ignore value
  testTokenData = addTokenData->GetChildToken("IgnoreIfValueEquals");
  if(testTokenData)
  {
    selectValue.useIgnoreValue = true;
    testTokenData->Get(selectValue.ignoreIfValueEquals);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::ExtractConfigValueType(ConfigSelectValue &selectValue, const string & valueTypeString) const
{
  // Loop for all type mappings 
  for(uint i=0; i<CT_MaxNumTypes; i++)
  {
    // If the config string matches
    if(configTypeStrMap[i].typeString == valueTypeString)
    {
      selectValue.configType = configTypeStrMap[i].type;
      return true;
    }
  }

  wxLogError("ExtractConfigValueType - Unknown value type string %s", valueTypeString.c_str());
  return false;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::SetProfileData(const ConfigToken &setProfile, ConfigHeaderValue & headerToken)
{
  uint i;

  // If this is the plugins header
  if(headerToken.headerName == CONFIG_PLUGINS_HEADER_NAME)
  {
    // Process from the plugins array
    for(i=0; i<pluginGridValues.size(); i++)
    {
      // If there is a child with the plugin name
      const ConfigToken * childToken = setProfile.GetChildToken(pluginGridValues[i].headerName);
      if(childToken)
      { 
        // Set the Plugin as enabled 
        for(uint pluginValue=0; pluginValue<pluginGridValues[i].childSelectArray.size(); pluginValue++)
        {
          // Set the plugin enabled flag to true
          if(pluginGridValues[i].childSelectArray[pluginValue].typeName == CONFIG_PLUGIN_ENABLE_NAME)
          {
            pluginGridValues[i].childSelectArray[pluginValue].isValueDisplayed = true;
            pluginGridValues[i].childSelectArray[pluginValue].defaultValue.clear();
            pluginGridValues[i].childSelectArray[pluginValue].defaultValue.push_back(CONFIG_PLUGIN_TOKEN_TRUE);
            break;
          }
        }

        // Recurse
        if(!SetProfileData(*childToken, pluginGridValues[i]))
        {
          return false;
        }
      }
    }

    return true;
  }

  // Loop for all read tokens
  for(i=0; i<headerToken.childSelectArray.size(); i++)
  {
    // Find the token in the config data
    const ConfigToken * childToken = setProfile.GetChildToken(headerToken.childSelectArray[i].typeName);
    if(childToken)
    { 
      // Get the new defaults
      vector<string> newValues;
      childToken->GetArray(childToken->GetNumValues(), newValues);

      // Check the new values
      if(newValues.size() == 0)
      {
        wxLogError("SetProfileData - Empty value for token %s", headerToken.childSelectArray[i].typeName.c_str());        
        return false; 
      }

      // Check if there are a different number of values than the existing defaults  
      if(newValues.size() != headerToken.childSelectArray[i].defaultValue.size())
      {
        headerToken.childSelectArray[i].isValueDisplayed = true;
      }
      else
      {
        // Loop and check if any of the new values are different from the default string
        for(uint cmpIndex=0; cmpIndex<newValues.size(); cmpIndex++)
        {
          // Find the first string that does not match
          if(newValues[cmpIndex] != headerToken.childSelectArray[i].defaultValue[cmpIndex])
          {
            headerToken.childSelectArray[i].isValueDisplayed = true;
            break;
          }
        }
      }

      // Assign the new values as the defaults
      headerToken.childSelectArray[i].defaultValue = newValues;
    }
  }

  // Loop for all child header tokens
  for(i=0; i<headerToken.childHeaderArray.size(); i++)
  {
    // If there is a child with the header name
    const ConfigToken * childToken = setProfile.GetChildToken(headerToken.childHeaderArray[i].headerName);
    if(childToken)
    { 
      // Recurse
      if(!SetProfileData(*childToken, headerToken.childHeaderArray[i]))
      {
        return false;
      }
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::GetDisplayConfigData(ConfigToken & newConfigData) const
{
  string pathParent = string("");
  return GetDisplayConfigData(newConfigData, gridValues, pathParent);
}


///////////////////////////////////////////////////////////////////////////////
//
bool ConfigOptionsDialog::GetDisplayConfigData(ConfigToken & newConfigData, const ConfigHeaderValue & headerValue, const string & parentNamePath) const
{
  // Loop for all values
  uint i=0;
  for(i=0; i<headerValue.childSelectArray.size(); i++)
  {
    // Create the new value child token
    ConfigToken newChildToken(headerValue.childSelectArray[i].typeName);

    // Create the property name
    string propertyName = parentNamePath + string(".") + newChildToken.GetName();

    // Get the values from the dialog
    vector<string> newChildValues;
    wxPGId propID = propGrid->GetPropertyByName(propertyName.c_str());
    if(GetPropGridSelectValue(headerValue.childSelectArray[i], propID, newChildValues))
    {
      // Set the new token values
      newChildToken.SetValueArray(newChildValues);
    }
    else
    {
      wxLogError("GetDisplayConfigData - Unable to get property %s", propertyName.c_str());        
    }

    // If the value has an "ignore value" option set
    if(headerValue.childSelectArray[i].useIgnoreValue)
    {
      // If the value dose not match the ignore value, add the child token
      if(newChildValues.size() > 0 && 
         newChildValues[0] != headerValue.childSelectArray[i].ignoreIfValueEquals)
      {
        newConfigData.AddChild(newChildToken);
      }
    }
    else
    {
      // Add the new token to then header
      newConfigData.AddChild(newChildToken);
    }
  }

  // Loop for all children of the header
  for(i=0; i<headerValue.childHeaderArray.size(); i++)
  {
    // Add a child token 
    ConfigToken newChildToken(headerValue.childHeaderArray[i].headerName);

    // Calculate the new header path
    string childHeaderPath = parentNamePath + string(".") + newChildToken.GetName();

    // Recurse
    if(!GetDisplayConfigData(newChildToken, headerValue.childHeaderArray[i], childHeaderPath))
    {
      return false;
    }

    // Add to the output
    newConfigData.AddChild(newChildToken);
  }

  // If this is the plugins header, loop over the plugins
  if(headerValue.headerName == CONFIG_PLUGINS_HEADER_NAME)
  {
    for(i=0; i<pluginGridValues.size(); i++)
    {
      // Add a child token 
      ConfigToken newChildToken(pluginGridValues[i].headerName);

      // Calculate the new header path
      string childHeaderPath = parentNamePath + string(".") + newChildToken.GetName();

      // Recurse
      if(!GetDisplayConfigData(newChildToken, pluginGridValues[i], childHeaderPath))
      {
        return false;
      }

      // For plugins, check if the enabled flag is set 
      const ConfigToken * pluginEnabledToken = newChildToken.GetChildToken(CONFIG_PLUGIN_ENABLE_NAME);
      if(pluginEnabledToken && pluginEnabledToken->GetNumValues() == 1)
      {
        // Check if the flag is true
        string enableFlagStr;
        pluginEnabledToken->Get(enableFlagStr);
        if(enableFlagStr == CONFIG_PLUGIN_TOKEN_TRUE)
        {
          // Remove the flag token
          newChildToken.RemoveChild(CONFIG_PLUGIN_ENABLE_NAME);
          pluginEnabledToken = NULL;

          // Set the plugin path
          vector<string> pluginLoadArray;
          pluginLoadArray.push_back(pluginGridValues[i].pluginLoadString);
          newChildToken.SetValueArray(pluginLoadArray);

          // Add to the output
          newConfigData.AddChild(newChildToken);
        }
      }
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigOptionsDialog::OnButtonSaveAs(wxCommandEvent& event)
{
  // Get the profile data
  ConfigToken saveConfigData;
  if(!GetDisplayConfigData(saveConfigData))
  {
    wxLogError("Unable to get config data");
    return;
  }

  // Convert the token data to a string
  string stlConfigString;
  for(uint childNum =0;childNum < saveConfigData.GetNumChildren(); childNum++)
  {
    //Convert each child back to raw config string data
    const ConfigToken * childData = saveConfigData.GetChildToken(childNum);
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
        return;
      }
    }
  }
  wxString saveConfigString = stlConfigString.c_str();

  wxStandardPaths stdPaths;
  wxFileName dirProcess;

  // Get the main profile directory
  dirProcess.AssignDir(stdPaths.GetDataDir());
  dirProcess.AppendDir("Profiles");
  wxString profileDir = dirProcess.GetPath();

  // Open the profile save dialog
  wxFileDialog fileDialog(this, _T("Save profile as"), profileDir, _T(""), _T("GLIntercept Profile (*.xni)|*.xni|Any (*.*)|*.*"), wxSAVE | wxOVERWRITE_PROMPT );
  if (fileDialog.ShowModal() != wxID_OK)
  {
    return;
  }
  wxFile configWrite;
  if(!configWrite.Open(fileDialog.GetPath(), wxFile::write))
  {
    wxLogError("Unable to open config file (%s)", fileDialog.GetPath().c_str());     
    return;
  }
  if(!configWrite.Write(saveConfigString))
  {
    wxLogError("Unable to write to config file (%s)", fileDialog.GetPath().c_str());     
    return;
  }

  // Flag that a config was saved
  isConfigSaved = true;
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigOptionsDialog::OnButtonCancel(wxCommandEvent& event)
{
  // Set that no config values were selected
  isCustomConfigSelected = false;

  // Close the dialog
  Close();
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigOptionsDialog::OnButtonDone(wxCommandEvent &event)
{
  // Close the dialog
  Close();  
}

///////////////////////////////////////////////////////////////////////////////
//
void ConfigOptionsDialog::OnPropertyGridChange(wxPropertyGridEvent& event)
{
  // If a property changes, a custom config is now in use.
  isCustomConfigSelected = true;
}

