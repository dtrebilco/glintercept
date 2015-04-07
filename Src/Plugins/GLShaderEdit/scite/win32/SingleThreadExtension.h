// SciTE - Scintilla based Text Editor
/** @file SingleThreadExtension.h
 ** Extension that wraps another extension so that OnExecute calls are always
 ** seen by the wrapped extension as coming from the initial thread, despite
 ** the fact that SciTE calls it from a worker thread on Windows.
 **/
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SINGLETHREADEXTENSION_H
#define SINGLETHREADEXTENSION_H

#include <windows.h>
#include "Extender.h"

class SingleThreadExtension: public Extension {
public:
	SingleThreadExtension(Extension &ext_) : ext(&ext_), hwndDispatcher(NULL) {}
	virtual ~SingleThreadExtension() { Finalise(); }

	virtual bool Initialise(ExtensionAPI *host_);
	virtual bool Finalise();
	virtual bool Clear();
	virtual bool Load(const char *filename);

	virtual bool InitBuffer(int);
	virtual bool ActivateBuffer(int);
	virtual bool RemoveBuffer(int);

	virtual bool OnOpen(const char *);
	virtual bool OnSwitchFile(const char *);
	virtual bool OnBeforeSave(const char *);
	virtual bool OnSave(const char *);
	virtual bool OnChar(char);
	virtual bool OnExecute(const char *);
	virtual bool OnSavePointReached();
	virtual bool OnSavePointLeft();
	virtual bool OnStyle(unsigned int, int, int, Accessor *);
	virtual bool OnDoubleClick();
	virtual bool OnUpdateUI();
	virtual bool OnMarginClick();
	virtual bool OnMacro(const char *, const char *);
	virtual bool OnUserListSelection(int, const char *);
	virtual bool SendProperty(const char *);

protected:
	Extension *ext;

	// Since the number of extensions requiring this is never likely to be large,
	// each wrapped extension gets its own dispatcher window.
	HWND hwndDispatcher;
	static LRESULT PASCAL WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	// Copying is unsupported.
	SingleThreadExtension(const SingleThreadExtension & copy);
	SingleThreadExtension & operator=(const SingleThreadExtension & copy);
};

#endif
