// SciTE - Scintilla based Text Editor
/** @file SciTEBase.h
 ** Definition of platform independent base class of editor.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <vector>

extern const char appName[];

extern const char propUserFileName[];
extern const char propGlobalFileName[];
extern const char propAbbrevFileName[];

extern const char menuAccessIndicator[];


#ifdef WIN32
#ifdef _MSC_VER
// Shut up level 4 warning:
// warning C4710: function 'void whatever(...)' not inlined
// warning C4800: forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable: 4710 4800)
#endif
#ifdef __DMC__
#include <time.h>
#endif
#endif

/**
 * The order of menus on Windows - the Buffers menu may not be present
 * and there is a Help menu at the end.
 */
enum { menuFile = 0, menuEdit = 1, menuSearch = 2, menuView = 3,
       menuTools = 4, menuOptions = 5, menuLanguage = 6, menuBuffers = 7,
       menuHelp = 8};

/**
 * This is a fixed length list of strings suitable for display in combo boxes
 * as a memory of user entries.
 */
template < int sz >
class EntryMemory {
	SString entries[sz];
public:
	void Insert(const SString &s) {
		for (int i = 0; i < sz; i++) {
			if (entries[i] == s) {
				for (int j = i; j > 0; j--) {
					entries[j] = entries[j - 1];
				}
				entries[0] = s;
				return;
			}
		}
		for (int k = sz - 1; k > 0; k--) {
			entries[k] = entries[k - 1];
		}
		entries[0] = s;
	}
	void AppendIfNotPresent(const SString &s) {
		for (int i = 0; i < sz; i++) {
			if (entries[i] == s) {
				return;
			}
			if (0 == entries[i].length()) {
				entries[i] = s;
				return;
			}
		}
	}
	void AppendList(const SString &s, char sep='|') {
		int start = 0;
		int end = 0;
		while (s[end] != '\0') {
			while ((s[end] != sep) && (s[end] != '\0'))
				++end;
			AppendIfNotPresent(SString(s.c_str(), start, end));
			start = end + 1;
			end = start;
		}
	}
	int Length() const {
		int len = 0;
		for (int i = 0; i < sz; i++)
			if (entries[i].length())
				len++;
		return len;
	}
	SString At(int n) const {
		return entries[n];
	}
};

class PropSetFile : public PropSet {
	bool lowerKeys;
public:
	PropSetFile(bool lowerKeys_=false);
	~PropSetFile();
	bool ReadLine(const char *data, bool ifIsTrue, FilePath directoryForImports, FilePath imports[] = 0, int sizeImports = 0);
	void ReadFromMemory(const char *data, int len, FilePath directoryForImports, FilePath imports[] = 0, int sizeImports = 0);
	bool Read(FilePath filename, FilePath directoryForImports, FilePath imports[] = 0, int sizeImports = 0);
	void SetInteger(const char *key, sptr_t i);
};

class RecentFile : public FilePath {
public:
	CharacterRange selection;
	int scrollPosition;
	RecentFile() {
		selection.cpMin = INVALID_POSITION;
		selection.cpMax = INVALID_POSITION;
		scrollPosition = 0;
	}
	void Init() {
		FilePath::Init();
		selection.cpMin = INVALID_POSITION;
		selection.cpMax = INVALID_POSITION;
		scrollPosition = 0;
	}
};

// Related to Utf8_16::encodingType but with additional values at end
enum UniMode {
	uni8Bit=0, uni16BE=1, uni16LE=2, uniUTF8=3,
	uniCookie=4
};

// State of folding in a given document, remembers line/state pairs,
// restore aborts when these pairs mismatch
class FoldState {
private:
	int *lines;
	bool *folded;
	int size;
	int fill;
	int readout;

	void CopyFrom(const FoldState& b) {
		Alloc(b.size);
		memcpy(lines, b.lines, size*sizeof(int));
		memcpy(folded, b.folded, size*sizeof(bool));
		fill = b.fill;

		readout = 0;
	}

public:
	FoldState() {
		lines = 0;
		folded = 0;
		size = 0;
		fill = 0;
		readout = -1;
	}

	FoldState &operator=(const FoldState &b) {
		if (this != &b) {
			CopyFrom(b);
		}
		return *this;
	}

	FoldState(const FoldState &b) {
		lines = 0;
		folded = 0;
		readout = -1;
		size = 0;
		fill = 0;

		CopyFrom(b);
	}

	void Alloc(int s) {
		//assert(s>0);
		//assert(size==0);

		delete []lines;
		lines = new int[s];
		delete []folded;
		folded = new bool[s];
		size = s;
		fill = 0;

		//assert(lines && folded && size>0);
	}

	void Clear() {

		delete []lines;
		lines = 0;
		delete []folded;
		folded = 0;

		size = 0;
		fill = 0;
		readout = -1;
	}

	virtual ~FoldState() {
		Clear();
	}

	void PushState(int line, bool folded_) {
		//assert(fill<size);
		lines[fill] = line;
		folded[fill] = folded_;
		fill++;
	}

	// returns maximum numbers a readout can succeed
	int BeginIteration() {
		// strong assertion
		//assert(fill==size);

		readout = 0;
		return fill;
	}

	// returns false at end of iteration
	// no results are written then
	bool GetState(int *pline, bool *pfold) {
		//assert(readout>0);
		if (readout >= fill) {
			readout = -1;
			return false;
		} else {
			if (pline && pfold) {
				*pline = lines[readout];
				*pfold = folded[readout];
			}
			readout++;
			return true;
		}
	}
};

class Buffer : public RecentFile {
public:
	sptr_t doc;
	bool isDirty;
	bool useMonoFont;
	UniMode unicodeMode;
	time_t fileModTime;
	SString overrideExtension;
	FoldState foldState;
	Buffer() :
		RecentFile(), doc(0), isDirty(false), useMonoFont(false),
		unicodeMode(uni8Bit), fileModTime(0), foldState() {
	}

	void Init() {
		RecentFile::Init();
		isDirty = false;
		useMonoFont = false;
		unicodeMode = uni8Bit;
		fileModTime = 0;
		overrideExtension = "";
		foldState.Clear();
	}
};

class BufferList {
protected:
	int current;
public:
	Buffer *buffers;
	int size;
	int length;
	BufferList();
	~BufferList();
	void Allocate(int maxSize);
	int Add();
	int GetDocumentByName(FilePath filename);
	void RemoveCurrent();
	int Current();
	void SetCurrent(int index);
};

enum JobSubsystem {
    jobCLI = 0, jobGUI = 1, jobShell = 2, jobExtension = 3, jobHelp = 4, jobOtherHelp = 5, jobGrep = 6};

enum JobFlags {
    jobForceQueue=1, jobHasInput=2, jobQuiet=4, // 8 reserved for jobVeryQuiet
    jobRepSelMask=48, jobRepSelYes=16, jobRepSelAuto=32,
	jobGroupUndo=64};

class Job {
public:
	SString command;
	FilePath directory;
	SString input;
	JobSubsystem jobType;
	int flags;

	Job();
	void Clear();
};

// class to hold user defined keyboard short cuts
class ShortcutItem {
public:
	SString menuKey; // the keyboard short cut
	SString menuCommand; // the menu command to be passed to "SciTEBase::MenuCommand"
};

class LanguageMenuItem {
public:
	SString menuItem;
	SString menuKey;
	SString extension;
};

/// Find the character following a name which is made up of character from
/// the set [a-zA-Z.]
char AfterName(const char *s);

typedef EntryMemory < 10 > ComboMemory;

enum {
	heightTools = 24,
	heightTab = 24,
	heightStatus = 20,
	statusPosWidth = 256
};

/// Warning IDs.
enum {
	warnFindWrapped = 1,
	warnNotFound,
	warnNoOtherBookmark,
	warnWrongFile,
	warnExecuteOK,
	warnExecuteKO
};

/// Codes representing the effect a line has on indentation.
enum IndentationStatus {
	isNone,		// no effect on indentation
	isBlockStart,	// indentation block begin such as "{" or VB "function"
	isBlockEnd,	// indentation end indicator such as "}" or VB "end"
	isKeyWordStart	// Keywords that cause indentation
};

int IntFromHexDigit(int ch);
int IntFromHexByte(const char *hexByte);

class StyleDefinition {
public:
	SString font;
	int size;
	SString fore;
	SString back;
	bool bold;
	bool italics;
	bool eolfilled;
	bool underlined;
	int caseForce;
	bool visible;
	bool changeable;
	enum flags { sdNone = 0, sdFont = 0x1, sdSize = 0x2, sdFore = 0x4, sdBack = 0x8,
	             sdBold = 0x10, sdItalics = 0x20, sdEOLFilled = 0x40, sdUnderlined = 0x80,
	              sdCaseForce = 0x100, sdVisible = 0x200, sdChangeable = 0x400} specified;
	StyleDefinition(const char *definition);
	bool ParseStyleDefinition(const char *definition);
	long ForeAsLong() const;
	long BackAsLong() const;
};

struct StyleAndWords {
	int styleNumber;
	SString words;
	bool IsEmpty() { return words.length() == 0; }
	bool IsSingleChar() { return words.length() == 1; }
};

#define SciTE_MARKER_BOOKMARK   1
#define SciTE_MARKER_BREAKPOINT 2
#define SciTE_MARKER_ARROW      3

class SciTEBase : public ExtensionAPI {

friend class GLIShaderData;
protected:
	SString windowName;
	FilePath filePath;
	FilePath dirNameAtExecute;
	FilePath dirNameForExecute;
	bool useMonoFont;
	time_t fileModTime;
	time_t fileModLastAsk;

	enum { fileStackMax = 10 };
	RecentFile recentFileStack[fileStackMax];
	enum { fileStackCmdID = IDM_MRUFILE, bufferCmdID = IDM_BUFFER };

	enum { importMax = 50 };
	FilePath importFiles[importMax];
	enum { importCmdID = IDM_IMPORT };

	SString findWhat;
	SString replaceWhat;
	Window wFindReplace;
	Window wFindIncrement;
	bool replacing;
	bool havefound;
	bool matchCase;
	bool wholeWord;
	bool reverseFind;
	bool regExp;
	bool wrapFind;
	bool unSlash;
	bool findInStyle;
	int findStyle;
	ComboMemory memFinds;
	ComboMemory memReplaces;
	ComboMemory memFiles;
	ComboMemory memDirectory;
	enum { maxParam = 4 };
	Window wParameters;
	SString parameterisedCommand;
	char abbrevInsert[200];

	enum { languageCmdID = IDM_LANGUAGE };
	LanguageMenuItem *languageMenu;
	int languageItems;

	// an array of short cut items that are defined by the user in the properties file.
	ShortcutItem *shortCutItemList; // array
	int shortCutItems; // length of array

	int codePage;
	int characterSet;
	UniMode unicodeMode;
	SString language;
	int lexLanguage;
	SString overrideExtension;	///< User has chosen to use a particular language
	WordList apis;
	SString apisFileNames;
	SString functionDefinition;

	bool indentOpening;
	bool indentClosing;
	bool indentMaintain;
	int statementLookback;
	StyleAndWords statementIndent;
	StyleAndWords statementEnd;
	StyleAndWords blockStart;
	StyleAndWords blockEnd;
	enum { noPPC, ppcStart, ppcMiddle, ppcEnd, ppcDummy };	///< Indicate the kind of preprocessor condition line
	char preprocessorSymbol;	///< Preprocessor symbol (in C: #)
	WordList preprocCondStart;	///< List of preprocessor conditional start keywords (in C: if ifdef ifndef)
	WordList preprocCondMiddle;	///< List of preprocessor conditional middle keywords (in C: else elif)
	WordList preprocCondEnd;	///< List of preprocessor conditional end keywords (in C: endif)

	Window wSciTE;  ///< Contains wToolBar, wTabBar, wContent, and wStatusBar
	Window wContent;    ///< Contains wEditor and wOutput
	Window wEditor;
	Window wOutput;
	Window wIncrement;
#if PLAT_GTK
	Window wDivider;	// Not used on Windows
#endif
	Window wToolBar;
	Window wStatusBar;
	Window wTabBar;
	Menu popup;
	SciFnDirect fnEditor;
	long ptrEditor;
	SciFnDirect fnOutput;
	long ptrOutput;
	bool tbVisible;
	bool tabVisible;
	bool tabHideOne; // Hide tab bar if one buffer is opened only
	bool tabMultiLine;
	bool sbVisible;	///< @c true if status bar is visible.
	SString sbValue;	///< Status bar text.
	int sbNum;	///< Number of the currenly displayed status bar information.
	int visHeightTools;
	int visHeightTab;
	int visHeightStatus;
	int visHeightEditor;
	int heightBar;
	// Prevent automatic load dialog appearing at the same time as
	// other dialogs as this can leads to reentry errors.
	int dialogsOnScreen;
	bool topMost;
	bool wrap;
	bool wrapOutput;
	int wrapStyle;
	bool isReadOnly;
	bool openFilesHere;
	bool fullScreen;
	enum { toolMax = 50 };
	Extension *extender;
	bool needReadProperties;

	int heightOutput;
	int heightOutputStartDrag;
	Point ptStartDrag;
	bool capturedMouse;
	int previousHeightOutput;
	bool firstPropertiesRead;
	bool localisationRead;
	bool splitVertical;	///< @c true if the split bar between editor and output is vertical.
	bool bufferedDraw;
	bool twoPhaseDraw;
	bool bracesCheck;
	bool bracesSloppy;
	int bracesStyle;
	int braceCount;

	bool indentationWSVisible;

	bool autoCompleteIgnoreCase;
	bool callTipIgnoreCase;
	bool autoCCausedByOnlyOne;
	SString calltipWordCharacters;
	SString calltipParametersStart;
	SString calltipParametersEnd;
	SString calltipParametersSeparators;
	SString calltipEndDefinition;
	SString autoCompleteStartCharacters;
	SString autoCompleteFillUpCharacters;
	SString wordCharacters;
	SString whitespaceCharacters;
	int startCalltipWord;
	int currentCallTip;
	int maxCallTips;
	SString currentCallTipWord;
	int lastPosCallTip;

	bool margin;
	int marginWidth;
	enum { marginWidthDefault = 20};

	bool foldMargin;
	int foldMarginWidth;
	enum { foldMarginWidthDefault = 14};

	bool lineNumbers;
	int lineNumbersWidth;
	enum { lineNumbersWidthDefault = 4 };
	bool lineNumbersExpand;

	bool usePalette;
	bool clearBeforeExecute;
	bool allowMenuActions;
	bool isDirty;
	bool isBuilding;
	bool isBuilt;
	bool executing;
	bool scrollOutput;
	bool returnOutputToCommand;
	enum { commandMax = 2 };
	int commandCurrent;
	Job jobQueue[commandMax];
	bool jobUsesOutputPane;
	long cancelFlag;
	bool timeCommands;

	bool macrosEnabled;
	SString currentMacro;
	bool recording;

	PropSetFile propsEmbed;
	PropSetFile propsBase;
	PropSetFile propsUser;
	PropSetFile propsLocal;
	PropSetFile props;

	PropSetFile propsAbbrev;

	PropSetFile propsUI;

	PropSetFile propsStatus;	// Not attached to a file but need SetInteger method.

	enum { bufferMax = 100 };
	BufferList buffers;

	// Handle buffers
	sptr_t GetDocumentAt(int index);
	int AddBuffer();
	void UpdateBuffersCurrent();
	bool IsBufferAvailable();
	bool CanMakeRoom(bool maySaveIfDirty=true);
	void SetDocumentAt(int index);
	void BuffersMenu();
	void Next();
	void Prev();

	void ReadGlobalPropFile();
	void ReadAbbrevPropFile();
	void ReadLocalPropFile();

	sptr_t SendEditor(unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0);
	sptr_t SendEditorString(unsigned int msg, uptr_t wParam, const char *s);
	sptr_t SendOutput(unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0);
	sptr_t SendOutputString(unsigned int msg, uptr_t wParam, const char *s);
	sptr_t SendFocused(unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0);
	sptr_t SendPane(int destination, unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0);
	sptr_t SendWindow(Window &w, unsigned int msg, uptr_t wParam=0, sptr_t lParam=0);
	void SendChildren(unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0);
	sptr_t SendOutputEx(unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0, bool direct = true);
	SString GetTranslationToAbout(const char * const propname, bool retainIfNotFound=true);
	int LengthDocument();
	int GetCaretInLine();
	void GetLine(char *text, int sizeText, int line=-1);
	SString GetLine(int line=-1);
	void GetRange(Window &win, int start, int end, char *text);
	int IsLinePreprocessorCondition(char *line);
	bool FindMatchingPreprocessorCondition(int &curLine, int direction, int condEnd1, int condEnd2);
	bool FindMatchingPreprocCondPosition(bool isForward, int &mppcAtCaret, int &mppcMatch);
	bool FindMatchingBracePosition(bool editor, int &braceAtCaret, int &braceOpposite, bool sloppy);
	void BraceMatch(bool editor);

	virtual void WarnUser(int warnID) = 0;
	void SetWindowName();
	void SetFileName(FilePath openName, bool fixCase = true);
	void ClearDocument();
	void InitialiseBuffers();
	void LoadRecentMenu();
	void SaveRecentStack();
	void LoadSession(const char *sessionName);
	void SaveSession(const char *sessionName);
	void SetIndentSettings();
	void SetEol();
	void New();
	void Close(bool updateUI = true, bool loadingSession = false, bool makingRoomForNew = false);
	bool IsAbsolutePath(const char *path);
	bool Exists(const char *dir, const char *path, FilePath *resultPath);
	void DiscoverEOLSetting();
	void DiscoverIndentSetting();
	void OpenFile(int fileSize, bool suppressMessage);
	virtual void OpenUriList(const char *) {};
	virtual bool OpenDialog(FilePath directory, const char *filter) = 0;
	virtual bool SaveAsDialog() = 0;
	virtual void LoadSessionDialog() { };
	virtual void SaveSessionDialog() { };
	void CountLineEnds(int &linesCR, int &linesLF, int &linesCRLF);
	enum OpenFlags {
		ofNone=0, 		// Default
		ofNoSaveIfDirty=1, 	// Suppress check for unsaved changes
		ofForceLoad=2,	// Reload file even if already in a buffer
		ofPreserveUndo=4,	// Do not delete undo history
		ofQuiet=8		// Avoid "Could not open file" message
	};
	virtual bool PreOpenCheck(const char *file);
	bool Open(FilePath file, OpenFlags of=ofNone);
	bool OpenSelected();
	void Revert();
	int SaveIfUnsure(bool forceQuestion = false);
	int SaveIfUnsureAll(bool forceQuestion = false);
	int SaveIfUnsureForBuilt();
	bool Save();
	void SaveAs(const char *file);
	virtual void SaveACopy() = 0;
	void SaveToHTML(FilePath saveName);
	void StripTrailingSpaces();
	void EnsureFinalNewLine();
	bool SaveBuffer(FilePath saveName);
	virtual void SaveAsHTML() = 0;
	void SaveToRTF(FilePath saveName, int start = 0, int end = -1);
	virtual void SaveAsRTF() = 0;
	void SaveToPDF(FilePath saveName);
	virtual void SaveAsPDF() = 0;
	void SaveToTEX(FilePath saveName);
	virtual void SaveAsTEX() = 0;
	void SaveToXML(FilePath saveName);
	virtual void SaveAsXML() = 0;
	virtual FilePath GetDefaultDirectory() = 0;
	virtual FilePath GetSciteDefaultHome() = 0;
	virtual FilePath GetSciteUserHome() = 0;
	FilePath GetDefaultPropertiesFileName();
	FilePath GetUserPropertiesFileName();
	FilePath GetAbbrevPropertiesFileName();
	void OpenProperties(int propsFile);
	int GetMenuCommandAsInt(SString commandName);
	virtual void Print(bool) {};
	virtual void PrintSetup() {};
	CharacterRange GetSelection();
	void SetSelection(int anchor, int currentPos);
	//	void SelectionExtend(char *sel, int len, char *notselchar);
	void GetCTag(char *sel, int len);
	virtual SString GetRangeInUIEncoding(Window &win, int selStart, int selEnd);
	SString RangeExtendAndGrab(Window &wCurrent, int &selStart, int &selEnd,
		bool (*ischarforsel)(char ch), bool stripEol = true);
	SString SelectionExtend(bool (*ischarforsel)(char ch), bool stripEol = true);
	void FindWordAtCaret(int &start, int &end);
	bool SelectWordAtCaret();
	SString SelectionWord(bool stripEol = true);
	SString SelectionFilename();
	void SelectionIntoProperties();
	void SelectionIntoFind(bool stripEol = true);
	virtual SString EncodeString(const SString &s);
	virtual void Find() = 0;
	virtual int WindowMessageBox(Window &w, const SString &m, int style)=0;
	void FindMessageBox(const SString &msg);
	int FindInTarget(const char *findWhat, int lenFind, int startPosition, int endPosition);
	int FindNext(bool reverseDirection, bool showWarnings = true);
	virtual void FindIncrement() = 0;
	int IncrementSearchMode();
	virtual void FindInFiles() = 0;
	virtual void Replace() = 0;
	void ReplaceOnce();
	int DoReplaceAll(bool inSelection); // returns number of replacements or negative value if error
	int ReplaceAll(bool inSelection);
	int ReplaceInBuffers();
	virtual void DestroyFindReplace() = 0;
	virtual void GoLineDialog() = 0;
	virtual bool AbbrevDialog() = 0;
	virtual void TabSizeDialog() = 0;
	virtual void ParamGrab() = 0;
	virtual bool ParametersDialog(bool modal) = 0;
	bool HandleXml(char ch);
	SString FindOpenXmlTag(const char sel[], int nSize);
	void GoMatchingBrace(bool select);
	void GoMatchingPreprocCond(int direction, bool select);
	virtual void FindReplace(bool replace) = 0;

	void OutputAppendStringSynchronised(const char *s, int len = -1, bool startLine = false);
	void MakeOutputVisible();
	void ClearJobQueue();
	virtual void Execute();
	virtual void StopExecute() = 0;
	void GoMessage(int dir);
	virtual bool StartCallTip();
	virtual void FillFunctionDefinition(int pos = -1);
	void ContinueCallTip();
	virtual void EliminateDuplicateWords(char *words);
	virtual bool StartAutoComplete();
	virtual bool StartAutoCompleteWord(bool onlyOneWord);
	virtual bool StartExpandAbbreviation();
	virtual bool StartInsertAbbreviation();
	virtual bool StartBlockComment();
	virtual bool StartBoxComment();
	virtual bool StartStreamComment();
	unsigned int GetLinePartsInStyle(int line, int style1, int style2, SString sv[], int len);
	void SetLineIndentation(int line, int indent);
	int GetLineIndentation(int line);
	int GetLineIndentPosition(int line);
	bool RangeIsAllWhitespace(int start, int end);
	IndentationStatus GetIndentState(int line);
	int IndentOfBlock(int line);
	void MaintainIndentation(char ch);
	void AutomaticIndentation(char ch);
	void CharAdded(char ch);
	void SetTextProperties(PropSetFile &ps);
	virtual void SetFileProperties(PropSet &ps) = 0;
	virtual void UpdateStatusBar(bool bUpdateSlowData);
	int GetLineLength(int line);
	int GetCurrentLineNumber();
	int GetCurrentScrollPosition();
	virtual void AddCommand(const SString &cmd, const SString &dir,
		JobSubsystem jobType, const SString &input = "",
		int flags = 0);
	virtual void AboutDialog() = 0;
	virtual void QuitProgram() = 0;
	void CloseTab(int tab);
	void CloseAllBuffers(bool loadingSession = false);
	int SaveAllBuffers(bool forceQuestion, bool alwaysYes=false);
	void SaveTitledBuffers();
	virtual void CopyAsRTF() {};
	void SetLineNumberWidth();
	void MenuCommand(int cmdID, int source=0);
	void FoldChanged(int line, int levelNow, int levelPrev);
	void FoldChanged(int position);
	void Expand(int &line, bool doExpand, bool force = false,
	            int visLevels = 0, int level = -1);
	void FoldAll();
	void ToggleFoldRecursive(int line, int level);
	void EnsureAllChildrenVisible(int line, int level);
	void EnsureRangeVisible(int posStart, int posEnd, bool enforcePolicy=true);
	void GotoLineEnsureVisible(int line);
	bool MarginClick(int position, int modifiers);
	void NewLineInOutput();
	virtual void SetStatusBarText(const char *s) = 0;
	virtual void Notify(SCNotification *notification);
	virtual void ShowToolBar() = 0;
	virtual void ShowTabBar() = 0;
	virtual void ShowStatusBar() = 0;

	int MarkAll();
	void BookmarkAdd(int lineno = -1);
	void BookmarkDelete(int lineno = -1);
	bool BookmarkPresent(int lineno = -1);
	void BookmarkToggle(int lineno = -1);
	void BookmarkNext(bool forwardScan = true, bool select = false);
	void ToggleOutputVisible();
	virtual void SizeContentWindows() = 0;
	virtual void SizeSubWindows() = 0;

	virtual void SetMenuItem(int menuNumber, int position, int itemID,
	                         const char *text, const char *mnemonic = 0) = 0;
	virtual void DestroyMenuItem(int menuNumber, int itemID) = 0;
	virtual void CheckAMenuItem(int wIDCheckItem, bool val) = 0;
	virtual void EnableAMenuItem(int wIDCheckItem, bool val) = 0;
	virtual void CheckMenusClipboard();
	virtual void CheckMenus();
	virtual void AddToPopUp(const char *label, int cmd=0, bool enabled=true)=0;
	void ContextMenu(Window wSource, Point pt, Window wCmd);

	void DeleteFileStackMenu();
	void SetFileStackMenu();
	void DropFileStackTop();
	void AddFileToBuffer(FilePath file, int pos);
	void AddFileToStack(FilePath file, CharacterRange selection, int scrollPos);
	void RemoveFileFromStack(FilePath file);
	RecentFile GetFilePosition();
	void DisplayAround(const RecentFile &rf);
	void StackMenu(int pos);
	void StackMenuNext();
	void StackMenuPrev();

	void RemoveToolsMenu();
	void SetMenuItemLocalised(int menuNumber, int position, int itemID,
		const char *text, const char *mnemonic);
	void SetToolsMenu();
	JobSubsystem SubsystemType(char c);
	JobSubsystem SubsystemType(const char *cmd, int item = -1);
	void ToolsMenu(int item);

	void AssignKey(int key, int mods, int cmd);
	void ViewWhitespace(bool view);
	void SetAboutMessage(WindowID wsci, const char *appTitle);
	void SetImportMenu();
	void ImportMenu(int pos);
	void SetLanguageMenu();
	void SetPropertiesInitial();
	SString LocaliseString(const char *s, bool retainIfNotFound=true);
	SString LocaliseMessage(const char *s, const char *param0=0,
		const char *param1=0, const char *param2=0);
	virtual void ReadLocalisation();
	virtual void ReadPropertiesInitial();
	void ReadFontProperties();
	void SetOverrideLanguage(int cmdID);
	StyleAndWords GetStyleAndWords(const char *base);
	SString ExtensionFileName();
	const char *GetNextPropItem(const char *pStart, char *pPropItem, int maxLen);
	void ForwardPropertyToEditor(const char *key);
	void DefineMarker(int marker, int markerType, ColourDesired fore, ColourDesired back);
	void ReadAPI(const SString &fileNameForExtension);
	SString FindLanguageProperty(const char *pattern, const char *defaultValue="");
	virtual void ReadProperties();
	void SetOneStyle(Window &win, int style, const StyleDefinition &sd);
	void SetStyleFor(Window &win, const char *language);
	void ReloadProperties();

	void CheckReload();
	void Activate(bool activeApp);
	PRectangle GetClientRectangle();
	void Redraw();
	int NormaliseSplit(int splitPos);
	void MoveSplit(Point ptNewDrag);

	void UIAvailable();
	void PerformOne(char *action);
	void StartRecordMacro();
	void StopRecordMacro();
	void StartPlayMacro();
	bool RecordMacroCommand(SCNotification *notification);
	void ExecuteMacroCommand(const char * command);
	void AskMacroList();
	bool StartMacroList(const char *words);
	void ContinueMacroList(const char *stxt);
	void LoadMRUAndSession(bool allowLoadSession);
	bool ProcessCommandLine(SString &args, int phase);
	virtual bool IsStdinBlocked();
	void OpenFromStdin(bool UseOutputPane);
	void OpenFilesFromStdin();
	void GrepRecursive(FilePath baseDir, const char *searchString, const char *fileTypes, bool forStdOut);
	void InternalGrep(bool forStdOut, const char *directory, const char *files, const char *search);
	void EnumProperties(const char *action);
	void SendOneProperty(const char *kind, const char *key, const char *val);
	void PropertyFromDirector(const char *arg);
	void PropertyToDirector(const char *arg);

	// ExtensionAPI
	sptr_t Send(Pane p, unsigned int msg, uptr_t wParam = 0, sptr_t lParam = 0);
	char *Range(Pane p, int start, int end);
	void Remove(Pane p, int start, int end);
	void Insert(Pane p, int pos, const char *s);
	void Trace(const char *s);

	void SetProperty(const char *key, const char *val);
	void UnsetProperty(const char *key);
	uptr_t GetInstance();
	void ShutDown();
	void Perform(const char *actions);

#ifdef GLI_CHANGES

  //GLIntercept new function
  virtual void CompileGLIShader() =0;
  virtual void ToggleBreakpoint(unsigned int line) =0;

  //@
  //  Summary:
  //    To save the breakpoint data for the current document.
  //  
  void SaveBreakPointData();

  //@
  //  Summary:
  //    To load the breakpoint data for the current document.
  //  
  void LoadBreakPointData();

protected:

  //Structure to cache breakpoints when the document is not open
  struct BreakPointCache
  {
    SString fileName;                             // The filename of the breakpoint data
    std::vector<int> lineNumbers;                 // The list of line numbers that have breakpoints on them
  };
  std::vector<BreakPointCache> breakPointCacheArray;// Array of breakpoints that are not in a current file


#endif //GLI_CHANGES

public:

	SciTEBase(Extension *ext = 0);
	virtual ~SciTEBase();

  //GLI - Raw open buffer
  bool OpenBuffer(const char *fileName, const char *buffer);

  //GLI - close buffer of sepcified name
  bool CloseBuffer(const char *fileName);

  //Made public
	void OutputAppendString(const char *s, int len = -1, bool startLine = false);
	char *Property(const char *key);

	void ProcessExecute();
	WindowID GetID() { return wSciTE.GetID(); }

private:
	// un-implemented copy-constructor and assignment operator
	SciTEBase(const SciTEBase&);
	void operator=(const SciTEBase&);
};

/// Base size of file I/O operations.
const int blockSize = 131072;

#if PLAT_GTK || PLAT_FOX
// MessageBox
#define MB_OK	(0L)
#define MB_YESNO	(0x4L)
#define MB_YESNOCANCEL	(0x3L)
#define MB_ICONWARNING	(0x30L)
#define MB_ICONQUESTION (0x20L)
#define IDOK	(1)
#define IDCANCEL	(2)
#define IDYES	(6)
#define IDNO	(7)
#endif

int ControlIDOfCommand(unsigned long);
void LowerCaseString(char *s);
long ColourOfProperty(PropSet &props, const char *key, ColourDesired colourDefault);
char *Slash(const char *s, bool quoteQuotes);
unsigned int UnSlash(char *s);
void WindowSetFocus(Window &w);
