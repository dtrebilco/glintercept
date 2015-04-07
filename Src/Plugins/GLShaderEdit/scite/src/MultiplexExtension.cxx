// SciTE - Scintilla based Text Editor
/** @file MultiplexExtension.cxx
 ** Extension that manages / dispatches messages to multiple extensions.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include "MultiplexExtension.h"

MultiplexExtension::MultiplexExtension(): extensions(0), extensionCount(0), host(0) {}
	
MultiplexExtension::~MultiplexExtension() {
	Finalise();
	delete [] extensions;
}

bool MultiplexExtension::RegisterExtension(Extension &ext_) {
	for (int i=0; i < extensionCount; ++i)
		if (extensions[i] == &ext_) 
			return true;
	
	Extension **newExtensions = new Extension *[extensionCount+1];
		
	if (newExtensions) {
		if (extensions) {
			for (int i = 0; i < extensionCount; ++i)
				newExtensions[i] = extensions[i];
			delete[] extensions;
		}
		
		extensions = newExtensions;
		extensions[extensionCount++] = &ext_;
		
		if (host)
			ext_.Initialise(host);
		
		return true;
	} else {
		return false;
	}
}


// Initialise, Finalise, Clear, and SetProperty get broadcast to all extensions,
// regardless of return code.  This does not strictly match the documentation, but
// seems like the right thing to do.  The others methods stop processing once one
// Extension returns true.
//
// Load will eventually be changed to be smarter, so that each Extension can have 
// something different loaded into it.  OnExecute and OnMacro might also be made 
// smarter with a syntax to indicate to which extension the command should be sent.

bool MultiplexExtension::Initialise(ExtensionAPI *host_) {
	if (host)
                Finalise(); // shouldn't happen.

	host = host_;
	for (int i=0; i < extensionCount; ++i)
		extensions[i]->Initialise(host_);
	
	return false;
}

bool MultiplexExtension::Finalise() {
	if (host) {
                for (int i=extensionCount-1; i>=0; --i)
                        extensions[i]->Finalise();
                
                host = 0;
        }
	return false;
}

bool MultiplexExtension::Clear() {
	for (int i=0; i<extensionCount; ++i)
		extensions[i]->Clear();
	return false;
}

bool MultiplexExtension::Load(const char *filename) {
	bool handled = false;

	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->Load(filename))
                        handled = true;

	return handled;
}

bool MultiplexExtension::InitBuffer(int index) {
	for (int i=0; i<extensionCount; ++i)
		extensions[i]->InitBuffer(index);
	return false;
}

bool MultiplexExtension::ActivateBuffer(int index) {
	for (int i=0; i<extensionCount; ++i)
		extensions[i]->ActivateBuffer(index);
	return false;
}

bool MultiplexExtension::RemoveBuffer(int index) {
	for (int i=0; i<extensionCount; ++i)
		extensions[i]->RemoveBuffer(index);
	return false;
}

bool MultiplexExtension::OnOpen(const char *filename) {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
		if (extensions[i]->OnOpen(filename))
			handled = true;
	return handled;
}

bool MultiplexExtension::OnSwitchFile(const char *filename) {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
		if (extensions[i]->OnSwitchFile(filename))
			handled = true;
	return handled;
}

bool MultiplexExtension::OnBeforeSave(const char *filename) {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
		if (extensions[i]->OnBeforeSave(filename))
			handled = true;
	return handled;
}

bool MultiplexExtension::OnSave(const char *filename) {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->OnSave(filename))
                        handled = true;
	return handled;
}

bool MultiplexExtension::OnChar(char c) {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->OnChar(c))
                        handled = true;
	return handled;
}

bool MultiplexExtension::OnExecute(const char *cmd) {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->OnExecute(cmd))
                        handled = true;
	return handled;
}

bool MultiplexExtension::OnSavePointReached() {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->OnSavePointReached())
                        handled = true;
	return handled;
}

bool MultiplexExtension::OnSavePointLeft() {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->OnSavePointLeft())
                        handled = true;
	return handled;
}

bool MultiplexExtension::OnStyle(unsigned int p, int q, int r, Accessor *s) {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->OnStyle(p,q,r,s))
                        handled = true;
	return handled;
}

bool MultiplexExtension::OnDoubleClick() {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->OnDoubleClick())
                        handled = true;
	return handled;
}

bool MultiplexExtension::OnUpdateUI() {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->OnUpdateUI())
                        handled = true;
	return handled;
}

bool MultiplexExtension::OnMarginClick() {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->OnMarginClick())
                        handled = true;
	return handled;
}

bool MultiplexExtension::OnMacro(const char *p, const char *q) {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
                if (extensions[i]->OnMacro(p,q))
                        handled = true;
	return handled;
}

bool MultiplexExtension::OnUserListSelection(int listType, const char *selection) {
	bool handled = false;
	for (int i=0; i<extensionCount && !handled; ++i)
		if (extensions[i]->OnUserListSelection(listType, selection))
			handled = true;
	return handled;
}

bool MultiplexExtension::SendProperty(const char *prop) {
	for (int i=0; i<extensionCount; ++i)
		extensions[i]->SendProperty(prop);
	return false;
}
