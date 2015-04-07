// SciTE - Scintilla based Text Editor
/** @file SciTEBuffers.cxx
 ** Buffers and jobs management.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>

#include "Platform.h"

#if PLAT_GTK

#include <unistd.h>
#include <gtk/gtk.h>

#endif

#if PLAT_WIN

#define _WIN32_WINNT  0x0400
#ifdef _MSC_VER
// windows.h, et al, use a lot of nameless struct/unions - can't fix it, so allow it
#pragma warning(disable: 4201)
#endif
#include <windows.h>
#ifdef _MSC_VER
// okay, that's done, don't allow it in our code
#pragma warning(default: 4201)
#endif
#include <commctrl.h>

// For chdir
#ifdef _MSC_VER
#include <direct.h>
#endif
#ifdef __BORLANDC__
#include <dir.h>
#endif
#ifdef __DMC__
#include <dir.h>
#endif

#endif

#include "SciTE.h"
#include "PropSet.h"
#include "Accessor.h"
#include "WindowAccessor.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"
#include "Extender.h"
#include "FilePath.h"
#include "SciTEBase.h"

const char recentFileName[] = "SciTE.recent";
const char defaultSessionFileName[] = "SciTE.ses";

Job::Job() {
	Clear();
}

void Job::Clear() {
	command = "";
	directory.Init();
	jobType = jobCLI;
	input = "";
	flags = 0;
}

BufferList::BufferList() : current(0), buffers(0), size(0), length(0) {}

BufferList::~BufferList() {
	delete []buffers;
}

void BufferList::Allocate(int maxSize) {
	length = 1;
	current = 0;
	size = maxSize;
	buffers = new Buffer[size];
}

int BufferList::Add() {
	if (length < size) {
		length++;
	}
	buffers[length - 1].Init();

	return length - 1;
}

int BufferList::GetDocumentByName(FilePath filename) {
	if (!filename.IsSet()) {
		return -1;
	}
	for (int i = 0;i < length;i++) {
		if (buffers[i].SameNameAs(filename)) {
			return i;
		}
	}
	return -1;
}

void BufferList::RemoveCurrent() {
	// Delete and move up to fill gap but ensure doc pointer is saved.
	int currentDoc = buffers[current].doc;
	for (int i = current;i < length - 1;i++) {
		buffers[i] = buffers[i + 1];
	}
	buffers[length - 1].doc = currentDoc;

	if (length > 1) {
		length--;

		buffers[length].Init();
		if (current >= length) {
			SetCurrent(length - 1);
		}
		if (current < 0) {
			SetCurrent(0);
		}
	} else {
		buffers[current].Init();
	}
}

int BufferList::Current() {
	return current;
}

void BufferList::SetCurrent(int index) {
	current = index;
}

sptr_t SciTEBase::GetDocumentAt(int index) {
	if (index < 0 || index >= buffers.size) {
		//Platform::DebugPrintf("SciTEBase::GetDocumentAt: Index out of range.\n");
		return 0;
	}
	if (buffers.buffers[index].doc == 0) {
		// Create a new document buffer
		buffers.buffers[index].doc = SendEditor(SCI_CREATEDOCUMENT, 0, 0);
	}
	return buffers.buffers[index].doc;
}

void SciTEBase::SetDocumentAt(int index) {
	int currentbuf = buffers.Current();

	if (	index < 0 ||
	        index >= buffers.length ||
	        index == currentbuf ||
	        currentbuf < 0 ||
	        currentbuf >= buffers.length) {
		return;
	}
	UpdateBuffersCurrent();

	buffers.SetCurrent(index);

	if (extender) {
		if (buffers.size > 1)
			extender->ActivateBuffer(index);
		else
			extender->InitBuffer(0);
	}

	Buffer bufferNext = buffers.buffers[buffers.Current()];
	isDirty = bufferNext.isDirty;
	useMonoFont = bufferNext.useMonoFont;
	unicodeMode = bufferNext.unicodeMode;
	fileModTime = bufferNext.fileModTime;
	overrideExtension = bufferNext.overrideExtension;
	SetFileName(bufferNext);
	SendEditor(SCI_SETDOCPOINTER, 0, GetDocumentAt(buffers.Current()));
	SetWindowName();
	ReadProperties();
	if (unicodeMode != uni8Bit) {
		// Override the code page if Unicode
		codePage = SC_CP_UTF8;
		SendEditor(SCI_SETCODEPAGE, codePage);
	}
	isReadOnly = SendEditor(SCI_GETREADONLY);

	// check to see whether there is saved fold state, restore
	bufferNext.foldState.BeginIteration();
	// Platform::DebugPrintf("Restoring fold state... (%d states)", count);

	int line = 0;
	bool folded = false;
	while (bufferNext.foldState.GetState(&line, &folded)) {
		bool expanded = SendEditor(SCI_GETFOLDEXPANDED, line);
		// set line to state folded
		if (folded && !expanded) {
			SendEditor(SCI_TOGGLEFOLD, line);
		}
		if (!folded && expanded) {
			SendEditor(SCI_TOGGLEFOLD, line);
		}
	}

#if PLAT_WIN
	// Tab Bar
	::SendMessage(reinterpret_cast<HWND>(wTabBar.GetID()), TCM_SETCURSEL, (WPARAM)index, (LPARAM)0);
#endif
#if PLAT_GTK

	if (wTabBar.GetID())
		gtk_notebook_set_page(GTK_NOTEBOOK(wTabBar.GetID()), index);
#endif

	if (lineNumbers && lineNumbersExpand)
		SetLineNumberWidth();

	DisplayAround(bufferNext);

	CheckMenus();
	UpdateStatusBar(true);

	if (extender) {
		extender->OnSwitchFile(filePath.AsFileSystem());
	}
}

void SciTEBase::UpdateBuffersCurrent() {
	int currentbuf = buffers.Current();

	if ((buffers.length > 0) && (currentbuf >= 0)) {
		buffers.buffers[currentbuf].Set(filePath);
		buffers.buffers[currentbuf].selection = GetSelection();
		buffers.buffers[currentbuf].scrollPosition = GetCurrentScrollPosition();
		buffers.buffers[currentbuf].isDirty = isDirty;
		buffers.buffers[currentbuf].useMonoFont = useMonoFont;
		buffers.buffers[currentbuf].fileModTime = fileModTime;
		buffers.buffers[currentbuf].overrideExtension = overrideExtension;
		buffers.buffers[currentbuf].unicodeMode = unicodeMode;

		// retrieve fold state and store in buffer state info
		int maxLine = SendEditor(SCI_GETLINECOUNT);
		int foldPoints = 0;

		for (int line = 0; line < maxLine; line++) {
			int level = SendEditor(SCI_GETFOLDLEVEL, line);
			if (level & SC_FOLDLEVELHEADERFLAG) {

				foldPoints ++;
			}
		}

		FoldState* f = &buffers.buffers[currentbuf].foldState;
		f->Clear();

		if (foldPoints > 0) {
			// Platform::DebugPrintf("Retrieving %d fold points and storing them...", foldPoints);

			f->Alloc(foldPoints);

			for (int line = 0; line < maxLine; line++) {
				int level = SendEditor(SCI_GETFOLDLEVEL, line);
				if (level & SC_FOLDLEVELHEADERFLAG) {

					bool expanded = SendEditor(SCI_GETFOLDEXPANDED, line);
					f->PushState(line, expanded);
				}
			}
		}
	}
}

bool SciTEBase::IsBufferAvailable() {
	return buffers.size > 1 && buffers.length < buffers.size;
}

bool SciTEBase::CanMakeRoom(bool maySaveIfDirty) {
	if (IsBufferAvailable()) {
		return true;
	} else if (maySaveIfDirty) {
		// All available buffers are taken, try and close the current one
		if (SaveIfUnsure(true) != IDCANCEL) {
			// The file isn't dirty, or the user agreed to close the current one
			return true;
		}
	} else {
		return true;	// Told not to save so must be OK.
	}
	return false;
}

void SciTEBase::ClearDocument() {
	SendEditor(SCI_CLEARALL);
	SendEditor(SCI_EMPTYUNDOBUFFER);
	SendEditor(SCI_SETSAVEPOINT);
}

void SciTEBase::InitialiseBuffers() {
	if (buffers.size == 0) {
		int buffersWanted = props.GetInt("buffers");
		if (buffersWanted > bufferMax) {
			buffersWanted = bufferMax;
		}
		if (buffersWanted < 1) {
			buffersWanted = 1;
		}
		buffers.Allocate(buffersWanted);
		// First document is the default from creation of control
		buffers.buffers[0].doc = SendEditor(SCI_GETDOCPOINTER, 0, 0);
		SendEditor(SCI_ADDREFDOCUMENT, 0, buffers.buffers[0].doc); // We own this reference
		if (buffersWanted == 1) {
			// No buffers, delete the Buffers main menu entry
			DestroyMenuItem(menuBuffers, 0);
#if PLAT_WIN
			// Make previous change visible.
			::DrawMenuBar(reinterpret_cast<HWND>(wSciTE.GetID()));
#endif
			// Destroy command "View Tab Bar" in the menu "View"
			DestroyMenuItem(menuView, IDM_VIEWTABBAR);

		}
	}
}

static void EnsureEndsWithPathSeparator(char *path) {
	size_t pathLen = strlen(path);
	if ((pathLen > 0) && path[pathLen - 1] != pathSepChar) {
		strcat(path, pathSepString);
	}
}

static void RecentFilePath(char *path, const char *name) {
	char *where = getenv("SciTE_HOME");
	if (!where)
		where = getenv("HOME");
	if (!where) {
#if PLAT_WIN
		::GetModuleFileName(0, path, MAX_PATH);
		char *lastSlash = strrchr(path, pathSepChar);
		if (lastSlash)
			*(lastSlash + 1) = '\0';
#else

		*path = '\0';
#endif

	} else {
		strcpy(path, where);
		EnsureEndsWithPathSeparator(path);
	}
	strcat(path, configFileVisibilityString);
	strcat(path, name);
}

void SciTEBase::LoadRecentMenu() {
	char recentPathName[MAX_PATH + 1];
	RecentFilePath(recentPathName, recentFileName);
	FILE *recentFile = fopen(recentPathName, fileRead);
	if (!recentFile) {
		DeleteFileStackMenu();
		return;
	}
	char line[MAX_PATH + 1];
	CharacterRange cr;
	cr.cpMin = cr.cpMax = 0;
	for (int i = 0; i < fileStackMax; i++) {
		if (!fgets (line, sizeof (line), recentFile))
			break;
		line[strlen (line) - 1] = '\0';
		AddFileToStack(line, cr, 0);
	}
	fclose(recentFile);
}

void SciTEBase::SaveRecentStack() {
	char recentPathName[MAX_PATH + 1];
	RecentFilePath(recentPathName, recentFileName);
	FILE *recentFile = fopen(recentPathName, fileWrite);
	if (!recentFile)
		return;
	int i;
	// save recent files list
	for (i = fileStackMax - 1; i >= 0; i--) {
		if (recentFileStack[i].IsSet())
			fprintf(recentFile, "%s\n", recentFileStack[i].AsInternal());
	}
	// save buffers list
	for (i = buffers.length - 1; i >= 0; i--) {
		if (buffers.buffers[i].IsSet())
			fprintf(recentFile, "%s\n", buffers.buffers[i].AsInternal());
	}
	fclose(recentFile);
}

void SciTEBase::LoadSession(const char *sessionName) {
	char sessionPathName[MAX_PATH + 1];
	if (sessionName[0] == '\0') {
		RecentFilePath(sessionPathName, defaultSessionFileName);
	} else {
		strcpy(sessionPathName, sessionName);
	}
	FILE *sessionFile = fopen(sessionPathName, fileRead);
	if (!sessionFile)
		return;
	// comment next line if you don't want to close all buffers before loading session
	CloseAllBuffers(true);
	int curr = -1, pos = 0;
	char *file, line[MAX_PATH + 128];
	for (int i = 0; i < bufferMax; i++) {
		if (!fgets (line, sizeof (line), sessionFile))
			break;
		line[strlen (line) - 1] = '\0';
		if (sscanf(line, "<pos=%i>", &pos) != 1)
			break;
		file = strchr(line, '>') + 2;
		//Platform::DebugPrintf("pos=%i file=:%s:", pos, file);
		if (pos < 0) {
			curr = i;
			pos = -pos;
		}
		AddFileToBuffer(file, pos - 1);
	}
	fclose(sessionFile);

	FilePath sessionFilePath = FilePath(sessionPathName).AbsolutePath();
	// add/update SessionPath environment variable
	props.Set("SessionPath", sessionFilePath.AsFileSystem());

	if (curr != -1)
		SetDocumentAt(curr);
}

void SciTEBase::SaveSession(const char *sessionName) {
	char sessionPathName[MAX_PATH + 1];
	if (sessionName[0] == '\0') {
		RecentFilePath(sessionPathName, defaultSessionFileName);
	} else {
		strcpy(sessionPathName, sessionName);
	}
	FILE *sessionFile = fopen(sessionPathName, fileWrite);
	if (!sessionFile)
		return;
	int curr = buffers.Current();
	for (int i = 0; i < buffers.length; i++) {
		if (buffers.buffers[i].IsSet() && !buffers.buffers[i].IsUntitled()) {
			int pos;
			SetDocumentAt(i);
			pos = SendEditor(SCI_GETCURRENTPOS) + 1;
			if (i == curr)
				pos = -pos;
			fprintf(sessionFile, "<pos=%i> %s\n", pos, buffers.buffers[i].AsInternal());
		}
	}
	fclose(sessionFile);

	FilePath sessionFilePath = FilePath(sessionPathName).AbsolutePath();
	// add/update SessionPath environment variable
	props.Set("SessionPath", sessionFilePath.AsFileSystem());

	SetDocumentAt(curr);
}

void SciTEBase::SetIndentSettings() {
	// Get default values
	int useTabs = props.GetInt("use.tabs", 1);
	int tabSize = props.GetInt("tabsize");
	int indentSize = props.GetInt("indent.size");
	// Either set the settings related to the extension or the default ones
	SString fileNameForExtension = ExtensionFileName();
	SString useTabsChars = props.GetNewExpand("use.tabs.",
	                       fileNameForExtension.c_str());
	if (useTabsChars.length() != 0) {
		SendEditor(SCI_SETUSETABS, useTabsChars.value());
	} else {
		SendEditor(SCI_SETUSETABS, useTabs);
	}
	SString tabSizeForExt = props.GetNewExpand("tab.size.",
	                        fileNameForExtension.c_str());
	if (tabSizeForExt.length() != 0) {
		SendEditor(SCI_SETTABWIDTH, tabSizeForExt.value());
	} else if (tabSize != 0) {
		SendEditor(SCI_SETTABWIDTH, tabSize);
	}
	SString indentSizeForExt = props.GetNewExpand("indent.size.",
	                           fileNameForExtension.c_str());
	if (indentSizeForExt.length() != 0) {
		SendEditor(SCI_SETINDENT, indentSizeForExt.value());
	} else {
		SendEditor(SCI_SETINDENT, indentSize);
	}
}

void SciTEBase::SetEol() {
	SString eol_mode = props.Get("eol.mode");
	if (eol_mode == "LF") {
		SendEditor(SCI_SETEOLMODE, SC_EOL_LF);
	} else if (eol_mode == "CR") {
		SendEditor(SCI_SETEOLMODE, SC_EOL_CR);
	} else if (eol_mode == "CRLF") {
		SendEditor(SCI_SETEOLMODE, SC_EOL_CRLF);
	}
}

void SciTEBase::New() {
	InitialiseBuffers();
	UpdateBuffersCurrent();

	if ((buffers.size == 1) && (!buffers.buffers[0].IsUntitled())) {
		AddFileToStack(buffers.buffers[0],
		               buffers.buffers[0].selection,
		               buffers.buffers[0].scrollPosition);
	}

	// If the current buffer is the initial untitled, clean buffer then overwrite it,
	// otherwise add a new buffer.
	if ((buffers.length > 1) ||
	        (buffers.Current() != 0) ||
	        (buffers.buffers[0].isDirty) ||
			(!buffers.buffers[0].IsUntitled())) {
			if (buffers.size == buffers.length) {
				Close(false, false, true);
			}
			buffers.SetCurrent(buffers.Add());
	}

	sptr_t doc = GetDocumentAt(buffers.Current());
	SendEditor(SCI_SETDOCPOINTER, 0, doc);

	filePath.Set("");
	SetFileName(filePath);
	isDirty = false;
	isBuilding = false;
	isBuilt = false;
	isReadOnly = false;	// No sense to create an empty, read-only buffer...

	ClearDocument();
	DeleteFileStackMenu();
	SetFileStackMenu();
	if (extender)
		extender->InitBuffer(buffers.Current());
}

void SciTEBase::Close(bool updateUI, bool loadingSession, bool makingRoomForNew) {
	bool closingLast = false;

#ifdef GLI_CHANGES

  //Save the breakpoints (if any) of the file that is being closed
  SaveBreakPointData();

#endif //GLI_CHANGES

	if (buffers.size == 1) {
		// With no buffer list, Close means close from MRU
		closingLast = !(recentFileStack[0].IsSet());
		buffers.buffers[0].Init();
		buffers.buffers[0].useMonoFont = useMonoFont;
		filePath.Set("");
		ClearDocument(); //avoid double are-you-sure
		if (!makingRoomForNew)
			StackMenu(0); // calls New, or Open, which calls InitBuffer
	} else if (buffers.size > 1) {
		if (buffers.Current() >= 0 && buffers.Current() < buffers.length) {
			UpdateBuffersCurrent();
			Buffer buff = buffers.buffers[buffers.Current()];
			AddFileToStack(buff, buff.selection, buff.scrollPosition);
		}
		closingLast = (buffers.length == 1);
		if (closingLast) {
			buffers.buffers[0].Init();
			buffers.buffers[0].useMonoFont = useMonoFont;
			if (extender)
				extender->InitBuffer(0);
		} else {
			extender->RemoveBuffer(buffers.Current());
			buffers.RemoveCurrent();
			if (extender && !makingRoomForNew)
				extender->ActivateBuffer(buffers.Current());
		}
		Buffer bufferNext = buffers.buffers[buffers.Current()];
		isDirty = bufferNext.isDirty;
		useMonoFont = bufferNext.useMonoFont;
		unicodeMode = bufferNext.unicodeMode;
		fileModTime = bufferNext.fileModTime;
		overrideExtension = bufferNext.overrideExtension;

		if (updateUI)
			SetFileName(bufferNext);
		SendEditor(SCI_SETDOCPOINTER, 0, GetDocumentAt(buffers.Current()));
		if (closingLast) {
			ClearDocument();
		}
		if (updateUI) {
			SetWindowName();
			ReadProperties();
			if (unicodeMode != uni8Bit) {
				// Override the code page if Unicode
				codePage = SC_CP_UTF8;
				SendEditor(SCI_SETCODEPAGE, codePage);
			}
			isReadOnly = SendEditor(SCI_GETREADONLY);

			// check to see whether there is saved fold state, restore
			bufferNext.foldState.BeginIteration();

			int line = 0;
			bool folded = false;
			while (bufferNext.foldState.GetState(&line, &folded)) {
				bool expanded = SendEditor(SCI_GETFOLDEXPANDED, line);
				// set line to state folded
				if (folded && !expanded) {
					SendEditor(SCI_TOGGLEFOLD, line);
				}
				if (!folded && expanded) {
					SendEditor(SCI_TOGGLEFOLD, line);
				}
			}

			DisplayAround(bufferNext);
		}
	}

	if (updateUI) {
		BuffersMenu();
		UpdateStatusBar(true);
	}

	if (closingLast && props.GetInt("quit.on.close.last") && !loadingSession) {
		QuitProgram();
	}
}

void SciTEBase::CloseTab(int tab) {
	int tabCurrent = buffers.Current();
	if (tab == tabCurrent) {
		Close();
	} else {
		FilePath fpCurrent = buffers.buffers[tabCurrent].AbsolutePath();
		SetDocumentAt(tab);
		if (SaveIfUnsure() != IDCANCEL) {
			Close();
			WindowSetFocus(wEditor);
			// Return to the previous buffer
			SetDocumentAt(buffers.GetDocumentByName(fpCurrent));
		}
	}
}

void SciTEBase::CloseAllBuffers(bool loadingSession) {
	if (SaveAllBuffers(false) != IDCANCEL) {
		while (buffers.length > 1)
			Close(false, loadingSession);

		Close(true, loadingSession);
	}
}

int SciTEBase::SaveAllBuffers(bool forceQuestion, bool alwaysYes) {
	int choice = IDYES;
	UpdateBuffersCurrent();	// Ensure isDirty copied
	int currentBuffer = buffers.Current();
	for (int i = 0; (i < buffers.length) && (choice != IDCANCEL); i++) {
		if (buffers.buffers[i].isDirty) {
			SetDocumentAt(i);
			if (alwaysYes) {
				if (!Save()) {
					choice = IDCANCEL;
				}
			} else {
				choice = SaveIfUnsure(forceQuestion);
			}
		}
	}
	SetDocumentAt(currentBuffer);
	return choice;
}

void SciTEBase::SaveTitledBuffers() {
	UpdateBuffersCurrent(); // Ensure isDirty copied
	int currentBuffer = buffers.Current();
	for (int i = 0; i < buffers.length; i++) {
		if (buffers.buffers[i].isDirty && !buffers.buffers[i].IsUntitled()) {
			SetDocumentAt(i);
			Save();
		}
	}
	SetDocumentAt(currentBuffer);
}

void SciTEBase::Next() {
	int next = buffers.Current();
	if (++next >= buffers.length)
		next = 0;
	SetDocumentAt(next);
	CheckReload();
}

void SciTEBase::Prev() {
	int prev = buffers.Current();
	if (--prev < 0)
		prev = buffers.length - 1;

	SetDocumentAt(prev);
	CheckReload();
}

void SciTEBase::BuffersMenu() {
	UpdateBuffersCurrent();
	DestroyMenuItem(menuBuffers, IDM_BUFFERSEP);
#if PLAT_WIN

	::SendMessage(reinterpret_cast<HWND>(wTabBar.GetID()), TCM_DELETEALLITEMS, (WPARAM)0, (LPARAM)0);
#endif

#if PLAT_GTK

	if (wTabBar.GetID()) {
		GtkWidget *tab;

		while ((tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(wTabBar.GetID()), 0)))
			gtk_notebook_remove_page(GTK_NOTEBOOK(wTabBar.GetID()), 0);
	}
#endif
	int pos;
	for (pos = 0; pos < bufferMax; pos++) {
		DestroyMenuItem(menuBuffers, IDM_BUFFER + pos);
	}
	if (buffers.size > 1) {
		int menuStart = 5;
		SetMenuItem(menuBuffers, menuStart, IDM_BUFFERSEP, "");
		for (pos = 0; pos < buffers.length; pos++) {
			int itemID = bufferCmdID + pos;
			char entry[MAX_PATH*2 + 20];
			entry[0] = '\0';
			char titleTab[MAX_PATH*2 + 20];
			titleTab[0] = '\0';
#if PLAT_WIN

			if (pos < 10) {
				sprintf(entry, "&%d ", (pos + 1) % 10 ); // hotkey 1..0
				sprintf(titleTab, "&%d ", (pos + 1) % 10); // add hotkey to the tabbar
			}
#endif

			if (buffers.buffers[pos].IsUntitled()) {
				SString untitled = LocaliseString("Untitled");
				strcat(entry, untitled.c_str());
				strcat(titleTab, untitled.c_str());
			} else {
				SString path = buffers.buffers[pos].AsInternal();
#if PLAT_WIN
				// Handle '&' characters in path, since they are interpreted in
				// menues and tab names.
				int amp = 0;
				while ((amp = path.search("&", amp)) >= 0) {
					path.insert(amp, "&");
					amp += 2;
				}
#endif
				strcat(entry, path.c_str());

				char *cpDirEnd = strrchr(entry, pathSepChar);
				if (cpDirEnd) {
					strcat(titleTab, cpDirEnd + 1);
				} else {
					strcat(titleTab, entry);
				}
			}
			// For short file names:
			//char *cpDirEnd = strrchr(buffers.buffers[pos]->fileName, pathSepChar);
			//strcat(entry, cpDirEnd + 1);

			if (buffers.buffers[pos].isDirty) {
				strcat(entry, " *");
				strcat(titleTab, " *");
			}

			SetMenuItem(menuBuffers, menuStart + pos + 1, itemID, entry);
#if PLAT_WIN
			// Windows specific !
			TCITEM tie;
			tie.mask = TCIF_TEXT | TCIF_IMAGE;
			tie.iImage = -1;

			tie.pszText = titleTab;
			::SendMessage(reinterpret_cast<HWND>(wTabBar.GetID()), TCM_INSERTITEM, (WPARAM)pos, (LPARAM)&tie);
			//::SendMessage(wTabBar.GetID(), TCM_SETCURSEL, (WPARAM)pos, (LPARAM)0);
#endif
#if PLAT_GTK

			if (wTabBar.GetID()) {
				GtkWidget *tablabel, *tabcontent;

				tablabel = gtk_label_new(titleTab);

				if (buffers.buffers[pos].IsUntitled())
					tabcontent = gtk_label_new(LocaliseString("Untitled").c_str());
				else
					tabcontent = gtk_label_new(buffers.buffers[pos].AsInternal());

				gtk_widget_show(tablabel);
				gtk_widget_show(tabcontent);

				gtk_notebook_append_page(GTK_NOTEBOOK(wTabBar.GetID()), tabcontent, tablabel);
			}
#endif

		}
	}
	CheckMenus();
#if PLAT_WIN

	if (tabVisible)
		SizeSubWindows();
#endif
#if PLAT_GTK

	ShowTabBar();
#endif
}

void SciTEBase::DeleteFileStackMenu() {
	for (int stackPos = 0; stackPos < fileStackMax; stackPos++) {
		DestroyMenuItem(menuFile, fileStackCmdID + stackPos);
	}
	DestroyMenuItem(menuFile, IDM_MRU_SEP);
}

void SciTEBase::SetFileStackMenu() {
	if (recentFileStack[0].IsSet()) {
		SetMenuItem(menuFile, MRU_START, IDM_MRU_SEP, "");
		for (int stackPos = 0; stackPos < fileStackMax; stackPos++) {
			//Platform::DebugPrintf("Setfile %d %s\n", stackPos, recentFileStack[stackPos].fileName.c_str());
			int itemID = fileStackCmdID + stackPos;
			if (recentFileStack[stackPos].IsSet()) {
				char entry[MAX_PATH + 20];
				entry[0] = '\0';
#if PLAT_WIN

				sprintf(entry, "&%d ", (stackPos + 1) % 10);
#endif

				strcat(entry, recentFileStack[stackPos].AsInternal());
				SetMenuItem(menuFile, MRU_START + stackPos + 1, itemID, entry);
			}
		}
	}
}

void SciTEBase::DropFileStackTop() {
	DeleteFileStackMenu();
	for (int stackPos = 0; stackPos < fileStackMax - 1; stackPos++)
		recentFileStack[stackPos] = recentFileStack[stackPos + 1];
	recentFileStack[fileStackMax - 1].Init();
	SetFileStackMenu();
}

void SciTEBase::AddFileToBuffer(FilePath file, int pos) {
	// file existence test
	if (file.Exists()) {                      // for missing files Open() gives an empty buffer - do not want this
		Open(file, ofForceLoad);
		SendEditor(SCI_GOTOPOS, pos);
	}
}

void SciTEBase::AddFileToStack(FilePath file, CharacterRange selection, int scrollPos) {
	if (!file.IsSet())
		return;
	DeleteFileStackMenu();
	// Only stack non-empty names
	if (file.IsSet() && !file.IsUntitled()) {
		int stackPos;
		int eqPos = fileStackMax - 1;
		for (stackPos = 0; stackPos < fileStackMax; stackPos++)
			if (recentFileStack[stackPos].SameNameAs(file))
				eqPos = stackPos;
		for (stackPos = eqPos; stackPos > 0; stackPos--)
			recentFileStack[stackPos] = recentFileStack[stackPos - 1];
		recentFileStack[0].Set(file);
		recentFileStack[0].selection = selection;
		recentFileStack[0].scrollPosition = scrollPos;
	}
	SetFileStackMenu();
}

void SciTEBase::RemoveFileFromStack(FilePath file) {
	if (!file.IsSet())
		return;
	DeleteFileStackMenu();
	int stackPos;
	for (stackPos = 0; stackPos < fileStackMax; stackPos++) {
		if (recentFileStack[stackPos].SameNameAs(file)) {
			for (int movePos = stackPos; movePos < fileStackMax - 1; movePos++)
				recentFileStack[movePos] = recentFileStack[movePos + 1];
			recentFileStack[fileStackMax - 1].Init();
			break;
		}
	}
	SetFileStackMenu();
}

RecentFile SciTEBase::GetFilePosition() {
	RecentFile rf;
	rf.selection = GetSelection();
	rf.scrollPosition = GetCurrentScrollPosition();
	return rf;
}

void SciTEBase::DisplayAround(const RecentFile &rf) {
	if ((rf.selection.cpMin != INVALID_POSITION) && (rf.selection.cpMax != INVALID_POSITION)) {
		// This can produce better file state restoring
		bool foldOnOpen = props.GetInt("fold.on.open");
		//~ if (foldOnOpen)
			//~ FoldAll();

		int lineStart = SendEditor(SCI_LINEFROMPOSITION, rf.selection.cpMin);
		SendEditor(SCI_ENSUREVISIBLEENFORCEPOLICY, lineStart);
		int lineEnd = SendEditor(SCI_LINEFROMPOSITION, rf.selection.cpMax);
		SendEditor(SCI_ENSUREVISIBLEENFORCEPOLICY, lineEnd);
		SetSelection(rf.selection.cpMax, rf.selection.cpMin);

		// Folding can mess up next scrolling, so will be better without scrolling
		if (!foldOnOpen) {
			int curTop = SendEditor(SCI_GETFIRSTVISIBLELINE);
			int lineTop = SendEditor(SCI_VISIBLEFROMDOCLINE, rf.scrollPosition);
			SendEditor(SCI_LINESCROLL, 0, lineTop - curTop);
		}
	}
}

// Next and Prev file comments.
// Decided that "Prev" file should mean the file you had opened last
// This means "Next" file means the file you opened longest ago.
void SciTEBase::StackMenuNext() {
	DeleteFileStackMenu();
	for (int stackPos = fileStackMax - 1; stackPos >= 0;stackPos--) {
		if (recentFileStack[stackPos].IsSet()) {
			SetFileStackMenu();
			StackMenu(stackPos);
			return;
		}
	}
	SetFileStackMenu();
}

void SciTEBase::StackMenuPrev() {
	if (recentFileStack[0].IsSet()) {
		// May need to restore last entry if removed by StackMenu
		RecentFile rfLast = recentFileStack[fileStackMax - 1];
		StackMenu(0);	// Swap current with top of stack
		for (int checkPos = 0; checkPos < fileStackMax; checkPos++) {
			if (rfLast.SameNameAs(recentFileStack[checkPos])) {
				rfLast.Init();
			}
		}
		// And rotate the MRU
		RecentFile rfCurrent = recentFileStack[0];
		// Move them up
		for (int stackPos = 0; stackPos < fileStackMax - 1; stackPos++) {
			recentFileStack[stackPos] = recentFileStack[stackPos + 1];
		}
		recentFileStack[fileStackMax - 1].Init();
		// Copy current file into first empty
		for (int emptyPos = 0; emptyPos < fileStackMax; emptyPos++) {
			if (!recentFileStack[emptyPos].IsSet()) {
				if (rfLast.IsSet()) {
					recentFileStack[emptyPos] = rfLast;
					rfLast.Init();
				} else {
					recentFileStack[emptyPos] = rfCurrent;
					break;
				}
			}
		}

		DeleteFileStackMenu();
		SetFileStackMenu();
	}
}

void SciTEBase::StackMenu(int pos) {
	//Platform::DebugPrintf("Stack menu %d\n", pos);
	if (CanMakeRoom(true)) {
		if (pos >= 0) {
			if ((pos == 0) && (!recentFileStack[pos].IsSet())) {	// Empty
				New();
				SetWindowName();
				ReadProperties();
				SetIndentSettings();
			} else if (recentFileStack[pos].IsSet()) {
				RecentFile rf = recentFileStack[pos];
				//Platform::DebugPrintf("Opening pos %d %s\n",recentFileStack[pos].lineNumber,recentFileStack[pos].fileName);
				overrideExtension = "";
				// Already asked user so don't allow Open to ask again.
				Open(rf, ofNoSaveIfDirty);
				DisplayAround(rf);
			}
		}
	}
}

void SciTEBase::RemoveToolsMenu() {
	for (int pos = 0; pos < toolMax; pos++) {
		DestroyMenuItem(menuTools, IDM_TOOLS + pos);
	}
}

void SciTEBase::SetMenuItemLocalised(int menuNumber, int position, int itemID,
                                     const char *text, const char *mnemonic) {
	SString localised = LocaliseString(text);
	SetMenuItem(menuNumber, position, itemID, localised.c_str(), mnemonic);
}

void SciTEBase::SetToolsMenu() {
	//command.name.0.*.py=Edit in PythonWin
	//command.0.*.py="c:\program files\python\pythonwin\pythonwin" /edit c:\coloreditor.py
	RemoveToolsMenu();
	int menuPos = TOOLS_START;
	for (int item = 0; item < toolMax; item++) {
		int itemID = IDM_TOOLS + item;
		SString prefix = "command.name.";
		prefix += SString(item);
		prefix += ".";
		SString commandName = props.GetNewExpand(prefix.c_str(), filePath.AsInternal());
		if (commandName.length()) {
			SString sMenuItem = commandName;
			prefix = "command.shortcut.";
			prefix += SString(item);
			prefix += ".";
			SString sMnemonic = props.GetNewExpand(prefix.c_str(), filePath.AsInternal());
			if (item < 10 && sMnemonic.length() == 0) {
				sMnemonic += "Ctrl+";
				sMnemonic += SString(item);
			}
			SetMenuItem(menuTools, menuPos, itemID, sMenuItem.c_str(), sMnemonic[0] ? sMnemonic.c_str() : NULL);
			menuPos++;
		}
	}

	DestroyMenuItem(menuTools, IDM_MACRO_SEP);
	DestroyMenuItem(menuTools, IDM_MACROLIST);
	DestroyMenuItem(menuTools, IDM_MACROPLAY);
	DestroyMenuItem(menuTools, IDM_MACRORECORD);
	DestroyMenuItem(menuTools, IDM_MACROSTOPRECORD);
	menuPos++;
	if (macrosEnabled) {
		SetMenuItem(menuTools, menuPos++, IDM_MACRO_SEP, "");
		SetMenuItemLocalised(menuTools, menuPos++, IDM_MACROLIST,
		                     "&List Macros...", "Shift+F9");
		SetMenuItemLocalised(menuTools, menuPos++, IDM_MACROPLAY,
		                     "Run Current &Macro", "F9");
		SetMenuItemLocalised(menuTools, menuPos++, IDM_MACRORECORD,
		                     "&Record Macro", "Ctrl+F9");
		SetMenuItemLocalised(menuTools, menuPos++, IDM_MACROSTOPRECORD,
		                     "S&top Recording Macro", "Ctrl+Shift+F9");
	}
}

JobSubsystem SciTEBase::SubsystemType(char c) {
	if (c == '1')
		return jobGUI;
	else if (c == '2')
		return jobShell;
	else if (c == '3')
		return jobExtension;
	else if (c == '4')
		return jobHelp;
	else if (c == '5')
		return jobOtherHelp;
	return jobCLI;
}

JobSubsystem SciTEBase::SubsystemType(const char *cmd, int item) {
	SString subsysprefix = cmd;
	if (item >= 0) {
		subsysprefix += SString(item);
		subsysprefix += ".";
	}
	SString subsystem = props.GetNewExpand(subsysprefix.c_str(), filePath.AsInternal());
	return SubsystemType(subsystem[0]);
}

void SciTEBase::ToolsMenu(int item) {
	SelectionIntoProperties();

	SString itemSuffix = item;
	itemSuffix += '.';

	SString propName = "command.";
	propName += itemSuffix;

	SString command = props.GetWild(propName.c_str(), filePath.AsInternal());
	if (command.length()) {
		int saveBefore = 0;

		JobSubsystem jobType = jobCLI;
		bool filter = false;
		bool quiet = false;
		int repSel = 0;
		bool groupUndo = false;

		propName = "command.mode.";
		propName += itemSuffix;
		SString modeVal = props.GetNewExpand(propName.c_str(), filePath.AsInternal());
		modeVal.remove(" ");
		if (modeVal.length()) {
			char *modeTags = modeVal.detach();

			// copy/paste from style selectors.
			char *opt = modeTags;
			while (opt) {
				// Find attribute separator
				char *cpComma = strchr(opt, ',');
				if (cpComma) {
					// If found, we terminate the current attribute (opt) string
					*cpComma = '\0';
				}
				// Find attribute name/value separator
				char *colon = strchr(opt, ':');
				if (colon) {
					// If found, we terminate the current attribute name and point on the value
					*colon++ = '\0';
				}

				if (0 == strcmp(opt, "subsystem") && colon) {
					if (colon[0] == '0' || 0==strcmp(colon, "console"))
						jobType = jobCLI;
					else if (colon[0] == '1' || 0==strcmp(colon, "windows"))
						jobType = jobGUI;
					else if (colon[0] == '2' || 0==strcmp(colon, "shellexec"))
						jobType = jobShell;
					else if (colon[0] == '3' || 0==strcmp(colon, "lua") || strcmp(colon, "director"))
						jobType = jobExtension;
					else if (colon[0] == '4' || 0==strcmp(colon, "winhelp"))
						jobType = jobHelp;
					else if (colon[0] == '5' || 0==strcmp(colon, "htmlhelp"))
						jobType = jobOtherHelp;
				}

				if (0 == strcmp(opt, "quiet")) {
					if (!colon || colon[0] == '1' || 0==strcmp(colon, "yes"))
						quiet = true;
					else if (colon[0] == '0' || 0==strcmp(colon, "no"))
						quiet = false;
				}

				if (0 == strcmp(opt, "savebefore")) {
					if (!colon || colon[0] == '1' || 0==strcmp(colon, "yes"))
						saveBefore = 1;
					else if (colon[0] == '0' || 0==strcmp(colon, "no"))
						saveBefore = 2;
					else if (0==strcmp(colon, "prompt"))
						saveBefore = 0;
				}

				if (0 == strcmp(opt, "filter")) {
					if (!colon || colon[0] == '1' || 0==strcmp(colon, "yes"))
						filter = true;
					else if (colon[1] == '0' || 0==strcmp(colon, "no"))
						filter = false;
				}

				if (0 == strcmp(opt, "replaceselection")) {
					if (!colon || colon[0] == '1' || 0==strcmp(colon, "yes"))
						repSel = 1;
					else if (colon[0] == '0' || 0==strcmp(colon, "no"))
						repSel = 0;
					else if (0==strcmp(colon, "auto"))
						repSel = 2;
				}

				if (0 == strcmp(opt, "groupundo")) {
					if (!colon || colon[0] == '1' || 0==strcmp(colon, "yes"))
						groupUndo = true;
					else if (colon[0] == '0' || 0==strcmp(colon, "no"))
						groupUndo = false;
				}

				opt = cpComma ? cpComma + 1 : 0;
			}
			delete []modeTags;
		}

		// The mode flags also have classic properties with similar effect.
		// If the classic property is specified, it overrides the mode.
		// To see if the property is absent (as opposed to merely evaluating
		// to nothing after variable expansion), use GetWild for the
		// existence check.  However, for the value check, use getNewExpand.

		propName = "command.save.before.";
		propName += itemSuffix;
		if (props.GetWild(propName.c_str(), filePath.AsInternal()).length())
			saveBefore = props.GetNewExpand(propName.c_str(), filePath.AsInternal()).value();

		if (saveBefore == 2 || (saveBefore == 1 && Save()) || SaveIfUnsure() != IDCANCEL) {
			int flags = 0;

			propName = "command.is.filter.";
			propName += itemSuffix;
			if (props.GetWild(propName.c_str(), filePath.AsInternal()).length())
				filter = (props.GetNewExpand(propName.c_str(), filePath.AsInternal())[0]=='1');
			if (filter)
				fileModTime -= 1;

			propName = "command.subsystem.";
			propName += itemSuffix;
			if (props.GetWild(propName.c_str(), filePath.AsInternal()).length()) {
				SString subsystemVal = props.GetNewExpand(propName.c_str(), filePath.AsInternal());
				jobType = SubsystemType(subsystemVal[0]);
			}

			propName = "command.input.";
			propName += itemSuffix;
			SString input;
			if (props.GetWild(propName.c_str(), filePath.AsInternal()).length()) {
				input = props.GetNewExpand(propName.c_str(), filePath.AsInternal());
				flags |= jobHasInput;
			}

			propName = "command.quiet.";
			propName += itemSuffix;
			if (props.GetWild(propName.c_str(), filePath.AsInternal()).length())
				quiet = (props.GetNewExpand(propName.c_str(), filePath.AsInternal()).value()==1);
			if (quiet)
				flags |= jobQuiet;

			propName = "command.replace.selection.";
			propName += itemSuffix;
			if (props.GetWild(propName.c_str(), filePath.AsInternal()).length())
				repSel = props.GetNewExpand(propName.c_str(), filePath.AsInternal()).value();

			if (repSel == 1)
				flags |= jobRepSelYes;
			else if (repSel == 2)
				flags |= jobRepSelAuto;

			if (groupUndo)
				flags |= jobGroupUndo;

			AddCommand(command, "", jobType, input, flags);
			if (commandCurrent > 0)
				Execute();
		}
	}
}

inline bool isdigitchar(int ch) {
    return (ch >= '0') && (ch <= '9');
}

int DecodeMessage(char *cdoc, char *sourcePath, int format, int &column) {
	sourcePath[0] = '\0';
	column= -1; // default to not detected
	switch (format) {
	case SCE_ERR_PYTHON: {
			// Python
			char *startPath = strchr(cdoc, '\"') + 1;
			char *endPath = strchr(startPath, '\"');
			int length = endPath - startPath;
			if (length > 0) {
				strncpy(sourcePath, startPath, length);
				sourcePath[length] = 0;
			}
			endPath++;
			while (*endPath && !isdigitchar(*endPath)) {
				endPath++;
			}
			int sourceNumber = atoi(endPath) - 1;
			return sourceNumber;
		}
	case SCE_ERR_GCC: {
			// GCC - look for number after colon to be line number
			// This will be preceded by file name.
			// Lua debug traceback messages also piggyback this style, but begin with a tab.
			if (cdoc[0] == '\t')
				++cdoc;
			for (int i = 0; cdoc[i]; i++) {
				if (cdoc[i] == ':' && isdigitchar(cdoc[i + 1])) {
					int sourceNumber = atoi(cdoc + i + 1) - 1;
					if (i > 0) {
						strncpy(sourcePath, cdoc, i);
						sourcePath[i] = 0;
					}
					return sourceNumber;
				}
			}
			break;
		}
	case SCE_ERR_MS: {
			// Visual *
			char *start = cdoc;
			while (isspacechar(*start)) {
				start++;
			}
			char *endPath = strchr(start, '(');
			int length = endPath - start;
			if ((length > 0) && (length < MAX_PATH)) {
				strncpy(sourcePath, start, length);
				sourcePath[length] = 0;
			}
			endPath++;
			return atoi(endPath) - 1;
		}
	case SCE_ERR_BORLAND: {

    //The borland tag is hijacked for GLSL 
    char *space = strchr(cdoc, ':');
    if (space)
    {
      char *space2 = strchr(space+1, ':');
      if(space2)
      {
        return atoi(space2+1) -1;
      }
    }

    /*
      // Borland
			char *space = strchr(cdoc, ' ');
			if (space) {
				while (isspacechar(*space)) {
					space++;
				}
				while (*space && !isspacechar(*space)) {
					space++;
				}
				while (isspacechar(*space)) {
					space++;
				}

				char *space2 = NULL;

				if (strlen(space) > 2) {
					space2 = strchr(space + 2, ':');
				}

				if (space2) {
					while (!isspacechar(*space2)) {
						space2--;
					}

					while (isspacechar(*(space2 - 1))) {
						space2--;
					}

					int length = space2 - space;

					if (length > 0) {
						strncpy(sourcePath, space, length);
						sourcePath[length] = '\0';
						return atoi(space2) - 1;
					}
				}
			}
      */
			break;
		}
	case SCE_ERR_PERL: {
			// perl
			char *at = strstr(cdoc, " at ");
			char *line = strstr(cdoc, " line ");
			int length = line - (at + 4);
			if (at && line && length > 0) {
				strncpy(sourcePath, at + 4, length);
				sourcePath[length] = 0;
				line += 6;
				return atoi(line) - 1;
			}
			break;
		}
	case SCE_ERR_NET: {
			// .NET traceback
			char *in = strstr(cdoc, " in ");
			char *line = strstr(cdoc, ":line ");
			if (in && line && (line > in)) {
				in += 4;
				strncpy(sourcePath, in, line - in);
				sourcePath[line - in] = 0;
				line += 6;
				return atoi(line) - 1;
			}
		}
	case SCE_ERR_LUA: {
			// Lua error look like: last token read: `result' at line 40 in file `Test.lua'
			char *idLine = "at line ";
			char *idFile = "file ";
			size_t lenLine = strlen(idLine);
			size_t lenFile = strlen(idFile);
			char *line = strstr(cdoc, idLine);
			char *file = strstr(cdoc, idFile);
			if (line && file) {
				char *quote = strstr(file, "'");
				size_t length = quote - (file + lenFile + 1);
				if (quote && length > 0) {
					strncpy(sourcePath, file + lenFile + 1, length);
					sourcePath[length] = '\0';
				}
				line += lenLine;
				return atoi(line) - 1;
			}
			break;
		}

	case SCE_ERR_CTAG: {
			for (int i = 0; cdoc[i]; i++) {
				if ((isdigitchar(cdoc[i + 1]) || (cdoc[i + 1] == '/' && cdoc[i + 2] == '^')) && cdoc[i] == '\t') {
					int j = i - 1;
					while (j > 0 && ! strchr("\t\n\r \"$%'*,;<>?[]^`{|}", cdoc[j])) {
						j--;
					}
					if (strchr("\t\n\r \"$%'*,;<>?[]^`{|}", cdoc[j])) {
						j++;
					}
					strncpy(sourcePath, &cdoc[j], i - j);
					sourcePath[i - j] = 0;
					// Because usually the address is a searchPattern, lineNumber has to be evaluated later
					return 0;
				}
			}
		}
	case SCE_ERR_PHP: {
			// PHP error look like: Fatal error: Call to undefined function:  foo() in example.php on line 11
			char *idLine = " on line ";
			char *idFile = " in ";
			size_t lenLine = strlen(idLine);
			size_t lenFile = strlen(idFile);
			char *line = strstr(cdoc, idLine);
			char *file = strstr(cdoc, idFile);
			if (line && file && (line > file)) {
				file += lenFile;
				size_t length = line - file;
				strncpy(sourcePath, file, length);
				sourcePath[length] = '\0';
				line += lenLine;
				return atoi(line) - 1;
			}
			break;
		}

	case SCE_ERR_ELF: {
			// Essential Lahey Fortran error look like: Line 11, file c:\fortran90\codigo\demo.f90
			char *line = strchr(cdoc, ' ');
			if (line) {
				while (isspacechar(*line)) {
					line++;
				}
				char *file = strchr(line, ' ');
				if (file) {
					while (isspacechar(*file)) {
						file++;
					}
					while (*file && !isspacechar(*file)) {
						file++;
					}
					size_t length = strlen(file);
					strncpy(sourcePath, file, length);
					sourcePath[length] = '\0';
					return atoi(line) - 1;
				}
			}
			break;
		}

	case SCE_ERR_IFC: {
			/* Intel Fortran Compiler error/warnings look like:
			 * Error 71 at (17:teste.f90) : The program unit has no name
			 * Warning 4 at (9:modteste.f90) : Tab characters are an extension to standard Fortran 95
			 *
			 * Depending on the option, the error/warning messages can also appear on the form:
			 * modteste.f90(9): Warning 4 : Tab characters are an extension to standard Fortran 95
			 *
			 * These are trapped by the MS handler, and are identified OK, so no problem...
			 */
			char *line = strchr(cdoc, '(');
			char *file = strchr(line, ':');
			if (line && file) {
				file++;
				char *endfile = strchr(file, ')');
				size_t length = endfile - file;
				strncpy(sourcePath, file, length);
				sourcePath[length] = '\0';
				line++;
				return atoi(line) - 1;
			}
			break;
		}

	case SCE_ERR_ABSF: {
			// Absoft Pro Fortran 90/95 v8.x, v9.x  errors look like: cf90-113 f90fe: ERROR SHF3D, File = shf.f90, Line = 1101, Column = 19
			char *idFile = " File = ";
			char *idLine = ", Line = ";
			char *idColumn = ", Column = ";
			size_t lenFile = strlen(idFile);
			size_t lenLine = strlen(idLine);
			char *file = strstr(cdoc, idFile);
			char *line = strstr(cdoc, idLine);
			char *column = strstr(cdoc, idColumn);
			if (line && file && (line > file)) {
				file += lenFile;
				size_t length = line - file;
				strncpy(sourcePath, file, length);
				sourcePath[length] = '\0';
				line += lenLine;
				length = column - line;
				strncpy(line, line, length);
				return atoi(line) - 1;
			}
			break;
		}

	case SCE_ERR_IFORT: {
			/* Intel Fortran Compiler v8.x error/warnings look like:
			 * fortcom: Error: shf.f90, line 5602: This name does not have ...
				 */
			char *idFile = ": Error: ";
			char *idLine = ", line ";
			size_t lenFile = strlen(idFile);
			size_t lenLine = strlen(idLine);
			char *file = strstr(cdoc, idFile);
			char *line = strstr(cdoc, idLine);
			char *lineend = strrchr(cdoc, ':');
			if (line && file && (line > file)) {
				file += lenFile;
				size_t length = line - file;
				strncpy(sourcePath, file, length);
				sourcePath[length] = '\0';
				line += lenLine;
				if ((lineend > line)) {
					length = lineend - line;
					strncpy(line, line, length);
					return atoi(line) - 1;
				}
			}
			break;
		}

	case SCE_ERR_TIDY: {
			/* HTML Tidy error/warnings look like:
			 * line 8 column 1 - Error: unexpected </head> in <meta>
			 * line 41 column 1 - Warning: <table> lacks "summary" attribute
			 */
			char *line = strchr(cdoc, ' ');
			if (line) {
				char *col = strchr(line+1, ' ');
				if (col) {
					*col = '\0';
					int lnr = atoi(line) - 1;
					col = strchr(col+1, ' ');
					if (col) {
						char *endcol = strchr(col+1, ' ');
						if (endcol) {
							*endcol = '\0';
							column = atoi(col)-1;
							return lnr;
						}
					}
				}
			}
			break;
		}

	case SCE_ERR_JAVA_STACK: {
			/* Java runtime stack trace
				\tat <methodname>(<filename>:<line>)
				 */
			char *startPath = strchr(cdoc, '(') + 1;
			char *endPath = strchr(startPath, ':');
			int length = endPath - startPath;
			if (length > 0) {
				strncpy(sourcePath, startPath, length);
				sourcePath[length] = 0;
				int sourceNumber = atoi(endPath+1) - 1;
				return sourceNumber;
			}
			break;
		}
		
	case SCE_ERR_DIFF_MESSAGE: {
			// Diff file header, either +++ <filename>\t or --- <filename>\t
			// Often followed by a position line @@ <linenumber>
			char *startPath = cdoc + 4;
			char *endPath = strchr(startPath, '\t');
			if (endPath) {
				int length = endPath - startPath;
				strncpy(sourcePath, startPath, length);
				sourcePath[length] = 0;
				return 0;
			}
			break;
		}
	}	// switch
	return -1;
}

void SciTEBase::GoMessage(int dir) {

	CharacterRange crange;
	crange.cpMin = SendOutput(SCI_GETSELECTIONSTART);
	crange.cpMax = SendOutput(SCI_GETSELECTIONEND);
	int selStart = crange.cpMin;
	int curLine = SendOutput(SCI_LINEFROMPOSITION, selStart);
	int maxLine = SendOutput(SCI_GETLINECOUNT);
	int lookLine = curLine + dir;
	if (lookLine < 0)
		lookLine = maxLine - 1;
	else if (lookLine >= maxLine)
		lookLine = 0;
	WindowAccessor acc(wOutput.GetID(), props);
	while ((dir == 0) || (lookLine != curLine)) {
		int startPosLine = SendOutput(SCI_POSITIONFROMLINE, lookLine, 0);
		int lineLength = SendOutput(SCI_LINELENGTH, lookLine, 0);
		//Platform::DebugPrintf("GOMessage %d %d %d of %d linestart = %d\n", selStart, curLine, lookLine, maxLine, startPosLine);
		char style = acc.StyleAt(startPosLine);
		if (style != SCE_ERR_DEFAULT &&
			style != SCE_ERR_CMD &&
			style != SCE_ERR_DIFF_ADDITION &&
			style != SCE_ERR_DIFF_CHANGED &&
			style != SCE_ERR_DIFF_DELETION) {
			//Platform::DebugPrintf("Marker to %d\n", lookLine);
			SendOutput(SCI_MARKERDELETEALL, static_cast<uptr_t>(-1));
			SendOutput(SCI_MARKERDEFINE, 0, SC_MARK_SMALLRECT);
			SendOutput(SCI_MARKERSETFORE, 0, ColourOfProperty(props,
			           "error.marker.fore", ColourDesired(0x7f, 0, 0)));
			SendOutput(SCI_MARKERSETBACK, 0, ColourOfProperty(props,
			           "error.marker.back", ColourDesired(0xff, 0xff, 0)));
			SendOutput(SCI_MARKERADD, lookLine, 0);
			SendOutput(SCI_SETSEL, startPosLine, startPosLine);
			char *cdoc = new char[lineLength + 1];
			if (!cdoc)
				return;
			GetRange(wOutput, startPosLine, startPosLine + lineLength, cdoc);
			SString message(cdoc);
			char sourcePath[MAX_PATH];
			int column;
			int sourceLine = DecodeMessage(cdoc, sourcePath, style, column);
			//Platform::DebugPrintf("<%s> %d %d\n",sourcePath, sourceLine, lookLine);
			if (sourceLine >= 0) {
				if (!filePath.Name().SameNameAs(FilePath(sourcePath))) {
					FilePath messagePath;
					bool bExists = false;
					if (Exists(dirNameAtExecute.AsInternal(), sourcePath, &messagePath)) {
						bExists = true;
					} else if (Exists(dirNameForExecute.AsInternal(), sourcePath, &messagePath)) {
						bExists = true;
					} else if (Exists(filePath.Directory().AsInternal(), sourcePath, &messagePath)) {
						bExists = true;
					} else if (Exists(NULL, sourcePath, &messagePath)) {
						bExists = true;
					}
					if (bExists) {
						if (!Open(messagePath)) {
							delete []cdoc;
							return;
						}
						CheckReload();
					}
				}

				// If ctag then get line number after search tag or use ctag line number
				if (style == SCE_ERR_CTAG) {
					char cTag[200];
					//without following focus GetCTag wouldn't work correct
					WindowSetFocus(wOutput);
					GetCTag(cTag, 200);
					if (cTag[0] != '\0') {
						if (atol(cTag) > 0) {
							//if tag is linenumber, get line
							sourceLine = atol(cTag) - 1;
						} else {
							findWhat = cTag;
							FindNext(false);
							//get linenumber for marker from found position
							sourceLine = SendEditor(SCI_LINEFROMPOSITION, SendEditor(SCI_GETCURRENTPOS));
						}
					}
				}

				SendEditor(SCI_MARKERDELETEALL, 0);
				SendEditor(SCI_MARKERDEFINE, 0, SC_MARK_CIRCLE);
				SendEditor(SCI_MARKERSETFORE, 0, ColourOfProperty(props,
				           "error.marker.fore", ColourDesired(0x7f, 0, 0)));
				SendEditor(SCI_MARKERSETBACK, 0, ColourOfProperty(props,
				           "error.marker.back", ColourDesired(0xff, 0xff, 0)));
				SendEditor(SCI_MARKERADD, sourceLine, 0);
				int startSourceLine = SendEditor(SCI_POSITIONFROMLINE, sourceLine, 0);
				int endSourceline= SendEditor(SCI_POSITIONFROMLINE, sourceLine+1, 0);
				if (column >= 0) {
					// Get the position in line according to current tab setting
					startSourceLine = SendEditor(SCI_FINDCOLUMN, sourceLine, column); 
				}
				EnsureRangeVisible(startSourceLine, startSourceLine);
				if (props.GetInt("error.select.line")==1) {
					//select whole source source line from column with error
					SetSelection(endSourceline, startSourceLine);
				} else {
					//simply move cursor to line, don't do any selection
					SetSelection(startSourceLine, startSourceLine);
				}
				message.substitute('\t', ' ');
				message.remove("\n");
				props.Set("CurrentMessage", message.c_str());
				UpdateStatusBar(false);
				WindowSetFocus(wEditor);
			}
			delete []cdoc;
			return;
		}
		lookLine += dir;
		if (lookLine < 0)
			lookLine = maxLine - 1;
		else if (lookLine >= maxLine)
			lookLine = 0;
		if (dir == 0)
			return;
	}
}
