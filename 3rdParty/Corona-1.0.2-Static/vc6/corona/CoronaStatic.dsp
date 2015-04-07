# Microsoft Developer Studio Project File - Name="CoronaStatic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CoronaStatic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CoronaStatic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CoronaStatic.mak" CFG="CoronaStatic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CoronaStatic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CoronaStatic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "StaticRelease"
# PROP Intermediate_Dir "StaticRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../src/libungif-4.1.0" /I "../../src/jpeg-6b" /I "../../src/libpng-1.2.1" /I "../../src/zlib-1.1.4" /D "NDEBUG" /D for="if (0) ; else for" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORONA_EXPORTS" /D "GLI_CHANGES" /YX /FD /c
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\Lib\Corona_static_vc6.lib"

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "StaticDebug"
# PROP Intermediate_Dir "StaticDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../src/libungif-4.1.0" /I "../../src/jpeg-6b" /I "../../src/libpng-1.2.1" /I "../../src/zlib-1.1.4" /D "_DEBUG" /D for="if (0) ; else for" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORONA_EXPORTS" /D "GLI_CHANGES" /YX /FD /GZ /c
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "CoronaStatic - Win32 Release"
# Name "CoronaStatic - Win32 Debug"
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

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OpenGIF.cpp

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OpenJPEG.cpp

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OpenPCX.cpp

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OpenPNG.cpp

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\OpenTGA.cpp

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

!ENDIF 

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

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\src\libungif-4.1.0\gif_err.c"

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\src\libungif-4.1.0\gif_lib_private.h"

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\src\libungif-4.1.0\gifalloc.c"

!IF  "$(CFG)" == "CoronaStatic - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "CoronaStatic - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
