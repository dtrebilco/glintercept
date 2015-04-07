///////////////////////////////////////////////////////////////////////////////
// File:        setup0.h
// Purpose:     wxSTEditor setup definitions (rename to setup.h)
// Maintainer:  
// Created:     2003-04-04
// RCS-ID:      
// Copyright:   (c) John Labenski
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

// This file is distributed as setup0.h and should be copied to setup.h
// You may modify the setup.h to control the behavior of the wxSTEditor.

#ifndef _STESETUP_H_
#define _STESETUP_H_

//Include GLI changes
#define GLI_CHANGES

// STE_USE_LANG_XXX determines whether or not the language information will be 
// compiled in or not. Note that the wxSTEditorLangs always has the 
// languages in the order given by enum STE_LangTypes STE_LANG_XXX 
// (wxSTC_LEX_XXX), however when the STE_USE_LANG_XXX is 0 the language struct
// is NULL and the language will not be shown in the preference dialog. 
// Turning off unused languages serves two purposes, making a smaller binary 
// and a simplier interface by stripping out esoteric languages. 
// In order to make it easy to exclude languages be sure to use 
// wxSTEditorLangs::HasLanguage(lang_n) before accessing any values.

#define STE_USE_LANG_CONTAINER   0 // 0  probably never want this shown 
#define STE_USE_LANG_NULL        1 // 1
#define STE_USE_LANG_PYTHON      1 // 2
#define STE_USE_LANG_CPP         1 // 3
#define STE_USE_LANG_HTML        0 // 4
#define STE_USE_LANG_XML         0 // 5
#define STE_USE_LANG_PERL        0 // 6
#define STE_USE_LANG_SQL         0 // 7
#define STE_USE_LANG_VB          0 // 8
#define STE_USE_LANG_PROPERTIES  0 // 9
#define STE_USE_LANG_ERRORLIST   0 // 10
#define STE_USE_LANG_MAKEFILE    0 // 11
#define STE_USE_LANG_BATCH       0 // 12
#define STE_USE_LANG_XCODE       0 // 13
#define STE_USE_LANG_LATEX       0 // 14
#define STE_USE_LANG_LUA         0 // 15
#define STE_USE_LANG_DIFF        0 // 16
#define STE_USE_LANG_CONF        0 // 17
#define STE_USE_LANG_PASCAL      0 // 18
#define STE_USE_LANG_AVE         0 // 19
#define STE_USE_LANG_ADA         0 // 20
#define STE_USE_LANG_LISP        0 // 21
#define STE_USE_LANG_RUBY        0 // 22
#define STE_USE_LANG_EIFFEL      0 // 23
#define STE_USE_LANG_EIFFELKW    0 // 24
#define STE_USE_LANG_TCL         0 // 25
#define STE_USE_LANG_NNCRONTAB   0 // 26
#define STE_USE_LANG_BULLANT     0 // 27
#define STE_USE_LANG_VBSCRIPT    0 // 28
#define STE_USE_LANG_ASP         0 // 29
#define STE_USE_LANG_PHP         0 // 30
#define STE_USE_LANG_BAAN        0 // 31
#define STE_USE_LANG_MATLAB      0 // 32
#define STE_USE_LANG_SCRIPTOL    0 // 33
#define STE_USE_LANG_ASM         0 // 34
#define STE_USE_LANG_CPPNOCASE   0 // 35
#define STE_USE_LANG_FORTRAN     0 // 36
#define STE_USE_LANG_F77         0 // 37
#define STE_USE_LANG_CSS         0 // 38
#define STE_USE_LANG_POV         0 // 39
#define STE_USE_LANG_LOUT        0 // 40
#define STE_USE_LANG_ESCRIPT     0 // 41
#define STE_USE_LANG_PS          0 // 42
#define STE_USE_LANG_NSIS        0 // 43
#define STE_USE_LANG_MMIXAL      0 // 44
#define STE_USE_LANG_CLW         0 // 45
#define STE_USE_LANG_CLWNOCASE   0 // 46
#define STE_USE_LANG_LOT         0 // 47
#define STE_USE_LANG_YAML        0 // 48
#define STE_USE_LANG_TEX         0 // 49
#define STE_USE_LANG_METAPOST    0 // 50
#define STE_USE_LANG_POWERBASIC  0 // 51
#define STE_USE_LANG_FORTH       0 // 52
#define STE_USE_LANG_ERLANG      0 // 53
#define STE_USE_LANG_OCTAVE      0 // 54
#define STE_USE_LANG_MSSQL       0 // 55
#define STE_USE_LANG_VERILOG     0 // 56
#define STE_USE_LANG_KIX         0 // 57
#define STE_USE_LANG_GUI4CLI     0 // 58
#define STE_USE_LANG_SPECMAN     0 // 59
#define STE_USE_LANG_AU3         0 // 60
#define STE_USE_LANG_APDL        0 // 61
#define STE_USE_LANG_BASH        0 // 62
#define STE_USE_LANG_ASN1        0 // 63
#define STE_USE_LANG_VHDL        0 // 64

// Derived languages 
#define STE_USE_LANG_JAVA        0 // 65
#define STE_USE_LANG_JAVASCRIPT  0 // 66
#define STE_USE_LANG_BASH        0 // 67
#define STE_USE_LANG_RC          0 // 68
#define STE_USE_LANG_CS          0 // 69
#define STE_USE_LANG_IDL         0 // 70
#define STE_USE_LANG_PLSQL       0 // 71

#ifdef GLI_CHANGES

#define STE_USE_LANG_GLSL        1 // 72
#define STE_USE_LANG_GLASM       1 // 73

#endif //GLI_CHANGES



#endif // _STESETUP_H_
