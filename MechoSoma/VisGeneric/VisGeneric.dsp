# Microsoft Developer Studio Project File - Name="VisGeneric" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VisGeneric - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisGeneric.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisGeneric.mak" CFG="VisGeneric - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisGeneric - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VisGeneric - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "VisGeneric - Win32 Final" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Garbage/MECHOSOMA/Release/VisGeneric"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Release/VisGeneric"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gi /GR /GX /Zd /O2 /I "..\IVisGeneric" /I "Scripts" /I "Graph3d" /I "PluginMAX" /I "Util" /I "Terra" /I "..\RUNTIME" /I "..\XREAL" /I "..\FX" /I "..\TERRA" /I "..\XMath" /I "PluginMAX\src" /I "PluginMAX\inc" /I "..\Sound" /I "md3d" /D "_USE_TILEMAP_" /D "_USE_TERRA_" /D "NO_STRICT" /D "_MECHOSOMA_" /D "_USE_XTOOL_" /D "_WINDOWS" /D "NDEBUG" /D "WIN32" /YX /FD /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"MechoSoma.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\Garbage/MECHOSOMA/DEBUG/VisGeneric"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/DEBUG/VisGeneric"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gi /GR /GX /Zi /Od /I "..\IVisGeneric" /I "Scripts" /I "Graph3d" /I "PluginMAX" /I "Util" /I "Terra" /I "..\RUNTIME" /I "..\XREAL" /I "..\FX" /I "..\TERRA" /I "..\XMath" /I "PluginMAX\src" /I "PluginMAX\inc" /I "..\Sound" /I "md3d" /D "_USE_TILEMAP_" /D "_USE_TERRA_" /D "NO_STRICT" /D "_MECHOSOMA_" /D "_USE_XTOOL_" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "VisGeneric___Win32_Final"
# PROP BASE Intermediate_Dir "VisGeneric___Win32_Final"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Garbage/MECHOSOMA/Final/VisGeneric"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Final/VisGeneric"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gi /GX /O2 /I "..\IVisGeneric" /I "Scripts" /I "Graph3d" /I "PluginMAX" /I "Util" /I "Terra" /I "..\RUNTIME" /I "..\XREAL" /I "..\FX" /I "..\TERRA" /I "..\XMath" /D "_USE_TILEMAP_" /D "_USE_TERRA_" /D "NO_STRICT" /D "_MECHOSOMA_" /D "_USE_XTOOL_" /D "_WINDOWS" /D "NDEBUG" /D "WIN32" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GR /GX /O2 /I "..\IVisGeneric" /I "Scripts" /I "Graph3d" /I "PluginMAX" /I "Util" /I "Terra" /I "..\RUNTIME" /I "..\XREAL" /I "..\FX" /I "..\TERRA" /I "..\XMath" /I "PluginMAX\src" /I "PluginMAX\inc" /I "..\Sound" /I "md3d" /D "_USE_TILEMAP_" /D "_USE_TERRA_" /D "NO_STRICT" /D "_MECHOSOMA_" /D "_USE_XTOOL_" /D "_WINDOWS" /D "NDEBUG" /D "WIN32" /D "_FINAL_VERSION_" /YX /FD /c
# ADD BASE RSC /l 0x419
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"MechoSoma.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "VisGeneric - Win32 Release"
# Name "VisGeneric - Win32 Debug"
# Name "VisGeneric - Win32 Final"
# Begin Group "Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BaseClass.h
# End Source File
# Begin Source File

SOURCE=.\BaseDefine.h
# End Source File
# Begin Source File

SOURCE=.\BoundingBox.h
# End Source File
# Begin Source File

SOURCE=.\Util\cString.h
# End Source File
# Begin Source File

SOURCE=.\IncTerra.h
# End Source File
# End Group
# Begin Group "Unknown"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UnkLibrary.h
# End Source File
# Begin Source File

SOURCE=.\Unknown.cpp
# End Source File
# End Group
# Begin Group "Scripts"

# PROP Default_Filter ""
# Begin Group "WorldScript"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Scripts\ForestBeginWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\ForestBeginWS.scr
InputName=ForestBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\ForestBeginWS.scr
InputName=ForestBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\ForestBeginWS.scr
InputName=ForestBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\ForestVictoryWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\ForestVictoryWS.scr
InputName=ForestVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\ForestVictoryWS.scr
InputName=ForestVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\ForestVictoryWS.scr
InputName=ForestVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\GameVictoryWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\GameVictoryWS.scr
InputName=GameVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\GameVictoryWS.scr
InputName=GameVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\GameVictoryWS.scr
InputName=GameVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\LavaBeginWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\LavaBeginWS.scr
InputName=LavaBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\LavaBeginWS.scr
InputName=LavaBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\LavaBeginWS.scr
InputName=LavaBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\LavaVictoryWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\LavaVictoryWS.scr
InputName=LavaVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\LavaVictoryWS.scr
InputName=LavaVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\LavaVictoryWS.scr
InputName=LavaVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\MountainBeginWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\MountainBeginWS.scr
InputName=MountainBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\MountainBeginWS.scr
InputName=MountainBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\MountainBeginWS.scr
InputName=MountainBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\MountainVictoryWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\MountainVictoryWS.scr
InputName=MountainVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\MountainVictoryWS.scr
InputName=MountainVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\MountainVictoryWS.scr
InputName=MountainVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\WatchBeginWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WatchBeginWS.scr
InputName=WatchBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WatchBeginWS.scr
InputName=WatchBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WatchBeginWS.scr
InputName=WatchBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\WatchVictoryWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WatchVictoryWS.scr
InputName=WatchVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WatchVictoryWS.scr
InputName=WatchVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WatchVictoryWS.scr
InputName=WatchVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\WaterBeginWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WaterBeginWS.scr
InputName=WaterBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WaterBeginWS.scr
InputName=WaterBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WaterBeginWS.scr
InputName=WaterBeginWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\WaterVictoryWS.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WaterVictoryWS.scr
InputName=WaterVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WaterVictoryWS.scr
InputName=WaterVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\WaterVictoryWS.scr
InputName=WaterVictoryWS

"$(ProjDir)\..\Resource\WorldScript\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr WorldScript.def -ifVISGENERIC\SCRIPTS\   -ofRESOURCE\WorldScript\  -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\WorldScript.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Scripts\WorldScript.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Scripts\Arcane.inc
# End Source File
# Begin Source File

SOURCE=.\Scripts\arcane_name.inc
# End Source File
# Begin Source File

SOURCE=.\Scripts\M3d.def
# End Source File
# Begin Source File

SOURCE=.\Scripts\M3d.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

USERDEP__M3D_S="..\IVisGeneric\m3d_id.h"	"Scripts\arcane_name.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\M3d.scr
InputName=M3d

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

USERDEP__M3D_S="..\IVisGeneric\m3d_id.h"	"Scripts\arcane_name.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\M3d.scr
InputName=M3d

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

USERDEP__M3D_S="..\IVisGeneric\m3d_id.h"	"Scripts\arcane_name.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\M3d.scr
InputName=M3d

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\m3d_eff.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

USERDEP__M3D_E="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_eff.scr
InputName=m3d_eff

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

USERDEP__M3D_E="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_eff.scr
InputName=m3d_eff

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

USERDEP__M3D_E="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_eff.scr
InputName=m3d_eff

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\m3d_font.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

USERDEP__M3D_F="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_font.scr
InputName=m3d_font

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

USERDEP__M3D_F="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_font.scr
InputName=m3d_font

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

USERDEP__M3D_F="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_font.scr
InputName=m3d_font

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\m3d_font_add.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

USERDEP__M3D_FO="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_font_add.scr
InputName=m3d_font_add

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

USERDEP__M3D_FO="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_font_add.scr
InputName=m3d_font_add

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

USERDEP__M3D_FO="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_font_add.scr
InputName=m3d_font_add

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\m3d_iscreen.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

USERDEP__M3D_I="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_iscreen.scr
InputName=m3d_iscreen

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

USERDEP__M3D_I="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_iscreen.scr
InputName=m3d_iscreen

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

USERDEP__M3D_I="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\m3d_iscreen.scr
InputName=m3d_iscreen

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\M3D_Xreal.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

USERDEP__M3D_X="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\M3D_Xreal.scr
InputName=M3D_Xreal

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

USERDEP__M3D_X="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\M3D_Xreal.scr
InputName=M3D_Xreal

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

USERDEP__M3D_X="..\IVisGeneric\m3d_id.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\M3D_Xreal.scr
InputName=M3D_Xreal

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr m3d.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\M3dsetup.h
# End Source File
# Begin Source File

SOURCE=.\Scripts\world.def
# End Source File
# Begin Source File

SOURCE=.\Scripts\World.scr

!IF  "$(CFG)" == "VisGeneric - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\World.scr
InputName=World

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr world.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\World.scr
InputName=World

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr world.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VisGeneric - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Scripts\World.scr
InputName=World

"$(ProjDir)\..\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(ProjDir)\.. 
	parser $(InputName).scr world.def -ifVISGENERIC\SCRIPTS\  -ofRESOURCE\          -out$(InputName).scb -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\WorldSetup.h
# End Source File
# End Group
# Begin Group "VisGeneric"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vgBaseObject.cpp
# End Source File
# Begin Source File

SOURCE=.\vgCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\vgGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\vgObject.cpp
# End Source File
# Begin Source File

SOURCE=.\vgOmni.cpp
# End Source File
# Begin Source File

SOURCE=.\vgUnknown.cpp
# End Source File
# Begin Source File

SOURCE=.\vgWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\VisGeneric.cpp
# End Source File
# Begin Source File

SOURCE=.\VisGeneric.h
# End Source File
# End Group
# Begin Group "PolyMgr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\drawPolygonShade.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawPolygonShade.h
# End Source File
# Begin Source File

SOURCE=.\PolyMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\PolyMgr.h
# End Source File
# Begin Source File

SOURCE=.\PolyMgrD3D.cpp
# End Source File
# Begin Source File

SOURCE=.\PolyMgrDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\PolyMgrTex.cpp
# End Source File
# End Group
# Begin Group "Scene"

# PROP Default_Filter ""
# Begin Group "OLD"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\base.h
# End Source File
# Begin Source File

SOURCE=.\BaseLight.h
# End Source File
# Begin Source File

SOURCE=.\BaseObject.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseObject.h
# End Source File
# Begin Source File

SOURCE=.\BasePolyGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\BasePolyGrid.h
# End Source File
# Begin Source File

SOURCE=.\BaseReflection.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseReflection.h
# End Source File
# Begin Source File

SOURCE=.\BaseTrail.h
# End Source File
# Begin Source File

SOURCE=.\Dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Dummy.cpp
# End Source File
# Begin Source File

SOURCE=.\Dummy.h
# End Source File
# Begin Source File

SOURCE=.\File3ds.cpp
# End Source File
# Begin Source File

SOURCE=.\File3ds.h
# End Source File
# Begin Source File

SOURCE=.\Frame.cpp
# End Source File
# Begin Source File

SOURCE=.\Frame.h
# End Source File
# Begin Source File

SOURCE=.\HashStringGroup.h
# End Source File
# Begin Source File

SOURCE=.\Levin.cpp
# End Source File
# Begin Source File

SOURCE=.\Levin.h
# End Source File
# Begin Source File

SOURCE=.\M3d_effects.h
# End Source File
# Begin Source File

SOURCE=.\Math3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Math3d.h
# End Source File
# Begin Source File

SOURCE=.\Maths.cpp
# End Source File
# Begin Source File

SOURCE=.\Maths.h
# End Source File
# Begin Source File

SOURCE=.\Mesh3ds.cpp
# End Source File
# Begin Source File

SOURCE=.\Mesh3ds.h
# End Source File
# Begin Source File

SOURCE=.\Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Object.h
# End Source File
# Begin Source File

SOURCE=.\Object3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Object3d.h
# End Source File
# Begin Source File

SOURCE=.\PolyGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\PolyGrid.h
# End Source File
# Begin Source File

SOURCE=.\xgr.cpp
# End Source File
# Begin Source File

SOURCE=.\XGR.h
# End Source File
# End Group
# Begin Group "Camera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# Begin Source File

SOURCE=.\RenderDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\RenderDevice.h
# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TileMap.cpp
# End Source File
# Begin Source File

SOURCE=.\TileMap.h
# End Source File
# Begin Source File

SOURCE=.\TileWater.cpp
# End Source File
# Begin Source File

SOURCE=.\TileWater.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\Scene.h
# End Source File
# End Group
# Begin Group "TexMgr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Material.cpp
# End Source File
# Begin Source File

SOURCE=.\Material.h
# End Source File
# Begin Source File

SOURCE=.\TexMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TexMgr.h
# End Source File
# Begin Source File

SOURCE=.\TgaServ.cpp
# End Source File
# Begin Source File

SOURCE=.\TgaServ.h
# End Source File
# End Group
# Begin Group "GameClient"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AnimChannelNode.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimChannelNode.h
# End Source File
# Begin Source File

SOURCE=.\DefineWSC.h
# End Source File
# Begin Source File

SOURCE=.\GameClient.cpp
# End Source File
# Begin Source File

SOURCE=.\GameClient.h
# End Source File
# Begin Source File

SOURCE=.\PlayWSC.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayWSC.h
# End Source File
# Begin Source File

SOURCE=.\SurmapClient.cpp
# End Source File
# Begin Source File

SOURCE=.\SurmapClient.h
# End Source File
# Begin Source File

SOURCE=.\WinVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\WinVideo.h
# End Source File
# End Group
# Begin Group "IVisGeneric"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\IVisGeneric\IVisGeneric.h
# End Source File
# Begin Source File

SOURCE=..\IVisGeneric\M3d_id.h
# End Source File
# Begin Source File

SOURCE=..\IVisGeneric\Umath.h
# End Source File
# Begin Source File

SOURCE=..\IVisGeneric\Unknown.h
# End Source File
# Begin Source File

SOURCE=..\IVisGeneric\VisGenericDefine.h
# End Source File
# Begin Source File

SOURCE=..\IVisGeneric\World_id.h
# End Source File
# End Group
# End Target
# End Project
