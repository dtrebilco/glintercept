// SciTE - Scintilla based Text Editor
/** @file SciTEBase.cxx
 ** Platform independent base class of editor.
 **/
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <time.h>

#include "Platform.h"

#if PLAT_FOX

#include <unistd.h>

#endif

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
#include "ScintillaWidget.h"
#include "SciLexer.h"
#include "Extender.h"
#include "FilePath.h"
#include "SciTEBase.h"

// Contributor names are in UTF-8
const char *contributors[] = {
                                 "Atsuo Ishimoto",
                                 "Mark Hammond",
                                 "Francois Le Coguiec",
                                 "Dale Nagata",
                                 "Ralf Reinhardt",
                                 "Philippe Lhoste",
                                 "Andrew McKinlay",
                                 "Stephan R. A. Deibel",
                                 "Hans Eckardt",
                                 "Vassili Bourdo",
                                 "Maksim Lin",
                                 "Robin Dunn",
                                 "John Ehresman",
                                 "Steffen Goeldner",
                                 "Deepak S.",
                                 "DevelopMentor http://www.develop.com",
                                 "Yann Gaillard",
                                 "Aubin Paul",
                                 "Jason Diamond",
                                 "Ahmad Baitalmal",
                                 "Paul Winwood",
                                 "Maxim Baranov",
#if PLAT_GTK
                                 "Icons Copyright(C) 1998 by Dean S. Jones",
                                 "    http://jfa.javalobby.org/projects/icons/",
#endif
                                 "Ragnar H\xc3\xb8jland",
                                 "Christian Obrecht",
                                 "Andreas Neukoetter",
                                 "Adam Gates",
                                 "Steve Lhomme",
                                 "Ferdinand Prantl",
                                 "Jan Dries",
                                 "Markus Gritsch",
                                 "Tahir Karaca",
                                 "Ahmad Zawawi",
                                 "Laurent le Tynevez",
                                 "Walter Braeu",
                                 "Ashley Cambrell",
                                 "Garrett Serack",
                                 "Holger Schmidt",
                                 "ActiveState http://www.activestate.com",
                                 "James Larcombe",
                                 "Alexey Yutkin",
                                 "Jan Hercek",
                                 "Richard Pecl",
                                 "Edward K. Ream",
                                 "Valery Kondakoff",
                                 "Sm\xc3\xa1ri McCarthy",
                                 "Clemens Wyss",
                                 "Simon Steele",
                                 "Serge A. Baranov",
                                 "Xavier Nodet",
                                 "Willy Devaux",
                                 "David Clain",
                                 "Brendon Yenson",
                                 "Vamsi Potluru http://www.baanboard.com",
                                 "Praveen Ambekar",
                                 "Alan Knowles",
                                 "Kengo Jinno",
                                 "Valentin Valchev",
                                 "Marcos E. Wurzius",
                                 "Martin Alderson",
                                 "Robert Gustavsson",
                                 "Jos\xc3\xa9 Fonseca",
                                 "Holger Kiemes",
                                 "Francis Irving",
                                 "Scott Kirkwood",
                                 "Brian Quinlan",
                                 "Ubi",
                                 "Michael R. Duerig",
                                 "Deepak T",
                                 "Don Paul Beletsky",
                                 "Gerhard Kalab",
                                 "Olivier Dagenais",
                                 "Josh Wingstrom",
                                 "Bruce Dodson",
                                 "Sergey Koshcheyev",
                                 "Chuan-jian Shen",
                                 "Shane Caraveo",
                                 "Alexander Scripnik",
                                 "Ryan Christianson",
                                 "Martin Steffensen",
                                 "Jakub Vr\xc3\xa1na",
                                 "The Black Horus",
                                 "Bernd Kreuss",
                                 "Thomas Lauer",
                                 "Mike Lansdaal",
                                 "Yukihiro Nakai",
                                 "Jochen Tucht",
                                 "Greg Smith",
                                 "Steve Schoettler",
                                 "Mauritius Thinnes",
                                 "Darren Schroeder",
                                 "Pedro Guerreiro",
                                 "Steven te Brinke",
                                 "Dan Petitt",
                                 "Biswapesh Chattopadhyay",
                                 "Kein-Hong Man",
                                 "Patrizio Bekerle",
                                 "Nigel Hathaway",
                                 "Hrishikesh Desai",
                                 "Sergey Puljajev",
                                 "Mathias Rauen",
                                 "Angelo Mandato http://www.spaceblue.com",
                                 "Denis Sureau",
                                 "Kaspar Schiess",
                                 "Christoph H\xc3\xb6sler",
                                 "Jo\xc3\xa3o Paulo F Farias",
                                 "Ron Schofield",
                                 "Stefan Wosnik",
                                 "Marius Gheorghe",
                                 "Naba Kumar",
                                 "Sean O'Dell",
                                 "Stefanos Togoulidis",
                                 "Hans Hagen",
                                 "Jim Cape",
                                 "Roland Walter",
                                 "Brian Mosher",
                                 "Nicholas Nemtsev",
                                 "Roy Wood",
                                 "Peter-Henry Mander",
                                 "Robert Boucher",
                                 "Christoph Dalitz",
                                 "April White",
                                 "S. Umar",
                                 "Trent Mick",
                                 "Filip Yaghob",
                                 "Avi Yegudin",
                                 "Vivi Orunitia",
                                 "Manfred Becker",
                                 "Dimitris Keletsekis",
                                 "Yuiga",
                                 "Davide Scola",
                                 "Jason Boggs",
                                 "Reinhold Niesner",
                                 "Jos van der Zande",
                                 "Pescuma",
                                 "Pavol Bosik",
                                 "Johannes Schmid",
                                 "Blair McGlashan",
                                 "Mikael Hultgren",
                                 "Florian Balmer",
                                 "Hadar Raz",
                                 "Herr Pfarrer",
                                 "Ben Key",
                                 "Gene Barry",
                                 "Niki Spahiev",
                                 "Carsten Sperber",
                                 "Phil Reid",
                                 "Iago Rubio",
                                 "R\xc3\xa9gis Vaquette",
                                 "Massimo Cor\xc3\xa0",
                                 "Elias Pschernig",
                                 "Chris Jones",
                                 "Josiah Reynolds",
                                 "Robert Roessler http://www.rftp.com",
                                 "Steve Donovan",
                                 "Jan Martin Pettersen",
                                 "Sergey Philippov",
                                 "Borujoa",
                                 "Michael Owens",
                                 "Franck Marcia",
                                 "Massimo Maria Ghisalberti",
                                 "Frank Wunderlich",
                                 "Josepmaria Roca",
                                 "Tobias Engvall",
                                 "Suzumizaki Kimitaka",
                                 "Michael Cartmell",
                                 "Pascal Hurni",
                                 "Andre",
                                 "Randy Butler",
                                 "Georg Ritter",
                                 "Michael Goffioul",
                                 "Ben Harper",
                                 "Adam Strzelecki",
                                 "Kamen Stanev",
                                 "Steve Menard",
                             };

// AddStyledText only called from About so static size buffer is OK
void AddStyledText(WindowID hwnd, const char *s, int attr) {
	char buf[1000];
	size_t len = strlen(s);
	for (size_t i = 0; i < len; i++) {
		buf[i*2] = s[i];
		buf[i*2 + 1] = static_cast<char>(attr);
	}
	Platform::SendScintillaPointer(hwnd, SCI_ADDSTYLEDTEXT,
	                               static_cast<int>(len*2), const_cast<char *>(buf));
}

void SetAboutStyle(WindowID wsci, int style, ColourDesired fore) {
	Platform::SendScintilla(wsci, SCI_STYLESETFORE, style, fore.AsLong());
}

static void HackColour(int &n) {
	n += (rand() % 100) - 50;
	if (n > 0xE7)
		n = 0x60;
	if (n < 0)
		n = 0x80;
}

SciTEBase::SciTEBase(Extension *ext) : apis(true), extender(ext), propsUI(true) {

	codePage = 0;
	characterSet = 0;
	unicodeMode = uni8Bit; // Set to 'unknown'
	language = "java";
	lexLanguage = SCLEX_CPP;
	functionDefinition = 0;
	indentOpening = true;
	indentClosing = true;
	statementLookback = 10;

	fnEditor = 0;
	ptrEditor = 0;
	fnOutput = 0;
	ptrOutput = 0;
	tbVisible = false;
	sbVisible = false;
	tabVisible = false;
	tabHideOne = false;
	tabMultiLine = false;
	sbNum = 1;
	visHeightTools = 0;
	visHeightStatus = 0;
	visHeightEditor = 1;
	heightBar = 7;
	dialogsOnScreen = 0;
	topMost = false;
	wrap = false;
	wrapOutput = false;
	wrapStyle = SC_WRAP_WORD;
	isReadOnly = false;
	openFilesHere = false;
	fullScreen = false;

	heightOutput = 0;
	previousHeightOutput = 0;

	allowMenuActions = true;
	isDirty = false;
	isBuilding = false;
	isBuilt = false;
	executing = false;
	scrollOutput = true;
	returnOutputToCommand = true;
	commandCurrent = 0;
	jobUsesOutputPane = false;
	cancelFlag = 0L;

	ptStartDrag.x = 0;
	ptStartDrag.y = 0;
	capturedMouse = false;
	firstPropertiesRead = true;
	localisationRead = false;
	splitVertical = false;
	bufferedDraw = true;
	twoPhaseDraw = true;
	bracesCheck = true;
	bracesSloppy = false;
	bracesStyle = 0;
	braceCount = 0;

	indentationWSVisible = true;

	autoCompleteIgnoreCase = false;
	callTipIgnoreCase = false;
	autoCCausedByOnlyOne = false;
	startCalltipWord = 0;
	currentCallTip = 0;
	maxCallTips = 1;
	currentCallTipWord = "";
	lastPosCallTip = 0;

	margin = false;
	marginWidth = marginWidthDefault;
	foldMargin = true;
	foldMarginWidth = foldMarginWidthDefault;
	lineNumbers = false;
	lineNumbersWidth = lineNumbersWidthDefault;
	lineNumbersExpand = false;
	usePalette = false;

	clearBeforeExecute = false;
	replacing = false;
	havefound = false;
	matchCase = false;
	wholeWord = false;
	reverseFind = false;
	regExp = false;
	wrapFind = true;
	unSlash = false;
	findInStyle = false;
	findStyle = 0;

	languageMenu = 0;
	languageItems = 0;

	shortCutItemList = 0;
	shortCutItems = 0;

	useMonoFont = false;
	fileModTime = 0;
	fileModLastAsk = 0;

	macrosEnabled = false;
	recording = false;

	propsBase.superPS = &propsEmbed;
	propsUser.superPS = &propsBase;
	propsLocal.superPS = &propsUser;
	props.superPS = &propsLocal;

	propsStatus.superPS = &props;

	needReadProperties = false;
}

SciTEBase::~SciTEBase() {
	if (extender)
		extender->Finalise();
	delete []languageMenu;
	delete []shortCutItemList;
	popup.Destroy();
}

sptr_t SciTEBase::SendEditor(unsigned int msg, uptr_t wParam, sptr_t lParam) {
	return fnEditor(ptrEditor, msg, wParam, lParam);
}

sptr_t SciTEBase::SendEditorString(unsigned int msg, uptr_t wParam, const char *s) {
	return SendEditor(msg, wParam, reinterpret_cast<sptr_t>(s));
}

sptr_t SciTEBase::SendOutput(unsigned int msg, uptr_t wParam, sptr_t lParam) {
	return fnOutput(ptrOutput, msg, wParam, lParam);
}

sptr_t SciTEBase::SendOutputString(unsigned int msg, uptr_t wParam, const char *s) {
	return SendOutput(msg, wParam, reinterpret_cast<sptr_t>(s));
}

sptr_t SciTEBase::SendFocused(unsigned int msg, uptr_t wParam, sptr_t lParam) {
	if (wOutput.HasFocus())
		return SendOutput(msg, wParam, lParam);
	else
		return SendEditor(msg, wParam, lParam);
}

sptr_t SciTEBase::SendPane(int destination, unsigned int msg, uptr_t wParam, sptr_t lParam) {
	if (destination == IDM_SRCWIN)
		return SendEditor(msg, wParam, lParam);
	else if (destination == IDM_RUNWIN)
		return SendOutput(msg, wParam, lParam);
	else
		return SendFocused(msg, wParam, lParam);
}

sptr_t SciTEBase::SendWindow(Window &w, unsigned int msg, uptr_t wParam, sptr_t lParam) {
	if (w.GetID() == wOutput.GetID())
		return SendOutput(msg, wParam, lParam);
	else
		return SendEditor(msg, wParam, lParam);
}

void SciTEBase::SendChildren(unsigned int msg, uptr_t wParam, sptr_t lParam) {
	SendEditor(msg, wParam, lParam);
	SendOutput(msg, wParam, lParam);
}

sptr_t SciTEBase::SendOutputEx(unsigned int msg, uptr_t wParam /*= 0*/, sptr_t lParam /*= 0*/, bool direct /*= true*/) {
	if (direct)
		return SendOutput(msg, wParam, lParam);
	return Platform::SendScintilla(wOutput.GetID(), msg, wParam, lParam);
}

#if PLAT_WIN
static unsigned int UTF8Length(const wchar_t *uptr, unsigned int tlen) {
	unsigned int len = 0;
	for (unsigned int i = 0; i < tlen && uptr[i]; i++) {
		unsigned int uch = uptr[i];
		if (uch < 0x80)
			len++;
		else if (uch < 0x800)
			len += 2;
		else
			len +=3;
	}
	return len;
}

static void UTF8FromUCS2(const wchar_t *uptr, unsigned int tlen, char *putf, unsigned int len) {
	int k = 0;
	for (unsigned int i = 0; i < tlen && uptr[i]; i++) {
		unsigned int uch = uptr[i];
		if (uch < 0x80) {
			putf[k++] = static_cast<char>(uch);
		} else if (uch < 0x800) {
			putf[k++] = static_cast<char>(0xC0 | (uch >> 6));
			putf[k++] = static_cast<char>(0x80 | (uch & 0x3f));
		} else {
			putf[k++] = static_cast<char>(0xE0 | (uch >> 12));
			putf[k++] = static_cast<char>(0x80 | ((uch >> 6) & 0x3f));
			putf[k++] = static_cast<char>(0x80 | (uch & 0x3f));
		}
	}
	putf[len] = '\0';
}
#endif

SString SciTEBase::GetTranslationToAbout(const char * const propname, bool retainIfNotFound){
#if PLAT_WIN
	// By code below, all translators can write their name in their own
	// language in locale.properties on Windows.
	SString result = LocaliseString(propname, retainIfNotFound);
	if (!result.length())
		return result;
	int translationCodePage = props.GetInt("code.page", CP_ACP);
	int bufwSize = ::MultiByteToWideChar(translationCodePage, MB_PRECOMPOSED, result.c_str(), -1, NULL, 0);
	if (!bufwSize)
		return result;
	wchar_t *bufw = new wchar_t[bufwSize+1];
	bufwSize = ::MultiByteToWideChar(translationCodePage, MB_PRECOMPOSED, result.c_str(), -1, bufw, bufwSize);
	if(!bufwSize) {
		delete []bufw;
		return result;
	}
	int bufcSize = UTF8Length(bufw, bufwSize);
	if (!bufcSize)
		return result;
	char *bufc = new char[bufcSize+1];
	UTF8FromUCS2(bufw, bufwSize, bufc, bufcSize);
	delete []bufw;
	result = bufcSize ? bufc : "";
	delete []bufc;
	return result;
#else
	// On GTK+, LocaliseString always converts to UTF-8.
	return LocaliseString(propname, retainIfNotFound);
#endif
}

void SciTEBase::SetAboutMessage(WindowID wsci, const char *appTitle) {
	if (wsci) {
		Platform::SendScintilla(wsci, SCI_SETSTYLEBITS, 7, 0);
		Platform::SendScintilla(wsci, SCI_STYLERESETDEFAULT, 0, 0);
		int fontSize = 15;
#if PLAT_GTK
#if GTK_MAJOR_VERSION == 1
		// On GTK+ 1.x, try a font set that may allow unicode display
		Platform::SendScintilla(wsci, SCI_STYLESETFONT, STYLE_DEFAULT,
		                        reinterpret_cast<uptr_t>("misc-fixed-iso10646-1,*"));
#else
		Platform::SendScintilla(wsci, SCI_STYLESETFONT, STYLE_DEFAULT,
		                        reinterpret_cast<uptr_t>("!Serif"));
#endif
		fontSize = 14;
#endif

		Platform::SendScintilla(wsci, SCI_SETCODEPAGE, SC_CP_UTF8, 0);

		Platform::SendScintilla(wsci, SCI_STYLESETSIZE, STYLE_DEFAULT, fontSize);
		Platform::SendScintilla(wsci, SCI_STYLESETBACK, STYLE_DEFAULT, ColourDesired(0xff, 0xff, 0xff).AsLong());
		Platform::SendScintilla(wsci, SCI_STYLECLEARALL, 0, 0);

		SetAboutStyle(wsci, 0, ColourDesired(0xff, 0xff, 0xff));
		Platform::SendScintilla(wsci, SCI_STYLESETSIZE, 0, fontSize);
		Platform::SendScintilla(wsci, SCI_STYLESETBACK, 0, ColourDesired(0, 0, 0x80).AsLong());
		AddStyledText(wsci, appTitle, 0);
		AddStyledText(wsci, "\n", 0);
		SetAboutStyle(wsci, 1, ColourDesired(0, 0, 0));
		int trsSty = 5; // define the stylenumber to assign font for translators.
		SString translator = GetTranslationToAbout("TranslationCredit", false);
		SetAboutStyle(wsci, trsSty, ColourDesired(0, 0, 0));
#if PLAT_WIN
		// On Windows Me (maybe 9x also), we must assign another font to display translation.
		if (translator.length()) {
			SString fontBase = props.GetExpanded("font.translators");
			StyleDefinition sd(fontBase.c_str());
			if (sd.specified & StyleDefinition::sdFont) {
				Platform::SendScintilla(wsci, SCI_STYLESETFONT, trsSty,
							reinterpret_cast<uptr_t>(sd.font.c_str()));
			}
			if (sd.specified & StyleDefinition::sdSize) {
				Platform::SendScintilla(wsci, SCI_STYLESETSIZE, trsSty, sd.size);
			}
		}
#endif
		AddStyledText(wsci, GetTranslationToAbout("Version").c_str(), trsSty);
		AddStyledText(wsci, " 1.65\n", 1);
		AddStyledText(wsci, "    " __DATE__ " " __TIME__ "\n", 1);
		SetAboutStyle(wsci, 2, ColourDesired(0, 0, 0));
		Platform::SendScintilla(wsci, SCI_STYLESETITALIC, 2, 1);
		AddStyledText(wsci, GetTranslationToAbout("by").c_str(), trsSty);
		AddStyledText(wsci, " Neil Hodgson.\n", 2);
		SetAboutStyle(wsci, 3, ColourDesired(0, 0, 0));
		AddStyledText(wsci, "December 1998-August 2005.\n", 3);
		SetAboutStyle(wsci, 4, ColourDesired(0, 0x7f, 0x7f));
		AddStyledText(wsci, "http://www.scintilla.org\n", 4);
		AddStyledText(wsci, "\nGLIntercept plugin/updates\n by Damian Trebilco - Romans 12:2\n", 3);
		AddStyledText(wsci, "    http://glintercept.nutty.org\n\n", 4);
    AddStyledText(wsci, "Lua scripting language by TeCGraf, PUC-Rio\n", 3);
		AddStyledText(wsci, "    http://www.lua.org\n", 4);
		if (translator.length()) {
			AddStyledText(wsci, translator.c_str(), trsSty);
			AddStyledText(wsci, "\n", 5);
		}
		AddStyledText(wsci, GetTranslationToAbout("Contributors:").c_str(), trsSty);
		srand(static_cast<unsigned>(time(0)));
		for (unsigned int co = 0;co < (sizeof(contributors) / sizeof(contributors[0]));co++) {
			int colourIndex = 50 + (co % 78);
			AddStyledText(wsci, "\n    ", colourIndex);
			AddStyledText(wsci, contributors[co], colourIndex);
		}
		int r = rand() % 256;
		int g = rand() % 256;
		int b = rand() % 256;
		for (unsigned int sty = 0;sty < 78; sty++) {
			HackColour(r);
			HackColour(g);
			HackColour(b);
			SetAboutStyle(wsci, sty + 50, ColourDesired(r, g, b));
		}
		Platform::SendScintilla(wsci, SCI_SETREADONLY, 1, 0);
	}
}

void SciTEBase::ViewWhitespace(bool view) {
	if (view && indentationWSVisible)
		SendEditor(SCI_SETVIEWWS, SCWS_VISIBLEALWAYS);
	else if (view)
		SendEditor(SCI_SETVIEWWS, SCWS_VISIBLEAFTERINDENT);
	else
		SendEditor(SCI_SETVIEWWS, SCWS_INVISIBLE);
}

StyleAndWords SciTEBase::GetStyleAndWords(const char *base) {
	StyleAndWords sw;
	SString fileNameForExtension = ExtensionFileName();
	SString sAndW = props.GetNewExpand(base, fileNameForExtension.c_str());
	sw.styleNumber = sAndW.value();
	const char *space = strchr(sAndW.c_str(), ' ');
	if (space)
		sw.words = space + 1;
	return sw;
}

void SciTEBase::AssignKey(int key, int mods, int cmd) {
	SendEditor(SCI_ASSIGNCMDKEY,
	           Platform::LongFromTwoShorts(static_cast<short>(key),
	                                       static_cast<short>(mods)), cmd);
}

/**
 * Override the language of the current file with the one indicated by @a cmdID.
 * Mostly used to set a language on a file of unknown extension.
 */
void SciTEBase::SetOverrideLanguage(int cmdID) {
	RecentFile rf = GetFilePosition();
	EnsureRangeVisible(0, SendEditor(SCI_GETLENGTH), false);
	// Zero all the style bytes
	SendEditor(SCI_CLEARDOCUMENTSTYLE);

	overrideExtension = "x.";
	overrideExtension += languageMenu[cmdID].extension;
	ReadProperties();
	SetIndentSettings();
	SendEditor(SCI_COLOURISE, 0, -1);
	Redraw();
	DisplayAround(rf);
}

int SciTEBase::LengthDocument() {
	return SendEditor(SCI_GETLENGTH);
}

int SciTEBase::GetCaretInLine() {
	int caret = SendEditor(SCI_GETCURRENTPOS);
	int line = SendEditor(SCI_LINEFROMPOSITION, caret);
	int lineStart = SendEditor(SCI_POSITIONFROMLINE, line);
	return caret - lineStart;
}

void SciTEBase::GetLine(char *text, int sizeText, int line) {
	if (line < 0)
		line = GetCurrentLineNumber();
	int lineStart = SendEditor(SCI_POSITIONFROMLINE, line);
	int lineEnd = SendEditor(SCI_GETLINEENDPOSITION, line);
	int lineMax = lineStart + sizeText - 1;
	if (lineEnd > lineMax)
		lineEnd = lineMax;
	GetRange(wEditor, lineStart, lineEnd, text);
	text[lineEnd - lineStart] = '\0';
}

SString SciTEBase::GetLine(int line) {
	int len;
	// Get needed buffer size
	if (line < 0) {
		len = Platform::SendScintilla(wEditor.GetID(),
			SCI_GETCURLINE, 0, 0);
	} else {
		len = Platform::SendScintilla(wEditor.GetID(),
			SCI_GETLINE, line, 0);
	}
	// Allocate buffer
	SBuffer text(len);
	// And get the line
	if (line < 0) {
		Platform::SendScintillaPointer(wEditor.GetID(),
			SCI_GETCURLINE, len, text.ptr());
	} else {
		Platform::SendScintillaPointer(wEditor.GetID(),
			SCI_GETLINE, line, text.ptr());
	}
	return SString(text);
}

void SciTEBase::GetRange(Window &win, int start, int end, char *text) {
	TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = text;
	Platform::SendScintillaPointer(win.GetID(), SCI_GETTEXTRANGE, 0, &tr);
}

#ifdef OLD_CODE
void SciTEBase::Colourise(int start, int end, bool editor) {
	// Colourisation is now performed by the SciLexer DLL
	Window &win = editor ? wEditor : wOutput;
	int lengthDoc = Platform::SendScintilla(win.GetID(), SCI_GETLENGTH, 0, 0);
	if (end == -1)
		end = lengthDoc;
	int len = end - start;

	StylingContext styler(win.GetID(), props);

	int styleStart = 0;
	if (start > 0)
		styleStart = styler.StyleAt(start - 1);
	styler.SetCodePage(codePage);

	if (editor) {
		LexerModule::Colourise(start, len, styleStart, lexLanguage, keyWordLists, styler);
	} else {
		LexerModule::Colourise(start, len, 0, SCLEX_ERRORLIST, 0, styler);
	}
	styler.Flush();
}

#endif

/**
 * Check if the given line is a preprocessor condition line.
 * @return The kind of preprocessor condition (enum values).
 */
int SciTEBase::IsLinePreprocessorCondition(char *line) {
	char *currChar = line;
	char word[32];
	int i = 0;

	if (!currChar) {
		return false;
	}
	while (isspacechar(*currChar) && *currChar) {
		currChar++;
	}
	if (preprocessorSymbol && (*currChar == preprocessorSymbol)) {
		currChar++;
		while (isspacechar(*currChar) && *currChar) {
			currChar++;
		}
		while (!isspacechar(*currChar) && *currChar) {
			word[i++] = *currChar++;
		}
		word[i] = '\0';
		if (preprocCondStart.InList(word)) {
			return ppcStart;
		}
		if (preprocCondMiddle.InList(word)) {
			return ppcMiddle;
		}
		if (preprocCondEnd.InList(word)) {
			return ppcEnd;
		}
	}
	return noPPC;
}

/**
 * Search a matching preprocessor condition line.
 * @return @c true if the end condition are meet.
 * Also set curLine to the line where one of these conditions is mmet.
 */
bool SciTEBase::FindMatchingPreprocessorCondition(
    int &curLine,   		///< Number of the line where to start the search
    int direction,   		///< Direction of search: 1 = forward, -1 = backward
    int condEnd1,   		///< First status of line for which the search is OK
    int condEnd2) {		///< Second one

	bool isInside = false;
	char line[800];	// No need for full line
	int status, level = 0;
	int maxLines = SendEditor(SCI_GETLINECOUNT)-1;

	while (curLine < maxLines && curLine > 0 && !isInside) {
		curLine += direction;	// Increment or decrement
		GetLine(line, sizeof(line), curLine);
		status = IsLinePreprocessorCondition(line);

		if ((direction == 1 && status == ppcStart) || (direction == -1 && status == ppcEnd)) {
			level++;
		} else if (level > 0 && ((direction == 1 && status == ppcEnd) || (direction == -1 && status == ppcStart))) {
			level--;
		} else if (level == 0 && (status == condEnd1 || status == condEnd2)) {
			isInside = true;
		}
	}

	return isInside;
}

/**
 * Find if there is a preprocessor condition after or before the caret position,
 * @return @c true if inside a preprocessor condition.
 */
#ifdef __BORLANDC__
// Borland warns that isInside is assigned a value that is never used in this method.
// This is OK so turn off the warning just for this method.
#pragma warn -aus
#endif
bool SciTEBase::FindMatchingPreprocCondPosition(
    bool isForward,   		///< @c true if search forward
    int &mppcAtCaret,   	///< Matching preproc. cond.: current position of caret
    int &mppcMatch) {		///< Matching preproc. cond.: matching position

	bool isInside = false;
	int curLine;
	char line[800];	// Probably no need to get more characters, even if the line is longer, unless very strange layout...
	int status;

	// Get current line
	curLine = SendEditor(SCI_LINEFROMPOSITION, mppcAtCaret);
	GetLine(line, sizeof(line), curLine);
	status = IsLinePreprocessorCondition(line);

	switch (status) {
	case ppcStart:
		if (isForward) {
			isInside = FindMatchingPreprocessorCondition(curLine, 1, ppcMiddle, ppcEnd);
		} else {
			mppcMatch = mppcAtCaret;
			return true;
		}
		break;
	case ppcMiddle:
		if (isForward) {
			isInside = FindMatchingPreprocessorCondition(curLine, 1, ppcMiddle, ppcEnd);
		} else {
			isInside = FindMatchingPreprocessorCondition(curLine, -1, ppcStart, ppcMiddle);
		}
		break;
	case ppcEnd:
		if (isForward) {
			mppcMatch = mppcAtCaret;
			return true;
		} else {
			isInside = FindMatchingPreprocessorCondition(curLine, -1, ppcStart, ppcMiddle);
		}
		break;
	default:   	// Should be noPPC

		if (isForward) {
			isInside = FindMatchingPreprocessorCondition(curLine, 1, ppcMiddle, ppcEnd);
		} else {
			isInside = FindMatchingPreprocessorCondition(curLine, -1, ppcStart, ppcMiddle);
		}
		break;
	}

	if (isInside) {
		mppcMatch = SendEditor(SCI_POSITIONFROMLINE, curLine);
	}
	return isInside;
}
#ifdef __BORLANDC__
#pragma warn .aus
#endif

static bool IsBrace(char ch) {
	return ch == '[' || ch == ']' || ch == '(' || ch == ')' || ch == '{' || ch == '}';
}

/**
 * Find if there is a brace next to the caret, checking before caret first, then
 * after caret. If brace found also find its matching brace.
 * @return @c true if inside a bracket pair.
 */
bool SciTEBase::FindMatchingBracePosition(bool editor, int &braceAtCaret, int &braceOpposite, bool sloppy) {
	bool isInside = false;
	Window &win = editor ? wEditor : wOutput;
	int bracesStyleCheck = editor ? bracesStyle : 0;
	int caretPos = Platform::SendScintilla(win.GetID(), SCI_GETCURRENTPOS, 0, 0);
	braceAtCaret = -1;
	braceOpposite = -1;
	char charBefore = '\0';
	char styleBefore = '\0';
	int lengthDoc = Platform::SendScintilla(win.GetID(), SCI_GETLENGTH, 0, 0);
	WindowAccessor acc(win.GetID(), props);
	if ((lengthDoc > 0) && (caretPos > 0)) {
		// Check to ensure not matching brace that is part of a multibyte character
		if (Platform::SendScintilla(win.GetID(), SCI_POSITIONBEFORE, caretPos) == (caretPos - 1)) {
			charBefore = acc[caretPos - 1];
			styleBefore = static_cast<char>(acc.StyleAt(caretPos - 1) & 31);
		}
	}
	// Priority goes to character before caret
	if (charBefore && IsBrace(charBefore) &&
	        ((styleBefore == bracesStyleCheck) || (!bracesStyle))) {
		braceAtCaret = caretPos - 1;
	}
	bool colonMode = false;
	if ((lexLanguage == SCLEX_PYTHON) &&
	        (':' == charBefore) && (SCE_P_OPERATOR == styleBefore)) {
		braceAtCaret = caretPos - 1;
		colonMode = true;
	}
	bool isAfter = true;
	if (lengthDoc > 0 && sloppy && (braceAtCaret < 0) && (caretPos < lengthDoc)) {
		// No brace found so check other side
		// Check to ensure not matching brace that is part of a multibyte character
		if (Platform::SendScintilla(win.GetID(), SCI_POSITIONAFTER, caretPos) == (caretPos + 1)) {
			char charAfter = acc[caretPos];
			char styleAfter = static_cast<char>(acc.StyleAt(caretPos) & 31);
			if (charAfter && IsBrace(charAfter) && (styleAfter == bracesStyleCheck)) {
				braceAtCaret = caretPos;
				isAfter = false;
			}
			if ((lexLanguage == SCLEX_PYTHON) &&
			        (':' == charAfter) && (SCE_P_OPERATOR == styleAfter)) {
				braceAtCaret = caretPos;
				colonMode = true;
			}
		}
	}
	if (braceAtCaret >= 0) {
		if (colonMode) {
			int lineStart = Platform::SendScintilla(win.GetID(), SCI_LINEFROMPOSITION, braceAtCaret);
			int lineMaxSubord = Platform::SendScintilla(win.GetID(), SCI_GETLASTCHILD, lineStart, -1);
			braceOpposite = Platform::SendScintilla(win.GetID(), SCI_GETLINEENDPOSITION, lineMaxSubord);
		} else {
			braceOpposite = Platform::SendScintilla(win.GetID(), SCI_BRACEMATCH, braceAtCaret, 0);
		}
		if (braceOpposite > braceAtCaret) {
			isInside = isAfter;
		} else {
			isInside = !isAfter;
		}
	}
	return isInside;
}

void SciTEBase::BraceMatch(bool editor) {
	if (!bracesCheck)
		return;
	int braceAtCaret = -1;
	int braceOpposite = -1;
	FindMatchingBracePosition(editor, braceAtCaret, braceOpposite, bracesSloppy);
	Window &win = editor ? wEditor : wOutput;
	if ((braceAtCaret != -1) && (braceOpposite == -1)) {
		Platform::SendScintilla(win.GetID(), SCI_BRACEBADLIGHT, braceAtCaret, 0);
		SendEditor(SCI_SETHIGHLIGHTGUIDE, 0);
	} else {
		char chBrace = static_cast<char>(Platform::SendScintilla(
		                                     win.GetID(), SCI_GETCHARAT, braceAtCaret, 0));
		Platform::SendScintilla(win.GetID(), SCI_BRACEHIGHLIGHT, braceAtCaret, braceOpposite);
		int columnAtCaret = Platform::SendScintilla(win.GetID(), SCI_GETCOLUMN, braceAtCaret, 0);
		int columnOpposite = Platform::SendScintilla(win.GetID(), SCI_GETCOLUMN, braceOpposite, 0);
		if (chBrace == ':') {
			int lineStart = Platform::SendScintilla(win.GetID(), SCI_LINEFROMPOSITION, braceAtCaret);
			int indentPos = Platform::SendScintilla(win.GetID(), SCI_GETLINEINDENTPOSITION, lineStart, 0);
			int indentPosNext = Platform::SendScintilla(win.GetID(), SCI_GETLINEINDENTPOSITION, lineStart + 1, 0);
			columnAtCaret = Platform::SendScintilla(win.GetID(), SCI_GETCOLUMN, indentPos, 0);
			int columnAtCaretNext = Platform::SendScintilla(win.GetID(), SCI_GETCOLUMN, indentPosNext, 0);
			int indentSize = Platform::SendScintilla(win.GetID(), SCI_GETINDENT);
			if (columnAtCaretNext - indentSize > 1)
				columnAtCaret = columnAtCaretNext - indentSize;
			//Platform::DebugPrintf(": %d %d %d\n", lineStart, indentPos, columnAtCaret);
			if (columnOpposite == 0)	// If the final line of the structure is empty
				columnOpposite = columnAtCaret;
		}

		if (props.GetInt("highlight.indentation.guides"))
			Platform::SendScintilla(win.GetID(), SCI_SETHIGHLIGHTGUIDE, Platform::Minimum(columnAtCaret, columnOpposite), 0);
	}
}

void SciTEBase::SetWindowName() {
	if (filePath.IsUntitled()) {
		windowName = LocaliseString("Untitled");
		windowName.insert(0, "(");
		windowName += ")";
	} else if (props.GetInt("title.full.path") == 2) {
		windowName = filePath.Name().AsInternal();
		windowName += " in ";
		windowName += filePath.Directory().AsInternal();
	} else if (props.GetInt("title.full.path") == 1) {
		windowName = filePath.AsInternal();
	} else {
		windowName = filePath.Name().AsInternal();
	}
	if (isDirty)
		windowName += " * ";
	else
		windowName += " - ";
	windowName += appName;

	if (buffers.length > 1 && props.GetInt("title.show.buffers")) {
		windowName += " [";
		windowName += SString(buffers.Current() + 1);
		windowName += " of ";
		windowName += SString(buffers.length);
		windowName += "]";
	}

	wSciTE.SetTitle(windowName.c_str());
}

CharacterRange SciTEBase::GetSelection() {
	CharacterRange crange;
	crange.cpMin = SendEditor(SCI_GETSELECTIONSTART);
	crange.cpMax = SendEditor(SCI_GETSELECTIONEND);
	return crange;
}

void SciTEBase::SetSelection(int anchor, int currentPos) {
	SendEditor(SCI_SETSEL, anchor, currentPos);
}

void SciTEBase::GetCTag(char *sel, int len) {
	int lengthDoc, selStart, selEnd;
	int mustStop = 0;
	char c;
	Window wCurrent;

	if (wEditor.HasFocus()) {
		wCurrent = wEditor;
	} else {
		wCurrent = wOutput;
	}
	lengthDoc = SendFocused(SCI_GETLENGTH);
	selStart = selEnd = SendFocused(SCI_GETSELECTIONEND);
	WindowAccessor acc(wCurrent.GetID(), props);
	while (!mustStop) {
		if (selStart < lengthDoc - 1) {
			selStart++;
			c = acc[selStart];
			if (c == '\r' || c == '\n') {
				mustStop = -1;
			} else if (c == '\t' && ((acc[selStart + 1] == '/' && acc[selStart + 2] == '^') || isdigit(acc[selStart + 1]))) {
				mustStop = 1;
			}
		} else {
			mustStop = -1;
		}
	}
	if (mustStop == 1 && (acc[selStart + 1] == '/' && acc[selStart + 2] == '^')) {	// Found
		selEnd = selStart += 3;
		mustStop = 0;
		while (!mustStop) {
			if (selEnd < lengthDoc - 1) {
				selEnd++;
				c = acc[selEnd];
				if (c == '\r' || c == '\n') {
					mustStop = -1;
				} else if (c == '$' && acc[selEnd + 1] == '/') {
					mustStop = 1;	// Found!
				}

			} else {
				mustStop = -1;
			}
		}
	} else if (mustStop == 1 && isdigit(acc[selStart + 1])) {
		// a Tag can be referenced by line Number also
		selEnd = selStart += 1;
		while (isdigit(acc[selEnd]) && (selEnd < lengthDoc)) {
			selEnd++;
		}
	}

	sel[0] = '\0';
	if ((selStart < selEnd) && ((selEnd - selStart + 1) < len)) {
		GetRange(wCurrent, selStart, selEnd, sel);
	}
}

// Should also use word.characters.*, if exists, in the opposite way (in set instead of not in set)
static bool iswordcharforsel(char ch) {
	return !strchr("\t\n\r !\"#$%&'()*+,-./:;<=>?@[\\]^`{|}~", ch);
}

// Accept slighly more characters than for a word
// Doesn't accept all valid characters, as they are rarely used in source filenames...
// Accept path separators '/' and '\', extension separator '.', and ':', MS drive unit
// separator, and also used for separating the line number for grep. Same for '(' and ')' for cl.
static bool isfilenamecharforsel(char ch) {
	return !strchr("\t\n\r \"$%'*,;<>?[]^`{|}", ch);
}

SString SciTEBase::GetRangeInUIEncoding(Window &win, int selStart, int selEnd) {
	SBuffer sel(selEnd - selStart);
	GetRange(win, selStart, selEnd, sel.ptr());
	return SString(sel);
}

SString SciTEBase::RangeExtendAndGrab(
    Window &wCurrent,
    int &selStart,
    int &selEnd,
    bool (*ischarforsel)(char ch),	///< Function returning @c true if the given char. is part of the selection.
	bool stripEol /*=true*/) {

	if (selStart == selEnd && ischarforsel) {
		// Empty range and have a function to extend it
		int lengthDoc = SendWindow(wCurrent, SCI_GETLENGTH);
		WindowAccessor acc(wCurrent.GetID(), props);
		// Try and find a word at the caret
		// On the left...
		while ((selStart > 0) && (ischarforsel(acc[selStart - 1]))) {
			selStart--;
		}
		// and on the right
		while ((selEnd < lengthDoc) && (ischarforsel(acc[selEnd]))) {
			selEnd++;
		}
	}
	SString selected;
	if (selStart != selEnd) {
		selected = GetRangeInUIEncoding(wCurrent, selStart, selEnd);
	}
	if (stripEol) {
		// Change whole line selected but normally end of line characters not wanted.
		// Remove possible terminating \r, \n, or \r\n.
		size_t sellen = selected.length();
		if (sellen >= 2 && (selected[sellen - 2] == '\r' && selected[sellen - 1] == '\n')) {
			selected.remove(sellen - 2, 0);
		} else if (sellen >= 1 && (selected[sellen - 1] == '\r' || selected[sellen - 1] == '\n')) {
			selected.remove(sellen - 1, 0);
		}
	}

	return selected;
}

/**
 * If there is selected text, either in the editor or the output pane,
 * put the selection in the @a sel buffer, up to @a len characters.
 * Otherwise, try and select characters around the caret, as long as they are OK
 * for the @a ischarforsel function.
 * Remove the last two character controls from the result, as they are likely
 * to be CR and/or LF.
 */
SString SciTEBase::SelectionExtend(
    bool (*ischarforsel)(char ch),	///< Function returning @c true if the given char. is part of the selection.
	bool stripEol /*=true*/) {

	Window wCurrent;

	if (wOutput.HasFocus()) {
		wCurrent = wOutput;
	} else {
		wCurrent = wEditor;
	}
	int selStart = SendFocused(SCI_GETSELECTIONSTART);
	int selEnd = SendFocused(SCI_GETSELECTIONEND);
	return RangeExtendAndGrab(wCurrent, selStart, selEnd, ischarforsel, stripEol);
}

void SciTEBase::FindWordAtCaret(int &start, int &end) {
	Window wCurrent;

	if (wOutput.HasFocus()) {
		wCurrent = wOutput;
	} else {
		wCurrent = wEditor;
	}
	start = SendFocused(SCI_GETSELECTIONSTART);
	end = SendFocused(SCI_GETSELECTIONEND);
	// Call just to update start & end
	RangeExtendAndGrab(wCurrent, start, end, iswordcharforsel, false);
}

bool SciTEBase::SelectWordAtCaret() {
	int selStart = 0;
	int selEnd = 0;
	FindWordAtCaret(selStart, selEnd);
	SetSelection(selStart, selEnd);
	return selStart != selEnd;
}

SString SciTEBase::SelectionWord(bool stripEol /*=true*/) {
	return SelectionExtend(iswordcharforsel, stripEol);
}

SString SciTEBase::SelectionFilename() {
	return SelectionExtend(isfilenamecharforsel);
}

void SciTEBase::SelectionIntoProperties() {
	SString currentSelection = SelectionExtend(0, false);
	props.Set("CurrentSelection", currentSelection.c_str());

	SString word = SelectionWord();
	props.Set("CurrentWord", word.c_str());

	int selStart = SendFocused(SCI_GETSELECTIONSTART);
	int selEnd = SendFocused(SCI_GETSELECTIONEND);
	props.SetInteger("SelectionStartLine", SendFocused(SCI_LINEFROMPOSITION, selStart) + 1);
	props.SetInteger("SelectionStartColumn", SendFocused(SCI_GETCOLUMN, selStart) + 1);
	props.SetInteger("SelectionEndLine", SendFocused(SCI_LINEFROMPOSITION, selEnd) + 1);
	props.SetInteger("SelectionEndColumn", SendFocused(SCI_GETCOLUMN, selEnd) + 1);
}

void SciTEBase::SelectionIntoFind(bool stripEol /*=true*/) {
	SString sel = SelectionWord(stripEol);
	if (sel.length() && !sel.contains('\r') && !sel.contains('\n')) {
		// The selection does not include a new line, so is likely to be
		// the expression to search...
		findWhat = sel;
		if (unSlash) {
			char *slashedFind = Slash(findWhat.c_str(), false);
			if (slashedFind) {
				findWhat = slashedFind;
				delete []slashedFind;
			}
		}
	}
	// else findWhat remains the same as last time.
}

SString SciTEBase::EncodeString(const SString &s) {
	return SString(s);
}

void SciTEBase::FindMessageBox(const SString &msg) {
#if PLAT_GTK || PLAT_FOX
	WindowMessageBox(wSciTE, msg, MB_OK | MB_ICONWARNING);
#endif
#if PLAT_WIN
	WindowMessageBox(wFindReplace, msg, MB_OK | MB_ICONWARNING);
#endif
}

/**
 * Convert a string into C string literal form using \a, \b, \f, \n, \r, \t, \v, and \ooo.
 * The return value is a newly allocated character array containing the result.
 * 4 bytes are allocated for each byte of the input because that is the maximum
 * expansion needed when all of the input needs to be output using the octal form.
 * The return value should be deleted with delete[].
 */
char *Slash(const char *s, bool quoteQuotes) {
	char *oRet = new char[strlen(s) * 4 + 1];
	if (oRet) {
		char *o = oRet;
		while (*s) {
			if (*s == '\a') {
				*o++ = '\\';
				*o++ = 'a';
			} else if (*s == '\b') {
				*o++ = '\\';
				*o++ = 'b';
			} else if (*s == '\f') {
				*o++ = '\\';
				*o++ = 'f';
			} else if (*s == '\n') {
				*o++ = '\\';
				*o++ = 'n';
			} else if (*s == '\r') {
				*o++ = '\\';
				*o++ = 'r';
			} else if (*s == '\t') {
				*o++ = '\\';
				*o++ = 't';
			} else if (*s == '\v') {
				*o++ = '\\';
				*o++ = 'v';
			} else if (*s == '\\') {
				*o++ = '\\';
				*o++ = '\\';
			} else if (quoteQuotes && (*s == '\'')) {
				*o++ = '\\';
				*o++ = '\'';
			} else if (quoteQuotes && (*s == '\"')) {
				*o++ = '\\';
				*o++ = '\"';
			} else if (isascii(*s) && (*s < ' ')) {
				*o++ = '\\';
				*o++ = static_cast<char>((*s >> 6) + '0');
				*o++ = static_cast<char>((*s >> 3) + '0');
				*o++ = static_cast<char>((*s & 0x7) + '0');
			} else {
				*o++ = *s;
			}
			s++;
		}
		*o = '\0';
	}
	return oRet;
}

/**
 * Is the character an octal digit?
 */
static bool IsOctalDigit(char ch) {
	return ch >= '0' && ch <= '7';
}

/**
 * If the character is an hexa digit, get its value.
 */
static int GetHexaDigit(char ch) {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	}
	if (ch >= 'A' && ch <= 'F') {
		return ch - 'A' + 10;
	}
	if (ch >= 'a' && ch <= 'f') {
		return ch - 'a' + 10;
	}
	return -1;
}

/**
 * Convert C style \a, \b, \f, \n, \r, \t, \v, \ooo and \xhh into their indicated characters.
 */
unsigned int UnSlash(char *s) {
	char *sStart = s;
	char *o = s;

	while (*s) {
		if (*s == '\\') {
			s++;
			if (*s == 'a') {
				*o = '\a';
			} else if (*s == 'b') {
				*o = '\b';
			} else if (*s == 'f') {
				*o = '\f';
			} else if (*s == 'n') {
				*o = '\n';
			} else if (*s == 'r') {
				*o = '\r';
			} else if (*s == 't') {
				*o = '\t';
			} else if (*s == 'v') {
				*o = '\v';
			} else if (IsOctalDigit(*s)) {
				int val = *s - '0';
				if (IsOctalDigit(*(s + 1))) {
					s++;
					val *= 8;
					val += *s - '0';
					if (IsOctalDigit(*(s + 1))) {
						s++;
						val *= 8;
						val += *s - '0';
					}
				}
				*o = static_cast<char>(val);
			} else if (*s == 'x') {
				s++;
				int val = 0;
				int ghd = GetHexaDigit(*s);
				if (ghd >= 0) {
					s++;
					val = ghd;
					ghd = GetHexaDigit(*s);
					if (ghd >= 0) {
						s++;
						val *= 16;
						val += ghd;
					}
				}
				*o = static_cast<char>(val);
			} else {
				*o = *s;
			}
		} else {
			*o = *s;
		}
		o++;
		if (*s) {
			s++;
		}
	}
	*o = '\0';
	return o - sStart;
}

/**
 * Convert C style \0oo into their indicated characters.
 * This is used to get control characters into the regular expresion engine.
 */
unsigned int UnSlashLowOctal(char *s) {
	char *sStart = s;
	char *o = s;
	while (*s) {
		if ((s[0] == '\\') && (s[1] == '0') && IsOctalDigit(s[2]) && IsOctalDigit(s[3])) {
			*o = static_cast<char>(8 * (s[2] - '0') + (s[3] - '0'));
			s += 3;
		} else {
			*o = *s;
		}
		o++;
		if (*s)
			s++;
	}
	*o = '\0';
	return o - sStart;
}

static int UnSlashAsNeeded(SString &s, bool escapes, bool regularExpression) {
	if (escapes) {
		char *sUnslashed = StringDup(s.c_str(), s.length());
		size_t len;
		if (regularExpression) {
			// For regular expressions, the only escape sequences allowed start with \0
			// Other sequences, like \t, are handled by the RE engine.
			len = UnSlashLowOctal(sUnslashed);
		} else {
			// C style escapes allowed
			len = UnSlash(sUnslashed);
		}
		s = sUnslashed;
		delete []sUnslashed;
		return static_cast<int>(len);
	} else {
		return s.length();
	}
}

int SciTEBase::MarkAll() {
	int posCurrent = SendEditor(SCI_GETCURRENTPOS);
	int marked = 0;
	int posFirstFound = FindNext(false, false);
	if (posFirstFound != -1) {
		int posFound = posFirstFound;
		do {
			marked++;
			int line = SendEditor(SCI_LINEFROMPOSITION, posFound);
			BookmarkAdd(line);
			posFound = FindNext(false, false);
		} while ((posFound != -1) && (posFound != posFirstFound));
	}
	SendEditor(SCI_SETCURRENTPOS, posCurrent);
	return marked;
}

int SciTEBase::IncrementSearchMode() {
	FindIncrement();
	return 0;
}

int SciTEBase::FindInTarget(const char *findWhat, int lenFind, int startPosition, int endPosition) {
	SendEditor(SCI_SETTARGETSTART, startPosition);
	SendEditor(SCI_SETTARGETEND, endPosition);
	int posFind = SendEditorString(SCI_SEARCHINTARGET, lenFind, findWhat);
	while (findInStyle && posFind != -1 && findStyle != SendEditor(SCI_GETSTYLEAT, posFind)) {
		if (startPosition < endPosition) {
			SendEditor(SCI_SETTARGETSTART, posFind+1);
			SendEditor(SCI_SETTARGETEND, endPosition);
		} else {
			SendEditor(SCI_SETTARGETSTART, startPosition);
			SendEditor(SCI_SETTARGETEND, posFind+1);
		}
		posFind = SendEditorString(SCI_SEARCHINTARGET, lenFind, findWhat);
	}
	return posFind;
}

int SciTEBase::FindNext(bool reverseDirection, bool showWarnings) {
	if (findWhat.length() == 0) {
		Find();
		return -1;
	}
	SString findTarget = EncodeString(findWhat);
	int lenFind = UnSlashAsNeeded(findTarget, unSlash, regExp);
	if (lenFind == 0)
		return -1;

	CharacterRange cr = GetSelection();
	int startPosition = cr.cpMax;
	int endPosition = LengthDocument();
	if (reverseDirection) {
		startPosition = cr.cpMin;
		endPosition = 0;
	}

	int flags = (wholeWord ? SCFIND_WHOLEWORD : 0) |
	            (matchCase ? SCFIND_MATCHCASE : 0) |
	            (regExp ? SCFIND_REGEXP : 0) |
	            (props.GetInt("find.replace.regexp.posix") ? SCFIND_POSIX : 0);

	SendEditor(SCI_SETSEARCHFLAGS, flags);
	int posFind = FindInTarget(findTarget.c_str(), lenFind, startPosition, endPosition);
	if (posFind == -1 && wrapFind) {
		// Failed to find in indicated direction
		// so search from the beginning (forward) or from the end (reverse)
		// unless wrapFind is false
		if (reverseDirection) {
			startPosition = LengthDocument();
			endPosition = 0;
		} else {
			startPosition = 0;
			endPosition = LengthDocument();
		}
		posFind = FindInTarget(findTarget.c_str(), lenFind, startPosition, endPosition);
		WarnUser(warnFindWrapped);
	}
	if (posFind == -1) {
		havefound = false;
		if (showWarnings) {
			WarnUser(warnNotFound);
			SString msg = LocaliseMessage("Can not find the string '^0'.",
				findWhat.c_str());
			if (wFindReplace.Created()) {
				FindMessageBox(msg);
			} else {
				WindowMessageBox(wSciTE, msg, MB_OK | MB_ICONWARNING);
			}
		}
	} else {
		havefound = true;
		int start = SendEditor(SCI_GETTARGETSTART);
		int end = SendEditor(SCI_GETTARGETEND);
		EnsureRangeVisible(start, end);
		SetSelection(start, end);
		if (!replacing) {
			DestroyFindReplace();
		}
	}
	return posFind;
}

void SciTEBase::ReplaceOnce() {
	if (havefound) {
		SString replaceTarget = EncodeString(replaceWhat);
		int replaceLen = UnSlashAsNeeded(replaceTarget, unSlash, regExp);
		CharacterRange cr = GetSelection();
		SendEditor(SCI_SETTARGETSTART, cr.cpMin);
		SendEditor(SCI_SETTARGETEND, cr.cpMax);
		int lenReplaced = replaceLen;
		if (regExp)
			lenReplaced = SendEditorString(SCI_REPLACETARGETRE, replaceLen, replaceTarget.c_str());
		else	// Allow \0 in replacement
			SendEditorString(SCI_REPLACETARGET, replaceLen, replaceTarget.c_str());
		SetSelection(cr.cpMin + lenReplaced, cr.cpMin);
		havefound = false;
	}

	FindNext(reverseFind);
}

int SciTEBase::DoReplaceAll(bool inSelection) {
	SString findTarget = EncodeString(findWhat);
	int findLen = UnSlashAsNeeded(findTarget, unSlash, regExp);
	if (findLen == 0) {
		return -1;
	}

	CharacterRange cr = GetSelection();
	int startPosition = cr.cpMin;
	int endPosition = cr.cpMax;
	int selType = SC_SEL_STREAM;
	if (inSelection) {
		if (startPosition == endPosition) {
			return -2;
		}
		selType = SendEditor(SCI_GETSELECTIONMODE);
		if (selType == SC_SEL_LINES) {
			// Take care to replace in whole lines
			int startLine = SendEditor(SCI_LINEFROMPOSITION, startPosition);
			startPosition = SendEditor(SCI_POSITIONFROMLINE, startLine);
			int endLine = SendEditor(SCI_LINEFROMPOSITION, endPosition);
			endPosition = SendEditor(SCI_POSITIONFROMLINE, endLine+1);
		}
	} else {
		endPosition = LengthDocument();
		if (wrapFind) {
			// Whole document
			startPosition = 0;
		}
		// If not wrapFind, replace all only from caret to end of document
	}

	SString replaceTarget = EncodeString(replaceWhat);
	int replaceLen = UnSlashAsNeeded(replaceTarget, unSlash, regExp);
	int flags = (wholeWord ? SCFIND_WHOLEWORD : 0) |
	            (matchCase ? SCFIND_MATCHCASE : 0) |
	            (regExp ? SCFIND_REGEXP : 0) |
	            (props.GetInt("find.replace.regexp.posix") ? SCFIND_POSIX : 0);
	SendEditor(SCI_SETSEARCHFLAGS, flags);
	int posFind = FindInTarget(findTarget.c_str(), findLen, startPosition, endPosition);
	if ((findLen == 1) && regExp && (findTarget[0] == '^')) {
		// Special case for replace all start of line so it hits the first line
		posFind = startPosition;
		SendEditor(SCI_SETTARGETSTART, startPosition);
		SendEditor(SCI_SETTARGETEND, startPosition);
	}
	if ((posFind != -1) && (posFind <= endPosition)) {
		int lastMatch = posFind;
		int replacements = 0;
		SendEditor(SCI_BEGINUNDOACTION);
		// Replacement loop
		while (posFind != -1) {
			int lenTarget = SendEditor(SCI_GETTARGETEND) - SendEditor(SCI_GETTARGETSTART);
			if (inSelection && selType == SC_SEL_RECTANGLE) {
				// We must check that the found target is entirely inside the rectangular selection:
				// it must fit in one line, and inside the selection bounds of this line.
				int line = SendEditor(SCI_LINEFROMPOSITION, posFind);
				int startPos = SendEditor(SCI_GETLINESELSTARTPOSITION, line);
				int endPos = SendEditor(SCI_GETLINESELENDPOSITION, line);
				if (startPos == INVALID_POSITION ||	// No selection on this line (?)
				    posFind < startPos || posFind + lenTarget > endPos) {
					// Found target is totally or partly outside the rectangular selection
					lastMatch = posFind + 1;
					if (lastMatch >= endPosition) {
						// Run off the end of the document/selection with an empty match
						posFind = -1;
					} else {
						posFind = FindInTarget(findTarget.c_str(), findLen, lastMatch, endPosition);
					}
					continue;	// No replacement
				}
			}
			int movepastEOL = 0;
			if (lenTarget <= 0) {
				char chNext = static_cast<char>(SendEditor(SCI_GETCHARAT, SendEditor(SCI_GETTARGETEND)));
				if (chNext == '\r' || chNext == '\n') {
					movepastEOL = 1;
				}
			}
			int lenReplaced = replaceLen;
			if (regExp) {
				lenReplaced = SendEditorString(SCI_REPLACETARGETRE, replaceLen, replaceTarget.c_str());
			} else {
				SendEditorString(SCI_REPLACETARGET, replaceLen, replaceTarget.c_str());
			}
			// Modify for change caused by replacement
			endPosition += lenReplaced - lenTarget;
			// For the special cases of start of line and end of line
			// something better could be done but there are too many special cases
			lastMatch = posFind + lenReplaced + movepastEOL;
			if (lenTarget == 0) {
				lastMatch = SendEditor(SCI_POSITIONAFTER, lastMatch);
			}
			if (lastMatch >= endPosition) {
				// Run off the end of the document/selection with an empty match
				posFind = -1;
			} else {
				posFind = FindInTarget(findTarget.c_str(), findLen, lastMatch, endPosition);
			}
			replacements++;
		}
		if (inSelection) {
			SetSelection(startPosition, endPosition);
		} else {
			SetSelection(lastMatch, lastMatch);
		}
		SendEditor(SCI_ENDUNDOACTION);
		return replacements;
	}
	return 0;
	//Platform::DebugPrintf("ReplaceAll <%s> -> <%s>\n", findWhat, replaceWhat);
}

int SciTEBase::ReplaceAll(bool inSelection) {
	int replacements = DoReplaceAll(inSelection);
	props.SetInteger("Replacements", (replacements > 0 ? replacements : 0));
	UpdateStatusBar(false);
	if (replacements == -1) {
		SString msg = LocaliseMessage(
		                  inSelection ?
		                  "Find string must not be empty for 'Replace in Selection' command." :
		                  "Find string must not be empty for 'Replace All' command.");
		FindMessageBox(msg);
	} else if (replacements == -2) {
		SString msg = LocaliseMessage(
		                  "Selection must not be empty for 'Replace in Selection' command.");
		FindMessageBox(msg);
	} else if (replacements == 0) {
		SString msg = LocaliseMessage(
		                  "No replacements because string '^0' was not present.", findWhat.c_str());
		FindMessageBox(msg);
	}
	return replacements;
}

int SciTEBase::ReplaceInBuffers() {
	int currentBuffer = buffers.Current();
	int replacements = 0;
	for (int i = 0; i < buffers.length; i++) {
		SetDocumentAt(i);
		replacements += DoReplaceAll(false);
		if (i == 0 && replacements < 0) {
			SString msg = LocaliseMessage(
			                  "Find string must not be empty for 'Replace in Buffers' command.");
			FindMessageBox(msg);
			break;
		}
	}
	SetDocumentAt(currentBuffer);
	props.SetInteger("Replacements", replacements);
	UpdateStatusBar(false);
	if (replacements == 0) {
		SString msg = LocaliseMessage(
		                  "No replacements because string '^0' was not present.", findWhat.c_str());
		FindMessageBox(msg);
	}
	return replacements;
}

void SciTEBase::OutputAppendString(const char *s, int len, bool startLine) {
	if (len == -1)
		len = static_cast<int>(strlen(s));
	int docLength = SendOutput(SCI_GETTEXTLENGTH);

	if (startLine && docLength > 0) {
		char lastChar = static_cast<char>(SendOutput(SCI_GETCHARAT, docLength - 1));
		if (lastChar != '\n' && lastChar != '\r') {
			SendOutput(SCI_APPENDTEXT, 1, reinterpret_cast<sptr_t>("\n"));
		}
	}

	SendOutput(SCI_APPENDTEXT, len, reinterpret_cast<sptr_t>(s));
	if (scrollOutput) {
		int line = SendOutput(SCI_GETLINECOUNT, 0, 0);
		int lineStart = SendOutput(SCI_POSITIONFROMLINE, line);
		SendOutput(SCI_GOTOPOS, lineStart);
	}
}

void SciTEBase::OutputAppendStringSynchronised(const char *s, int len, bool startLine) {
	if (len == -1)
		len = static_cast<int>(strlen(s));
	int docLength = SendOutputEx(SCI_GETTEXTLENGTH, 0, 0, false);

	if (startLine && docLength > 0) {
		char lastChar = static_cast<char>(SendOutputEx(SCI_GETCHARAT, docLength - 1, 0, false));
		if (lastChar != '\n' && lastChar != '\r') {
			SendOutputEx(SCI_APPENDTEXT, 1, reinterpret_cast<sptr_t>("\n"), false);
		}
	}

	SendOutputEx(SCI_APPENDTEXT, len, reinterpret_cast<sptr_t>(s), false);
	if (scrollOutput) {
		int line = SendOutputEx(SCI_GETLINECOUNT, 0, 0, false);
		int lineStart = SendOutputEx(SCI_POSITIONFROMLINE, line, 0, false);
		SendOutputEx(SCI_GOTOPOS, lineStart, 0, false);
	}
}

void SciTEBase::MakeOutputVisible() {
	if (heightOutput < 20) {
		if (splitVertical)
			heightOutput = NormaliseSplit(300);
		else
			heightOutput = NormaliseSplit(100);
		SizeSubWindows();
		Redraw();
	}
}

void SciTEBase::ClearJobQueue() {
	for (int ic = 0; ic < commandMax; ic++) {
		jobQueue[ic].Clear();
	}
	commandCurrent = 0;
}

void SciTEBase::Execute() {


	props.Set("CurrentMessage", "");
	dirNameForExecute = FilePath();
	bool displayParameterDialog = false;
	int ic;
	parameterisedCommand = "";
	for (ic = 0; ic < commandMax; ic++) {
		if (jobQueue[ic].command[0] == '*') {
			displayParameterDialog = true;
			jobQueue[ic].command.remove(0, 1);
			parameterisedCommand = jobQueue[ic].command;
		}
		if (jobQueue[ic].directory.IsSet()) {
			dirNameForExecute = jobQueue[ic].directory;
		}
	}
	if (displayParameterDialog) {
		if (!ParametersDialog(true)) {
			ClearJobQueue();
			return;
		}
	} else {
		ParamGrab();
	}
	for (ic = 0; ic < commandMax; ic++) {
		jobQueue[ic].command = props.Expand(jobQueue[ic].command.c_str());
	}

	if (clearBeforeExecute) {
		SendOutputEx(SCI_CLEARALL, 0, 0, false);
	}

	SendOutput(SCI_MARKERDELETEALL, static_cast<uptr_t>(-1));
	SendEditor(SCI_MARKERDELETEALL, 0);
	// Ensure the output pane is visible
	if (jobUsesOutputPane) {
		MakeOutputVisible();
	}

	cancelFlag = 0L;
	executing = true;
	CheckMenus();
	filePath.Directory().SetWorkingDirectory();
	dirNameAtExecute = filePath.Directory();
}

void SciTEBase::ToggleOutputVisible() {
	if (heightOutput > 0) {
		heightOutput = NormaliseSplit(0);
		WindowSetFocus(wEditor);
	} else {
		if (previousHeightOutput < 20) {
			if (splitVertical)
				heightOutput = NormaliseSplit(300);
			else
				heightOutput = NormaliseSplit(100);
			previousHeightOutput = heightOutput;
		} else {
			heightOutput = NormaliseSplit(previousHeightOutput);
		}
	}
	SizeSubWindows();
	Redraw();
}

void SciTEBase::BookmarkAdd(int lineno) {
	if (lineno == -1)
		lineno = GetCurrentLineNumber();
	if (!BookmarkPresent(lineno))
		SendEditor(SCI_MARKERADD, lineno, SciTE_MARKER_BOOKMARK);
}

void SciTEBase::BookmarkDelete(int lineno) {
	if (lineno == -1)
		lineno = GetCurrentLineNumber();
	if (BookmarkPresent(lineno))
		SendEditor(SCI_MARKERDELETE, lineno, SciTE_MARKER_BOOKMARK);
}

bool SciTEBase::BookmarkPresent(int lineno) {
	if (lineno == -1)
		lineno = GetCurrentLineNumber();
	int state = SendEditor(SCI_MARKERGET, lineno);
	return state & (1 << SciTE_MARKER_BOOKMARK);
}

void SciTEBase::BookmarkToggle(int lineno) {
	if (lineno == -1)
		lineno = GetCurrentLineNumber();
	if (BookmarkPresent(lineno)) {
		BookmarkDelete(lineno);
	} else {
		BookmarkAdd(lineno);
	}
}

void SciTEBase::BookmarkNext(bool forwardScan, bool select) {
	int lineno = GetCurrentLineNumber();
	int sci_marker = SCI_MARKERNEXT;
	int lineStart = lineno + 1;	//Scan starting from next line
	int lineRetry = 0;				//If not found, try from the beginning
	int anchor = SendEditor(SCI_GETANCHOR);
	if (!forwardScan) {
		lineStart = lineno - 1;		//Scan starting from previous line
		lineRetry = SendEditor(SCI_GETLINECOUNT, 0, 0L);	//If not found, try from the end
		sci_marker = SCI_MARKERPREVIOUS;
	}
	int nextLine = SendEditor(sci_marker, lineStart, 1 << SciTE_MARKER_BOOKMARK);
	if (nextLine < 0)
		nextLine = SendEditor(sci_marker, lineRetry, 1 << SciTE_MARKER_BOOKMARK);
	if (nextLine < 0 || nextLine == lineno)	// No bookmark (of the given type) or only one, and already on it
		WarnUser(warnNoOtherBookmark);
	else {
		GotoLineEnsureVisible(nextLine);
		if (select) {
			SendEditor(SCI_SETANCHOR, anchor);
		}
	}
}

PRectangle SciTEBase::GetClientRectangle() {
	return wContent.GetClientPosition();
}

void SciTEBase::Redraw() {
	wSciTE.InvalidateAll();
	wEditor.InvalidateAll();
	wOutput.InvalidateAll();
}

void SciTEBase::FillFunctionDefinition(int pos /*= -1*/) {
	if (pos > 0) {
		lastPosCallTip = pos;
	}
	if (apis) {
		char *words = apis.GetNearestWords(currentCallTipWord.c_str(), currentCallTipWord.length(),
		                    callTipIgnoreCase, calltipParametersStart[0], true);
		if (!words)
			return;
		// Counts how many call tips
		const char *spacePos = strchr(words, ' ');
		maxCallTips = 1;
		while (spacePos) {
			maxCallTips++;
			spacePos = strchr(spacePos + 1, ' ');
		}
		delete []words;

		// Should get current api definition
		const char *word = apis.GetNearestWord(currentCallTipWord.c_str(), currentCallTipWord.length(),
		                                       callTipIgnoreCase, calltipWordCharacters, currentCallTip);
		if (word) {
			functionDefinition = word;
			if (maxCallTips > 1) {
				functionDefinition.insert(0, "\001");

        SString funcNumCount;
        funcNumCount += "[";
        funcNumCount += SString(currentCallTip+1);
        funcNumCount += " of ";
        funcNumCount += SString(maxCallTips);
        funcNumCount += "] ";

        functionDefinition.insert(1, funcNumCount.c_str());
			}

			if (calltipEndDefinition != "") {
				int posEndDef = functionDefinition.search(calltipEndDefinition.c_str());
				if (maxCallTips > 1) {
					if ((posEndDef > 1) &&
					        ((posEndDef + calltipEndDefinition.length()) < functionDefinition.length())) {
            functionDefinition.insert(posEndDef + calltipEndDefinition.length(), "\n\002");
					} else {
						functionDefinition.append("\n\002");
					}
				} else {
					if ((posEndDef > 1) &&
					        ((posEndDef + calltipEndDefinition.length()) < functionDefinition.length())) {
						functionDefinition.insert(posEndDef + calltipEndDefinition.length(), "\n");
					}
				}
			} else {
				functionDefinition.insert(1, "\002");
			}

			SendEditorString(SCI_CALLTIPSHOW, lastPosCallTip - currentCallTipWord.length(), functionDefinition.c_str());
			ContinueCallTip();
		}
	}
}

bool SciTEBase::StartCallTip() {
	currentCallTip = 0;
	currentCallTipWord = "";
	SString line = GetLine();
	int current = GetCaretInLine();
	int pos = SendEditor(SCI_GETCURRENTPOS);
	int braces;
	do {
		braces = 0;
		while (current > 0 && (braces || !calltipParametersStart.contains(line[current - 1]))) {
			if (calltipParametersStart.contains(line[current - 1]))
				braces--;
			else if (calltipParametersEnd.contains(line[current - 1]))
				braces++;
			current--;
			pos--;
		}
		if (current > 0) {
			current--;
			pos--;
		} else
			break;
		while (current > 0 && isspacechar(line[current - 1])) {
			current--;
			pos--;
		}
	} while (current > 0 && !calltipWordCharacters.contains(line[current - 1]));
	if (current <= 0)
		return true;

	startCalltipWord = current - 1;
	while (startCalltipWord > 0 &&
	        calltipWordCharacters.contains(line[startCalltipWord - 1])) {
		startCalltipWord--;
	}

	line.change(current, '\0');
	currentCallTipWord = line.c_str() + startCalltipWord;
	functionDefinition = "";
	//Platform::DebugPrintf("word  is [%s] %d %d %d\n", currentCallTipWord.c_str(), currentCallTipWord.length(), pos, pos - rootlen);
	FillFunctionDefinition(pos);
	return true;
}

void SciTEBase::ContinueCallTip() {
	SString line = GetLine();
	int current = GetCaretInLine();

	int braces = 0;
	int commas = 0;
	for (int i = startCalltipWord; i < current; i++) {
		if (calltipParametersStart.contains(line[i]))
			braces++;
		else if (calltipParametersEnd.contains(line[i]) && braces > 0)
			braces--;
		else if (braces == 1 && calltipParametersSeparators.contains(line[i]))
			commas++;
	}

	int startHighlight = 0;
	while (functionDefinition[startHighlight] && !calltipParametersStart.contains(functionDefinition[startHighlight]))
		startHighlight++;
	if (calltipParametersStart.contains(functionDefinition[startHighlight]))
		startHighlight++;
	while (functionDefinition[startHighlight] && commas > 0) {
		if (calltipParametersSeparators.contains(functionDefinition[startHighlight]))
			commas--;
		// If it reached the end of the argument list it means that the user typed in more
		// arguments than the ones listed in the calltip
		if (calltipParametersEnd.contains(functionDefinition[startHighlight]))
			commas = 0;
		else
			startHighlight++;
	}
	if (calltipParametersSeparators.contains(functionDefinition[startHighlight]))
		startHighlight++;
	int endHighlight = startHighlight;
	while (functionDefinition[endHighlight] && !calltipParametersSeparators.contains(functionDefinition[endHighlight]) && !calltipParametersEnd.contains(functionDefinition[endHighlight]))
		endHighlight++;

	SendEditor(SCI_CALLTIPSETHLT, startHighlight, endHighlight);
}

void SciTEBase::EliminateDuplicateWords(char *words) {
	char *firstWord = words;
	char *firstSpace = strchr(firstWord, ' ');
	char *secondWord;
	char *secondSpace;
	int firstLen, secondLen;

	while (firstSpace) {
		firstLen = firstSpace - firstWord;
		secondWord = firstWord + firstLen + 1;
		secondSpace = strchr(secondWord, ' ');

		if (secondSpace)
			secondLen = secondSpace - secondWord;
		else
			secondLen = strlen(secondWord);

		if (firstLen == secondLen &&
		        !strncmp(firstWord, secondWord, firstLen)) {
			strcpy(firstWord, secondWord);
			firstSpace = strchr(firstWord, ' ');
		} else {
			firstWord = secondWord;
			firstSpace = secondSpace;
		}
	}
}

bool SciTEBase::StartAutoComplete() {
	SString line = GetLine();
	int current = GetCaretInLine();

	int startword = current;

	while ((startword > 0) &&
	        (calltipWordCharacters.contains(line[startword - 1]) ||
	         autoCompleteStartCharacters.contains(line[startword - 1]))) {
		startword--;
	}

	SString root = line.substr(startword, current - startword);
	if (apis) {
		char *words = apis.GetNearestWords(root.c_str(), root.length(),
											autoCompleteIgnoreCase, calltipParametersStart[0]);
		if (words) {
			EliminateDuplicateWords(words);
			SendEditorString(SCI_AUTOCSHOW, root.length(), words);
			delete []words;
		}
	}
	return true;
}

bool SciTEBase::StartAutoCompleteWord(bool onlyOneWord) {
	SString line = GetLine();
	int current = GetCaretInLine();
//~ 	if (current >= line.size())
//~ 		return false;

	int startword = current;
	// Autocompletion of pure numbers is mostly an annoyance
	bool allNumber = true;
	while (startword > 0 && wordCharacters.contains(line[startword - 1])) {
		startword--;
		if (line[startword] < '0' || line[startword] > '9') {
			allNumber = false;
		}
	}
	if (startword == current || allNumber)
		return true;
	SString root = line.substr(startword, current - startword);
	int doclen = LengthDocument();
	TextToFind ft = {{0, 0}, 0, {0, 0}};
	ft.lpstrText = const_cast<char*>(root.c_str());
	ft.chrg.cpMin = 0;
	ft.chrgText.cpMin = 0;
	ft.chrgText.cpMax = 0;
	int flags = SCFIND_WORDSTART | (autoCompleteIgnoreCase ? 0 : SCFIND_MATCHCASE);
	int posCurrentWord = SendEditor(SCI_GETCURRENTPOS) - root.length();
	unsigned int minWordLength = 0;
	unsigned int nwords = 0;

	// wordsNear contains a list of words separated by single spaces and with a space
	// at the start and end. This makes it easy to search for words.
	SString wordsNear;
	wordsNear.setsizegrowth(1000);
	wordsNear += " ";

	for (;;) {	// search all the document
		ft.chrg.cpMax = doclen;
		int posFind = SendEditorString(SCI_FINDTEXT, flags, reinterpret_cast<char *>(&ft));
		if (posFind == -1 || posFind >= doclen)
			break;
		if (posFind == posCurrentWord) {
			ft.chrg.cpMin = posFind + root.length();
			continue;
		}
		// Grab the word and put spaces around it
		const unsigned int wordMaxSize = 800;
		char wordstart[wordMaxSize];
		wordstart[0] = ' ';
		GetRange(wEditor, posFind, Platform::Minimum(posFind + wordMaxSize - 3, doclen), wordstart + 1);
		char *wordend = wordstart + 1 + root.length();
		while (iswordcharforsel(*wordend))
			wordend++;
		*wordend++ = ' ';
		*wordend = '\0';
		unsigned int wordlen = wordend - wordstart - 2;
		if (wordlen > root.length()) {
			if (!wordsNear.contains(wordstart)) {	// add a new entry
				wordsNear += wordstart + 1;
				if (minWordLength < wordlen)
					minWordLength = wordlen;

				nwords++;
				if (onlyOneWord && nwords > 1) {
					return true;
				}
			}
		}
		ft.chrg.cpMin = posFind + wordlen;
	}
	size_t length = wordsNear.length();
	if ((length > 2) && (!onlyOneWord || (minWordLength > root.length()))) {
		WordList wl;
		wl.Set(wordsNear.c_str());
		char *words = wl.GetNearestWords("", 0, autoCompleteIgnoreCase);
		SendEditorString(SCI_AUTOCSHOW, root.length(), words);
		delete []words;
	} else {
		SendEditor(SCI_AUTOCCANCEL);
	}
	return true;
}

bool SciTEBase::StartInsertAbbreviation() {
	if (!AbbrevDialog()) {
		return true;
	}

	SString data = propsAbbrev.Get(abbrevInsert);
	size_t dataLength = data.length();
	if (dataLength == 0) {
		return true; // returning if expanded abbreviation is empty
	}

	char *expbuf = new char[dataLength + 1];
	strcpy(expbuf, data.c_str());
	UnSlash(expbuf);
	size_t expbuflen = strlen(expbuf);
	int caret_pos = SendEditor(SCI_GETSELECTIONSTART);
	int sel_start = caret_pos;
	int sel_length = SendEditor(SCI_GETSELECTIONEND) - sel_start;
	bool at_start = true;
	bool double_pipe = false;
	size_t last_pipe = expbuflen;
	int currentLineNumber = SendEditor(SCI_LINEFROMPOSITION, caret_pos);
	int indent = 0;
	int indentSize = SendEditor(SCI_GETINDENT);
	int indentChars = (SendEditor(SCI_GETUSETABS) && SendEditor(SCI_GETTABWIDTH) ? SendEditor(SCI_GETTABWIDTH) : 1);
	int indentExtra = 0;
	bool isIndent = true;
	int eolMode = SendEditor(SCI_GETEOLMODE);
	if (props.GetInt("indent.automatic")) {
		indent = GetLineIndentation(currentLineNumber);
	}

	size_t i;
	// find last |, can't be strrchr(exbuf, '|') because of ||
	for (i = expbuflen; i--; ) {
		if (expbuf[i] == '|' && (i == 0 || expbuf[i-1] != '|')) {
			last_pipe = i;
			break;
		}
	}

	SendEditor(SCI_BEGINUNDOACTION);

	// add the abbreviation one character at a time
	for (i = 0; i < expbuflen; i++) {
		char c = expbuf[i];
		SString abbrevText("");
		if (isIndent && c == '\t') {
			if (props.GetInt("indent.automatic")) {
				indentExtra++;
				SetLineIndentation(currentLineNumber, indent + indentSize * indentExtra);
				caret_pos += indentSize / indentChars;
			}
		} else {
			switch (c) {
			case '|':
				// user may want to insert '|' instead of caret
				if (i < (dataLength - 1) && expbuf[i + 1] == '|') {
					// put '|' into the line
					abbrevText += c;
					i++;
				} else if (i != last_pipe) {
					double_pipe = true;
				} else {
					// indent on multiple lines
					int j = currentLineNumber + 1; // first line indented as others
					currentLineNumber = SendEditor(SCI_LINEFROMPOSITION, caret_pos + sel_length);
					for (; j <= currentLineNumber; j++) {
						SetLineIndentation(j, GetLineIndentation(j) + indentSize * indentExtra);
						caret_pos += indentExtra * indentSize / indentChars;
					}

					at_start = false;
					caret_pos += sel_length;
				}
				break;
			case '\r':
				// backward compatibility
				break;
			case '\n':
				if (eolMode == SC_EOL_CRLF || eolMode == SC_EOL_CR) {
					abbrevText += '\r';
				}
				if (eolMode == SC_EOL_CRLF || eolMode == SC_EOL_LF) {
					abbrevText += '\n';
				}
				break;
			default:
				abbrevText += c;
				break;
			}
			if (caret_pos > SendEditor(SCI_GETLENGTH)) {
				caret_pos = SendEditor(SCI_GETLENGTH);
			}
			SendEditorString(SCI_INSERTTEXT, caret_pos, abbrevText.c_str());
			if (!double_pipe && at_start) {
				sel_start += static_cast<int>(abbrevText.length());
			}
			caret_pos += static_cast<int>(abbrevText.length());
			if (c == '\n') {
				isIndent = true;
				indentExtra = 0;
				currentLineNumber++;
				SetLineIndentation(currentLineNumber, indent);
				caret_pos += indent / indentChars;
				if (!double_pipe && at_start) {
					sel_start += indent / indentChars;
				}
			} else {
				isIndent = false;
			}
		}
	}

	// set the caret to the desired position
	if (double_pipe) {
		sel_length = 0;
	} else if (!at_start && sel_length == 0) {
		sel_start += static_cast<int>(expbuflen);
	}
	SendEditor(SCI_SETSEL, sel_start, sel_start + sel_length);

	SendEditor(SCI_ENDUNDOACTION);
	delete []expbuf;
	return true;
}

bool SciTEBase::StartExpandAbbreviation() {
	int currentPos = GetCaretInLine();
	int position = SendEditor(SCI_GETCURRENTPOS); // from the beginning
	char *linebuf = new char[currentPos + 2];
	GetLine(linebuf, currentPos + 2);	// Just get text to the left of the caret
	linebuf[currentPos] = '\0';
	int abbrevPos = (currentPos > 32 ? currentPos - 32 : 0);
	const char *abbrev = linebuf + abbrevPos;
	SString data;
	size_t dataLength = 0;
	int abbrevLength = currentPos - abbrevPos;
	// Try each potential abbreviation from the first letter on a line
	// and expanding to the right.
	// We arbitrarily limits the length of an abbreviation (seems a reasonable value..),
	// and of course stop on the caret.
	while (abbrevLength > 0) {
		data = propsAbbrev.Get(abbrev);
		dataLength = data.length();
		if (dataLength > 0) {
			break;	/* Found */
		}
		abbrev++;	// One more letter to the right
		abbrevLength--;
	}

	if (dataLength == 0) {
		WarnUser(warnNotFound);	// No need for a special warning
		return true; // returning if expanded abbreviation is empty
	}

	char *expbuf = new char[dataLength + 1];
	strcpy(expbuf, data.c_str());
	UnSlash(expbuf);
	size_t expbuflen = strlen(expbuf);
	int caret_pos = -1; // caret position
	int currentLineNumber = GetCurrentLineNumber();
	int indent = 0;
	int indentExtra = 0;
	bool isIndent = true;
	int eolMode = SendEditor(SCI_GETEOLMODE);
	if (props.GetInt("indent.automatic")) {
		indent = GetLineIndentation(currentLineNumber);
	}

	SendEditor(SCI_BEGINUNDOACTION);
	SendEditor(SCI_SETSEL, position - abbrevLength, position);

	// add the abbreviation one character at a time
	for (size_t i = 0; i < expbuflen; i++) {
		char c = expbuf[i];
		SString abbrevText("");
		if (isIndent && c == '\t') {
			indentExtra++;
			SetLineIndentation(currentLineNumber, indent + SendEditor(SCI_GETINDENT) * indentExtra);
		} else {
			switch (c) {
			case '|':
				// user may want to insert '|' instead of caret
				if (i < (dataLength - 1) && expbuf[i + 1] == '|') {
					// put '|' into the line
					abbrevText += c;
					i++;
				} else if (caret_pos == -1) {
					if (i == 0) {
						// when caret is set at the first place in abbreviation
						caret_pos = SendEditor(SCI_GETCURRENTPOS) - abbrevLength;
					} else {
						caret_pos = SendEditor(SCI_GETCURRENTPOS);
					}
				}
				break;
			case '\n':
				if (eolMode == SC_EOL_CRLF || eolMode == SC_EOL_CR) {
					abbrevText += '\r';
				}
				if (eolMode == SC_EOL_CRLF || eolMode == SC_EOL_LF) {
					abbrevText += '\n';
				}
				break;
			default:
				abbrevText += c;
				break;
			}
			SendEditorString(SCI_REPLACESEL, 0, abbrevText.c_str());
			if (c == '\n') {
				isIndent = true;
				indentExtra = 0;
				currentLineNumber++;
				SetLineIndentation(currentLineNumber, indent);
			} else {
				isIndent = false;
			}
		}
	}

	// set the caret to the desired position
	if (caret_pos != -1) {
		SendEditor(SCI_GOTOPOS, caret_pos);
	}

	SendEditor(SCI_ENDUNDOACTION);
	delete []expbuf;
	delete []linebuf;
	return true;
}

bool SciTEBase::StartBlockComment() {
	SString fileNameForExtension = ExtensionFileName();
	SString language = props.GetNewExpand("lexer.", fileNameForExtension.c_str());
	SString base("comment.block.");
	SString comment_at_line_start("comment.block.at.line.start.");
	base += language;
	comment_at_line_start += language;
	SString comment = props.Get(base.c_str());
	if (comment == "") { // user friendly error message box
		SString error = LocaliseMessage(
		                    "Block comment variable '^0' is not defined in SciTE *.properties!", base.c_str());
		WindowMessageBox(wSciTE, error, MB_OK | MB_ICONWARNING);
		return true;
	}
	comment += " ";
	SString long_comment = comment;
	char linebuf[1000];
	size_t comment_length = comment.length();
	size_t selectionStart = SendEditor(SCI_GETSELECTIONSTART);
	size_t selectionEnd = SendEditor(SCI_GETSELECTIONEND);
	size_t caretPosition = SendEditor(SCI_GETCURRENTPOS);
	// checking if caret is located in _beginning_ of selected block
	bool move_caret = caretPosition < selectionEnd;
	int selStartLine = SendEditor(SCI_LINEFROMPOSITION, selectionStart);
	int selEndLine = SendEditor(SCI_LINEFROMPOSITION, selectionEnd);
	int lines = selEndLine - selStartLine;
	size_t firstSelLineStart = SendEditor(SCI_POSITIONFROMLINE, selStartLine);
	// "caret return" is part of the last selected line
	if ((lines > 0) &&
	        (selectionEnd == static_cast<size_t>(SendEditor(SCI_POSITIONFROMLINE, selEndLine))))
		selEndLine--;
	SendEditor(SCI_BEGINUNDOACTION);
	for (int i = selStartLine; i <= selEndLine; i++) {
		int lineStart = SendEditor(SCI_POSITIONFROMLINE, i);
		int lineIndent = lineStart;
		int lineEnd = SendEditor(SCI_GETLINEENDPOSITION, i);
		if ((lineEnd - lineIndent) >= static_cast<int>(sizeof(linebuf)))	// Avoid buffer size problems
			continue;
		if (props.GetInt(comment_at_line_start.c_str())) {
			GetRange(wEditor, lineIndent, lineEnd, linebuf);
		} else {
			lineIndent = GetLineIndentPosition(i);
			GetRange(wEditor, lineIndent, lineEnd, linebuf);
		}
		// empty lines are not commented
		if (strlen(linebuf) < 1)
			continue;
		if (memcmp(linebuf, comment.c_str(), comment_length - 1) == 0) {
			if (memcmp(linebuf, long_comment.c_str(), comment_length) == 0) {
				// removing comment with space after it
				SendEditor(SCI_SETSEL, lineIndent, lineIndent + comment_length);
				SendEditorString(SCI_REPLACESEL, 0, "");
				if (i == selStartLine) // is this the first selected line?
					selectionStart -= comment_length;
				selectionEnd -= comment_length; // every iteration
				continue;
			} else {
				// removing comment _without_ space
				SendEditor(SCI_SETSEL, lineIndent, lineIndent + comment_length - 1);
				SendEditorString(SCI_REPLACESEL, 0, "");
				if (i == selStartLine) // is this the first selected line?
					selectionStart -= (comment_length - 1);
				selectionEnd -= (comment_length - 1); // every iteration
				continue;
			}
		}
		if (i == selStartLine) // is this the first selected line?
			selectionStart += comment_length;
		selectionEnd += comment_length; // every iteration
		SendEditorString(SCI_INSERTTEXT, lineIndent, long_comment.c_str());
	}
	// after uncommenting selection may promote itself to the lines
	// before the first initially selected line;
	// another problem - if only comment symbol was selected;
	if (selectionStart < firstSelLineStart) {
		if (selectionStart >= selectionEnd - (comment_length - 1))
			selectionEnd = firstSelLineStart;
		selectionStart = firstSelLineStart;
	}
	if (move_caret) {
		// moving caret to the beginning of selected block
		SendEditor(SCI_GOTOPOS, selectionEnd);
		SendEditor(SCI_SETCURRENTPOS, selectionStart);
	} else {
		SendEditor(SCI_SETSEL, selectionStart, selectionEnd);
	}
	SendEditor(SCI_ENDUNDOACTION);
	return true;
}

bool SciTEBase::StartBoxComment() {
	// Get start/middle/end comment strings from options file(s)
	SString fileNameForExtension = ExtensionFileName();
	SString language = props.GetNewExpand("lexer.", fileNameForExtension.c_str());
	SString start_base("comment.box.start.");
	SString middle_base("comment.box.middle.");
	SString end_base("comment.box.end.");
	SString white_space(" ");
	start_base += language;
	middle_base += language;
	end_base += language;
	SString start_comment = props.Get(start_base.c_str());
	SString middle_comment = props.Get(middle_base.c_str());
	SString end_comment = props.Get(end_base.c_str());
	if (start_comment == "" || middle_comment == "" || end_comment == "") {
		SString error = LocaliseMessage(
		                    "Box comment variables '^0', '^1' and '^2' are not defined in SciTE *.properties!",
		                    start_base.c_str(), middle_base.c_str(), end_base.c_str());
		WindowMessageBox(wSciTE, error, MB_OK | MB_ICONWARNING);
		return true;
	}

	// Note selection and cursor location so that we can reselect text and reposition cursor after we insert comment strings
	size_t selectionStart = SendEditor(SCI_GETSELECTIONSTART);
	size_t selectionEnd = SendEditor(SCI_GETSELECTIONEND);
	size_t caretPosition = SendEditor(SCI_GETCURRENTPOS);
	bool move_caret = caretPosition < selectionEnd;
	size_t selStartLine = SendEditor(SCI_LINEFROMPOSITION, selectionStart);
	size_t selEndLine = SendEditor(SCI_LINEFROMPOSITION, selectionEnd);
	size_t lines = selEndLine - selStartLine + 1;

	// If selection ends at start of last selected line, fake it so that selection goes to end of second-last selected line
	if (lines > 1 && selectionEnd == static_cast<size_t>(SendEditor(SCI_POSITIONFROMLINE, selEndLine))) {
		selEndLine--;
		lines--;
		selectionEnd = SendEditor(SCI_GETLINEENDPOSITION, selEndLine);
	}

	// Pad comment strings with appropriate whitespace, then figure out their lengths (end_comment is a bit special-- see below)
	start_comment += white_space;
	middle_comment += white_space;
	size_t start_comment_length = start_comment.length();
	size_t middle_comment_length = middle_comment.length();
	size_t end_comment_length = end_comment.length();
	size_t whitespace_length = white_space.length();

	// Calculate the length of the longest comment string to be inserted, and allocate a null-terminated char buffer of equal size
	size_t maxCommentLength = start_comment_length;
	if (middle_comment_length > maxCommentLength)
		maxCommentLength = middle_comment_length;
	if (end_comment_length + whitespace_length > maxCommentLength)
		maxCommentLength = end_comment_length + whitespace_length;
	char *tempString = new char[maxCommentLength + 1];

	SendEditor(SCI_BEGINUNDOACTION);

	// Insert start_comment if needed
	int lineStart = SendEditor(SCI_POSITIONFROMLINE, selStartLine);
	GetRange(wEditor, lineStart, lineStart + start_comment_length, tempString);
	tempString[start_comment_length] = '\0';
	if (start_comment != tempString) {
		SendEditorString(SCI_INSERTTEXT, lineStart, start_comment.c_str());
		selectionStart += start_comment_length;
		selectionEnd += start_comment_length;
	}

	if (lines <= 1) {
		// Only a single line was selected, so just append whitespace + end-comment at end of line if needed
		int lineEnd = SendEditor(SCI_GETLINEENDPOSITION, selEndLine);
		GetRange(wEditor, lineEnd - end_comment_length, lineEnd, tempString);
		tempString[end_comment_length] = '\0';
		if (end_comment != tempString) {
			end_comment.insert(0, white_space.c_str());
			SendEditorString(SCI_INSERTTEXT, lineEnd, end_comment.c_str());
		}
	} else {
		// More than one line selected, so insert middle_comments where needed
		for (size_t i = selStartLine + 1; i < selEndLine; i++) {
			lineStart = SendEditor(SCI_POSITIONFROMLINE, i);
			GetRange(wEditor, lineStart, lineStart + middle_comment_length, tempString);
			tempString[middle_comment_length] = '\0';
			if (middle_comment != tempString) {
				SendEditorString(SCI_INSERTTEXT, lineStart, middle_comment.c_str());
				selectionEnd += middle_comment_length;
			}
		}

		// If last selected line is not middle-comment or end-comment, we need to insert
		// a middle-comment at the start of last selected line and possibly still insert
		// and end-comment tag after the last line (extra logic is necessary to
		// deal with the case that user selected the end-comment tag)
		lineStart = SendEditor(SCI_POSITIONFROMLINE, selEndLine);
		GetRange(wEditor, lineStart, lineStart + end_comment_length, tempString);
		tempString[end_comment_length] = '\0';
		if (end_comment != tempString) {
			GetRange(wEditor, lineStart, lineStart + middle_comment_length, tempString);
			tempString[middle_comment_length] = '\0';
			if (middle_comment != tempString) {
				SendEditorString(SCI_INSERTTEXT, lineStart, middle_comment.c_str());
				selectionEnd += middle_comment_length;
			}

			// And since we didn't find the end-comment string yet, we need to check the *next* line
			//  to see if it's necessary to insert an end-comment string and a linefeed there....
			lineStart = SendEditor(SCI_POSITIONFROMLINE, selEndLine + 1);
			GetRange(wEditor, lineStart, lineStart + (int) end_comment_length, tempString);
			tempString[end_comment_length] = '\0';
			if (end_comment != tempString) {
				end_comment.append("\n");
				SendEditorString(SCI_INSERTTEXT, lineStart, end_comment.c_str());
			}
		}
	}

	if (move_caret) {
		// moving caret to the beginning of selected block
		SendEditor(SCI_GOTOPOS, selectionEnd);
		SendEditor(SCI_SETCURRENTPOS, selectionStart);
	} else {
		SendEditor(SCI_SETSEL, selectionStart, selectionEnd);
	}

	SendEditor(SCI_ENDUNDOACTION);

	delete[] tempString;

	return true;
}

bool SciTEBase::StartStreamComment() {
	SString fileNameForExtension = ExtensionFileName();
	SString language = props.GetNewExpand("lexer.", fileNameForExtension.c_str());
	SString start_base("comment.stream.start.");
	SString end_base("comment.stream.end.");
	SString white_space(" ");
	start_base += language;
	end_base += language;
	SString start_comment = props.Get(start_base.c_str());
	SString end_comment = props.Get(end_base.c_str());
	if (start_comment == "" || end_comment == "") {
		SString error = LocaliseMessage(
		                    "Stream comment variables '^0' and '^1' are not defined in SciTE *.properties!",
		                    start_base.c_str(), end_base.c_str());
		WindowMessageBox(wSciTE, error, MB_OK | MB_ICONWARNING);
		return true;
	}
	start_comment += white_space;
	white_space += end_comment;
	end_comment = white_space;
	size_t start_comment_length = start_comment.length();
	size_t selectionStart = SendEditor(SCI_GETSELECTIONSTART);
	size_t selectionEnd = SendEditor(SCI_GETSELECTIONEND);
	size_t caretPosition = SendEditor(SCI_GETCURRENTPOS);
	// checking if caret is located in _beginning_ of selected block
	bool move_caret = caretPosition < selectionEnd;
	// if there is no selection?
	if (selectionEnd - selectionStart <= 0) {
		int selLine = SendEditor(SCI_LINEFROMPOSITION, selectionStart);
		int lineIndent = GetLineIndentPosition(selLine);
		int lineEnd = SendEditor(SCI_GETLINEENDPOSITION, selLine);
		if (RangeIsAllWhitespace(lineIndent, lineEnd))
			return true; // we are not dealing with empty lines
		char linebuf[1000];
		GetLine(linebuf, sizeof(linebuf));
		int current = GetCaretInLine();
		// checking if we are not inside a word
		if (!wordCharacters.contains(linebuf[current]))
			return true; // caret is located _between_ words
		int startword = current;
		int endword = current;
		int start_counter = 0;
		int end_counter = 0;
		while (startword > 0 && wordCharacters.contains(linebuf[startword - 1])) {
			start_counter++;
			startword--;
		}
		// checking _beginning_ of the word
		if (startword == current)
			return true; // caret is located _before_ a word
		while (linebuf[endword + 1] != '\0' && wordCharacters.contains(linebuf[endword + 1])) {
			end_counter++;
			endword++;
		}
		selectionStart -= start_counter;
		selectionEnd += (end_counter + 1);
	}
	SendEditor(SCI_BEGINUNDOACTION);
	SendEditorString(SCI_INSERTTEXT, selectionStart, start_comment.c_str());
	selectionEnd += start_comment_length;
	selectionStart += start_comment_length;
	SendEditorString(SCI_INSERTTEXT, selectionEnd, end_comment.c_str());
	if (move_caret) {
		// moving caret to the beginning of selected block
		SendEditor(SCI_GOTOPOS, selectionEnd);
		SendEditor(SCI_SETCURRENTPOS, selectionStart);
	} else {
		SendEditor(SCI_SETSEL, selectionStart, selectionEnd);
	}
	SendEditor(SCI_ENDUNDOACTION);
	return true;
}

/**
 * Return the length of the given line, not counting the EOL.
 */
int SciTEBase::GetLineLength(int line) {
	return SendEditor(SCI_GETLINEENDPOSITION, line) - SendEditor(SCI_POSITIONFROMLINE, line);
}

int SciTEBase::GetCurrentLineNumber() {
	return SendEditor(SCI_LINEFROMPOSITION,
	                  SendEditor(SCI_GETCURRENTPOS));
}

int SciTEBase::GetCurrentScrollPosition() {
	int lineDisplayTop = SendEditor(SCI_GETFIRSTVISIBLELINE);
	return SendEditor(SCI_DOCLINEFROMVISIBLE, lineDisplayTop);
}

/**
 * Set up properties for ReadOnly, EOLMode, BufferLength, NbOfLines, SelLength, SelHeight.
 */
void SciTEBase::SetTextProperties(
    PropSetFile &ps) {			///< Property set to update.

	const int TEMP_LEN = 100;
	char temp[TEMP_LEN];

	SString ro = LocaliseString("READ");
	ps.Set("ReadOnly", isReadOnly ? ro.c_str() : "");

	int eolMode = SendEditor(SCI_GETEOLMODE);
	ps.Set("EOLMode", eolMode == SC_EOL_CRLF ? "CR+LF" : (eolMode == SC_EOL_LF ? "LF" : "CR"));

	sprintf(temp, "%d", LengthDocument());
	ps.Set("BufferLength", temp);

	ps.SetInteger("NbOfLines", SendEditor(SCI_GETLINECOUNT));

	CharacterRange crange = GetSelection();
	sprintf(temp, "%ld", crange.cpMax - crange.cpMin);
	ps.Set("SelLength", temp);
	int selFirstLine = SendEditor(SCI_LINEFROMPOSITION, crange.cpMin);
	int selLastLine = SendEditor(SCI_LINEFROMPOSITION, crange.cpMax);
	int caretPos = SendEditor(SCI_GETCURRENTPOS);
	int selAnchor = SendEditor(SCI_GETANCHOR);
	if (0 == (crange.cpMax - crange.cpMin)) {
		sprintf(temp, "%d", 0);
	} else if (selLastLine == selFirstLine) {
		sprintf(temp, "%d", 1);
	} else if ((SendEditor(SCI_GETCOLUMN, caretPos) == 0 && (selAnchor <= caretPos)) ||
 		((SendEditor( SCI_GETCOLUMN, selAnchor) == 0) && (selAnchor > caretPos ))) {
		sprintf(temp, "%d", selLastLine - selFirstLine);
	} else {
		sprintf(temp, "%d", selLastLine - selFirstLine + 1);
	}
	ps.Set("SelHeight", temp);
}

void SciTEBase::UpdateStatusBar(bool bUpdateSlowData) {
	if (sbVisible) {
		if (bUpdateSlowData) {
			SetFileProperties(propsStatus);
		}
		SetTextProperties(propsStatus);
		int caretPos = SendEditor(SCI_GETCURRENTPOS);
		propsStatus.SetInteger("LineNumber",
		                       SendEditor(SCI_LINEFROMPOSITION, caretPos) + 1);
		propsStatus.SetInteger("ColumnNumber",
		                       SendEditor(SCI_GETCOLUMN, caretPos) + 1);
		propsStatus.Set("OverType", SendEditor(SCI_GETOVERTYPE) ? "OVR" : "INS");

		char sbKey[32];
		sprintf(sbKey, "statusbar.text.%d", sbNum);
		SString msg = propsStatus.GetExpanded(sbKey);
		if (msg.length() && sbValue != msg) {	// To avoid flickering, update only if needed
			SetStatusBarText(msg.c_str());
			sbValue = msg;
		}
	} else {
		sbValue = "";
	}
}

void SciTEBase::SetLineIndentation(int line, int indent) {
	if (indent < 0)
		return;
	CharacterRange crange = GetSelection();
	int posBefore = GetLineIndentPosition(line);
	SendEditor(SCI_SETLINEINDENTATION, line, indent);
	int posAfter = GetLineIndentPosition(line);
	int posDifference = posAfter - posBefore;
	if (posAfter > posBefore) {
		// Move selection on
		if (crange.cpMin >= posBefore) {
			crange.cpMin += posDifference;
		}
		if (crange.cpMax >= posBefore) {
			crange.cpMax += posDifference;
		}
	} else if (posAfter < posBefore) {
		// Move selection back
		if (crange.cpMin >= posAfter) {
			if (crange.cpMin >= posBefore)
				crange.cpMin += posDifference;
			else
				crange.cpMin = posAfter;
		}
		if (crange.cpMax >= posAfter) {
			if (crange.cpMax >= posBefore)
				crange.cpMax += posDifference;
			else
				crange.cpMax = posAfter;
		}
	}
	SetSelection(crange.cpMin, crange.cpMax);
}

int SciTEBase::GetLineIndentation(int line) {
	return SendEditor(SCI_GETLINEINDENTATION, line);
}

int SciTEBase::GetLineIndentPosition(int line) {
	return SendEditor(SCI_GETLINEINDENTPOSITION, line);
}

bool SciTEBase::RangeIsAllWhitespace(int start, int end) {
	WindowAccessor acc(wEditor.GetID(), props);
	for (int i = start;i < end;i++) {
		if ((acc[i] != ' ') && (acc[i] != '\t'))
			return false;
	}
	return true;
}

unsigned int SciTEBase::GetLinePartsInStyle(int line, int style1, int style2, SString sv[], int len) {
	for (int i = 0; i < len; i++)
		sv[i] = "";
	WindowAccessor acc(wEditor.GetID(), props);
	SString s;
	int part = 0;
	int thisLineStart = SendEditor(SCI_POSITIONFROMLINE, line);
	int nextLineStart = SendEditor(SCI_POSITIONFROMLINE, line + 1);
	for (int pos = thisLineStart; pos < nextLineStart; pos++) {
		if ((acc.StyleAt(pos) == style1) || (acc.StyleAt(pos) == style2)) {
			char c[2];
			c[0] = acc[pos];
			c[1] = '\0';
			s += c;
		} else if (s.length() > 0) {
			if (part < len) {
				sv[part++] = s;
			}
			s = "";
		}
	}
	if ((s.length() > 0) && (part < len)) {
		sv[part++] = s;
	}
	return part;
}

static bool includes(const StyleAndWords &symbols, const SString value) {
	if (symbols.words.length() == 0) {
		return false;
	} else if (IsAlphabetic(symbols.words[0])) {
		// Set of symbols separated by spaces
		size_t lenVal = value.length();
		const char *symbol = symbols.words.c_str();
		while (symbol) {
			const char *symbolEnd = strchr(symbol, ' ');
			size_t lenSymbol = strlen(symbol);
			if (symbolEnd)
				lenSymbol = symbolEnd - symbol;
			if (lenSymbol == lenVal) {
				if (strncmp(symbol, value.c_str(), lenSymbol) == 0) {
					return true;
				}
			}
			symbol = symbolEnd;
			if (symbol)
				symbol++;
		}
	} else {
		// Set of individual characters. Only one character allowed for now
		char ch = symbols.words[0];
		return strchr(value.c_str(), ch) != 0;
	}
	return false;
}

#define ELEMENTS(a)	(sizeof(a) / sizeof(a[0]))

IndentationStatus SciTEBase::GetIndentState(int line) {
	// C like language indentation defined by braces and keywords
	IndentationStatus indentState = isNone;
	SString controlWords[20];
	unsigned int parts = GetLinePartsInStyle(line, statementIndent.styleNumber,
	                     -1, controlWords, ELEMENTS(controlWords));
	for (unsigned int i = 0; i < parts; i++) {
		if (includes(statementIndent, controlWords[i]))
			indentState = isKeyWordStart;
	}
	// Braces override keywords
	SString controlStrings[20];
	parts = GetLinePartsInStyle(line, blockEnd.styleNumber,
	                            -1, controlStrings, ELEMENTS(controlStrings));
	for (unsigned int j = 0; j < parts; j++) {
		if (includes(blockEnd, controlStrings[j]))
			indentState = isBlockEnd;
		if (includes(blockStart, controlStrings[j]))
			indentState = isBlockStart;
	}
	return indentState;
}

int SciTEBase::IndentOfBlock(int line) {
	if (line < 0)
		return 0;
	int indentSize = SendEditor(SCI_GETINDENT);
	int indentBlock = GetLineIndentation(line);
	int backLine = line;
	IndentationStatus indentState = isNone;
	if (statementIndent.IsEmpty() && blockStart.IsEmpty() && blockEnd.IsEmpty())
		indentState = isBlockStart;	// Don't bother searching backwards

	int lineLimit = line - statementLookback;
	if (lineLimit < 0)
		lineLimit = 0;
	while ((backLine >= lineLimit) && (indentState == 0)) {
		indentState = GetIndentState(backLine);
		if (indentState != 0) {
			indentBlock = GetLineIndentation(backLine);
			if (indentState == isBlockStart) {
				if (!indentOpening)
					indentBlock += indentSize;
			}
			if (indentState == isBlockEnd) {
				if (indentClosing)
					indentBlock -= indentSize;
				if (indentBlock < 0)
					indentBlock = 0;
			}
			if ((indentState == isKeyWordStart) && (backLine == line))
				indentBlock += indentSize;
		}
		backLine--;
	}
	return indentBlock;
}

void SciTEBase::MaintainIndentation(char ch) {
	int eolMode = SendEditor(SCI_GETEOLMODE);
	int curLine = GetCurrentLineNumber();
	int lastLine = curLine - 1;
	int indentAmount = 0;

	if (((eolMode == SC_EOL_CRLF || eolMode == SC_EOL_LF) && ch == '\n') ||
	        (eolMode == SC_EOL_CR && ch == '\r')) {
		if (props.GetInt("indent.automatic")) {
			while (lastLine >= 0 && GetLineLength(lastLine) == 0)
				lastLine--;
		}
		if (lastLine >= 0) {
			indentAmount = GetLineIndentation(lastLine);
		}
		if (indentAmount > 0) {
			SetLineIndentation(curLine, indentAmount);
		}
	}
}

void SciTEBase::AutomaticIndentation(char ch) {
	CharacterRange crange = GetSelection();
	int selStart = crange.cpMin;
	int curLine = GetCurrentLineNumber();
	int thisLineStart = SendEditor(SCI_POSITIONFROMLINE, curLine);
	int indentSize = SendEditor(SCI_GETINDENT);
	int indentBlock = IndentOfBlock(curLine - 1);

	if (blockEnd.IsSingleChar() && ch == blockEnd.words[0]) {	// Dedent maybe
		if (!indentClosing) {
			if (RangeIsAllWhitespace(thisLineStart, selStart - 1)) {
				SetLineIndentation(curLine, indentBlock - indentSize);
			}
		}
	} else if (!blockEnd.IsSingleChar() && (ch == ' ')) {	// Dedent maybe
		if (!indentClosing && (GetIndentState(curLine) == isBlockEnd)) {}
	} else if (blockStart.IsSingleChar() && (ch == blockStart.words[0])) {
		// Dedent maybe if first on line and previous line was starting keyword
		if (!indentOpening && (GetIndentState(curLine - 1) == isKeyWordStart)) {
			if (RangeIsAllWhitespace(thisLineStart, selStart - 1)) {
				SetLineIndentation(curLine, indentBlock - indentSize);
			}
		}
	} else if ((ch == '\r' || ch == '\n') && (selStart == thisLineStart)) {
		if (!indentClosing && !blockEnd.IsSingleChar()) {	// Dedent previous line maybe
			SString controlWords[1];
			if (GetLinePartsInStyle(curLine - 1, blockEnd.styleNumber,
			                        -1, controlWords, ELEMENTS(controlWords))) {
				if (includes(blockEnd, controlWords[0])) {
					// Check if first keyword on line is an ender
					SetLineIndentation(curLine - 1, IndentOfBlock(curLine - 2) - indentSize);
					// Recalculate as may have changed previous line
					indentBlock = IndentOfBlock(curLine - 1);
				}
			}
		}
		SetLineIndentation(curLine, indentBlock);
	}
}

/**
 * Upon a character being added, SciTE may decide to perform some action
 * such as displaying a completion list or auto-indentation.
 */
void SciTEBase::CharAdded(char ch) {
	if (recording)
		return;
	CharacterRange crange = GetSelection();
	int selStart = crange.cpMin;
	int selEnd = crange.cpMax;
	if ((selEnd == selStart) && (selStart > 0)) {
		int style = SendEditor(SCI_GETSTYLEAT, selStart - 1, 0);
		//Platform::DebugPrintf("Char added %d style = %d %d\n", ch, style, braceCount);
		if (style != 1) {
			if (SendEditor(SCI_CALLTIPACTIVE)) {
				if (calltipParametersEnd.contains(ch)) {
					braceCount--;
					if (braceCount < 1)
						SendEditor(SCI_CALLTIPCANCEL);
					else
						StartCallTip();
				} else if (calltipParametersStart.contains(ch)) {
					braceCount++;
					StartCallTip();
				} else {
					ContinueCallTip();
				}
			} else if (SendEditor(SCI_AUTOCACTIVE)) {
				if (calltipParametersStart.contains(ch)) {
					braceCount++;
					StartCallTip();
				} else if (calltipParametersEnd.contains(ch)) {
					braceCount--;
				} else if (!wordCharacters.contains(ch)) {
					SendEditor(SCI_AUTOCCANCEL);
					if (autoCompleteStartCharacters.contains(ch)) {
						StartAutoComplete();
					}
				} else if (autoCCausedByOnlyOne) {
					StartAutoCompleteWord(true);
				}
			} else if (HandleXml(ch)) {
				// Handled in the routine
			} else {
				if (calltipParametersStart.contains(ch)) {
					braceCount = 1;
					StartCallTip();
				} else {
					autoCCausedByOnlyOne = false;
					if (indentMaintain)
						MaintainIndentation(ch);
					else if (props.GetInt("indent.automatic"))
						AutomaticIndentation(ch);
					if (autoCompleteStartCharacters.contains(ch)) {
						StartAutoComplete();
					} else if (props.GetInt("autocompleteword.automatic") && wordCharacters.contains(ch)) {
						StartAutoCompleteWord(true);
						autoCCausedByOnlyOne = SendEditor(SCI_AUTOCACTIVE);
					}
				}
			}
		}
	}
}

/**
 * This routine will auto complete XML or HTML tags that are still open by closing them
 * @parm ch The characer we are dealing with, currently only works with the '>' character
 * @return True if handled, false otherwise
 */
bool SciTEBase::HandleXml(char ch) {
	// We're looking for this char
	// Quit quickly if not found
	if (ch != '>') {
		return false;
	}

	// This may make sense only in certain languages
	if (lexLanguage != SCLEX_HTML && lexLanguage != SCLEX_XML &&
	        lexLanguage != SCLEX_ASP && lexLanguage != SCLEX_PHP) {
		return false;
	}

	// If the user has turned us off, quit now.
	// Default is off
	SString value = props.GetExpanded("xml.auto.close.tags");
	if ((value.length() == 0) || (value == "0")) {
		return false;
	}

	// Grab the last 512 characters or so
	int nCaret = SendEditor(SCI_GETCURRENTPOS);
	char sel[512];
	int nMin = nCaret - (sizeof(sel) - 1);
	if (nMin < 0) {
		nMin = 0;
	}

	if (nCaret - nMin < 3) {
		return false; // Smallest tag is 3 characters ex. <p>
	}
	GetRange(wEditor, nMin, nCaret, sel);
	sel[sizeof(sel) - 1] = '\0';

	if (sel[nCaret - nMin - 2] == '/') {
		// User typed something like "<br/>"
		return false;
	}

	SString strFound = FindOpenXmlTag(sel, nCaret - nMin);

	if (strFound.length() > 0) {
		SendEditor(SCI_BEGINUNDOACTION);
		SString toInsert = "</";
		toInsert += strFound;
		toInsert += ">";
		SendEditorString(SCI_REPLACESEL, 0, toInsert.c_str());
		SetSelection(nCaret, nCaret);
		SendEditor(SCI_ENDUNDOACTION);
		return true;
	}

	return false;
}

/** Search backward through nSize bytes looking for a '<', then return the tag if any
 * @return The tag name
 */
SString SciTEBase::FindOpenXmlTag(const char sel[], int nSize) {
	SString strRet = "";

	if (nSize < 3) {
		// Smallest tag is "<p>" which is 3 characters
		return strRet;
	}
	const char* pBegin = &sel[0];
	const char* pCur = &sel[nSize - 1];

	pCur--; // Skip past the >
	while (pCur > pBegin) {
		if (*pCur == '<') {
			break;
		} else if (*pCur == '>') {
			break;
		}
		--pCur;
	}

	if (*pCur == '<') {
		pCur++;
		while (strchr(":_-.", *pCur) || isalnum(*pCur)) {
			strRet += *pCur;
			pCur++;
		}
	}

	// Return the tag name or ""
	return strRet;
}

void SciTEBase::GoMatchingBrace(bool select) {
	int braceAtCaret = -1;
	int braceOpposite = -1;
	bool isInside = FindMatchingBracePosition(true, braceAtCaret, braceOpposite, true);
	// Convert the character positions into caret positions based on whether
	// the caret position was inside or outside the braces.
	if (isInside) {
		if (braceOpposite > braceAtCaret) {
			braceAtCaret++;
		} else {
			braceOpposite++;
		}
	} else {    // Outside
		if (braceOpposite > braceAtCaret) {
			braceOpposite++;
		} else {
			braceAtCaret++;
		}
	}
	if (braceOpposite >= 0) {
		EnsureRangeVisible(braceOpposite, braceOpposite);
		if (select) {
			SetSelection(braceAtCaret, braceOpposite);
		} else {
			SetSelection(braceOpposite, braceOpposite);
		}
	}
}

// Text	ConditionalUp	Ctrl+J	Finds the previous matching preprocessor condition
// Text	ConditionalDown	Ctrl+K	Finds the next matching preprocessor condition
void SciTEBase::GoMatchingPreprocCond(int direction, bool select) {
	int mppcAtCaret = SendEditor(SCI_GETCURRENTPOS);
	int mppcMatch = -1;
	int forward = (direction == IDM_NEXTMATCHPPC);
	bool isInside = FindMatchingPreprocCondPosition(forward, mppcAtCaret, mppcMatch);

	if (isInside && mppcMatch >= 0) {
		EnsureRangeVisible(mppcMatch, mppcMatch);
		if (select) {
			// Selection changes the rules a bit...
			int selStart = SendEditor(SCI_GETSELECTIONSTART);
			int selEnd = SendEditor(SCI_GETSELECTIONEND);
			// pivot isn't the caret position but the opposite (if there is a selection)
			int pivot = (mppcAtCaret == selStart ? selEnd : selStart);
			if (forward) {
				// Caret goes one line beyond the target, to allow selecting the whole line
				int lineNb = SendEditor(SCI_LINEFROMPOSITION, mppcMatch);
				mppcMatch = SendEditor(SCI_POSITIONFROMLINE, lineNb + 1);
			}
			SetSelection(pivot, mppcMatch);
		} else {
			SetSelection(mppcMatch, mppcMatch);
		}
	} else {
		WarnUser(warnNotFound);
	}
}

void SciTEBase::AddCommand(const SString &cmd, const SString &dir, JobSubsystem jobType, const SString &input, int flags) {
	if (commandCurrent >= commandMax)
		return;
	if (commandCurrent == 0)
		jobUsesOutputPane = false;
	if (cmd.length()) {
		jobQueue[commandCurrent].command = cmd;
		jobQueue[commandCurrent].directory.Set(dir.c_str());
		jobQueue[commandCurrent].jobType = jobType;
		jobQueue[commandCurrent].input = input;
		jobQueue[commandCurrent].flags = flags;
		commandCurrent++;
		if (jobType == jobCLI)
			jobUsesOutputPane = true;
		// For jobExtension, the Trace() method shows output pane on demand.
	}
}

int ControlIDOfCommand(unsigned long wParam) {
	return wParam & 0xffff;
}

void WindowSetFocus(Window &w) {
	Platform::SendScintilla(w.GetID(), SCI_GRABFOCUS, 0, 0);
}

void SciTEBase::SetLineNumberWidth() {
	if (lineNumbers) {
		int lineNumWidth = lineNumbersWidth;

		if (lineNumbersExpand) {
			// The margin size will be expanded if the current buffer's maximum
			// line number would overflow the margin.

			int lineCount = SendEditor(SCI_GETLINECOUNT);

			lineNumWidth = 1;
			while (lineCount >= 10) {
				lineCount /= 10;
				++lineNumWidth;
			}

			if (lineNumWidth < lineNumbersWidth) {
				lineNumWidth = lineNumbersWidth;
			}
		}

		// The 4 here allows for spacing: 1 pixel on left and 3 on right.
		int pixelWidth = 4 + lineNumWidth * SendEditorString(SCI_TEXTWIDTH, STYLE_LINENUMBER, "9");

		SendEditor(SCI_SETMARGINWIDTHN, 0, pixelWidth);
	} else {
		SendEditor(SCI_SETMARGINWIDTHN, 0, 0);
	}
}

void SciTEBase::MenuCommand(int cmdID, int source) {
	switch (cmdID) {
	case IDM_NEW:
		// For the New command, the "are you sure" question is always asked as this gives
		// an opportunity to abandon the edits made to a file when are.you.sure is turned off.
		if (CanMakeRoom()) {
			New();
			ReadProperties();
			SetIndentSettings();
			if (props.GetInt("eol.auto"))
				SetEol();
			UpdateStatusBar(true);
			WindowSetFocus(wEditor);
		}
		break;
	case IDM_OPEN:
		// No need to see if can make room as that will occur
		// when doing the opening. Must be done there as user
		// may decide to open multiple files so do not know yet
		// how much room needed.
		OpenDialog(filePath.Directory(), props.GetExpanded("open.filter").c_str());
		WindowSetFocus(wEditor);
		break;
	case IDM_OPENSELECTED:
		if (OpenSelected())
			WindowSetFocus(wEditor);
		break;
	case IDM_CLOSE:
		if (SaveIfUnsure() != IDCANCEL) {
			Close();
			WindowSetFocus(wEditor);
		}
		break;
	case IDM_CLOSEALL:
		CloseAllBuffers();
		break;
	case IDM_SAVE:
		Save();
		WindowSetFocus(wEditor);
		break;
	case IDM_SAVEALL:
		SaveAllBuffers(false, true);
		break;
	case IDM_SAVEAS:
		SaveAsDialog();
		WindowSetFocus(wEditor);
		break;
	case IDM_SAVEACOPY:
		SaveACopy();
		WindowSetFocus(wEditor);
		break;
	case IDM_SAVEASHTML:
		SaveAsHTML();
		WindowSetFocus(wEditor);
		break;
	case IDM_SAVEASRTF:
		SaveAsRTF();
		WindowSetFocus(wEditor);
		break;
	case IDM_SAVEASPDF:
		SaveAsPDF();
		WindowSetFocus(wEditor);
		break;
	case IDM_SAVEASTEX:
		SaveAsTEX();
		WindowSetFocus(wEditor);
		break;
	case IDM_SAVEASXML:
		SaveAsXML();
		WindowSetFocus(wEditor);
		break;
	case IDM_REVERT:
		Revert();
		WindowSetFocus(wEditor);
		break;
	case IDM_PRINT:
		Print(true);
		break;
	case IDM_PRINTSETUP:
		PrintSetup();
		break;
	case IDM_LOADSESSION:
		LoadSessionDialog();
		WindowSetFocus(wEditor);
		break;
	case IDM_SAVESESSION:
		SaveSessionDialog();
		WindowSetFocus(wEditor);
		break;
	case IDM_ABOUT:
		AboutDialog();
		break;
	case IDM_QUIT:
		QuitProgram();
		break;
	case IDM_ENCODING_DEFAULT:
	case IDM_ENCODING_UCS2BE:
	case IDM_ENCODING_UCS2LE:
	case IDM_ENCODING_UTF8:
	case IDM_ENCODING_UCOOKIE:
		unicodeMode = static_cast<UniMode>(cmdID - IDM_ENCODING_DEFAULT);
		if (unicodeMode != uni8Bit) {
			// Override the code page if Unicode
			codePage = SC_CP_UTF8;
		} else {
			codePage = props.GetInt("code.page");
		}
		SendEditor(SCI_SETCODEPAGE, codePage);
		break;

	case IDM_NEXTFILE:
		if (buffers.size > 1) {
			Next(); // Use Next to tabs move left-to-right
			WindowSetFocus(wEditor);
		} else {
			// Not using buffers - switch to next file on MRU
			StackMenuNext();
		}
		break;
	case IDM_PREVFILE:
		if (buffers.size > 1) {
			Prev(); // Use Prev to tabs move right-to-left
			WindowSetFocus(wEditor);
		} else {
			// Not using buffers - switch to previous file on MRU
			StackMenuPrev();
		}
		break;

	case IDM_UNDO:
		SendPane(source, SCI_UNDO);
		CheckMenus();
		break;
	case IDM_REDO:
		SendPane(source, SCI_REDO);
		CheckMenus();
		break;

	case IDM_CUT:
		if (SendPane(source, SCI_GETSELECTIONSTART) != SendPane(source, SCI_GETSELECTIONEND)) {
			SendPane(source, SCI_CUT);
		}
		break;
	case IDM_COPY:
		if (SendPane(source, SCI_GETSELECTIONSTART) != SendPane(source, SCI_GETSELECTIONEND)) {
			//fprintf(stderr, "Copy from %d\n", source);
			SendPane(source, SCI_COPY);
		}
		// does not trigger SCN_UPDATEUI, so do CheckMenusClipboard() here
		CheckMenusClipboard();
		break;
	case IDM_PASTE:
		SendPane(source, SCI_PASTE);
		break;
	case IDM_PASTEANDDOWN: {
			int pos = SendFocused(SCI_GETCURRENTPOS);
			SendFocused(SCI_PASTE);
			SendFocused(SCI_SETCURRENTPOS, pos);
			SendFocused(SCI_CHARLEFT);
			SendFocused(SCI_LINEDOWN);
		}
		break;
	case IDM_CLEAR:
		SendPane(source, SCI_CLEAR);
		break;
	case IDM_SELECTALL:
		SendPane(source, SCI_SELECTALL);
		break;
	case IDM_COPYASRTF:
		CopyAsRTF();
		break;

	case IDM_FIND:
		Find();
		break;

	case IDM_INCSEARCH:
		IncrementSearchMode();
		break;

	case IDM_FINDNEXT:
		FindNext(reverseFind);
		break;

	case IDM_FINDNEXTBACK:
		FindNext(!reverseFind);
		break;

	case IDM_FINDNEXTSEL:
		SelectionIntoFind();
		FindNext(reverseFind);
		break;

	case IDM_ENTERSELECTION:
		SelectionIntoFind();
		break;

	case IDM_FINDNEXTBACKSEL:
		SelectionIntoFind();
		FindNext(!reverseFind);
		break;

	case IDM_FINDINFILES:
		FindInFiles();
		break;

	case IDM_REPLACE:
		Replace();
		break;

	case IDM_GOTO:
		GoLineDialog();
		break;

	case IDM_MATCHBRACE:
		GoMatchingBrace(false);
		break;

	case IDM_SELECTTOBRACE:
		GoMatchingBrace(true);
		break;

	case IDM_PREVMATCHPPC:
		GoMatchingPreprocCond(IDM_PREVMATCHPPC, false);
		break;

	case IDM_SELECTTOPREVMATCHPPC:
		GoMatchingPreprocCond(IDM_PREVMATCHPPC, true);
		break;

	case IDM_NEXTMATCHPPC:
		GoMatchingPreprocCond(IDM_NEXTMATCHPPC, false);
		break;

	case IDM_SELECTTONEXTMATCHPPC:
		GoMatchingPreprocCond(IDM_NEXTMATCHPPC, true);
		break;

	case IDM_SHOWCALLTIP:
		StartCallTip();
		break;

	case IDM_COMPLETE:
		autoCCausedByOnlyOne = false;
		StartAutoComplete();
		break;

	case IDM_COMPLETEWORD:
		autoCCausedByOnlyOne = false;
		StartAutoCompleteWord(false);
		break;

	case IDM_ABBREV:
		SendEditor(SCI_CANCEL);
		StartExpandAbbreviation();
		break;

	case IDM_INS_ABBREV:
		SendEditor(SCI_CANCEL);
		StartInsertAbbreviation();
		break;

	case IDM_BLOCK_COMMENT:
		StartBlockComment();
		break;

	case IDM_BOX_COMMENT:
		StartBoxComment();
		break;

	case IDM_STREAM_COMMENT:
		StartStreamComment();
		break;

	case IDM_TOGGLE_FOLDALL:
		FoldAll();
		break;

	case IDM_UPRCASE:
		SendFocused(SCI_UPPERCASE);
		break;

	case IDM_LWRCASE:
		SendFocused(SCI_LOWERCASE);
		break;

	case IDM_JOIN:
		SendFocused(SCI_TARGETFROMSELECTION);
		SendFocused(SCI_LINESJOIN);
		break;

	case IDM_SPLIT:
		SendFocused(SCI_TARGETFROMSELECTION);
		SendFocused(SCI_LINESSPLIT);
		break;

	case IDM_EXPAND:
		SendEditor(SCI_TOGGLEFOLD, GetCurrentLineNumber());
		break;

	case IDM_TOGGLE_FOLDRECURSIVE: {
			int line = GetCurrentLineNumber();
			int level = SendEditor(SCI_GETFOLDLEVEL, line);
			ToggleFoldRecursive(line, level);
		}
		break;

	case IDM_EXPAND_ENSURECHILDRENVISIBLE: {
			int line = GetCurrentLineNumber();
			int level = SendEditor(SCI_GETFOLDLEVEL, line);
			EnsureAllChildrenVisible(line, level);
		}
		break;

	case IDM_SPLITVERTICAL:
		splitVertical = !splitVertical;
		heightOutput = NormaliseSplit(heightOutput);
		SizeSubWindows();
		CheckMenus();
		Redraw();
		break;

	case IDM_LINENUMBERMARGIN:
		lineNumbers = !lineNumbers;
		SetLineNumberWidth();
		CheckMenus();
		break;

	case IDM_SELMARGIN:
		margin = !margin;
		SendEditor(SCI_SETMARGINWIDTHN, 1, margin ? marginWidth : 0);
		CheckMenus();
		break;

	case IDM_FOLDMARGIN:
		foldMargin = !foldMargin;
		SendEditor(SCI_SETMARGINWIDTHN, 2, foldMargin ? foldMarginWidth : 0);
		CheckMenus();
		break;

	case IDM_VIEWEOL:
		SendEditor(SCI_SETVIEWEOL, !SendEditor(SCI_GETVIEWEOL));
		CheckMenus();
		break;

	case IDM_VIEWTOOLBAR:
		tbVisible = !tbVisible;
		ShowToolBar();
		CheckMenus();
		break;

	case IDM_TOGGLEOUTPUT:
		ToggleOutputVisible();
		CheckMenus();
		break;
/*
	case IDM_TOGGLEPARAMETERS:
		ParametersDialog(false);
		CheckMenus();
		break;
*/
	case IDM_WRAP:
		wrap = !wrap;
		SendEditor(SCI_SETWRAPMODE, wrap ? wrapStyle : SC_WRAP_NONE);
		CheckMenus();
		break;

	case IDM_WRAPOUTPUT:
		wrapOutput = !wrapOutput;
		SendOutput(SCI_SETWRAPMODE, wrapOutput ? wrapStyle : SC_WRAP_NONE);
		CheckMenus();
		break;

	case IDM_READONLY:
		isReadOnly = !isReadOnly;
		SendEditor(SCI_SETREADONLY, isReadOnly);
		UpdateStatusBar(true);
		CheckMenus();
		break;

	case IDM_VIEWTABBAR:
		tabVisible = !tabVisible;
		ShowTabBar();
		CheckMenus();
		break;

	case IDM_VIEWSTATUSBAR:
		sbVisible = !sbVisible;
		ShowStatusBar();
		UpdateStatusBar(true);
		CheckMenus();
		break;

	case IDM_CLEAROUTPUT:
		SendOutputEx(SCI_CLEARALL, 0, 0, false);
		break;

	case IDM_SWITCHPANE:
		if (wEditor.HasFocus())
			WindowSetFocus(wOutput);
		else
			WindowSetFocus(wEditor);
		break;

	case IDM_EOL_CRLF:
		SendEditor(SCI_SETEOLMODE, SC_EOL_CRLF);
		CheckMenus();
		UpdateStatusBar(false);
		break;

	case IDM_EOL_CR:
		SendEditor(SCI_SETEOLMODE, SC_EOL_CR);
		CheckMenus();
		UpdateStatusBar(false);
		break;
	case IDM_EOL_LF:
		SendEditor(SCI_SETEOLMODE, SC_EOL_LF);
		CheckMenus();
		UpdateStatusBar(false);
		break;
	case IDM_EOL_CONVERT:
		SendEditor(SCI_CONVERTEOLS, SendEditor(SCI_GETEOLMODE));
		break;

	case IDM_VIEWSPACE:
		ViewWhitespace(!SendEditor(SCI_GETVIEWWS));
		CheckMenus();
		Redraw();
		break;

	case IDM_VIEWGUIDES: {
			int viewIG = SendEditor(SCI_GETINDENTATIONGUIDES, 0, 0);
			SendEditor(SCI_SETINDENTATIONGUIDES, !viewIG);
			CheckMenus();
			Redraw();
		}
		break;

#ifdef GLI_CHANGES

	case IDM_COMPILE: {

      //Compile the current shader with GLIntercept
      CompileGLIShader();
    }
		break;

  case IDM_TOGGLEBREAKPOINT: {

      //Get the current line in the current buffer
      int currentPosition = SendEditor(SCI_GETCURRENTPOS);
      int currLineNum     = SendEditor(SCI_LINEFROMPOSITION, currentPosition);

      //Toggle the breakpoint
      ToggleBreakpoint(currLineNum);
    }
    break;

#endif //GLI_CHANGES

/*
	case IDM_COMPILE: {

			if (SaveIfUnsureForBuilt() != IDCANCEL) {
				SelectionIntoProperties();
				AddCommand(props.GetWild("command.compile.", filePath.AsInternal()), "",
				           SubsystemType("command.compile.subsystem."));
				if (commandCurrent > 0)
					Execute();
			}
		}
		break;

	case IDM_BUILD: {
			if (SaveIfUnsureForBuilt() != IDCANCEL) {
				SelectionIntoProperties();
				AddCommand(
				    props.GetWild("command.build.", filePath.AsInternal()),
				    props.GetNewExpand("command.build.directory.", filePath.AsInternal()),
				    SubsystemType("command.build.subsystem."));
				if (commandCurrent > 0) {
					isBuilding = true;
					Execute();
				}
			}
		}
		break;

	case IDM_GO: {
			if (SaveIfUnsureForBuilt() != IDCANCEL) {
				SelectionIntoProperties();
				long flags = 0;

				if (!isBuilt) {
					SString buildcmd = props.GetNewExpand("command.go.needs.", filePath.AsInternal());
					AddCommand(buildcmd, "",
					           SubsystemType("command.go.needs.subsystem."));
					if (buildcmd.length() > 0) {
						isBuilding = true;
						flags |= jobForceQueue;
					}
				}
				AddCommand(props.GetWild("command.go.", filePath.AsInternal()), "",
				           SubsystemType("command.go.subsystem."), "", flags);
				if (commandCurrent > 0)
					Execute();
			}
		}
		break;

	case IDM_STOPEXECUTE:
		StopExecute();
		break;
*/
	case IDM_NEXTMSG:
		GoMessage(1);
		break;

	case IDM_PREVMSG:
		GoMessage(-1);
		break;

	case IDM_OPENLOCALPROPERTIES:
		OpenProperties(IDM_OPENLOCALPROPERTIES);
		WindowSetFocus(wEditor);
		break;

	case IDM_OPENUSERPROPERTIES:
		OpenProperties(IDM_OPENUSERPROPERTIES);
		WindowSetFocus(wEditor);
		break;

	case IDM_OPENGLOBALPROPERTIES:
		OpenProperties(IDM_OPENGLOBALPROPERTIES);
		WindowSetFocus(wEditor);
		break;

	case IDM_OPENABBREVPROPERTIES:
		OpenProperties(IDM_OPENABBREVPROPERTIES);
		WindowSetFocus(wEditor);
		break;

	case IDM_OPENLUAEXTERNALFILE:
		OpenProperties(IDM_OPENLUAEXTERNALFILE);
		WindowSetFocus(wEditor);
		break;

	case IDM_SRCWIN:
		break;

	case IDM_BOOKMARK_TOGGLE:
		BookmarkToggle();
		break;

	case IDM_BOOKMARK_NEXT:
		BookmarkNext(true);
		break;

	case IDM_BOOKMARK_PREV:
		BookmarkNext(false);
		break;

	case IDM_BOOKMARK_NEXT_SELECT:
		BookmarkNext(true, true);
		break;

	case IDM_BOOKMARK_PREV_SELECT:
		BookmarkNext(false, true);
		break;

	case IDM_BOOKMARK_CLEARALL:
		SendEditor(SCI_MARKERDELETEALL, SciTE_MARKER_BOOKMARK);
		break;

	case IDM_TABSIZE:
		TabSizeDialog();
		break;

	case IDM_MONOFONT:
		useMonoFont = !useMonoFont;
		ReadFontProperties();
		Redraw();
		break;

	case IDM_MACROLIST:
		AskMacroList();
		break;
	case IDM_MACROPLAY:
		StartPlayMacro();
		break;
	case IDM_MACRORECORD:
		StartRecordMacro();
		break;
	case IDM_MACROSTOPRECORD:
		StopRecordMacro();
		break;

	case IDM_HELP: {
			SelectionIntoProperties();
			AddCommand(props.GetWild("command.help.", filePath.AsInternal()), "",
			           SubsystemType("command.help.subsystem."));
			if (commandCurrent > 0) {
				isBuilding = true;
				Execute();
			}
		}
		break;

	case IDM_HELP_SCITE: {
			SelectionIntoProperties();
			AddCommand(props.Get("command.scite.help"), "",
			           SubsystemType(props.Get("command.scite.help.subsystem")[0]));
			if (commandCurrent > 0) {
				isBuilding = true;
				Execute();
			}
		}
		break;

	default:
		if ((cmdID >= bufferCmdID) &&
		        (cmdID < bufferCmdID + buffers.size)) {
			SetDocumentAt(cmdID - bufferCmdID);
			CheckReload();
		} else if ((cmdID >= fileStackCmdID) &&
		           (cmdID < fileStackCmdID + fileStackMax)) {
			StackMenu(cmdID - fileStackCmdID);
		} else if (cmdID >= importCmdID &&
		           (cmdID < importCmdID + importMax)) {
			ImportMenu(cmdID - importCmdID);
		} else if (cmdID >= IDM_TOOLS && cmdID < IDM_TOOLS + toolMax) {
			ToolsMenu(cmdID - IDM_TOOLS);
		} else if (cmdID >= IDM_LANGUAGE && cmdID < IDM_LANGUAGE + 100) {
			SetOverrideLanguage(cmdID - IDM_LANGUAGE);
		}
		break;
	}
}

void SciTEBase::FoldChanged(int line, int levelNow, int levelPrev) {
	//Platform::DebugPrintf("Fold %d %x->%x\n", line, levelPrev, levelNow);
	if (levelNow & SC_FOLDLEVELHEADERFLAG) {
		if (!(levelPrev & SC_FOLDLEVELHEADERFLAG)) {
			// Adding a fold point.
			SendEditor(SCI_SETFOLDEXPANDED, line, 1);
			Expand(line, true, false, 0, levelPrev);
		}
	} else if (levelPrev & SC_FOLDLEVELHEADERFLAG) {
		//Platform::DebugPrintf("Fold removed %d-%d\n", line, SendEditor(SCI_GETLASTCHILD, line));
		if (!SendEditor(SCI_GETFOLDEXPANDED, line)) {
			// Removing the fold from one that has been contracted so should expand
			// otherwise lines are left invisible with no way to make them visible
			SendEditor(SCI_SETFOLDEXPANDED, line, 1);
			Expand(line, true, false, 0, levelPrev);
		}
	} else if ((levelPrev & SC_FOLDLEVELNUMBERMASK) > (levelNow & SC_FOLDLEVELNUMBERMASK)) {
		// See if should still be hidden
		int parentLine = SendEditor(SCI_GETFOLDPARENT, line);
		if (parentLine < 0) { 
			SendEditor(SCI_SHOWLINES, line, line);
		} else if (SendEditor(SCI_GETFOLDEXPANDED, parentLine) && SendEditor(SCI_GETLINEVISIBLE, parentLine)) {
			SendEditor(SCI_SHOWLINES, line, line);
		}
	}
}

void SciTEBase::Expand(int &line, bool doExpand, bool force, int visLevels, int level) {
	int lineMaxSubord = SendEditor(SCI_GETLASTCHILD, line, level & SC_FOLDLEVELNUMBERMASK);
	line++;
	while (line <= lineMaxSubord) {
		if (force) {
			if (visLevels > 0)
				SendEditor(SCI_SHOWLINES, line, line);
			else
				SendEditor(SCI_HIDELINES, line, line);
		} else {
			if (doExpand)
				SendEditor(SCI_SHOWLINES, line, line);
		}
		int levelLine = level;
		if (levelLine == -1)
			levelLine = SendEditor(SCI_GETFOLDLEVEL, line);
		if (levelLine & SC_FOLDLEVELHEADERFLAG) {
			if (force) {
				if (visLevels > 1)
					SendEditor(SCI_SETFOLDEXPANDED, line, 1);
				else
					SendEditor(SCI_SETFOLDEXPANDED, line, 0);
				Expand(line, doExpand, force, visLevels - 1);
			} else {
				if (doExpand) {
					if (!SendEditor(SCI_GETFOLDEXPANDED, line))
						SendEditor(SCI_SETFOLDEXPANDED, line, 1);
					Expand(line, true, force, visLevels - 1);
				} else {
					Expand(line, false, force, visLevels - 1);
				}
			}
		} else {
			line++;
		}
	}
}

void SciTEBase::FoldAll() {
	SendEditor(SCI_COLOURISE, 0, -1);
	int maxLine = SendEditor(SCI_GETLINECOUNT);
	bool expanding = true;
	for (int lineSeek = 0; lineSeek < maxLine; lineSeek++) {
		if (SendEditor(SCI_GETFOLDLEVEL, lineSeek) & SC_FOLDLEVELHEADERFLAG) {
			expanding = !SendEditor(SCI_GETFOLDEXPANDED, lineSeek);
			break;
		}
	}
	for (int line = 0; line < maxLine; line++) {
		int level = SendEditor(SCI_GETFOLDLEVEL, line);
		if ((level & SC_FOLDLEVELHEADERFLAG) &&
		        (SC_FOLDLEVELBASE == (level & SC_FOLDLEVELNUMBERMASK))) {
			if (expanding) {
				SendEditor(SCI_SETFOLDEXPANDED, line, 1);
				Expand(line, true, false, 0, level);
				line--;
			} else {
				int lineMaxSubord = SendEditor(SCI_GETLASTCHILD, line, -1);
				SendEditor(SCI_SETFOLDEXPANDED, line, 0);
				if (lineMaxSubord > line)
					SendEditor(SCI_HIDELINES, line + 1, lineMaxSubord);
			}
		}
	}
}

void SciTEBase::GotoLineEnsureVisible(int line) {
	SendEditor(SCI_ENSUREVISIBLEENFORCEPOLICY, line);
	SendEditor(SCI_GOTOLINE, line);
}

void SciTEBase::EnsureRangeVisible(int posStart, int posEnd, bool enforcePolicy) {
	int lineStart = SendEditor(SCI_LINEFROMPOSITION, Platform::Minimum(posStart, posEnd));
	int lineEnd = SendEditor(SCI_LINEFROMPOSITION, Platform::Maximum(posStart, posEnd));
	for (int line = lineStart; line <= lineEnd; line++) {
		SendEditor(enforcePolicy ? SCI_ENSUREVISIBLEENFORCEPOLICY : SCI_ENSUREVISIBLE, line);
	}
}

bool SciTEBase::MarginClick(int position, int modifiers) {
	int lineClick = SendEditor(SCI_LINEFROMPOSITION, position);
	//Platform::DebugPrintf("Margin click %d %d %x\n", position, lineClick,
	//	SendEditor(SCI_GETFOLDLEVEL, lineClick) & SC_FOLDLEVELHEADERFLAG);
	if ((modifiers & SCMOD_SHIFT) && (modifiers & SCMOD_CTRL)) {
		FoldAll();
	} else {
		int levelClick = SendEditor(SCI_GETFOLDLEVEL, lineClick);
		if (levelClick & SC_FOLDLEVELHEADERFLAG) {
			if (modifiers & SCMOD_SHIFT) {
				EnsureAllChildrenVisible(lineClick, levelClick);
			} else if (modifiers & SCMOD_CTRL) {
				ToggleFoldRecursive(lineClick, levelClick);
			} else {
				// Toggle this line
				SendEditor(SCI_TOGGLEFOLD, lineClick);
			}
		}
	}
	return true;
}

void SciTEBase::ToggleFoldRecursive(int line, int level) {
	if (SendEditor(SCI_GETFOLDEXPANDED, line)) {
		// Contract this line and all children
		SendEditor(SCI_SETFOLDEXPANDED, line, 0);
		Expand(line, false, true, 0, level);
	} else {
		// Expand this line and all children
		SendEditor(SCI_SETFOLDEXPANDED, line, 1);
		Expand(line, true, true, 100, level);
	}
}

void SciTEBase::EnsureAllChildrenVisible(int line, int level) {
	// Ensure all children visible
	SendEditor(SCI_SETFOLDEXPANDED, line, 1);
	Expand(line, true, true, 100, level);
}

void SciTEBase::NewLineInOutput() {
	if (executing)
		return;
	char cmd[200];
	int line = SendOutput(SCI_LINEFROMPOSITION,
	                      SendOutput(SCI_GETCURRENTPOS)) - 1;
	int lineStart = SendOutput(SCI_POSITIONFROMLINE, line);
	int lineEnd = SendOutput(SCI_GETLINEENDPOSITION, line);
	int lineMax = lineStart + sizeof(cmd) - 1;
	if (lineEnd > lineMax)
		lineEnd = lineMax;
	GetRange(wOutput, lineStart, lineEnd, cmd);
	cmd[lineEnd - lineStart] = '\0';
	// TODO: put a '>' at beginning of line
	returnOutputToCommand = false;
	AddCommand(cmd, ".", jobCLI);
	Execute();
}

void SciTEBase::Notify(SCNotification *notification) {
	bool handled = false;
	//Platform::DebugPrintf("Notify %d\n", notification->nmhdr.code);
	switch (notification->nmhdr.code) {
	case SCEN_SETFOCUS:
	case SCEN_KILLFOCUS:
		CheckMenus();
		break;

	case SCN_STYLENEEDED: {
			if (extender) {
				// Colourisation may be performed by script
				if ((notification->nmhdr.idFrom == IDM_SRCWIN) && (lexLanguage == SCLEX_CONTAINER)) {
					int endStyled = SendEditor(SCI_GETENDSTYLED);
					int lineEndStyled = SendEditor(SCI_LINEFROMPOSITION, endStyled);
					endStyled = SendEditor(SCI_POSITIONFROMLINE, lineEndStyled);
					WindowAccessor styler(wEditor.GetID(), props);
					int styleStart = 0;
					if (endStyled > 0)
						styleStart = styler.StyleAt(endStyled - 1);
					styler.SetCodePage(codePage);
					extender->OnStyle(endStyled, notification->position - endStyled,
					                  styleStart, &styler);
					styler.Flush();
				}
			}
			// Colourisation is now normally performed by the SciLexer DLL
#ifdef OLD_CODE
			if (notification->nmhdr.idFrom == IDM_SRCWIN) {
				int endStyled = SendEditor(SCI_GETENDSTYLED);
				int lineEndStyled = SendEditor(SCI_LINEFROMPOSITION, endStyled);
				endStyled = SendEditor(SCI_POSITIONFROMLINE, lineEndStyled);
				Colourise(endStyled, notification->position);
			} else {
				int endStyled = SendOutput(SCI_GETENDSTYLED);
				int lineEndStyled = SendOutput(SCI_LINEFROMPOSITION, endStyled);
				endStyled = SendOutput(SCI_POSITIONFROMLINE, lineEndStyled);
				Colourise(endStyled, notification->position, false);
			}
#endif

		}
		break;

	case SCN_CHARADDED:
		if (extender)
			handled = extender->OnChar(static_cast<char>(notification->ch));
		if (!handled) {
			if (notification->nmhdr.idFrom == IDM_SRCWIN) {
				CharAdded(static_cast<char>(notification->ch));
			} else if (notification->ch == '\n') {
				NewLineInOutput();
			}
		}
		break;

	case SCN_SAVEPOINTREACHED:
		if (notification->nmhdr.idFrom == IDM_SRCWIN) {
			if (extender)
				handled = extender->OnSavePointReached();
			if (!handled) {
				isDirty = false;
			}
		}
		CheckMenus();
		SetWindowName();
		BuffersMenu();
		break;

	case SCN_SAVEPOINTLEFT:
		if (notification->nmhdr.idFrom == IDM_SRCWIN) {
			if (extender)
				handled = extender->OnSavePointLeft();
			if (!handled) {
				isDirty = true;
				isBuilt = false;
			}
		}
		CheckMenus();
		SetWindowName();
		BuffersMenu();
		break;

	case SCN_DOUBLECLICK:
		if (extender)
			handled = extender->OnDoubleClick();
		if (!handled && notification->nmhdr.idFrom == IDM_RUNWIN) {
			GoMessage(0);
		}
		break;

	case SCN_UPDATEUI:
		if (extender)
			handled = extender->OnUpdateUI();
		if (!handled) {
			BraceMatch(notification->nmhdr.idFrom == IDM_SRCWIN);
			if (notification->nmhdr.idFrom == IDM_SRCWIN) {
				UpdateStatusBar(false);
			}
			CheckMenusClipboard();
		}
		break;

	case SCN_MODIFIED:
		if (notification->modificationType & SC_LASTSTEPINUNDOREDO) {
			//when the user hits undo or redo, several normal insert/delete
			//notifications may fire, but we will end up here in the end
			EnableAMenuItem(IDM_UNDO, SendFocused(SCI_CANUNDO));
			EnableAMenuItem(IDM_REDO, SendFocused(SCI_CANREDO));
		} else if (notification->modificationType & (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT)) {
			//this will be called a lot, and usually means "typing".
			EnableAMenuItem(IDM_UNDO, TRUE);
			EnableAMenuItem(IDM_REDO, FALSE);
		}

		if (notification->linesAdded && lineNumbers && lineNumbersExpand)
			SetLineNumberWidth();

		if (0 != (notification->modificationType & SC_MOD_CHANGEFOLD)) {
			FoldChanged(notification->line,
			            notification->foldLevelNow, notification->foldLevelPrev);
		}
		break;


	case SCN_MARGINCLICK: {
			if (extender)
				handled = extender->OnMarginClick();
			if (!handled) {

#ifdef GLI_CHANGES

        //If clicking on the outer border, add/remove a breakpoint
        if(notification->margin == 1)
        {
          //Get the line click position
          int lineClick = SendEditor(SCI_LINEFROMPOSITION, notification->position);
          
          //Toggle the breakpoint at the line position
          ToggleBreakpoint(lineClick);
        }

#endif //GLI_CHANGES

				if (notification->margin == 2) {
					MarginClick(notification->position, notification->modifiers);
				}
			}
		}
		break;

	case SCN_NEEDSHOWN: {
			EnsureRangeVisible(notification->position, notification->position + notification->length, false);
		}
		break;

	case SCN_USERLISTSELECTION: {
			if (notification->wParam == 2)
				ContinueMacroList(notification->text);
			else if (extender && notification->wParam > 2)
				extender->OnUserListSelection(notification->wParam, notification->text);
		}
		break;

	case SCN_CALLTIPCLICK: {
			if (notification->position == 1 && currentCallTip > 0) {
				currentCallTip--;
				FillFunctionDefinition();
			} else if (notification->position == 2 && currentCallTip + 1 < maxCallTips) {
				currentCallTip++;
				FillFunctionDefinition();
			}
		}
		break;

	case SCN_MACRORECORD:
		RecordMacroCommand(notification);
		break;

	case SCN_URIDROPPED:
		OpenUriList(notification->text);
		break;

	case SCN_DWELLSTART: {
			if (INVALID_POSITION == notification->position) {
				char message[200];
				sprintf(message, "%0d (%0d,%0d)", notification->position, notification->x, notification->y);
			} else {
				int endWord = notification->position;
				SString message =
					RangeExtendAndGrab(wEditor,
									   notification->position, endWord, iswordcharforsel);
				if (message.length()) {
					SendEditorString(SCI_CALLTIPSHOW, notification->position, message.c_str());
				}
			}
		}
		break;

	case SCN_DWELLEND:
		SendEditorString(SCI_CALLTIPCANCEL, 0, 0);
		break;

	case SCN_ZOOM:
		SetLineNumberWidth();
		break;
	}
}

void SciTEBase::CheckMenusClipboard() {
	bool hasSelection = SendFocused(SCI_GETSELECTIONSTART) != SendFocused(SCI_GETSELECTIONEND);
	EnableAMenuItem(IDM_CUT, hasSelection);
	EnableAMenuItem(IDM_COPY, hasSelection);
	EnableAMenuItem(IDM_CLEAR, hasSelection);
	EnableAMenuItem(IDM_PASTE, SendFocused(SCI_CANPASTE));
}

void SciTEBase::CheckMenus() {
	CheckMenusClipboard();
	EnableAMenuItem(IDM_SAVE, isDirty);
	EnableAMenuItem(IDM_UNDO, SendFocused(SCI_CANUNDO));
	EnableAMenuItem(IDM_REDO, SendFocused(SCI_CANREDO));
	EnableAMenuItem(IDM_FINDINFILES, !executing);
	EnableAMenuItem(IDM_SHOWCALLTIP, apis != 0);
	EnableAMenuItem(IDM_COMPLETE, apis != 0);
	CheckAMenuItem(IDM_SPLITVERTICAL, splitVertical);
	EnableAMenuItem(IDM_OPENFILESHERE, props.GetInt("check.if.already.open") != 0);
	CheckAMenuItem(IDM_OPENFILESHERE, openFilesHere);
	CheckAMenuItem(IDM_WRAP, wrap);
	CheckAMenuItem(IDM_WRAPOUTPUT, wrapOutput);
	CheckAMenuItem(IDM_READONLY, isReadOnly);
	CheckAMenuItem(IDM_FULLSCREEN, fullScreen);
	CheckAMenuItem(IDM_VIEWTOOLBAR, tbVisible);
	CheckAMenuItem(IDM_VIEWTABBAR, tabVisible);
	CheckAMenuItem(IDM_VIEWSTATUSBAR, sbVisible);
	CheckAMenuItem(IDM_VIEWEOL, SendEditor(SCI_GETVIEWEOL));
	CheckAMenuItem(IDM_VIEWSPACE, SendEditor(SCI_GETVIEWWS));
	CheckAMenuItem(IDM_VIEWGUIDES, SendEditor(SCI_GETINDENTATIONGUIDES));
	CheckAMenuItem(IDM_LINENUMBERMARGIN, lineNumbers);
	CheckAMenuItem(IDM_SELMARGIN, margin);
	CheckAMenuItem(IDM_FOLDMARGIN, foldMargin);
	CheckAMenuItem(IDM_TOGGLEOUTPUT, heightOutput > 0);
	CheckAMenuItem(IDM_TOGGLEPARAMETERS, wParameters.Created());
	CheckAMenuItem(IDM_MONOFONT, useMonoFont);

#ifdef GLI_CHANGES

  EnableAMenuItem(IDM_COMPILE, true);
  EnableAMenuItem(IDM_TOGGLEBREAKPOINT, true);

#endif //GLI_CHANGES

/*  
	EnableAMenuItem(IDM_COMPILE, !executing &&
	                props.GetWild("command.compile.", filePath.AsInternal()).size() != 0);
	EnableAMenuItem(IDM_BUILD, !executing &&
	                props.GetWild("command.build.", filePath.AsInternal()).size() != 0);
	EnableAMenuItem(IDM_GO, !executing &&
	                props.GetWild("command.go.", filePath.AsInternal()).size() != 0);
*/
	for (int toolItem = 0; toolItem < toolMax; toolItem++)
		EnableAMenuItem(IDM_TOOLS + toolItem, !executing);
	EnableAMenuItem(IDM_STOPEXECUTE, executing);
	if (buffers.size > 0) {
#if PLAT_WIN
		// Tab Bar
#ifndef TCM_DESELECTALL
#define TCM_DESELECTALL TCM_FIRST+50
#endif
		::SendMessage(reinterpret_cast<HWND>(wTabBar.GetID()), TCM_DESELECTALL, (WPARAM)0, (LPARAM)0);
		::SendMessage(reinterpret_cast<HWND>(wTabBar.GetID()), TCM_SETCURSEL, (WPARAM)buffers.Current(), (LPARAM)0);
#endif
#if PLAT_GTK

		if (wTabBar.GetID())
			gtk_notebook_set_page(GTK_NOTEBOOK(wTabBar.GetID()), buffers.Current());
#endif

		for (int bufferItem = 0; bufferItem < buffers.length; bufferItem++) {
			CheckAMenuItem(IDM_BUFFER + bufferItem, bufferItem == buffers.Current());
		}
	}
	EnableAMenuItem(IDM_MACROPLAY, !recording);
	EnableAMenuItem(IDM_MACRORECORD, !recording);
	EnableAMenuItem(IDM_MACROSTOPRECORD, recording);
}

void SciTEBase::ContextMenu(Window wSource, Point pt, Window wCmd) {
	int currentPos = SendWindow(wSource, SCI_GETCURRENTPOS);
	int anchor = SendWindow(wSource, SCI_GETANCHOR);
	popup.CreatePopUp();
	bool writable = !SendWindow(wSource, SCI_GETREADONLY);
	AddToPopUp("Undo", IDM_UNDO, writable && SendWindow(wSource, SCI_CANUNDO));
	AddToPopUp("Redo", IDM_REDO, writable && SendWindow(wSource, SCI_CANREDO));
	AddToPopUp("");
	AddToPopUp("Cut", IDM_CUT, writable && currentPos != anchor);
	AddToPopUp("Copy", IDM_COPY, currentPos != anchor);
	AddToPopUp("Paste", IDM_PASTE, writable && SendWindow(wSource, SCI_CANPASTE));
	AddToPopUp("Delete", IDM_CLEAR, writable && currentPos != anchor);
	AddToPopUp("");
	AddToPopUp("Select All", IDM_SELECTALL);
	AddToPopUp("");
	if (wSource.GetID() == wOutput.GetID()) {
		AddToPopUp("Hide", IDM_TOGGLEOUTPUT, true);
	} else {
		AddToPopUp("Close", IDM_CLOSE, true);
	}
	SString userContextMenu = props.GetNewExpand("user.context.menu");
	userContextMenu.substitute('|', '\0');
	const char *userContextItem = userContextMenu.c_str();
	const char *endDefinition = userContextItem + userContextMenu.length();
	while (userContextItem < endDefinition) {
		const char *caption = userContextItem;
		userContextItem += strlen(userContextItem) + 1;
		if (userContextItem < endDefinition) {
			int cmd = GetMenuCommandAsInt(userContextItem);
			userContextItem += strlen(userContextItem) + 1;
			AddToPopUp(caption, cmd);
		}
	}
	popup.Show(pt, wCmd);
}

/**
 * Ensure that a splitter bar position is inside the main window.
 */
int SciTEBase::NormaliseSplit(int splitPos) {
	PRectangle rcClient = GetClientRectangle();
	int w = rcClient.Width();
	int h = rcClient.Height();
	if (splitPos < 20)
		splitPos = 0;
	if (splitVertical) {
		if (splitPos > w - heightBar - 20)
			splitPos = w - heightBar;
	} else {
		if (splitPos > h - heightBar - 20)
			splitPos = h - heightBar;
	}
	return splitPos;
}

void SciTEBase::MoveSplit(Point ptNewDrag) {
	int newHeightOutput = heightOutputStartDrag + (ptStartDrag.y - ptNewDrag.y);
	if (splitVertical)
		newHeightOutput = heightOutputStartDrag + (ptStartDrag.x - ptNewDrag.x);
	newHeightOutput = NormaliseSplit(newHeightOutput);
	if (heightOutput != newHeightOutput) {
		heightOutput = newHeightOutput;
		SizeContentWindows();
		//Redraw();
	}

	previousHeightOutput = newHeightOutput;
}

void SciTEBase::UIAvailable() {
	SetImportMenu();
	if (extender) {
		FilePath homepath = GetSciteDefaultHome();
		props.Set("SciteDefaultHome", homepath.AsFileSystem());
		homepath = GetSciteUserHome();
		props.Set("SciteUserHome", homepath.AsFileSystem());
		extender->Initialise(this);
	}
}

/**
 * Find the character following a name which is made up of characters from
 * the set [a-zA-Z.]
 */
char AfterName(const char *s) {
	while (*s && ((*s == '.') ||
	              (*s >= 'a' && *s <= 'z') ||
	              (*s >= 'A' && *s <= 'A')))
		s++;
	return *s;
}

void SciTEBase::PerformOne(char *action) {
	unsigned int len = UnSlash(action);
	char *arg = strchr(action, ':');
	if (arg) {
		arg++;
		if (isprefix(action, "askfilename:")) {
			extender->OnMacro("filename", filePath.AsFileSystem());
		} else if (isprefix(action, "askproperty:")) {
			PropertyToDirector(arg);
		} else if (isprefix(action, "close:")) {
			Close();
			WindowSetFocus(wEditor);
		} else if (isprefix(action, "currentmacro:")) {
			currentMacro = arg;
		} else if (isprefix(action, "cwd:")) {
			if (_chdir(arg) != 0) {
				SString msg = LocaliseMessage("Invalid directory '^0'.", arg);
				WindowMessageBox(wSciTE, msg, MB_OK | MB_ICONWARNING);
			}
		} else if (isprefix(action, "enumproperties:")) {
			EnumProperties(arg);
		} else if (isprefix(action, "exportashtml:")) {
			SaveToHTML(arg);
		} else if (isprefix(action, "exportasrtf:")) {
			SaveToRTF(arg);
		} else if (isprefix(action, "exportaspdf:")) {
			SaveToPDF(arg);
		} else if (isprefix(action, "exportaslatex:")) {
			SaveToTEX(arg);
		} else if (isprefix(action, "exportasxml:")) {
			SaveToXML(arg);
		} else if (isprefix(action, "find:") && fnEditor) {
			findWhat = arg;
			FindNext(false, false);
		} else if (isprefix(action, "goto:") && fnEditor) {
			int line = atoi(arg) - 1;
			GotoLineEnsureVisible(line);
			// jump to column if given and greater than 0
			char *colstr = strchr(arg, ',');
			if (colstr != NULL) {
				int col = atoi(colstr + 1);
				if (col > 0) {
					int pos = SendEditor(SCI_GETCURRENTPOS) + col;
					// select the word you have found there
					int wordStart = SendEditor(SCI_WORDSTARTPOSITION, pos, true);
					int wordEnd = SendEditor(SCI_WORDENDPOSITION, pos, true);
					SendEditor(SCI_SETSEL, wordStart, wordEnd);
				}
			}
		} else if (isprefix(action, "insert:") && fnEditor) {
			SendEditorString(SCI_REPLACESEL, 0, arg);
		} else if (isprefix(action, "macrocommand:")) {
			ExecuteMacroCommand(arg);
		} else if (isprefix(action, "macroenable:")) {
			macrosEnabled = atoi(arg);
			SetToolsMenu();
		} else if (isprefix(action, "macrolist:")) {
			StartMacroList(arg);
		} else if (isprefix(action, "menucommand:")) {
			MenuCommand(atoi(arg));
		} else if (isprefix(action, "open:")) {
			Open(arg);
		} else if (isprefix(action, "output:") && fnOutput) {
			SendOutput(SCI_REPLACESEL, 0, reinterpret_cast<sptr_t>(arg));
		} else if (isprefix(action, "property:")) {
			PropertyFromDirector(arg);
		} else if (isprefix(action, "reloadproperties:")) {
			ReloadProperties();
		} else if (isprefix(action, "quit:")) {
			QuitProgram();
		} else if (isprefix(action, "replaceall:") && fnEditor) {
			if (len > strlen(action)) {
				char *arg2 = arg + strlen(arg) + 1;
				findWhat = arg;
				replaceWhat = arg2;
				ReplaceAll(false);
			}
		} else if (isprefix(action, "saveas:")) {
			if (*arg) {
				SaveAs(arg);
			} else {
				SaveAsDialog();
			}
		} else if (isprefix(action, "loadsession:")) {
			if (*arg) {
				LoadSession(arg);
			}
		} else if (isprefix(action, "extender:")) {
			extender->OnExecute(arg);
		}
	}
}

static bool IsSwitchCharacter(char ch) {
#ifdef unix
	return ch == '-';
#else

	return (ch == '-') || (ch == '/');
#endif
}

// Called by SciTEBase::PerformOne when action="enumproperties:"
void SciTEBase::EnumProperties(const char *propkind) {
	char *key = NULL;
	char *val = NULL;
	PropSetFile *pf = NULL;

	if (!extender)
		return;
	if (!strcmp(propkind, "dyn")) {
		SelectionIntoProperties(); // Refresh properties ...
		pf = &props;
	} else if (!strcmp(propkind, "local"))
		pf = &propsLocal;
	else if (!strcmp(propkind, "user"))
		pf = &propsUser;
	else if (!strcmp(propkind, "base"))
		pf = &propsBase;
	else if (!strcmp(propkind, "embed"))
		pf = &propsEmbed;
	else if (!strcmp(propkind, "abbrev"))
		pf = &propsAbbrev;

	if (pf != NULL) {
		bool b = pf->GetFirst(&key, &val);
		while (b) {
			SendOneProperty(propkind, key, val);
			b = pf->GetNext(&key, &val);
		}
	}
}

void SciTEBase::SendOneProperty(const char *kind, const char *key, const char *val) {
	size_t keysize = strlen(kind) + 1 + strlen(key) + 1 + strlen(val) + 1;
	char *m = new char[keysize];
	if (m) {
		strcpy(m, kind);
		strcat(m, ":");
		strcat(m, key);
		strcat(m, "=");
		strcat(m, val);
		extender->SendProperty(m);
		delete []m;
	}
}

void SciTEBase::PropertyFromDirector(const char *arg) {
	props.Set(arg);
}

void SciTEBase::PropertyToDirector(const char *arg) {
	if (!extender)
		return;
	SelectionIntoProperties();
	SString gotprop = props.Get(arg);
	extender->OnMacro("macro:stringinfo", gotprop.c_str());
}

/**
 * Menu/Toolbar command "Record".
 */
void SciTEBase::StartRecordMacro() {
	recording = true;
	CheckMenus();
	SendEditor(SCI_STARTRECORD);
}

/**
 * Received a SCN_MACRORECORD from Scintilla: send it to director.
 */
bool SciTEBase::RecordMacroCommand(SCNotification *notification) {
	if (extender) {
		char *szMessage;
		char *t;
		bool handled;
		t = (char*)(notification->lParam);
		if (t != NULL) {
			//format : "<message>;<wParam>;1;<text>"
			szMessage = new char[50 + strlen(t) + 4];
			sprintf(szMessage, "%d;%ld;1;%s", notification->message, notification->wParam, t);
		} else {
			//format : "<message>;<wParam>;0;"
			szMessage = new char[50];
			sprintf(szMessage, "%d;%ld;0;", notification->message, notification->wParam);
		}
		handled = extender->OnMacro("macro:record", szMessage);
		delete []szMessage;
		return handled;
	}
	return true;
}

/**
 * Menu/Toolbar command "Stop recording".
 */
void SciTEBase::StopRecordMacro() {
	SendEditor(SCI_STOPRECORD);
	if (extender)
		extender->OnMacro("macro:stoprecord", "");
	recording = false;
	CheckMenus();
}

/**
 * Menu/Toolbar command "Play macro...": tell director to build list of Macro names
 * Through this call, user has access to all macros in Filerx.
 */
void SciTEBase::AskMacroList() {
	if (extender)
		extender->OnMacro("macro:getlist", "");
}

/**
 * List of Macro names has been created. Ask Scintilla to show it.
 */
bool SciTEBase::StartMacroList(const char *words) {
	if (words) {
		SendEditorString(SCI_USERLISTSHOW, 2, words); //listtype=2
	}

	return true;
}

/**
 * User has chosen a macro in the list. Ask director to execute it.
 */
void SciTEBase::ContinueMacroList(const char *stext) {
	if ((extender) && (*stext != '\0')) {
		currentMacro = stext;
		StartPlayMacro();
	}
}

/**
 * Menu/Toolbar command "Play current macro" (or called from ContinueMacroList).
 */
void SciTEBase::StartPlayMacro() {
	if (extender)
		extender->OnMacro("macro:run", currentMacro.c_str());
}

/*
SciTE received a macro command from director : execute it.
If command needs answer (SCI_GETTEXTLENGTH ...) : give answer to director
*/

static uptr_t ReadNum(const char *&t) {
	const char *argend = strchr(t, ';');	// find ';'
	uptr_t v = 0;
	if (*t)
		v = atoi(t);					// read value
	t = argend + 1;					// update pointer
	return v;						// return value
}

void SciTEBase::ExecuteMacroCommand(const char *command) {
	const char *nextarg = command;
	uptr_t wParam;
	sptr_t lParam = 0;
	int rep = 0;				//Scintilla's answer
	char *answercmd;
	int l;
	char *string1 = NULL;
	char params[4];
	//params describe types of return values and of arguments
	//0 : void or no param
	//I : integer
	//S : string
	//R : return string (for lParam only)

	//extract message,wParam ,lParam

	uptr_t message = ReadNum(nextarg);
	strncpy(params, nextarg, 3);
	nextarg += 4;
	if (*(params + 1) == 'R') {
		// in one function wParam is a string  : void SetProperty(string key,string name)
		const char *s1 = nextarg;
		while (*nextarg != ';')
			nextarg++;
		int lstring1 = nextarg - s1;
		string1 = new char[lstring1 + 1];
		if (lstring1 > 0)
			strncpy(string1, s1, lstring1);
		*(string1 + lstring1) = '\0';
		wParam = reinterpret_cast<uptr_t>(string1);
		nextarg++;
	} else {
		wParam = ReadNum(nextarg);
	}

	if (*(params + 2) == 'S')
		lParam = reinterpret_cast<sptr_t>(nextarg);
	else if (*(params + 2) == 'I')
		lParam = atoi(nextarg);

	if (*params == '0') {
		// no answer ...
		SendEditor(message, wParam, lParam);
		if (string1 != NULL)
			delete []string1;
		return;
	}

	if (*params == 'S') {
		// string answer
		if (message == SCI_GETSELTEXT) {
			l = SendEditor(SCI_GETSELECTIONEND) - SendEditor(SCI_GETSELECTIONSTART);
			wParam = 0;
		} else if (message == SCI_GETCURLINE) {
			int line = SendEditor(SCI_LINEFROMPOSITION, SendEditor(SCI_GETCURRENTPOS));
			l = SendEditor(SCI_LINELENGTH, line);
			wParam = l;
		} else if (message == SCI_GETTEXT) {
			l = SendEditor(SCI_GETLENGTH);
			wParam = l;
		} else if (message == SCI_GETLINE) {
			l = SendEditor(SCI_LINELENGTH, wParam);
		} else {
			l = 0; //unsupported calls EM
		}
		answercmd = "stringinfo:";

	} else {
		//int answer
		answercmd = "intinfo:";
		l = 30;
	}

	size_t alen = strlen(answercmd);
	char *tbuff = new char[l + alen + 1];
	strcpy(tbuff, answercmd);
	if (*params == 'S')
		lParam = reinterpret_cast<sptr_t>(tbuff + alen);

	if (l > 0)
		rep = SendEditor(message, wParam, lParam);
	if (*params == 'I')
		sprintf(tbuff + alen, "%0d", rep);
	extender->OnMacro("macro", tbuff);
	delete []tbuff;
}

void SciTEBase::LoadMRUAndSession(bool allowLoadSession) {
	InitialiseBuffers();
	if (props.GetInt("save.recent")) {
		LoadRecentMenu();
	}
	if (allowLoadSession && props.GetInt("buffers") &&
	        props.GetInt("save.session")) {
		LoadSession("");
	}
}

/**
 * Process all the command line arguments.
 * Arguments that start with '-' (also '/' on Windows) are switches or commands with
 * other arguments being file names which are opened. Commands are distinguished
 * from switches by containing a ':' after the command name.
 * The print switch /p is special cased.
 * Processing occurs in two phases to allow switches that occur before any file opens
 * to be evaluated before creating the UI.
 * Call twice, first with phase=0, then with phase=1 after creating UI.
 */
bool SciTEBase::ProcessCommandLine(SString &args, int phase) {
	bool performPrint = false;
	bool evaluate = phase == 0;
	WordList wlArgs(true);
	wlArgs.Set(args.c_str());
	for (int i = 0; i < wlArgs.len; i++) {
		char *arg = wlArgs[i];
		if (IsSwitchCharacter(arg[0])) {
			arg++;
			if (arg[0] == '\0' || arg[0] == '-') {
				if (phase == 1) {
					OpenFromStdin(arg[0] == '-');
				}
			} else if (arg[0] == '@') {
				if (phase == 1) {
					OpenFilesFromStdin();
				}
			} else if ((tolower(arg[0]) == 'p') && (strlen(arg) == 1)) {
				performPrint = true;
			} else if (strcmp(arg, "grep") == 0) {
				// wlArgs[i+1] will be options in future
				char unquoted[1000];
				strcpy(unquoted, wlArgs[i+3]);
				UnSlash(unquoted);
				InternalGrep(true, FilePath::GetWorkingDirectory().AsInternal(), wlArgs[i+2], unquoted);
				exit(0);
			} else {
				if (AfterName(arg) == ':') {
					if (isprefix(arg, "open:") || isprefix(arg, "loadsession:")) {
						if (phase == 0)
							return performPrint;
						else
							evaluate = true;
					}
					if (evaluate)
						PerformOne(arg);
				} else {
					if (evaluate)
						props.ReadLine(arg, true, "");
				}
			}
		} else {	// Not a switch: it is a file name
			if (phase == 0)
				return performPrint;
			else
				evaluate = true;
			LoadMRUAndSession(false);
			if (!PreOpenCheck(arg))
				Open(arg, ofQuiet);
		}
	}
	if (phase == 1) {
		// If we have finished with all args and no buffer is open
		// try to load session.
		if (buffers.size == 0) {
			LoadMRUAndSession(true);
		}
		// No open file after session load so create empty document.
		if (filePath.IsUntitled() && buffers.length == 1 && !buffers.buffers[0].isDirty) {
			Open("");
		}
	}
	return performPrint;
}

// Implement ExtensionAPI methods
sptr_t SciTEBase::Send(Pane p, unsigned int msg, uptr_t wParam, sptr_t lParam) {
	if (p == paneEditor)
		return SendEditor(msg, wParam, lParam);
	else
		return SendOutput(msg, wParam, lParam);
}

char *SciTEBase::Range(Pane p, int start, int end) {
	int len = end - start;
	char *s = new char[len + 1];
	if (s) {
		if (p == paneEditor)
			GetRange(wEditor, start, end, s);
		else
			GetRange(wOutput, start, end, s);
	}
	return s;
}

void SciTEBase::Remove(Pane p, int start, int end) {
	// Should have a scintilla call for this
	if (p == paneEditor) {
		SendEditor(SCI_SETSEL, start, end);
		SendEditor(SCI_CLEAR);
	} else {
		SendOutput(SCI_SETSEL, start, end);
		SendOutput(SCI_CLEAR);
	}
}

void SciTEBase::Insert(Pane p, int pos, const char *s) {
	if (p == paneEditor)
		SendEditorString(SCI_INSERTTEXT, pos, s);
	else
		SendOutputString(SCI_INSERTTEXT, pos, s);
}

void SciTEBase::Trace(const char *s) {
	MakeOutputVisible();
	OutputAppendString(s);
}

char *SciTEBase::Property(const char *key) {
	SString value = props.GetExpanded(key);
	char *retval = new char[value.length() + 1];
	if (retval)
		strcpy(retval, value.c_str());
	return retval;
}

void SciTEBase::SetProperty(const char *key, const char *val) {
	props.Set(key, val);
	needReadProperties = true;
}

void SciTEBase::UnsetProperty(const char *key) {
	props.Unset(key);
	needReadProperties = true;
}

uptr_t SciTEBase::GetInstance() {
	return 0;
}

void SciTEBase::ShutDown() {
	QuitProgram();
}

void SciTEBase::Perform(const char *actionList) {
	char *actionsDup = StringDup(actionList);
	char *actions = actionsDup;
	char *nextAct;
	while ((nextAct = strchr(actions, '\n')) != NULL) {
		*nextAct = '\0';
		PerformOne(actions);
		actions = nextAct + 1;
	}
	PerformOne(actions);
	delete []actionsDup;
}



#ifdef GLI_CHANGES

///////////////////////////////////////////////////////////////////////////////
//
void SciTEBase::SaveBreakPointData()
{
  SString saveFileName = buffers.buffers[buffers.Current()].AsInternal();

  //Find any existing data by the same name and remove it
  for(uint i=0;i<breakPointCacheArray.size(); i++)
  {
    //If the file is found (should never happen!)
    if(breakPointCacheArray[i].fileName == saveFileName)
    {
      breakPointCacheArray.erase(breakPointCacheArray.begin() + i);
      break;
    }
  }

  BreakPointCache newCache;
  newCache.fileName = saveFileName;

  //Get a listing of all the breakpoint locations
  int lineOffset = SendEditor(SCI_MARKERNEXT, 0, 1 << SciTE_MARKER_BREAKPOINT);
  while(lineOffset >= 0)
  {
    //Add the line number to the array
    newCache.lineNumbers.push_back(lineOffset);

    //Get the next line number
    lineOffset = SendEditor(SCI_MARKERNEXT, lineOffset + 1, 1 << SciTE_MARKER_BREAKPOINT);
  }

  //Add to the array if any values were added
  if(newCache.lineNumbers.size() > 0)
  {
    breakPointCacheArray.push_back(newCache);
  }
}


///////////////////////////////////////////////////////////////////////////////
//
void SciTEBase::LoadBreakPointData()
{
  SString loadFileName = buffers.buffers[buffers.Current()].AsInternal();

  //Find the shader matching the name
  for(uint i=0;i<breakPointCacheArray.size(); i++)
  {
    //If found, loop and set the breakpoints 
    if(breakPointCacheArray[i].fileName == loadFileName)
    {
      //Loop and add all the breakpoints
      for(uint b=0; b<breakPointCacheArray[i].lineNumbers.size(); b++)
      {
        SendEditor(SCI_MARKERADD, breakPointCacheArray[i].lineNumbers[b], SciTE_MARKER_BREAKPOINT);
      }

      //Remove from the array
      breakPointCacheArray.erase(breakPointCacheArray.begin() + i);
      break;
    }
  }

}

#endif //GLI_CHANGES

