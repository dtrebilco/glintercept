// SciTE - Scintilla based Text Editor
// SciTEGTK.cxx - main code for the GTK+ version of the editor
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <assert.h>
#include <time.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "Platform.h"

#include <unistd.h>
#include <glib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

#include "SciTE.h"
#include "PropSet.h"
#include "Accessor.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "ScintillaWidget.h"
#include "Extender.h"
#include "FilePath.h"
#include "SciTEBase.h"
#include "SciTEKeys.h"

#ifndef NO_EXTENSIONS
#include "MultiplexExtension.h"

#ifndef NO_LUA
#include "LuaExtension.h"
#endif

#ifndef NO_FILER
#include "DirectorExtension.h"
#endif

#endif

#include "pixmapsGNOME.h"
#include "SciIcon.h"

#if GTK_MAJOR_VERSION >= 2
#if !PLAT_GTK_WIN32
#define ENCODE_TRANSLATION
#include <iconv.h>
// Since various versions of iconv can not agree on whether the src argument
// is char ** or const char ** provide a templatised adaptor.
template<typename T>
size_t iconv_adaptor(size_t(*f_iconv)(iconv_t, T, size_t *, char **, size_t *),
		iconv_t cd, char** src, size_t *srcleft,
		char **dst, size_t *dstleft) {
	return f_iconv(cd, (T)src, srcleft, dst, dstleft);
}
#endif
#endif

#if GTK_MAJOR_VERSION >= 2 && GTK_MINOR_VERSION >= 4
#define USE_FILE_CHOOSER
#endif

#define MB_ABOUTBOX	0x100000L

const char appName[] = "SciTE";

#ifdef __vms
char g_modulePath[MAX_PATH];
#endif

static GtkWidget *PWidget(Window &w) {
	return reinterpret_cast<GtkWidget *>(w.GetID());
}

class Dialog : public Window {
public:
	Dialog() : dialogCanceled(true) {}
	Dialog &operator=(WindowID id_) {
		id = id_;
		return *this;
	}
	bool ShowModal(GtkWidget *parent = 0) {
		GtkWidget * widgetThis = reinterpret_cast<GtkWidget *>(GetID());
		// Mark it as a modal transient dialog
		gtk_window_set_modal(GTK_WINDOW(widgetThis), TRUE);
		if (parent) {
			gtk_window_set_transient_for(GTK_WINDOW(widgetThis), GTK_WINDOW(parent));
		}
		gtk_signal_connect(GTK_OBJECT(widgetThis), "key_press_event",
		                   GtkSignalFunc(SignalKey), this);
		gtk_signal_connect(GTK_OBJECT(widgetThis), "destroy",
		                   GtkSignalFunc(SignalDestroy), this);
		Show();
		while (Created()) {
			gtk_main_iteration();
		}
		return dialogCanceled;
	}
	void OK() {
		dialogCanceled = false;
		Destroy();
	}
	void Cancel() {
		dialogCanceled = true;
		Destroy();
	}
	static void SignalCancel(GtkWidget *, Dialog *d) {
		if (d) {
			d->Cancel();
		}
	}
private:
	bool dialogCanceled;
	static void SignalDestroy(GtkWidget *, Dialog *d) {
		if (d) {
			d->id = 0;
		}
	}
	static gint SignalKey(GtkWidget *w, GdkEventKey *event, Dialog *d) {
		if (event->keyval == GDK_Escape) {
			gtk_signal_emit_stop_by_name(GTK_OBJECT(w), "key_press_event");
			d->Cancel();
		}
		return FALSE;
	}
};

// Field added to GTK+ 1.x ItemFactoryEntry for 2.x  so have a struct that is the same as 1.x
struct SciTEItemFactoryEntry {
	char *path;
	char *accelerator;
	GtkItemFactoryCallback callback;
	unsigned int callback_action;
	char *item_type;
};

long SciTEKeys::ParseKeyCode(const char *mnemonic) {
	int modsInKey = 0;
	int keyval = -1;

	if (mnemonic && *mnemonic) {
		SString sKey = mnemonic;

		if (sKey.contains("Ctrl+")) {
			modsInKey |= GDK_CONTROL_MASK;
			sKey.remove("Ctrl+");
		}
		if (sKey.contains("Shift+")) {
			modsInKey |= GDK_SHIFT_MASK;
			sKey.remove("Shift+");
		}
		if (sKey.contains("Alt+")) {
			modsInKey |= GDK_MOD1_MASK;
			sKey.remove("Alt+");
		}

		if (sKey.length() == 1) {
			if (modsInKey & GDK_CONTROL_MASK && !(modsInKey & GDK_SHIFT_MASK))
				sKey.lowercase();
			keyval = sKey[0];
		} else if ((sKey.length() > 1)) {
			if ((sKey[0] == 'F') && (isdigit(sKey[1]))) {
				sKey.remove("F");
				int fkeyNum = sKey.value();
				if (fkeyNum >= 1 && fkeyNum <= 12)
					keyval = fkeyNum - 1 + GDK_F1;
			} else {
				if (sKey == "Left") {
					keyval = GDK_Left;
				} else if (sKey == "Right") {
					keyval = GDK_Right;
				} else if (sKey == "Up") {
					keyval = GDK_Up;
				} else if (sKey == "Down") {
					keyval = GDK_Down;
				} else if (sKey == "Insert") {
					keyval = GDK_Escape;
				} else if (sKey == "End") {
					keyval = GDK_End;
				} else if (sKey == "Home") {
					keyval = GDK_Home;
				} else if (sKey == "Enter") {
					keyval = GDK_Return;
				} else if (sKey == "Space") {
					keyval = GDK_space;
				} else if (sKey == "KeypadPlus") {
					keyval = GDK_KP_Add;
				} else if (sKey == "KeypadMinus") {
					keyval = GDK_KP_Subtract;
				} else if (sKey == "Escape") {
					keyval = GDK_Escape;
				} else if (sKey == "Delete") {
					keyval = GDK_Delete;
				} else if (sKey == "PageUp") {
					keyval = GDK_Page_Up;
				} else if (sKey == "PageDown") {
					keyval = GDK_Page_Down;
				} else if (sKey == "Slash") {
					keyval = GDK_slash;
				} else if (sKey == "Question") {
					keyval = GDK_question;
				} else if (sKey == "Equal") {
					keyval = GDK_equal;
				}
			}
		}
	}

	return (keyval > 0) ? (keyval | (modsInKey<<16)) : 0;
}

bool SciTEKeys::MatchKeyCode(long parsedKeyCode, int keyval, int modifiers) {
	return parsedKeyCode && !(0xFFFF0000 & (keyval | modifiers)) && (parsedKeyCode == (keyval | (modifiers<<16)));
}

class SciTEGTK : public SciTEBase {

protected:

	Point ptOld;
	GdkGC *xor_gc;

	guint sbContextID;
	Window wToolBarBox;
	int toolbarDetachable;
	int menuSource;

	// Control of sub process
	FilePath sciteExecutable;
	int icmd;
	int originalEnd;
	int fdFIFO;
	int pidShell;
	bool triedKill;
	int exitStatus;
	guint pollID;
	char resultsFile[MAX_PATH];
	int inputHandle;
	ElapsedTime commandTime;

	// Command Pipe variables
	int  pipeFD;
	char pipeName[MAX_PATH];

	enum FileFormat { sfSource, sfCopy, sfHTML, sfRTF, sfPDF, sfTEX, sfXML } saveFormat;
	Dialog dlgFileSelector;
	Dialog dlgFindInFiles;
	GtkWidget *comboFiles;
	Dialog dlgGoto;
	bool paramDialogCanceled;
	GtkWidget *wIncrementPanel;
	Dialog dlgFindIncrement;
	GtkWidget *IncSearchEntry;

	GtkWidget *gotoEntry;
	GtkWidget *toggleWord;
	GtkWidget *toggleCase;
	GtkWidget *toggleRegExp;
	GtkWidget *toggleWrap;
	GtkWidget *toggleUnSlash;
	GtkWidget *toggleReverse;
	GtkWidget *toggleRec;
	GtkWidget *comboFind;
	GtkWidget *comboFindInFiles;
	GtkWidget *comboDir;
	GtkWidget *comboReplace;
	GtkWidget *entryParam[maxParam];
	GtkWidget *btnCompile;
	GtkWidget *btnBuild;
	GtkWidget *btnStop;
	GtkItemFactory *itemFactory;
	GtkAccelGroup *accelGroup;

	gint	fileSelectorWidth;
	gint	fileSelectorHeight;

	// Fullscreen handling
	GdkRectangle saved;

	GtkWidget *AddMBButton(GtkWidget *dialog, const char *label,
	                       int val, GtkAccelGroup *accel_group, bool isDefault = false);
	void SetWindowName();
	void ShowFileInStatus();
	void SetIcon();

	virtual void ReadLocalisation();
	virtual void ReadPropertiesInitial();
	virtual void ReadProperties();

	virtual void SizeContentWindows();
	virtual void SizeSubWindows();

	virtual void SetMenuItem(int menuNumber, int position, int itemID,
	                         const char *text, const char *mnemonic = 0);
	virtual void DestroyMenuItem(int menuNumber, int itemID);
	virtual void CheckAMenuItem(int wIDCheckItem, bool val);
	virtual void EnableAMenuItem(int wIDCheckItem, bool val);
	virtual void CheckMenus();
	virtual void AddToPopUp(const char *label, int cmd = 0, bool enabled = true);
	virtual void ExecuteNext();

	virtual void OpenUriList(const char *list);
	virtual bool OpenDialog(FilePath directory, const char *filter);
	void HandleSaveAs(const char *savePath);
	bool SaveAsXXX(FileFormat fmt, const char *title);
	virtual bool SaveAsDialog();
	virtual void SaveACopy();
	virtual void SaveAsHTML();
	virtual void SaveAsRTF();
	virtual void SaveAsPDF();
	virtual void SaveAsTEX();
	virtual void SaveAsXML();
	virtual void LoadSessionDialog();
	virtual void SaveSessionDialog();

	virtual void Print(bool);
	virtual void PrintSetup();

	virtual SString GetRangeInUIEncoding(Window &wCurrent, int selStart, int selEnd);

	virtual int WindowMessageBox(Window &w, const SString &msg, int style);
	virtual void AboutDialog();
	virtual void QuitProgram();

	virtual SString EncodeString(const SString &s);
	void FindReplaceGrabFields();
	void HandleFindReplace();
	virtual void Find();
	void TranslatedSetTitle(GtkWindow *w, const char *original);
	GtkWidget *TranslatedLabel(const char *original);
	GtkWidget *TranslatedCommand(const char *original, GtkAccelGroup *accel_group,
		GtkSignalFunc func, gpointer data, GdkModifierType accelMask=GDK_MOD1_MASK);
	GtkWidget *TranslatedToggle(const char *original, GtkAccelGroup *accel_group, bool active);
	virtual void FindIncrement();
	virtual void FindInFiles();
	virtual void Replace();
	virtual void FindReplace(bool replace);
	virtual void DestroyFindReplace();
	virtual void GoLineDialog();
	virtual bool AbbrevDialog();
	virtual void TabSizeDialog();
	virtual bool ParametersDialog(bool modal);

	virtual FilePath GetDefaultDirectory();
	virtual FilePath GetSciteDefaultHome();
	virtual FilePath GetSciteUserHome();

	virtual void SetStatusBarText(const char *s);
	virtual void SetFileProperties(PropSet &ps);
	virtual void UpdateStatusBar(bool bUpdateSlowData);

	virtual void Notify(SCNotification *notification);
	virtual void ShowToolBar();
	virtual void ShowTabBar();
	virtual void ShowStatusBar();
	void Command(unsigned long wParam, long lParam = 0);
	void ContinueExecute(int fromPoll);

	static void ReadPipe(gpointer data, gint source, GdkInputCondition condition);
	void SendFileName(int sendPipe, const char* filename);
	void CheckForRunningInstance(int argc, char* argv[]);

	// GTK+ Signal Handlers

	static void OpenCancelSignal(GtkWidget *w, SciTEGTK *scitew);
	static gint OpenKeySignal(GtkWidget *w, GdkEventKey *event, SciTEGTK *scitew);
	static void OpenOKSignal(GtkWidget *w, SciTEGTK *scitew);
	static void OpenSessionOKSignal(GtkWidget *w, SciTEGTK *scitew);
	static void SaveSessionOKSignal(GtkWidget *w, SciTEGTK *scitew);
	static void OpenResizeSignal(GtkWidget *w, GtkAllocation *allocation, SciTEGTK *scitew);
	static void SaveAsSignal(GtkWidget *w, SciTEGTK *scitew);

	void FindInFilesCmd();
	static void FindInFilesSignal(GtkWidget */*w*/, SciTEGTK *scitew) {
		scitew->FindInFilesCmd();
	}
	void FindInFilesDotDot();
	static void DotDotSignal(GtkWidget */*w*/, SciTEGTK *scitew) {
		scitew->FindInFilesDotDot();
	}

	static void GotoSignal(GtkWidget *w, SciTEGTK *scitew);
	static void FindIncrementSignal(GtkWidget *, SciTEGTK *scitew);
	static void FindIncrementCompleteSignal(GtkWidget *, SciTEGTK *scitew);
	static void FindIncrementEscapeSignal(GtkWidget *w, GdkEventKey *event, SciTEGTK *scitew);

	static void FRCancelSignal(GtkWidget *w, SciTEGTK *scitew);
	static gint FRKeySignal(GtkWidget *w, GdkEventKey *event, SciTEGTK *scitew);
	static void FRFindSignal(GtkWidget *w, SciTEGTK *scitew);
	static void FRReplaceSignal(GtkWidget *w, SciTEGTK *scitew);
	static void FRReplaceAllSignal(GtkWidget *w, SciTEGTK *scitew);
	static void FRReplaceInSelectionSignal(GtkWidget *w, SciTEGTK *scitew);

	virtual void ParamGrab();
	static gint ParamKeySignal(GtkWidget *w, GdkEventKey *event, SciTEGTK *scitew);
	static void ParamCancelSignal(GtkWidget *w, SciTEGTK *scitew);
	static void ParamSignal(GtkWidget *w, SciTEGTK *scitew);

	static void IOSignal(SciTEGTK *scitew);
	static gint MoveResize(GtkWidget *widget, GtkAllocation *allocation, SciTEGTK *scitew);
	static gint QuitSignal(GtkWidget *w, GdkEventAny *e, SciTEGTK *scitew);
	static void ButtonSignal(GtkWidget *widget, gpointer data);
	static void MenuSignal(SciTEGTK *scitew, guint action, GtkWidget *w);
	static void CommandSignal(GtkWidget *w, gint wParam, gpointer lParam, SciTEGTK *scitew);
	static void NotifySignal(GtkWidget *w, gint wParam, gpointer lParam, SciTEGTK *scitew);
	static gint KeyPress(GtkWidget *widget, GdkEventKey *event, SciTEGTK *scitew);
	gint Key(GdkEventKey *event);
	static gint MousePress(GtkWidget *widget, GdkEventButton *event, SciTEGTK *scitew);
	gint Mouse(GdkEventButton *event);

	void DividerXOR(Point pt);
	static gint DividerExpose(GtkWidget *widget, GdkEventExpose *ose, SciTEGTK *scitew);
	static gint DividerMotion(GtkWidget *widget, GdkEventMotion *event, SciTEGTK *scitew);
	static gint DividerPress(GtkWidget *widget, GdkEventButton *event, SciTEGTK *scitew);
	static gint DividerRelease(GtkWidget *widget, GdkEventButton *event, SciTEGTK *scitew);
	static void DragDataReceived(GtkWidget *widget, GdkDragContext *context,
	                             gint x, gint y, GtkSelectionData *selection_data, guint info, guint time, SciTEGTK *scitew);
	gint TabBarRelease(GtkNotebook *notebook, GdkEventButton *event);
	static gint TabBarReleaseSignal(GtkNotebook *notebook, GdkEventButton *event, SciTEGTK *scitew) {
		return scitew->TabBarRelease(notebook, event);
	}

#if GTK_MAJOR_VERSION >= 2
	// This is used to create the pixmaps used in the interface.
	GdkPixbuf *CreatePixbuf(const char *filename);
#endif
#ifdef USE_FILE_CHOOSER
	// Callback function to show hidden files in filechooser
	static void toggle_hidden_cb(GtkToggleButton *toggle, gpointer data);
#endif	

  //GLI Added method
  virtual void CompileGLIShader();

public:

	// TODO: get rid of this - use callback argument to find SciTEGTK
	static SciTEGTK *instance;

	SciTEGTK(Extension *ext = 0);
	~SciTEGTK();

	void WarnUser(int warnID);
	GtkWidget *pixmap_new(GtkWidget *window, gchar **xpm);
	GtkWidget *AddToolButton(const char *text, int cmd, GtkWidget *toolbar_icon);
	void AddToolBar();
	SString SciTEGTK::TranslatePath(const char *path);
	void CreateTranslatedMenu(int n, SciTEItemFactoryEntry items[],
	                          int nRepeats = 0, const char *prefix = 0, int startNum = 0,
	                          int startID = 0, const char *radioStart = 0);
	void CreateMenu();
	void CreateUI();
	void Run(int argc, char *argv[]);
	void ProcessExecute();
	virtual void Execute();
	virtual void StopExecute();
	static int PollTool(SciTEGTK *scitew);
	static void ChildSignal(int);
};

SciTEGTK *SciTEGTK::instance;

SciTEGTK::SciTEGTK(Extension *ext) : SciTEBase(ext) {
	menuSource = 0;
	// Control of sub process
	icmd = 0;
	originalEnd = 0;
	fdFIFO = 0;
	pidShell = 0;
	triedKill = false;
	exitStatus = 0;
	pollID = 0;
	sprintf(resultsFile, "/tmp/SciTE%x.results",
	        static_cast<int>(getpid()));
	inputHandle = 0;

	pipeFD = -1;

	propsEmbed.Set("PLAT_GTK", "1");

	ReadGlobalPropFile();
	ReadAbbrevPropFile();

	ptOld = Point(0, 0);
	xor_gc = 0;
	saveFormat = sfSource;
	comboFiles = 0;
	paramDialogCanceled = true;
	gotoEntry = 0;
	IncSearchEntry = 0;
	toggleWord = 0;
	toggleCase = 0;
	toggleRegExp = 0;
	toggleWrap = 0;
	toggleUnSlash = 0;
	toggleReverse = 0;
	comboFind = 0;
	comboFindInFiles = 0;
	comboReplace = 0;
	btnCompile = 0;
	btnBuild = 0;
	btnStop = 0;
	itemFactory = 0;

	fileSelectorWidth = 580;
	fileSelectorHeight = 480;

	// Fullscreen handling
	fullScreen = false;

	instance = this;
}

SciTEGTK::~SciTEGTK() {}

static void destroyDialog(GtkWidget *, gpointer *window) {
	if (window) {
		Window *pwin = reinterpret_cast<Window *>(window);
		*(pwin) = 0;
	}
}

void SciTEGTK::WarnUser(int) {}

static GtkWidget *messageBoxDialog = 0;
static long messageBoxResult = 0;

static gint messageBoxKey(GtkWidget *w, GdkEventKey *event, gpointer p) {
	if (event->keyval == GDK_Escape) {
		gtk_signal_emit_stop_by_name(GTK_OBJECT(w), "key_press_event");
		gtk_widget_destroy(GTK_WIDGET(w));
		messageBoxDialog = 0;
		messageBoxResult = reinterpret_cast<long>(p);
	}
	return FALSE;
}

static void messageBoxDestroy(GtkWidget *, gpointer *) {
	messageBoxDialog = 0;
}

static void messageBoxOK(GtkWidget *, gpointer p) {
	gtk_widget_destroy(GTK_WIDGET(messageBoxDialog));
	messageBoxDialog = 0;
	messageBoxResult = reinterpret_cast<long>(p);
}

static GtkWidget *MakeToggle(const char *text, GtkAccelGroup *accel_group, bool active) {
	GtkWidget *toggle = gtk_check_button_new_with_label("");
	guint key = gtk_label_parse_uline(GTK_LABEL(GTK_BIN(toggle)->child), text);
	gtk_widget_add_accelerator(toggle, "clicked", accel_group,
	                           key, GDK_MOD1_MASK, (GtkAccelFlags)0);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), active);
	return toggle;
}

static GtkWidget *MakeCommand(const char *text, GtkAccelGroup *accel_group,
		GtkSignalFunc func, gpointer data, GdkModifierType accelMask) {
	GtkWidget *command = gtk_button_new_with_label("");
	GTK_WIDGET_SET_FLAGS(command, GTK_CAN_DEFAULT);
	guint key = gtk_label_parse_uline(GTK_LABEL(GTK_BIN(command)->child), text);
	gtk_widget_add_accelerator(command, "clicked", accel_group,
	                           key, accelMask, (GtkAccelFlags)0);
	gtk_signal_connect(GTK_OBJECT(command), "clicked", func, data);
	return command;
}

GtkWidget *SciTEGTK::AddMBButton(GtkWidget *dialog, const char *label,
                                 int val, GtkAccelGroup *accel_group, bool isDefault) {
	GtkWidget *button = TranslatedCommand(label, accel_group,
		GtkSignalFunc(messageBoxOK), reinterpret_cast<gpointer>(val),
		GdkModifierType(0));
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),
	                   button, TRUE, TRUE, 0);
	if (isDefault) {
		gtk_widget_grab_default(GTK_WIDGET(button));
	}
	gtk_widget_show(button);
	return button;
}

#if GTK_MAJOR_VERSION >= 2
// This is an internally used function to create pixmaps.
GdkPixbuf *SciTEGTK::CreatePixbuf(const char *filename) {
	char path[MAX_PATH + 20];
	strncpy(path, PIXMAP_PATH, sizeof(path));
	strcat(path, pathSepString);
	strcat(path, filename);

	GError *error = NULL;
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, &error);
	if (!pixbuf) {
		fprintf(stderr, "Failed to load pixbuf file: %s: %s\n",
			path, error->message);
		g_error_free(error);
	}
	return pixbuf;
}
#endif

FilePath SciTEGTK::GetDefaultDirectory() {
	char *where = getenv("SciTE_HOME");
#ifdef SYSCONF_PATH
	if (!where) {
		where = SYSCONF_PATH;
	}
#else
	if (!where) {
		where = getenv("HOME");
	}
#endif
	if (where) {
#ifdef __vms
		return FilePath(VMSToUnixStyle(where));
#else
		return FilePath(where);
#endif
	}

	return FilePath("");
}

FilePath SciTEGTK::GetSciteDefaultHome() {
	char *where = getenv("SciTE_HOME");
#ifdef __vms
	if (where == NULL) {
		where = g_modulePath;
	}
#endif
#ifdef SYSCONF_PATH
	if (!where) {
		where = SYSCONF_PATH;
	}
#else
	if (!where) {
		where = getenv("HOME");
	}
#endif
	if (where) {
#ifdef __vms
		return FilePath(VMSToUnixStyle(where));
#else
		return FilePath(where);
#endif

	}
	return FilePath("");
}

FilePath SciTEGTK::GetSciteUserHome() {
	char *where = getenv("SciTE_HOME");
	if (!where) {
		where = getenv("HOME");
	}
	return FilePath(where);
}

void SciTEGTK::ShowFileInStatus() {
	char sbText[1000];
	sprintf(sbText, " File: ");
	if (filePath.IsUntitled())
		strcat(sbText, "Untitled");
	else
		strcat(sbText, filePath.AsInternal());
	SetStatusBarText(sbText);
}

void SciTEGTK::SetWindowName() {
	SciTEBase::SetWindowName();
	ShowFileInStatus();
}

void SciTEGTK::SetStatusBarText(const char *s) {
	gtk_statusbar_pop(GTK_STATUSBAR(PWidget(wStatusBar)), sbContextID);
	gtk_statusbar_push(GTK_STATUSBAR(PWidget(wStatusBar)), sbContextID, s);
}

void SciTEGTK::SetFileProperties(PropSet &ps) {
	// Could use Unix standard calls here, someone less lazy than me (PL) should do it.
	ps.Set("FileTime", "");
	ps.Set("FileDate", "");
	ps.Set("FileAttr", "");
	ps.Set("CurrentDate", "");
	ps.Set("CurrentTime", "");
}

void SciTEGTK::UpdateStatusBar(bool bUpdateSlowData) {
	SciTEBase::UpdateStatusBar(bUpdateSlowData);
}

void SciTEGTK::Notify(SCNotification *notification) {
	SciTEBase::Notify(notification);
}

void SciTEGTK::ShowToolBar() {
	if (GTK_TOOLBAR(PWidget(wToolBar))->num_children < 1) {
		AddToolBar();
	}

	if (tbVisible) {
		if (toolbarDetachable == 1) {
			gtk_widget_show(GTK_WIDGET(PWidget(wToolBarBox)));
		} else {
			gtk_widget_show(GTK_WIDGET(PWidget(wToolBar)));
		}
	} else {
		if (toolbarDetachable == 1) {
			gtk_widget_hide(GTK_WIDGET(PWidget(wToolBarBox)));
		} else {
			gtk_widget_hide(GTK_WIDGET(PWidget(wToolBar)));
		}
	}
}

void SciTEGTK::ShowTabBar() {
#if GTK_MAJOR_VERSION >= 2
	if (tabVisible && (!tabHideOne || buffers.length > 1) && buffers.size>1) {
		gtk_widget_show(GTK_WIDGET(PWidget(wTabBar)));
	} else {
		gtk_widget_hide(GTK_WIDGET(PWidget(wTabBar)));
	}
#endif
}

void SciTEGTK::ShowStatusBar() {
	if (sbVisible) {
		gtk_widget_show(GTK_WIDGET(PWidget(wStatusBar)));
	} else {
		gtk_widget_hide(GTK_WIDGET(PWidget(wStatusBar)));
	}
}

void SciTEGTK::Command(unsigned long wParam, long) {
	int cmdID = ControlIDOfCommand(wParam);
	int notifyCode = wParam >> 16;
	switch (cmdID) {

	case IDM_SRCWIN:
		if (notifyCode == SCEN_SETFOCUS) {
			Activate(true);
			CheckMenus();
		} else if (notifyCode == SCEN_KILLFOCUS) {
			Activate(false);
		}
		break;

	case IDM_RUNWIN:
		if (notifyCode == SCEN_SETFOCUS)
			CheckMenus();
		break;

	case IDM_FULLSCREEN:
		fullScreen = !fullScreen;
		if (fullScreen) {
			int screen_x, screen_y;
			int scite_x, scite_y;
			int width, height;
			GdkWindow* parent_w = PWidget(wSciTE)->window;

			gdk_window_get_origin(parent_w, &screen_x, &screen_y);
			gdk_window_get_geometry(parent_w, &scite_x, &scite_y, &width, &height, NULL);

			saved.x = screen_x - scite_x;
			saved.y = screen_y - scite_y;
			saved.width = width;
			saved.height = height;
			gdk_window_move_resize(parent_w, -scite_x, -scite_y, gdk_screen_width() + 1, gdk_screen_height() + 1);
			SizeSubWindows();
		} else {
			GdkWindow* parent_w = PWidget(wSciTE)->window;
			gdk_window_move_resize(parent_w, saved.x, saved.y, saved.width, saved.height);
			SizeSubWindows();
		}
		CheckMenus();
		break;

	default:
		SciTEBase::MenuCommand(cmdID, menuSource);
		menuSource = 0;
	}
	UpdateStatusBar(true);
}

void SciTEGTK::ReadLocalisation() {
	SciTEBase::ReadLocalisation();
#ifdef ENCODE_TRANSLATION
	SString encoding = propsUI.Get("translation.encoding");
	if (encoding.length()) {
		iconv_t iconvh = iconv_open("UTF-8", encoding.c_str());
		char *key = NULL;
		char *val = NULL;
		// Get encoding
		bool more = propsUI.GetFirst(&key, &val);
		while (more) {
			char converted[1000];
			converted[0] = '\0';
			char *pin = val;
			size_t inLeft = strlen(val);
			char *pout = converted;
			size_t outLeft = sizeof(converted);
			size_t conversions = iconv_adaptor(iconv, iconvh, &pin, &inLeft, &pout, &outLeft);
			if (conversions != ((size_t)(-1))) {
				*pout = '\0';
				propsUI.Set(key, converted);
			}
			more = propsUI.GetNext(&key, &val);
		}
		iconv_close(iconvh);
	}
#endif
}

void SciTEGTK::ReadPropertiesInitial() {
	SciTEBase::ReadPropertiesInitial();
	ShowToolBar();
	ShowTabBar();
	ShowStatusBar();
}

void SciTEGTK::ReadProperties() {
	SciTEBase::ReadProperties();
	CheckMenus();

	// Need this here to handle tabbar.hide.one properly
	ShowTabBar();
}

void SciTEGTK::SizeContentWindows() {
	PRectangle rcClient = GetClientRectangle();
#if GTK_MAJOR_VERSION < 2
	int left = 0;
	int top = 0;
#else
	int left = rcClient.left;
	int top = rcClient.top;
#endif
	int w = rcClient.right - rcClient.left;
	int h = rcClient.bottom - rcClient.top;
	heightOutput = NormaliseSplit(heightOutput);
	if (splitVertical) {
		wEditor.SetPosition(PRectangle(left, top, w - heightOutput - heightBar + left, h + top));
		wDivider.SetPosition(PRectangle(w - heightOutput - heightBar + left, top, w - heightOutput + left, h + top));
		wOutput.SetPosition(PRectangle(w - heightOutput + left, top, w + left, h + top));
	} else {
		wEditor.SetPosition(PRectangle(left, top, w + left, h - heightOutput - heightBar + top));
		wDivider.SetPosition(PRectangle(left, h - heightOutput - heightBar + top, w + left, h - heightOutput + top));
		wOutput.SetPosition(PRectangle(left, h - heightOutput + top, w + left, h + top));
	}
}

void SciTEGTK::SizeSubWindows() {
	SizeContentWindows();
}

void SciTEGTK::SetMenuItem(int, int, int itemID, const char *text, const char *mnemonic) {
	DestroyMenuItem(0, itemID);

	// On GTK+ the menuNumber and position are ignored as the menu item already exists and is in the right
	// place so only needs to be shown and have its text set.

	SString itemText(text);
	// Remove accelerator as does not work.
	itemText.remove("&");

	long keycode = 0;
	if (mnemonic && *mnemonic) {
		keycode = SciTEKeys::ParseKeyCode(mnemonic);
		if (keycode) {
			itemText += " ";
			itemText += mnemonic;
		}
		// the keycode could be used to make a custom accelerator table
		// but for now, the menu's item data is used instead for command
		// tools, and for other menu entries it is just discarded.
	}

	// Reorder shift and ctrl indicators for compatibility with other menus
	itemText.substitute("Ctrl+Shift+", "Shift+Ctrl+");

	GtkWidget *item = gtk_item_factory_get_widget_by_action(itemFactory, itemID);
	if (item) {
		GList *al = gtk_container_children(GTK_CONTAINER(item));
		for (unsigned int ii = 0; ii < g_list_length(al); ii++) {
			gpointer d = g_list_nth(al, ii);
			GtkWidget **w = (GtkWidget **)d;
			gtk_label_set_text(GTK_LABEL(*w), itemText.c_str());
			// Have not managed to make accelerator work
			//guint key = gtk_label_parse_uline(GTK_LABEL(*w), itemText);
			//gtk_widget_add_accelerator(*w, "clicked", accelGroup,
			//           key, 0, (GtkAccelFlags)0);
		}
		g_list_free(al);
		gtk_widget_show(item);

		if (itemID >= IDM_TOOLS && itemID < IDM_TOOLS + toolMax) {
			// Stow the keycode for later retrieval.
			// Do this even if 0, in case the menu already existed (e.g. ModifyMenu)
			gtk_object_set_user_data(GTK_OBJECT(item), reinterpret_cast<gpointer>(keycode));
		}
	}
}

void SciTEGTK::DestroyMenuItem(int, int itemID) {
	// On GTK+ menu items are just hidden rather than destroyed as they can not be recreated in the middle of a menu
	// The menuNumber is ignored as all menu items in GTK+ can be found from the root of the menu tree

	if (itemID) {
		GtkWidget *item = gtk_item_factory_get_widget_by_action(itemFactory, itemID);

		if (item) {
			gtk_widget_hide(item);
			gtk_object_set_user_data(GTK_OBJECT(item), 0);
		}
	}
}

void SciTEGTK::CheckAMenuItem(int wIDCheckItem, bool val) {
	GtkWidget *item = gtk_item_factory_get_widget_by_action(itemFactory, wIDCheckItem);
	allowMenuActions = false;
	if (item)
		gtk_check_menu_item_set_state(GTK_CHECK_MENU_ITEM(item), val ? TRUE : FALSE);
	//else
	//	Platform::DebugPrintf("Could not find %x\n", wIDCheckItem);
	allowMenuActions = true;
}

void SciTEGTK::EnableAMenuItem(int wIDCheckItem, bool val) {
	GtkWidget *item = gtk_item_factory_get_widget_by_action(itemFactory, wIDCheckItem);
	if (item) {
		//Platform::DebugPrintf("Set %d to %d\n", wIDCheckItem, val);
		if (GTK_IS_WIDGET(item))
			gtk_widget_set_sensitive(item, val);
	}
}

void SciTEGTK::CheckMenus() {
	SciTEBase::CheckMenus();

	CheckAMenuItem(IDM_EOL_CRLF, SendEditor(SCI_GETEOLMODE) == SC_EOL_CRLF);
	CheckAMenuItem(IDM_EOL_CR, SendEditor(SCI_GETEOLMODE) == SC_EOL_CR);
	CheckAMenuItem(IDM_EOL_LF, SendEditor(SCI_GETEOLMODE) == SC_EOL_LF);

	CheckAMenuItem(IDM_ENCODING_DEFAULT, unicodeMode == uni8Bit);
	CheckAMenuItem(IDM_ENCODING_UCS2BE, unicodeMode == uni16BE);
	CheckAMenuItem(IDM_ENCODING_UCS2LE, unicodeMode == uni16LE);
	CheckAMenuItem(IDM_ENCODING_UTF8, unicodeMode == uniUTF8);
	CheckAMenuItem(IDM_ENCODING_UCOOKIE, unicodeMode == uniCookie);

	CheckAMenuItem(IDM_VIEWSTATUSBAR, sbVisible);
	CheckAMenuItem(IDM_VIEWTABBAR, tabVisible);

	if (btnBuild) {
		gtk_widget_set_sensitive(btnBuild, !executing);
		gtk_widget_set_sensitive(btnCompile, !executing);
		gtk_widget_set_sensitive(btnStop, executing);
	}
}

/**
 * Replace any %xx escapes by their single-character equivalent.
 */
static void unquote(char *s) {
	char *o = s;
	while (*s) {
		if ((*s == '%') && s[1] && s[2]) {
			*o = IntFromHexDigit(s[1]) * 16 + IntFromHexDigit(s[2]);
			s += 2;
		} else {
			*o = *s;
		}
		o++;
		s++;
	}
	*o = '\0';
}

/**
 * Open a list of URIs each terminated by "\r\n".
 * Only "file:" URIs currently understood.
 */
void SciTEGTK::OpenUriList(const char *list) {
	if (list) {
		char *uri = StringDup(list);
		if (uri) {
			char *enduri = strchr(uri, '\r');
			while (enduri) {
				*enduri = '\0';
				if (isprefix(uri, "file:")) {
					uri += strlen("file:");
					if (isprefix(uri, "///")) {
						uri += 2;	// There can be an optional // before the file path that starts with /
					}

					unquote(uri);
					Open(uri);
				} else {
					SString msg = LocaliseMessage("URI '^0' not understood.", uri);
					WindowMessageBox(wSciTE, msg, MB_OK | MB_ICONWARNING);
				}

				uri = enduri + 1;
				if (*uri == '\n')
					uri++;
				enduri = strchr(uri, '\r');
			}
		}
	}
}

bool SciTEGTK::OpenDialog(FilePath directory, const char *filter) {
	directory.SetWorkingDirectory();
	bool canceled = true;
	if (!dlgFileSelector.Created()) {
#ifndef USE_FILE_CHOOSER
		filter=filter;	// Ignore unused argument
		dlgFileSelector = gtk_file_selection_new(LocaliseString("Open File").c_str());
		gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(PWidget(dlgFileSelector))->ok_button),
		                   "clicked", GtkSignalFunc(OpenOKSignal), this);
		gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(PWidget(dlgFileSelector))->cancel_button),
		                   "clicked", GtkSignalFunc(OpenCancelSignal), this);
		gtk_signal_connect(GTK_OBJECT(PWidget(dlgFileSelector)),
		                   "size_allocate", GtkSignalFunc(OpenResizeSignal),
		                   this);
		// Get a bigger open dialog
		gtk_window_set_default_size(GTK_WINDOW(PWidget(dlgFileSelector)),
		                            fileSelectorWidth, fileSelectorHeight);
		canceled = dlgFileSelector.ShowModal(PWidget(wSciTE));
#else
		GtkWidget *dlg = gtk_file_chooser_dialog_new(
					LocaliseString("Open File").c_str(),
				      GTK_WINDOW(wSciTE.GetID()),
				      GTK_FILE_CHOOSER_ACTION_OPEN,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				      NULL);
		gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dlg), TRUE);
		gtk_dialog_set_default_response(GTK_DIALOG(dlg), GTK_RESPONSE_ACCEPT);
		if (props.GetInt("open.dialog.in.file.directory")) {
			gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dlg), 
				filePath.Directory().AsInternal());
		}

		// Add a show hidden files toggle
		GtkWidget *toggle = gtk_check_button_new_with_label(
			LocaliseString("Show hidden files").c_str());
		gtk_widget_show(toggle);
		gtk_file_chooser_set_extra_widget(GTK_FILE_CHOOSER(dlg), toggle);
		g_signal_connect(toggle, "toggled",
			G_CALLBACK(toggle_hidden_cb), GTK_DIALOG(dlg));
		if (props.GetInt("fileselector.show.hidden"))
			g_object_set(GTK_OBJECT(toggle), "active", TRUE, NULL);

		SString openFilter = filter;
		if (openFilter.length()) {
			openFilter.substitute('|', '\0');
			size_t start = 0;
			while (start < openFilter.length()) {
				const char *filterName = openFilter.c_str() + start;
				SString localised = LocaliseString(filterName, false);
				if (localised.length()) {
					openFilter.remove(start, strlen(filterName));
					openFilter.insert(start, localised.c_str());
				}
				GtkFileFilter *filter = gtk_file_filter_new();
				gtk_file_filter_set_name(filter, openFilter.c_str() + start);
				start += strlen(openFilter.c_str() + start) + 1;
				SString oneSet(openFilter.c_str() + start);
				oneSet.substitute(';', '\0');
				size_t item = 0;
				while (item < oneSet.length()) {
					gtk_file_filter_add_pattern(filter, oneSet.c_str() + item);
					item += strlen(oneSet.c_str() + item) + 1;
				}
				gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg), filter);
				start += strlen(openFilter.c_str() + start) + 1;
			}
		}

		gtk_window_set_default_size(GTK_WINDOW(dlg), fileSelectorWidth, fileSelectorHeight);
		if (gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_ACCEPT) {
			GSList *names = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dlg));
			GSList *nameCurrent = names;
			while (nameCurrent) {
				char *filename = static_cast<char *>(nameCurrent->data);
				Open(filename);
				g_free(filename);
				nameCurrent = g_slist_next(nameCurrent);
			}
			g_slist_free(names);
			canceled = false;
		}
		gtk_widget_destroy(dlg);
#endif
	}
	return !canceled;
}

#ifdef USE_FILE_CHOOSER
// Callback function to show hidden files in filechooser
void SciTEGTK::toggle_hidden_cb(GtkToggleButton *toggle, gpointer data) {
	GtkWidget *file_chooser = GTK_WIDGET(data);

	if (gtk_toggle_button_get_active(toggle))
		g_object_set(GTK_FILE_CHOOSER(file_chooser), "show-hidden", TRUE, NULL);
	else
		g_object_set(GTK_FILE_CHOOSER(file_chooser), "show-hidden", FALSE, NULL);
}
#endif

void SciTEGTK::HandleSaveAs(const char *savePath) {
	switch (saveFormat) {
	case sfCopy:
		SaveBuffer(savePath);
		break;
	case sfHTML:
		SaveToHTML(savePath);
		break;
	case sfRTF:
		SaveToRTF(savePath);
		break;
	case sfPDF:
		SaveToPDF(savePath);
		break;
	case sfTEX:
		SaveToTEX(savePath);
		break;
	case sfXML:
		SaveToXML(savePath);
		break;
	default:
		SaveAs(savePath);
	}
	dlgFileSelector.OK();
}

bool SciTEGTK::SaveAsXXX(FileFormat fmt, const char *title) {
	filePath.SetWorkingDirectory();
	bool canceled = true;
	saveFormat = fmt;
	if (!dlgFileSelector.Created()) {
#ifndef USE_FILE_CHOOSER
		dlgFileSelector = gtk_file_selection_new(LocaliseString(title).c_str());
		gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(PWidget(dlgFileSelector))->ok_button),
		                   "clicked", GtkSignalFunc(SaveAsSignal), this);
		gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(PWidget(dlgFileSelector))->cancel_button),
		                   "clicked", GtkSignalFunc(OpenCancelSignal), this);
		// Get a bigger save as dialog
		gtk_window_set_default_size(GTK_WINDOW(PWidget(dlgFileSelector)),
		                            fileSelectorWidth, fileSelectorHeight);
		canceled = dlgFileSelector.ShowModal(PWidget(wSciTE));
#else
		GtkWidget *dlg = gtk_file_chooser_dialog_new(LocaliseString(title).c_str(),
				      GTK_WINDOW(wSciTE.GetID()),
				      GTK_FILE_CHOOSER_ACTION_SAVE,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
				      NULL);
		gtk_dialog_set_default_response(GTK_DIALOG(dlg), GTK_RESPONSE_ACCEPT);

		if (gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_ACCEPT) {
			char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));
			gtk_widget_destroy(dlg);
			HandleSaveAs(filename);
			g_free(filename);
		} else {
			gtk_widget_destroy(dlg);
		}
#endif
	}
	return !canceled;
}

bool SciTEGTK::SaveAsDialog() {
	return SaveAsXXX(sfSource, "Save File As");
}

void SciTEGTK::SaveACopy() {
	SaveAsXXX(sfCopy, "Save a Copy");
}

void SciTEGTK::SaveAsHTML() {
	SaveAsXXX(sfHTML, "Export File As HTML");
}

void SciTEGTK::SaveAsRTF() {
	SaveAsXXX(sfRTF, "Export File As RTF");
}

void SciTEGTK::SaveAsPDF() {
	SaveAsXXX(sfPDF, "Export File As PDF");
}

void SciTEGTK::SaveAsTEX() {
	SaveAsXXX(sfTEX, "Export File As LaTeX");
}

void SciTEGTK::SaveAsXML() {
	SaveAsXXX(sfXML, "Export File As XML");
}

void SciTEGTK::LoadSessionDialog() {
	filePath.SetWorkingDirectory();
	if (!dlgFileSelector.Created()) {
#ifndef USE_FILE_CHOOSER
		dlgFileSelector = gtk_file_selection_new(LocaliseString("Load Session").c_str());
		gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(PWidget(dlgFileSelector))->ok_button),
		                   "clicked", GtkSignalFunc(OpenSessionOKSignal), this);
		gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(PWidget(dlgFileSelector))->cancel_button),
		                   "clicked", GtkSignalFunc(OpenCancelSignal), this);
		gtk_signal_connect(GTK_OBJECT(PWidget(dlgFileSelector)),
		                   "size_allocate", GtkSignalFunc(OpenResizeSignal),
		                   this);
		// Get a bigger open dialog
		gtk_window_set_default_size(GTK_WINDOW(PWidget(dlgFileSelector)),
		                            fileSelectorWidth, fileSelectorHeight);
		dlgFileSelector.ShowModal(PWidget(wSciTE));
#else
		GtkWidget *dlg = gtk_file_chooser_dialog_new("Load Session",
				      GTK_WINDOW(wSciTE.GetID()),
				      GTK_FILE_CHOOSER_ACTION_OPEN,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				      NULL);

		gtk_window_set_default_size(GTK_WINDOW(dlg), fileSelectorWidth, fileSelectorHeight);
		if (gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_ACCEPT) {
			char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));

			LoadSession(filename);
			g_free(filename);
		}
		gtk_widget_destroy(dlg);
#endif
	}
}

void SciTEGTK::SaveSessionDialog() {
	filePath.SetWorkingDirectory();
	if (!dlgFileSelector.Created()) {
#ifndef USE_FILE_CHOOSER
		dlgFileSelector = gtk_file_selection_new(LocaliseString("Save Session").c_str());
		gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(PWidget(dlgFileSelector))->ok_button),
		                   "clicked", GtkSignalFunc(SaveSessionOKSignal), this);
		gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(PWidget(dlgFileSelector))->cancel_button),
		                   "clicked", GtkSignalFunc(OpenCancelSignal), this);
		gtk_signal_connect(GTK_OBJECT(PWidget(dlgFileSelector)),
		                   "size_allocate", GtkSignalFunc(OpenResizeSignal),
		                   this);
		// Get a bigger open dialog
		gtk_window_set_default_size(GTK_WINDOW(PWidget(dlgFileSelector)),
		                            fileSelectorWidth, fileSelectorHeight);
		dlgFileSelector.ShowModal(PWidget(wSciTE));
#else
		GtkWidget *dlg = gtk_file_chooser_dialog_new("Save Session",
				      GTK_WINDOW(wSciTE.GetID()),
				      GTK_FILE_CHOOSER_ACTION_SAVE,
				      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
				      NULL);

		if (gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_ACCEPT) {
			char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));

			SaveSession(filename);
			g_free(filename);
		}
		gtk_widget_destroy(dlg);
#endif
	}
}

void SciTEGTK::Print(bool) {
	SelectionIntoProperties();
	AddCommand(props.GetWild("command.print.", filePath.AsInternal()), "",
	           SubsystemType("command.print.subsystem."));
	if (commandCurrent > 0) {
		isBuilding = true;
		Execute();
	}
}

void SciTEGTK::PrintSetup() {
	// Printing not yet supported on GTK+
}

SString SciTEGTK::GetRangeInUIEncoding(Window &win, int selStart, int selEnd) {
	int len = selEnd - selStart;
	SBuffer allocation(len * 3);
	Platform::SendScintilla(win.GetID(), SCI_SETTARGETSTART, selStart);
	Platform::SendScintilla(win.GetID(), SCI_SETTARGETEND, selEnd);
	int byteLength = Platform::SendScintillaPointer(
		win.GetID(), SCI_TARGETASUTF8, 0, allocation.ptr());
	SString sel(allocation);
	sel.remove(byteLength, 0);
	return sel;
}

void SciTEGTK::HandleFindReplace() {}

void SciTEGTK::Find() {
	if (wFindReplace.Created())
		return;
	SelectionIntoFind();
	FindReplace(false);
}

static SString Padded(const SString &s) {
	SString ret(s);
	ret.insert(0, "  ");
	ret += "  ";
	return ret;
}

void SciTEGTK::TranslatedSetTitle(GtkWindow *w, const char *original) {
	gtk_window_set_title(w, LocaliseString(original).c_str());
}

GtkWidget *SciTEGTK::TranslatedLabel(const char *original) {
	SString text = LocaliseString(original);
	// Don't know how to make an access key on a label transfer focus
	// to the next widget so remove the access key indicator.
	text.remove("_");
	return gtk_label_new(text.c_str());
}

GtkWidget *SciTEGTK::TranslatedCommand(const char *original, GtkAccelGroup *accel_group,
                              GtkSignalFunc func, gpointer data, GdkModifierType accelMask) {
	return MakeCommand(Padded(LocaliseString(original)).c_str(), accel_group, func, data, accelMask);
}

GtkWidget *SciTEGTK::TranslatedToggle(const char *original, GtkAccelGroup *accel_group, bool active) {
	return MakeToggle(LocaliseString(original).c_str(), accel_group, active);
}

static void FillComboFromMemory(GtkWidget *combo, const ComboMemory &mem, bool useTop = false) {
	GtkWidget * list = GTK_COMBO(combo)->list;
	for (int i = 0; i < mem.Length(); i++) {
		GtkWidget *item = gtk_list_item_new_with_label(mem.At(i).c_str());
		gtk_container_add(GTK_CONTAINER(list), item);
		gtk_widget_show(item);
	}
	if (useTop) {
		gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry), mem.At(0).c_str());
	}
}

SString SciTEGTK::EncodeString(const SString &s) {
	Platform::SendScintilla(PWidget(wEditor), SCI_SETLENGTHFORENCODE, s.length(), 0);
	int len = Platform::SendScintillaPointer(PWidget(wEditor), SCI_ENCODEDFROMUTF8,
		reinterpret_cast<uptr_t>(s.c_str()), 0);
	SBuffer ret(len);
	Platform::SendScintillaPointer(PWidget(wEditor), SCI_ENCODEDFROMUTF8,
		reinterpret_cast<uptr_t>(s.c_str()), ret.ptr());
	return SString(ret);
}

void SciTEGTK::FindReplaceGrabFields() {
	const char *findEntry = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(comboFind)->entry));
	findWhat = findEntry;
	memFinds.Insert(findWhat);
	if (comboReplace) {
		const char *replaceEntry = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(comboReplace)->entry));
		replaceWhat = replaceEntry;
		memReplaces.Insert(replaceWhat);
	}
	wholeWord = GTK_TOGGLE_BUTTON(toggleWord)->active;
	matchCase = GTK_TOGGLE_BUTTON(toggleCase)->active;
	regExp = GTK_TOGGLE_BUTTON(toggleRegExp)->active;
	wrapFind = GTK_TOGGLE_BUTTON(toggleWrap)->active;
	unSlash = GTK_TOGGLE_BUTTON(toggleUnSlash)->active;
	reverseFind = GTK_TOGGLE_BUTTON(toggleReverse)->active;
}

void SciTEGTK::FRCancelSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->wFindReplace.Destroy();
}

gint SciTEGTK::FRKeySignal(GtkWidget *w, GdkEventKey *event, SciTEGTK *scitew) {
	if (event->keyval == GDK_Escape) {
		gtk_signal_emit_stop_by_name(GTK_OBJECT(w), "key_press_event");
		scitew->wFindReplace.Destroy();
	}
	return FALSE;
}

void SciTEGTK::FRFindSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->FindReplaceGrabFields();
	if (!scitew->comboReplace)
		scitew->wFindReplace.Destroy();
	if (scitew->findWhat[0]) {
		scitew->FindNext(scitew->reverseFind);
	}
}

void SciTEGTK::FRReplaceSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->FindReplaceGrabFields();
	if (scitew->findWhat[0]) {
		scitew->ReplaceOnce();
	}
}

void SciTEGTK::FRReplaceAllSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->FindReplaceGrabFields();
	if (scitew->findWhat[0]) {
		scitew->ReplaceAll(false);
		scitew->wFindReplace.Destroy();
	}
}

void SciTEGTK::FRReplaceInSelectionSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->FindReplaceGrabFields();
	if (scitew->findWhat[0]) {
		scitew->ReplaceAll(true);
		scitew->wFindReplace.Destroy();
	}
}

void SciTEGTK::FindInFilesCmd() {
	const char *findEntry = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(comboFindInFiles)->entry));
	props.Set("find.what", findEntry);
	memFinds.Insert(findEntry);

	const char *dirEntry = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(comboDir)->entry));
	props.Set("find.directory", dirEntry);
	memDirectory.Insert(dirEntry);

#ifdef RECURSIVE_GREP_WORKING

	if (GTK_TOGGLE_BUTTON(toggleRec)->active)
		props.Set("find.recursive", props.Get("find.recursive.recursive").c_str());
	else
		props.Set("find.recursive", props.Get("find.recursive.not").c_str());
#endif

	const char *filesEntry = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(comboFiles)->entry));
	props.Set("find.files", filesEntry);
	memFiles.Insert(filesEntry);

	dlgFindInFiles.Destroy();

	//printf("Grepping for <%s> in <%s>\n",
	//	props.Get("find.what"),
	//	props.Get("find.files"));
	SelectionIntoProperties();
	SString findCommand = props.GetNewExpand("find.command");
	if (findCommand == "") {
		findCommand = sciteExecutable.AsInternal();
		findCommand += " -grep n \"";
		findCommand += props.Get("find.files");
		findCommand += "\" \"";
		char *quotedForm = Slash(props.Get("find.what").c_str(), true);
		findCommand += quotedForm;
		findCommand += "\"";
		delete []quotedForm;
		//~ fprintf(stderr, "%s\n", findCommand.c_str());
	}
	AddCommand(findCommand, props.Get("find.directory"), jobCLI);
	if (commandCurrent > 0)
		Execute();
}

void SciTEGTK::FindInFilesDotDot() {
	FilePath findInDir(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(comboDir)->entry)));
	gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(comboDir)->entry), findInDir.Directory().AsInternal());
}

void SciTEGTK::FindInFiles() {
	GtkAccelGroup *accel_group = gtk_accel_group_new();

	SelectionIntoFind();
	props.Set("find.what", findWhat.c_str());

	FilePath findInDir = filePath.Directory().AbsolutePath();
	props.Set("find.directory", findInDir.AsInternal());

	dlgFindInFiles = gtk_dialog_new();
	gtk_window_set_policy(GTK_WINDOW(PWidget(dlgFindInFiles)), FALSE, TRUE, FALSE);
	TranslatedSetTitle(GTK_WINDOW(PWidget(dlgFindInFiles)), "Find in Files");

#ifdef RECURSIVE_GREP_WORKING

	GtkWidget *table = gtk_table_new(4, 3, FALSE);
#else

	GtkWidget *table = gtk_table_new(1, 3, FALSE);
#endif

	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(dlgFindInFiles))->vbox),
	                   table, TRUE, TRUE, 0);

	GtkAttachOptions opts = static_cast<GtkAttachOptions>(
	                            GTK_SHRINK | GTK_FILL);

	GtkAttachOptions optse = static_cast<GtkAttachOptions>(
	                             GTK_SHRINK | GTK_FILL | GTK_EXPAND);

	int row = 0;

	comboFindInFiles = gtk_combo_new();

	GtkWidget *labelFind = TranslatedLabel("Find what:");
	gtk_table_attach(GTK_TABLE(table), labelFind, 0, 1,
	                 row, row + 1, opts, opts, 5, 5);
	gtk_widget_show(labelFind);

	FillComboFromMemory(comboFindInFiles, memFinds);
	gtk_combo_set_case_sensitive(GTK_COMBO(comboFindInFiles), TRUE);
	gtk_combo_set_use_arrows_always(GTK_COMBO(comboFindInFiles), TRUE);

	gtk_table_attach(GTK_TABLE(table), comboFindInFiles, 1, 3,
	                 row, row + 1, optse, opts, 5, 5);
	gtk_widget_show(comboFindInFiles);
	gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(comboFindInFiles)->entry), findWhat.c_str());
	gtk_entry_select_region(GTK_ENTRY(GTK_COMBO(comboFindInFiles)->entry), 0, findWhat.length());
	gtk_signal_connect(GTK_OBJECT(GTK_COMBO(comboFindInFiles)->entry),
	                   "activate", GtkSignalFunc(FindInFilesSignal), this);
	gtk_combo_disable_activate(GTK_COMBO(comboFindInFiles));

	row++;

	GtkWidget *labelFiles = TranslatedLabel("Files:");
	gtk_table_attach(GTK_TABLE(table), labelFiles, 0, 1,
	                 row, row + 1, opts, opts, 5, 5);
	gtk_widget_show(labelFiles);

	comboFiles = gtk_combo_new();
	FillComboFromMemory(comboFiles, memFiles, true);
	gtk_combo_set_case_sensitive(GTK_COMBO(comboFiles), TRUE);
	gtk_combo_set_use_arrows_always(GTK_COMBO(comboFiles), TRUE);

	gtk_table_attach(GTK_TABLE(table), comboFiles, 1, 3,
	                 row, row + 1, optse, opts, 5, 5);
	gtk_widget_show(comboFiles);
	gtk_signal_connect(GTK_OBJECT(GTK_COMBO(comboFiles)->entry),
	                   "activate", GtkSignalFunc(FindInFilesSignal), this);
	gtk_combo_disable_activate(GTK_COMBO(comboFiles));

	row++;

	GtkWidget *labelDir = TranslatedLabel("Directory:");

	gtk_table_attach(GTK_TABLE(table), labelDir, 0, 1,
	                 row, row + 1, opts, opts, 5, 5);
	gtk_widget_show(labelDir);

	comboDir = gtk_combo_new();
	FillComboFromMemory(comboDir, memDirectory);
	gtk_combo_set_case_sensitive(GTK_COMBO(comboDir), TRUE);
	gtk_combo_set_use_arrows_always(GTK_COMBO(comboDir), TRUE);

	gtk_table_attach(GTK_TABLE(table), comboDir, 1, 2,
	                 row, row + 1, optse, opts, 5, 5);
	gtk_widget_show(comboDir);
	gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(comboDir)->entry), findInDir.AsInternal());
	// Make a little wider than would happen automatically to show realistic paths
#if GTK_MAJOR_VERSION >= 2
	gtk_entry_set_width_chars(GTK_ENTRY(GTK_COMBO(comboDir)->entry), 40);
#endif
	gtk_signal_connect(GTK_OBJECT(GTK_COMBO(comboDir)->entry),
	                   "activate", GtkSignalFunc(FindInFilesSignal), this);
	gtk_combo_disable_activate(GTK_COMBO(comboDir));

	GtkWidget *btnDotDot = TranslatedCommand("_..",
	                       accel_group,
	                       GtkSignalFunc(DotDotSignal), this);
	gtk_table_attach(GTK_TABLE(table), btnDotDot, 2, 3,
	                 row, row + 1, optse, opts, 5, 5);
	gtk_widget_show(btnDotDot);

#ifdef RECURSIVE_GREP_WORKING

	row++;

	toggleRec = TranslatedToggle("Re_cursive Directories", accel_group, false);
	gtk_table_attach(GTK_TABLE(table), toggleRec, 1, 2, row, row + 1, opts, opts, 3, 0);
	gtk_widget_show(toggleRec);
#endif

	gtk_widget_show(table);

	GtkWidget *btnFind = TranslatedCommand("F_ind", accel_group,
	                                       GtkSignalFunc(FindInFilesSignal), this);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(dlgFindInFiles))->action_area),
	                   btnFind, TRUE, TRUE, 0);
	gtk_widget_show(btnFind);

	GtkWidget *btnCancel = TranslatedCommand("_Cancel", accel_group,
	                       GtkSignalFunc(Dialog::SignalCancel), &dlgFindInFiles);

	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(dlgFindInFiles))->action_area),
	                   btnCancel, TRUE, TRUE, 0);
	gtk_widget_show(btnCancel);

	gtk_widget_grab_default(GTK_WIDGET(btnFind));
	gtk_widget_grab_focus(GTK_WIDGET(GTK_COMBO(comboFindInFiles)->entry));

	gtk_window_add_accel_group(GTK_WINDOW(PWidget(dlgFindInFiles)), accel_group);
	dlgFindInFiles.ShowModal(PWidget(wSciTE));
}

void SciTEGTK::Replace() {
	if (wFindReplace.Created())
		return;
	SelectionIntoFind();
	FindReplace(true);
}

void SciTEGTK::ExecuteNext() {
	icmd++;
	if (icmd < commandCurrent && icmd < commandMax) {
		Execute();
	} else {
		icmd = 0;
		executing = false;
		if (needReadProperties)
			ReadProperties();
		CheckReload();
		CheckMenus();
		ClearJobQueue();
	}
}

void SciTEGTK::ContinueExecute(int fromPoll) {
	char buf[8192];
	int count = read(fdFIFO, buf, sizeof(buf) - 1);
	if (count > 0) {
		buf[count] = '\0';
		OutputAppendString(buf);
	} else if (count == 0) {
		SString sExitMessage(WEXITSTATUS(exitStatus));
		sExitMessage.insert(0, ">Exit code: ");
		if (WIFSIGNALED(exitStatus)) {
			SString sSignal(WTERMSIG(exitStatus));
			sSignal.insert(0, " Signal: ");
			sExitMessage += sSignal;
		}
		if (timeCommands) {
			sExitMessage += "    Time: ";
			sExitMessage += SString(commandTime.Duration(), 3);
		}
		sExitMessage.append("\n");
		OutputAppendString(sExitMessage.c_str());
		// Move selection back to beginning of this run so that F4 will go
		// to first error of this run.
		if (scrollOutput && returnOutputToCommand)
			SendOutput(SCI_GOTOPOS, originalEnd);
		returnOutputToCommand = true;
		gdk_input_remove(inputHandle);
		inputHandle = 0;
		gtk_timeout_remove(pollID);
		pollID = 0;
		close(fdFIFO);
		fdFIFO = 0;
		unlink(resultsFile);
		pidShell = 0;
		triedKill = false;
		ExecuteNext();
	} else { // count < 0
		// The FIFO is not ready - expected when called from polling callback.
		if (!fromPoll) {
			OutputAppendString(">End Bad\n");
		}
	}
}

void SciTEGTK::IOSignal(SciTEGTK *scitew) {
	scitew->ContinueExecute(FALSE);
}

int xsystem(const char *s, const char *resultsFile) {
	int pid = 0;
	//printf("xsystem %s %s\n", s, resultsFile);
	if ((pid = fork()) == 0) {
		close(0);
		int fh = open(resultsFile, O_WRONLY);
		close(1);
		dup(fh);
		close(2);
		dup(fh);
		execlp("/bin/sh", "sh", "-c", s, static_cast<char *>(NULL));
		exit(127);
	}
	return pid;
}

static bool MakePipe(const char *name) {
	// comment: () isn't implemented in cygwin yet
#if defined(__vms) || defined(__CYGWIN__)
	// No mkfifo on OpenVMS or CYGWIN
	int fd = creat(name, 0777);
	close(fd);	// Handle must be closed before re-opened
#else

	int fd = mkfifo(name, S_IRUSR | S_IWUSR);
#endif

	return fd >= 0;
}

void SciTEGTK::Execute() {
	SciTEBase::Execute();

	commandTime.Duration(true);
	if (scrollOutput)
		SendOutput(SCI_GOTOPOS, SendOutput(SCI_GETTEXTLENGTH));
	originalEnd = SendOutput(SCI_GETCURRENTPOS);

	OutputAppendString(">");
	OutputAppendString(jobQueue[icmd].command.c_str());
	OutputAppendString("\n");

	unlink(resultsFile);
	if (jobQueue[icmd].directory.IsSet()) {
		jobQueue[icmd].directory.SetWorkingDirectory();
	}

	if (jobQueue[icmd].jobType == jobShell) {
		if (fork() == 0)
			execlp("/bin/sh", "sh", "-c", jobQueue[icmd].command.c_str(), 
				static_cast<char *>(NULL));
		else
			ExecuteNext();
	} else if (jobQueue[icmd].jobType == jobExtension) {
		if (extender)
			extender->OnExecute(jobQueue[icmd].command.c_str());
		ExecuteNext();
	} else {
		if (!MakePipe(resultsFile)) {
			OutputAppendString(">Failed to create FIFO\n");
			ExecuteNext();
			return;
		}

		pidShell = xsystem(jobQueue[icmd].command.c_str(), resultsFile);
		triedKill = false;
		fdFIFO = open(resultsFile, O_RDONLY | O_NONBLOCK);
		if (fdFIFO < 0) {
			OutputAppendString(">Failed to open\n");
			fdFIFO = 0;
			return;
		}
		inputHandle = gdk_input_add(fdFIFO, GDK_INPUT_READ,
		                            (GdkInputFunction) IOSignal, this);
		// Also add a background task in case there is no output from the tool
		pollID = gtk_timeout_add(200, (gint (*)(void *)) SciTEGTK::PollTool, this);
	}
}

void SciTEGTK::StopExecute() {
	if (!triedKill && pidShell) {
		kill(pidShell, SIGKILL);
		triedKill = true;
	}
}

void SciTEGTK::GotoSignal(GtkWidget *, SciTEGTK *scitew) {
	const char *lineEntry = gtk_entry_get_text(GTK_ENTRY(scitew->gotoEntry));
	int lineNo = atoi(lineEntry);

	scitew->GotoLineEnsureVisible(lineNo - 1);

	scitew->dlgGoto.Destroy();
}

void SciTEGTK::GoLineDialog() {
	GtkAccelGroup *accel_group = gtk_accel_group_new();

	dlgGoto = gtk_dialog_new();
	TranslatedSetTitle(GTK_WINDOW(PWidget(dlgGoto)), "Go To");
	gtk_container_border_width(GTK_CONTAINER(PWidget(dlgGoto)), 0);

	GtkWidget *table = gtk_table_new(2, 1, FALSE);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(dlgGoto))->vbox),
	                   table, TRUE, TRUE, 0);

	GtkAttachOptions opts = static_cast<GtkAttachOptions>(
	                            GTK_EXPAND | GTK_SHRINK | GTK_FILL);
	GtkWidget *label = TranslatedLabel("Destination Line Number:");
	gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, opts, opts, 5, 5);
	gtk_widget_show(label);

	gotoEntry = gtk_entry_new();
	gtk_table_attach(GTK_TABLE(table), gotoEntry, 1, 2, 0, 1, opts, opts, 5, 5);
	gtk_signal_connect(GTK_OBJECT(gotoEntry),
	                   "activate", GtkSignalFunc(GotoSignal), this);
	gtk_widget_grab_focus(GTK_WIDGET(gotoEntry));
	gtk_widget_show(gotoEntry);

	gtk_widget_show(table);

	GtkWidget *btnGoTo = TranslatedCommand("_Go To", accel_group,
	                                       GtkSignalFunc(GotoSignal), this);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(dlgGoto))->action_area),
	                   btnGoTo, TRUE, TRUE, 0);
	gtk_widget_grab_default(GTK_WIDGET(btnGoTo));
	gtk_widget_show(btnGoTo);

	GtkWidget *btnCancel = TranslatedCommand("_Cancel", accel_group,
	                       GtkSignalFunc(Dialog::SignalCancel), &dlgGoto);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(dlgGoto))->action_area),
	                   btnCancel, TRUE, TRUE, 0);
	gtk_widget_show(btnCancel);

	gtk_window_add_accel_group(GTK_WINDOW(PWidget(dlgGoto)), accel_group);
	dlgGoto.ShowModal(PWidget(wSciTE));
}

bool SciTEGTK::AbbrevDialog() { return false; }
void SciTEGTK::TabSizeDialog() {}

void SciTEGTK::ParamGrab() {
	if (wParameters.Created()) {
		for (int param = 0; param < maxParam; param++) {
			SString paramText(param + 1);
			const char *paramVal = gtk_entry_get_text(GTK_ENTRY(entryParam[param]));
			props.Set(paramText.c_str(), paramVal);
		}
		UpdateStatusBar(true);
	}
}

gint SciTEGTK::ParamKeySignal(GtkWidget *w, GdkEventKey *event, SciTEGTK *scitew) {
	if (event->keyval == GDK_Escape) {
		gtk_signal_emit_stop_by_name(GTK_OBJECT(w), "key_press_event");
		scitew->wParameters.Destroy();
	}
	return FALSE;
}

void SciTEGTK::ParamCancelSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->wParameters.Destroy();
	scitew->CheckMenus();
}

void SciTEGTK::ParamSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->paramDialogCanceled = false;
	scitew->ParamGrab();
	scitew->wParameters.Destroy();
	scitew->CheckMenus();
}

bool SciTEGTK::ParametersDialog(bool modal) {
	if (wParameters.Created()) {
		ParamGrab();
		if (!modal) {
			wParameters.Destroy();
		}
		return true;
	}
	paramDialogCanceled = true;
	GtkAccelGroup *accel_group = gtk_accel_group_new();
	wParameters = gtk_dialog_new();
	TranslatedSetTitle(GTK_WINDOW(PWidget(wParameters)), "Parameters");
	gtk_container_border_width(GTK_CONTAINER(PWidget(wParameters)), 0);

	gtk_signal_connect(GTK_OBJECT(PWidget(wParameters)),
	                   "destroy", GtkSignalFunc(destroyDialog), &wParameters);

	GtkWidget *table = gtk_table_new(2, modal ? 10 : 9, FALSE);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(wParameters))->vbox),
	                   table, TRUE, TRUE, 0);

	GtkAttachOptions opts = static_cast<GtkAttachOptions>(
	                            GTK_EXPAND | GTK_SHRINK | GTK_FILL);

	int row = 0;
	if (modal) {
		GtkWidget *cmd = gtk_label_new(parameterisedCommand.c_str());
		gtk_table_attach(GTK_TABLE(table), cmd, 0, 2, row, row + 1, opts, opts, 5, 5);
		gtk_widget_show(cmd);
		row++;
	}

	for (int param = 0; param < maxParam; param++) {
		SString paramText(param + 1);
		SString paramTextVal = props.Get(paramText.c_str());
		paramText.append(":");
		GtkWidget *label = gtk_label_new(paramText.c_str());
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, row, row + 1, opts, opts, 5, 5);
		gtk_widget_show(label);

		entryParam[param] = gtk_entry_new();
		gtk_entry_set_text(GTK_ENTRY(entryParam[param]), paramTextVal.c_str());
		if (param == 0)
			gtk_entry_select_region(GTK_ENTRY(entryParam[param]), 0, paramTextVal.length());
		gtk_table_attach(GTK_TABLE(table), entryParam[param], 1, 2, row, row + 1, opts, opts, 5, 5);
		gtk_signal_connect(GTK_OBJECT(entryParam[param]),
		                   "activate", GtkSignalFunc(ParamSignal), this);
		gtk_widget_show(entryParam[param]);

		row++;
	}

	gtk_widget_grab_focus(GTK_WIDGET(entryParam[0]));
	gtk_widget_show(table);

	GtkWidget *btnExecute = TranslatedCommand(modal ? "_Execute" : "_Set", accel_group,
	                        GtkSignalFunc(ParamSignal), this);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(wParameters))->action_area),
	                   btnExecute, TRUE, TRUE, 0);
	gtk_widget_grab_default(GTK_WIDGET(btnExecute));
	gtk_widget_show(btnExecute);

	GtkWidget *btnCancel = TranslatedCommand(modal ? "_Cancel" : "_Close",
	                       accel_group,
	                       GtkSignalFunc(ParamCancelSignal), this);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(wParameters))->action_area),
	                   btnCancel, TRUE, TRUE, 0);
	gtk_widget_show(btnCancel);

	// Mark it as a modal transient dialog
	gtk_window_set_modal(GTK_WINDOW(PWidget(wParameters)), modal);
	gtk_window_set_transient_for (GTK_WINDOW(PWidget(wParameters)),
	                              GTK_WINDOW(PWidget(wSciTE)));

	gtk_window_add_accel_group(GTK_WINDOW(PWidget(wParameters)), accelGroup);
	wParameters.Show();
	if (modal) {
		while (wParameters.Created()) {
			gtk_main_iteration();
		}
	}
	return !paramDialogCanceled;
}

void SciTEGTK::FindReplace(bool replace) {
	GtkAccelGroup *accel_group = gtk_accel_group_new();

	replacing = replace;
	wFindReplace = gtk_dialog_new();
	gtk_window_set_policy(GTK_WINDOW(PWidget(wFindReplace)), TRUE, TRUE, FALSE);
	TranslatedSetTitle(GTK_WINDOW(PWidget(wFindReplace)), replace ? "Replace" : "Find");

	gtk_signal_connect(GTK_OBJECT(PWidget(wFindReplace)),
	                   "destroy", GtkSignalFunc(destroyDialog), &wFindReplace);

	GtkWidget *table = gtk_table_new(2, replace ? 4 : 3, FALSE);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(wFindReplace))->vbox),
	                   table, TRUE, TRUE, 0);

	GtkAttachOptions opts = static_cast<GtkAttachOptions>(
	                            GTK_SHRINK | GTK_FILL);

	GtkAttachOptions optse = static_cast<GtkAttachOptions>(
	                             GTK_SHRINK | GTK_FILL | GTK_EXPAND);

	int row = 0;

	GtkWidget *labelFind = TranslatedLabel("Find what:");
	gtk_table_attach(GTK_TABLE(table), labelFind, 0, 1,
	                 row, row + 1, opts, opts, 5, 5);
	gtk_widget_show(labelFind);

	comboFind = gtk_combo_new();
	FillComboFromMemory(comboFind, memFinds);

	gtk_table_attach(GTK_TABLE(table), comboFind, 1, 2,
	                 row, row + 1, optse, opts, 5, 5);
	gtk_widget_show(comboFind);
	gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(comboFind)->entry), findWhat.c_str());
#if GTK_MAJOR_VERSION >= 2
	gtk_entry_set_width_chars(GTK_ENTRY(GTK_COMBO(comboFind)->entry), 40);
#endif
	gtk_entry_select_region(GTK_ENTRY(GTK_COMBO(comboFind)->entry), 0, findWhat.length());
	gtk_signal_connect(GTK_OBJECT(GTK_COMBO(comboFind)->entry),
	                   "activate", GtkSignalFunc(FRFindSignal), this);
	gtk_combo_disable_activate(GTK_COMBO(comboFind));
	gtk_combo_set_case_sensitive(GTK_COMBO(comboFind), TRUE);
	gtk_combo_set_use_arrows_always(GTK_COMBO(comboFind), TRUE);
	row++;

	if (replace) {
		GtkWidget *labelReplace = TranslatedLabel("Replace with:");
		gtk_table_attach(GTK_TABLE(table), labelReplace, 0, 1,
		                 row, row + 1, opts, opts, 5, 5);
		gtk_widget_show(labelReplace);

		comboReplace = gtk_combo_new();
		FillComboFromMemory(comboReplace, memReplaces);

		gtk_table_attach(GTK_TABLE(table), comboReplace, 1, 2,
		                 row, row + 1, optse, opts, 5, 5);
		gtk_widget_show(comboReplace);
		gtk_signal_connect(GTK_OBJECT(GTK_COMBO(comboReplace)->entry),
		                   "activate", GtkSignalFunc(FRFindSignal), this);
		gtk_combo_disable_activate(GTK_COMBO(comboReplace));
		gtk_combo_set_case_sensitive(GTK_COMBO(comboReplace), TRUE);
		gtk_combo_set_use_arrows_always(GTK_COMBO(comboReplace), TRUE);

		row++;
	} else {
		comboReplace = 0;
	}

	// Whole Word
	toggleWord = TranslatedToggle("Match whole word _only", accel_group, wholeWord);
	gtk_table_attach(GTK_TABLE(table), toggleWord, 0, 2, row, row + 1, opts, opts, 3, 0);
	row++;

	// Case Sensitive
	toggleCase = TranslatedToggle("_Match case", accel_group, matchCase);
	gtk_table_attach(GTK_TABLE(table), toggleCase, 0, 2, row, row + 1, opts, opts, 3, 0);
	row++;

	// Regular Expression
	toggleRegExp = TranslatedToggle("Regular e_xpression", accel_group, regExp);
	gtk_table_attach(GTK_TABLE(table), toggleRegExp, 0, 2, row, row + 1, opts, opts, 3, 0);
	row++;

	// Wrap Around
	toggleWrap = TranslatedToggle("_Wrap around", accel_group, wrapFind);
	gtk_table_attach(GTK_TABLE(table), toggleWrap, 0, 2, row, row + 1, opts, opts, 3, 0);
	row++;

	// Transform backslash expressions
	toggleUnSlash = TranslatedToggle("_Transform backslash expressions", accel_group, unSlash);
	gtk_table_attach(GTK_TABLE(table), toggleUnSlash, 0, 2, row, row + 1, opts, opts, 3, 0);
	row++;

	// Reverse
	toggleReverse = TranslatedToggle("Re_verse direction", accel_group, reverseFind);
	gtk_table_attach(GTK_TABLE(table), toggleReverse, 0, 2, row, row + 1, opts, opts, 3, 0);
	row++;

	gtk_widget_show_all(table);

	gtk_box_set_homogeneous(
	    GTK_BOX(GTK_DIALOG(PWidget(wFindReplace))->action_area), false);

	GtkWidget *btnFind = TranslatedCommand("F_ind", accel_group,
	                                       GtkSignalFunc(FRFindSignal), this);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(wFindReplace))->action_area),
	                   btnFind, TRUE, TRUE, 0);

	if (replace) {
		GtkWidget *btnReplace = TranslatedCommand("_Replace", accel_group,
		                        GtkSignalFunc(FRReplaceSignal), this);
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(wFindReplace))->action_area),
		                   btnReplace, TRUE, TRUE, 0);

		GtkWidget *btnReplaceAll = TranslatedCommand("Replace _All", accel_group,
		                           GtkSignalFunc(FRReplaceAllSignal), this);
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(wFindReplace))->action_area),
		                   btnReplaceAll, TRUE, TRUE, 0);
#if GTK_MAJOR_VERSION < 2
		GtkWidget *btnReplaceInSelection = TranslatedCommand("Replace in _Selection", accel_group,
		                                   GtkSignalFunc(FRReplaceInSelectionSignal), this);
#else
		// GTK+ 2 uses more space so use shorter button text
		GtkWidget *btnReplaceInSelection = TranslatedCommand("In _Selection", accel_group,
		                                   GtkSignalFunc(FRReplaceInSelectionSignal), this);
#endif
		gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(wFindReplace))->action_area),
		                   btnReplaceInSelection, TRUE, TRUE, 0);
	}

	GtkWidget *btnCancel = TranslatedCommand("_Cancel", accel_group,
	                       GtkSignalFunc(FRCancelSignal), this);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(PWidget(wFindReplace))->action_area),
	                   btnCancel, TRUE, TRUE, 0);

	gtk_signal_connect(GTK_OBJECT(PWidget(wFindReplace)),
	                   "key_press_event", GtkSignalFunc(FRKeySignal), this);

	gtk_widget_show_all(GTK_WIDGET(GTK_DIALOG(PWidget(wFindReplace))->action_area));

	GTK_WIDGET_SET_FLAGS(GTK_WIDGET(btnFind), GTK_CAN_DEFAULT);
	gtk_widget_grab_default(GTK_WIDGET(btnFind));
	gtk_widget_grab_focus(GTK_WIDGET(GTK_COMBO(comboFind)->entry));

	// Mark it as a transient dialog
	gtk_window_set_transient_for (GTK_WINDOW(PWidget(wFindReplace)),
	                              GTK_WINDOW(PWidget(wSciTE)));

	gtk_window_add_accel_group(GTK_WINDOW(PWidget(wFindReplace)), accel_group);
	wFindReplace.Show();
}

void SciTEGTK::DestroyFindReplace() {
	wFindReplace.Destroy();
}

int SciTEGTK::WindowMessageBox(Window &w, const SString &msg, int style) {
	if (!messageBoxDialog) {
		dialogsOnScreen++;
		GtkAccelGroup *accel_group = gtk_accel_group_new();

		messageBoxResult = -1;
		messageBoxDialog = gtk_dialog_new();
		gtk_window_set_title(GTK_WINDOW(messageBoxDialog), appName);
		gtk_container_border_width(GTK_CONTAINER(messageBoxDialog), 0);

		gtk_signal_connect(GTK_OBJECT(messageBoxDialog),
		                   "destroy", GtkSignalFunc(messageBoxDestroy), &messageBoxDialog);

		int escapeResult = IDOK;
		if ((style & 0xf) == MB_OK) {
			AddMBButton(messageBoxDialog, "_OK", IDOK, accel_group, true);
		} else {
			AddMBButton(messageBoxDialog, "_Yes", IDYES, accel_group, true);
			AddMBButton(messageBoxDialog, "_No", IDNO, accel_group);
			escapeResult = IDNO;
			if ((style & 0xf) == MB_YESNOCANCEL) {
				AddMBButton(messageBoxDialog, "_Cancel", IDCANCEL, accel_group);
				escapeResult = IDCANCEL;
			}
		}
		gtk_signal_connect(GTK_OBJECT(messageBoxDialog),
		                   "key_press_event", GtkSignalFunc(messageBoxKey),
		                   reinterpret_cast<gpointer>(escapeResult));

		if (style & MB_ABOUTBOX) {
			GtkWidget *explanation = scintilla_new();
			scintilla_set_id(SCINTILLA(explanation), 0);
			gtk_box_pack_start(GTK_BOX(GTK_DIALOG(messageBoxDialog)->vbox),
			                   explanation, TRUE, TRUE, 0);
			gtk_widget_set_usize(GTK_WIDGET(explanation), 480, 380);
			gtk_widget_show_all(explanation);
			SetAboutMessage(explanation, "SciTE");
		} else {
			GtkWidget *label = gtk_label_new(msg.c_str());
			gtk_misc_set_padding(GTK_MISC(label), 10, 10);
			gtk_box_pack_start(GTK_BOX(GTK_DIALOG(messageBoxDialog)->vbox),
			                   label, TRUE, TRUE, 0);
			gtk_widget_show(label);
		}

		// Mark it as a modal transient dialog
		gtk_window_set_modal(GTK_WINDOW(messageBoxDialog), TRUE);
		gtk_window_set_transient_for(GTK_WINDOW(messageBoxDialog),
		                             GTK_WINDOW(PWidget(w)));

		gtk_widget_show(messageBoxDialog);
		gtk_window_add_accel_group(GTK_WINDOW(messageBoxDialog), accel_group);
		while (messageBoxResult < 0) {
			gtk_main_iteration();
		}
		dialogsOnScreen--;
	}
	return messageBoxResult;
}

void SciTEGTK::AboutDialog() {
	WindowMessageBox(wSciTE, "SciTE\nby Neil Hodgson neilh@scintilla.org .",
	                 MB_OK | MB_ABOUTBOX);
}

void SciTEGTK::QuitProgram() {
	if (SaveIfUnsureAll() != IDCANCEL) {

#ifndef NO_FILER
		int fdPipe = props.GetInt("scite.ipc_fdpipe");
		if (fdPipe != -1) {
			close(fdPipe);
			unlink(props.Get("scite.ipc_name").c_str());
		}
#else
		//clean up any pipes that are ours
		if (pipeFD != -1) {
			//printf("Cleaning up pipe\n");
			close(pipeFD);
			unlink(pipeName);
		}
#endif
		gtk_exit(0);
	}
}

gint SciTEGTK::MoveResize(GtkWidget *, GtkAllocation * /*allocation*/, SciTEGTK *scitew) {
	//Platform::DebugPrintf("SciTEGTK move resize %d %d\n", allocation->width, allocation->height);
	scitew->SizeSubWindows();
	return TRUE;
}

gint SciTEGTK::QuitSignal(GtkWidget *, GdkEventAny *, SciTEGTK *scitew) {
	if (scitew->SaveIfUnsureAll() != IDCANCEL) {

		//clean up any pipes that are ours
		if (scitew->pipeFD != -1) {
			//printf("Cleaning up pipe\n");
			close(scitew->pipeFD);
			unlink(scitew->pipeName);

		}
		gtk_exit(0);
	}
	// No need to return FALSE for quit as gtk_exit will have been called
	// if needed.
	return TRUE;
}

void SciTEGTK::ButtonSignal(GtkWidget *, gpointer data) {
	instance->Command((guint)(long)data);
}

void SciTEGTK::MenuSignal(SciTEGTK *scitew, guint action, GtkWidget *) {
	//Platform::DebugPrintf("action %d %x \n", action, w);
	if (scitew->allowMenuActions)
		scitew->Command(action);
}

void SciTEGTK::CommandSignal(GtkWidget *, gint wParam, gpointer lParam, SciTEGTK *scitew) {

	//Platform::DebugPrintf("Command: %x %x %x\n", w, wParam, lParam);
	scitew->Command(wParam, reinterpret_cast<long>(lParam));
}

void SciTEGTK::NotifySignal(GtkWidget *, gint /*wParam*/, gpointer lParam, SciTEGTK *scitew) {
	//Platform::DebugPrintf("Notify: %x %x %x\n", w, wParam, lParam);
	scitew->Notify(reinterpret_cast<SCNotification *>(lParam));
}

gint SciTEGTK::KeyPress(GtkWidget * /*widget*/, GdkEventKey *event, SciTEGTK *scitew) {
	return scitew->Key(event);
}

gint SciTEGTK::MousePress(GtkWidget * /*widget*/, GdkEventButton *event, SciTEGTK *scitew) {
	return scitew->Mouse(event);
}

// Translate key strokes that are not in a menu into commands
class KeyToCommand {
public:
	int modifiers;
	unsigned int key;	// For alphabetic keys has to match the shift modifier.
	int msg;
};

enum {
    m__ = 0,
    mS_ = GDK_SHIFT_MASK,
    m_C = GDK_CONTROL_MASK,
    mSC = GDK_SHIFT_MASK | GDK_CONTROL_MASK
};

static KeyToCommand kmap[] = {
                                 {m_C, GDK_Tab, IDM_NEXTFILE},
                                 {mSC, GDK_ISO_Left_Tab, IDM_PREVFILE},
                                 {m_C, GDK_KP_Enter, IDM_COMPLETEWORD},
                                 {m_C, GDK_F3, IDM_FINDNEXTSEL},
                                 {mSC, GDK_F3, IDM_FINDNEXTBACKSEL},
                                 {m_C, GDK_F4, IDM_CLOSE},
                                 {m_C, 'j', IDM_PREVMATCHPPC},
                                 {mSC, 'J', IDM_SELECTTOPREVMATCHPPC},
                                 {m_C, 'k', IDM_NEXTMATCHPPC},
                                 {mSC, 'K', IDM_SELECTTONEXTMATCHPPC},
                                 {m_C, GDK_KP_Multiply, IDM_EXPAND},
                                 {0, 0, 0},
                             };

inline bool KeyMatch(const char *menuKey, int keyval, int modifiers) {
	return SciTEKeys::MatchKeyCode(
		SciTEKeys::ParseKeyCode(menuKey), keyval, modifiers);
}

gint SciTEGTK::Key(GdkEventKey *event) {
	//printf("S-key: %d %x %x %x %x\n",event->keyval, event->state, GDK_SHIFT_MASK, GDK_CONTROL_MASK, GDK_F3);
	int modifiers = event->state & (GDK_SHIFT_MASK | GDK_CONTROL_MASK | GDK_MOD1_MASK);
	int commandID = 0;
	for (int i = 0; kmap[i].msg; i++) {
		if ((event->keyval == kmap[i].key) && (modifiers == kmap[i].modifiers)) {
			commandID = kmap[i].msg;
		}
	}
	if (!commandID) {
		// Look through language menu
		for (int j = 0; j < languageItems; j++) {
			if (KeyMatch(languageMenu[j].menuKey.c_str(), event->keyval, modifiers)) {
				commandID = IDM_LANGUAGE + j;
			}
		}
	}
	if (commandID) {
		Command(commandID);
	}
	if ((commandID == IDM_NEXTFILE) || (commandID == IDM_PREVFILE)) {
		// Stop the default key processing from moving the focus
		gtk_signal_emit_stop_by_name(
		    GTK_OBJECT(PWidget(wSciTE)), "key_press_event");
	}

	// check tools menu command shortcuts
	// TODO: test this on GTK+ 1 and 2.
	for (int tool_i = 0; tool_i < toolMax; ++tool_i) {
		GtkWidget *item = gtk_item_factory_get_widget_by_action(itemFactory, IDM_TOOLS + tool_i);
		if (item) {
			long keycode = reinterpret_cast<long>(gtk_object_get_user_data(GTK_OBJECT(item)));
			if (keycode && SciTEKeys::MatchKeyCode(keycode, event->keyval, modifiers)) {
				SciTEBase::MenuCommand(IDM_TOOLS + tool_i);
				return 1;
			}
		}
	}

	// check user defined keys
	for (int cut_i = 0; cut_i < shortCutItems; cut_i++) {
		if (KeyMatch(shortCutItemList[cut_i].menuKey.c_str(), event->keyval, modifiers)) {
			int commandNum = SciTEBase::GetMenuCommandAsInt(shortCutItemList[cut_i].menuCommand);
			if (commandNum != -1) {
				if (commandNum < 2000) {
					SciTEBase::MenuCommand(commandNum);
				} else {
					SciTEBase::SendFocused(commandNum);
				}
				gtk_signal_emit_stop_by_name(
				    GTK_OBJECT(PWidget(wSciTE)), "key_press_event");
				return 1;
			}
		}
	}

	return 0;
}

void SciTEGTK::AddToPopUp(const char *label, int cmd, bool enabled) {
	SString localised = LocaliseString(label);
	localised.insert(0, "/");
	GtkItemFactoryEntry itemEntry = {
		const_cast<char *>(localised.c_str()), NULL,
		GTK_SIGNAL_FUNC(MenuSignal), cmd,
		const_cast<gchar *>(label[0] ? "<Item>" : "<Separator>")
#if GTK_MAJOR_VERSION >= 2
		,0
#endif
	};
	gtk_item_factory_create_item(GTK_ITEM_FACTORY(popup.GetID()),
	                             &itemEntry, this, 1);
	if (cmd) {
		GtkWidget *item = gtk_item_factory_get_widget_by_action(
		                      reinterpret_cast<GtkItemFactory *>(popup.GetID()), cmd);
		if (item)
			gtk_widget_set_sensitive(item, enabled);
	}
}

gint SciTEGTK::Mouse(GdkEventButton *event) {
	if (event->button == 3) {
		// PopUp menu
		Window w = wEditor;
		menuSource = IDM_SRCWIN;
		if (PWidget(w)->window != event->window) {
			if (PWidget(wOutput)->window == event->window) {
				menuSource = IDM_RUNWIN;
				w = wOutput;
			} else {
				menuSource = 0;
				//fprintf(stderr, "Menu source focus\n");
				return FALSE;
			}
		}
		// Convert to screen
		int ox = 0;
		int oy = 0;
		gdk_window_get_origin(PWidget(w)->window, &ox, &oy);
		ContextMenu(w, Point(static_cast<int>(event->x) + ox,
		                     static_cast<int>(event->y) + oy), wSciTE);
		//fprintf(stderr, "Menu source %s\n",
		//	(menuSource == IDM_SRCWIN) ? "IDM_SRCWIN" : "IDM_RUNWIN");
	} else {
		menuSource = 0;
		//fprintf(stderr, "Menu source focus\n");
	}

	return FALSE;
}

void SciTEGTK::DividerXOR(Point pt) {
	if (!xor_gc) {
		GdkGCValues values;
		values.foreground = PWidget(wSciTE)->style->white;
		values.function = GDK_XOR;
		values.subwindow_mode = GDK_INCLUDE_INFERIORS;
		xor_gc = gdk_gc_new_with_values(PWidget(wSciTE)->window,
		                                &values,
		                                static_cast<GdkGCValuesMask>(
		                                    GDK_GC_FOREGROUND | GDK_GC_FUNCTION | GDK_GC_SUBWINDOW));
	}
	if (splitVertical) {
		gdk_draw_line(PWidget(wSciTE)->window, xor_gc,
		              pt.x,
		              0,
		              pt.x,
		              PWidget(wSciTE)->allocation.height - 1);
	} else {
		gdk_draw_line(PWidget(wSciTE)->window, xor_gc,
		              0,
		              pt.y,
		              PWidget(wSciTE)->allocation.width - 1,
		              pt.y);
	}
	ptOld = pt;
}

gint SciTEGTK::DividerExpose(GtkWidget *widget, GdkEventExpose *, SciTEGTK *sciThis) {
	//GtkStyle style = gtk_widget_get_default_style();
	GdkRectangle area;
	area.x = 0;
	area.y = 0;
	area.width = widget->allocation.width;
	area.height = widget->allocation.height;
	gdk_window_clear_area(widget->window,
	                      area.x, area.y, area.width, area.height);
	if (widget->allocation.width > widget->allocation.height) {
		// Horizontal divider
		gtk_paint_hline(widget->style, widget->window, GTK_STATE_NORMAL,
		                &area, widget, const_cast<char *>("vpaned"),
		                0, widget->allocation.width - 1,
		                area.height / 2 - 1);
		gtk_paint_box (widget->style, widget->window,
		               GTK_STATE_NORMAL,
		               GTK_SHADOW_OUT,
		               &area, widget, const_cast<char *>("paned"),
		               area.width - sciThis->heightBar * 2, 1,
		               sciThis->heightBar - 2, sciThis->heightBar - 2);
	} else {
		// Vertical divider
		gtk_paint_vline(widget->style, widget->window, GTK_STATE_NORMAL,
		                &area, widget, const_cast<char *>("hpaned"),
		                0, widget->allocation.height - 1,
		                area.width / 2 - 1);
		gtk_paint_box (widget->style, widget->window,
		               GTK_STATE_NORMAL,
		               GTK_SHADOW_OUT,
		               &area, widget, const_cast<char *>("paned"),
		               1, area.height - sciThis->heightBar * 2,
		               sciThis->heightBar - 2, sciThis->heightBar - 2);
	}
	return TRUE;
}

gint SciTEGTK::DividerMotion(GtkWidget *, GdkEventMotion *event, SciTEGTK *scitew) {
	if (scitew->capturedMouse) {
		int x = 0;
		int y = 0;
		GdkModifierType state;
		if (event->is_hint) {
			gdk_window_get_pointer(PWidget(scitew->wSciTE)->window, &x, &y, &state);
			if (state & GDK_BUTTON1_MASK) {
				scitew->DividerXOR(scitew->ptOld);
				scitew->DividerXOR(Point(x, y));
			}
		}
	}
	return TRUE;
}

gint SciTEGTK::DividerPress(GtkWidget *, GdkEventButton *, SciTEGTK *scitew) {
	int x = 0;
	int y = 0;
	GdkModifierType state;
	gdk_window_get_pointer(PWidget(scitew->wSciTE)->window, &x, &y, &state);
	scitew->ptStartDrag = Point(x, y);
	scitew->capturedMouse = true;
	scitew->heightOutputStartDrag = scitew->heightOutput;
	gtk_widget_grab_focus(GTK_WIDGET(PWidget(scitew->wDivider)));
	gtk_grab_add(GTK_WIDGET(PWidget(scitew->wDivider)));
	scitew->DividerXOR(scitew->ptStartDrag);
	return TRUE;
}

gint SciTEGTK::DividerRelease(GtkWidget *, GdkEventButton *, SciTEGTK *scitew) {
	if (scitew->capturedMouse) {
		scitew->capturedMouse = false;
		gtk_grab_remove(GTK_WIDGET(PWidget(scitew->wDivider)));
		scitew->DividerXOR(scitew->ptOld);
		int x = 0;
		int y = 0;
		GdkModifierType state;
		gdk_window_get_pointer(PWidget(scitew->wSciTE)->window, &x, &y, &state);
		scitew->MoveSplit(Point(x, y));
	}
	return TRUE;
}

void SciTEGTK::DragDataReceived(GtkWidget *, GdkDragContext *context,
                                gint /*x*/, gint /*y*/, GtkSelectionData *seldata, guint /*info*/, guint time, SciTEGTK *scitew) {
	scitew->OpenUriList(reinterpret_cast<const char *>(seldata->data));
	gtk_drag_finish(context, TRUE, FALSE, time);
}

void SetFocus(GtkWidget *hwnd) {
	Platform::SendScintilla(hwnd, SCI_GRABFOCUS, 0, 0);
}

gint SciTEGTK::TabBarRelease(GtkNotebook *notebook, GdkEventButton *event) {
	if (event->button == 1) {
		SetDocumentAt(gtk_notebook_current_page(GTK_NOTEBOOK(wTabBar.GetID())));
		CheckReload();
	} else if (event->button == 2) {
		for (int pageNum=0;pageNum<gtk_notebook_get_n_pages(notebook);pageNum++) {
			GtkWidget *page = gtk_notebook_get_nth_page(notebook, pageNum);
			if (page) {
				GtkWidget *label = gtk_notebook_get_tab_label(notebook, page);
				if (event->x < (label->allocation.x + label->allocation.width)) {
					CloseTab(pageNum);
					break;
				}
			}
		}
	}
	return FALSE;
}

void SciTEGTK::OpenCancelSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->dlgFileSelector.Cancel();
}

gint SciTEGTK::OpenKeySignal(GtkWidget *w, GdkEventKey *event, SciTEGTK *scitew) {
	if (event->keyval == GDK_Escape) {
		gtk_signal_emit_stop_by_name(GTK_OBJECT(w), "key_press_event");
		scitew->dlgFileSelector.Cancel();
	}
	return FALSE;
}

void SciTEGTK::OpenOKSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->Open(gtk_file_selection_get_filename(
	                 GTK_FILE_SELECTION(PWidget(scitew->dlgFileSelector))));
	scitew->dlgFileSelector.OK();
}

void SciTEGTK::OpenSessionOKSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->LoadSession(gtk_file_selection_get_filename(
	                 GTK_FILE_SELECTION(PWidget(scitew->dlgFileSelector))));
	scitew->dlgFileSelector.OK();
}

void SciTEGTK::SaveSessionOKSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->SaveSession(gtk_file_selection_get_filename(
	                 GTK_FILE_SELECTION(PWidget(scitew->dlgFileSelector))));
	scitew->dlgFileSelector.OK();
}

void SciTEGTK::OpenResizeSignal(GtkWidget *, GtkAllocation *allocation, SciTEGTK *scitew) {
	scitew->fileSelectorWidth = allocation->width;
	scitew->fileSelectorHeight = allocation->height;
}

void SciTEGTK::SaveAsSignal(GtkWidget *, SciTEGTK *scitew) {
	scitew->HandleSaveAs(gtk_file_selection_get_filename(
	                         GTK_FILE_SELECTION(PWidget(scitew->dlgFileSelector))));
}

GtkWidget *SciTEGTK::pixmap_new(GtkWidget *window, gchar **xpm) {
	GdkBitmap *mask = 0;

	/* now for the pixmap from gdk */
	GtkStyle *style = gtk_widget_get_style(window);
	GdkPixmap *pixmap = gdk_pixmap_create_from_xpm_d(
	                        window->window,
	                        &mask,
	                        &style->bg[GTK_STATE_NORMAL],
	                        xpm);

	/* a pixmap widget to contain the pixmap */
	GtkWidget *pixmapwid = gtk_pixmap_new(pixmap, mask);
	gtk_widget_show(pixmapwid);

	return pixmapwid;
}

GtkWidget *SciTEGTK::AddToolButton(const char *text, int cmd, GtkWidget *toolbar_icon) {

	GtkWidget *button = gtk_toolbar_append_element(GTK_TOOLBAR(PWidget(wToolBar)),
	                    GTK_TOOLBAR_CHILD_BUTTON,
	                    NULL,
	                    NULL,
	                    text, NULL,
	                    toolbar_icon, NULL, NULL);

	gtk_signal_connect(GTK_OBJECT(button), "clicked",
	                   GTK_SIGNAL_FUNC (ButtonSignal),
	                   (gpointer)cmd);
	return button;
}

void SciTEGTK::AddToolBar() {
	if (props.GetInt("toolbar.usestockicons") == 1) {
		AddToolButton("New", IDM_NEW, gtk_image_new_from_stock("gtk-new", GTK_ICON_SIZE_LARGE_TOOLBAR));
		AddToolButton("Open", IDM_OPEN, gtk_image_new_from_stock("gtk-open", GTK_ICON_SIZE_LARGE_TOOLBAR));
		AddToolButton("Save", IDM_SAVE, gtk_image_new_from_stock("gtk-save", GTK_ICON_SIZE_LARGE_TOOLBAR));
		AddToolButton("Close", IDM_CLOSE, gtk_image_new_from_stock("gtk-close", GTK_ICON_SIZE_LARGE_TOOLBAR));

		gtk_toolbar_append_space(GTK_TOOLBAR(PWidget(wToolBar)));
		AddToolButton("Undo", IDM_UNDO, gtk_image_new_from_stock("gtk-undo", GTK_ICON_SIZE_LARGE_TOOLBAR));
		AddToolButton("Redo", IDM_REDO, gtk_image_new_from_stock("gtk-redo", GTK_ICON_SIZE_LARGE_TOOLBAR));

		gtk_toolbar_append_space(GTK_TOOLBAR(PWidget(wToolBar)));
		AddToolButton("Cut", IDM_CUT, gtk_image_new_from_stock("gtk-cut", GTK_ICON_SIZE_LARGE_TOOLBAR));
		AddToolButton("Copy", IDM_COPY, gtk_image_new_from_stock("gtk-copy", GTK_ICON_SIZE_LARGE_TOOLBAR));
		AddToolButton("Paste", IDM_PASTE, gtk_image_new_from_stock("gtk-paste", GTK_ICON_SIZE_LARGE_TOOLBAR));

		gtk_toolbar_append_space(GTK_TOOLBAR(PWidget(wToolBar)));
		AddToolButton("Find in Files", IDM_FINDINFILES, gtk_image_new_from_stock("gtk-find", GTK_ICON_SIZE_LARGE_TOOLBAR));
		AddToolButton("Find", IDM_FIND, gtk_image_new_from_stock("gtk-zoom-fit", GTK_ICON_SIZE_LARGE_TOOLBAR));
		AddToolButton("Find Next", IDM_FINDNEXT, gtk_image_new_from_stock("gtk-jump-to", GTK_ICON_SIZE_LARGE_TOOLBAR));
		AddToolButton("Replace", IDM_REPLACE, gtk_image_new_from_stock("gtk-find-and-replace", GTK_ICON_SIZE_LARGE_TOOLBAR));

		gtk_toolbar_append_space(GTK_TOOLBAR(PWidget(wToolBar)));
		btnCompile = AddToolButton("Compile", IDM_COMPILE, gtk_image_new_from_stock("gtk-execute", GTK_ICON_SIZE_LARGE_TOOLBAR));
		btnBuild = AddToolButton("Build", IDM_BUILD, gtk_image_new_from_stock("gtk-convert", GTK_ICON_SIZE_LARGE_TOOLBAR));
		btnStop = AddToolButton("Stop", IDM_STOPEXECUTE, gtk_image_new_from_stock("gtk-stop", GTK_ICON_SIZE_LARGE_TOOLBAR));

		gtk_toolbar_append_space(GTK_TOOLBAR(PWidget(wToolBar)));
		AddToolButton("Previous", IDM_PREVFILE, gtk_image_new_from_stock("gtk-go-back", GTK_ICON_SIZE_LARGE_TOOLBAR));
		AddToolButton("Next Buffer", IDM_NEXTFILE, gtk_image_new_from_stock("gtk-go-forward", GTK_ICON_SIZE_LARGE_TOOLBAR));
	} else {
		AddToolButton("New", IDM_NEW, pixmap_new(PWidget(wSciTE), filenew_xpm));
		AddToolButton("Open", IDM_OPEN, pixmap_new(PWidget(wSciTE), fileopen_xpm));
		AddToolButton("Save", IDM_SAVE, pixmap_new(PWidget(wSciTE), filesave_xpm));
		AddToolButton("Close", IDM_CLOSE, pixmap_new(PWidget(wSciTE), close_xpm));

		gtk_toolbar_append_space(GTK_TOOLBAR(PWidget(wToolBar)));
		AddToolButton("Undo", IDM_UNDO, pixmap_new(PWidget(wSciTE), undo_xpm));
		AddToolButton("Redo", IDM_REDO, pixmap_new(PWidget(wSciTE), redo_xpm));

		gtk_toolbar_append_space(GTK_TOOLBAR(PWidget(wToolBar)));
		AddToolButton("Cut", IDM_CUT, pixmap_new(PWidget(wSciTE), editcut_xpm));
		AddToolButton("Copy", IDM_COPY, pixmap_new(PWidget(wSciTE), editcopy_xpm));
		AddToolButton("Paste", IDM_PASTE, pixmap_new(PWidget(wSciTE), editpaste_xpm));

		gtk_toolbar_append_space(GTK_TOOLBAR(PWidget(wToolBar)));
		AddToolButton("Find in Files", IDM_FINDINFILES, pixmap_new(PWidget(wSciTE), findinfiles_xpm));
		AddToolButton("Find", IDM_FIND, pixmap_new(PWidget(wSciTE), search_xpm));
		AddToolButton("Find Next", IDM_FINDNEXT, pixmap_new(PWidget(wSciTE), findnext_xpm));
		AddToolButton("Replace", IDM_REPLACE, pixmap_new(PWidget(wSciTE), replace_xpm));

		gtk_toolbar_append_space(GTK_TOOLBAR(PWidget(wToolBar)));
		btnCompile = AddToolButton("Compile", IDM_COMPILE, pixmap_new(PWidget(wSciTE), compile_xpm));
		btnBuild = AddToolButton("Build", IDM_BUILD, pixmap_new(PWidget(wSciTE), build_xpm));
		btnStop = AddToolButton("Stop", IDM_STOPEXECUTE, pixmap_new(PWidget(wSciTE), stop_xpm));

		gtk_toolbar_append_space(GTK_TOOLBAR(PWidget(wToolBar)));
		AddToolButton("Previous", IDM_PREVFILE, pixmap_new(PWidget(wSciTE), prev_xpm));
		AddToolButton("Next Buffer", IDM_NEXTFILE, pixmap_new(PWidget(wSciTE), next_xpm));
	}
}

SString SciTEGTK::TranslatePath(const char *path) {
	if (path && path[0] == '/') {
		SString spathTranslated;
		SString spath(path, 1, strlen(path));
		spath.append("/");
		int end = spath.search("/");
		while (spath.length() > 1) {
			SString segment(spath.c_str(), 0, end);
			SString segmentLocalised = LocaliseString(segment.c_str());
			segmentLocalised.substitute("/", "|");
			spathTranslated.append("/");
			spathTranslated.append(segmentLocalised.c_str());
			spath.remove(0, end + 1);
			end = spath.search("/");
		}
		return spathTranslated;
	} else {
		return path;
	}
}

void SciTEGTK::CreateTranslatedMenu(int n, SciTEItemFactoryEntry items[],
                                    int nRepeats, const char *prefix, int startNum,
                                    int startID, const char *radioStart) {

	char *gthis = reinterpret_cast<char *>(this);
	int dim = n + nRepeats;
	GtkItemFactoryEntry *translatedItems = new GtkItemFactoryEntry[dim];
	SString *translatedText = new SString[dim];
	SString *translatedRadios = new SString[dim];
	char **userDefinedAccels = new char*[n];
	SString menuPath;
	int i = 0;

	for (; i < n; i++) {
		// Try to find user-defined accelerator key
		menuPath = "menukey";			// menupath="menukey"
		menuPath += items[i].path;		// menupath="menukey/File/Save _As..."
		menuPath.remove("_");			// menupath="menukey/File/Save As..."
		menuPath.remove(".");			// menupath="menukey/File/Save As"
		menuPath.substitute('/', '.');	// menupath="menukey.File.Save As"
		menuPath.substitute(' ', '_');	// menupath="menukey.File.Save_As"
		menuPath.lowercase();			// menupath="menukey.file.save_as"

		SString accelKey = props.Get(menuPath.c_str());

		int accLength = accelKey.length();
		if (accLength > 0) {
			if (accelKey == "\"\"" || accelKey == "none") {
				accelKey.clear();	// Allow user to clear accelerator key
			}
			userDefinedAccels[i] = new char[accLength + 1];
			strncpy(userDefinedAccels[i], accelKey.c_str(), accLength + 1);
			items[i].accelerator = userDefinedAccels[i];
		} else {
			userDefinedAccels[i] = NULL;
		}

		translatedItems[i].path = items[i].path;
		translatedItems[i].accelerator = items[i].accelerator;
		translatedItems[i].callback = items[i].callback;
		translatedItems[i].callback_action = items[i].callback_action;
		translatedItems[i].item_type = items[i].item_type;
#if GTK_MAJOR_VERSION >= 2
		translatedItems[i].extra_data = 0;
#endif
		translatedText[i] = TranslatePath(translatedItems[i].path);
		translatedItems[i].path = const_cast<char *>(translatedText[i].c_str());
		translatedRadios[i] = TranslatePath(translatedItems[i].item_type);
		translatedItems[i].item_type = const_cast<char *>(translatedRadios[i].c_str());

	}
	GtkItemFactoryCallback menuSig = GtkItemFactoryCallback(MenuSignal);
	for (; i < dim; i++) {
		int suffix = i - n + startNum;
		SString ssnum(suffix);
		translatedText[i] = TranslatePath(prefix);
		translatedText[i] += ssnum;
		translatedItems[i].path = const_cast<char *>(translatedText[i].c_str());
		translatedItems[i].accelerator = NULL;
		translatedItems[i].callback = menuSig;
		translatedItems[i].callback_action = startID + suffix;
		translatedRadios[i] = TranslatePath(radioStart);
		translatedItems[i].item_type = const_cast<char *>(translatedRadios[i].c_str());
	}
	gtk_item_factory_create_items(itemFactory, dim, translatedItems, gthis);
	delete []translatedRadios;
	delete []translatedText;
	delete []translatedItems;

	// Release all the memory allocated for the user-defined accelerator keys
	for (i = 0; i < n; i++) {
		if (userDefinedAccels[i] != NULL)
			delete[] userDefinedAccels[i];
	}
	delete[] userDefinedAccels;
}

#define ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

void SciTEGTK::CreateMenu() {

	GtkItemFactoryCallback menuSig = GtkItemFactoryCallback(MenuSignal);
	SciTEItemFactoryEntry menuItems[] = {
	                                      {"/_File", NULL, NULL, 0, "<Branch>"},
	                                      {"/File/_New", "<control>N", menuSig, IDM_NEW, 0},
	                                      {"/File/_Open...", "<control>O", menuSig, IDM_OPEN, 0},
	                                      {"/File/Open Selected _Filename", "<control><shift>O", menuSig, IDM_OPENSELECTED, 0},
	                                      {"/File/_Revert", "<control>R", menuSig, IDM_REVERT, 0},
	                                      {"/File/_Close", "<control>W", menuSig, IDM_CLOSE, 0},
	                                      {"/File/_Save", "<control>S", menuSig, IDM_SAVE, 0},
	                                      {"/File/Save _As...", "<control><shift>S", menuSig, IDM_SAVEAS, 0},
	                                      {"/File/Save a Co_py...", "<control><shift>P", menuSig, IDM_SAVEACOPY, 0},
	                                      {"/File/Encodin_g", NULL, NULL, 0, "<Branch>"},
	                                      {"/File/Encoding/_8 Bit", NULL, menuSig, IDM_ENCODING_DEFAULT, "<RadioItem>"},
	                                      {"/File/Encoding/UCS-2 _Big Endian", NULL, menuSig, IDM_ENCODING_UCS2BE, "/File/Encoding/8 Bit"},
	                                      {"/File/Encoding/UCS-2 _Little Endian", NULL, menuSig, IDM_ENCODING_UCS2LE, "/File/Encoding/8 Bit"},
	                                      {"/File/Encoding/_UTF-8", NULL, menuSig, IDM_ENCODING_UTF8, "/File/Encoding/8 Bit"},
	                                      {"/File/Encoding/UTF-8 _Cookie", NULL, menuSig, IDM_ENCODING_UCOOKIE, "/File/Encoding/8 Bit"},
	                                      {"/File/_Export", "", 0, 0, "<Branch>"},
	                                      {"/File/Export/As _HTML...", NULL, menuSig, IDM_SAVEASHTML, 0},
	                                      {"/File/Export/As _RTF...", NULL, menuSig, IDM_SAVEASRTF, 0},
	                                      {"/File/Export/As _PDF...", NULL, menuSig, IDM_SAVEASPDF, 0},
	                                      {"/File/Export/As _LaTeX...", NULL, menuSig, IDM_SAVEASTEX, 0},
	                                      {"/File/Export/As _XML...", NULL, menuSig, IDM_SAVEASXML, 0},
	                                      {"/File/_Print", "<control>P", menuSig, IDM_PRINT, 0},
	                                      {"/File/sep1", NULL, NULL, 0, "<Separator>"},
	                                      {"/File/Load Session...", "", menuSig, IDM_LOADSESSION, 0},
	                                      {"/File/Save Session...", "", menuSig, IDM_SAVESESSION, 0},
	                                      {"/File/sep2", NULL, menuSig, IDM_MRU_SEP, "<Separator>"},
	                                      {"/File/File0", "", menuSig, fileStackCmdID + 0, 0},
	                                      {"/File/File1", "", menuSig, fileStackCmdID + 1, 0},
	                                      {"/File/File2", "", menuSig, fileStackCmdID + 2, 0},
	                                      {"/File/File3", "", menuSig, fileStackCmdID + 3, 0},
	                                      {"/File/File4", "", menuSig, fileStackCmdID + 4, 0},
	                                      {"/File/File5", "", menuSig, fileStackCmdID + 5, 0},
	                                      {"/File/File6", "", menuSig, fileStackCmdID + 6, 0},
	                                      {"/File/File7", "", menuSig, fileStackCmdID + 7, 0},
	                                      {"/File/File8", "", menuSig, fileStackCmdID + 8, 0},
	                                      {"/File/File9", "", menuSig, fileStackCmdID + 9, 0},
	                                      {"/File/sep3", NULL, NULL, 0, "<Separator>"},
	                                      {"/File/E_xit", "", menuSig, IDM_QUIT, 0},

	                                      {"/_Edit", NULL, NULL, 0, "<Branch>"},
	                                      {"/Edit/_Undo", "<control>Z", menuSig, IDM_UNDO, 0},

	                                      {"/Edit/_Redo", "<control>Y", menuSig, IDM_REDO, 0},
	                                      {"/Edit/sep1", NULL, NULL, 0, "<Separator>"},
	                                      {"/Edit/Cu_t", "<control>X", menuSig, IDM_CUT, 0},
	                                      {"/Edit/_Copy", "<control>C", menuSig, IDM_COPY, 0},
	                                      {"/Edit/_Paste", "<control>V", menuSig, IDM_PASTE, 0},
	                                      {"/Edit/_Delete", "Del", menuSig, IDM_CLEAR, 0},
	                                      {"/Edit/Select _All", "<control>A", menuSig, IDM_SELECTALL, 0},
	                                      {"/Edit/sep2", NULL, NULL, 0, "<Separator>"},
	                                      {"/Edit/Match _Brace", "<control>E", menuSig, IDM_MATCHBRACE, 0},
	                                      {"/Edit/Select t_o Brace", "<control><shift>E", menuSig, IDM_SELECTTOBRACE, 0},
	                                      {"/Edit/S_how Calltip", "<control><shift>space", menuSig, IDM_SHOWCALLTIP, 0},
	                                      {"/Edit/Complete S_ymbol", "<control>I", menuSig, IDM_COMPLETE, 0},
	                                      {"/Edit/Complete _Word", "<control>Return", menuSig, IDM_COMPLETEWORD, 0},
	                                      {"/Edit/_Expand Abbreviation", "<control>B", menuSig, IDM_ABBREV, 0},
	                                      //{"/Edit/_Insert Abbreviation", "<control>D", menuSig, IDM_INS_ABBREV, 0},
	                                      {"/Edit/Block Co_mment or Uncomment", "<control>Q", menuSig, IDM_BLOCK_COMMENT, 0},
	                                      {"/Edit/Bo_x Comment", "<control><shift>B", menuSig, IDM_BOX_COMMENT, 0},
	                                      {"/Edit/Stream Comme_nt", "<control><shift>Q", menuSig, IDM_STREAM_COMMENT, 0},
	                                      {"/Edit/Make _Selection Uppercase", "<control><shift>U", menuSig, IDM_UPRCASE, 0},
	                                      {"/Edit/Make Selection _Lowercase", "<control>U", menuSig, IDM_LWRCASE, 0},
	                                      {"/Edit/Para_graph", NULL, NULL, 0, "<Branch>"},
	                                      {"/Edit/Para_graph/_Join", NULL, menuSig, IDM_JOIN, 0},
	                                      {"/Edit/Para_graph/_Split", NULL, menuSig, IDM_SPLIT, 0},

	                                      {"/_Search", NULL, NULL, 0, "<Branch>"},
	                                      {"/Search/_Find...", "<control>F", menuSig, IDM_FIND, 0},
	                                      {"/Search/Find _Next", "F3", menuSig, IDM_FINDNEXT, 0},
	                                      {"/Search/Find Previou_s", "<shift>F3", menuSig, IDM_FINDNEXTBACK, 0},
	                                      {"/Search/F_ind in Files...", "<control><shift>F", menuSig, IDM_FINDINFILES, 0},
	                                      {"/Search/R_eplace...", "<control>H", menuSig, IDM_REPLACE, 0},
#ifdef INCREMENTAL_SEARCH
	                                      {"/Search/Incrementa&l Search", "<control><alt>I", menuSig, IDM_INCSEARCH, 0},
#endif
	                                      {"/Search/sep3", NULL, NULL, 0, "<Separator>"},
	                                      {"/Search/_Go To...", "<control>G", menuSig, IDM_GOTO, 0},
	                                      {"/Search/Next Book_mark", "F2", menuSig, IDM_BOOKMARK_NEXT, 0},
	                                      {"/Search/Pre_vious Bookmark", "<shift>F2", menuSig, IDM_BOOKMARK_PREV, 0},
	                                      {"/Search/Toggle Bookmar_k", "<control>F2", menuSig, IDM_BOOKMARK_TOGGLE, 0},
	                                      {"/Search/_Clear All Bookmarks", "", menuSig, IDM_BOOKMARK_CLEARALL, 0},

	                                      {"/_View", NULL, NULL, 0, "<Branch>"},
	                                      {"/View/Toggle _current fold", "", menuSig, IDM_EXPAND, 0},
	                                      {"/View/Toggle _all folds", "", menuSig, IDM_TOGGLE_FOLDALL, 0},
	                                      {"/View/sep1", NULL, NULL, 0, "<Separator>"},
	                                      {"/View/Full Scree_n", "F11", menuSig, IDM_FULLSCREEN, "<CheckItem>"},
	                                      {"/View/_Tool Bar", "", menuSig, IDM_VIEWTOOLBAR, "<CheckItem>"},
#if GTK_MAJOR_VERSION >= 2
	                                      {"/View/Tab _Bar", "", menuSig, IDM_VIEWTABBAR, "<CheckItem>"},
#endif
	                                      {"/View/_Status Bar", "", menuSig, IDM_VIEWSTATUSBAR, "<CheckItem>"},
	                                      {"/View/sep2", NULL, NULL, 0, "<Separator>"},
	                                      {"/View/_Whitespace", "<control><shift>A", menuSig, IDM_VIEWSPACE, "<CheckItem>"},
	                                      {"/View/_End of Line", "<control><shift>D", menuSig, IDM_VIEWEOL, "<CheckItem>"},
	                                      {"/View/_Indentation Guides", NULL, menuSig, IDM_VIEWGUIDES, "<CheckItem>"},
	                                      {"/View/_Line Numbers", "", menuSig, IDM_LINENUMBERMARGIN, "<CheckItem>"},
	                                      {"/View/_Margin", NULL, menuSig, IDM_SELMARGIN, "<CheckItem>"},
	                                      {"/View/_Fold Margin", NULL, menuSig, IDM_FOLDMARGIN, "<CheckItem>"},
	                                      {"/View/_Output", "F8", menuSig, IDM_TOGGLEOUTPUT, "<CheckItem>"},
	                                      {"/View/_Parameters", NULL, menuSig, IDM_TOGGLEPARAMETERS, "<CheckItem>"},

	                                      {"/_Tools", NULL, NULL, 0, "<Branch>"},
	                                      {"/Tools/_Compile", "<control>F7", menuSig, IDM_COMPILE, 0},
	                                      {"/Tools/_Build", "F7", menuSig, IDM_BUILD, 0},
	                                      {"/Tools/_Go", "F5", menuSig, IDM_GO, 0},

	                                      {"/Tools/Tool0", NULL, menuSig, IDM_TOOLS + 0, 0},
	                                      {"/Tools/Tool1", NULL, menuSig, IDM_TOOLS + 1, 0},
	                                      {"/Tools/Tool2", NULL, menuSig, IDM_TOOLS + 2, 0},
	                                      {"/Tools/Tool3", NULL, menuSig, IDM_TOOLS + 3, 0},
	                                      {"/Tools/Tool4", NULL, menuSig, IDM_TOOLS + 4, 0},
	                                      {"/Tools/Tool5", NULL, menuSig, IDM_TOOLS + 5, 0},
	                                      {"/Tools/Tool6", NULL, menuSig, IDM_TOOLS + 6, 0},
	                                      {"/Tools/Tool7", NULL, menuSig, IDM_TOOLS + 7, 0},
	                                      {"/Tools/Tool8", NULL, menuSig, IDM_TOOLS + 8, 0},
	                                      {"/Tools/Tool9", NULL, menuSig, IDM_TOOLS + 9, 0},

	                                      {"/Tools/Tool10", NULL, menuSig, IDM_TOOLS + 10, 0},
	                                      {"/Tools/Tool11", NULL, menuSig, IDM_TOOLS + 11, 0},
	                                      {"/Tools/Tool12", NULL, menuSig, IDM_TOOLS + 12, 0},
	                                      {"/Tools/Tool13", NULL, menuSig, IDM_TOOLS + 13, 0},
	                                      {"/Tools/Tool14", NULL, menuSig, IDM_TOOLS + 14, 0},
	                                      {"/Tools/Tool15", NULL, menuSig, IDM_TOOLS + 15, 0},
	                                      {"/Tools/Tool16", NULL, menuSig, IDM_TOOLS + 16, 0},
	                                      {"/Tools/Tool17", NULL, menuSig, IDM_TOOLS + 17, 0},
	                                      {"/Tools/Tool18", NULL, menuSig, IDM_TOOLS + 18, 0},
	                                      {"/Tools/Tool19", NULL, menuSig, IDM_TOOLS + 19, 0},

	                                      {"/Tools/Tool20", NULL, menuSig, IDM_TOOLS + 20, 0},
	                                      {"/Tools/Tool21", NULL, menuSig, IDM_TOOLS + 21, 0},
	                                      {"/Tools/Tool22", NULL, menuSig, IDM_TOOLS + 22, 0},
	                                      {"/Tools/Tool23", NULL, menuSig, IDM_TOOLS + 23, 0},
	                                      {"/Tools/Tool24", NULL, menuSig, IDM_TOOLS + 24, 0},
	                                      {"/Tools/Tool25", NULL, menuSig, IDM_TOOLS + 25, 0},
	                                      {"/Tools/Tool26", NULL, menuSig, IDM_TOOLS + 26, 0},
	                                      {"/Tools/Tool27", NULL, menuSig, IDM_TOOLS + 27, 0},
	                                      {"/Tools/Tool28", NULL, menuSig, IDM_TOOLS + 28, 0},
	                                      {"/Tools/Tool29", NULL, menuSig, IDM_TOOLS + 29, 0},

	                                      {"/Tools/Tool30", NULL, menuSig, IDM_TOOLS + 30, 0},
	                                      {"/Tools/Tool31", NULL, menuSig, IDM_TOOLS + 31, 0},
	                                      {"/Tools/Tool32", NULL, menuSig, IDM_TOOLS + 32, 0},
	                                      {"/Tools/Tool33", NULL, menuSig, IDM_TOOLS + 33, 0},
	                                      {"/Tools/Tool34", NULL, menuSig, IDM_TOOLS + 34, 0},
	                                      {"/Tools/Tool35", NULL, menuSig, IDM_TOOLS + 35, 0},
	                                      {"/Tools/Tool36", NULL, menuSig, IDM_TOOLS + 36, 0},
	                                      {"/Tools/Tool37", NULL, menuSig, IDM_TOOLS + 37, 0},
	                                      {"/Tools/Tool38", NULL, menuSig, IDM_TOOLS + 38, 0},
	                                      {"/Tools/Tool39", NULL, menuSig, IDM_TOOLS + 39, 0},

	                                      {"/Tools/Tool40", NULL, menuSig, IDM_TOOLS + 40, 0},
	                                      {"/Tools/Tool41", NULL, menuSig, IDM_TOOLS + 41, 0},
	                                      {"/Tools/Tool42", NULL, menuSig, IDM_TOOLS + 42, 0},
	                                      {"/Tools/Tool43", NULL, menuSig, IDM_TOOLS + 43, 0},
	                                      {"/Tools/Tool44", NULL, menuSig, IDM_TOOLS + 44, 0},
	                                      {"/Tools/Tool45", NULL, menuSig, IDM_TOOLS + 45, 0},
	                                      {"/Tools/Tool46", NULL, menuSig, IDM_TOOLS + 46, 0},
	                                      {"/Tools/Tool47", NULL, menuSig, IDM_TOOLS + 47, 0},
	                                      {"/Tools/Tool48", NULL, menuSig, IDM_TOOLS + 48, 0},
	                                      {"/Tools/Tool49", NULL, menuSig, IDM_TOOLS + 49, 0},


	                                      {"/Tools/_Stop Executing", "<control>.", menuSig, IDM_STOPEXECUTE, NULL},
	                                      {"/Tools/sep1", NULL, NULL, 0, "<Separator>"},
	                                      {"/Tools/_Next Message", "F4", menuSig, IDM_NEXTMSG, 0},
	                                      {"/Tools/_Previous Message", "<shift>F4", menuSig, IDM_PREVMSG, 0},
	                                      {"/Tools/Clear _Output", "<shift>F5", menuSig, IDM_CLEAROUTPUT, 0},
	                                      {"/Tools/_Switch Pane", "<control>F6", menuSig, IDM_SWITCHPANE, 0},
	                                  };

	SciTEItemFactoryEntry menuItemsOptions[] = {
	            {"/_Options", NULL, NULL, 0, "<Branch>"},
	            {"/Options/Vertical _Split", "", menuSig, IDM_SPLITVERTICAL, "<CheckItem>"},
	            {"/Options/_Wrap", "", menuSig, IDM_WRAP, "<CheckItem>"},
	            {"/Options/Wrap Out_put", "", menuSig, IDM_WRAPOUTPUT, "<CheckItem>"},
	            {"/Options/_Read-Only", "", menuSig, IDM_READONLY, "<CheckItem>"},
	            {"/Options/sep1", NULL, NULL, 0, "<Separator>"},
	            {"/Options/_Line End Characters", "", 0, 0, "<Branch>"},
	            {"/Options/Line End Characters/CR _+ LF", "", menuSig, IDM_EOL_CRLF, "<RadioItem>"},
	            {"/Options/Line End Characters/_CR", "", menuSig, IDM_EOL_CR, "/Options/Line End Characters/CR + LF"},
	            {"/Options/Line End Characters/_LF", "", menuSig, IDM_EOL_LF, "/Options/Line End Characters/CR + LF"},
	            {"/Options/_Convert Line End Characters", "", menuSig, IDM_EOL_CONVERT, 0},
	            {"/Options/sep2", NULL, NULL, 0, "<Separator>"},
	            {"/Options/Use _Monospaced Font", "<control>F11", menuSig, IDM_MONOFONT, "<CheckItem>"},
	            {"/Options/sep3", NULL, NULL, 0, "<Separator>"},
	            {"/Options/Open Local _Options File", "", menuSig, IDM_OPENLOCALPROPERTIES, 0},
	            {"/Options/Open _User Options File", "", menuSig, IDM_OPENUSERPROPERTIES, 0},
	            {"/Options/Open _Global Options File", "", menuSig, IDM_OPENGLOBALPROPERTIES, 0},
	            {"/Options/Open A_bbreviations File", "", menuSig, IDM_OPENABBREVPROPERTIES, 0},
	            {"/Options/Open Lua Startup Scr_ipt", "", menuSig, IDM_OPENLUAEXTERNALFILE, 0},
	            {"/Options/sep4", NULL, NULL, 0, "<Separator>"},
	            {"/Options/Edit Properties", "", 0, 0, "<Branch>"},
	        };

	SciTEItemFactoryEntry menuItemsLanguage[] = {
	            {"/_Language", NULL, NULL, 0, "<Branch>"},
	        };

	SciTEItemFactoryEntry menuItemsBuffer[] = {
	                                            {"/_Buffers", NULL, NULL, 0, "<Branch>"},
	                                            {"/Buffers/_Previous", "<shift>F6", menuSig, IDM_PREVFILE, 0},
	                                            {"/Buffers/_Next", "F6", menuSig, IDM_NEXTFILE, 0},
	                                            {"/Buffers/_Close All", "", menuSig, IDM_CLOSEALL, 0},
	                                            {"/Buffers/_Save All", "", menuSig, IDM_SAVEALL, 0},
	                                            {"/Buffers/sep2", NULL, NULL, 0, "<Separator>"},
	                                            {"/Buffers/Buffer0", "<alt>1", menuSig, bufferCmdID + 0, "<RadioItem>"},
	                                            {"/Buffers/Buffer1", "<alt>2", menuSig, bufferCmdID + 1, "/Buffers/Buffer0"},
	                                            {"/Buffers/Buffer2", "<alt>3", menuSig, bufferCmdID + 2, "/Buffers/Buffer0"},
	                                            {"/Buffers/Buffer3", "<alt>4", menuSig, bufferCmdID + 3, "/Buffers/Buffer0"},
	                                            {"/Buffers/Buffer4", "<alt>5", menuSig, bufferCmdID + 4, "/Buffers/Buffer0"},
	                                            {"/Buffers/Buffer5", "<alt>6", menuSig, bufferCmdID + 5, "/Buffers/Buffer0"},
	                                            {"/Buffers/Buffer6", "<alt>7", menuSig, bufferCmdID + 6, "/Buffers/Buffer0"},
	                                            {"/Buffers/Buffer7", "<alt>8", menuSig, bufferCmdID + 7, "/Buffers/Buffer0"},
	                                            {"/Buffers/Buffer8", "<alt>9", menuSig, bufferCmdID + 8, "/Buffers/Buffer0"},
	                                            {"/Buffers/Buffer9", "<alt>0", menuSig, bufferCmdID + 9, "/Buffers/Buffer0"},
	                                        };

	SciTEItemFactoryEntry menuItemsHelp[] = {
	                                          {"/_Help", NULL, NULL, 0, "<Branch>"},
	                                          {"/Help/_Help", "F1", menuSig, IDM_HELP, 0},
	                                          {"/Help/_SciTE Help", "", menuSig, IDM_HELP_SCITE, 0},
	                                          {"/Help/_About SciTE", "", menuSig, IDM_ABOUT, 0},
	                                      };

	accelGroup = gtk_accel_group_new();
	itemFactory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<main>", accelGroup);
	CreateTranslatedMenu(ELEMENTS(menuItems), menuItems);
	CreateTranslatedMenu(ELEMENTS(menuItemsOptions), menuItemsOptions,
	                     50, "/Options/Edit Properties/Props", 0, IDM_IMPORT, 0);
	CreateTranslatedMenu(ELEMENTS(menuItemsLanguage), menuItemsLanguage,
	                     60, "/Language/Language", 0, IDM_LANGUAGE, 0);
	if (props.GetInt("buffers") > 1)
		CreateTranslatedMenu(ELEMENTS(menuItemsBuffer), menuItemsBuffer,
		                     30, "/Buffers/Buffer", 10, bufferCmdID, "/Buffers/Buffer0");
	CreateTranslatedMenu(ELEMENTS(menuItemsHelp), menuItemsHelp);
#if GTK_MAJOR_VERSION < 2
	gtk_accel_group_attach(accelGroup, GTK_OBJECT(PWidget(wSciTE)));
#else
	gtk_window_add_accel_group(GTK_WINDOW(PWidget(wSciTE)), accelGroup);
#endif
}

void SciTEGTK::CreateUI() {
	wSciTE = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//GTK_WIDGET_UNSET_FLAGS(PWidget(wSciTE), GTK_CAN_FOCUS);
	gtk_window_set_policy(GTK_WINDOW(PWidget(wSciTE)), TRUE, TRUE, FALSE);

	char *gthis = reinterpret_cast<char *>(this);

	gtk_widget_set_events(PWidget(wSciTE),
	                      GDK_EXPOSURE_MASK
	                      | GDK_LEAVE_NOTIFY_MASK
	                      | GDK_BUTTON_PRESS_MASK
	                      | GDK_BUTTON_RELEASE_MASK
	                     );
	gtk_signal_connect(GTK_OBJECT(PWidget(wSciTE)), "delete_event",
	                   GTK_SIGNAL_FUNC(QuitSignal), gthis);

	gtk_signal_connect(GTK_OBJECT(PWidget(wSciTE)), "key_press_event",
	                   GtkSignalFunc(KeyPress), gthis);

	gtk_signal_connect(GTK_OBJECT(PWidget(wSciTE)), "button_press_event",
	                   GtkSignalFunc(MousePress), gthis);

	gtk_window_set_title(GTK_WINDOW(PWidget(wSciTE)), appName);
	const int useDefault = 0x10000000;
	int left = props.GetInt("position.left", useDefault);
	int top = props.GetInt("position.top", useDefault);
	int width = props.GetInt("position.width", useDefault);
	int height = props.GetInt("position.height", useDefault);
	bool maximize = false;
	if (width == -1 || height == -1) {
		maximize = true;
		width = gdk_screen_width() - left - 10;
		height = gdk_screen_height() - top - 30;
	}

	fileSelectorWidth = props.GetInt("fileselector.width", fileSelectorWidth);
	fileSelectorHeight = props.GetInt("fileselector.height", fileSelectorHeight);

	GtkWidget *boxMain = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(PWidget(wSciTE)), boxMain);
	GTK_WIDGET_UNSET_FLAGS(boxMain, GTK_CAN_FOCUS);

 	// The Menubar
	CreateMenu();
	if (props.GetInt("menubar.detachable") == 1) {
		GtkWidget *handle_box = gtk_handle_box_new();
		gtk_container_add(GTK_CONTAINER(handle_box),
			gtk_item_factory_get_widget(itemFactory, "<main>"));
		gtk_box_pack_start(GTK_BOX(boxMain),
			handle_box,
			FALSE, FALSE, 0);
	} else {
		gtk_box_pack_start(GTK_BOX(boxMain),
			gtk_item_factory_get_widget(itemFactory, "<main>"),
			FALSE, FALSE, 0);
	}

	// The Toolbar
#if GTK_MAJOR_VERSION < 2
	wToolBar = gtk_toolbar_new(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_ICONS);
#else
	wToolBar = gtk_toolbar_new();
	gtk_toolbar_set_orientation(GTK_TOOLBAR(PWidget(wToolBar)), GTK_ORIENTATION_HORIZONTAL);
#endif
	gtk_toolbar_set_style(GTK_TOOLBAR(PWidget(wToolBar)), GTK_TOOLBAR_ICONS);
	toolbarDetachable = props.GetInt("toolbar.detachable");
	if (toolbarDetachable == 1) {
		wToolBarBox = gtk_handle_box_new();
		gtk_container_add(GTK_CONTAINER(PWidget(wToolBarBox)), PWidget(wToolBar));
		gtk_box_pack_start(GTK_BOX(boxMain), PWidget(wToolBarBox), FALSE, FALSE, 0);
		gtk_widget_hide(GTK_WIDGET(PWidget(wToolBarBox)));
	} else {
		gtk_box_pack_start(GTK_BOX(boxMain), PWidget(wToolBar), FALSE, FALSE, 0);
		gtk_widget_hide(GTK_WIDGET(PWidget(wToolBar)));
	}
	gtk_container_set_border_width(GTK_CONTAINER(PWidget(wToolBar)), 0);
#if GTK_MAJOR_VERSION < 2
	gtk_toolbar_set_space_size(GTK_TOOLBAR(PWidget(wToolBar)), 17);
	gtk_toolbar_set_space_style(GTK_TOOLBAR(PWidget(wToolBar)), GTK_TOOLBAR_SPACE_LINE);
	gtk_toolbar_set_button_relief(GTK_TOOLBAR(PWidget(wToolBar)), GTK_RELIEF_NONE);
#endif
	tbVisible = false;

	// The Notebook (GTK2)
#if GTK_MAJOR_VERSION >= 2
	wTabBar = gtk_notebook_new();
	GTK_WIDGET_UNSET_FLAGS(PWidget(wTabBar),GTK_CAN_FOCUS);
	gtk_box_pack_start(GTK_BOX(boxMain),PWidget(wTabBar),FALSE,FALSE,0);
	gtk_signal_connect_after(GTK_OBJECT(PWidget(wTabBar)),
		"button-release-event", GTK_SIGNAL_FUNC(TabBarReleaseSignal), gthis);
	//gtk_notebook_set_scrollable(GTK_NOTEBOOK(PWidget(wTabBar)), TRUE);
#endif
	tabVisible = false;

	wContent = gtk_fixed_new();
	GTK_WIDGET_UNSET_FLAGS(PWidget(wContent), GTK_CAN_FOCUS);
	gtk_box_pack_start(GTK_BOX(boxMain), PWidget(wContent), TRUE, TRUE, 0);

	gtk_signal_connect(GTK_OBJECT(PWidget(wContent)), "size_allocate",
	                   GTK_SIGNAL_FUNC(MoveResize), gthis);

	wEditor = scintilla_new();
	scintilla_set_id(SCINTILLA(PWidget(wEditor)), IDM_SRCWIN);
	fnEditor = reinterpret_cast<SciFnDirect>(Platform::SendScintilla(
	               PWidget(wEditor), SCI_GETDIRECTFUNCTION, 0, 0));
	ptrEditor = Platform::SendScintilla(PWidget(wEditor),
	                                    SCI_GETDIRECTPOINTER, 0, 0);
	SendEditor(SCI_USEPOPUP, 0);
	gtk_fixed_put(GTK_FIXED(PWidget(wContent)), PWidget(wEditor), 0, 0);

	gtk_signal_connect(GTK_OBJECT(PWidget(wEditor)), "command",
	                   GtkSignalFunc(CommandSignal), this);
	gtk_signal_connect(GTK_OBJECT(PWidget(wEditor)), SCINTILLA_NOTIFY,
	                   GtkSignalFunc(NotifySignal), this);

	wDivider = gtk_drawing_area_new();
	gtk_signal_connect(GTK_OBJECT(PWidget(wDivider)), "expose_event",
	                   GtkSignalFunc(DividerExpose), this);
	gtk_signal_connect(GTK_OBJECT(PWidget(wDivider)), "motion_notify_event",
	                   GtkSignalFunc(DividerMotion), this);
	gtk_signal_connect(GTK_OBJECT(PWidget(wDivider)), "button_press_event",
	                   GtkSignalFunc(DividerPress), this);
	gtk_signal_connect(GTK_OBJECT(PWidget(wDivider)), "button_release_event",
	                   GtkSignalFunc(DividerRelease), this);
	gtk_widget_set_events(PWidget(wDivider),
	                      GDK_EXPOSURE_MASK
	                      | GDK_LEAVE_NOTIFY_MASK
	                      | GDK_BUTTON_PRESS_MASK
	                      | GDK_BUTTON_RELEASE_MASK
	                      | GDK_POINTER_MOTION_MASK
	                      | GDK_POINTER_MOTION_HINT_MASK
	                     );
	gtk_drawing_area_size(GTK_DRAWING_AREA(PWidget(wDivider)), (width == useDefault) ? 100 : width, 10);
	gtk_fixed_put(GTK_FIXED(PWidget(wContent)), PWidget(wDivider), 0, 600);

	wOutput = scintilla_new();
	scintilla_set_id(SCINTILLA(PWidget(wOutput)), IDM_RUNWIN);
	fnOutput = reinterpret_cast<SciFnDirect>(Platform::SendScintilla(
	               PWidget(wOutput), SCI_GETDIRECTFUNCTION, 0, 0));
	ptrOutput = Platform::SendScintilla(PWidget(wOutput),
	                                    SCI_GETDIRECTPOINTER, 0, 0);
	SendOutput(SCI_USEPOPUP, 0);
	gtk_fixed_put(GTK_FIXED(PWidget(wContent)), PWidget(wOutput), (width == useDefault) ? 100 : width, 0);
	gtk_signal_connect(GTK_OBJECT(PWidget(wOutput)), "command",
	                   GtkSignalFunc(CommandSignal), this);
	gtk_signal_connect(GTK_OBJECT(PWidget(wOutput)), SCINTILLA_NOTIFY,
	                   GtkSignalFunc(NotifySignal), this);

#ifdef INCREMENTAL_SEARCH
	GtkWidget *table = gtk_table_new(1, 2, FALSE);
	wIncrementPanel = table;
	gtk_box_pack_start(GTK_BOX(boxMain), table, FALSE, FALSE, 0);
	GtkAttachOptions opts = static_cast<GtkAttachOptions>(
	                            GTK_EXPAND | GTK_SHRINK | GTK_FILL);
	GtkWidget *label = TranslatedLabel("Search for:");
	gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, opts, opts, 5, 5);
	gtk_widget_show(label);

	IncSearchEntry = gtk_entry_new();
	gtk_table_attach(GTK_TABLE(table), IncSearchEntry, 1, 2, 0, 1, opts, opts, 5, 5);
	gtk_signal_connect(GTK_OBJECT(IncSearchEntry),"activate", GtkSignalFunc(FindIncrementCompleteSignal), this);
	gtk_signal_connect(GTK_OBJECT(IncSearchEntry), "key_press_event", GtkSignalFunc(FindIncrementEscapeSignal), this);
	gtk_signal_connect(GTK_OBJECT(IncSearchEntry),"changed", GtkSignalFunc(FindIncrementSignal), this);
	gtk_signal_connect(GTK_OBJECT(IncSearchEntry),"focus-out-event", GtkSignalFunc(FindIncrementCompleteSignal), this);

	gtk_widget_show(IncSearchEntry);
#endif

	SendOutput(SCI_SETMARGINWIDTHN, 1, 0);

	wStatusBar = gtk_statusbar_new();
	sbContextID = gtk_statusbar_get_context_id(
	                  GTK_STATUSBAR(PWidget(wStatusBar)), "global");
	gtk_box_pack_start(GTK_BOX(boxMain), PWidget(wStatusBar), FALSE, FALSE, 0);
	gtk_statusbar_push(GTK_STATUSBAR(PWidget(wStatusBar)), sbContextID, "Initial");
	sbVisible = false;

	static const GtkTargetEntry dragtypes[] = { { "text/uri-list", 0, 0 } };
	static const gint n_dragtypes = sizeof(dragtypes) / sizeof(dragtypes[0]);

	gtk_drag_dest_set(PWidget(wSciTE), GTK_DEST_DEFAULT_ALL, dragtypes,
	                  n_dragtypes, GDK_ACTION_COPY);
	(void)gtk_signal_connect(GTK_OBJECT(PWidget(wSciTE)), "drag_data_received",
	                         GTK_SIGNAL_FUNC(DragDataReceived), this);

	SetFocus(PWidget(wOutput));

	if ((left != useDefault) && (top != useDefault))
		gtk_widget_set_uposition(GTK_WIDGET(PWidget(wSciTE)), left, top);
	if ((width != useDefault) && (height != useDefault))
		gtk_window_set_default_size(GTK_WINDOW(PWidget(wSciTE)), width, height);
	gtk_widget_show_all(PWidget(wSciTE));
	SetIcon();

#if GTK_MAJOR_VERSION >= 2
	if (maximize)
		gtk_window_maximize(GTK_WINDOW(PWidget(wSciTE)));
#endif

	UIAvailable();
#ifdef INCREMENTAL_SEARCH
	gtk_widget_hide(wIncrementPanel);
#endif
}

void SciTEGTK::FindIncrementSignal(GtkWidget *entry, SciTEGTK *scitew) {
	const char *lineEntry = gtk_entry_get_text(GTK_ENTRY(entry));
	Platform::DebugPrintf("Output in console");

	SString ffLastWhat;
	ffLastWhat = scitew->findWhat;
	scitew->findWhat = lineEntry;
	scitew->wholeWord = false;
	if (scitew->findWhat != "") {
		scitew->FindNext(false, false);
		if ((!scitew->havefound) && (ffLastWhat.length() == scitew->findWhat.length()-1)) {
			scitew->findWhat = ffLastWhat;
			gtk_entry_set_text(GTK_ENTRY(scitew->IncSearchEntry), scitew->findWhat.c_str());
			gtk_editable_set_position(GTK_EDITABLE(scitew->IncSearchEntry), scitew->findWhat.length());
		}
	}
}

void SciTEGTK::FindIncrementEscapeSignal(GtkWidget *w, GdkEventKey *event, SciTEGTK *scitew) {
	if (event->keyval == GDK_Escape) {
		gtk_signal_emit_stop_by_name(GTK_OBJECT(w), "key_press_event");
		gtk_widget_hide(scitew->wIncrementPanel);
		SetFocus(PWidget(scitew->wEditor));
	}
}

void SciTEGTK::FindIncrementCompleteSignal(GtkWidget *, SciTEGTK *scitew) {
	gtk_widget_hide(scitew->wIncrementPanel);
	SetFocus(PWidget(scitew->wEditor));
}

void SciTEGTK::FindIncrement() {
	gtk_widget_show(wIncrementPanel);
	gtk_widget_grab_focus(GTK_WIDGET(IncSearchEntry));
	gtk_entry_set_text(GTK_ENTRY(IncSearchEntry), "");
}

void SciTEGTK::SetIcon() {
#if GTK_MAJOR_VERSION >= 2
	GdkPixbuf *icon_pix_buf = CreatePixbuf("Sci48M.png");
	if (icon_pix_buf) {
		gtk_window_set_icon(GTK_WINDOW(PWidget(wSciTE)), icon_pix_buf);
		gdk_pixbuf_unref(icon_pix_buf);
		return;
	}
#endif
	GtkStyle *style = gtk_widget_get_style(PWidget(wSciTE));
	GdkBitmap *mask;
	GdkPixmap *icon_pix = gdk_pixmap_create_from_xpm_d(
		PWidget(wSciTE)->window, &mask,
		&style->bg[GTK_STATE_NORMAL], (gchar **)SciIcon_xpm);
	gdk_window_set_icon(PWidget(wSciTE)->window, NULL, icon_pix, mask);
}

// Callback function that gets called when there is data to be read
// from the pipe.
void SciTEGTK::ReadPipe(gpointer data, gint source, GdkInputCondition condition) {

	// Shouldn't happen.  We're just looking for data to read.
	if (condition != GDK_INPUT_READ)
		return;

	int readLength;
	char pipeData[8192];
	SciTEGTK* scitew = reinterpret_cast<SciTEGTK *>(data);

	// Multiple filenames could be read in one read call.  They will be NULL
	// separated.  An empty string means the window should be brought forward.
	while ((readLength = read(source, pipeData, sizeof(pipeData))) > 0) {
		char *ii = pipeData;
		char *start = pipeData;
		char *end = pipeData + readLength;

		while (ii < end) {
			while ((ii < end) && (*ii != '\0'))
				++ii;

			if (strlen(start) > 0)
				scitew->Open(start);
#if GTK_MAJOR_VERSION >= 2
			else
				gtk_window_present(GTK_WINDOW(scitew->GetID()));
#endif
			start = ++ii;
		}
	}
}

// Send the filename through the pipe.  Make the path absolute if it is not
// already.  If filename is empty, one NULL character will be written.
// This signifies that the existing instance should present itself.
void SciTEGTK::SendFileName(int sendPipe, const char* filename) {

	// Create the command to send thru the pipe.
	char pipeData[CHAR_MAX];

	// Check to see if path is already absolute.  If it isn't then add the
	// absolute path to the front of the command to send.
	if (g_path_is_absolute(filename) || (strlen(filename) == 0)) {
		snprintf(pipeData, sizeof(pipeData) - 1, "%s", filename);
	} else {
		gchar *currentPath = g_get_current_dir();
		snprintf(pipeData, sizeof(pipeData) - 1, "%s/%s", currentPath, filename);
		g_free(currentPath);
	}

	// Send it.
	if (write(sendPipe, pipeData, strlen(pipeData) + 1) == -1)
		perror("Unable to write to pipe");
}

void SciTEGTK::CheckForRunningInstance(int argc, char *argv[]) {

	// Use ipc.scite.name for the pipe name if it exists.
	const SString pipeFilename = props.Get("ipc.scite.name");

	if (pipeFilename.length() > 0)
		snprintf(pipeName, sizeof(pipeName), "%s", pipeFilename.c_str());
	else
		snprintf(pipeName, sizeof(pipeName), "%s/.SciTE.%s.ipc", g_get_tmp_dir (), g_get_user_name());

	// Attempt to open the pipe as a writer to send out data.
	int sendPipe = open(pipeName, O_WRONLY | O_NONBLOCK);

	// If open succeeded, write filename data.
	if (sendPipe != -1) {
		for (int ii = 1; ii < argc; ++ii) {
			if (argv[ii][0] != '-')
				SendFileName(sendPipe, argv[ii]);
		}

		// Force the SciTE instance to come to the front.
		SendFileName(sendPipe, "");
		gtk_exit(0);
	}

	// If pipe doesn't exist, create it.  If pipe exists without a
	// reader, do nothing.  Return an error on any other condition.
	if (errno == ENOENT)
		MakePipe(pipeName);
	else if (errno != ENXIO) {
		perror("Unable to open pipe as writer");
		gtk_exit(0);
	}

	// Now open it as a reader to receive data.
	pipeFD = open(pipeName, O_RDWR | O_NONBLOCK);
	if (pipeFD == -1) {
		perror("Unable to open pipe as reader");
		unlink(pipeName);
		gtk_exit(0);
	}

	// Handler to read data.
	gdk_input_add(pipeFD, GDK_INPUT_READ, ReadPipe, this);
}

void SciTEGTK::Run(int argc, char *argv[]) {
	// Find the SciTE executable, first trying to use argv[0] and converting 
	// to an absolute path and if that fails, searching the path.
	sciteExecutable = FilePath(argv[0]).AbsolutePath();
	if (!sciteExecutable.Exists()) {
		gchar *progPath = g_find_program_in_path(argv[0]);
		sciteExecutable = FilePath(progPath);
		g_free(progPath);
	}

	// Collect the argv into one string with each argument separated by '\n'
	SString args;
	int arg;
	for (arg = 1; arg < argc; arg++) {
		args.appendwithseparator(argv[arg], '\n');
	}

	// Process any initial switches
	ProcessCommandLine(args, 0);

	// Check if SciTE is already running.  This could exit the program.
	if ((props.Get("ipc.director.name").length() == 0) && props.GetInt ("check.if.already.open"))
		CheckForRunningInstance (argc, argv);

	CreateUI();

	// Process remaining switches and files
	ProcessCommandLine(args, 1);

	CheckMenus();
	SizeSubWindows();
	SetFocus(PWidget(wEditor));
	gtk_widget_grab_focus(GTK_WIDGET(PWidget(wSciTE)));
	gtk_main();
}

// Avoid zombie detached processes by reaping their exit statuses when
// they are shut down.
void SciTEGTK::ChildSignal(int) {
	int status = 0;
	int pid = wait(&status);
	if (pid == instance->pidShell) {
		// If this child is the currently running tool, save the exit status
		instance->pidShell = 0;
		instance->triedKill = false;
		instance->exitStatus = status;
	}
}


// Detect if the tool has exited without producing any output
int SciTEGTK::PollTool(SciTEGTK *scitew) {
	scitew->ContinueExecute(TRUE);
	return TRUE;
}


int main(int argc, char *argv[]) {
#ifdef NO_EXTENSIONS
	Extension *extender = 0;
#else
	MultiplexExtension multiExtender;
	Extension *extender = &multiExtender;

#ifndef NO_LUA
	multiExtender.RegisterExtension(LuaExtension::Instance());
#endif
#ifndef NO_FILER
	multiExtender.RegisterExtension(DirectorExtension::Instance());
#endif
#endif

	signal(SIGCHLD, SciTEGTK::ChildSignal);

#ifdef __vms
	// Store the path part of the module name
	strcpy(g_modulePath, argv[0]);
	char *p = strstr(g_modulePath, "][");
	if (p != NULL) {
		strcpy (p, p + 2);
	}
	p = strchr(g_modulePath, ']');
	if (p == NULL) {
		p = strchr(g_modulePath, '>');
	}
	if (p == NULL) {
		p = strchr(g_modulePath, ':');
	}
	if (p != NULL) {
		*(p + 1) = '\0';
	}
	strcpy(g_modulePath, VMSToUnixStyle(g_modulePath));
	g_modulePath[strlen(g_modulePath) - 1] = '\0';  // remove trailing "/"
#endif

	gtk_set_locale();
	gtk_init(&argc, &argv);
	SciTEGTK scite(extender);
	scite.Run(argc, argv);

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
void SciTEGTK::CompileGLIShader()
{
  //DT_TODO:
}

