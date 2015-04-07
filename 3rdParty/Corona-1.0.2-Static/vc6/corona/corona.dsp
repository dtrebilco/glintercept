# Microsoft Developer Studio Project File - Name="corona" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=corona - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "corona.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "corona.mak" CFG="corona - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "corona - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "corona - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "corona - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORONA_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../src/libungif-4.1.0" /I "../../src/jpeg-6b" /I "../../src/libpng-1.2.1" /I "../../src/zlib-1.1.4" /D "NDEBUG" /D for="if (0) ; else for" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORONA_EXPORTS" /D "GLI_CHANGES" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../bin/Release/corona.dll"

!ELSEIF  "$(CFG)" == "corona - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORONA_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../src/libungif-4.1.0" /I "../../src/jpeg-6b" /I "../../src/libpng-1.2.1" /I "../../src/zlib-1.1.4" /D "_DEBUG" /D for="if (0) ; else for" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORONA_EXPORTS" /D "GLI_CHANGES" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../bin/Debug/corona.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "corona - Win32 Release"
# Name "corona - Win32 Debug"
# Begin Group "files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Convert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Corona.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\corona.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Debug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Debug.h
# End Source File
# Begin Source File

SOURCE=..\..\src\DefaultFileSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\MemoryFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\MemoryFile.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Open.h
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenBMP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenGIF.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenJPEG.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenPCX.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenPNG.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\OpenTGA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Save.h
# End Source File
# Begin Source File

SOURCE=..\..\src\SaveJPEG.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\SavePNG.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\SaveTGA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\SimpleImage.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Types.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Utility.h
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\adler32.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\compress.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\crc32.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\deflate.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\deflate.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\gzio.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\infblock.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\infblock.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\infcodes.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\infcodes.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\inffast.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\inffast.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\inffixed.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\inflate.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\inftrees.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\inftrees.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\infutil.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\infutil.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\trees.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\trees.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\uncompr.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\zconf.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\zlib.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\zutil.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\zlib-1.1.4\zutil.h"
# End Source File
# End Group
# Begin Group "png"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\png.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\png.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngasmrd.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngconf.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngerror.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pnggccrd.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngget.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngmem.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngpread.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngread.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngrio.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngrtran.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngrutil.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngset.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngtest.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngtrans.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngvcrd.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngwio.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngwrite.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngwtran.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libpng-1.2.1\pngwutil.c"
# End Source File
# End Group
# Begin Group "jpeg"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcapimin.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcapistd.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jccoefct.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jccolor.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcdctmgr.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jchuff.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jchuff.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcinit.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcmainct.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcmarker.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcmaster.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcomapi.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jconfig.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcparam.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcphuff.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcprepct.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jcsample.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jctrans.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdapimin.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdapistd.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdatadst.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdatasrc.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdcoefct.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdcolor.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdct.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jddctmgr.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdhuff.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdhuff.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdinput.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdmainct.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdmarker.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdmaster.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdmerge.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdphuff.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdpostct.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdsample.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jdtrans.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jerror.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jerror.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jfdctflt.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jfdctfst.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jfdctint.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jidctflt.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jidctfst.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jidctint.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jidctred.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jinclude.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jmemmgr.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jmemnobs.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jmemsys.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jmorecfg.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jpegint.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jpeglib.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jquant1.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jquant2.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jutils.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\jpeg-6b\jversion.h"
# End Source File
# End Group
# Begin Group "gif"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\src\libungif-4.1.0\dgif_lib.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libungif-4.1.0\gif_err.c"
# End Source File
# Begin Source File

SOURCE="..\..\src\libungif-4.1.0\gif_lib.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\libungif-4.1.0\gif_lib_private.h"
# End Source File
# Begin Source File

SOURCE="..\..\src\libungif-4.1.0\gifalloc.c"
# End Source File
# End Group
# End Target
# End Project
