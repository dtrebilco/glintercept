///////////////////////////////////////////////////////////////////////////////
// Name:        wxstedit.cpp
// Purpose:     Simple wxSTEditor app
// Author:      John Labenski
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: wxstedit.cpp,v 1.11 2005/11/03 23:44:00 jrl1 Exp $
// Copyright:   (c) John Labenski
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// This nearly the absolute minimum of code to get an editor
/*
    // Create the options and tweak them as necessary
    wxSTEditorOptions steOptions(STE_DEFAULT_OPTIONS);
    steOptions.GetMenuManager()->SetMenuOptionType(STE_MENU_NOTEBOOK, true);
    // Create the frame for the editor
    wxSTEditorFrame *editor = new wxSTEditorFrame(NULL, wxID_ANY, wxT("Editor"));
    // Have the frame create the children and menus from the options
    // or you can do this part by hand
    editor->CreateOptions(steOptions);
    // optionally start with a file you load from disk or memory and show the frame
    editor->GetEditor()->LoadFile(wxT("textfile.txt"));
    editor->Show(true);
*/
// ----------------------------------------------------------------------------


// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/stedit/stedit.h"
#include "wx/stedit/steshell.h"
#include "wx/cmdline.h"
#include "wx/config.h"
#include "wx/fileconf.h"
#include "wx/dir.h"
#include "wx/filename.h"
#include "wx/sysopt.h"


// DT_TODO: Clean this out and create a custom config file name etc.
//          Also apply fixes for config file read-in (select bold font on bracket matching)

// ----------------------------------------------------------------------------

enum Menu_IDs
{
    ID_TEST_STESHELL = 9999
};

// ----------------------------------------------------------------------------
// wxCmdLineParser functions
// ----------------------------------------------------------------------------

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_OPTION, wxT("glintercepthwnd"), wxT("glintercepthwnd"), wxT("GLIntercept window handle"),
        wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_NEEDS_SEPARATOR },


    { wxCMD_LINE_SWITCH, wxT("h"), wxT("help"),   wxT("help on command line switches"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_OPTION_HELP },

    { wxCMD_LINE_SWITCH, wxT("1"), wxT("single"), wxT("single file mode"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

    { wxCMD_LINE_SWITCH, wxT("r"), wxT("recurse"), wxT("open the given filespecs recursively, quote values \"*.txt\""),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

    { wxCMD_LINE_OPTION, wxT("c"), wxT("config"), wxT("use config file"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_NEEDS_SEPARATOR },

    { wxCMD_LINE_PARAM,  wxT(""),  wxT(""),       wxT("input filenames(s)"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_PARAM_MULTIPLE },

    { wxCMD_LINE_NONE }
};

// ----------------------------------------------------------------------------
// wxStEditApp - the application class
// ----------------------------------------------------------------------------
class wxStEditApp : public wxApp
{
public:
    wxStEditApp() : wxApp(), m_frame(NULL) {}
    virtual bool OnInit();
    virtual int OnExit();

    void CreateShell();
    void OnMenuEvent(wxCommandEvent& event);
    void OnSTEShellEvent(wxSTEditorEvent& event);

    wxSTEditorFrame* m_frame;
};

IMPLEMENT_APP(wxStEditApp)

bool wxStEditApp::OnInit()
{
    //Set the default file options
    wxSTEditorOptions::SetGlobalDefaultFileName( wxString( wxT("untitled.glsl") ));
    wxSTEditorOptions::SetGlobalDefaultFileExtensions( wxString(wxT("Any file (*)|*|")
                                                       wxT("Text file (*.txt)|*.txt|")
                                                       wxT("GLSL file (*.glsl)|*.glsl|")
                                                       wxT("GL ARB ASM file (*.arbfp;*.arbvp)|*.arbfp;*.arbvp|")
                                                       wxT("GL NV ASM file (*.nvfp;*.nvvp)|*.nvfp;*.nvvp") ));


    // Create a set of options for your editing "system."
    //  These options control what components will be automatically
    //  created and/or managed for you. For every window created the
    //  virtual function CreateOptions will be called.
    //  You can start with all the options "turned off" by using
    //  the default constructor.

    // For this simple editor we'll basicly use the defaults for everything
    //  (we reset it in cmd line parser to use either a single editor or notebook)
    wxSTEditorOptions steOptions(STE_DEFAULT_OPTIONS);

    // =======================================================================
    // A sample of things that you might do to change the behavior

    // no bookmark items in menus or toolbar
    //steOptions.GetMenuManager().SetMenuItemType(STE_MENU_BOOKMARK, false);
    //steOptions.GetMenuManager().SetToolbarToolType(STE_MENU_BOOKMARK, false);

    // don't create a toolbar
    //steOptions.SetFrameOption(STF_TOOLBAR, false);
    // allow notebook to not have any pages
    //steOptions.SetNotebookOption(STN_ALLOW_NO_PAGES, true);
    // don't ask the user to save a modified document, close silently
    //steOptions.SetEditorOption(STE_QUERY_SAVE_MODIFIED, false);

    // Maybe we're editing only python files, set global initializers
    // wxSTEditorOptions::SetGlobalDefaultFileName(wxT("newfile.py"));
    // wxSTEditorOptions::SetGlobalDefaultFileExtensions(wxT("Python file (*.py)|*.py"));

    // maybe the editors that use these options are only for your ini files
    // steOptions.SetDefaultFileName(wxT("MyProgram.ini"));

    // Maybe you want your own special menu for the splitter?
    //  it'll delete the old one (if there was one) and replace it with yours.
    // steOptions.SetSplitterPopupMenu(myMenu, false);

    // Maybe you want this editor to not use the global preferences,
    //  create a new one, set it up the way you like it and push it onto the
    //  options so that every new editor sharing these options will use it.
    //  Remember, you can later detach a single editors to have them
    //  use some other prefs/styles/langs with STE::RegisterXXX(otherXXX)
    // wxSTEditorPrefs myPrefs(true);
    // myPrefs.SetPrefBool(STE_PREF_VIEW_EOL, true);
    // steOptions.SetEditorPrefs(myPrefs);

    // You can do the same for the styles and langs, though the languages
    //  are pretty generic and it probably won't be necessary.

    // Remember, the global versions are created to be used by a set of editors
    //  they are created because if a user likes their editor a
    //  certain way, you might as well make all of them look that way.
    //  There is nothing special about them and if you want to see what the
    //  defaults are just create a wxSTEditorPrefs/Styles/Langs(true).

    // etc... Ok, we set things up the way we like.

    // end sample code
    // =======================================================================

    //Turn off (very) slow bitmap remapping
    // NOTE: this takes advantage of some custom code modification to toolbar::Realize
    wxSystemOptions::SetOption(wxT("msw.remap"), 0);  

    //wxLogMessage(wxT("Start."));

    //Allow no pages 
    steOptions.SetNotebookOption(STN_ALLOW_NO_PAGES, true);

    //Don't sort the tabs (sorting tabs causes a callback which causes an assert in wxWidgets -custom windows messaging issue) 
    steOptions.SetNotebookOption(STN_ALPHABETICAL_TABS, false);
    
    // ------------------------------------------------------------------------
    // Read the command line and get the filenames/options, if any

    wxArrayString fileNames;
    wxCmdLineParser parser(cmdLineDesc, argc, argv);
    parser.SetSwitchChars(wxT("-")); 

    bool recurse = false;
    long gliWindowHandle = 0;

/*
    // test code for looking at the args passed in
    for (int k = 0; k < argc; k++)
    {
        wxArrayString a = parser.ConvertStringToArgs(argv[k]);
        for (int n=0; n < a.GetCount(); n++)
        {
            wxPrintf(wxT("Arg %d #%d '%s'\n"), k, n, a[n].c_str()); fflush(stdout);
        }
    }
*/

    switch ( parser.Parse() )
    {
        case -1 :
        {
            // help should be given by the wxCmdLineParser, exit program
            return false;
        }
        case 0:
        {
            parser.Found(wxT("glintercepthwnd"), &gliWindowHandle);
            

            // use single page, else use a notebook of editors
            if (parser.Found(wxT("1")))
            {
                steOptions.SetFrameOption(STF_CREATE_NOTEBOOK, false);
                steOptions.SetFrameOption(STF_CREATE_SINGLEPAGE, true);
                steOptions.GetMenuManager()->CreateForSinglePage();
            }
            else
            {
                steOptions.SetFrameOption(STF_CREATE_SIDEBAR, true);
                steOptions.GetMenuManager()->CreateForNotebook();
                steOptions.GetMenuManager()->SetMenuItemType(STE_MENU_BOOKMARK_MENU, STE_MENU_BOOKMARK_DEFAULT, false);
            }

            // use specified config file to load saved prefs, styles, langs
            wxString configFile;
            if (parser.Found(wxT("c"), &configFile))
            {
                wxFileName fN(configFile);

                if ( !fN.FileExists() )
                {
                    //wxLogMessage(wxT("Config file '")+configFile+wxT("' does not exist."));
                    if (configFile.IsEmpty() || !fN.IsOk() || wxIsWild(configFile))
                    {
                        int ret = wxMessageBox(wxT("Config file '")+configFile+wxT("' has an invalid name.\nContinue without using a config file?"),
                                               wxT("Invalid config file name"),
                                               wxICON_QUESTION|wxYES_NO);
                        if (ret == wxNO)
                            return false;

                        configFile = wxEmptyString;
                    }
                    else // file doesn't exist, ask if they want to create a new one
                    {
                        int ret = wxMessageBox(wxT("Config file '")+configFile+wxT("' does not exist.\nWould you like to create a new one?"),
                                               wxT("Invalid config file"),
                                               wxICON_QUESTION|wxYES_NO|wxCANCEL);
                        if (ret == wxCANCEL)
                            return false;
                        else if (ret == wxNO)
                            configFile = wxEmptyString;
                    }
                }

                // use the specified config file, if it's still set
                if ( !configFile.IsEmpty() )
                {
                    wxFileConfig *config = new wxFileConfig(wxT("wxStEdit"), wxT("wxWidgets"),
                                                            configFile, wxEmptyString,
                                                            wxCONFIG_USE_RELATIVE_PATH);
                    wxConfigBase::Set((wxConfigBase*)config);
                }
                else // don't use any config file at all, disable saving
                {
                    steOptions.GetMenuManager()->SetMenuItemType(STE_MENU_PREFS_MENU, STE_MENU_PREFS_SAVE, false);
                }
            }
            else
            {
                // Always use a wxFileConfig since I don't care for registry entries.
                wxFileConfig *config = new wxFileConfig(wxT("wxStEdit"), wxT("wxWidgets"));
                wxConfigBase::Set((wxConfigBase*)config);
            }

            // they want to open the files recursively
            if (parser.Found(wxT("r")))
                recurse = true;

            // gather up all the filenames to load
            size_t n, count = parser.GetParamCount();
            for (n = 0; n < count; n++)
                fileNames.Add(parser.GetParam(n));

            break;
        }
        default:
        {
            wxLogMessage(wxT("Unknown command line option, aborting."));
            return false;
        }
    }

    // create with the readonly menuitem, not set by default since I don't think
    //  it's generally useful, but good for debugging.
    steOptions.GetMenuManager()->SetMenuItemType(STE_MENU_EDIT_MENU, STE_MENU_EDIT_READONLY, true);

    // ------------------------------------------------------------------------
    // load the prefs/style/langs from the config, if we're using one
    if (wxConfigBase::Get(false))
        steOptions.LoadConfig(*wxConfigBase::Get(false));

    // ------------------------------------------------------------------------
    m_frame = new wxSTEditorFrame( NULL, wxID_ANY, wxT("GLI Shader Edit"),
                                   wxDefaultPosition, wxSize(575,825));

    // must call this if you want any of the options, else blank frame
    m_frame->CreateOptions(steOptions, gliWindowHandle);


/*
    // add menu item for testing the shell
    wxMenu* menu = m_frame->GetMenuBar()->GetMenu(m_frame->GetMenuBar()->GetMenuCount()-1);
    menu->AppendSeparator();
    menu->Append(ID_TEST_STESHELL, wxT("Test STE shell"), wxT("Test the STE shell component"));
    m_frame->Connect(ID_TEST_STESHELL, wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(wxStEditApp::OnMenuEvent));
*/
    // ------------------------------------------------------------------------
    // handle loading the files
    size_t n;
    wxArrayString badFileNames;

    // handle recursive file loading
    if (recurse && m_frame->GetEditorNotebook())
    {
        int max_page_count = m_frame->GetEditorNotebook()->GetMaxPageCount();

        wxArrayString recurseFileNames;
        for (n = 0; n < fileNames.GetCount(); n++)
        {
            wxFileName fN(fileNames[n]);
            fN.MakeAbsolute();
            //wxPrintf(wxT("Loading file '%s' to '%s'\n"), fileNames[n].c_str(), fN.GetFullPath().c_str()); fflush(stdout);
            wxDir::GetAllFiles(fN.GetPath(), &recurseFileNames, fN.GetFullName());

            // if they did wxstedit /r c:\*.* stop the insanity...
            if ((int)recurseFileNames.GetCount() >= max_page_count)
            {
                wxString msg = wxString::Format(wxT("Opening %d files, unable to open any more."), max_page_count);
                wxMessageBox(msg, wxT("Maximum number of files"), wxOK|wxICON_ERROR, m_frame);
                recurseFileNames.RemoveAt(max_page_count - 1, recurseFileNames.GetCount() - max_page_count);
                break;
            }
        }

        //for (n=0; n < recurseFileNames.GetCount(); n++)
        //  { wxPrintf(wxT("Loading file '%s'\n"), recurseFileNames[n].c_str()); fflush(stdout); }

        fileNames = recurseFileNames; // these are really the files to open
    }

    // if the files have *, ? or are directories, don't try to load them
    for (n=0; n < fileNames.GetCount(); n++)
    {
        if (wxIsWild(fileNames[n]))
        {
            badFileNames.Add(fileNames[n]);
            fileNames.RemoveAt(n);
            n--;
        }
        else if (wxDirExists(fileNames[n]))
        {
            fileNames.RemoveAt(n);
            n--;
        }
    }

    // If there are any good files left, try to load them
    if (fileNames.GetCount() > 0u)
    {
        if (wxFileExists(fileNames[0]))
            m_frame->GetEditor()->LoadFile( fileNames[0] );
        else
            m_frame->GetEditor()->NewFile( fileNames[0] );

        fileNames.RemoveAt(0);
        if (steOptions.HasFrameOption(STF_CREATE_NOTEBOOK) && fileNames.GetCount())
            m_frame->GetEditorNotebook()->LoadFiles( &fileNames );
    }

    m_frame->Show(true);

    // filenames had *, ? or other junk so we didn't load them
    if (badFileNames.GetCount())
    {
        wxString msg(wxT("There was a problem trying to load file(s):\n"));
        for (n=0; n < badFileNames.GetCount(); n++)
            msg += wxT("'") + badFileNames[n] + wxT("'\n");

        wxMessageBox(msg, wxT("Unable to load file(s)"), wxOK|wxICON_ERROR,
                     m_frame);
    }

    return true;
}

int wxStEditApp::OnExit()
{
    delete wxConfigBase::Set(NULL);
    return wxApp::OnExit();
}

void wxStEditApp::CreateShell()
{
    wxDialog dialog(m_frame, wxID_ANY, wxT("wxSTEditorShell"),
                    wxDefaultPosition, wxDefaultSize,
                    wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    wxSTEditorShell* shell = new wxSTEditorShell(&dialog, wxID_ANY);
    // Set the styles and langs to those of the frame (not necessary, but nice)
    // The prefs aren't shared since we want to control the look and feel.
    wxSTEditorPrefs prefs(true);
    prefs.SetPrefInt(STE_PREF_INDENT_GUIDES, 0);
    prefs.SetPrefInt(STE_PREF_EDGE_MODE, wxSTC_EDGE_NONE);
    prefs.SetPrefInt(STE_PREF_VIEW_LINEMARGIN, 0);
    prefs.SetPrefInt(STE_PREF_VIEW_MARKERMARGIN, 1);
    prefs.SetPrefInt(STE_PREF_VIEW_FOLDMARGIN, 0);
    shell->RegisterPrefs(prefs);
    shell->RegisterStyles(m_frame->GetOptions().GetEditorStyles());
    shell->RegisterLangs(m_frame->GetOptions().GetEditorLangs());
    shell->SetLanguage(STE_LANG_PYTHON); // arbitrarily set to c++

    shell->BeginWriteable();
    shell->AppendText(wxT("Welcome to a test of the wxSTEditorShell.\n\n"));
    shell->AppendText(wxT("This simple test merely responds to the wxEVT_STESHELL_ENTER\n"));
    shell->AppendText(wxT("events and prints the contents of the line when you press enter.\n\n"));
    shell->AppendText(wxT("For demo purposes, the shell understands these simple commands.\n"));
    shell->AppendText(wxT(" SetMaxHistoryLines num : set the number of lines in history buffer\n"));
    shell->AppendText(wxT(" SetMaxLines num        : set the number of lines displayed\n"));
    shell->AppendText(wxT(" Quit                   : quit the wxSTEditorShell demo\n"));
    shell->CheckPrompt(true); // add prompt
    shell->EndWriteable();

    shell->Connect(wxID_ANY, wxEVT_STESHELL_ENTER,
                   wxSTEditorEventHandler(wxStEditApp::OnSTEShellEvent));

    int width = shell->TextWidth(wxSTC_STYLE_DEFAULT, 
                                 wxT(" SetMaxHistoryLines num : set the number of lines in history buffer  "));
    dialog.SetSize(width + 30, -1);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    topSizer->Add(shell, 1, wxEXPAND);
    dialog.SetSizer(topSizer);
    dialog.ShowModal();
}

void wxStEditApp::OnMenuEvent(wxCommandEvent& event)
{
    // note: use wxGetApp().CreateShell() because we're called from a handler 
    //       Connected from the frame to the app and you'll crash otherwise.

    switch (event.GetId())
    {
        case ID_TEST_STESHELL : wxGetApp().CreateShell(); break;
        default : break;
    }
    event.Skip();
}

void wxStEditApp::OnSTEShellEvent(wxSTEditorEvent& event)
{
    // handle the event and for this example we just write it back
    wxSTEditorShell* shell = wxDynamicCast(event.GetEventObject(), wxSTEditorShell);
    wxString val = event.GetString();
    shell->AppendText(wxT("\nText Entered : '") + val + wxT("'\n"));

    // very simple mechanism to parse the line to do things, you may prefer
    //   using wxPython or wxLua and running the string as is.
    wxString token(val.BeforeFirst(wxT(' ')).Lower());

    if (val.Lower().Strip(wxString::both) == wxT("quit"))
    {
        wxCommandEvent quitEvent(wxEVT_COMMAND_BUTTON_CLICKED, wxID_OK);
        event.SetEventObject(shell);
        shell->GetEventHandler()->ProcessEvent(quitEvent);
    }
    else if (token == wxT("setmaxhistorylines"))
    {
        wxString num = val.AfterFirst(wxT(' '));
        long n = 0;
        if (num.ToLong(&n))
        {
            shell->SetMaxHistoryLines(n);
            shell->AppendText(wxString::Format(wxT("The maximum number of history lines is set to %d.\n"), n));
        }
        else
            shell->AppendText(wxT("ERR: Expected number, eg. SetMaxHistoryLines 10\n"));
    }
    else if (token == wxT("setmaxlines"))
    {
        wxString num = val.AfterFirst(wxT(' '));
        long n = 0;
        if (num.ToLong(&n))
        {
            shell->SetMaxLines(n);
            shell->AppendText(wxString::Format(wxT("The maximum number of displayed lines is set to %d.\n"), n));
        }
        else
            shell->AppendText(wxT("ERR: Expected number, eg. SetMaxLines 10\n"));
    }

    shell->CheckPrompt(true);
}
