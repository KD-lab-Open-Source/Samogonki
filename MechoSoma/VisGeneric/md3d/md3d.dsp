# Microsoft Developer Studio Project File - Name="md3d" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=md3d - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "md3d.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "md3d.mak" CFG="md3d - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "md3d - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "md3d - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "md3d - Win32 Final" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "md3d - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Garbage/MECHOSOMA/Release/md3d"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Release/md3d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /Gi /GR /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "md3d - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\Garbage/MECHOSOMA/Debug/md3d"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Debug/md3d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"md3d.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "md3d - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "md3d___Win32_Final"
# PROP BASE Intermediate_Dir "md3d___Win32_Final"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Garbage/MECHOSOMA/Final/md3d"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Final/md3d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FINAL_VERSION_" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"md3d.lib"
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "md3d - Win32 Release"
# Name "md3d - Win32 Debug"
# Name "md3d - Win32 Final"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\d3dBackBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dEnum.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dGamma.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dInit.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dMisc.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dRender.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dScreenShot.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dSprites.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dTSS.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dUtil.cpp

!IF  "$(CFG)" == "md3d - Win32 Release"

!ELSEIF  "$(CFG)" == "md3d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "md3d - Win32 Final"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\md3d.cpp

!IF  "$(CFG)" == "md3d - Win32 Release"

!ELSEIF  "$(CFG)" == "md3d - Win32 Debug"

# ADD CPP /Yc"md3d.h"

!ELSEIF  "$(CFG)" == "md3d - Win32 Final"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BaseClass.h
# End Source File
# Begin Source File

SOURCE=.\d3dEnum.h
# End Source File
# Begin Source File

SOURCE=.\d3dUtil.h
# End Source File
# Begin Source File

SOURCE=.\Md3d.h
# End Source File
# Begin Source File

SOURCE=.\x3dutil.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Docs.txt
# End Source File
# Begin Source File

SOURCE=.\RenderStates.txt
# End Source File
# Begin Source File

SOURCE=.\x3u_pixcvt.cpp

!IF  "$(CFG)" == "md3d - Win32 Release"

!ELSEIF  "$(CFG)" == "md3d - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "md3d - Win32 Final"

!ENDIF 

# End Source File
# End Target
# End Project
