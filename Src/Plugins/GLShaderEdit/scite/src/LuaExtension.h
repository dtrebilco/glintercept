// SciTE - Scintilla based Text Editor
// LuaExtension.h - Lua scripting extension
// Copyright 1998-2000 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

class LuaExtension : public Extension {
private:
    LuaExtension(); // Singleton
    LuaExtension(const LuaExtension &);   // Disable copy ctor
    void operator=(const LuaExtension &); // Disable operator=

public:
    static LuaExtension &Instance();

    virtual ~LuaExtension();

    virtual bool Initialise(ExtensionAPI *host_);
    virtual bool Finalise();
    virtual bool Clear();
    virtual bool Load(const char *filename);

    virtual bool InitBuffer(int);
    virtual bool ActivateBuffer(int);
    virtual bool RemoveBuffer(int);

    virtual bool OnOpen(const char *filename);
    virtual bool OnSwitchFile(const char *filename);
    virtual bool OnBeforeSave(const char *filename);
    virtual bool OnSave(const char *filename);
    virtual bool OnChar(char ch);
    virtual bool OnExecute(const char *s);
    virtual bool OnSavePointReached();
    virtual bool OnSavePointLeft();
    //virtual bool OnStyle(unsigned int startPos, int lengthDoc, int initStyle, Accessor *styler);
    virtual bool OnDoubleClick();
    virtual bool OnUpdateUI();
    virtual bool OnMarginClick();
    virtual bool OnUserListSelection(int listType, const char *selection);
};
