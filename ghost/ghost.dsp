# Microsoft Developer Studio Project File - Name="ghost" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ghost - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ghost.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ghost.mak" CFG="ghost - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ghost - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ghost - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ghost - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GHOST_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GHOST_EXPORTS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x415 /d "NDEBUG"
# ADD RSC /l 0x415 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib d3d8.lib d3dx8.lib strmiids.lib dxguid.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "ghost - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GHOST_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GHOST_EXPORTS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x415 /d "_DEBUG"
# ADD RSC /l 0x415 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib d3d8.lib d3dx8d.lib strmiids.lib dxguid.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy           debug\ghost.dll           c:\windows\          	copy           debug\ghost.lib           c:\windows\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ghost - Win32 Release"
# Name "ghost - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Source\Audio.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Button.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Drag3Button.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\DragButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Event.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\EventEVT.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\EVTSound.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\FileSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Font.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ghost.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Mask16.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Mask24.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Mask32.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Mask8.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Mouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\SinTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Suwak.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\YTable.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Include\Audio.h
# End Source File
# Begin Source File

SOURCE=.\Include\Button.h
# End Source File
# Begin Source File

SOURCE=.\Include\Drag3Button.h
# End Source File
# Begin Source File

SOURCE=.\Include\DragButton.h
# End Source File
# Begin Source File

SOURCE=.\Include\Event.h
# End Source File
# Begin Source File

SOURCE=.\Include\EventEVT.h
# End Source File
# Begin Source File

SOURCE=.\Include\EVTSound.h
# End Source File
# Begin Source File

SOURCE=.\Include\FileSystem.h
# End Source File
# Begin Source File

SOURCE=.\Include\Font.h
# End Source File
# Begin Source File

SOURCE=.\Include\Ghost.h
# End Source File
# Begin Source File

SOURCE=.\Include\Mask16.h
# End Source File
# Begin Source File

SOURCE=.\Include\Mask24.h
# End Source File
# Begin Source File

SOURCE=.\Include\Mask32.h
# End Source File
# Begin Source File

SOURCE=.\Include\Mask8.h
# End Source File
# Begin Source File

SOURCE=.\Include\Mouse.h
# End Source File
# Begin Source File

SOURCE=.\Include\SinTable.h
# End Source File
# Begin Source File

SOURCE=.\Include\Sound.h
# End Source File
# Begin Source File

SOURCE=.\Include\Sprite.h
# End Source File
# Begin Source File

SOURCE=.\Include\Suwak.h
# End Source File
# Begin Source File

SOURCE=.\Include\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Include\YTable.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
