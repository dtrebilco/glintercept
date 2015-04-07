// SciTE - Scintilla based Text Editor
/** @file SingleThreadExtension.cxx
 ** Extension that wraps another extension so that OnExecute calls
 ** are always funneled to the initial thread.
 **/
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.


// For the single case of LuaExtension where this is needed at present,
// it might have been simpler (well, a little less tedious and very
// slightly more efficient) to make this inherit from LuaExtension, and
// only override Initialise, Finalise, and OnExecute.  Or to do it with
// an #ifdef in LuaExtension.  But I did it this way so that other
// extensions can leverage it without needing to do anything special.

#include "SingleThreadExtension.h"

// Is it true that only OnExecute needs to be protected / serialized?
// Lua does not support macros, but does OnMacro also need it?  Others?

// For OnExecute and any other messages that needed to be dispatched,
//   WPARAM will point to the wrapped extension
//   LPARAM will hold the argument(s)
//   Return value is 0 for false (continue), 1 for true (handled / stop)

enum {
	STE_WM_ONEXECUTE = 2001
};

LRESULT PASCAL SingleThreadExtension::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == STE_WM_ONEXECUTE) {
		Extension *extension = reinterpret_cast<Extension *>(wParam);
		return extension->OnExecute(reinterpret_cast<char *>(lParam));
	}

	WNDPROC lpPrevWndProc = reinterpret_cast<WNDPROC>(GetWindowLong(hwnd, GWL_USERDATA));
	if (lpPrevWndProc)
		return ::CallWindowProc(lpPrevWndProc, hwnd, uMsg, wParam, lParam);

	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool SingleThreadExtension::Initialise(ExtensionAPI *host_) {
	hwndDispatcher = CreateWindow(
		"STATIC", "SciTE_SingleThreadExtension_Dispatcher",
		0, 0, 0, 0, 0, 0, 0, GetModuleHandle(NULL), 0
	);

	LONG subclassedProc = SetWindowLong(hwndDispatcher, GWL_WNDPROC, reinterpret_cast<LONG>(WndProc));
	SetWindowLong(hwndDispatcher, GWL_USERDATA, subclassedProc);

	return ext->Initialise(host_);
}

bool SingleThreadExtension::Finalise() {
	ext->Finalise();

	if (hwndDispatcher) {
		DestroyWindow(hwndDispatcher);
		hwndDispatcher = NULL;
	}

	return false;
}

bool SingleThreadExtension::Clear() {
	return ext->Clear();
}

bool SingleThreadExtension::Load(const char *filename) {
	return ext->Load(filename);
}

bool SingleThreadExtension::InitBuffer(int index) {
	return ext->InitBuffer(index);
}

bool SingleThreadExtension::ActivateBuffer(int index) {
	return ext->ActivateBuffer(index);
}

bool SingleThreadExtension::RemoveBuffer(int index) {
	return ext->RemoveBuffer(index);
}

bool SingleThreadExtension::OnOpen(const char *filename) {
	return ext->OnOpen(filename);
}

bool SingleThreadExtension::OnSwitchFile(const char *filename) {
	return ext->OnSwitchFile(filename);
}

bool SingleThreadExtension::OnBeforeSave(const char *filename) {
	return ext->OnBeforeSave(filename);
}

bool SingleThreadExtension::OnSave(const char *filename) {
	return ext->OnSave(filename);
}

bool SingleThreadExtension::OnChar(char c) {
	return ext->OnChar(c);
}

bool SingleThreadExtension::OnExecute(const char *cmd) {
	return (SendMessage(hwndDispatcher, STE_WM_ONEXECUTE, reinterpret_cast<WPARAM>(ext), reinterpret_cast<LPARAM>(cmd)) != 0);
}

bool SingleThreadExtension::OnSavePointReached() {
	return ext->OnSavePointReached();
}

bool SingleThreadExtension::OnSavePointLeft() {
	return ext->OnSavePointLeft();
}

bool SingleThreadExtension::OnStyle(unsigned int p, int q, int r, Accessor *s) {
	return ext->OnStyle(p,q,r,s);
}

bool SingleThreadExtension::OnDoubleClick() {
	return ext->OnDoubleClick();
}

bool SingleThreadExtension::OnUpdateUI() {
	return ext->OnUpdateUI();
}

bool SingleThreadExtension::OnMarginClick() {
	return ext->OnMarginClick();
}

bool SingleThreadExtension::OnMacro(const char *p, const char *q) {
	return ext->OnMacro(p,q);
}

bool SingleThreadExtension::OnUserListSelection(int listType, const char *selection) {
	return ext->OnUserListSelection(listType, selection);
}

bool SingleThreadExtension::SendProperty(const char *prop) {
	return ext->SendProperty(prop);
}
