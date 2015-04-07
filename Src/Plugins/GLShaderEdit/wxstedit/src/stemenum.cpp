///////////////////////////////////////////////////////////////////////////////
// Name:        stemenum.cpp
// Purpose:     wxSTEditorMenuManager
// Author:      John Labenski, parts taken from wxGuide by Otto Wyss
// Modified by:
// Created:     11/05/2002
// RCS-ID:
// Copyright:   (c) John Labenski, Otto Wyss
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "stemenum.h"
#endif

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif // WX_PRECOMP

#include "wx/stedit/stedit.h"
#include "wx/stedit/stemenum.h"
#include "wx/artprov.h"   // wxArtProvider

#define STE_MM wxSTEditorMenuManager

//-----------------------------------------------------------------------------
// wxSTEditorMenuManager - a holding place for menu generating code
//-----------------------------------------------------------------------------
void wxSTEditorMenuManager::Init()
{
    m_enabledEditorItems = true;
    m_menuOptionTypes = 0;
    m_menuItemTypes.Add(0, STE_MENU_HELP_MENU+1);
    m_toolBarToolTypes = 0;
}

void wxSTEditorMenuManager::CreateForSinglePage()
{
    m_menuOptionTypes  = 0;
    m_menuItemTypes[STE_MENU_FILE_MENU]     = STE_MENU_FILE_DEFAULT;
    m_menuItemTypes[STE_MENU_EDIT_MENU]     = STE_MENU_EDIT_DEFAULT;
    m_menuItemTypes[STE_MENU_TOOLS_MENU]    = STE_MENU_TOOLS_DEFAULT;
    m_menuItemTypes[STE_MENU_VIEW_MENU]     = STE_MENU_VIEW_DEFAULT;
    m_menuItemTypes[STE_MENU_BOOKMARK_MENU] = STE_MENU_BOOKMARK_DEFAULT;
    m_menuItemTypes[STE_MENU_PREFS_MENU]    = STE_MENU_PREFS_DEFAULT;
    m_menuItemTypes[STE_MENU_WINDOW_MENU]   = STE_MENU_WINDOW_DEFAULT;
    m_menuItemTypes[STE_MENU_HELP_MENU]     = STE_MENU_HELP_DEFAULT;
    m_toolBarToolTypes = STE_TOOLBAR_TOOLS;
}
void wxSTEditorMenuManager::CreateForNotebook()
{
    m_menuOptionTypes  = STE_MENU_NOTEBOOK;
    m_menuItemTypes[STE_MENU_FILE_MENU]     = STE_MENU_FILE_DEFAULT;
    m_menuItemTypes[STE_MENU_EDIT_MENU]     = STE_MENU_EDIT_DEFAULT;
    m_menuItemTypes[STE_MENU_TOOLS_MENU]    = STE_MENU_TOOLS_DEFAULT;
    m_menuItemTypes[STE_MENU_VIEW_MENU]     = STE_MENU_VIEW_DEFAULT;
    m_menuItemTypes[STE_MENU_BOOKMARK_MENU] = STE_MENU_BOOKMARK_DEFAULT;
    m_menuItemTypes[STE_MENU_PREFS_MENU]    = STE_MENU_PREFS_DEFAULT;
    m_menuItemTypes[STE_MENU_WINDOW_MENU]   = STE_MENU_WINDOW_DEFAULT|STE_MENU_WINDOW_FILECHOOSER|STE_MENU_WINDOW_PREVNEXT|STE_MENU_WINDOW_WINDOWS;
    m_menuItemTypes[STE_MENU_HELP_MENU]     = STE_MENU_HELP_DEFAULT;
    m_toolBarToolTypes = STE_TOOLBAR_TOOLS;
}

wxMenu *wxSTEditorMenuManager::CreateEditorPopupMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;

    bool add_sep = false;

    wxMenu *fileMenu     = GetMenuItemTypes(STE_MENU_FILE_MENU)     != 0 ? CreateFileMenu()     : NULL;
    wxMenu *editMenu     = GetMenuItemTypes(STE_MENU_EDIT_MENU)     != 0 ? CreateEditMenu()     : NULL;
    wxMenu *toolsMenu    = GetMenuItemTypes(STE_MENU_TOOLS_MENU)    != 0 ? CreateToolsMenu()    : NULL;
    wxMenu *viewMenu     = GetMenuItemTypes(STE_MENU_VIEW_MENU)     != 0 ? CreateViewMenu()     : NULL;
    wxMenu *bookmarkMenu = GetMenuItemTypes(STE_MENU_BOOKMARK_MENU) != 0 ? CreateBookmarkMenu() : NULL;
    wxMenu *prefMenu     = GetMenuItemTypes(STE_MENU_PREFS_MENU)    != 0 ? CreatePreferenceMenu() : NULL;
    wxMenu *windowMenu   = GetMenuItemTypes(STE_MENU_WINDOW_MENU)   != 0 ? CreateWindowMenu()   : NULL;
    wxMenu *helpMenu     = GetMenuItemTypes(STE_MENU_HELP_MENU)     != 0 ? CreateHelpMenu()     : NULL;

    if (fileMenu)
    {
        menu->Append(ID_STE_MENU_FILE, _T("&File"), fileMenu);
        add_sep = true;
    }

    if (editMenu)
    {
        if (add_sep) menu->AppendSeparator();
        menu->Append(ID_STE_MENU_EDIT, _T("&Edit"), editMenu);
        add_sep = true;
    }

    if (toolsMenu)
    {
        if (add_sep) menu->AppendSeparator();
        menu->Append(ID_STE_MENU_TOOLS, _T("&Tools"), toolsMenu);
        add_sep = true;
    }

    if (viewMenu)
    {
        if (add_sep) menu->AppendSeparator();
        menu->Append(ID_STE_MENU_VIEW, _T("&View"), viewMenu);
        add_sep = true;
    }

    if (bookmarkMenu)
    {
        if (add_sep) menu->AppendSeparator();
        menu->Append(ID_STE_MENU_BOOKMARK, _T("&Bookmarks"), bookmarkMenu);
        add_sep = true;
    }

    if (prefMenu)
    {
        if (add_sep) menu->AppendSeparator();
        menu->Append(ID_STE_MENU_PREF, _T("&Preferences"), prefMenu);
        add_sep = true;
    }

    if (windowMenu)
    {
        if (add_sep) menu->AppendSeparator();
        menu->Append(ID_STE_MENU_PREF, _T("&Window"), windowMenu);
    }

    if (helpMenu)
    {
        if (add_sep) menu->AppendSeparator();
        menu->Append(ID_STE_MENU_HELP, _T("&Help"), helpMenu);
    }

    if (!menu_ && menu && (menu->GetMenuItemCount() == 0))
    {
        delete menu;
        return NULL;
    }

    return menu;
}

wxMenu* wxSTEditorMenuManager::CreateSplitterPopupMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;

    menu->AppendRadioItem(ID_STS_UNSPLIT,     wxT("&Unsplit editor"), wxT("Unsplit the editor"));
    menu->AppendRadioItem(ID_STS_SPLIT_HORIZ, wxT("Split editor &horizontally"), wxT("Split editor horizontally"));
    menu->AppendRadioItem(ID_STS_SPLIT_VERT,  wxT("Split editor &vertically"), wxT("Split editor vertically"));

    return menu;
}
wxMenu* wxSTEditorMenuManager::CreateNotebookPopupMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;

    menu->Append(wxID_NEW,  wxT("&Add empty page"));
    menu->Append(wxID_OPEN, _T("&Open file(s)..."));
    menu->Append(ID_STN_SAVE_ALL,  _T("&Save all files"));

    menu->AppendSeparator();
    // These are empty and are filled by wxSTEditorNotebook::UpdateMenuItems
    wxMenu *gotoMenu  = new wxMenu;
    wxMenu *closeMenu = new wxMenu;

    menu->Append(ID_STN_WIN_PREVIOUS, wxT("Previous page"));
    menu->Append(ID_STN_WIN_NEXT,     wxT("Next page"));

    menu->Append(ID_STN_MENU_GOTO,  wxT("Goto page"), gotoMenu);
    menu->AppendSeparator();
    menu->Append(ID_STN_CLOSE_PAGE, wxT("Close current page"));
    menu->Append(ID_STN_CLOSE_ALL,  wxT("Close all pages..."));
    menu->Append(ID_STN_MENU_CLOSE, wxT("Close page"), closeMenu);
    menu->AppendSeparator();
    menu->Append(ID_STN_WINDOWS,    wxT("&Windows..."), wxT("Manage opened windows"));

    return menu;
}
bool wxSTEditorMenuManager::CreateMenuBar(wxMenuBar *menuBar, bool for_frame) const
{
    wxCHECK_MSG(menuBar, false, wxT("Invalid wxMenuBar"));
    size_t menu_count = menuBar->GetMenuCount();

    // Note! here's where we specify that we want menu items for the frame
    int was_set_frame = HasMenuOptionType(STE_MENU_FRAME);
    if (!was_set_frame && for_frame)
        ((wxSTEditorMenuManager*)this)->SetMenuOptionType(STE_MENU_FRAME, true);

    wxMenu *fileMenu     = GetMenuItemTypes(STE_MENU_FILE_MENU)     != 0 ? CreateFileMenu()     : NULL;
    wxMenu *editMenu     = GetMenuItemTypes(STE_MENU_EDIT_MENU)     != 0 ? CreateEditMenu()     : NULL;
    wxMenu *toolsMenu    = GetMenuItemTypes(STE_MENU_TOOLS_MENU)    != 0 ? CreateToolsMenu()    : NULL;
    wxMenu *viewMenu     = GetMenuItemTypes(STE_MENU_VIEW_MENU)     != 0 ? CreateViewMenu()     : NULL;
    wxMenu *bookmarkMenu = GetMenuItemTypes(STE_MENU_BOOKMARK_MENU) != 0 ? CreateBookmarkMenu() : NULL;
    wxMenu *prefMenu     = GetMenuItemTypes(STE_MENU_PREFS_MENU)    != 0 ? CreatePreferenceMenu() : NULL;
    wxMenu *windowMenu   = GetMenuItemTypes(STE_MENU_WINDOW_MENU)   != 0 ? CreateWindowMenu()   : NULL;
    wxMenu *helpMenu     = GetMenuItemTypes(STE_MENU_HELP_MENU)     != 0 ? CreateHelpMenu()     : NULL;

#ifdef GLI_CHANGES
    wxMenu *openglMenu   = CreateOpenGLMenu();
#endif //GLI_CHANGES

    if (fileMenu)     menuBar->Append(fileMenu,     _T("&File"));
    if (editMenu)     menuBar->Append(editMenu,     _T("&Edit"));
    if (toolsMenu)    menuBar->Append(toolsMenu,    _T("&Tools"));
    if (viewMenu)     menuBar->Append(viewMenu,     _T("&View"));
    if (bookmarkMenu) menuBar->Append(bookmarkMenu, _T("&Bookmarks"));

#ifdef GLI_CHANGES
    if (openglMenu)   menuBar->Append(openglMenu,   _T("&OpenGL"));
#endif //GLI_CHANGES
    
    if (prefMenu)     menuBar->Append(prefMenu,     _T("&Preferences"));
    if (windowMenu)   menuBar->Append(windowMenu,   _T("&Window"));
    if (helpMenu)     menuBar->Append(helpMenu,     _T("&Help"));

    // reset the frame bit if it wasn't set
    if (!was_set_frame)
        ((wxSTEditorMenuManager*)this)->SetMenuOptionType(STE_MENU_FRAME, false);

    return menuBar->GetMenuCount() > menu_count;
}

bool wxSTEditorMenuManager::CreateToolBar(wxToolBar *tb) const
{
    wxCHECK_MSG(tb, false, wxT("Invalid toolbar"));
    size_t tools_count = tb->GetToolsCount();

    if (HasToolbarToolType(STE_TOOLBAR_FILE_NEW))
    {
        tb->AddTool(wxID_NEW,    wxT("New"), STE_ARTBMP(wxART_STEDIT_NEW), wxNullBitmap, wxITEM_NORMAL, wxT("New"), wxT("Clear editor for new file"));
    }
    if (HasToolbarToolType(STE_TOOLBAR_FILE_OPEN))
    {
        tb->AddTool(wxID_OPEN,   wxT("Open..."), STE_ARTBMP(wxART_STEDIT_OPEN), wxNullBitmap, wxITEM_NORMAL, wxT("Open..."), wxT("Open a file to edit"));
    }
    if (HasToolbarToolType(STE_TOOLBAR_FILE_SAVE))
    {
        tb->AddTool(wxID_SAVE,   wxT("Save"),       STE_ARTBMP(wxART_STEDIT_SAVE), wxNullBitmap, wxITEM_NORMAL, wxT("Save"), wxT("Save current file"));

#ifdef GLI_CHANGES

        tb->AddTool(ID_STN_CLOSE_PAGE, wxT("Close current"), STE_ARTBMP(wxART_STEDIT_CLOSE), wxNullBitmap, wxITEM_NORMAL, wxT("Close current"), wxT("Close current page"));

#else //GLI_CHANGES

        tb->AddTool(wxID_SAVEAS, wxT("Save as..."), STE_ARTBMP(wxART_STEDIT_SAVEAS), wxNullBitmap, wxITEM_NORMAL, wxT("Save as..."), wxT("Save to a specific filename..."));
        tb->EnableTool(wxID_SAVE, false);

        if (HasMenuOptionType(STE_MENU_NOTEBOOK))
        {
            tb->AddTool(ID_STN_SAVE_ALL, wxT("Save all files"), STE_ARTBMP(wxART_STEDIT_SAVEALL), wxNullBitmap, wxITEM_NORMAL, wxT("Save all files"), wxT("Save all open files"));
            tb->EnableTool(ID_STN_SAVE_ALL, false);
        }
#endif //GLI_CHANGES

    }
    if (HasToolbarToolType(STE_TOOLBAR_EDIT_CUTCOPYPASTE))
    {
        if (tb->GetToolsCount()) tb->AddSeparator();
        tb->AddTool(wxID_CUT,    wxT("Cut"),   STE_ARTBMP(wxART_STEDIT_CUT), wxNullBitmap, wxITEM_NORMAL, wxT("Cut"), wxT("Cut selected text"));
        tb->AddTool(wxID_COPY,   wxT("Copy"),  STE_ARTBMP(wxART_STEDIT_COPY), wxNullBitmap, wxITEM_NORMAL, wxT("Copy"), wxT("Copy selected text"));
        tb->AddTool(wxID_PASTE,  wxT("Paste"), STE_ARTBMP(wxART_STEDIT_PASTE), wxNullBitmap, wxITEM_NORMAL, wxT("Paste"), wxT("Paste text at cursor"));
    }

#ifdef GLI_CHANGES

    //Add the compile/debug options here
    if (tb->GetToolsCount())
    {
      tb->AddSeparator();
    }
    tb->AddTool(ID_STN_COMPILE_SHADER, wxT("Compile shader"), STE_ARTBMP(wxART_STEDIT_COMPILE), wxNullBitmap, wxITEM_NORMAL, wxT("Compile"), wxT("Compile shader"));

#endif //GLI_CHANGES


    if (HasToolbarToolType(STE_TOOLBAR_EDIT_UNDOREDO))
    {
        if (tb->GetToolsCount()) tb->AddSeparator();
        tb->AddTool(wxID_UNDO,   wxT("Undo"), STE_ARTBMP(wxART_STEDIT_UNDO), wxNullBitmap, wxITEM_NORMAL, wxT("Undo"), wxT("Undo last editing"));
        tb->AddTool(wxID_REDO,   wxT("Redo"), STE_ARTBMP(wxART_STEDIT_REDO), wxNullBitmap, wxITEM_NORMAL, wxT("Redo"), wxT("Redo last undo"));
    }
    if (HasToolbarToolType(STE_TOOLBAR_EDIT_FINDREPLACE))
    {
        if (tb->GetToolsCount()) tb->AddSeparator();

#ifndef GLI_CHANGES
        tb->AddTool(ID_STE_FIND_DOWN, wxT("Search direction"), STE_ARTBMP(wxART_STEDIT_FINDDOWN), wxNullBitmap, wxITEM_CHECK, wxT("Search direction"), wxT("Search direction for next occurance in document"));
#endif //GLI_CHANGES

        tb->AddTool(wxID_FIND,        wxT("Find..."),    STE_ARTBMP(wxART_STEDIT_FIND), wxNullBitmap, wxITEM_NORMAL, wxT("Find..."), wxT("Find text in document..."));

#ifndef GLI_CHANGES        
        tb->AddTool(ID_STE_FIND_NEXT, wxT("Find next"),  STE_ARTBMP(wxART_STEDIT_FINDNEXT), wxNullBitmap, wxITEM_NORMAL, wxT("Find next"), wxT("Find next occurance in document"));
#endif //GLI_CHANGES

        tb->AddTool(ID_STE_REPLACE,   wxT("Replace..."), STE_ARTBMP(wxART_STEDIT_REPLACE), wxNullBitmap, wxITEM_NORMAL, wxT("Replace..."), wxT("Replace text in document"));
    }
    if (HasToolbarToolType(STE_TOOLBAR_EDIT_FINDCOMBO))
    {
        if (tb->GetToolsCount()) tb->AddSeparator();
        wxComboBox *combo = new wxComboBox(tb, ID_STE_TOOLBAR_FIND_COMBO);
        tb->AddControl(combo);
    }

#ifndef GLI_CHANGES        
    if (HasToolbarToolType(STE_TOOLBAR_BOOKMARK))
    {
        if (tb->GetToolsCount()) tb->AddSeparator();
        tb->AddTool(ID_STE_BOOKMARK_TOGGLE,   wxT("Toggle bookmark"),     STE_ARTBMP(wxART_ADD_BOOKMARK), wxNullBitmap, wxITEM_NORMAL, wxT("Toggle bookmark"), wxT("Toggle a bookmark on cursor line"));
        tb->AddTool(ID_STE_BOOKMARK_FIRST,    wxT("First bookmark"),      STE_ARTBMP(wxART_GO_UP),        wxNullBitmap, wxITEM_NORMAL, wxT("First bookmark"),  wxT("Goto first bookmark"));
        tb->AddTool(ID_STE_BOOKMARK_PREVIOUS, wxT("Previous bookmark"),   STE_ARTBMP(wxART_GO_BACK),      wxNullBitmap, wxITEM_NORMAL, wxT("Previous bookmark"), wxT("Goto previous bookmark"));
        tb->AddTool(ID_STE_BOOKMARK_NEXT,     wxT("Next bookmark"),       STE_ARTBMP(wxART_GO_FORWARD),   wxNullBitmap, wxITEM_NORMAL, wxT("Next bookmark"),   wxT("Goto next bookmark"));
        tb->AddTool(ID_STE_BOOKMARK_LAST,     wxT("Last bookmark"),       STE_ARTBMP(wxART_GO_DOWN),      wxNullBitmap, wxITEM_NORMAL, wxT("Last bookmark"),   wxT("Goto last bookmark"));
        tb->AddTool(ID_STE_BOOKMARK_CLEAR,    wxT("Clear all bookmarks"), STE_ARTBMP(wxART_DEL_BOOKMARK), wxNullBitmap, wxITEM_NORMAL, wxT("Clear bookmarks"), wxT("Clear all bookmarks"));
    }
#endif //GLI_CHANGES

    tb->Realize();

    return tb->GetToolsCount() > tools_count;
}

wxMenu *wxSTEditorMenuManager::CreateFileMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;
    bool add_sep = false;

    if (HasMenuItemType(STE_MENU_FILE_MENU, STE_MENU_FILE_NEW))
    {
        menu->Append(MenuItem(menu, wxID_NEW, _T("&New...\tCtrl-N"), _T("Clear contents and start a new file"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_NEW)));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_FILE_MENU, STE_MENU_FILE_OPEN))
    {
        menu->Append(MenuItem(menu, wxID_OPEN, _T("&Open...\tCtrl-O"), _T("Open file..."), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_OPEN)));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_FILE_MENU, STE_MENU_FILE_CLOSE) && HasMenuOptionType(STE_MENU_FRAME))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STN_CLOSE_PAGE, wxT("&Close current page\tCtrl-Shift-Q"), wxT("Close current page"));
        if (HasMenuOptionType(STE_MENU_NOTEBOOK))
            menu->Append(ID_STN_CLOSE_ALL, wxT("Cl&ose all pages..."), wxT("Close all pages"));

        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_FILE_MENU, STE_MENU_FILE_SAVE))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(MenuItem(menu, wxID_SAVE,   _T("&Save \tCtrl-S"), _T("Save current file"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_SAVE)));
        menu->Enable(wxID_SAVE, false);
        menu->Append(MenuItem(menu, wxID_SAVEAS, _T("Save &As...\tAlt-S"), _T("Save as file..."), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_SAVEAS)));
        if (HasMenuOptionType(STE_MENU_NOTEBOOK))
        {
            menu->Append(MenuItem(menu, ID_STN_SAVE_ALL, wxT("Save A&ll\tCtrl-Shift-S"), wxT("Save all files"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_SAVEALL)));
            menu->Enable(ID_STN_SAVE_ALL, false);
        }
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_FILE_MENU, STE_MENU_FILE_EXPORT))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(MenuItem(menu, ID_STE_EXPORT,   _T("Expor&t..."), _T("Export to file..."), wxITEM_NORMAL));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_FILE_MENU, STE_MENU_FILE_PROPERTY))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_PROPERTIES, _T("Document propert&ies..."), _T("Show document properties dialog"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_FILE_MENU, STE_MENU_FILE_PRINT))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(MenuItem(menu, wxID_PRINT,              _T("&Print...\tCtrl-P"), _T("Print current document"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_PRINT)));
        menu->Append(MenuItem(menu, wxID_PREVIEW,            _T("Print pre&view...\tCtrl-Shift-P"), _T("Print preview of the current document"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_PRINTPREVIEW)));
        menu->Append(MenuItem(menu, wxID_PRINT_SETUP,        _T("Printer set&up..."), _T("Setup the printer"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_PRINTSETUP)));
        menu->Append(MenuItem(menu, ID_STE_PRINT_PAGE_SETUP, _T("Printer pa&ge setup..."), _T("Setup the printout page"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_PRINTPAGESETUP)));
        menu->Append(MenuItem(menu, ID_STE_PRINT_OPTIONS,    _T("Printer options..."), _T("Set other printout options"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_PRINTPREVIEW)));
    }

    if (HasMenuOptionType(STE_MENU_FRAME))
    {
        if (add_sep) menu->AppendSeparator();
        menu->Append(MenuItem(menu, wxID_EXIT, wxT("E&xit\tCtrl-Q"), wxT("Exit editor"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_EXIT)));
    }

    if (!menu_ && menu && (menu->GetMenuItemCount() == 0))
    {
        delete menu;
        return NULL;
    }

    return menu;
}

wxMenu *wxSTEditorMenuManager::CreateEditMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;
    bool add_sep = false;

    if (HasMenuItemType(STE_MENU_EDIT_MENU, STE_MENU_EDIT_CUTCOPYPASTE))
    {
        if (!HasMenuOptionType(STE_MENU_READONLY))
            menu->Append(MenuItem(menu, wxID_CUT,   _T("Cu&t   \tCtrl-X"), wxT("Cut selected text to clipboard"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_CUT)));
        menu->Append(MenuItem(menu, wxID_COPY,  _T("&Copy  \tCtrl-C"), wxT("Copy selected text to clipboard"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_COPY)));
#ifdef __UNIX__
        menu->Append(ID_STE_COPY_PRIMARY,  _T("Copy primary\tCtrl-Alt-C"), wxT("Copy selected text to primary clipboard"));
#endif // __UNIX__
        if (!HasMenuOptionType(STE_MENU_READONLY))
        {
            menu->Append(MenuItem(menu, wxID_PASTE, _T("&Paste \tCtrl-V"), wxT("Paste text from clipboard"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_PASTE)));
            menu->Append(ID_STE_PASTE_RECT, _T("Paste Rect\tCtrl-Alt-V"), wxT("Paste rectangular text from clipboard"));
        }
        menu->Append(wxID_SELECTALL, _T("Select A&ll\tCtrl-A"), wxT("Selects entire document"));

        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_EDIT_MENU, STE_MENU_EDIT_LINE))
    {
        if (add_sep) menu->AppendSeparator();

        wxMenu *m = new wxMenu();
        // FIXME - these aren't Ctrl-L since the stc handles the keys already (works everywhere?)
        if (!HasMenuOptionType(STE_MENU_READONLY))
            m->Append(ID_STE_LINE_CUT,   _T("Line Cu&t \tCtrl-L"), wxT("Cut current line to clipboard"));

        m->Append(ID_STE_LINE_COPY,  _T("Line &Copy  \tCtrl-Shift-T"), wxT("Copy current line to clipboard"));

        if (!HasMenuOptionType(STE_MENU_READONLY))
        {
            m->Append(ID_STE_LINE_DELETE, _T("Line &Delete \tCtrl-Shift-L"), wxT("Delete current line"));
            m->Append(ID_STE_LINE_TRANSPOSE, _T("Line &Transpose \tCtrl-T"), wxT("Transpose current line upwards"));
            m->Append(ID_STE_LINE_DUPLICATE, _T("Line D&uplicate \tCtrl-D"), wxT("Duplicate current line"));
        }

        menu->Append(ID_STE_MENU_LINE, wxT("L&ine Editing"), m);
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_EDIT_MENU, STE_MENU_EDIT_FINDREPLACE))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(MenuItem(menu, wxID_FIND,        _T("&Find...\tCtrl-F"), wxT("Find text"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_FIND)));
        menu->Append(MenuItem(menu, ID_STE_FIND_NEXT, _T("Find &Next\tF3"),   wxT("Find next occurance"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_FINDNEXT)));
        menu->AppendCheckItem(ID_STE_FIND_DOWN,       _T("Search F&orward\tF2"), wxT("Search forward/reverse in document"));
        if (!HasMenuOptionType(STE_MENU_READONLY))
            menu->Append(MenuItem(menu, ID_STE_REPLACE,   _T("R&eplace...\tCtrl-H"), wxT("Replace text"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_REPLACE)));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_EDIT_MENU, STE_MENU_EDIT_GOTOLINE))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_GOTO_LINE, _T("&Go To... \tCtrl-G"), wxT("Goto line number"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_EDIT_MENU, STE_MENU_EDIT_UNDOREDO) && !HasMenuOptionType(STE_MENU_READONLY))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(MenuItem(menu, wxID_UNDO, _T("&Undo \tCtrl-Z"), wxT("Undo last operation"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_UNDO)));
        menu->Append(MenuItem(menu, wxID_REDO, _T("&Redo \tCtrl-Y"), wxT("Redo last undo"), wxITEM_NORMAL, STE_ARTBMP(wxART_STEDIT_REDO)));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_EDIT_MENU, STE_MENU_EDIT_READONLY) && !HasMenuOptionType(STE_MENU_READONLY))
    {
        if (add_sep) menu->AppendSeparator();

        menu->AppendCheckItem(ID_STE_READONLY, _T("Readonly"), wxT("Make document readonly"));
        add_sep = true;
    }

    if (!menu_ && menu && (menu->GetMenuItemCount() == 0))
    {
        delete menu;
        return NULL;
    }

    return menu;
}

wxMenu *wxSTEditorMenuManager::CreateToolsMenu(wxMenu *menu_) const
{
    // all of these modify the document
    if (HasMenuOptionType(STE_MENU_READONLY))
        return menu_;

    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;
    bool add_sep = false;

    if (HasMenuItemType(STE_MENU_TOOLS_MENU, STE_MENU_TOOLS_CASE))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_UPPERCASE, _T("Selection &uppercase\tCtrl-Shift-U"), wxT("Convert the selected text to uppercase"));
        menu->Append(ID_STE_LOWERCASE, _T("Selection &lowercase\tCtrl-U"), wxT("Convert the selected text to lowercase"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_TOOLS_MENU, STE_MENU_TOOLS_INDENT))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_INCREASE_INDENT, _T("&Increase indent"), wxT("Increase indent of selected text or current line"));
        menu->Append(ID_STE_DECREASE_INDENT, _T("&Decrease indent"), wxT("Decrease indent of selected text or current line"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_TOOLS_MENU, STE_MENU_TOOLS_JOINSPLIT))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_LINES_JOIN,  _T("&Join selected lines\tCtrl-J"), wxT("Join selected lines together"));
        menu->Append(ID_STE_LINES_SPLIT, _T("&Split selected lines\tCtrl-K"), wxT("Split selected lines to edge marker"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_TOOLS_MENU, STE_MENU_TOOLS_TABS_SP))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_TABS_TO_SPACES, _T("Convert &tabs to spaces"), wxT("Convert tabs to spaces in selection or current line"));
        menu->Append(ID_STE_SPACES_TO_TABS, _T("Convert s&paces to tabs"), wxT("Convert spaces to tabs in selection or current line"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_TOOLS_MENU, STE_MENU_TOOLS_EOL))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_CONVERT_EOL, _T("Convert &EOL characters..."), wxT("Convert all end of line characters in doc"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_TOOLS_MENU, STE_MENU_TOOLS_WHITE))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_TRAILING_WHITESPACE, _T("Remove trailing &whitespace\tCtrl-W"), wxT("Remove whitespace at the ends of lines"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_TOOLS_MENU, STE_MENU_TOOLS_INSERT))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_INSERT_TEXT, _T("I&nsert text...\tCtrl-I"), wxT("Prepend, Append, or insert text at column..."));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_TOOLS_MENU, STE_MENU_TOOLS_COLUMNIZE))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_COLUMNIZE, _T("&Columnize..."), wxT("Reformat selected lines in columns..."));
        add_sep = true;
    }

    if (!menu_ && menu && (menu->GetMenuItemCount() == 0))
    {
        delete menu;
        return NULL;
    }

    return menu;
}

wxMenu *wxSTEditorMenuManager::CreateViewMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;
    bool add_sep = false;

    if (HasMenuItemType(STE_MENU_VIEW_MENU, STE_MENU_VIEW_WRAP))
    {
        menu->AppendCheckItem(ID_STE_PREF_WRAP_MODE, _T("&Wrap text to window"), _T("Wrap the text to fit inside window"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_VIEW_MENU, STE_MENU_VIEW_GUI))
    {
        if (add_sep) menu->AppendSeparator();

        menu->AppendCheckItem(ID_STE_PREF_VIEW_EOL,        _T("Show &EOL"), _T("Show end of line symbols"));
        menu->AppendCheckItem(ID_STE_PREF_VIEW_WHITESPACE, _T("Show whi&tespace"), _T("Show whitespace using symbols"));

        menu->AppendSeparator();

        wxMenu *guideMenu = new wxMenu;
        guideMenu->AppendCheckItem(ID_STE_PREF_INDENT_GUIDES,   _T("Show indent &guides"), _T("Show indentation column guides"));
        guideMenu->AppendCheckItem(ID_STE_PREF_EDGE_MODE,       _T("Show l&ong line guide"), _T("Show column guide for long lines"));
        guideMenu->Append(ID_STE_PREF_EDGE_COLUMN,              _T("Set long l&ine guide column..."), _T("Set column long line guide..."));
        menu->Append(ID_STE_MENU_GUIDES, wxT("&Guides"), guideMenu);

        menu->AppendSeparator();

        wxMenu *marginMenu = new wxMenu;
        marginMenu->AppendCheckItem(ID_STE_PREF_VIEW_LINEMARGIN, _T("Show &line number margin"), _T("Show line number margin"));
        marginMenu->AppendCheckItem(ID_STE_PREF_VIEW_MARKERMARGIN, _T("Show &marker margin"), _T("Show a margin for markers"));
        marginMenu->AppendCheckItem(ID_STE_PREF_VIEW_FOLDMARGIN, _T("Show &folding margin"), _T("Show code folding margin"));
        menu->Append(ID_STE_MENU_MARGINS, wxT("&Margins"), marginMenu);
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_VIEW_MENU, STE_MENU_VIEW_FOLD))
    {
        if (add_sep) menu->AppendSeparator();

        wxMenu *foldsMenu = new wxMenu;
        foldsMenu->Append(ID_STE_FOLDS_TOGGLE_CURRENT, _T("To&ggle current fold"), _T("Toggle the current fold level"));
        foldsMenu->Append(ID_STE_FOLDS_COLLAPSE_LEVEL, _T("&Collapse folds below level..."), _T("Collapse all folds below the level in document"));
        foldsMenu->Append(ID_STE_FOLDS_EXPAND_LEVEL,   _T("E&xpand folds above level..."), _T("Expand all folds above the level in document"));
        foldsMenu->Append(ID_STE_FOLDS_COLLAPSE_ALL,   _T("&Collapse all folds"), _T("Collapse all folds in document"));
        foldsMenu->Append(ID_STE_FOLDS_EXPAND_ALL,     _T("E&xpand all folds"), _T("Expand all folds in document"));
        menu->Append(ID_STE_MENU_FOLDING, wxT("&Folding"), foldsMenu);
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_VIEW_MENU, STE_MENU_VIEW_HILIGHT))
    {
        if (add_sep) menu->AppendSeparator();

        menu->AppendCheckItem(ID_STE_PREF_HIGHLIGHT_SYNTAX, _T("S&yntax coloring"), _T("Hilight document based on the syntax"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_VIEW_MENU, STE_MENU_VIEW_ZOOM))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_PREF_ZOOM, _T("&Scale font size..."), _T("Increase or decrease the size of the text"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_VIEW_MENU, STE_MENU_VIEW_FULLSCREEN) && HasMenuOptionType(STE_MENU_FRAME))
    {
        if (add_sep) menu->AppendSeparator();

        menu->AppendCheckItem(ID_STE_SHOW_FULLSCREEN, _T("Show f&ullscreen\tF11"), _T("Show the editor fullscreen"));
        add_sep = true;
    }

    if (!menu_ && menu && (menu->GetMenuItemCount() == 0))
    {
        delete menu;
        return NULL;
    }

    return menu;
}

wxMenu *wxSTEditorMenuManager::CreateBookmarkMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;

    if (HasMenuItemType(STE_MENU_BOOKMARK_MENU, STE_MENU_BOOKMARK_DEFAULT))
    {
        if (!menu) menu = new wxMenu;
        menu->Append(MenuItem(menu, ID_STE_BOOKMARK_TOGGLE, _T("&Toggle bookmark\tF4"), _T("Toggle a bookmark on cursor line"), wxITEM_NORMAL, STE_ARTBMP(wxART_ADD_BOOKMARK)));
        menu->AppendSeparator();
        menu->Append(MenuItem(menu, ID_STE_BOOKMARK_FIRST,    _T("&First bookmark\tF5"),    _T("Goto first bookmark"), wxITEM_NORMAL, STE_ARTBMP(wxART_GO_UP)));
        menu->Append(MenuItem(menu, ID_STE_BOOKMARK_PREVIOUS, _T("&Previous bookmark\tF6"), _T("Goto previous bookmark"), wxITEM_NORMAL, STE_ARTBMP(wxART_GO_BACK)));
        menu->Append(MenuItem(menu, ID_STE_BOOKMARK_NEXT,     _T("&Next bookmark\tF7"),     _T("Goto next bookmark"), wxITEM_NORMAL, STE_ARTBMP(wxART_GO_FORWARD)));
        menu->Append(MenuItem(menu, ID_STE_BOOKMARK_LAST,     _T("&Last bookmark\tF8"),     _T("Goto last bookmark"), wxITEM_NORMAL, STE_ARTBMP(wxART_GO_DOWN)));
        menu->AppendSeparator();
        menu->Append(MenuItem(menu, ID_STE_BOOKMARK_CLEAR, _T("&Clear all bookmarks\tF9"), _T("Clear all bookmarks"), wxITEM_NORMAL, STE_ARTBMP(wxART_DEL_BOOKMARK)));
    }

    return menu;
}

#ifdef GLI_CHANGES

wxMenu *wxSTEditorMenuManager::CreateOpenGLMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;

    menu->Append(ID_STN_COMPILE_SHADER, _T("Compile\tF7"), _T("Compile OpenGL Shader"));

    //Add the GLSL validate option (default checked state)
    wxMenuItem *glslValidate = menu->AppendCheckItem(ID_STN_VALIDATE_SHADER, _T("GLSL Validate"),  _T("Validate GLSL code when compiling"));
    if(glslValidate)
    {
      glslValidate->Check(true);
    }

    menu->AppendSeparator();
    menu->Append(ID_STE_BREAKPOINT_TOGGLE, _T("Toggle Breakpoint\tF9"), _T("Toggle shader breakpoint"));
    menu->AppendSeparator();

    if (!menu_ && menu && (menu->GetMenuItemCount() == 0))
    {
        delete menu;
        return NULL;
    }

    return menu;
}

#endif //GLI_CHANGES

wxMenu *wxSTEditorMenuManager::CreatePreferenceMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;
    bool add_sep = false;

    if (HasMenuItemType(STE_MENU_PREFS_MENU, STE_MENU_PREFS_DLG))
    {
        menu->Append(ID_STE_PREFERENCES, _T("Show &preference dialog..."), _T("Show preference dialog..."));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_PREFS_MENU, STE_MENU_PREFS_INDENT))
    {
        if (add_sep) menu->AppendSeparator();

        menu->AppendCheckItem(ID_STE_PREF_USE_TABS,        _T("Use &tabs"),    _T("Tab key inserts a tab character"));
        menu->AppendCheckItem(ID_STE_PREF_TAB_INDENTS,     _T("Tab &indents"), _T("Tab key indents"));
        menu->AppendCheckItem(ID_STE_PREF_BACKSPACE_UNINDENTS, _T("&Backspace unindents"), _T("Backspace key unindents"));
        menu->AppendCheckItem(ID_STE_PREF_AUTOINDENT,     _T("&Auto indent"), _T("Indent new lines to previous indentation"));

        menu->Append(ID_STE_PREF_TAB_WIDTH,    _T("Set tab &width..."),    _T("Set the number of spaces to show for tab character"));
        menu->Append(ID_STE_PREF_INDENT_WIDTH, _T("Set indent wi&dth..."), _T("Set the number of spaces to use for indentation"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_PREFS_MENU, STE_MENU_PREFS_EOL))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_PREF_EOL_MODE, _T("&EOL Mode..."), wxT("Set the end of line mode"));
        add_sep = true;
    }
    if (HasMenuItemType(STE_MENU_PREFS_MENU, STE_MENU_PREFS_SAVE))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STE_SAVE_PREFERENCES, wxT("Save preferences"), wxT("Save current preferences"));
        add_sep = true;
    }

    if (!menu_ && menu && (menu->GetMenuItemCount() == 0))
    {
        delete menu;
        return NULL;
    }

    return menu;
}

wxMenu *wxSTEditorMenuManager::CreateWindowMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;
    bool add_sep = false;

    if (HasMenuItemType(STE_MENU_WINDOW_MENU, STE_MENU_WINDOW_SPLIT))
    {
        menu = CreateSplitterPopupMenu(menu);
        add_sep = true;
    }

    if (HasMenuItemType(STE_MENU_WINDOW_MENU, STE_MENU_WINDOW_FILECHOOSER))
    {
        if (add_sep) menu->AppendSeparator();
        menu->AppendCheckItem(ID_STF_SHOW_SIDEBAR, _T("&Show sidebar"), _T("Show the sidebar panel"));
    }

    if (HasMenuItemType(STE_MENU_WINDOW_MENU, STE_MENU_WINDOW_PREVNEXT))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STN_WIN_PREVIOUS, wxT("Pre&vious page"), wxT("Goto previous page"));
        menu->Append(ID_STN_WIN_NEXT,     wxT("Ne&xt page"), wxT("Goto next page"));
        add_sep = true;
    }

    if (HasMenuItemType(STE_MENU_WINDOW_MENU, STE_MENU_WINDOW_WINDOWS))
    {
        if (add_sep) menu->AppendSeparator();

        menu->Append(ID_STN_WINDOWS, wxT("&Windows..."), wxT("Manage opened windows"));
        add_sep = true;
    }

    if (!menu_ && menu && (menu->GetMenuItemCount() == 0))
    {
        delete menu;
        return NULL;
    }

    return menu;
}

wxMenu *wxSTEditorMenuManager::CreateHelpMenu(wxMenu *menu_) const
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;

    if (HasMenuOptionType(STE_MENU_FRAME) && HasMenuItemType(STE_MENU_HELP_MENU, STE_MENU_HELP_ABOUT))
    {
        menu->Append(wxID_ABOUT, wxT("&About"), wxT("About this program"));
    }

    if (!menu_ && menu && (menu->GetMenuItemCount() == 0))
    {
        delete menu;
        return NULL;
    }

    return menu;
}

wxMenu* wxSTEditorMenuManager::CreateInsertCharsMenu(wxMenu *menu_, int types)
{
    wxMenu *menu = menu_;
    if (!menu) menu = new wxMenu;

    if (STE_HASBIT(types, STE_MENU_INSERTCHARS_CHARS))
    {
        menu->Append(ID_STEDLG_INSERTMENU_TAB, wxT("Tab character"));
        menu->Append(ID_STEDLG_INSERTMENU_CR,  wxT("Carriage return"));
        menu->Append(ID_STEDLG_INSERTMENU_LF,  wxT("Line feed"));
    }
    if (STE_HASBIT(types, STE_MENU_INSERTCHARS_REGEXP))
    {
        wxMenu* reMenu = new wxMenu;
        reMenu->Append(ID_STEDLG_INSERTMENURE_ANYCHAR,   wxT("Any character"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_RANGE,     wxT("Character in range"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_NOTRANGE,  wxT("Character not in range"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_BEGINLINE, wxT("Beginning of line"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_ENDLINE,   wxT("End of line"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_TAGEXPR,   wxT("Tagged expression"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_0MATCHES,  wxT("0 or more matches"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_1MATCHES,  wxT("1 or more matches"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_01MATCHES, wxT("0 or 1 matches"));
        reMenu->AppendSeparator();
        reMenu->Append(ID_STEDLG_INSERTMENURE_ALPHANUM,  wxT("Alphanumeric characters"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_ALPHA,     wxT("Alphabetical characters"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_NUMERIC,   wxT("Numeric characters"));
        reMenu->Append(ID_STEDLG_INSERTMENURE_TAB,       wxT("Tab characters"));

        menu->Append(ID_STEDLG_MENU_INSERTMENURE, wxT("Regexp"), reMenu);
    }

    if (!menu_ && menu && (menu->GetMenuItemCount() == 0))
    {
        delete menu;
        return NULL;
    }

    return menu;
}

static bool AddAccelFromMenuItem(const wxMenu* menu, wxArrayPtrVoid& entries)
{
    if (!menu)
        return false;

    bool ret = false;
    const wxMenuItemList& itemList = menu->GetMenuItems();
    for ( wxMenuItemList::compatibility_iterator node = itemList.GetFirst();
          node;
          node = node->GetNext() )
    {
        wxMenuItem *menuItem = node->GetData();
        if (!menuItem)
            continue;

        if (menuItem->IsSubMenu())
            ret |= AddAccelFromMenuItem(menuItem->GetSubMenu(), entries);
        else
        {
            wxAcceleratorEntry *entry = menuItem->GetAccel();
            if (entry)
            {
                ret = true;
                // make sure the id is set correctly (for GTK at least)
                entry->Set(entry->GetFlags(), entry->GetKeyCode(), menuItem->GetId());
                bool exists = false;
                for (size_t n = 0; n < entries.GetCount(); n++)
                {
                    if (*entry == *((wxAcceleratorEntry*)entries[n]))
                    {
                        exists = true;
                        break;
                    }
                }

                if (exists)
                    delete entry;
                else
                    entries.Add(entry);
            }
        }
    }

    return ret;
}
bool wxSTEditorMenuManager::GetAcceleratorEntries(const wxMenu* menu,
                                                  const wxMenuBar* menuBar,
                                                  wxArrayPtrVoid& entries) const
{
    bool ret = false;
    if (menu)
        ret = AddAccelFromMenuItem(menu, entries);
    if (menuBar)
    {
        for (size_t n = 0; n < menuBar->GetMenuCount(); n++)
            ret |= AddAccelFromMenuItem(menuBar->GetMenu(n), entries);
    }

    return ret;
}
wxAcceleratorTable wxSTEditorMenuManager::CreateAcceleratorTable(wxArrayPtrVoid& entries)
{
    if (entries.GetCount() == 0)
        return wxAcceleratorTable();

    return wxAcceleratorTable(entries.GetCount(),
#if wxCHECK_VERSION(2,5,0)
                              (wxAcceleratorEntry*)entries.begin());
#else
                              (wxAcceleratorEntry*)entries[0]);
#endif // wxCHECK_VERSION(2,5,0)
}
wxAcceleratorTable wxSTEditorMenuManager::CreateAcceleratorTable(const wxMenu* menu,
                                                                 const wxMenuBar* menuBar)
{
    wxArrayPtrVoid entries;
    GetAcceleratorEntries(menu, menuBar, entries);
    wxAcceleratorTable table(CreateAcceleratorTable(entries));
    while ( entries.GetCount() > 0 )
    {
        wxAcceleratorEntry *item = (wxAcceleratorEntry*)entries[0];
        //wxPrintf(wxT("Accel %d, '%c' %d %d\n"), item->GetFlags(), (wxChar)item->GetKeyCode(), item->GetKeyCode(), item->GetCommand()); fflush(stdout);
        delete item;
        entries.RemoveAt(0);
    }
    return table;
}

void wxSTEditorMenuManager::EnableEditorItems(bool enable, wxMenu *menu,
                                              wxMenuBar *menuBar, wxToolBar *toolBar)
{
    m_enabledEditorItems = enable;

    int n, count;
    for (n = ID_STE_PREF__FIRST; n <= ID_STE_PREF__LAST; n++)
        DoEnableItem(menu, menuBar, toolBar, n, enable);

    for (n = 0; n < int(m_enableItemsArray.GetCount()); n++)
        DoEnableItem(menu, menuBar, toolBar, m_enableItemsArray[n], enable);

    int menuIds[] = {
        wxID_SAVE,
        wxID_SAVEAS,
        ID_STN_SAVE_ALL,
        ID_STN_CLOSE_PAGE,
        ID_STN_CLOSE_ALL,
        ID_STN_CLOSE_PAGE,
        ID_STN_CLOSE_ALL,
        ID_STE_PROPERTIES,
        wxID_PRINT,
        wxID_PREVIEW,
        wxID_PRINT_SETUP,
        ID_STE_PRINT_PAGE_SETUP,
        ID_STE_PRINT_OPTIONS,

        wxID_CUT,
        wxID_COPY,
        ID_STE_COPY_PRIMARY,
        wxID_PASTE,
        ID_STE_PASTE_RECT,
        wxID_SELECTALL,
        wxID_FIND,
        ID_STE_FIND_NEXT,
        ID_STE_FIND_DOWN,
        ID_STE_REPLACE,
        ID_STE_GOTO_LINE,
        wxID_UNDO,
        wxID_REDO,

        ID_STE_UPPERCASE,
        ID_STE_INCREASE_INDENT,
        ID_STE_DECREASE_INDENT,
        ID_STE_LINES_JOIN,
        ID_STE_LINES_SPLIT,
        ID_STE_TABS_TO_SPACES,
        ID_STE_SPACES_TO_TABS,
        ID_STE_CONVERT_EOL,
        ID_STE_TRAILING_WHITESPACE,

        ID_STE_FOLDS_TOGGLE_CURRENT,
        ID_STE_FOLDS_COLLAPSE_LEVEL,
        ID_STE_FOLDS_EXPAND_LEVEL,
        ID_STE_FOLDS_COLLAPSE_ALL,
        ID_STE_FOLDS_EXPAND_ALL,

        ID_STE_BOOKMARK_TOGGLE,
        ID_STE_BOOKMARK_FIRST,
        ID_STE_BOOKMARK_PREVIOUS,
        ID_STE_BOOKMARK_NEXT,
        ID_STE_BOOKMARK_LAST,
        ID_STE_BOOKMARK_CLEAR,

        ID_STE_PREFERENCES,
        ID_STE_SAVE_PREFERENCES,

        ID_STS_UNSPLIT,
        ID_STS_SPLIT_HORIZ,
        ID_STS_SPLIT_VERT
    };

    count = WXSIZEOF(menuIds);
    for (n = 0; n < count; n++)
        DoEnableItem(menu, menuBar, toolBar, menuIds[n], enable);
}

wxMenuItem *wxSTEditorMenuManager::MenuItem(wxMenu *menu, wxWindowID win_id,
                                     const wxString &text, const wxString &help,
                                     wxItemKind kind, const wxBitmap &bitmap) const
{
    wxMenuItem *item = new wxMenuItem(menu, win_id, text, help, kind);
    if (bitmap.Ok())
        item->SetBitmap(bitmap);
    return item;
}

void wxSTEditorMenuManager::DestroyMenuItem(wxMenu *menu, int menu_id, bool clean_sep) const
{
    wxCHECK_RET(menu, wxT("Invalid menu"));
    wxMenuItem *lastItem = menu->FindItem(menu_id);
    if (lastItem)
        menu->Destroy(lastItem);

    if (!clean_sep) return;

    // find any separators that are next to each other and delete them
    wxMenuItemList &menuItems = menu->GetMenuItems();
    wxwxMenuItemListNode *node = menuItems.GetFirst();

    // delete leading separator
    if (node && ((wxMenuItem*)node->GetData())->IsSeparator())
    {
        menu->Destroy((wxMenuItem*)node->GetData());
        node = node->GetNext();
    }

    // delete duplicate separators
    while (node)
    {
        wxMenuItem *item = (wxMenuItem*)node->GetData();
        if (lastItem && lastItem->IsSeparator() && item->IsSeparator())
            menu->Destroy(lastItem);

        lastItem = item;
        node = node->GetNext();
    }

    // delete trailing separator too
    node = menuItems.GetLast();
    if (node && ((wxMenuItem*)node->GetData())->IsSeparator())
    {
        menu->Destroy((wxMenuItem*)node->GetData());
    }
}

bool wxSTEditorMenuManager::DoEnableItem(wxMenu *menu, wxMenuBar *menuBar,
                               wxToolBar *toolBar, wxWindowID menu_id, bool val)
{
    //wxPrintf(wxT("DoEnableItem %d - val %d\n"), menu_id, int(val));
    bool ret = false;

    if (menu)
    {
        wxMenuItem *menuItem = menu->FindItem(menu_id);
        if (menuItem)
        {
            menuItem->Enable(val);
            ret = true;
        }
    }
    if (menuBar)
    {
        wxMenuItem *menuItem = menuBar->FindItem(menu_id);
        if (menuItem)
        {
            menuItem->Enable(val);
            ret = true;
        }
    }
    if (toolBar)
    {
        toolBar->EnableTool(menu_id, val);
        ret = true; // don't know if it exists
    }

    return ret;
}
bool wxSTEditorMenuManager::DoCheckItem(wxMenu *menu, wxMenuBar *menuBar,
                              wxToolBar *toolBar, wxWindowID menu_id, bool val)
{
    //wxPrintf(wxT("DoCheckItem %d - val %d\n"), menu_id, int(val));
    bool ret = false;

    if (menu)
    {
        wxMenuItem *menuItem = menu->FindItem(menu_id);
        if (menuItem)
        {
            menuItem->Check(val);
            ret = true;
        }
    }
    if (menuBar)
    {
        wxMenuItem *menuItem = menuBar->FindItem(menu_id);
        if (menuItem)
        {
            menuItem->Check(val);
            ret = true;
        }
    }
    if (toolBar)
    {
        toolBar->ToggleTool(menu_id, val);
        ret = true; // don't know if it exists
    }

    return ret;
}
bool wxSTEditorMenuManager::DoSetTextItem(wxMenu *menu, wxMenuBar *menuBar,
                                        wxWindowID menu_id, const wxString &val)
{
    bool ret = false;

    if (menu)
    {
        wxMenuItem *menuItem = menu->FindItem(menu_id);
        if (menuItem)
        {
            menuItem->SetText(val);
            ret = true;
        }
    }
    if (menuBar)
    {
        wxMenuItem *menuItem = menuBar->FindItem(menu_id);
        if (menuItem)
        {
            menuItem->SetText(val);
            ret = true;
        }
    }

    return ret;
}

//-----------------------------------------------------------------------------
// wxSTEditorArtProvider
//-----------------------------------------------------------------------------
#include "wx/image.h"

// Bitmaps used for the toolbar in the wxSTEditorFrame
#include "../art/new.xpm"
#include "../art/open.xpm"
#include "../art/save.xpm"
#include "../art/saveall.xpm"
#include "../art/saveas.xpm"
#include "../art/print.xpm"
#include "../art/print_preview.xpm"
#include "../art/print_setup.xpm"
#include "../art/print_page_setup.xpm"
#include "../art/x_red.xpm"

#include "../art/cut.xpm"
#include "../art/copy.xpm"
#include "../art/paste.xpm"
#include "../art/find.xpm"
#include "../art/findnext.xpm"
#include "../art/finddown.xpm"
//#include "../art/findup.xpm"
#include "../art/replace.xpm"
#include "../art/undo.xpm"
#include "../art/redo.xpm"

#ifdef GLI_CHANGES
#include "../art/closefile.xpm"
#include "../art/compile.xpm"

#include "../art/selection.xpm"
#include "../art/zoom.xpm"

#include "../art/R.xpm"
#include "../art/G.xpm"
#include "../art/B.xpm"
#include "../art/A.xpm"
#include "../art/RGB.xpm"

#include "../art/PreImage.xpm"
#include "../art/PostImage.xpm"
#include "../art/DiffImage.xpm"

#include "../art/IntScale.xpm"
#include "../art/Normalize.xpm"
#include "../art/NextRender.xpm"

#endif //GLI_CHANGES


#define ART(artid, xpmRc) \
    if (id == (artid)) return wxBitmap(xpmRc##_xpm);

wxBitmap wxSTEditorArtProvider_GetBitmap(const wxArtID& id)
{
    ART(wxART_STEDIT_NEW,            new)
    ART(wxART_STEDIT_OPEN,           open)
    ART(wxART_STEDIT_SAVE,           save)
    ART(wxART_STEDIT_SAVEALL,        saveall)
    ART(wxART_STEDIT_SAVEAS,         saveas)
    ART(wxART_STEDIT_PRINT,          print)
    ART(wxART_STEDIT_PRINTPREVIEW,   print_preview)
    ART(wxART_STEDIT_PRINTSETUP,     print_setup)
    ART(wxART_STEDIT_PRINTPAGESETUP, print_page_setup)
    ART(wxART_STEDIT_EXIT,           x_red)
    ART(wxART_STEDIT_CUT,            cut)
    ART(wxART_STEDIT_COPY,           copy)
    ART(wxART_STEDIT_PASTE,          paste)
    ART(wxART_STEDIT_FIND,           find)
    ART(wxART_STEDIT_FINDNEXT,       findnext)
    ART(wxART_STEDIT_FINDDOWN,       finddown)
    ART(wxART_STEDIT_REPLACE,        replace)
    ART(wxART_STEDIT_UNDO,           undo)
    ART(wxART_STEDIT_REDO,           redo)

#ifdef GLI_CHANGES
    ART(wxART_STEDIT_CLOSE,          closefile)
    ART(wxART_STEDIT_COMPILE,        compile)

    ART(wxART_STEDIT_SELECTION,      selection)
    ART(wxART_STEDIT_ZOOM,           zoom)
    
    ART(wxART_STEDIT_RED,            R)
    ART(wxART_STEDIT_GREEN,          G)
    ART(wxART_STEDIT_BLUE,           B)
    ART(wxART_STEDIT_ALPHA,          A)
    ART(wxART_STEDIT_RGB,            RGB)    

    ART(wxART_STEDIT_PREIMAGE,       PreImage)
    ART(wxART_STEDIT_POSTIMAGE,      PostImage)        
    ART(wxART_STEDIT_DIFFIMAGE,      DiffImage)                    

    ART(wxART_STEDIT_INTSCALE,       IntScale)
    ART(wxART_STEDIT_NORMALIZE,      Normalize)

    ART(wxART_STEDIT_NEXTRENDER,     NextRender)

#endif //GLI_CHANGES

    return wxNullBitmap;
}

wxBitmap wxSTEditorArtProvider::CreateBitmap(const wxArtID& id,
                                             const wxArtClient& client,
                                             const wxSize& reqSize)
{
    wxBitmap bmp(wxSTEditorArtProvider_GetBitmap(id));

#if wxUSE_IMAGE
    if (bmp.Ok())
    {
        // fit into transparent image with desired size hint from the client
        if (reqSize == wxDefaultSize)
        {
            // find out if there is a desired size for this client
            wxSize bestSize = GetSizeHint(client);
            if (bestSize != wxDefaultSize)
            {
                int bmp_w = bmp.GetWidth();
                int bmp_h = bmp.GetHeight();
                // want default size but it's smaller, paste into transparent image
                if ((bmp_h < bestSize.x) && (bmp_w < bestSize.y))
                {
                    wxPoint offset((bestSize.x - bmp_w)/2, (bestSize.y - bmp_h)/2);
                    wxImage img = bmp.ConvertToImage();
                    img.Resize(bestSize, offset);
                    bmp = wxBitmap(img);
                }
            }
        }
    }
#endif // wxUSE_IMAGE

    return bmp;
}
