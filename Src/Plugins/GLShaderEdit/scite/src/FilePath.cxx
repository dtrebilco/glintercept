// SciTE - Scintilla based Text Editor
/** @file FilePath.cxx
 ** Encapsulate a file path.
 **/
// Copyright 1998-2005 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#include "Platform.h"

#if PLAT_GTK

#include <unistd.h>
#include <gtk/gtk.h>

#include <dirent.h>
#include <errno.h>

#endif

#if PLAT_WIN

#include <io.h>

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

// For chdir
#ifdef _MSC_VER
#include <direct.h>
#endif
#ifdef __BORLANDC__
#include <dir.h>
#endif
#ifdef __DMC__
#include <dir.h>
#endif
#ifdef __MINGW32_VERSION
#include <direct.h>
#endif

#endif

#include "PropSet.h"
#include "FilePath.h"

#ifdef unix
const char pathSepString[] = "/";
const char pathSepChar = '/';
const char listSepString[] = ":";
const char configFileVisibilityString[] = ".";
const char fileRead[] = "rb";
const char fileWrite[] = "wb";
#endif
#ifdef __vms
const char pathSepString[] = "/";
const char pathSepChar = '/';
const char listSepString[] = ":";
const char configFileVisibilityString[] = "";
const char fileRead[] = "r";
const char fileWrite[] = "w";
#endif
#ifdef WIN32
// Windows
const char pathSepString[] = "\\";
const char pathSepChar = '\\';
const char listSepString[] = ";";
const char configFileVisibilityString[] = "";
const char fileRead[] = "rb";
const char fileWrite[] = "wb";
#endif

FilePath::FilePath(const char *fileName_) : fileName(fileName_) {
}

FilePath::FilePath(FilePath const &directory, FilePath const &name) {
	Set(directory, name);
}

void FilePath::Set(const char *fileName_) {
	fileName = fileName_;
}

const char *FilePath::AsFileSystem() const {
	return AsInternal();
}

void FilePath::Set(FilePath const &other) {
	fileName = other.fileName;
}

void FilePath::Set(FilePath const &directory, FilePath const &name) {
	fileName = directory.fileName;
	fileName.appendwithseparator(name.fileName.c_str(), pathSepChar);
}

void FilePath::SetDirectory(FilePath directory) {
	SString curName(fileName);
	fileName = directory.fileName;
	fileName.appendwithseparator(curName.c_str(), pathSepChar);
}

void FilePath::Init() {
	fileName = "";
}

bool FilePath::SameNameAs(const char *other) const {
#ifdef WIN32
	return EqualCaseInsensitive(fileName.c_str(), other);
#else
	return fileName == other;
#endif
}

bool FilePath::SameNameAs(const FilePath &other) const {
	return SameNameAs(other.fileName.c_str());
}

bool FilePath::IsSet() const { 
	return fileName.length() > 0; 
}

bool FilePath::IsUntitled() const {
	const char *dirEnd = strrchr(AsInternal(), pathSepChar);
	return !dirEnd || !dirEnd[1];
}

bool FilePath::IsAbsolute() const {
	if (fileName.length() == 0)
		return false;
#ifdef unix
	if (fileName[0] == '/')
		return true;
#endif
#ifdef __vms
	if (fileName[0] == '/')
		return true;
#endif
#ifdef WIN32
	if (fileName[0] == '\\' || fileName[1] == ':')	// UNC path or drive separator
		return true;
#endif

	return false;
}

bool FilePath::IsRoot() const {
#ifdef WIN32
	return (fileName.length() == 3) && (fileName[1] == ':') && (fileName[2] == '\\');
#else
	return fileName == "/";
#endif
}

int FilePath::RootLength() {
#ifdef WIN32
	return 3;
#else
	return 1;
#endif
}

const char *FilePath::AsInternal() const {
	return fileName.c_str();
}

FilePath FilePath::Name() const {
	const char *dirEnd = strrchr(fileName.c_str(), pathSepChar);
	if (dirEnd)
		return dirEnd + 1;
	else
		return fileName.c_str();
}

FilePath FilePath::BaseName() const {
	const char *dirEnd = strrchr(fileName.c_str(), pathSepChar);
	const char *extStart = strrchr(fileName.c_str(), '.');
	if (dirEnd) { 
		if (extStart) {
			return FilePath(SString(dirEnd + 1, 0, extStart - dirEnd - 1).c_str());
		} else {
			return FilePath(dirEnd + 1);
		}
	} else if (extStart) {
		return FilePath(SString(fileName.c_str(), 0, extStart - fileName.c_str()).c_str());
	} else {
		return fileName.c_str();
	}
}

FilePath FilePath::Extension() const {
	const char *extStart = strrchr(fileName.c_str(), '.');
	if (extStart)
		return extStart + 1;
	else
		return "";
}

FilePath FilePath::Directory() const {
	if (IsRoot()) {
		return FilePath(fileName.c_str());
	} else {
		const char *dirEnd = strrchr(fileName.c_str(), pathSepChar);
		if (dirEnd) {
			int lenDirectory = dirEnd - fileName.c_str();
			if (lenDirectory < RootLength()) {
				lenDirectory = RootLength();
			}
			return FilePath(fileName.substr(0, lenDirectory).c_str());
		} else {
			return FilePath();
		}
	}
}

static char *split(char*& s, char c) {
	char *t = s;
	if (s && (s = strchr(s, c)) != NULL)
		* s++ = '\0';
	return t;
}

FilePath FilePath::NormalizePath() const {
	char *path = new char[fileName.length() + 1];
	strcpy(path, AsInternal());
#ifdef WIN32
	// Convert unix path separators to Windows
	for (char *cp = path; *cp; cp++) {
		if (*cp == '/')
			*cp = pathSepChar;
	}
#endif
	char *absPath = new char[fileName.length() + 1];
	char *cur = absPath;
	*cur = '\0';
	char *tmp = path;
	if (*tmp == pathSepChar) {
		*cur++ = pathSepChar;
		*cur = '\0';
		tmp++;
	}
	char *part;
	while ((part = split(tmp, pathSepChar)) != NULL) {
		char *last;
		if (strcmp(part, ".") == 0)
			;
		else if (strcmp(part, "..") == 0 && (last = strrchr(absPath, pathSepChar)) != NULL) {
			if (last > absPath)
				cur = last;
			else
				cur = last + 1;
			*cur = '\0';
		} else {
			if (cur > absPath && *(cur - 1) != pathSepChar)
				*cur++ = pathSepChar;
			strcpy(cur, part);
			cur += strlen(part);
		}
	}
	FilePath ret(absPath);
	delete []path;
	delete []absPath;
	return ret;
}

#ifdef __vms

FilePath FilePath::VMSToUnixStyle() {
	// possible formats:
	// o disk:[dir.dir]file.type
	// o logical:file.type
	// o [dir.dir]file.type
	// o file.type
	// o /disk//dir/dir/file.type
	// o /disk/dir/dir/file.type

	char unixStyleFileName[MAX_PATH + 20];
	const char *vmsName = FullPath();

	if (strchr(vmsName, ':') == NULL && strchr(vmsName, '[') == NULL) {
		// o file.type
		// o /disk//dir/dir/file.type
		// o /disk/dir/dir/file.type
		if (strstr (vmsName, "//") == NULL) {
			return FilePath(vmsName);
		}
		strcpy(unixStyleFileName, vmsName);
		char *p;
		while ((p = strstr (unixStyleFileName, "//")) != NULL) {
			strcpy (p, p + 1);
		}
		return FilePath(unixStyleFileName);
	}

	// o disk:[dir.dir]file.type
	// o logical:file.type
	// o [dir.dir]file.type

	if (vmsName [0] == '/') {
		strcpy(unixStyleFileName, vmsName);
	} else {
		unixStyleFileName [0] = '/';
		strcpy(unixStyleFileName + 1, vmsName);
		char *p = strstr(unixStyleFileName, ":[");
		if (p == NULL) {
			// o logical:file.type
			p = strchr(unixStyleFileName, ':');
			*p = '/';
		} else {
			*p = '/';
			strcpy(p + 1, p + 2);
			char *end = strchr(unixStyleFileName, ']');
			if (*end != NULL) {
				*end = '/';
			}
			while (p = strchr(unixStyleFileName, '.'), p != NULL && p < end) {
				*p = '/';
			}
		}
	}
	return FilePath(unixStyleFileName);
} // VMSToUnixStyle

#endif

/**
 * Take a filename or relative path and put it at the end of the current path.
 * If the path is absolute, return the same path.
 */
FilePath FilePath::AbsolutePath() const {
#ifdef WIN32
	// The runtime libraries for GCC and Visual C++ give different results for _fullpath
	// so use the OS.
	char absPath[2000];
	absPath[0] = '\0';
	LPTSTR fileBit = 0;
	::GetFullPathNameA(AsFileSystem(), sizeof(absPath), absPath, &fileBit);
	return FilePath(absPath);
#else
	if (IsAbsolute()) {
		return NormalizePath();
	} else {
		return FilePath(GetWorkingDirectory(), *this).NormalizePath();
	}
#endif
}

// Only used on Windows to fix the case of file names

FilePath FilePath::GetWorkingDirectory() {
	char dir[MAX_PATH + 1];
	_getcwd(dir, MAX_PATH);
	dir[MAX_PATH] = '\0';
	// In Windows, getcwd returns a trailing backslash
	// when the CWD is at the root of a disk, so remove it
	size_t endOfPath = strlen(dir) - 1;
	if (dir[endOfPath] == pathSepChar) {
		dir[endOfPath] = '\0';
	}
	return FilePath(dir);
}

void FilePath::SetWorkingDirectory() const {
	_chdir(AsFileSystem());
}

void FilePath::FixCase() {
}

void FilePath::List(FilePathSet &directories, FilePathSet &files) {
#ifdef WIN32
	FilePath wildCard(*this, "*.*");
	bool complete = false;
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = ::FindFirstFile(wildCard.AsFileSystem(), &findFileData);
	if (hFind != INVALID_HANDLE_VALUE) {
		while (!complete) {
			if ((strcmp(findFileData.cFileName, ".") != 0) && (strcmp(findFileData.cFileName, "..") != 0)) {
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					directories.Append(FilePath(AsInternal(), findFileData.cFileName));
				} else {
					files.Append(FilePath(AsInternal(), findFileData.cFileName));
				}
			}
			if (!::FindNextFile(hFind, &findFileData)) {
				complete = true;
			}
		}
		::FindClose(hFind);
	}
#else
	errno = 0;
	DIR *dp = opendir(AsInternal());
	if (dp == NULL) {
		//~ fprintf(stderr, "%s: cannot open for reading: %s\n", AsInternal(), strerror(errno));
		return;
	}
	struct dirent *ent;
	while ((ent = readdir(dp)) != NULL) {
		if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) {
			FilePath pathFull(AsInternal(), ent->d_name);
			if (pathFull.IsDirectory()) {
				directories.Append(pathFull);
			} else {
				files.Append(pathFull);
			}
		}
	}

	if (errno == 0) {
		closedir(dp);
	}
#endif
}

FILE *FilePath::Open(const char *mode) const {
	return fopen(fileName.c_str(), mode);
}

void FilePath::Remove() const {
	_unlink(AsFileSystem());
}

time_t FilePath::ModifiedTime() const {
	if (IsUntitled())
		return 0;
	struct stat statusFile;
	if (stat(AsFileSystem(), &statusFile) != -1)
		return statusFile.st_mtime;
	else
		return 0;
}

int FilePath::GetFileLength() const {
	int size = -1;
	if (IsSet()) {
		FILE *fp = Open(fileRead);
		if (fp) {
			fseek(fp, 0, SEEK_END);
			size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			fclose(fp);
		}
	}
	return size;
}

bool FilePath::Exists() const {
	bool ret = false;
	if (IsSet()) {
		FILE *fp = Open(fileRead);
		if (fp) {
			ret = true;
			fclose(fp);
		}
	}
	return ret;
}

bool FilePath::IsDirectory() const {
	struct stat statusFile;
	if (stat(AsFileSystem(), &statusFile) != -1)
#ifdef WIN32
		return statusFile.st_mode & _S_IFDIR;
#else
		return statusFile.st_mode & S_IFDIR;
#endif
	else
		return false;
}

bool FilePath::Matches(const char *pattern) const {
	SString pat(pattern);
	pat.substitute(' ', '\0');
	SString nameCopy(fileName);
	nameCopy.lowercase();
	size_t start = 0;
	while (start < pat.length()) {
		const char *patElement = pat.c_str() + start;
		if (patElement[0] == '*') {
			if (nameCopy.endswith(patElement+1)) {
				return true;
			}
		} else {
			if (nameCopy == SString(patElement).lowercase()) {
				return true;
			}
		}
		start += strlen(patElement) + 1;
	}
	return false;
}

#ifdef WIN32
/**
 * Makes a long path from a given, possibly short path/file.
 *
 * The short path/file must exist, and if it is a file it must be fully specified
 * otherwise the function fails.
 *
 * sizeof @a longPath buffer must be a least _MAX_PATH
 * @returns true on success, and the long path in @a longPath buffer,
 * false on failure, and copies the @a shortPath arg to the @a longPath buffer.
 */
bool MakeLongPath(const char* shortPath, char* longPath) {
	// when we have pfnGetLong, we assume it never changes as kernel32 is always loaded
	static DWORD (STDAPICALLTYPE* pfnGetLong)(const char* lpszShortPath, char* lpszLongPath, DWORD cchBuffer) = NULL;
	static bool kernelTried = FALSE;
	bool ok = FALSE;

	if (!kernelTried) {
		HMODULE hModule;
		kernelTried = true;
		hModule = ::GetModuleHandleA("KERNEL32");
		//assert(hModule != NULL); // must not call FreeLibrary on such handle

		// attempt to get GetLongPathName (implemented in Win98/2000 only!)
		(FARPROC&)pfnGetLong = ::GetProcAddress(hModule, "GetLongPathNameA");
	}

	// the kernel GetLongPathName proc is faster and (hopefully) more reliable
	if (pfnGetLong != NULL) {
		// call kernel proc
		ok = (pfnGetLong)(shortPath, longPath, _MAX_PATH) != 0;
	} else {
		char short_path[_MAX_PATH];  // copy, so we can modify it
		char* tok;

		*longPath = '\0';

		lstrcpyn(short_path, shortPath, _MAX_PATH);

		for (;;) {
			tok = strtok(short_path, "\\");
			if (tok == NULL)
				break;

			if ((strlen(shortPath) > 3) &&
			        (shortPath[0] == '\\') && (shortPath[1] == '\\')) {
				// UNC, skip first seps
				strcat(longPath, "\\\\");
				strcat(longPath, tok);
				strcat(longPath, "\\");

				tok = strtok(NULL, "\\");
				if (tok == NULL)
					break;
			}
			strcat(longPath, tok);

			bool isDir = false;

			for (;;) {
				WIN32_FIND_DATA fd;
				HANDLE hfind;
				char* tokend;

				tok = strtok(NULL, "\\");
				if (tok == NULL)
					break;

				strcat(longPath, "\\");
				tokend = longPath + strlen(longPath);

				// temporary add short component
				strcpy(tokend, tok);

				hfind = ::FindFirstFile(longPath, &fd);
				if (hfind == INVALID_HANDLE_VALUE)
					break;

				isDir = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

				// finally add long component we got
				strcpy(tokend, fd.cFileName);

				::FindClose(hfind);
			}
			ok = tok == NULL;

			if (ok && isDir)
				strcat(longPath, "\\");

			break;
		}
	}

	if (!ok) {
		lstrcpyn(longPath, shortPath, _MAX_PATH);
	}
	return ok;
}
#endif

void FilePath::FixName() {
#ifdef WIN32
	// Only used on Windows to use long file names and fix the case of file names
	char longPath[_MAX_PATH];
	// first try MakeLongPath which corrects the path and the case of filename too
	if (MakeLongPath(AsFileSystem(), longPath)) {
		Set(longPath);
	} else {
		// On Windows file comparison is done case insensitively so the user can
		// enter scite.cxx and still open this file, SciTE.cxx. To ensure that the file
		// is saved with correct capitalisation FindFirstFile is used to find out the
		// real name of the file.
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = ::FindFirstFile(AsFileSystem(), &FindFileData);
		FilePath dir = Directory();
		if (hFind != INVALID_HANDLE_VALUE) {	// FindFirstFile found the file
			Set(dir, FindFileData.cFileName);
			::FindClose(hFind);
		}
	}
#endif
}

FilePathSet &FilePathSet::operator=(const FilePathSet &) {
	// Private so won't be called.
	return *this;
}

FilePathSet::FilePathSet(int size_) {
	size = size_;
	body = new FilePath[size];
	lengthBody = 0;
}

FilePathSet::FilePathSet(const FilePathSet &other) {
	size = other.size;
	lengthBody = other.lengthBody;
	body = new FilePath[size];
	for (size_t i=0; i<lengthBody; i++) {
		body[i] = other.body[i];
	}
}

FilePathSet::~FilePathSet() {
	delete []body;
	body = NULL;
	size = 0;
	lengthBody = 0;
}

FilePath FilePathSet::At(size_t pos) const {
	return body[pos];
}

void FilePathSet::Append(FilePath fp) {
	if (lengthBody >= size) {
		size *= 2;
		FilePath *bodyNew = new FilePath[size];
		for (size_t i=0; i<lengthBody; i++) {
			bodyNew[i] = body[i];
		}
		delete []body;
		body = bodyNew;
	}
	body[lengthBody++] = fp;
}

size_t FilePathSet::Length() const {
	return lengthBody;
}
