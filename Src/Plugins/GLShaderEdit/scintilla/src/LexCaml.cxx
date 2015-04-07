// Scintilla source code edit control
/** @file LexCaml.cxx
 ** Lexer for Objective Caml.
 **/
// Copyright 2005 by Robert Roessler <robertr@rftp.com>
// The License.txt file describes the conditions under which this software may be distributed.
/*	Release History
	20050204 Initial release.
	20050205 Quick compiler standards/"cleanliness" adjustment.
	20050206 Added cast for IsLeadByte().
	20050209 Changes to "external" build support.
	20050306 Fix for 1st-char-in-doc "corner" case.
	20050502 Fix for [harmless] one-past-the-end coloring.
	20050515 Refined numeric token recognition logic.
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"

//	Since the Microsoft __iscsym[f] funcs are not ANSI...
inline int  iscaml(int c) {return isalnum(c) || c == '_';}
inline int iscamlf(int c) {return isalpha(c) || c == '_';}
inline int iscamld(int c) {return isdigit(c) || c == '_';}

#ifdef BUILD_AS_EXTERNAL_LEXER
/*
	(actually seems to work!)
*/
#include "WindowAccessor.h"
#include "ExternalLexer.h"

#if PLAT_WIN
#include <windows.h>
#endif

static void ColouriseCamlDoc(
	unsigned int startPos, int length,
	int initStyle,
	WordList *keywordlists[],
	Accessor &styler);

static void FoldCamlDoc(
	unsigned int startPos, int length,
	int initStyle,
	WordList *keywordlists[],
	Accessor &styler);

static void InternalLexOrFold(int lexOrFold, unsigned int startPos, int length,
	int initStyle, char *words[], WindowID window, char *props);

static const char* LexerName = "caml";

#ifdef TRACE
void Platform::DebugPrintf(const char *format, ...) {
	char buffer[2000];
	va_list pArguments;
	va_start(pArguments, format);
	vsprintf(buffer,format,pArguments);
	va_end(pArguments);
	Platform::DebugDisplay(buffer);
}
#else
void Platform::DebugPrintf(const char *, ...) {
}
#endif

bool Platform::IsDBCSLeadByte(int codePage, char ch) {
	return ::IsDBCSLeadByteEx(codePage, ch) != 0;
}

long Platform::SendScintilla(WindowID w, unsigned int msg, unsigned long wParam, long lParam) {
	return ::SendMessage(reinterpret_cast<HWND>(w), msg, wParam, lParam);
}

long Platform::SendScintillaPointer(WindowID w, unsigned int msg, unsigned long wParam, void *lParam) {
	return ::SendMessage(reinterpret_cast<HWND>(w), msg, wParam,
		reinterpret_cast<LPARAM>(lParam));
}

void EXT_LEXER_DECL Fold(unsigned int lexer, unsigned int startPos, int length,
	int initStyle, char *words[], WindowID window, char *props)
{
	// below useless evaluation(s) to supress "not used" warnings
	lexer;
	// build expected data structures and do the Fold
	InternalLexOrFold(1, startPos, length, initStyle, words, window, props);

}

int EXT_LEXER_DECL GetLexerCount()
{
	return 1;	// just us [Objective] Caml lexers here!
}

void EXT_LEXER_DECL GetLexerName(unsigned int Index, char *name, int buflength)
{
	// below useless evaluation(s) to supress "not used" warnings
	Index;
	// return as much of our lexer name as will fit (what's up with Index?)
	if (buflength > 0) {
		buflength--;
		int n = strlen(LexerName);
		if (n > buflength)
			n = buflength;
		memcpy(name, LexerName, n), name[n] = '\0';
	}
}

void EXT_LEXER_DECL Lex(unsigned int lexer, unsigned int startPos, int length,
	int initStyle, char *words[], WindowID window, char *props)
{
	// below useless evaluation(s) to supress "not used" warnings
	lexer;
	// build expected data structures and do the Lex
	InternalLexOrFold(0, startPos, length, initStyle, words, window, props);
}

static void InternalLexOrFold(int foldOrLex, unsigned int startPos, int length,
	int initStyle, char *words[], WindowID window, char *props)
{
	// create and initialize a WindowAccessor (including contained PropSet)
	PropSet ps;
	ps.SetMultiple(props);
	WindowAccessor wa(window, ps);
	// create and initialize WordList(s)
	int nWL = 0;
	for (; words[nWL]; nWL++) ;	// count # of WordList PTRs needed
	WordList** wl = new WordList* [nWL + 1];// alloc WordList PTRs
	int i = 0;
	for (; i < nWL; i++) {
		wl[i] = new WordList();	// (works or THROWS bad_alloc EXCEPTION)
		wl[i]->Set(words[i]);
	}
	wl[i] = 0;
	// call our "internal" folder/lexer (... then do Flush!)
	if (foldOrLex)
		FoldCamlDoc(startPos, length, initStyle, wl, wa);
	else
		ColouriseCamlDoc(startPos, length, initStyle, wl, wa);
	wa.Flush();
	// clean up before leaving
	for (i = nWL - 1; i >= 0; i--)
		delete wl[i];
	delete [] wl;
}

static
#endif	/* BUILD_AS_EXTERNAL_LEXER */

void ColouriseCamlDoc(
	unsigned int startPos, int length,
	int initStyle,
	WordList *keywordlists[],
	Accessor &styler)
{
	// initialize styler
	styler.StartAt(startPos);
	styler.StartSegment(startPos);
	// set up [initial] state info (terminating states that shouldn't "bleed")
	int state = initStyle, nesting = 0;
	if (state < SCE_CAML_STRING)
		state = SCE_CAML_DEFAULT;
	if (state >= SCE_CAML_COMMENT)
		nesting = state - SCE_CAML_COMMENT;
	int chLast = startPos? static_cast<unsigned char>(styler[startPos - 1]): ' ';
	int chNext = static_cast<unsigned char>(styler[startPos]);

	int chBase = 'd', chToken = 0, chLit = 0, chSkip;
	WordList& keywords = *keywordlists[0];
	WordList& keywords2 = *keywordlists[1];

	// foreach char in range...
	unsigned int i = startPos;
	const unsigned int endPos = startPos + length;
	for (; i < endPos; i += chSkip) {
		// set up [per-char] state info
		int ch = chNext;
		chNext = static_cast<unsigned char>(styler.SafeGetCharAt(i + 1));
		int state2 = -1;	// (ASSUME no state change)
		int chColor = i - 1;// (ASSUME standard coloring range)
		chSkip = 1;			// (ASSUME scanner "eats" 1 char)

		// this may be the correct thing to do... or not
		if (styler.IsLeadByte(static_cast<char>(ch))) {
			chNext = static_cast<unsigned char>(styler.SafeGetCharAt(i + 2)),
				chSkip++;
			continue;
		}

		// step state machine
		switch (state) {
		case SCE_CAML_DEFAULT:
			// it's wide open; what do we have?
			if (iscamlf(ch))
				state2 = SCE_CAML_IDENTIFIER, chToken = i;
			else if (ch == '`')
				state2 = SCE_CAML_TAGNAME, chToken = i;
			else if (ch == '#' && isdigit(chNext))
				state2 = SCE_CAML_LINENUM, chToken = i;
			else if (isdigit(ch)) {
				state2 = SCE_CAML_NUMBER,
					chBase = strchr("xXoObB", chNext)? chNext: 'd';
				if (chBase != 'd')
					ch = chNext,
						chNext = static_cast<unsigned char>(styler.SafeGetCharAt(i + 2)),
						chSkip++;
			} else if (ch == '\'')	/* (char literal?) */
				state2 = SCE_CAML_CHAR, chToken = i, chLit = 0;
			else if (ch == '\"')
				state2 = SCE_CAML_STRING;
			else if (ch == '(' && chNext == '*')
				state2 = SCE_CAML_COMMENT,
					ch = ' ',	// (make SURE "(*)" isn't seen as a closed comment)
					chNext = static_cast<unsigned char>(styler.SafeGetCharAt(i + 2)),
					chSkip++, nesting = 0;
			else if (strchr("!?~"		/* Caml "prefix-symbol" */
					"=<>@^|&+-*/$%"		/* Caml "infix-symbol" */
					"()[]{};,:.#", ch))	/* Caml "bracket" or ;,:.# */
				state2 = SCE_CAML_OPERATOR, chToken = i;
			break;

		case SCE_CAML_IDENTIFIER:
			// [try to] interpret as [additional] identifier char
			if (!(iscaml(ch) || ch == '\'')) {
				const int n = i - chToken;
				if (n < 24) {
					// length is believable as keyword, [re-]construct token
					char t[24];
					int p = 0;
					for (int q = chToken; p < n; p++, q++)
						t[p] = styler[q];
					t[p] = '\0';
					// special-case "_" token as KEYWORD
					if ((n == 1 && chLast == '_') || keywords.InList(t))
						state = SCE_CAML_KEYWORD;
					else if (keywords2.InList(t))
						state = SCE_CAML_KEYWORD2;
				}
				state2 = SCE_CAML_DEFAULT, chNext = ch, chSkip--;
			}
			break;

		case SCE_CAML_TAGNAME:
			// [try to] interpret as [additional] tagname char
			if (!(iscaml(ch) || ch == '\''))
				state2 = SCE_CAML_DEFAULT, chNext = ch, chSkip--;
			break;

		/*case SCE_CAML_KEYWORD:
		case SCE_CAML_KEYWORD2:
			// [try to] interpret as [additional] keyword char
			if (!iscaml(ch))
				state2 = SCE_CAML_DEFAULT, chNext = ch, chSkip--;
			break;*/

		case SCE_CAML_LINENUM:
			// [try to] interpret as [additional] linenum directive char
			if (!isdigit(ch))
				state2 = SCE_CAML_DEFAULT, chNext = ch, chSkip--;
			break;

		case SCE_CAML_OPERATOR: {
			// [try to] interpret as [additional] operator char
			const char* o = 0;
			if (iscaml(ch) || isspace(ch)			/* ident or whitespace */
				|| ((o = strchr(")]};,\'\"`#", ch)) != 0)/* "termination" chars */
				|| !strchr("!$%&*+-./:<=>?@^|~", ch)/* "operator" chars */) {
				// check for INCLUSIVE termination
				if (o && strchr(")]};,", ch)) {
					if ((ch == ')' && chLast == '(') || (ch == ']' && chLast == '['))
						// special-case "()" and "[]" tokens as KEYWORDS
						state = SCE_CAML_KEYWORD;
					chColor++;
				} else
					chNext = ch, chSkip--;
				state2 = SCE_CAML_DEFAULT;
			}
			break;
		}

		case SCE_CAML_NUMBER:
			// [try to] interpret as [additional] numeric literal char
			// N.B. - improperly accepts "extra" digits in base 2 or 8 literals
			if (iscamld(ch) || ((chBase == 'x' || chBase == 'X') && isxdigit(ch)))
				break;
			// how about an integer suffix?
			if ((ch == 'l' || ch == 'L' || ch == 'n')&& (iscamld(chLast)
				|| ((chBase == 'x' || chBase == 'X') && isxdigit(chLast))))
				break;
			// or a floating-point literal?
			if (chBase == 'd') {
				// with a decimal point?
				if (ch == '.' && iscamld(chLast))
					break;
				// with an exponent? (I)
				if ((ch == 'e' || ch == 'E') && (iscamld(chLast) || chLast == '.'))
					break;
				// with an exponent? (II)
				if ((ch == '+' || ch == '-') && (chLast == 'e' || chLast == 'E'))
					break;
			}
			// it looks like we have run out of number
			state2 = SCE_CAML_DEFAULT, chNext = ch, chSkip--;
			break;

		case SCE_CAML_CHAR:
			// [try to] interpret as [additional] char literal char
			if (ch == '\\') {
				chLit = 1;	// (definitely IS a char literal)
				if (chLast == '\\')
					ch = ' ';	// (so termination test isn't fooled)
			// should we be terminating - one way or another?
			} else if ((ch == '\'' && chLast != '\\') || ch == '\r' || ch == '\n') {
				state2 = SCE_CAML_DEFAULT;
				if (ch == '\'')
					chColor++;
				else
					state = SCE_CAML_IDENTIFIER;
			// ... maybe a char literal, maybe not
			} else if (chLit < 1 && i - chToken >= 2)
				state = SCE_CAML_IDENTIFIER, chNext = ch, chSkip--;
			break;

		case SCE_CAML_STRING:
			// [try to] interpret as [additional] string literal char
			if (ch == '\\' && chLast == '\\')
				ch = ' ';	// (so '\\' doesn't cause us trouble)
			else if (ch == '\"' && chLast != '\\')
				state2 = SCE_CAML_DEFAULT, chColor++;
			break;

		case SCE_CAML_COMMENT:
		case SCE_CAML_COMMENT+1:
		case SCE_CAML_COMMENT+2:
		case SCE_CAML_COMMENT+3:
			// we're IN a comment - does this start a NESTED comment?
			if (ch == '(' && chNext == '*')
				state2 = state + 1,
					ch = ' ',	// (make SURE "(*)" isn't seen as a closed comment)
					chNext = static_cast<unsigned char>(styler.SafeGetCharAt(i + 2)),
					chSkip++, nesting++;
			// [try to] interpret as [additional] comment char
			else if (ch == ')' && chLast == '*')
				state2 = nesting? (state - 1): SCE_CAML_DEFAULT, chColor++, nesting--;
			break;
		}

		// handle state change and char coloring as required
		if (state2 >= 0) {
			// (1st char will NOT be colored until AT LEAST 2nd char)
			if (chColor >= 0)
				styler.ColourTo(chColor, state);
			state = state2;
		}
		chLast = ch;
	}

	// do any required terminal char coloring (JIC)
	if (i >= endPos)
		i = endPos - 1;
	styler.ColourTo(i, state);
//	styler.Flush();	// (is this always done by calling code?)
}

#ifdef BUILD_AS_EXTERNAL_LEXER
static
#endif	/* BUILD_AS_EXTERNAL_LEXER */
void FoldCamlDoc(
	unsigned int startPos, int length,
	int initStyle,
	WordList *keywordlists[],
	Accessor &styler)
{
	// below useless evaluation(s) to supress "not used" warnings
	startPos || length || initStyle || keywordlists[0] || styler.Length();
}

static const char * const camlWordListDesc[] = {
	"Keywords",		// primary Objective Caml keywords
	"Keywords2",	// "optional" keywords (typically from Pervasives)
	0
};

#ifndef BUILD_AS_EXTERNAL_LEXER
LexerModule lmCaml(SCLEX_CAML, ColouriseCamlDoc, "caml", FoldCamlDoc, camlWordListDesc);
#endif	/* BUILD_AS_EXTERNAL_LEXER */
