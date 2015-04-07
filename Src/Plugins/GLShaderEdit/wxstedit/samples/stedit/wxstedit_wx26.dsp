# Microsoft Developer Studio Project File - Name="wxStEdit_wx26" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=wxStEdit_wx26 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxstedit_wx26.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxstedit_wx26.mak" CFG="wxStEdit_wx26 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxStEdit_wx26 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "wxStEdit_wx26 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxStEdit_wx26 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_msw_wx26"
# PROP BASE Intermediate_Dir "vc_msw_wx26\wxStEdit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Intermediate\wxStEdit\vc_msw_wx26"
# PROP Intermediate_Dir "..\..\..\..\..\Intermediate\wxStEdit\vc_msw_wx26"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O1 /I "$(WXWIN)\include" /I "$(WXWIN)\lib\vc_lib\msw" /I "." /I "$(WXWIN)\contrib\include" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /Fd"vc_msw_wx26\wxStEdit.pdb" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O1 /I "$(WXWIN)\include" /I "$(WXWIN)\lib\vc_lib\msw" /I "." /I "$(WXWIN)\contrib\include" /I "..\..\include" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /Fd"..\..\..\..\..\Intermediate\wxStEdit\vc_msw_wx26\wxStEdit.pdb" /FD /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "$(WXWIN)\include" /i "$(WXWIN)\lib\vc_lib\msw" /i "." /d "__WXMSW__" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "$(WXWIN)\include" /i "$(WXWIN)\lib\vc_lib\msw" /i "." /d "__WXMSW__" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase26_net.lib wxmsw26_stc.lib wxmsw26_adv.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /machine:I386 /out:"vc_msw_wx26\wxStEdit.exe" /libpath:"$(WXWIN)\lib\vc_lib"
# ADD LINK32 ..\..\..\..\..\..\lib\StEditLib_wx26.lib wxmsw26_html.lib wxbase26_net.lib wxmsw26_stc.lib wxmsw26_adv.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib glslang.lib /nologo /subsystem:windows /machine:I386 /out:"..\..\..\..\..\Intermediate\wxStEdit\vc_msw_wx26\wxStEdit.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\..\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\..\..\..\..\Intermediate\wxStEdit\vc_msw_wx26\wxStEdit.exe c:\projects\glintercept\bin\plugins\GLShaderEdit\GLISciTE.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "wxStEdit_wx26 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswd_wx26"
# PROP BASE Intermediate_Dir "vc_mswd_wx26\wxStEdit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\Intermediate\wxStEdit\vc_mswd_wx26"
# PROP Intermediate_Dir "..\..\..\..\..\Intermediate\wxStEdit\vc_mswd_wx26"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "$(WXWIN)\include" /I "$(WXWIN)\lib\vc_lib\mswd" /I "." /I "$(WXWIN)\contrib\include" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /Fd"vc_mswd_wx26\wxStEdit.pdb" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "$(WXWIN)\include" /I "$(WXWIN)\lib\vc_lib\mswd" /I "." /I "$(WXWIN)\contrib\include" /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /Fd"..\..\..\..\..\Intermediate\wxStEdit\vc_mswd_wx26\wxStEdit.pdb" /FD /GZ /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /i "$(WXWIN)\include" /i "$(WXWIN)\lib\vc_lib\mswd" /i "." /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_WINDOWS"
# ADD RSC /l 0x405 /i "$(WXWIN)\include" /i "$(WXWIN)\lib\vc_lib\mswd" /i "." /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_WINDOWS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxbase26d_net.lib wxmsw26d_stc.lib wxmsw26d_adv.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /subsystem:windows /debug /machine:I386 /out:"vc_mswd_wx26\wxStEdit.exe" /libpath:"$(WXWIN)\lib\vc_lib"
# ADD LINK32 ..\..\..\..\..\..\lib\StEditLibd_wx26.lib wxmsw26d_html.lib wxbase26d_net.lib wxmsw26d_stc.lib wxmsw26d_adv.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib glslang.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\..\..\..\Intermediate\wxStEdit\vc_mswd_wx26\wxStEdit.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\..\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\..\..\..\..\Intermediate\wxStEdit\vc_mswd_wx26\wxStEdit.exe c:\projects\glintercept\bin\plugins\GLShaderEdit\GLISciTE.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "wxStEdit_wx26 - Win32 Release"
# Name "wxStEdit_wx26 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\wxStEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\wxStEdit.rc
# End Source File
# End Group
# End Target
# End Project
