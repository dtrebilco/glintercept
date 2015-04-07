///////////////////////////////////////////////////////////////////////////////
// Name:        steframe.h
// Purpose:     wxSTEditorFrame
// Author:      John Labenski, parts taken from wxGuide by Otto Wyss
// Modified by:
// Created:     11/05/2002
// Copyright:   (c) John Labenski, Otto Wyss
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _STEFRAME_H_
#define _STEFRAME_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "steframe.h"
#endif

class WXDLLEXPORT wxFrame;
class WXDLLEXPORT wxSplitterWindow;
class WXDLLEXPORT wxNotebook;
class WXDLLEXPORT wxMenu;
class WXDLLEXPORT wxKeyEvent;
class WXDLLEXPORT wxToolBar;
class WXDLLEXPORT wxConfigBase;
class WXDLLEXPORT wxFileHistory;

#include "wx/frame.h"
#include "wx/dnd.h"

#include "wx/stedit/stedefs.h"


#ifdef GLI_CHANGES

class GLIShaders;
class GLIShaderDebug;

#endif //GLI_CHANGES

//-----------------------------------------------------------------------------
// wxSTEditorFrame - a generic frame to contain either a single editor
//   or editors in a wxSTEditorNotebook. You can optionally have a splitter
//   that will contain a wxNotebook in the left panel with a wxListBox
//   containing a list of the pages in the notebook.
//   See the CreateOptions function.
//
// In order to make this as generic as possible all requests for windows call
//   the virtual functions GetEditor, GetEditorSplitter, GetEditorNotebook,
//   GetSideSplitter etc. You can also just create the windows any way you like
//   and set the member pointers appropriately, even to NULL.
//   This way you can still allow HandleMenuEvent to
//   do some work for you, but you can make a more complicated window layout
//   so long as you return the appropriate windows. The functions to get the
//   windows can return NULL and any action from the HandleMenuEvent function
//   will be silently ignored.
//-----------------------------------------------------------------------------

//  |--------------------------------------------------------|
//  | wxSTEditorFrame, menu & toolbar                        |
//  |--------------------------------------------------------|
//  |                |                                       |
//  |                |         wxSTEditorNotebook            |
//  |                |                or                     |
//  |  SideNotebook  |         wxSTEditorSplitter            |
//  |                |                                       |
//  |                |---------------------------------------| < MainSplitter
//  |                |    Some user added window (unused)    |
//  |--------------------------------------------------------|
//                   ^
//              SideSplitter

class WXDLLIMPEXP_STEDIT wxSTEditorFrame : public wxFrame
{
public:
    wxSTEditorFrame() : wxFrame() { Init(); }
    wxSTEditorFrame(wxWindow *parent, wxWindowID id,
                    const wxString& title = wxT("wxStEdit"),
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxSize(400, 400),
                    long style = wxDEFAULT_FRAME_STYLE,
                    const wxString& name = wxT("wxSTEditorFrame")) : wxFrame()
    {
        Init();
        Create(parent, id, title, pos, size, style, name);
    }

    virtual ~wxSTEditorFrame();
    virtual bool Destroy();

    bool Create(wxWindow *parent, wxWindowID id,
                const wxString& title = wxT("wxStEdit"),
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxT("wxSTEditorFrame"));

    // Create and set the wxSTEditorOptions, call this after creation
    //  or just create the child windows yourself.

#ifdef GLI_CHANGES
    
    virtual void CreateOptions(const wxSTEditorOptions& options, long gliWindowHandle);

#else //GLI_CHANGES

    virtual void CreateOptions(const wxSTEditorOptions& options);

#endif //GLI_CHANGES


    // GetEditorOptions, use this to change editor option values
    const wxSTEditorOptions& GetOptions() const { return m_options; }
    wxSTEditorOptions& GetOptions() { return m_options; }
    // Set the options, the options will now be refed copies of the ones you send
    // in. This can be used to detach the options for a particular editor from
    // the rest of them.
    void SetOptions(const wxSTEditorOptions& options) { m_options = options; }

    // enable/disable sending wxSTEditor events from children editors
    void SetSendSTEEvents(bool send);

    // returns either the single editor for STF_SINGLEPAGE or
    // the editor at page, if page = -1 get the current editor
    // override these in your frame for more complex scenarios where you may have
    // different or more layers of windows.
    virtual wxSTEditor *GetEditor(int page = -1) const;
    virtual wxSTEditorSplitter *GetEditorSplitter(int page = -1) const;
    // Get the notebook containing the editor, NULL if style STF_SINGLEPAGE
    virtual wxSTEditorNotebook *GetEditorNotebook() const { return m_steNotebook; }

    // Get the splitter between editor (notebook) and some user set window
    virtual wxSplitterWindow* GetMainSplitter() const { return m_mainSplitter; }
    // Get the splitter between sidebar notebook and editors, NULL if not style STF_SIDEBAR
    virtual wxSplitterWindow* GetSideSplitter() const { return m_sideSplitter; }
    // Get the sidebar notebook, NULL if not style STF_SIDEBAR
    virtual wxNotebook *GetSideNotebook() const { return m_sideNotebook; }
    // Get the file listbox in the sidebar notebook, NULL if not style STF_SIDEBAR
    virtual wxListBox *GetFileListBox() const { return m_fileListBox; }

    // Update all the menu/tool items in the wxSTEditorOptions
    virtual void UpdateAllItems();
    // Update popupmenu, menubar, toolbar if any
    virtual void UpdateItems(wxMenu *menu=NULL, wxMenuBar *menuBar=NULL, wxToolBar *toolBar=NULL);

    // Get/Set the base of the title (default is title set in constructor)
    // Title is "titleBase - [*] editor fileName", * used to denote modified
    wxString GetTitleBase() const { return m_titleBase; }
    void SetTitleBase( const wxString& titleBase ) { m_titleBase = titleBase; }

    // Show the about dialog, called for wxID_ABOUT
    virtual void ShowAboutDialog();

    // Load/Save the config for showing sidebar and frame size
    //   See also wxSTEditorOptions for paths and internal saving config.
    void LoadConfig( wxConfigBase &config,
                     const wxString &configPath = wxT("/wxSTEditor/Frame") );
    void SaveConfig( wxConfigBase &config,
                     const wxString &configPath = wxT("/wxSTEditor/Frame") );

    // -----------------------------------------------------------------------
    // implementation
    void OnNotebookPageChanged(wxNotebookEvent &event);
    void OnFileListBox(wxCommandEvent &event);
    virtual void UpdateFileListBox();

    void OnSTECreated(wxCommandEvent &event);
    void OnSTEState(wxSTEditorEvent &event);
    void OnSTEPopupMenu(wxSTEditorEvent &event);
    void OnSTCUpdateUI(wxStyledTextEvent &event);

    void OnMenuOpen(wxMenuEvent &event);
    void OnMenu(wxCommandEvent &event);
    virtual bool HandleMenuEvent(wxCommandEvent &event);

    void OnClose(wxCloseEvent &event);


#ifdef GLI_CHANGES

    // Set the active page on the side notebook to the passed window 
    //  (if it is contained in the notebook)
    void ShowSideNotebookWindow(wxWindow * showWindow);

    void OutputClear();
    void OutputAppendString(const wxString &logString);

    inline GLIShaders * GetGLIShadersDialog();
    inline GLIShaderDebug * GetGLIShaderDebugDialog();

    //Called when the shader debug dialog is destroyed
    inline void OnShaderDebugDestroy();

    bool GLSLValidate(const wxString &src); 
    bool ExtractShaderString(const wxString &shaderToken, const wxString &shaderSource, wxString &retSource) const;

    // Handle the main window moving
    void OnMove(wxMoveEvent &event);

#endif //GLI_CHANGES


protected:
    wxSTEditorOptions m_options;
    wxString m_titleBase;

    wxSTEditorNotebook *m_steNotebook;   // notebook for editors (not single editor), may be NULL
    wxSTEditorSplitter *m_steSplitter;   // single editor (not notebook), may be NULL
    wxSplitterWindow   *m_mainSplitter;  // splitter for notebook/editor and bottom notebook
    wxSplitterWindow   *m_sideSplitter;  // splitter for editor and left hand panels
    wxNotebook         *m_sideNotebook;  // notebook to hold... file listbox
    wxListBox          *m_fileListBox;   // display the notebook pages, may be NULL

    wxWindow           *m_sideSplitterWin1; // these are the two pages of the side splitter
    wxWindow           *m_sideSplitterWin2;


#ifdef GLI_CHANGES

    wxStyledTextCtrl   *m_outputTextCtrl; // The output log text control
    GLIShaders         *gliShadersDialog; // The shaders dialog window
    GLIShaderDebug     *gliDebugDialog;   // The shader debug dialog

    bool                glslValidate;     // Flag indicating if GLSL validation takes place

#endif //GLI_CHANGES


    wxSTERecursionGuardFlag m_rGuard_OnMenu;
    wxSTERecursionGuardFlag m_rGuard_HandleMenuEvent;

private:
    void Init();
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxSTEditorFrame)
};

#ifdef GLI_CHANGES

///////////////////////////////////////////////////////////////////////////////
//
inline GLIShaders * wxSTEditorFrame::GetGLIShadersDialog()
{
  return gliShadersDialog;
}

///////////////////////////////////////////////////////////////////////////////
//
inline GLIShaderDebug * wxSTEditorFrame::GetGLIShaderDebugDialog()
{
  return gliDebugDialog;
}

///////////////////////////////////////////////////////////////////////////////
//
inline void wxSTEditorFrame::OnShaderDebugDestroy()
{
  gliDebugDialog = NULL;
}

#endif //GLI_CHANGES


//-----------------------------------------------------------------------------
// wxSTEditorFrameFileDropTarget
//-----------------------------------------------------------------------------
#if wxUSE_DRAG_AND_DROP
class WXDLLIMPEXP_STEDIT wxSTEditorFrameFileDropTarget : public wxFileDropTarget
{
public:
    wxSTEditorFrameFileDropTarget(wxSTEditorFrame *owner) : m_owner(owner) {}
    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

    wxSTEditorFrame *m_owner;
};
#endif //wxUSE_DRAG_AND_DROP

#endif  // _STEFRAME_H_
