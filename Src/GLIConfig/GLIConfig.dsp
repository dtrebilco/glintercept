# Microsoft Developer Studio Project File - Name="GLIConfig" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GLIConfig - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GLIConfig.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GLIConfig.mak" CFG="GLIConfig - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GLIConfig - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GLIConfig - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GLIConfig - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_msw"
# PROP BASE Intermediate_Dir "vc_msw\GLIConfig"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Intermediate\GLIConfig\vc_msw"
# PROP Intermediate_Dir "..\Intermediate\GLIConfig\vc_msw\GLIConfig"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I ".\..\..\lib\vc_lib\msw" /I ".\..\..\include" /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /Fd"vc_msw\GLIConfig.pdb" /FD /EHsc /c
# ADD CPP /nologo /MD /W3 /GR /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "$(WXWIN)\contrib\include" /I "." /I "..\Common" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /D "GLI_BUILD_WINDOWS" /Fd"..\Intermediate\GLIConfig\vc_msw\GLIConfig.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\lib\vc_lib\msw" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "__WXMSW__" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i ".\..\..\lib\vc_lib\msw" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "__WXMSW__" /d "_WINDOWS" /d "NOPCH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib /nologo /subsystem:windows /machine:I386 /libpath:".\..\..\lib\vc_lib"
# ADD LINK32 wxmsw26_propgrid.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\Bin\GLIConfig.exe" /libpath:"$(WXWIN)\lib\vc_lib"

!ELSEIF  "$(CFG)" == "GLIConfig - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswd"
# PROP BASE Intermediate_Dir "vc_mswd\GLIConfig"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Intermediate\GLIConfig\vc_mswd"
# PROP Intermediate_Dir "..\Intermediate\GLIConfig\vc_mswd\GLIConfig"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /I ".\..\..\lib\vc_lib\mswd" /I ".\..\..\include" /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /Fd"vc_mswd\GLIConfig.pdb" /FD /GZ /EHsc /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /Zi /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "$(WXWIN)\contrib\include" /I "." /I "..\Common" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /D "GLI_BUILD_WINDOWS" /Fd"..\Intermediate\GLIConfig\vc_mswd\GLIConfig.pdb" /FD /GZ /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\lib\vc_lib\mswd" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i ".\..\..\lib\vc_lib\mswd" /i ".\..\..\include" /i "." /i ".\..\..\samples" /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_WINDOWS" /d "NOPCH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\lib\vc_lib"
# ADD LINK32 wxmsw26d_propgrid.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\Bin\GLIConfig.exe" /libpath:"$(WXWIN)\lib\vc_lib"

!ENDIF 

# Begin Target

# Name "GLIConfig - Win32 Release"
# Name "GLIConfig - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CommonErrorLog.h
# End Source File
# Begin Source File

SOURCE=.\ConfigOptionsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigOptionsDialog.h
# End Source File
# Begin Source File

SOURCE=.\GLIConfigApp.cpp
# End Source File
# Begin Source File

SOURCE=.\GLIConfigApp.h
# End Source File
# Begin Source File

SOURCE=.\MainDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDialog.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\ConfigParser.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\ConfigParser.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GLIConfig.rc

!IF  "$(CFG)" == "GLIConfig - Win32 Release"

# ADD BASE RSC /l 0xc09
# ADD RSC /l 0xc09 /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include"
# SUBTRACT RSC /i ".\..\..\lib\vc_lib\msw" /i ".\..\..\include" /i ".\..\..\samples"

!ELSEIF  "$(CFG)" == "GLIConfig - Win32 Debug"

# ADD BASE RSC /l 0xc09
# ADD RSC /l 0xc09 /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include"
# SUBTRACT RSC /i ".\..\..\lib\vc_lib\mswd" /i ".\..\..\include" /i ".\..\..\samples"

!ENDIF 

# End Source File
# End Target
# End Project
