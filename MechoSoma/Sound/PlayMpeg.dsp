# Microsoft Developer Studio Project File - Name="PlayMpeg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=PlayMpeg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PlayMpeg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PlayMpeg.mak" CFG="PlayMpeg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PlayMpeg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "PlayMpeg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MindMaze/Sound/SOUND", PJGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PlayMpeg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PlayMpeg___Win32_Release"
# PROP BASE Intermediate_Dir "PlayMpeg___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PlayMpeg___Win32_Release"
# PROP Intermediate_Dir "PlayMpeg___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"PlayMpeg.lib"

!ELSEIF  "$(CFG)" == "PlayMpeg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PlayMpeg___Win32_Debug"
# PROP BASE Intermediate_Dir "PlayMpeg___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PlayMpeg___Win32_Debug"
# PROP Intermediate_Dir "PlayMpeg___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"PlayMpegDBG.lib"

!ENDIF 

# Begin Target

# Name "PlayMpeg - Win32 Release"
# Name "PlayMpeg - Win32 Debug"
# Begin Group "mp+"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\mp+\bitstream_decode.h"
# End Source File
# Begin Source File

SOURCE=".\mp+\huff_new.h"
# End Source File
# Begin Source File

SOURCE=".\mp+\huff_old.h"
# End Source File
# Begin Source File

SOURCE=".\mp+\mppdec.cpp"
# End Source File
# Begin Source File

SOURCE=".\mp+\mppdec.h"
# End Source File
# Begin Source File

SOURCE=".\mp+\requant.h"
# End Source File
# Begin Source File

SOURCE=".\mp+\synth_filter.h"
# End Source File
# End Group
# Begin Source File

SOURCE=.\PlayMpeg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayMpeg.h
# End Source File
# End Target
# End Project
