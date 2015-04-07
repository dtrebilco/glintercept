// SciTE - Scintilla based Text Editor
/** @file IFaceTable.h
 ** SciTE iface function and constant descriptors.
 **/
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef IFACETABLE_H
#define IFACETABLE_H

enum IFaceType {
	iface_void,
	iface_int,
	iface_length,
	iface_position,
	iface_colour,
	iface_bool,
	iface_keymod,
	iface_string,
	iface_stringresult,
	iface_cells,
	iface_textrange,
	iface_findtext,
	iface_formatrange
};	

struct IFaceConstant {
	const char *name;
	int value;
};

struct IFaceFunction {
	const char *name;
	int value;
	IFaceType returnType;
	IFaceType paramType[2];
};

struct IFaceProperty {
	const char *name;
	int getter;
	int setter;
	IFaceType valueType;
	IFaceType paramType;
	
	IFaceFunction GetterFunction() const {
		IFaceFunction result = {"(property getter)",getter,valueType,{paramType,iface_void}};
		return result;
	}
	
	IFaceFunction SetterFunction() const {
		IFaceFunction result = {"(property setter)",setter,iface_void,{valueType, iface_void}};
		if (paramType != iface_void) {
			result.paramType[0] = paramType;
			result.paramType[1] = valueType;
		}
		return result;
	}
};

struct IFaceObject {
	const char *name;
	const char *prefix;
	IFaceType indexType;
};

class IFaceTable {
public:
	static const IFaceFunction * const functions;
	static const IFaceConstant * const constants;
	static const IFaceProperty * const properties;

	static const int functionCount;
	static const int constantCount;
	static const int propertyCount;

	static int FindConstant(const char *name);
	static int FindFunction(const char *name);
	static int FindFunctionByConstantName(const char *name);
	static int FindProperty(const char *name);

	static int GetConstantName(int value, char *nameOut, unsigned nameBufferLen);
};

#endif
