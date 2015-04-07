// SciTE - Scintilla based Text Editor
/** @file FilePath.h
 ** Definition of platform independent base class of editor.
 **/
// Copyright 1998-2005 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

extern const char pathSepString[];
extern const char pathSepChar;
extern const char listSepString[];
extern const char configFileVisibilityString[];
extern const char fileRead[];
extern const char fileWrite[];

#ifdef unix
#include <limits.h>
#ifdef PATH_MAX
#define MAX_PATH PATH_MAX
#else
#define MAX_PATH 260
#endif
#endif

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

class FilePath;

class FilePathSet;

class FilePath {
	SString fileName;
public:
	FilePath(const char *fileName_="");
	FilePath(FilePath const &directory, FilePath const &name);
	void Set(const char *fileName_);
	const char *AsFileSystem() const;
#ifdef __vms
	FilePath VMSToUnixStyle();
#endif
	void Set(FilePath const &other);
	void Set(FilePath const &directory, FilePath const &name);
	void SetDirectory(FilePath directory);
	void Init();
	bool SameNameAs(const char *other) const;
	bool SameNameAs(const FilePath &other) const;
	bool IsSet() const;
	bool IsUntitled() const;
	bool IsAbsolute() const;
	bool IsRoot() const;
	static int RootLength();
	const char *AsInternal() const;
	FilePath Name() const;
	FilePath BaseName() const;
	FilePath Extension() const;
	FilePath Directory() const;
	void FixName();
	FilePath AbsolutePath() const;
	FilePath NormalizePath() const;
	static FilePath GetWorkingDirectory();
	void SetWorkingDirectory() const;
	void FixCase();
	void List(FilePathSet &directories, FilePathSet &files);
	FILE *Open(const char *mode) const;
	void Remove() const;
	time_t ModifiedTime() const;
	int GetFileLength() const;
	bool Exists() const;
	bool IsDirectory() const;
	bool Matches(const char *pattern) const;
};

class FilePathSet {
	size_t size;
	size_t lengthBody;
	FilePath *body;
	// Private so won't be called.
	FilePathSet &operator=(const FilePathSet &);
public:
	FilePathSet(int size_=10);
	FilePathSet(const FilePathSet &other);
	~FilePathSet();
	FilePath At(size_t pos) const;
	void Append(FilePath fp);
	size_t Length() const;
};
