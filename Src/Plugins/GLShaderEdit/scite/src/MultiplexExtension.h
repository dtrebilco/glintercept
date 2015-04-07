// SciTE - Scintilla based Text Editor
/** @file MultiplexExtension.h
 ** Extension that manages / dispatches messages to multiple extensions.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef MULTIPLEXEXTENSION_H
#define MULTIPLEXEXTENSION_H

#include "Extender.h"

// MultiplexExtension manages multiple Extension objects, similar to
// what is proposed in the SciTE Extension documentation.  Each 
// message is sent to each contained extension object in turn until
// one indicates that the message has been handled and does not need
// to be processed further.  Certain messages (Initialise, Finalise
// Clear, and SendProperty) are sent to all contained extensions
// regardless of return code.
//
// The Director extension incorrectly returns true for all messages,
// meaning that other extensions will never see the message if 
// DirectorExtension comes before them in the list.  This has been 
// fixed at source.
//
// Extensions are added to the multiplexer by calling RegisterExtension.
// The extensions are prioritized with the first one added having the
// highest priority.  If more flexibility is needed in order to support
// dynamic discovery of extensions and assignment of priority, that will
// be added later.  If the ability to remove extensions becomes important,
// that can be added as well (later).
//
// The multiplexer does not manage the lifetime of the extension objects
// that are registered with it.  If that functionality later turns out 
// to be needed, it will be added at that time.  (Broken record?  Do the
// simplest thing...)  However, the option to "not" manage the lifecycle
// is a valid one, since it often makes sense to implement extensions as
// singletons.

class MultiplexExtension: public Extension {
public:
	MultiplexExtension();
	virtual ~MultiplexExtension();

	bool RegisterExtension(Extension &ext_);

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

private:
	Extension **extensions;
	int extensionCount;
	ExtensionAPI *host;

	// Copying is unsupported.
	MultiplexExtension(const MultiplexExtension & copy);
	MultiplexExtension & operator=(const MultiplexExtension & copy);
};

#endif
