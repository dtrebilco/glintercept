// SciTE - Scintilla based Text Editor
/** @file UniqueInstance.cxx
 ** Class to ensure a unique instance of the editor, if requested.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>

#include "Platform.h"
#include "SciTEWin.h"

UniqueInstance::UniqueInstance() {
	stw = 0;
	identityMessage = ::RegisterWindowMessage("SciTEInstanceIdentifier");
	mutex = 0;
	bAlreadyRunning = false;
	hOtherWindow = NULL;
}

UniqueInstance::~UniqueInstance() {
	if (mutex) {
		::CloseHandle(mutex);
	}
}

void UniqueInstance::Init(SciTEWin *stw_) {
	stw = stw_;
}

/**
 * Try to create a mutex.
 * If succeed, it is the first/only instance.
 * Otherwise, there is already an instance holding this mutex.
 */
bool UniqueInstance::AcceptToOpenFiles(bool bAccept) {
	bool bError = false;

	stw->openFilesHere = bAccept;
	if (bAccept) {
		// We create a mutex because it is an atomic operation.
		// An operation like EnumWindows is long, so if we use it only, we can fall in a race condition.
		// Note from MSDN: "The system closes the handle automatically when the process terminates.
		// The mutex object is destroyed when its last handle has been closed."
		// If the mutex already exists, the new process get a handle on it, so even if the first
		// process exits, the mutex isn't destroyed, until all SciTE instances exit.
		mutex = ::CreateMutex(NULL, FALSE, mutexName.c_str());
		// The call fails with ERROR_ACCESS_DENIED if the mutex was
		// created in a different user session because of passing
		// NULL for the SECURITY_ATTRIBUTES on mutex creation
		bError = (::GetLastError() == ERROR_ALREADY_EXISTS ||
		          ::GetLastError() == ERROR_ACCESS_DENIED);
	} else {
		::CloseHandle(mutex);
	}
	return !bError;
}

/**
 * Toggle the open files here option.
 * If set, search if another instance have this option set.
 * If found, we ask it to yield this option,
 * so we are the only one to accept files.
 */
void UniqueInstance::ToggleOpenFilesHere() {
	// If the openFilesHere option is set, we unset it and remove the handle.
	// Else, we set the option and try to set the mutex.
	if (!AcceptToOpenFiles(!stw->openFilesHere)) {
		// Cannot set the mutex, search the previous instance holding it
		HWND hOtherWindow = NULL;
		::EnumWindows(reinterpret_cast<WNDENUMPROC>(SearchOtherInstance), reinterpret_cast<LPARAM>(this));
		if (hOtherWindow != NULL) {
			// Found, we indicate it to yield the acceptation of files
			::SendMessage(hOtherWindow, identityMessage, 0,
			              static_cast<LPARAM>(1));
		}
	}
	stw->CheckMenus();
}

/**
 * Manage the received COPYDATA message with a command line from another instance.
 */
LRESULT UniqueInstance::CopyData(COPYDATASTRUCT *pcds) {
	if (pcds) {
		if (stw->props.GetInt("minimize.to.tray")) {
			stw->RestoreFromTray();
		}
		const char *text = static_cast<char *>(pcds->lpData);
		if (text && strlen(text) > 0) {
			SString args = stw->ProcessArgs(text);
			stw->ProcessCommandLine(args, 0);
			stw->ProcessCommandLine(args, 1);
		}
		::FlashWindow(stw->MainHWND(), FALSE);
	}
	return TRUE;
}

/**
 * If the given message is the identity message and we hold the
 * open files here option, we answer the message (so other instances know
 * we are the one holding the option).
 * If the message ask to yield this option, we do it nicely...
 */
LRESULT UniqueInstance::CheckMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == identityMessage) {
		if (stw->openFilesHere || wParam != 0) {
			// We answer only if the menu item is checked to accept files,
			// or if the caller force answering by setting wParam to non null
			// which can be used to find all instances (not used yet).
			if (stw->openFilesHere && lParam != 0) {
				// An instance indicates it takes control of the Open Files Here
				// feature, so this one no longer accept them.
				AcceptToOpenFiles(false);
				stw->CheckMenus();	// Update the checkmark
			}
			return identityMessage;
		}
	}
	return 0;
}

/**
 * To be called only if check.if.already.open option is set to 1.
 * Create the mutex name, try to set the mutex.
 * If failed, renounce to the open files here option.
 */
void UniqueInstance::CheckOtherInstance() {
	// Use the method explained by Joseph M. Newcomer to avoid multiple instances of an application:
	// http://www.codeproject.com/cpp/avoidmultinstance.asp
	// I limit instances by desktop, it seems to make sense with a GUI application...
	mutexName = "SciTE-UniqueInstanceMutex-";	// I doubt I really need a GUID here...
	HDESK desktop = ::GetThreadDesktop(::GetCurrentThreadId());
	DWORD len = 0;
	// Query the needed size for the buffer
	BOOL result = ::GetUserObjectInformation(desktop, UOI_NAME, NULL, 0, &len);
	if (result == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		// WinNT / Win2000
		char *info = new char[len];
		::GetUserObjectInformation(desktop, UOI_NAME, info, len, &len);
		mutexName += info;
		delete []info;
	} else {
		// Win9x: no multiple desktop, GetUserObjectInformation can be called
		// but is bogus...
		mutexName += "Win9x";
	}
	// Try to set the mutex. If return false, it failed, there is already another instance.
	bAlreadyRunning = !AcceptToOpenFiles(true);
	if (bAlreadyRunning) {
		// Don't answer to requests from other starting instances
		stw->openFilesHere = false;
	}
}

/**
 * If we know there is another instance with open files here option,
 * we search it by enumerating windows.
 * @return true if found.
 */
bool UniqueInstance::FindOtherInstance() {
	if (bAlreadyRunning && identityMessage != 0) {
		::EnumWindows(reinterpret_cast<WNDENUMPROC>(SearchOtherInstance), reinterpret_cast<LPARAM>(this));
		if (hOtherWindow) {
			return true;
		}
	}
	return false;
}

/**
 * Send the COPYDATA messages to transmit the command line to
 * the instance holding the open files here option.
 * After that, the current instance will exit.
 */
void UniqueInstance::SendCommands(const char *cmdLine) {
	// On Win2k, windows can't get focus by themselves,
	// so it is the responsability of the new process to bring the window
	// to foreground.
	// Put the other SciTE uniconized and to forefront.
	if (::IsIconic(hOtherWindow)) {
		::ShowWindow(hOtherWindow, SW_RESTORE);
	}
	::SetForegroundWindow(hOtherWindow);

	COPYDATASTRUCT cds;
	cds.dwData = 0;
	// Send 2 messages - first the CWD, so paths relative to
	// the new instance can be resolved in the old instance,
	// then the real command line.
	// (Restoring the cwd could be done,
	// but keeping it to the last file opened can also
	// be useful)
	TCHAR cwdCmd[MAX_PATH + 7]; // 7 for "-cwd:" and 2x'"'
	strcpy(cwdCmd, "\"-cwd:");
	FilePath cwd = FilePath::GetWorkingDirectory();
	strncpy(cwdCmd + strlen(cwdCmd), cwd.AsInternal(), MAX_PATH);
	strcat(cwdCmd, "\"");
	// Defeat the "\" mangling - convert "\" to "/"
	for (char *temp = cwdCmd; *temp; temp++) {
		if (*temp == '\\') {
			*temp = '/';
		}
	}
	cds.cbData = static_cast<DWORD>(strlen(cwdCmd) + 1);
	cds.lpData = static_cast<void *>(cwdCmd);
	::SendMessage(hOtherWindow, WM_COPYDATA, 0,
	              reinterpret_cast<LPARAM>(&cds));
	// Now the command line itself.
	cds.cbData = static_cast<DWORD>(strlen(cmdLine) + 1);
	cds.lpData = static_cast<void *>(const_cast<char *>(cmdLine));
	::SendMessage(hOtherWindow, WM_COPYDATA, 0,
	              reinterpret_cast<LPARAM>(&cds));
}

/**
 * Function called by EnumWindows.
 * @a hWnd is the handle to the currently enumerated window.
 * @a lParam is seen as a pointer to the current UniqueInstance
 * so it can be used to access all members.
 * @return FALSE if found, to stop EnumWindows.
 */
BOOL CALLBACK UniqueInstance::SearchOtherInstance(HWND hWnd, LPARAM lParam) {
	BOOL bResult = TRUE;
	DWORD result;

	UniqueInstance *ui = reinterpret_cast<UniqueInstance *>(lParam);

	// First, avoid to send a message to ourself
	if (hWnd != reinterpret_cast<HWND>(ui->stw->MainHWND())) {
		// Send a message to the given window, to see if it will answer with
		// the same message. If it does, it is a Gui window with
		// openFilesHere set.
		// We use a timeout to avoid being blocked by hung processes.
		LRESULT found = ::SendMessageTimeout(hWnd,
		                                     ui->identityMessage, 0, 0,
		                                     SMTO_BLOCK | SMTO_ABORTIFHUNG, 200, &result);
		if (found != 0 && result == static_cast<DWORD>(ui->identityMessage)) {
			// Another Gui window found!
			// We memorise its window handle
			ui->hOtherWindow = hWnd;
			// We stop the EnumWindows
			bResult = FALSE;
		}
	}
	return bResult;
}
