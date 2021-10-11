# Microsoft Developer Studio Project File - Name="Graph3d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Graph3d - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Graph3d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Graph3d.mak" CFG="Graph3d - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Graph3d - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Graph3d - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Graph3d - Win32 Final" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Graph3d - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Garbage/MECHOSOMA/Release/Graph3d"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Release/Graph3d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gi /GR /GX /Zd /O2 /I "..\Util" /I ".." /I "..\.." /I "..\..\IVisGeneric" /I "..\md3d" /D "NO_STRICT" /D "_WINDOWS" /D "NDEBUG" /D "WIN32" /YX /FD /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Graph3d - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\Garbage/MECHOSOMA/Debug/Graph3d"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Debug/Graph3d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gi /GR /GX /Zi /Od /I "..\Util" /I ".." /I "..\.." /I "..\..\IVisGeneric" /I "..\md3d" /D "NO_STRICT" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Graph3d - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Graph3d___Win32_Final"
# PROP BASE Intermediate_Dir "Graph3d___Win32_Final"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Garbage/MECHOSOMA/Final/Graph3d"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Final/Graph3d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gi /GX /O2 /I "..\Util" /I ".." /I "..\.." /I "..\..\IVisGeneric" /D "NO_STRICT" /D "_WINDOWS" /D "NDEBUG" /D "WIN32" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GR /GX /O2 /I "..\Util" /I ".." /I "..\.." /I "..\..\IVisGeneric" /I "..\md3d" /D "NO_STRICT" /D "_WINDOWS" /D "NDEBUG" /D "WIN32" /D "_FINAL_VERSION_" /YX /FD /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Graph3d - Win32 Release"
# Name "Graph3d - Win32 Debug"
# Name "Graph3d - Win32 Final"
# Begin Group "Software"

# PROP Default_Filter ""
# Begin Group "Software16"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\S16Color.cpp
# End Source File
# Begin Source File

SOURCE=.\S16Texture1.cpp
# End Source File
# Begin Source File

SOURCE=.\Software16.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Graph3d_Software.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph3d_Software.h
# End Source File
# End Group
# Begin Group "Direct3d"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Graph3d_Direct3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph3d_Direct3d.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\IGraph3d.cpp
# End Source File
# Begin Source File

SOURCE=.\IGraph3d.h
# End Source File
# End Target
# End Project
