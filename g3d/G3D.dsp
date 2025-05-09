# Microsoft Developer Studio Project File - Name="G3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=G3D - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "G3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "G3D.mak" CFG="G3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "G3D - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "G3D - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "G3D - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x415 /d "NDEBUG"
# ADD RSC /l 0x415 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "G3D - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x415 /d "_DEBUG"
# ADD RSC /l 0x415 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "G3D - Win32 Release"
# Name "G3D - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\source\Flare.cpp
# End Source File
# Begin Source File

SOURCE=.\source\FlareFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\source\FlareTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\source\keyframer.cpp
# End Source File
# Begin Source File

SOURCE=.\source\MATERIAL.CPP
# End Source File
# Begin Source File

SOURCE=.\source\MATRIX.CPP
# End Source File
# Begin Source File

SOURCE=.\source\MTXSTACK.CPP
# End Source File
# Begin Source File

SOURCE=.\source\OBJECT.CPP
# End Source File
# Begin Source File

SOURCE=.\source\QUATERNION.CPP
# End Source File
# Begin Source File

SOURCE=.\source\SCENE.CPP
# End Source File
# Begin Source File

SOURCE=.\source\Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\source\TEXTURE.CPP
# End Source File
# Begin Source File

SOURCE=.\source\TRACK.CPP
# End Source File
# Begin Source File

SOURCE=.\source\VECTOR.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\Debug.h
# End Source File
# Begin Source File

SOURCE=.\include\Flare.h
# End Source File
# Begin Source File

SOURCE=.\include\FlareFrame.h
# End Source File
# Begin Source File

SOURCE=.\include\FlareTrack.h
# End Source File
# Begin Source File

SOURCE=.\include\keyframer.h
# End Source File
# Begin Source File

SOURCE=.\include\MATERIAL.H
# End Source File
# Begin Source File

SOURCE=.\include\MATH3D.H
# End Source File
# Begin Source File

SOURCE=.\include\NAMEMAP.H
# End Source File
# Begin Source File

SOURCE=.\include\OBJECT.H
# End Source File
# Begin Source File

SOURCE=.\include\SCENE.H
# End Source File
# Begin Source File

SOURCE=.\include\Sprite.h
# End Source File
# Begin Source File

SOURCE=.\include\TEXTURE.H
# End Source File
# Begin Source File

SOURCE=.\include\TRACK.H
# End Source File
# End Group
# End Target
# End Project
