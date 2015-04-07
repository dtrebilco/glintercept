// SciTE - Scintilla based Text Editor
/** @file SciTEProps.cxx
 ** Properties management.
 **/
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <locale.h>

#include "Platform.h"

#if PLAT_FOX

#include <unistd.h>

const char menuAccessIndicator[] = "&";

#endif

#if PLAT_GTK

#include <unistd.h>
#include <gtk/gtk.h>

const char menuAccessIndicator[] = "_";

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

const char menuAccessIndicator[] = "&";

#endif

#include "SciTE.h"
#include "PropSet.h"
#include "Accessor.h"
#include "Scintilla.h"
#include "SciLexer.h"
#include "Extender.h"
#include "FilePath.h"
#include "SciTEBase.h"

PropSetFile::PropSetFile(bool lowerKeys_) : lowerKeys(lowerKeys_) {}

PropSetFile::~PropSetFile() {}

/**
 * Get a line of input. If end of line escaped with '\\' then continue reading.
 */
static bool GetFullLine(const char *&fpc, int &lenData, char *s, int len) {
	bool continuation = true;
	s[0] = '\0';
	while ((len > 1) && lenData > 0) {
		char ch = *fpc;
		fpc++;
		lenData--;
		if ((ch == '\r') || (ch == '\n')) {
			if (!continuation) {
				if ((lenData > 0) && (ch == '\r') && ((*fpc) == '\n')) {
					// munch the second half of a crlf
					fpc++;
					lenData--;
				}
				*s = '\0';
				return true;
			}
		} else if ((ch == '\\') && (lenData > 0) && ((*fpc == '\r') || (*fpc == '\n'))) {
			continuation = true;
		} else {
			continuation = false;
			*s++ = ch;
			*s = '\0';
			len--;
		}
	}
	return false;
}

static bool IsSpaceOrTab(char ch) {
	return (ch == ' ') || (ch == '\t');
}

static bool IsCommentLine(const char *line) {
	while (IsSpaceOrTab(*line)) ++line;
	return (*line == '#');
}

bool PropSetFile::ReadLine(const char *lineBuffer, bool ifIsTrue, FilePath directoryForImports,
                           FilePath imports[], int sizeImports) {
	//UnSlash(lineBuffer);
	if (!IsSpaceOrTab(lineBuffer[0]))    // If clause ends with first non-indented line
		ifIsTrue = true;
	if (isprefix(lineBuffer, "if ")) {
		const char *expr = lineBuffer + strlen("if") + 1;
		ifIsTrue = GetInt(expr);
	} else if (isprefix(lineBuffer, "import ") && directoryForImports.IsSet()) {
		SString importName(lineBuffer + strlen("import") + 1);
		importName += ".properties";
		FilePath importPath(directoryForImports, FilePath(importName.c_str()));
		if (Read(importPath, directoryForImports, imports, sizeImports)) {
			if (imports) {
				for (int i = 0; i < sizeImports; i++) {
					if (!imports[i].IsSet()) {
						imports[i] = importPath;
						break;
					}
				}
			}
		}
	} else if (ifIsTrue && !IsCommentLine(lineBuffer)) {
		Set(lineBuffer);
	}
	return ifIsTrue;
}

void PropSetFile::ReadFromMemory(const char *data, int len, FilePath directoryForImports,
                                 FilePath imports[], int sizeImports) {
	const char *pd = data;
	char lineBuffer[60000];
	bool ifIsTrue = true;
	while (len > 0) {
		GetFullLine(pd, len, lineBuffer, sizeof(lineBuffer));
		if (lowerKeys) {
			for (int i=0; lineBuffer[i] && (lineBuffer[i] != '='); i++) {
				if ((lineBuffer[i] >= 'A') && (lineBuffer[i] <= 'Z')) {
					lineBuffer[i] = static_cast<char>(lineBuffer[i] - 'A' + 'a');
				}
			}
		}
		ifIsTrue = ReadLine(lineBuffer, ifIsTrue, directoryForImports, imports, sizeImports);
	}
}

bool PropSetFile::Read(FilePath filename, FilePath directoryForImports,
                       FilePath imports[], int sizeImports) {
	FILE *rcfile = filename.Open(fileRead);
	if (rcfile) {
		char propsData[60000];
		int lenFile = static_cast<int>(fread(propsData, 1, sizeof(propsData), rcfile));
		fclose(rcfile);
		const char *data = propsData;
		if (memcmp(data, "\xef\xbb\xbf", 3) == 0)
			data += 3;
		ReadFromMemory(data, lenFile, directoryForImports, imports, sizeImports);
		return true;
	}
	return false;
}

void PropSetFile::SetInteger(const char *key, sptr_t i) {
	char tmp[32];
	sprintf(tmp, "%d", static_cast<int>(i));
	Set(key, tmp);
}

void SciTEBase::SetImportMenu() {
	for (int i = 0; i < importMax; i++) {
		DestroyMenuItem(menuOptions, importCmdID + i);
	}
	if (importFiles[0].IsSet()) {
		for (int stackPos = 0; stackPos < importMax; stackPos++) {
			int itemID = importCmdID + stackPos;
			if (importFiles[stackPos].IsSet()) {
				SString entry = LocaliseString("Open");
				entry += " ";
				entry += importFiles[stackPos].Name().AsInternal();
				SetMenuItem(menuOptions, IMPORT_START + stackPos, itemID, entry.c_str());
			}
		}
	}
}

void SciTEBase::ImportMenu(int pos) {
	//Platform::DebugPrintf("Stack menu %d\n", pos);
	if (pos >= 0) {
		if (importFiles[pos].IsSet()) {
			Open(importFiles[pos]);
		}
	}
}

void SciTEBase::SetLanguageMenu() {
	for (int i = 0; i < 100; i++) {
		DestroyMenuItem(menuLanguage, languageCmdID + i);
	}
	for (int item = 0; item < languageItems; item++) {
		int itemID = languageCmdID + item;
		SString entry = LocaliseString(languageMenu[item].menuItem.c_str());
		if (languageMenu[item].menuKey.length()) {
#if PLAT_GTK
			entry += " ";
#else
			entry += "\t";
#endif
			entry += languageMenu[item].menuKey;
		}
		if (entry[0] != '#') {
			SetMenuItem(menuLanguage, item, itemID, entry.c_str());
		}
	}
}

const char propFileName[] = "SciTE.properties";

void SciTEBase::ReadGlobalPropFile() {
#ifdef unix
	extern char **environ;
	char **e=environ;
#else
	char **e=_environ;
#endif
	for (; *e; e++) {
		char key[1024];
		char *k=*e;
		char *v=strchr(k,'=');
		if (v && (static_cast<size_t>(v-k) < sizeof(key))) {
			memcpy(key, k, v-k);
			key[v-k] = '\0';
			propsEmbed.Set(key, v+1);
		}
	}

	for (int stackPos = 0; stackPos < importMax; stackPos++) {
		importFiles[stackPos] = "";
	}

	propsBase.Clear();
	FilePath propfileBase = GetDefaultPropertiesFileName();
	propsBase.Read(propfileBase, propfileBase.Directory(), importFiles, importMax);

	propsUser.Clear();
	FilePath propfileUser = GetUserPropertiesFileName();
	propsUser.Read(propfileUser, propfileBase.Directory(), importFiles, importMax);

	if (!localisationRead) {
		ReadLocalisation();
	}
}

void SciTEBase::ReadAbbrevPropFile() {
	propsAbbrev.Clear();
	FilePath propfileAbbrev = GetAbbrevPropertiesFileName();
	propsAbbrev.Read(propfileAbbrev, propfileAbbrev.Directory(), importFiles, importMax);
}

void SciTEBase::ReadLocalPropFile() {
	FilePath propdir = filePath.Directory();
	FilePath propfile;
	propfile.Set(propdir, propFileName);

	propsLocal.Clear();
	propsLocal.Read(propfile, propdir);
	//Platform::DebugPrintf("Reading local properties from %s\n", propfile);

	// TODO: Grab these from Platform and update when environment says to
	props.Set("Chrome", "#C0C0C0");
	props.Set("ChromeHighlight", "#FFFFFF");
}

int IntFromHexDigit(int ch) {
	if ((ch >= '0') && (ch <= '9')) {
		return ch - '0';
	} else if (ch >= 'A' && ch <= 'F') {
		return ch - 'A' + 10;
	} else if (ch >= 'a' && ch <= 'f') {
		return ch - 'a' + 10;
	} else {
		return 0;
	}
}

int IntFromHexByte(const char *hexByte) {
	return IntFromHexDigit(hexByte[0]) * 16 + IntFromHexDigit(hexByte[1]);
}

static long ColourFromString(const SString &s) {
	if (s.length()) {
		int r = IntFromHexByte(s.c_str() + 1);
		int g = IntFromHexByte(s.c_str() + 3);
		int b = IntFromHexByte(s.c_str() + 5);
		return ColourDesired(r, g, b).AsLong();
	} else {
		return ColourDesired().AsLong();
	}
}

long ColourOfProperty(PropSet &props, const char *key, ColourDesired colourDefault) {
	SString colour = props.Get(key);
	if (colour.length()) {
		return ColourFromString(colour);
	}
	return colourDefault.AsLong();
}

/**
 * Put the next property item from the given property string
 * into the buffer pointed by @a pPropItem.
 * @return NULL if the end of the list is met, else, it points to the next item.
 */
const char *SciTEBase::GetNextPropItem(
	const char *pStart,	/**< the property string to parse for the first call,
						 * pointer returned by the previous call for the following. */
	char *pPropItem,	///< pointer on a buffer receiving the requested prop item
	int maxLen)			///< size of the above buffer
{
	int size = maxLen - 1;

	*pPropItem = '\0';
	if (pStart == NULL) {
		return NULL;
	}
	const char *pNext = strchr(pStart, ',');
	if (pNext) {	// Separator is found
		if (size > pNext - pStart) {
			// Found string fits in buffer
			size = pNext - pStart;
		}
		pNext++;
	}
	strncpy(pPropItem, pStart, size);
	pPropItem[size] = '\0';
	return pNext;
}

StyleDefinition::StyleDefinition(const char *definition) :
		size(0), fore("#000000"), back("#FFFFFF"),
		bold(false), italics(false), eolfilled(false), underlined(false),
		caseForce(SC_CASE_MIXED),
		visible(true), changeable(true),
		specified(sdNone) {
	ParseStyleDefinition(definition);
}

bool StyleDefinition::ParseStyleDefinition(const char *definition) {
	if (definition == 0 || *definition == '\0') {
		return false;
	}
	char *val = StringDup(definition);
	//Platform::DebugPrintf("Style %d is [%s]\n", style, val);
	char *opt = val;
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
		if (0 == strcmp(opt, "italics")) {
			specified = static_cast<flags>(specified | sdItalics);
			italics = true;
		}
		if (0 == strcmp(opt, "notitalics")) {
			specified = static_cast<flags>(specified | sdItalics);
			italics = false;
		}
		if (0 == strcmp(opt, "bold")) {
			specified = static_cast<flags>(specified | sdBold);
			bold = true;
		}
		if (0 == strcmp(opt, "notbold")) {
			specified = static_cast<flags>(specified | sdBold);
			bold = false;
		}
		if (0 == strcmp(opt, "font")) {
			specified = static_cast<flags>(specified | sdFont);
			font = colon;
			font.substitute('|', ',');
		}
		if (0 == strcmp(opt, "fore")) {
			specified = static_cast<flags>(specified | sdFore);
			fore = colon;
		}
		if (0 == strcmp(opt, "back")) {
			specified = static_cast<flags>(specified | sdBack);
			back = colon;
		}
		if (0 == strcmp(opt, "size")) {
			specified = static_cast<flags>(specified | sdSize);
			size = atoi(colon);
		}
		if (0 == strcmp(opt, "eolfilled")) {
			specified = static_cast<flags>(specified | sdEOLFilled);
			eolfilled = true;
		}
		if (0 == strcmp(opt, "noteolfilled")) {
			specified = static_cast<flags>(specified | sdEOLFilled);
			eolfilled = false;
		}
		if (0 == strcmp(opt, "underlined")) {
			specified = static_cast<flags>(specified | sdUnderlined);
			underlined = true;
		}
		if (0 == strcmp(opt, "notunderlined")) {
			specified = static_cast<flags>(specified | sdUnderlined);
			underlined = false;
		}
		if (0 == strcmp(opt, "case")) {
			specified = static_cast<flags>(specified | sdCaseForce);
			caseForce = SC_CASE_MIXED;
			if (colon) {
				if (*colon == 'u')
					caseForce = SC_CASE_UPPER;
				else if (*colon == 'l')
					caseForce = SC_CASE_LOWER;
			}
		}
		if (0 == strcmp(opt, "visible")) {
			specified = static_cast<flags>(specified | sdVisible);
			visible = true;
		}
		if (0 == strcmp(opt, "notvisible")) {
			specified = static_cast<flags>(specified | sdVisible);
			visible = false;
		}
		if (0 == strcmp(opt, "changeable")) {
			specified = static_cast<flags>(specified | sdChangeable);
			changeable = true;
		}
		if (0 == strcmp(opt, "notchangeable")) {
			specified = static_cast<flags>(specified | sdChangeable);
			changeable = false;
		}
		if (cpComma)
			opt = cpComma + 1;
		else
			opt = 0;
	}
	delete []val;
	return true;
}

long StyleDefinition::ForeAsLong() const {
	return ColourFromString(fore);
}

long StyleDefinition::BackAsLong() const {
	return ColourFromString(back);
}

void SciTEBase::SetOneStyle(Window &win, int style, const StyleDefinition &sd) {
	if (sd.specified & StyleDefinition::sdItalics)
		Platform::SendScintilla(win.GetID(), SCI_STYLESETITALIC, style, sd.italics ? 1 : 0);
	if (sd.specified & StyleDefinition::sdBold)
		Platform::SendScintilla(win.GetID(), SCI_STYLESETBOLD, style, sd.bold ? 1 : 0);
	if (sd.specified & StyleDefinition::sdFont)
		Platform::SendScintillaPointer(win.GetID(), SCI_STYLESETFONT, style,
			const_cast<char *>(sd.font.c_str()));
	if (sd.specified & StyleDefinition::sdFore)
		Platform::SendScintilla(win.GetID(), SCI_STYLESETFORE, style, sd.ForeAsLong());
	if (sd.specified & StyleDefinition::sdBack)
		Platform::SendScintilla(win.GetID(), SCI_STYLESETBACK, style, sd.BackAsLong());
	if (sd.specified & StyleDefinition::sdSize)
		Platform::SendScintilla(win.GetID(), SCI_STYLESETSIZE, style, sd.size);
	if (sd.specified & StyleDefinition::sdEOLFilled)
		Platform::SendScintilla(win.GetID(), SCI_STYLESETEOLFILLED, style, sd.eolfilled ? 1 : 0);
	if (sd.specified & StyleDefinition::sdUnderlined)
		Platform::SendScintilla(win.GetID(), SCI_STYLESETUNDERLINE, style, sd.underlined ? 1 : 0);
	if (sd.specified & StyleDefinition::sdCaseForce)
		Platform::SendScintilla(win.GetID(), SCI_STYLESETCASE, style, sd.caseForce);
	if (sd.specified & StyleDefinition::sdVisible)
		Platform::SendScintilla(win.GetID(), SCI_STYLESETVISIBLE, style, sd.visible ? 1 : 0);
	if (sd.specified & StyleDefinition::sdChangeable)
		Platform::SendScintilla(win.GetID(), SCI_STYLESETCHANGEABLE, style, sd.changeable ? 1 : 0);
	Platform::SendScintilla(win.GetID(), SCI_STYLESETCHARACTERSET, style, characterSet);
}

void SciTEBase::SetStyleFor(Window &win, const char *lang) {
	for (int style = 0; style <= STYLE_MAX; style++) {
		if (style != STYLE_DEFAULT) {
			char key[200];
			sprintf(key, "style.%s.%0d", lang, style);
			SString sval = props.GetExpanded(key);
			SetOneStyle(win, style, sval.c_str());
		}
	}
}

void LowerCaseString(char *s) {
	while (*s) {
		if ((*s >= 'A') && (*s <= 'Z')) {
			*s = static_cast<char>(*s - 'A' + 'a');
		}
		s++;
	}
}

SString SciTEBase::ExtensionFileName() {
	if (overrideExtension.length())
		return overrideExtension;
	else {
		FilePath name = filePath.Name();
		if (name.IsSet()) {
			// Force extension to lower case
			char fileNameWithLowerCaseExtension[MAX_PATH];
				strcpy(fileNameWithLowerCaseExtension, name.AsInternal());
			char *extension = strrchr(fileNameWithLowerCaseExtension, '.');
			if (extension) {
				LowerCaseString(extension);
			}
			return SString(fileNameWithLowerCaseExtension);
		} else {
			return props.Get("default.file.ext");
		}
	}
}

void SciTEBase::ForwardPropertyToEditor(const char *key) {
	SString value = props.Get(key);
	SendEditorString(SCI_SETPROPERTY,
	                 reinterpret_cast<uptr_t>(key), value.c_str());
}

void SciTEBase::DefineMarker(int marker, int markerType, ColourDesired fore, ColourDesired back) {
	SendEditor(SCI_MARKERDEFINE, marker, markerType);
	SendEditor(SCI_MARKERSETFORE, marker, fore.AsLong());
	SendEditor(SCI_MARKERSETBACK, marker, back.AsLong());
}

static int FileLength(const char *path) {
	int len = 0;
	FILE *fp = fopen(path, fileRead);
	if (fp) {
		fseek(fp, 0, SEEK_END);
		len = ftell(fp);
		fclose(fp);
	}
	return len;
}

void SciTEBase::ReadAPI(const SString &fileNameForExtension) {
	SString apisFileNames = props.GetNewExpand("api.",
	                        fileNameForExtension.c_str());
	size_t nameLength = apisFileNames.length();

	FilePath defaultDir = GetDefaultDirectory();

	if (nameLength) {
		apisFileNames.substitute(';', '\0');
		const char *apiFileName = apisFileNames.c_str();
		const char *nameEnd = apiFileName + nameLength;

		int tlen = 0;    // total api length

		// Calculate total length
		while (apiFileName < nameEnd) {

      //GLI adition to make the API paths in the main directory
      SString fullOpenAPIName = defaultDir.AsInternal();
      fullOpenAPIName += pathSepChar;
      fullOpenAPIName += apiFileName;

			tlen += FileLength(fullOpenAPIName.c_str());
			apiFileName += strlen(apiFileName) + 1;
		}

		// Load files
		if (tlen > 0) {
			char *buffer = apis.Allocate(tlen);
			if (buffer) {
				apiFileName = apisFileNames.c_str();

  			tlen = 0;
				while (apiFileName < nameEnd) {

          //GLI adition to make the API paths in the main directory
          SString fullOpenAPIName = defaultDir.AsInternal();
          fullOpenAPIName += pathSepChar;
          fullOpenAPIName += apiFileName;

					FILE *fp = fopen(fullOpenAPIName.c_str(), fileRead);
					if (fp) {
						fseek(fp, 0, SEEK_END);
						int len = ftell(fp);
						fseek(fp, 0, SEEK_SET);
						fread(buffer + tlen, 1, len, fp);
						tlen += len;
						fclose(fp);
					}
					apiFileName += strlen(apiFileName) + 1;
				}
				apis.SetFromAllocated();
			}
		}
	}
}

SString SciTEBase::FindLanguageProperty(const char *pattern, const char *defaultValue) {
	SString key = pattern;
	key.substitute("*", language.c_str());
	SString ret = props.GetExpanded(key.c_str());
	if (ret == "")
		ret = props.GetExpanded(pattern);
	if (ret == "")
		ret = defaultValue;
	return ret;
}

/**
 * A list of all the properties that should be forwarded to Scintilla lexers.
 */
static const char *propertiesToForward[] = {
	"fold",
	"fold.comment",
	"fold.compact",
	"fold.at.else",
	"fold.html",
	"fold.html.preprocessor",
	"fold.perl.package",
	"fold.perl.pod",
	"fold.preprocessor",
	"fold.quotes.python",
	"fold.verilog.flags",
	"styling.within.preprocessor",
	"tab.timmy.whinge.level",
	"asp.default.language",
	"html.tags.case.sensitive",
	"ps.level",
	"ps.tokenize",
	"sql.backslash.escapes",

	"lexer.tex.interface.default",
	"lexer.tex.comment.process",
	"lexer.tex.auto.if",
	"lexer.tex.use.keywords",
	"lexer.metapost.interface.default",
	"lexer.metapost.comment.process",
	"nsis.uservars",
	"nsis.ignorecase",

	0,
};

void SciTEBase::ReadProperties() {
	if (extender)
		extender->Clear();

	SString fileNameForExtension = ExtensionFileName();

	SString modulePath = props.GetNewExpand("lexerpath.",
	    fileNameForExtension.c_str());
	if (modulePath.length())
	    SendEditorString(SCI_LOADLEXERLIBRARY, 0, modulePath.c_str());
	language = props.GetNewExpand("lexer.", fileNameForExtension.c_str());
	if (language.length())
	    SendEditorString(SCI_SETLEXERLANGUAGE, 0, language.c_str());
	else
	    SendEditorString(SCI_SETLEXER, 0, SCLEX_CONTAINER);

	lexLanguage = SendEditor(SCI_GETLEXER);

	if ((lexLanguage == SCLEX_HTML) || (lexLanguage == SCLEX_XML) ||
	        (lexLanguage == SCLEX_ASP) || (lexLanguage == SCLEX_PHP))
		SendEditor(SCI_SETSTYLEBITS, 7);
	else
		SendEditor(SCI_SETSTYLEBITS, 5);

	SendOutput(SCI_SETLEXER, SCLEX_ERRORLIST);

	SString kw0 = props.GetNewExpand("keywords.", fileNameForExtension.c_str());
	SendEditorString(SCI_SETKEYWORDS, 0, kw0.c_str());

	for (int wl = 1; wl <= KEYWORDSET_MAX; wl++) {
		SString kwk(wl+1);
		kwk += '.';
		kwk.insert(0, "keywords");
		SString kw = props.GetNewExpand(kwk.c_str(), fileNameForExtension.c_str());
		SendEditorString(SCI_SETKEYWORDS, wl, kw.c_str());
	}

	FilePath homepath = GetSciteDefaultHome();
	props.Set("SciteDefaultHome", homepath.AsFileSystem());
	homepath = GetSciteUserHome();
	props.Set("SciteUserHome", homepath.AsFileSystem());

	for (size_t i=0; propertiesToForward[i]; i++) {
		ForwardPropertyToEditor(propertiesToForward[i]);
	}

	if (apisFileNames != props.GetNewExpand("api.",	fileNameForExtension.c_str())) {
		apis.Clear();
		ReadAPI(fileNameForExtension);
		apisFileNames = props.GetNewExpand("api.", fileNameForExtension.c_str());
	}

	if (!props.GetInt("eol.auto")) {
		SetEol();
	}

	codePage = props.GetInt("code.page");
	if (unicodeMode != uni8Bit) {
		// Override properties file to ensure Unicode displayed.
		codePage = SC_CP_UTF8;
	}
	SendEditor(SCI_SETCODEPAGE, codePage);
	int outputCodePage = props.GetInt("output.code.page", codePage);
	SendOutput(SCI_SETCODEPAGE, outputCodePage);

	characterSet = props.GetInt("character.set", SC_CHARSET_DEFAULT);

#ifdef unix
	SString localeCType = props.Get("LC_CTYPE");
	if (localeCType.length())
		setlocale(LC_CTYPE, localeCType.c_str());
	else
		setlocale(LC_CTYPE, "C");
#endif

	wrapStyle = props.GetInt("wrap.style", SC_WRAP_WORD);

	SendEditor(SCI_SETCARETFORE,
	           ColourOfProperty(props, "caret.fore", ColourDesired(0, 0, 0)));

	SendEditor(SCI_SETMOUSEDWELLTIME,
	           props.GetInt("dwell.period", SC_TIME_FOREVER), 0);

	SendEditor(SCI_SETCARETWIDTH, props.GetInt("caret.width", 1));
	SendOutput(SCI_SETCARETWIDTH, props.GetInt("caret.width", 1));

	SString caretLineBack = props.Get("caret.line.back");
	if (caretLineBack.length()) {
		SendEditor(SCI_SETCARETLINEVISIBLE, 1);
		SendEditor(SCI_SETCARETLINEBACK, ColourFromString(caretLineBack));
	} else {
		SendEditor(SCI_SETCARETLINEVISIBLE, 0);
	}

	SString controlCharSymbol = props.Get("control.char.symbol");
	if (controlCharSymbol.length()) {
		SendEditor(SCI_SETCONTROLCHARSYMBOL, static_cast<unsigned char>(controlCharSymbol[0]));
	} else {
		SendEditor(SCI_SETCONTROLCHARSYMBOL, 0);
	}

	SendEditor(SCI_CALLTIPSETBACK,
	           ColourOfProperty(props, "calltip.back", ColourDesired(0xff, 0xff, 0xff)));

	SString caretPeriod = props.Get("caret.period");
	if (caretPeriod.length()) {
		SendEditor(SCI_SETCARETPERIOD, caretPeriod.value());
		SendOutput(SCI_SETCARETPERIOD, caretPeriod.value());
	}

	int caretSlop = props.GetInt("caret.policy.xslop", 1) ? CARET_SLOP : 0;
	int caretZone = props.GetInt("caret.policy.width", 50);
	int caretStrict = props.GetInt("caret.policy.xstrict") ? CARET_STRICT : 0;
	int caretEven = props.GetInt("caret.policy.xeven", 1) ? CARET_EVEN : 0;
	int caretJumps = props.GetInt("caret.policy.xjumps") ? CARET_JUMPS : 0;
	SendEditor(SCI_SETXCARETPOLICY, caretStrict | caretSlop | caretEven | caretJumps, caretZone);

	caretSlop = props.GetInt("caret.policy.yslop", 1) ? CARET_SLOP : 0;
	caretZone = props.GetInt("caret.policy.lines");
	caretStrict = props.GetInt("caret.policy.ystrict") ? CARET_STRICT : 0;
	caretEven = props.GetInt("caret.policy.yeven", 1) ? CARET_EVEN : 0;
	caretJumps = props.GetInt("caret.policy.yjumps") ? CARET_JUMPS : 0;
	SendEditor(SCI_SETYCARETPOLICY, caretStrict | caretSlop | caretEven | caretJumps, caretZone);

	int visibleStrict = props.GetInt("visible.policy.strict") ? VISIBLE_STRICT : 0;
	int visibleSlop = props.GetInt("visible.policy.slop", 1) ? VISIBLE_SLOP : 0;
	int visibleLines = props.GetInt("visible.policy.lines");
	SendEditor(SCI_SETVISIBLEPOLICY, visibleStrict | visibleSlop, visibleLines);

	SendEditor(SCI_SETEDGECOLUMN, props.GetInt("edge.column", 0));
	SendEditor(SCI_SETEDGEMODE, props.GetInt("edge.mode", EDGE_NONE));
	SendEditor(SCI_SETEDGECOLOUR,
	           ColourOfProperty(props, "edge.colour", ColourDesired(0xff, 0xda, 0xda)));

	SString selFore = props.Get("selection.fore");
	if (selFore.length()) {
		SendChildren(SCI_SETSELFORE, 1, ColourFromString(selFore));
	} else {
		SendChildren(SCI_SETSELFORE, 0, 0);
	}
	SString selBack = props.Get("selection.back");
	if (selBack.length()) {
		SendChildren(SCI_SETSELBACK, 1, ColourFromString(selBack));
	} else {
		if (selFore.length())
			SendChildren(SCI_SETSELBACK, 0, 0);
		else	// Have to show selection somehow
			SendChildren(SCI_SETSELBACK, 1, ColourDesired(0xC0, 0xC0, 0xC0).AsLong());
	}

	SString foldColour = props.Get("fold.margin.colour");
	if (foldColour.length()) {
		SendChildren(SCI_SETFOLDMARGINCOLOUR, 1, ColourFromString(foldColour));
	} else {
		SendChildren(SCI_SETFOLDMARGINCOLOUR, 0, 0);
	}
	SString foldHiliteColour = props.Get("fold.margin.highlight.colour");
	if (foldHiliteColour.length()) {
		SendChildren(SCI_SETFOLDMARGINHICOLOUR, 1, ColourFromString(foldHiliteColour));
	} else {
		SendChildren(SCI_SETFOLDMARGINHICOLOUR, 0, 0);
	}

	SString whitespaceFore = props.Get("whitespace.fore");
	if (whitespaceFore.length()) {
		SendChildren(SCI_SETWHITESPACEFORE, 1, ColourFromString(whitespaceFore));
	} else {
		SendChildren(SCI_SETWHITESPACEFORE, 0, 0);
	}
	SString whitespaceBack = props.Get("whitespace.back");
	if (whitespaceBack.length()) {
		SendChildren(SCI_SETWHITESPACEBACK, 1, ColourFromString(whitespaceBack));
	} else {
		SendChildren(SCI_SETWHITESPACEBACK, 0, 0);
	}

	char bracesStyleKey[200];
	sprintf(bracesStyleKey, "braces.%s.style", language.c_str());
	bracesStyle = props.GetInt(bracesStyleKey, 0);

	char key[200];
	SString sval;

	sval = FindLanguageProperty("calltip.*.ignorecase");
	callTipIgnoreCase = sval == "1";

	calltipWordCharacters = FindLanguageProperty("calltip.*.word.characters",
		"_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

	calltipParametersStart = FindLanguageProperty("calltip.*.parameters.start", "(");
	calltipParametersEnd = FindLanguageProperty("calltip.*.parameters.end", ")");
	calltipParametersSeparators = FindLanguageProperty("calltip.*.parameters.separators", ",;");

	calltipEndDefinition = FindLanguageProperty("calltip.*.end.definition");

	sprintf(key, "autocomplete.%s.start.characters", language.c_str());
	autoCompleteStartCharacters = props.GetExpanded(key);
	if (autoCompleteStartCharacters == "")
		autoCompleteStartCharacters = props.GetExpanded("autocomplete.*.start.characters");
	// "" is a quite reasonable value for this setting

	sprintf(key, "autocomplete.%s.fillups", language.c_str());
	autoCompleteFillUpCharacters = props.GetExpanded(key);
	if (autoCompleteFillUpCharacters == "")
		autoCompleteFillUpCharacters =
			props.GetExpanded("autocomplete.*.fillups");
	SendEditorString(SCI_AUTOCSETFILLUPS, 0,
		autoCompleteFillUpCharacters.c_str());

	sprintf(key, "autocomplete.%s.ignorecase", "*");
	sval = props.GetNewExpand(key);
	autoCompleteIgnoreCase = sval == "1";
	sprintf(key, "autocomplete.%s.ignorecase", language.c_str());
	sval = props.GetNewExpand(key);
	if (sval != "")
		autoCompleteIgnoreCase = sval == "1";
	SendEditor(SCI_AUTOCSETIGNORECASE, autoCompleteIgnoreCase ? 1 : 0);

	int autoCChooseSingle = props.GetInt("autocomplete.choose.single");
	SendEditor(SCI_AUTOCSETCHOOSESINGLE, autoCChooseSingle),

	SendEditor(SCI_AUTOCSETCANCELATSTART, 0);
	SendEditor(SCI_AUTOCSETDROPRESTOFWORD, 0);

	if (firstPropertiesRead) {
		ReadPropertiesInitial();
	}

	ReadFontProperties();

	SendEditor(SCI_SETUSEPALETTE, props.GetInt("use.palette"));
	SendEditor(SCI_SETPRINTMAGNIFICATION, props.GetInt("print.magnification"));
	SendEditor(SCI_SETPRINTCOLOURMODE, props.GetInt("print.colour.mode"));

	clearBeforeExecute = props.GetInt("clear.before.execute");
	timeCommands = props.GetInt("time.commands");

	int blankMarginLeft = props.GetInt("blank.margin.left", 1);
	int blankMarginRight = props.GetInt("blank.margin.right", 1);
	SendEditor(SCI_SETMARGINLEFT, 0, blankMarginLeft);
	SendEditor(SCI_SETMARGINRIGHT, 0, blankMarginRight);
	SendOutput(SCI_SETMARGINLEFT, 0, blankMarginLeft);
	SendOutput(SCI_SETMARGINRIGHT, 0, blankMarginRight);

	SendEditor(SCI_SETMARGINWIDTHN, 1, margin ? marginWidth : 0);

	SString lineMarginProp = props.Get("line.margin.width");
	lineNumbersWidth = lineMarginProp.value();
	if (lineNumbersWidth == 0)
		lineNumbersWidth = lineNumbersWidthDefault;
	lineNumbersExpand = lineMarginProp.contains('+');

	SetLineNumberWidth();

	bufferedDraw = props.GetInt("buffered.draw", 1);
	SendEditor(SCI_SETBUFFEREDDRAW, bufferedDraw);

	twoPhaseDraw = props.GetInt("two.phase.draw", 1);
	SendEditor(SCI_SETTWOPHASEDRAW, twoPhaseDraw);

	SendEditor(SCI_SETLAYOUTCACHE, props.GetInt("cache.layout"));
	SendOutput(SCI_SETLAYOUTCACHE, props.GetInt("output.cache.layout"));

	bracesCheck = props.GetInt("braces.check");
	bracesSloppy = props.GetInt("braces.sloppy");

	SendEditor(SCI_SETCHARSDEFAULT);
	wordCharacters = props.GetNewExpand("word.characters.", fileNameForExtension.c_str());
	if (wordCharacters.length()) {
		SendEditorString(SCI_SETWORDCHARS, 0, wordCharacters.c_str());
	} else {
		wordCharacters = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	}

	whitespaceCharacters = props.GetNewExpand("whitespace.characters.", fileNameForExtension.c_str());
	if (whitespaceCharacters.length()) {
		SendEditorString(SCI_SETWHITESPACECHARS, 0, whitespaceCharacters.c_str());
	}

	SendEditor(SCI_SETTABINDENTS, props.GetInt("tab.indents", 1));
	SendEditor(SCI_SETBACKSPACEUNINDENTS, props.GetInt("backspace.unindents", 1));

	indentOpening = props.GetInt("indent.opening");
	indentClosing = props.GetInt("indent.closing");
	indentMaintain = props.GetNewExpand("indent.maintain.", fileNameForExtension.c_str()).value();

	SString lookback = props.GetNewExpand("statement.lookback.", fileNameForExtension.c_str());
	statementLookback = lookback.value();
	statementIndent = GetStyleAndWords("statement.indent.");
	statementEnd = GetStyleAndWords("statement.end.");
	blockStart = GetStyleAndWords("block.start.");
	blockEnd = GetStyleAndWords("block.end.");

	SString list;
	list = props.GetNewExpand("preprocessor.symbol.", fileNameForExtension.c_str());
	preprocessorSymbol = list[0];
	list = props.GetNewExpand("preprocessor.start.", fileNameForExtension.c_str());
	preprocCondStart.Clear();
	preprocCondStart.Set(list.c_str());
	list = props.GetNewExpand("preprocessor.middle.", fileNameForExtension.c_str());
	preprocCondMiddle.Clear();
	preprocCondMiddle.Set(list.c_str());
	list = props.GetNewExpand("preprocessor.end.", fileNameForExtension.c_str());
	preprocCondEnd.Clear();
	preprocCondEnd.Set(list.c_str());

	memFiles.AppendList(props.GetNewExpand("find.files"));

	SendEditor(SCI_SETWRAPVISUALFLAGS, props.GetInt("wrap.visual.flags"));
	SendEditor(SCI_SETWRAPVISUALFLAGSLOCATION, props.GetInt("wrap.visual.flags.location"));
 	SendEditor(SCI_SETWRAPSTARTINDENT, props.GetInt("wrap.visual.startindent"));

	if (props.GetInt("wrap.aware.home.end.keys",0)) {
		if (props.GetInt("vc.home.key", 1)) {
			AssignKey(SCK_HOME, 0, SCI_VCHOMEWRAP);
			AssignKey(SCK_HOME, SCMOD_SHIFT, SCI_VCHOMEWRAPEXTEND);
			AssignKey(SCK_HOME, SCMOD_SHIFT | SCMOD_ALT, SCI_VCHOMERECTEXTEND);
		} else {
			AssignKey(SCK_HOME, 0, SCI_HOMEWRAP);
			AssignKey(SCK_HOME, SCMOD_SHIFT, SCI_HOMEWRAPEXTEND);
			AssignKey(SCK_HOME, SCMOD_SHIFT | SCMOD_ALT, SCI_HOMERECTEXTEND);
		}
		AssignKey(SCK_END, 0, SCI_LINEENDWRAP);
		AssignKey(SCK_END, SCMOD_SHIFT, SCI_LINEENDWRAPEXTEND);
	} else {
		if (props.GetInt("vc.home.key", 1)) {
			AssignKey(SCK_HOME, 0, SCI_VCHOME);
			AssignKey(SCK_HOME, SCMOD_SHIFT, SCI_VCHOMEEXTEND);
			AssignKey(SCK_HOME, SCMOD_SHIFT | SCMOD_ALT, SCI_VCHOMERECTEXTEND);
		} else {
			AssignKey(SCK_HOME, 0, SCI_HOME);
			AssignKey(SCK_HOME, SCMOD_SHIFT, SCI_HOMEEXTEND);
			AssignKey(SCK_HOME, SCMOD_SHIFT | SCMOD_ALT, SCI_HOMERECTEXTEND);
		}
		AssignKey(SCK_END, 0, SCI_LINEEND);
		AssignKey(SCK_END, SCMOD_SHIFT, SCI_LINEENDEXTEND);
	}

	AssignKey('L', SCMOD_SHIFT | SCMOD_CTRL, SCI_LINEDELETE);

	scrollOutput = props.GetInt("output.scroll", 1);

	tabHideOne = props.GetInt("tabbar.hide.one");

	SetToolsMenu();

	SendEditor(SCI_SETFOLDFLAGS, props.GetInt("fold.flags"));

	// To put the folder markers in the line number region
	//SendEditor(SCI_SETMARGINMASKN, 0, SC_MASK_FOLDERS);

	SendEditor(SCI_SETMODEVENTMASK, SC_MOD_CHANGEFOLD);

	if (0==props.GetInt("undo.redo.lazy")) {
		// Trap for insert/delete notifications (also fired by undo
		// and redo) so that the buttons can be enabled if needed.
		SendEditor(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT
			| SC_LASTSTEPINUNDOREDO | SendEditor(SCI_GETMODEVENTMASK, 0));

		//SC_LASTSTEPINUNDOREDO is probably not needed in the mask; it
		//doesn't seem to fire as an event of its own; just modifies the
		//insert and delete events.
	}

	// Create a margin column for the folding symbols
	SendEditor(SCI_SETMARGINTYPEN, 2, SC_MARGIN_SYMBOL);

	SendEditor(SCI_SETMARGINWIDTHN, 2, foldMargin ? foldMarginWidth : 0);

	SendEditor(SCI_SETMARGINMASKN, 2, SC_MASK_FOLDERS);
	SendEditor(SCI_SETMARGINSENSITIVEN, 2, 1);
	
#ifdef GLI_CHANGES

  //Enable the marker bar to be clickable
  SendEditor(SCI_SETMARGINSENSITIVEN, 1, 1);

#endif //GLI_CHANGES

	switch (props.GetInt("fold.symbols")) {
	case 0:
		// Arrow pointing right for contracted folders, arrow pointing down for expanded
		DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_ARROWDOWN,
		             ColourDesired(0, 0, 0), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_ARROW,
		             ColourDesired(0, 0, 0), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY,
		             ColourDesired(0, 0, 0), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY,
		             ColourDesired(0, 0, 0), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY,
		             ColourDesired(0xff, 0xff, 0xff), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY,
		             ColourDesired(0xff, 0xff, 0xff), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0, 0, 0));
		break;
	case 1:
		// Plus for contracted folders, minus for expanded
		DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_PLUS, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0, 0, 0));
		DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0, 0, 0));
		break;
	case 2:
		// Like a flattened tree control using circular headers and curved joins
		DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_CIRCLEMINUS, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x40, 0x40, 0x40));
		DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_CIRCLEPLUS, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x40, 0x40, 0x40));
		DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x40, 0x40, 0x40));
		DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x40, 0x40, 0x40));
		DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_CIRCLEPLUSCONNECTED, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x40, 0x40, 0x40));
		DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_CIRCLEMINUSCONNECTED, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x40, 0x40, 0x40));
		DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x40, 0x40, 0x40));
		break;
	case 3:
		// Like a flattened tree control using square headers
		DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x80, 0x80, 0x80));
		DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x80, 0x80, 0x80));
		DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x80, 0x80, 0x80));
		DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x80, 0x80, 0x80));
		DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x80, 0x80, 0x80));
		DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x80, 0x80, 0x80));
		DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER, ColourDesired(0xff, 0xff, 0xff), ColourDesired(0x80, 0x80, 0x80));
		break;
	}

	SendEditor(SCI_MARKERSETFORE, SciTE_MARKER_BOOKMARK,
	           ColourOfProperty(props, "bookmark.fore", ColourDesired(0, 0, 0x7f)));
	SendEditor(SCI_MARKERSETBACK, SciTE_MARKER_BOOKMARK,
	           ColourOfProperty(props, "bookmark.back", ColourDesired(0x80, 0xff, 0xff)));
	SString bookMarkXPM = props.Get("bookmark.pixmap");
	if (bookMarkXPM.length()) {
		SendEditorString(SCI_MARKERDEFINEPIXMAP, SciTE_MARKER_BOOKMARK,
			bookMarkXPM.c_str());
	} else {
		SendEditor(SCI_MARKERDEFINE, SciTE_MARKER_BOOKMARK, SC_MARK_CIRCLE);
	}

#ifdef GLI_CHANGES

  //Register the breakpoint and arrow markers
  SendEditor(SCI_MARKERSETFORE, SciTE_MARKER_BREAKPOINT, 0x000080);
  SendEditor(SCI_MARKERSETBACK, SciTE_MARKER_BREAKPOINT, 0x000080);
  SendEditor(SCI_MARKERDEFINE,  SciTE_MARKER_BREAKPOINT, SC_MARK_CIRCLE);

  SendEditor(SCI_MARKERSETFORE, SciTE_MARKER_ARROW, 0x00FFFF);
  SendEditor(SCI_MARKERSETBACK, SciTE_MARKER_ARROW, 0x00FFFF);
  SendEditor(SCI_MARKERDEFINE,  SciTE_MARKER_ARROW, SC_MARK_SHORTARROW);

#endif //GLI_CHANGES

	SendEditor(SCI_SETSCROLLWIDTH, props.GetInt("horizontal.scroll.width", 2000));
	SendOutput(SCI_SETSCROLLWIDTH, props.GetInt("output.horizontal.scroll.width", 2000));

	// Do these last as they force a style refresh
	SendEditor(SCI_SETHSCROLLBAR, props.GetInt("horizontal.scrollbar", 1));
	SendOutput(SCI_SETHSCROLLBAR, props.GetInt("output.horizontal.scrollbar", 1));

	SendEditor(SCI_SETENDATLASTLINE, props.GetInt("end.at.last.line", 1));
	SendEditor(SCI_SETCARETSTICKY, props.GetInt("caret.sticky", 0));

	if (extender) {
		FilePath defaultDir = GetDefaultDirectory();
		FilePath scriptPath;

		// Check for an extension script
		SString extensionFile = props.GetNewExpand("extension.", fileNameForExtension.c_str());
		if (extensionFile.length()) {
			// find file in local directory
			FilePath docDir = filePath.Directory();
			if (Exists(docDir.AsInternal(), extensionFile.c_str(), &scriptPath)) {
				// Found file in document directory
				extender->Load(scriptPath.AsFileSystem());
			} else if (Exists(defaultDir.AsInternal(), extensionFile.c_str(), &scriptPath)) {
				// Found file in global directory
				extender->Load(scriptPath.AsFileSystem());
			} else if (Exists("", extensionFile.c_str(), &scriptPath)) {
				// Found as completely specified file name
				extender->Load(scriptPath.AsFileSystem());
			}
		}
	}
	firstPropertiesRead = false;
	needReadProperties = false;
}

void SciTEBase::ReadFontProperties() {
	char key[200];
	SString sval;

	// Set styles
	// For each window set the global default style, then the language default style, then the other global styles, then the other language styles

	SendEditor(SCI_STYLERESETDEFAULT, 0, 0);
	SendOutput(SCI_STYLERESETDEFAULT, 0, 0);

	sprintf(key, "style.%s.%0d", "*", STYLE_DEFAULT);
	sval = props.GetNewExpand(key);
	SetOneStyle(wEditor, STYLE_DEFAULT, sval.c_str());
	SetOneStyle(wOutput, STYLE_DEFAULT, sval.c_str());

	sprintf(key, "style.%s.%0d", language.c_str(), STYLE_DEFAULT);
	sval = props.GetNewExpand(key);
	SetOneStyle(wEditor, STYLE_DEFAULT, sval.c_str());

	SendEditor(SCI_STYLECLEARALL, 0, 0);

	SetStyleFor(wEditor, "*");
	SetStyleFor(wEditor, language.c_str());

	SendOutput(SCI_STYLECLEARALL, 0, 0);

	sprintf(key, "style.%s.%0d", "errorlist", STYLE_DEFAULT);
	sval = props.GetNewExpand(key);
	SetOneStyle(wOutput, STYLE_DEFAULT, sval.c_str());

	SendOutput(SCI_STYLECLEARALL, 0, 0);

	SetStyleFor(wOutput, "*");
	SetStyleFor(wOutput, "errorlist");

	if (useMonoFont) {
		sval = props.GetExpanded("font.monospace");
		StyleDefinition sd(sval.c_str());
		for (int style = 0; style <= STYLE_MAX; style++) {
			if (style != STYLE_LINENUMBER) {
				if (sd.specified & StyleDefinition::sdFont) {
					SendEditorString(SCI_STYLESETFONT, style, sd.font.c_str());
				}
				if (sd.specified & StyleDefinition::sdSize) {
					SendEditor(SCI_STYLESETSIZE, style, sd.size);
				}
			}
		}
	}
}

// Properties that are interactively modifiable are only read from the properties file once.
void SciTEBase::SetPropertiesInitial() {
  
	splitVertical = props.GetInt("split.vertical");
	openFilesHere = props.GetInt("check.if.already.open");
	wrap = props.GetInt("wrap");
	wrapOutput = props.GetInt("output.wrap");
	indentationWSVisible = props.GetInt("view.indentation.whitespace", 1);
	sbVisible = props.GetInt("statusbar.visible");
	tbVisible = props.GetInt("toolbar.visible");
	tabVisible = props.GetInt("tabbar.visible");
	tabMultiLine = props.GetInt("tabbar.multiline");
	lineNumbers = props.GetInt("line.margin.visible");
	marginWidth = 0;
	SString margwidth = props.Get("margin.width");
	if (margwidth.length())
		marginWidth = margwidth.value();
	margin = marginWidth;
	if (marginWidth == 0)
		marginWidth = marginWidthDefault;
	foldMarginWidth = props.GetInt("fold.margin.width", foldMarginWidthDefault);
	foldMargin = foldMarginWidth;
	if (foldMarginWidth == 0)
		foldMarginWidth = foldMarginWidthDefault;

	matchCase = props.GetInt("find.replace.matchcase");
	regExp = props.GetInt("find.replace.regexp");
	unSlash = props.GetInt("find.replace.escapes");
	wrapFind = props.GetInt("find.replace.wrap", 1);
}

SString SciTEBase::LocaliseString(const char *s, bool retainIfNotFound) {
	SString translation = s;
	int ellipseIndicator = translation.remove("...");
	int accessKeyPresent = translation.remove(menuAccessIndicator);
	translation.lowercase();
	translation.substitute("\n", "\\n");
	translation = propsUI.Get(translation.c_str());
	if (translation.length()) {
		if (ellipseIndicator)
			translation += "...";
		if (0 == accessKeyPresent) {
#if PLAT_WIN
			// Following codes are required because accelerator is not always
			// part of alphabetical word in several language. In these cases,
			// accelerator is written like "(&O)".
			int posOpenParenAnd = translation.search("(&");
			if (posOpenParenAnd > 0 && translation.search(")", posOpenParenAnd) == posOpenParenAnd+3) {
				translation.remove(posOpenParenAnd, 4);
			} else {
				translation.remove("&");
			}
#else
			translation.remove("&");
#endif
		}
		translation.substitute("&", menuAccessIndicator);
		translation.substitute("\\n", "\n");
	} else {
		translation = props.Get("translation.missing");
	}
	if ((translation.length() > 0) || !retainIfNotFound) {
		return translation;
	}
	return s;
}

SString SciTEBase::LocaliseMessage(const char *s, const char *param0, const char *param1, const char *param2) {
	SString translation = LocaliseString(s);
	if (param0)
		translation.substitute("^0", param0);
	if (param1)
		translation.substitute("^1", param1);
	if (param2)
		translation.substitute("^2", param2);
	return translation;
}

void SciTEBase::ReadLocalisation() {
	propsUI.Clear();
	const char *title = "locale.properties";
	SString localeProps = props.GetExpanded(title);
	if (localeProps.length()) {
		title = localeProps.c_str();
	}
	FilePath propdir = GetSciteDefaultHome();
	FilePath localePath(propdir, title);
	propsUI.Read(localePath, propdir, importFiles, importMax);
	localisationRead = true;
}

void SciTEBase::ReadPropertiesInitial() {
	SetPropertiesInitial();
	int sizeHorizontal = props.GetInt("output.horizontal.size", 0);
	int sizeVertical = props.GetInt("output.vertical.size", 0);
	if ((!splitVertical && (sizeVertical > 0) && (heightOutput < sizeVertical)) ||
		(splitVertical && (sizeHorizontal > 0) && (heightOutput < sizeHorizontal))) {
		heightOutput = NormaliseSplit(splitVertical ? sizeHorizontal : sizeVertical);
		SizeSubWindows();
		Redraw();
	}
	ViewWhitespace(props.GetInt("view.whitespace"));
	SendEditor(SCI_SETINDENTATIONGUIDES, props.GetInt("view.indentation.guides"));
	SendEditor(SCI_SETVIEWEOL, props.GetInt("view.eol"));
	SendEditor(SCI_SETZOOM, props.GetInt("magnification"));
	SendOutput(SCI_SETZOOM, props.GetInt("output.magnification"));
	SendEditor(SCI_SETWRAPMODE, wrap ? wrapStyle : SC_WRAP_NONE);
	SendOutput(SCI_SETWRAPMODE, wrapOutput ? wrapStyle : SC_WRAP_NONE);

	useMonoFont = props.GetInt("use.monospaced");

	SString menuLanguageProp = props.GetNewExpand("menu.language");
	languageItems = 0;
	for (unsigned int i = 0; i < menuLanguageProp.length(); i++) {
		if (menuLanguageProp[i] == '|')
			languageItems++;
	}
	languageItems /= 3;
	languageMenu = new LanguageMenuItem[languageItems];

	menuLanguageProp.substitute('|', '\0');
	const char *sMenuLanguage = menuLanguageProp.c_str();
	for (int item = 0; item < languageItems; item++) {
		languageMenu[item].menuItem = sMenuLanguage;
		sMenuLanguage += strlen(sMenuLanguage) + 1;
		languageMenu[item].extension = sMenuLanguage;
		sMenuLanguage += strlen(sMenuLanguage) + 1;
		languageMenu[item].menuKey = sMenuLanguage;
		sMenuLanguage += strlen(sMenuLanguage) + 1;
	}
	SetLanguageMenu();

	// load the user defined short cut props
	SString shortCutProp = props.GetNewExpand("user.shortcuts");
	if (shortCutProp.length()) {
		shortCutItems = 0;
		for (unsigned int i = 0; i < shortCutProp.length(); i++) {
			if (shortCutProp[i] == '|')
				shortCutItems++;
		}
		shortCutItems /= 2;
		shortCutItemList = new ShortcutItem[shortCutItems];
		shortCutProp.substitute('|', '\0');
		const char *sShortCutProp = shortCutProp.c_str();
		for (int item = 0; item < shortCutItems; item++) {
			shortCutItemList[item].menuKey = sShortCutProp;
			sShortCutProp += strlen(sShortCutProp) + 1;
			shortCutItemList[item].menuCommand = sShortCutProp;
			sShortCutProp += strlen(sShortCutProp) + 1;
		}
	}
	// end load the user defined short cut props


#if PLAT_WIN

	if (tabMultiLine) {	// Windows specific!
		long wl = ::GetWindowLong(reinterpret_cast<HWND>(wTabBar.GetID()), GWL_STYLE);
		::SetWindowLong(reinterpret_cast<HWND>(wTabBar.GetID()), GWL_STYLE, wl | TCS_MULTILINE);
	}
#endif

	FilePath homepath = GetSciteDefaultHome();
	props.Set("SciteDefaultHome", homepath.AsFileSystem());
	homepath = GetSciteUserHome();
	props.Set("SciteUserHome", homepath.AsFileSystem());
}

FilePath SciTEBase::GetDefaultPropertiesFileName() {
	return FilePath(GetSciteDefaultHome(), propGlobalFileName);
}

FilePath SciTEBase::GetAbbrevPropertiesFileName() {
	return FilePath(GetSciteUserHome(), propAbbrevFileName);
}

FilePath SciTEBase::GetUserPropertiesFileName() {
	return FilePath(GetSciteUserHome(), propUserFileName);
}

void SciTEBase::OpenProperties(int propsFile) {
	FilePath propfile;
	if (propsFile == IDM_OPENLOCALPROPERTIES) {
		FilePath propdir = filePath.Directory();
		propfile.Set(propdir, propFileName);
		Open(propfile, ofQuiet);
	} else if (propsFile == IDM_OPENUSERPROPERTIES) {
		propfile = GetUserPropertiesFileName();
		Open(propfile, ofQuiet);
	} else if (propsFile == IDM_OPENABBREVPROPERTIES) {
		propfile = GetAbbrevPropertiesFileName();
		Open(propfile, ofQuiet);
	} else if (propsFile == IDM_OPENGLOBALPROPERTIES) {
		propfile = GetDefaultPropertiesFileName();
		Open(propfile, ofQuiet);
	} else if (propsFile == IDM_OPENLUAEXTERNALFILE) {
		SString extlua = props.GetExpanded("ext.lua.startup.script");
		if (extlua.length()) {
			Open(extlua.c_str(), ofQuiet);
		}
	}
}

// return the int value of the command name passed in.
int SciTEBase::GetMenuCommandAsInt(SString commandName) {
	struct {
		const char *name;
		int id;
	} idmTable[] = {
//++Autogenerated -- run ../scintilla/src/LexGen.py to regenerate
//**\(\t\t{"\*",\t\*},\n\)
		{"IDM_MRUFILE",	IDM_MRUFILE},
		{"IDM_TOOLS",	IDM_TOOLS},
		{"IDM_BUFFER",	IDM_BUFFER},
		{"IDM_IMPORT",	IDM_IMPORT},
		{"IDM_LANGUAGE",	IDM_LANGUAGE},
		{"IDM_NEW",	IDM_NEW},
		{"IDM_OPEN",	IDM_OPEN},
		{"IDM_OPENSELECTED",	IDM_OPENSELECTED},
		{"IDM_REVERT",	IDM_REVERT},
		{"IDM_CLOSE",	IDM_CLOSE},
		{"IDM_SAVE",	IDM_SAVE},
		{"IDM_SAVEAS",	IDM_SAVEAS},
		{"IDM_SAVEASHTML",	IDM_SAVEASHTML},
		{"IDM_SAVEASRTF",	IDM_SAVEASRTF},
		{"IDM_SAVEASPDF",	IDM_SAVEASPDF},
		{"IDM_FILER",	IDM_FILER},
		{"IDM_SAVEASTEX",	IDM_SAVEASTEX},
		{"IDM_SAVEACOPY",	IDM_SAVEACOPY},
		{"IDM_SAVEASXML",	IDM_SAVEASXML},
		{"IDM_MRU_SEP",	IDM_MRU_SEP},
		{"IDM_PRINTSETUP",	IDM_PRINTSETUP},
		{"IDM_PRINT",	IDM_PRINT},
		{"IDM_LOADSESSION",	IDM_LOADSESSION},
		{"IDM_SAVESESSION",	IDM_SAVESESSION},
		{"IDM_QUIT",	IDM_QUIT},
		{"IDM_ENCODING_DEFAULT",	IDM_ENCODING_DEFAULT},
		{"IDM_ENCODING_UCS2BE",	IDM_ENCODING_UCS2BE},
		{"IDM_ENCODING_UCS2LE",	IDM_ENCODING_UCS2LE},
		{"IDM_ENCODING_UTF8",	IDM_ENCODING_UTF8},
		{"IDM_ENCODING_UCOOKIE",	IDM_ENCODING_UCOOKIE},
		{"IDM_UNDO",	IDM_UNDO},
		{"IDM_REDO",	IDM_REDO},
		{"IDM_CUT",	IDM_CUT},
		{"IDM_COPY",	IDM_COPY},
		{"IDM_PASTE",	IDM_PASTE},
		{"IDM_CLEAR",	IDM_CLEAR},
		{"IDM_SELECTALL",	IDM_SELECTALL},
		{"IDM_PASTEANDDOWN",	IDM_PASTEANDDOWN},
		{"IDM_FIND",	IDM_FIND},
		{"IDM_FINDNEXT",	IDM_FINDNEXT},
		{"IDM_FINDNEXTBACK",	IDM_FINDNEXTBACK},
		{"IDM_FINDNEXTSEL",	IDM_FINDNEXTSEL},
		{"IDM_FINDNEXTBACKSEL",	IDM_FINDNEXTBACKSEL},
		{"IDM_FINDINFILES",	IDM_FINDINFILES},
		{"IDM_REPLACE",	IDM_REPLACE},
		{"IDM_GOTO",	IDM_GOTO},
		{"IDM_BOOKMARK_NEXT",	IDM_BOOKMARK_NEXT},
		{"IDM_BOOKMARK_TOGGLE",	IDM_BOOKMARK_TOGGLE},
		{"IDM_BOOKMARK_PREV",	IDM_BOOKMARK_PREV},
		{"IDM_BOOKMARK_CLEARALL",	IDM_BOOKMARK_CLEARALL},
		{"IDM_BOOKMARK_NEXT_SELECT",	IDM_BOOKMARK_NEXT_SELECT},
		{"IDM_BOOKMARK_PREV_SELECT",	IDM_BOOKMARK_PREV_SELECT},
		{"IDM_MATCHBRACE",	IDM_MATCHBRACE},
		{"IDM_SELECTTOBRACE",	IDM_SELECTTOBRACE},
		{"IDM_SHOWCALLTIP",	IDM_SHOWCALLTIP},
		{"IDM_COMPLETE",	IDM_COMPLETE},
		{"IDM_COMPLETEWORD",	IDM_COMPLETEWORD},
		{"IDM_EXPAND",	IDM_EXPAND},
		{"IDM_TOGGLE_FOLDALL",	IDM_TOGGLE_FOLDALL},
		{"IDM_TOGGLE_FOLDRECURSIVE",	IDM_TOGGLE_FOLDRECURSIVE},
		{"IDM_EXPAND_ENSURECHILDRENVISIBLE",	IDM_EXPAND_ENSURECHILDRENVISIBLE},
		{"IDM_UPRCASE",	IDM_UPRCASE},
		{"IDM_LWRCASE",	IDM_LWRCASE},
		{"IDM_ABBREV",	IDM_ABBREV},
		{"IDM_BLOCK_COMMENT",	IDM_BLOCK_COMMENT},
		{"IDM_STREAM_COMMENT",	IDM_STREAM_COMMENT},
		{"IDM_COPYASRTF",	IDM_COPYASRTF},
		{"IDM_BOX_COMMENT",	IDM_BOX_COMMENT},
		{"IDM_INS_ABBREV",	IDM_INS_ABBREV},
		{"IDM_JOIN",	IDM_JOIN},
		{"IDM_SPLIT",	IDM_SPLIT},
		{"IDM_INCSEARCH",	IDM_INCSEARCH},
		{"IDM_ENTERSELECTION",	IDM_ENTERSELECTION},
		{"IDM_PREVMATCHPPC",	IDM_PREVMATCHPPC},
		{"IDM_SELECTTOPREVMATCHPPC",	IDM_SELECTTOPREVMATCHPPC},
		{"IDM_NEXTMATCHPPC",	IDM_NEXTMATCHPPC},
		{"IDM_SELECTTONEXTMATCHPPC",	IDM_SELECTTONEXTMATCHPPC},
		{"IDM_COMPILE",	IDM_COMPILE},

#ifdef GLI_CHANGES

		{"IDM_TOGGLEBREAKPOINT",	IDM_TOGGLEBREAKPOINT},

#endif //GLI_CHANGES

		{"IDM_BUILD",	IDM_BUILD},
		{"IDM_GO",	IDM_GO},
		{"IDM_STOPEXECUTE",	IDM_STOPEXECUTE},
		{"IDM_FINISHEDEXECUTE",	IDM_FINISHEDEXECUTE},
		{"IDM_NEXTMSG",	IDM_NEXTMSG},
		{"IDM_PREVMSG",	IDM_PREVMSG},
		{"IDM_MACRO_SEP",	IDM_MACRO_SEP},
		{"IDM_MACRORECORD",	IDM_MACRORECORD},
		{"IDM_MACROSTOPRECORD",	IDM_MACROSTOPRECORD},
		{"IDM_MACROPLAY",	IDM_MACROPLAY},
		{"IDM_MACROLIST",	IDM_MACROLIST},
		{"IDM_ACTIVATE",	IDM_ACTIVATE},
		{"IDM_SRCWIN",	IDM_SRCWIN},
		{"IDM_RUNWIN",	IDM_RUNWIN},
		{"IDM_TOOLWIN",	IDM_TOOLWIN},
		{"IDM_STATUSWIN",	IDM_STATUSWIN},
		{"IDM_TABWIN",	IDM_TABWIN},
		{"IDM_SPLITVERTICAL",	IDM_SPLITVERTICAL},
		{"IDM_VIEWSPACE",	IDM_VIEWSPACE},
		{"IDM_VIEWEOL",	IDM_VIEWEOL},
		{"IDM_VIEWGUIDES",	IDM_VIEWGUIDES},
		{"IDM_SELMARGIN",	IDM_SELMARGIN},
		{"IDM_FOLDMARGIN",	IDM_FOLDMARGIN},
		{"IDM_LINENUMBERMARGIN",	IDM_LINENUMBERMARGIN},
		{"IDM_VIEWTOOLBAR",	IDM_VIEWTOOLBAR},
		{"IDM_TOGGLEOUTPUT",	IDM_TOGGLEOUTPUT},
		{"IDM_VIEWTABBAR",	IDM_VIEWTABBAR},
		{"IDM_VIEWSTATUSBAR",	IDM_VIEWSTATUSBAR},
		{"IDM_TOGGLEPARAMETERS",	IDM_TOGGLEPARAMETERS},
		{"IDM_OPENFILESHERE",	IDM_OPENFILESHERE},
		{"IDM_WRAP",	IDM_WRAP},
		{"IDM_WRAPOUTPUT",	IDM_WRAPOUTPUT},
		{"IDM_READONLY",	IDM_READONLY},
		{"IDM_CLEAROUTPUT",	IDM_CLEAROUTPUT},
		{"IDM_SWITCHPANE",	IDM_SWITCHPANE},
		{"IDM_EOL_CRLF",	IDM_EOL_CRLF},
		{"IDM_EOL_CR",	IDM_EOL_CR},
		{"IDM_EOL_LF",	IDM_EOL_LF},
		{"IDM_EOL_CONVERT",	IDM_EOL_CONVERT},
		{"IDM_TABSIZE",	IDM_TABSIZE},
		{"IDM_MONOFONT",	IDM_MONOFONT},
		{"IDM_OPENLOCALPROPERTIES",	IDM_OPENLOCALPROPERTIES},
		{"IDM_OPENUSERPROPERTIES",	IDM_OPENUSERPROPERTIES},
		{"IDM_OPENGLOBALPROPERTIES",	IDM_OPENGLOBALPROPERTIES},
		{"IDM_OPENABBREVPROPERTIES",	IDM_OPENABBREVPROPERTIES},
		{"IDM_OPENLUAEXTERNALFILE",	IDM_OPENLUAEXTERNALFILE},
		{"IDM_PREVFILE",	IDM_PREVFILE},
		{"IDM_NEXTFILE",	IDM_NEXTFILE},
		{"IDM_CLOSEALL",	IDM_CLOSEALL},
		{"IDM_SAVEALL",	IDM_SAVEALL},
		{"IDM_BUFFERSEP",	IDM_BUFFERSEP},
		{"IDM_HELP",	IDM_HELP},
		{"IDM_ABOUT",	IDM_ABOUT},
		{"IDM_HELP_SCITE",	IDM_HELP_SCITE},
		{"IDM_ONTOP",	IDM_ONTOP},
		{"IDM_FULLSCREEN",	IDM_FULLSCREEN},

//--Autogenerated -- end of automatically generated section
		{0, 0},
	};

	for (int i=0; idmTable[i].id; i++) {
		if (commandName == idmTable[i].name) {
			return idmTable[i].id;
		}
	}
	// Otherwise we might have entered a number as command to access a "SCI_" command
	return commandName.value();
}
