# Microsoft Developer Studio Project File - Name="PluginMAX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PluginMAX - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PluginMAX.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PluginMAX.mak" CFG="PluginMAX - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PluginMAX - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PluginMAX - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "PluginMAX - Win32 Final" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PluginMAX - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Garbage/MECHOSOMA/Release/PluginMAX"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Release/PluginMAX"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gi /GR /GX /Zd /O2 /I "..\Util" /I ".." /I "inc" /I "src" /I "util" /D "NDEBUG" /D "_USE_XTOOL_" /D "_WINDOWS" /D "WIN32" /YX /FD /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "PluginMAX - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\Garbage/MECHOSOMA/Debug/PluginMAX"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Debug/PluginMAX"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gi /GR /GX /Zi /Od /I "..\Util" /I ".." /I "inc" /I "src" /I "util" /D "_USE_XTOOL_" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "PluginMAX - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PluginMAX___Win32_Final"
# PROP BASE Intermediate_Dir "PluginMAX___Win32_Final"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Garbage/MECHOSOMA/Final/PluginMAX"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Final/PluginMAX"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gi /GX /O2 /I "..\Util" /I ".." /D "_USE_XTOOL_" /D "_WINDOWS" /D "NDEBUG" /D "WIN32" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GR /GX /O2 /I "..\Util" /I ".." /I "inc" /I "src" /I "util" /D "NDEBUG" /D "_FINAL_VERSION_" /D "_USE_XTOOL_" /D "_WINDOWS" /D "WIN32" /YX /FD /c
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

# Name "PluginMAX - Win32 Release"
# Name "PluginMAX - Win32 Debug"
# Name "PluginMAX - Win32 Final"
# Begin Group "Scene"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\AnimationMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AnimationMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ChannelAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ChannelMapping.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HelperObject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\LodObject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MaterialObject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NodeObject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ObjectMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SceneMesh.cpp
# End Source File
# End Group
# Begin Group "MeshFile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\DefMeshFile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MeshFile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\StreamBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\StreamBuffer.h
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\inc\DefMeshFile.h
# End Source File
# Begin Source File

SOURCE=.\inc\meshfile.h
# End Source File
# Begin Source File

SOURCE=.\inc\scenemesh.h
# End Source File
# End Group
# Begin Group "util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\util\BaseClass.h
# End Source File
# Begin Source File

SOURCE=.\util\cString.h
# End Source File
# End Group
# End Target
# End Project
