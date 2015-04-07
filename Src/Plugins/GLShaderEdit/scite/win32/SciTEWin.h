// SciTE - Scintilla based Text Editor
/** @file SciTEWin.h
 ** Header of main code for the Windows version of the editor.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SCITE_WIN_H
#define SCITE_WIN_H


#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>


#ifdef __MINGW_H
#define _WIN32_IE	0x0400
#endif

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
#include <richedit.h>

#include "Platform.h"

#include <io.h>
#include <process.h>
#include <mmsystem.h>
#include <commctrl.h>
#ifdef _MSC_VER
#include <direct.h>
#endif
#ifdef __BORLANDC__
#include <dir.h>
#endif
#ifdef __DMC__
#include <dir.h>
#endif

#include "SciTE.h"
#include "PropSet.h"
#include "Accessor.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "Extender.h"
#include "FilePath.h"
#include "SciTEBase.h"
#include "SciTEKeys.h"
#include "UniqueInstance.h"

const int SCITE_TRAY = WM_APP + 0;

/** Windows specific stuff.
 **/
class SciTEWin : public SciTEBase {

protected:

	int cmdShow;
	static HINSTANCE hInstance;
	static char *className;
	static char *classNameInternal;
	static SciTEWin *app;
	WINDOWPLACEMENT winPlace;
	RECT rcWorkArea;
	FINDREPLACE fr;
	char openWhat[200];
	bool modalParameters;
	int filterDefault;
	bool staticBuild;
	int menuSource;

	HANDLE hWriteSubProcess;
	DWORD subProcessGroupId;

	HACCEL hAccTable;

	PRectangle pagesetupMargin;
	HGLOBAL hDevMode;
	HGLOBAL hDevNames;

	UniqueInstance uniqueInstance;

	/// HTMLHelp module
	HMODULE hHH;
	/// Multimedia (sound) module
	HMODULE hMM;

	// Tab Bar
	TCITEM tie;
	HFONT fontTabs;

	/// Preserve focus during deactivation
	HWND wFocus;

	Window wFindInFiles;

	virtual void SizeContentWindows();
	virtual void SizeSubWindows();

	virtual void SetMenuItem(int menuNumber, int position, int itemID,
	                         const char *text, const char *mnemonic = 0);
	virtual void DestroyMenuItem(int menuNumber, int itemID);
	virtual void CheckAMenuItem(int wIDCheckItem, bool val);
	virtual void EnableAMenuItem(int wIDCheckItem, bool val);
	virtual void CheckMenus();

	void LocaliseAccelerators();
	SString LocaliseAccelerator(const char *Accelerator, int cmd);
	void LocaliseMenu(HMENU hmenu);
	void LocaliseMenus();
	void LocaliseControl(HWND w);
	void LocaliseDialog(HWND wDialog);

	int DoDialog(HINSTANCE hInst, const char *resName, HWND hWnd, DLGPROC lpProc);
	virtual bool OpenDialog(FilePath directory, const char *filter);
	FilePath ChooseSaveName(FilePath directory, const char *title, const char *filter=0, const char *ext=0);
	virtual bool SaveAsDialog();
	virtual void SaveACopy();
	virtual void SaveAsHTML();
	virtual void SaveAsRTF();
	virtual void SaveAsPDF();
	virtual void SaveAsTEX();
	virtual void SaveAsXML();
	virtual void LoadSessionDialog();
	virtual void SaveSessionDialog();
	virtual bool PreOpenCheck(const char *file);
	virtual bool IsStdinBlocked();

	/// Print the current buffer.
	virtual void Print(bool showDialog);
	/// Handle default print setup values and ask the user its preferences.
	virtual void PrintSetup();

	BOOL HandleReplaceCommand(int cmd);

	virtual int WindowMessageBox(Window &w, const SString &msg, int style);
	virtual void AboutDialog();
	void DropFiles(HDROP hdrop);
	void MinimizeToTray();
	void RestoreFromTray();
	SString ProcessArgs(const char *cmdLine);
	virtual void QuitProgram();

	virtual FilePath GetDefaultDirectory();
	virtual FilePath GetSciteDefaultHome();
	virtual FilePath GetSciteUserHome();

	virtual void SetFileProperties(PropSet &ps);
	virtual void SetStatusBarText(const char *s);

	/// Warn the user, by means defined in its properties.
	virtual void WarnUser(int warnID);

	virtual void Notify(SCNotification *notification);
	virtual void ShowToolBar();
	virtual void ShowTabBar();
	virtual void ShowStatusBar();
	void ExecuteHelp(const char *cmd);
	void ExecuteOtherHelp(const char *cmd);
	void CopyAsRTF();
	void FullScreenToggle();
	void Command(WPARAM wParam, LPARAM lParam);
	HWND MainHWND();

	BOOL FindMessage(HWND hDlg, UINT message, WPARAM wParam);
	static BOOL CALLBACK FindDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL ReplaceMessage(HWND hDlg, UINT message, WPARAM wParam);
	static BOOL CALLBACK ReplaceDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void PerformGrep();
	void FillCombos(HWND hDlg);
	BOOL GrepMessage(HWND hDlg, UINT message, WPARAM wParam);
	static BOOL CALLBACK GrepDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL IncrementFindMessage(HWND hDlg, UINT message, WPARAM wParam);
	static BOOL CALLBACK FindIncrementDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void FindIncrement();
	virtual void Find();
	virtual void FindInFiles();
	virtual void Replace();
	virtual void FindReplace(bool replace);
	virtual void DestroyFindReplace();

	BOOL GoLineMessage(HWND hDlg, UINT message, WPARAM wParam);
	static BOOL CALLBACK GoLineDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void GoLineDialog();

	BOOL AbbrevMessage(HWND hDlg, UINT message, WPARAM wParam);
	static BOOL CALLBACK AbbrevDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool AbbrevDialog();

	BOOL TabSizeMessage(HWND hDlg, UINT message, WPARAM wParam);
	static BOOL CALLBACK TabSizeDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void TabSizeDialog();

	void ParamGrab();
	virtual bool ParametersDialog(bool modal);
	BOOL ParametersMessage(HWND hDlg, UINT message, WPARAM wParam);
	static BOOL CALLBACK ParametersDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	BOOL AboutMessage(HWND hDlg, UINT message, WPARAM wParam);
	static BOOL CALLBACK AboutDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void AboutDialogWithBuild(int staticBuild);

	void MakeAccelerator(SString sKey, ACCEL &Accel);

#ifdef GLI_CHANGES

  virtual void CompileGLIShader();
  virtual void ToggleBreakpoint(unsigned int line);
  
#endif //GLI_CHANGES


public:

	SciTEWin(Extension *ext = 0);
	~SciTEWin();

	bool ModelessHandler(MSG *pmsg);

	void CreateUI();
	/// Management of the command line parameters.
	void Run(const char *cmdLine);
	DWORD ExecuteOne(const Job &jobToRun, bool &seenOutput);
	void ProcessExecute();
	void ShellExec(const SString &cmd, const char *dir);
	virtual void Execute();
	virtual void StopExecute();
	virtual void AddCommand(const SString &cmd, const SString &dir, JobSubsystem jobType, const SString &input = "", int flags=0);

	void Paint(Surface *surfaceWindow, PRectangle rcPaint);
	void Creation();
	LRESULT KeyDown(WPARAM wParam);
	LRESULT KeyUp(WPARAM wParam);
	virtual void AddToPopUp(const char *label, int cmd=0, bool enabled=true);
	LRESULT ContextMenuMessage(UINT iMessage, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(UINT iMessage, WPARAM wParam, LPARAM lParam);
	LRESULT WndProcI(UINT iMessage, WPARAM wParam, LPARAM lParam);

	virtual SString EncodeString(const SString &s);
	virtual SString GetRangeInUIEncoding(Window &wCurrent, int selStart, int selEnd);

	HACCEL GetAcceleratorTable() {
		return hAccTable;
	}

	uptr_t GetInstance();
	static void Register(HINSTANCE hInstance_);
	static LRESULT PASCAL TWndProc(
	    HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	static LRESULT PASCAL IWndProc(
	    HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	friend class UniqueInstance;
};


#endif //SCITE_WIN_H

