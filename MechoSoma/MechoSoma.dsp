# Microsoft Developer Studio Project File - Name="MechoSoma" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MechoSoma - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MechoSoma.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MechoSoma.mak" CFG="MechoSoma - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MechoSoma - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MechoSoma - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "MechoSoma - Win32 Final" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Garbage/MECHOSOMA/Release"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gi /GR /GX /Zd /O2 /I "RUNTIME" /I "Terra" /I "Xreal" /I "XMath" /I "iScreen" /I "iScreen\SCRIPTS" /I "FX" /I "SOUND" /I "Network" /I "VisGeneric\SCRIPTS" /I "VisGeneric" /I "IVisGeneric" /I "VisGeneric\Graph3d" /I "VisGeneric\Util" /I "Xreal\Joystick" /I "Runtime\Scripts" /I "Xreal\TnT" /I "VisGeneric\md3d" /D "_MECHOSOMA_" /D "_WINDOWS" /D "NDEBUG" /D "WIN32" /Yu"StdAfx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"MechoSoma.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 wininet.lib PlayMpeg.lib xtool.lib xgraph.lib aci_parser.lib _xsound.lib xreal\qhull\qhull.lib PrmEdit.lib ftkvc50.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ddraw.lib dxguid.lib winmm.lib dsound.lib wsock32.lib jpeg.lib dinput.lib strmbase.lib /nologo /subsystem:windows /pdb:"MechoSoma.pdb" /map:"MechoSoma.map" /debug /machine:I386 /out:"game.exe"
# SUBTRACT LINK32 /profile /pdb:none

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\Garbage/MECHOSOMA/Debug"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gm /Gi /GR /GX /Zi /Od /I "RUNTIME" /I "Terra" /I "Xreal" /I "XMath" /I "iScreen" /I "iScreen\SCRIPTS" /I "FX" /I "SOUND" /I "Network" /I "VisGeneric\SCRIPTS" /I "VisGeneric" /I "IVisGeneric" /I "VisGeneric\Graph3d" /I "VisGeneric\Util" /I "Xreal\Joystick" /I "Runtime\Scripts" /I "Xreal\TnT" /I "VisGeneric\md3d" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MECHOSOMA_" /Yu"StdAfx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"MechoSoma.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wininet.lib PlayMpegDBG.lib xtoolDBG.lib xgraphDBG.lib aci_parserDBG.lib _xsoundDBG.lib xreal\qhull\qhull.lib prmedit.lib ftkvc50.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ddraw.lib dxguid.lib winmm.lib dsound.lib wsock32.lib jpeg.lib dinput.lib strmbase.lib /nologo /subsystem:windows /pdb:"MechoSomaDbg.pdb" /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"gameDbg.exe"
# SUBTRACT LINK32 /profile /pdb:none /incremental:no /map

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MechoSoma___Win32_Final"
# PROP BASE Intermediate_Dir "MechoSoma___Win32_Final"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\Garbage/MECHOSOMA/Final"
# PROP Intermediate_Dir "\Garbage/MECHOSOMA/Final"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /W3 /Gi /GR /GX /O2 /I "RUNTIME" /I "Terra" /I "Xreal" /I "XMath" /I "iScreen" /I "iScreen\SCRIPTS" /I "FX" /I "SOUND" /I "Network" /I "VisGeneric\SCRIPTS" /I "VisGeneric" /I "IVisGeneric" /I "VisGeneric\Graph3d" /I "VisGeneric\Util" /I "Xreal\Joystick" /I "Runtime\Scripts" /I "Xreal\TnT" /D "_MECHOSOMA_" /D "_WINDOWS" /D "NDEBUG" /D "WIN32" /Yu"StdAfx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G6 /W3 /GR /GX /O2 /I "RUNTIME" /I "Terra" /I "Xreal" /I "XMath" /I "iScreen" /I "iScreen\SCRIPTS" /I "FX" /I "SOUND" /I "Network" /I "VisGeneric\SCRIPTS" /I "VisGeneric" /I "IVisGeneric" /I "VisGeneric\Graph3d" /I "VisGeneric\Util" /I "Xreal\Joystick" /I "Runtime\Scripts" /I "Xreal\TnT" /I "VisGeneric\md3d" /D "_MECHOSOMA_" /D "_WINDOWS" /D "NDEBUG" /D "WIN32" /D "_FINAL_VERSION_" /Yu"StdAfx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"MechoSoma.bsc"
# ADD BSC32 /nologo /o"MechoSoma.bsc"
LINK32=link.exe
# ADD BASE LINK32 xtool.lib xgraph.lib aci_parser.lib _xsound.lib md3d.lib xreal\qhull\qhull.lib ftkvc50.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ddraw.lib dxguid.lib winmm.lib dsound.lib wsock32.lib jpeg.lib dinput.lib xreal\prmedit.lib /nologo /subsystem:windows /pdb:none /map:"mechosoma.map" /machine:I386 /out:"MechoSoma.exe"
# SUBTRACT BASE LINK32 /profile /debug
# ADD LINK32 wininet.lib PlayMpeg.lib xtool.lib xgraph.lib aci_parser.lib _xsound.lib ftkvc50.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib ddraw.lib dxguid.lib winmm.lib dsound.lib wsock32.lib jpeg.lib dinput.lib strmbase.lib /nologo /subsystem:windows /pdb:none /map:"mechosoma.map" /machine:I386 /out:"game.exe"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "MechoSoma - Win32 Release"
# Name "MechoSoma - Win32 Debug"
# Name "MechoSoma - Win32 Final"
# Begin Group "Xreal"

# PROP Default_Filter ""
# Begin Group "Kernel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\BaseObject.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\BodyDispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\BodyDispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\BodyFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\BodyFactory.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Dynamics.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\LCP_ContactSolver.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\LCP_ContactSolver.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\LCP_frictional.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\LCP_Solver.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\LCP_Solver.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Xreal.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Xreal.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Xreal_utl.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Xreal_utl.h
# End Source File
# End Group
# Begin Group "Collision Detection"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\BodyFeatureHT.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Contact.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Contact.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Grid2D.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\MultiBodyCollision.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\MultibodyContact.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Polyhedron.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Polyhedron.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\PolyhedronClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\PolyhedronLibrary.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\PolyhedronLibrary.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\POLYTREE.CPP
# End Source File
# Begin Source File

SOURCE=.\Xreal\QHull\qhull_io.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\SimpleClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\SimpleClip.h
# End Source File
# End Group
# Begin Group "Application"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\AnimatedBody.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\ArcaneStatistics.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\ControlledObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\ControlledObject.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\CrawlingObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\CrawlingObject.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\FirePoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\FirePoint.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\FlyingObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\FlyingObject.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\GrowingBody.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\GrowingBody.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\HydrophobicObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\HydrophobicObject.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\OwnerProtection.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\PointControlledObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\PointControlledObject.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\SimpleTrack.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\TrackIterator.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\TrackIterator.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\TrackSpline.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\TrackSpline.h
# End Source File
# End Group
# Begin Group "Body"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\Archimed.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Body.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Body.h
# End Source File
# End Group
# Begin Group "Mechos"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\ArcansOfMechos.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Mechos.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Mechos.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\MechosEffects.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Part.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Part.h
# End Source File
# End Group
# Begin Group "Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\Ball.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Ball.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\BonusMan.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\BonusMan.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Brevno.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Brevno.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Bubble.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Bubble.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Butterfly.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Butterfly.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Cannon.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Cannon.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Clown.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Clown.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Crystal.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Crystal.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Debris.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Debris.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Dog.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Dragon.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Dragon.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\EvilEye.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\EvilEye.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\FixedPathBody.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\FixedPathBody.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Frog.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Frog.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\LittleSpider.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\LittleSpider.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\MechosForSale.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\MechosForSale.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Meteorite.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Meteorite.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Penguin.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Pig.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Pig.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Shark.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Ship.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Ship.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\SmartMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\SmartMissile.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\SmartStone.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\SmartStone.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Spider.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Spider.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\StaticBonus.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\StaticBonus.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Stone.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Stone.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Suicide.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Suicide.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\TitaniumBall.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\TitaniumBall.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Tomato.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Tomato.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Whale.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Whale.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Worm.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Worm.h
# End Source File
# End Group
# Begin Group "Worlds"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\Forest_world.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Moutain_world.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Water_world.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\WorldDispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\XrealWorlds.h
# End Source File
# End Group
# Begin Group "Fields"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\CustomFieldSources.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\CustomFieldSources.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\FieldGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\FieldGrid.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\FieldSource.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\FieldSource.h
# End Source File
# End Group
# Begin Group "Camera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\CameraClipping.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\CameraDispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\CameraDispatcher.h
# End Source File
# End Group
# Begin Group "Joystick"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\Joystick\XJoystick.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Joystick\XJoystick.h
# End Source File
# End Group
# Begin Group "TnT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\TnT\FMAT.H
# End Source File
# Begin Source File

SOURCE=.\Xreal\TnT\QR.H
# End Source File
# Begin Source File

SOURCE=.\Xreal\TnT\SUBSCRPT.H
# End Source File
# Begin Source File

SOURCE=.\Xreal\TnT\TNT.H
# End Source File
# Begin Source File

SOURCE=.\Xreal\TnT\TNTMATH.H
# End Source File
# Begin Source File

SOURCE=.\Xreal\TnT\VEC.H
# End Source File
# End Group
# Begin Group "Utils"

# PROP Default_Filter ""
# Begin Group "Statistics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\AllocationTracking.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\AllocationTracking.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\StatisticalAnalizator.h
# End Source File
# End Group
# Begin Group "MathUtils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\CycledMath.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Line2D.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\RungeKutta4.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Xreal\Handle.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\MemoryHeap.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\MemoryHeap.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\ResourceDispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\ResourceDispatcher.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\SST_reader.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\SynchroTimer.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Timers.h
# End Source File
# End Group
# Begin Group "Afx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# Begin Source File

SOURCE=.\Xreal\StdAfx.h
# End Source File
# End Group
# Begin Group "Prms"

# PROP Default_Filter "*.prm"
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\AnimalArcansPrm.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\AnimalsPrm.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\ArcansPrm.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\CameraPrm.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\DebugPrm.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\MechosPrm.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\Params.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Params.h
# End Source File
# Begin Source File

SOURCE=.\Xreal\PersonageDifferetiationPrm.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Scripts\Xreal\AnimalArcans.prm

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Release
InputPath=.\Scripts\Xreal\AnimalArcans.prm
InputName=AnimalArcans

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Debug
InputPath=.\Scripts\Xreal\AnimalArcans.prm
InputName=AnimalArcans

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Final
InputPath=.\Scripts\Xreal\AnimalArcans.prm
InputName=AnimalArcans

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\Xreal\Animals.prm

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Release
InputPath=.\Scripts\Xreal\Animals.prm
InputName=Animals

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Debug
InputPath=.\Scripts\Xreal\Animals.prm
InputName=Animals

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Final
InputPath=.\Scripts\Xreal\Animals.prm
InputName=Animals

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\Xreal\Arcans.prm

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Release
InputPath=.\Scripts\Xreal\Arcans.prm
InputName=Arcans

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Debug
InputPath=.\Scripts\Xreal\Arcans.prm
InputName=Arcans

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Final
InputPath=.\Scripts\Xreal\Arcans.prm
InputName=Arcans

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\Xreal\Camera.prm

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Release
InputPath=.\Scripts\Xreal\Camera.prm
InputName=Camera

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Debug
InputPath=.\Scripts\Xreal\Camera.prm
InputName=Camera

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Final
InputPath=.\Scripts\Xreal\Camera.prm
InputName=Camera

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\Xreal\Debug.prm

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Release
InputPath=.\Scripts\Xreal\Debug.prm
InputName=Debug

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Debug
InputPath=.\Scripts\Xreal\Debug.prm
InputName=Debug

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Final
InputPath=.\Scripts\Xreal\Debug.prm
InputName=Debug

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\Xreal\Game.prm

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Release
InputPath=.\Scripts\Xreal\Game.prm
InputName=Game

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Debug
InputPath=.\Scripts\Xreal\Game.prm
InputName=Game

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Final
InputPath=.\Scripts\Xreal\Game.prm
InputName=Game

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\Xreal\Mechos.prm

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Release
InputPath=.\Scripts\Xreal\Mechos.prm
InputName=Mechos

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Debug
InputPath=.\Scripts\Xreal\Mechos.prm
InputName=Mechos

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Final
InputPath=.\Scripts\Xreal\Mechos.prm
InputName=Mechos

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\Xreal\PersonageDifferentiation.prm

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Release
InputPath=.\Scripts\Xreal\PersonageDifferentiation.prm
InputName=PersonageDifferentiation

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Debug
InputPath=.\Scripts\Xreal\PersonageDifferentiation.prm
InputName=PersonageDifferentiation

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Final
InputPath=.\Scripts\Xreal\PersonageDifferentiation.prm
InputName=PersonageDifferentiation

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Scripts\Xreal\Xreal.prm

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Release
InputPath=.\Scripts\Xreal\Xreal.prm
InputName=Xreal

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Debug
InputPath=.\Scripts\Xreal\Xreal.prm
InputName=Xreal

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# Begin Custom Build
IntDir=\Garbage/MECHOSOMA/Final
InputPath=.\Scripts\Xreal\Xreal.prm
InputName=Xreal

"$(IntDir)\$(InputName).bpr" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	XPrm.exe Scripts\Xreal\$(InputName).prm /Fo$(IntDir)\$(InputName).bpr

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "FastCar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Xreal\CollisionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\CollisionHandler.hpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\VectorOperations.inl
# End Source File
# Begin Source File

SOURCE=.\Xreal\VectorOperationsAligned.inl
# End Source File
# Begin Source File

SOURCE=.\Xreal\Vitamina.cpp
# End Source File
# Begin Source File

SOURCE=.\Xreal\Vitamina.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Arcans.txt
# End Source File
# Begin Source File

SOURCE=.\VisGeneric\Scripts\M3D_Xreal.scr
# End Source File
# Begin Source File

SOURCE=.\MyNotes.txt
# End Source File
# Begin Source File

SOURCE=.\SurMapObjects.txt
# End Source File
# End Group
# Begin Group "RunTime"

# PROP Default_Filter ""
# Begin Group "SpriteDispatcher"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Runtime\SpriteDispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime\SpriteDispatcher.h
# End Source File
# End Group
# Begin Group "Setup"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RUNTIME\setup.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime\xINI.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime\xINI_File.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime\xINI_File.h
# End Source File
# End Group
# Begin Group "Arcanes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RUNTIME\arcane.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\arcane.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\arcane_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\arcane_menu.h
# End Source File
# Begin Source File

SOURCE=.\Runtime\arcane_menu_d3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime\arcane_menu_d3d.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\ArcaneDefine.h
# End Source File
# End Group
# Begin Group "Race"

# PROP Default_Filter ""
# Begin Group "Scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RUNTIME\SCRIPTS\mch_script.h
# End Source File
# Begin Source File

SOURCE=.\Runtime\Scripts\mch_script.set
# End Source File
# End Group
# Begin Source File

SOURCE=.\Runtime\ai_arcane.h
# End Source File
# Begin Source File

SOURCE=.\Runtime\ai_arcane_prm.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\ctl_point.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\ctl_point.h
# End Source File
# Begin Source File

SOURCE=.\Runtime\parts_pool.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime\parts_pool.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\race.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\race.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\track.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\track.h
# End Source File
# End Group
# Begin Group "Runtime_"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RUNTIME\demo_dispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\demo_dispatcher.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\intro.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\intro.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\LocalVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\LocalVersion.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\mch_list.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\mch_rto.H
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\resource.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\timer.cpp
# End Source File
# End Group
# Begin Group "Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RUNTIME\cdcheck.h
# End Source File
# Begin Source File

SOURCE=.\Runtime\Chtree.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime\Chtree.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\fcontainer.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\fcontainer.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\KEYS.CPP
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\KEYS.H
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\tga.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\TGA.H
# End Source File
# Begin Source File

SOURCE=.\Runtime\TrackRecorder.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime\TrackRecorder.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\vsetup.cpp
# End Source File
# End Group
# Begin Group "TrackDispatcher"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Runtime\Scripts\mch_tracks.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__MCH_T="RUNTIME\Scripts\mch_script.set"	"RUNTIME\Scripts\mch_script.h"	"iScreen\Scripts\aci_str.inc"	
# Begin Custom Build - mch_tracks.scr
ProjDir=.
InputPath=.\Runtime\Scripts\mch_tracks.scr
InputName=mch_tracks

".\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr mch_script.set -ifRUNTIME\SCRIPTS\  -ofRESOURCE\ -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__MCH_T="RUNTIME\Scripts\mch_script.set"	"RUNTIME\Scripts\mch_script.h"	"iScreen\Scripts\aci_str.inc"	
# Begin Custom Build - mch_tracks.scr
ProjDir=.
InputPath=.\Runtime\Scripts\mch_tracks.scr
InputName=mch_tracks

".\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr mch_script.set -ifRUNTIME\SCRIPTS\  -ofRESOURCE\ -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__MCH_T="RUNTIME\Scripts\mch_script.set"	"RUNTIME\Scripts\mch_script.h"	"iScreen\Scripts\aci_str.inc"	
# Begin Custom Build - mch_tracks.scr
ProjDir=.
InputPath=.\Runtime\Scripts\mch_tracks.scr
InputName=mch_tracks

".\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr mch_script.set -ifRUNTIME\SCRIPTS\  -ofRESOURCE\ -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Runtime\TrackDispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Runtime\TrackDispatcher.h
# End Source File
# End Group
# Begin Group "OnlineGame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RUNTIME\og_buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\online_game.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\online_game.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\wininet_api.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\wininet_api.h
# End Source File
# End Group
# Begin Group "IntroMovie"

# PROP Default_Filter ""
# Begin Group "imScripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RUNTIME\Scripts\intro_movie.inc
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\Scripts\intro_movie.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__INTRO="Runtime\Scripts\mch_script.set"	"Runtime\Scripts\mch_script.h"	"Runtime\intro_movie_id.h"	"Runtime\Scripts\intro_movie.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\RUNTIME\Scripts\intro_movie.scr
InputName=intro_movie

".\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr mch_script.set -ifRUNTIME\SCRIPTS\  -ofRESOURCE\ -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__INTRO="Runtime\Scripts\mch_script.set"	"Runtime\Scripts\mch_script.h"	"Runtime\intro_movie_id.h"	"Runtime\Scripts\intro_movie.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\RUNTIME\Scripts\intro_movie.scr
InputName=intro_movie

".\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr mch_script.set -ifRUNTIME\SCRIPTS\  -ofRESOURCE\ -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__INTRO="Runtime\Scripts\mch_script.set"	"Runtime\Scripts\mch_script.h"	"Runtime\intro_movie_id.h"	"Runtime\Scripts\intro_movie.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\RUNTIME\Scripts\intro_movie.scr
InputName=intro_movie

".\Resource\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr mch_script.set -ifRUNTIME\SCRIPTS\  -ofRESOURCE\ -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\RUNTIME\intro_movie.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\intro_movie.h
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\intro_movie_id.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\RUNTIME\mechosoma.cpp
# End Source File
# Begin Source File

SOURCE=.\RUNTIME\mechosoma.h
# End Source File
# End Group
# Begin Group "XMath"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XMath\XMath.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\XMath\xmath.h
# End Source File
# End Group
# Begin Group "FX"

# PROP Default_Filter ""
# Begin Group "FXScripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Fx\Script\Action.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\BallWind.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Bonus.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\BubbleRush.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Charge.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\CheckPoint.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Clown.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Dust.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Electric.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Explosive.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\FireBall.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\FireRose.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\FireStone.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\FireWorld.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\ForestWorld.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\FuryFungus.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\fxlabScript.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__FXLAB="FX\Script\Action.inc"	"FX\Script\BallWind.inc"	"FX\Script\Bonus.inc"	"FX\Script\BubbleRush.inc"	"FX\Script\Charge.inc"	"FX\Script\Dust.inc"	"FX\Script\Electric.inc"	"FX\Script\Explosive.inc"	"FX\Script\FireBall.inc"	"FX\Script\FireRose.inc"	"FX\Script\FireStone.inc"	"FX\Script\FireWorld.inc"	"FX\Script\ForestWorld.inc"	"FX\Script\FuryFungus.inc"	"FX\Script\Kinetic.inc"	"FX\Script\LightningBolt.inc"	"FX\Script\Mine.inc"	"FX\Script\MountainWorld.inc"	"FX\Script\Petard.inc"	"FX\Script\PlasmaClaw.inc"	"FX\Script\Protection.inc"	"FX\Script\Revolt.inc"	"FX\Script\ScarletTrace.inc"	"FX\Script\Shaft.inc"	"FX\Script\Skin.inc"	"FX\Script\Slime.inc"	"FX\Script\SnowFall.inc"	"FX\Script\Spike.inc"	"FX\Script\Storm.inc"	"FX\Script\Suicide.inc"	"FX\Script\Teleportation.inc"	"FX\Script\Tomato.inc"	"FX\Script\Volcano.inc"	"FX\Script\Voodoo.inc"	"FX\Script\WaterWorld.inc"	"FX\Script\CheckPoint.inc"	"FX\Script\Clown.inc"	"FX\Script\WorldIrradiate.inc"	"Sound\snd_ids.h"	"FX\fxlabID.h"	"FX\Script\Movies.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\Fx\Script\fxlabScript.scr
InputName=fxlabScript

".\Resource\FX\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr fxlabScript.set -ifFX\SCRIPT\  -ofRESOURCE\FX\     -out$(InputName).scb /q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__FXLAB="FX\Script\Action.inc"	"FX\Script\BallWind.inc"	"FX\Script\Bonus.inc"	"FX\Script\BubbleRush.inc"	"FX\Script\Charge.inc"	"FX\Script\Dust.inc"	"FX\Script\Electric.inc"	"FX\Script\Explosive.inc"	"FX\Script\FireBall.inc"	"FX\Script\FireRose.inc"	"FX\Script\FireStone.inc"	"FX\Script\FireWorld.inc"	"FX\Script\ForestWorld.inc"	"FX\Script\FuryFungus.inc"	"FX\Script\Kinetic.inc"	"FX\Script\LightningBolt.inc"	"FX\Script\Mine.inc"	"FX\Script\MountainWorld.inc"	"FX\Script\Petard.inc"	"FX\Script\PlasmaClaw.inc"	"FX\Script\Protection.inc"	"FX\Script\Revolt.inc"	"FX\Script\ScarletTrace.inc"	"FX\Script\Shaft.inc"	"FX\Script\Skin.inc"	"FX\Script\Slime.inc"	"FX\Script\SnowFall.inc"	"FX\Script\Spike.inc"	"FX\Script\Storm.inc"	"FX\Script\Suicide.inc"	"FX\Script\Teleportation.inc"	"FX\Script\Tomato.inc"	"FX\Script\Volcano.inc"	"FX\Script\Voodoo.inc"	"FX\Script\WaterWorld.inc"	"FX\Script\CheckPoint.inc"	"FX\Script\Clown.inc"	"FX\Script\WorldIrradiate.inc"	"Sound\snd_ids.h"	"FX\fxlabID.h"	"FX\Script\Movies.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\Fx\Script\fxlabScript.scr
InputName=fxlabScript

".\Resource\FX\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr fxlabScript.set -ifFX\SCRIPT\  -ofRESOURCE\FX\     -out$(InputName).scb /q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__FXLAB="FX\Script\Action.inc"	"FX\Script\BallWind.inc"	"FX\Script\Bonus.inc"	"FX\Script\BubbleRush.inc"	"FX\Script\Charge.inc"	"FX\Script\Dust.inc"	"FX\Script\Electric.inc"	"FX\Script\Explosive.inc"	"FX\Script\FireBall.inc"	"FX\Script\FireRose.inc"	"FX\Script\FireStone.inc"	"FX\Script\FireWorld.inc"	"FX\Script\ForestWorld.inc"	"FX\Script\FuryFungus.inc"	"FX\Script\Kinetic.inc"	"FX\Script\LightningBolt.inc"	"FX\Script\Mine.inc"	"FX\Script\MountainWorld.inc"	"FX\Script\Petard.inc"	"FX\Script\PlasmaClaw.inc"	"FX\Script\Protection.inc"	"FX\Script\Revolt.inc"	"FX\Script\ScarletTrace.inc"	"FX\Script\Shaft.inc"	"FX\Script\Skin.inc"	"FX\Script\Slime.inc"	"FX\Script\SnowFall.inc"	"FX\Script\Spike.inc"	"FX\Script\Storm.inc"	"FX\Script\Suicide.inc"	"FX\Script\Teleportation.inc"	"FX\Script\Tomato.inc"	"FX\Script\Volcano.inc"	"FX\Script\Voodoo.inc"	"FX\Script\WaterWorld.inc"	"FX\Script\CheckPoint.inc"	"FX\Script\Clown.inc"	"FX\Script\WorldIrradiate.inc"	"Sound\snd_ids.h"	"FX\fxlabID.h"	"FX\Script\Movies.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\Fx\Script\fxlabScript.scr
InputName=fxlabScript

".\Resource\FX\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr fxlabScript.set -ifFX\SCRIPT\  -ofRESOURCE\FX\     -out$(InputName).scb /q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Fx\Script\fxlabScript.set
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\fxlabWorld.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

# Begin Custom Build
ProjDir=.
InputPath=.\Fx\Script\fxlabWorld.scr
InputName=fxlabWorld

".\Resource\FX\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr fxlabScript.set -ifFX\SCRIPT\  -ofRESOURCE\FX\     -out$(InputName).scb /q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

# Begin Custom Build
ProjDir=.
InputPath=.\Fx\Script\fxlabWorld.scr
InputName=fxlabWorld

".\Resource\FX\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr fxlabScript.set -ifFX\SCRIPT\  -ofRESOURCE\FX\     -out$(InputName).scb /q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

# Begin Custom Build
ProjDir=.
InputPath=.\Fx\Script\fxlabWorld.scr
InputName=fxlabWorld

".\Resource\FX\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr fxlabScript.set -ifFX\SCRIPT\  -ofRESOURCE\FX\     -out$(InputName).scb /q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Kinetic.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\LightningBolt.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Mine.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\MountainWorld.inc
# End Source File
# Begin Source File

SOURCE=.\FX\Script\Movies.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Petard.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\PlasmaClaw.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Protection.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Revolt.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\ScarletTrace.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Shaft.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Skin.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Slime.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\SnowFall.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Spike.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Storm.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Suicide.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Teleportation.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Tomato.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Volcano.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\Voodoo.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\WaterWorld.inc
# End Source File
# Begin Source File

SOURCE=.\Fx\Script\WorldIrradiate.inc
# End Source File
# End Group
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Fx\fxlabKey.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabKey.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabResource.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabResource.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabSprite.h
# End Source File
# End Group
# Begin Group "Server"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Fx\fxlabServerArcane.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabServerArcane.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabServerGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabServerGeneral.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabServerStuff.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabServerStuff.h
# End Source File
# End Group
# Begin Group "Client"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Fx\fxlabClientEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientEffect.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientGeneral.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientLauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientLauncher.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientLightning.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientLightning.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientModel.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientModel.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientSound.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientSound.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientStuff.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabClientStuff.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Fx\fxlabApplication.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabBodyInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabBodyInterface.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabGeneral.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabID.h
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\Fx\fxlabInterface.h
# End Source File
# Begin Source File

SOURCE=.\Fx\WIN32F.CPP
# End Source File
# Begin Source File

SOURCE=.\Fx\WIN32F.H
# End Source File
# End Group
# End Group
# Begin Group "Terra"

# PROP Default_Filter ""
# Begin Group "FxSource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Terra\ch_points.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\JPEG_FM.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\LAND.CPP
# End Source File
# Begin Source File

SOURCE=.\Terra\non_rotate_scan_min_full.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\siderend.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\sur_scr.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\tgai.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\tools.cpp
# End Source File
# Begin Source File

SOURCE=.\Terra\VMAP.CPP
# End Source File
# End Group
# Begin Group "Include Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Terra\ch_points.h
# End Source File
# Begin Source File

SOURCE=.\Terra\points.h
# End Source File
# Begin Source File

SOURCE=.\Terra\RENDER.H
# End Source File
# Begin Source File

SOURCE=.\Terra\terra.h
# End Source File
# Begin Source File

SOURCE=.\Terra\tools.h
# End Source File
# Begin Source File

SOURCE=.\Terra\VMAP.H
# End Source File
# End Group
# End Group
# Begin Group "Sound"

# PROP Default_Filter ""
# Begin Group "SoundSource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sound\sound.cpp
# ADD CPP /Yu
# End Source File
# Begin Source File

SOURCE=.\Sound\sound_api.cpp
# End Source File
# End Group
# Begin Group "SoundHeaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sound\snd_cd.h
# End Source File
# Begin Source File

SOURCE=.\Sound\snd_def.h
# End Source File
# Begin Source File

SOURCE=.\Sound\snd_ids.h
# End Source File
# Begin Source File

SOURCE=.\Sound\sound.h
# End Source File
# Begin Source File

SOURCE=.\Sound\sound_api.h
# End Source File
# End Group
# Begin Group "SoundScripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Sound\SCRIPTS\COMMON.INC
# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\forestbegin_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__FORES="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\forestbegin_snd.scr
InputName=forestbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__FORES="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\forestbegin_snd.scr
InputName=forestbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__FORES="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\forestbegin_snd.scr
InputName=forestbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\forestvictory_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__FOREST="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\forestvictory_snd.scr
InputName=forestvictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__FOREST="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\forestvictory_snd.scr
InputName=forestvictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__FOREST="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\forestvictory_snd.scr
InputName=forestvictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\gamevictory_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__GAMEV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\gamevictory_snd.scr
InputName=gamevictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__GAMEV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\gamevictory_snd.scr
InputName=gamevictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__GAMEV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\gamevictory_snd.scr
InputName=gamevictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\intromovie_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__INTROM="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\intromovie_snd.scr
InputName=intromovie_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__INTROM="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\intromovie_snd.scr
InputName=intromovie_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__INTROM="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\intromovie_snd.scr
InputName=intromovie_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\lavabegin_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__LAVAB="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\lavabegin_snd.scr
InputName=lavabegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__LAVAB="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\lavabegin_snd.scr
InputName=lavabegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__LAVAB="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\lavabegin_snd.scr
InputName=lavabegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\lavavictory_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__LAVAV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\lavavictory_snd.scr
InputName=lavavictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__LAVAV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\lavavictory_snd.scr
InputName=lavavictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__LAVAV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\lavavictory_snd.scr
InputName=lavavictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\logo_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__LOGO_="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\logo_snd.scr
InputName=logo_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__LOGO_="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\logo_snd.scr
InputName=logo_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__LOGO_="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\logo_snd.scr
InputName=logo_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\MCH_SND.SCR

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__MCH_S="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\MCH_SND.SCR
InputName=MCH_SND

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__MCH_S="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\MCH_SND.SCR
InputName=MCH_SND

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__MCH_S="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\MCH_SND.SCR
InputName=MCH_SND

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\mountainbegin_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__MOUNT="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\mountainbegin_snd.scr
InputName=mountainbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__MOUNT="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\mountainbegin_snd.scr
InputName=mountainbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__MOUNT="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\mountainbegin_snd.scr
InputName=mountainbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\mountainvictory_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__MOUNTA="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\mountainvictory_snd.scr
InputName=mountainvictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__MOUNTA="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\mountainvictory_snd.scr
InputName=mountainvictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__MOUNTA="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\mountainvictory_snd.scr
InputName=mountainvictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\Scripts\sound.set
# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\watchbegin_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WATCH="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\watchbegin_snd.scr
InputName=watchbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WATCH="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\watchbegin_snd.scr
InputName=watchbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WATCH="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\watchbegin_snd.scr
InputName=watchbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\watchvictory_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WATCHV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\watchvictory_snd.scr
InputName=watchvictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WATCHV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\watchvictory_snd.scr
InputName=watchvictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WATCHV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\watchvictory_snd.scr
InputName=watchvictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\waterbegin_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WATER="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\waterbegin_snd.scr
InputName=waterbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WATER="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\waterbegin_snd.scr
InputName=waterbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WATER="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\waterbegin_snd.scr
InputName=waterbegin_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\SCRIPTS\watervictory_snd.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WATERV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\watervictory_snd.scr
InputName=watervictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WATERV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\watervictory_snd.scr
InputName=watervictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WATERV="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\SCRIPTS\watervictory_snd.scr
InputName=watervictory_snd

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\Scripts\world00.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WORLD="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world00.scr
InputName=world00

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WORLD="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world00.scr
InputName=world00

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WORLD="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world00.scr
InputName=world00

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\Scripts\world01.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WORLD0="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world01.scr
InputName=world01

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WORLD0="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world01.scr
InputName=world01

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WORLD0="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world01.scr
InputName=world01

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\Scripts\world02.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WORLD02="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world02.scr
InputName=world02

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WORLD02="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world02.scr
InputName=world02

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WORLD02="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world02.scr
InputName=world02

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\Scripts\world03.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WORLD03="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world03.scr
InputName=world03

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WORLD03="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world03.scr
InputName=world03

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WORLD03="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world03.scr
InputName=world03

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\Scripts\world04.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WORLD04="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world04.scr
InputName=world04

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WORLD04="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world04.scr
InputName=world04

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WORLD04="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world04.scr
InputName=world04

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\Scripts\world07.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WORLD07="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world07.scr
InputName=world07

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WORLD07="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world07.scr
InputName=world07

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WORLD07="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world07.scr
InputName=world07

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sound\Scripts\world08.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__WORLD08="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world08.scr
InputName=world08

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__WORLD08="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world08.scr
InputName=world08

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__WORLD08="Sound\snd_ids.h"	"Sound\snd_def.h"	"Sound\Scripts\common.inc"	"Sound\Scripts\sound.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\Sound\Scripts\world08.scr
InputName=world08

"Resource\Sound\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr sound.set -ifSound\Scripts\ -ofResource\Sound\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "iScreen"

# PROP Default_Filter ""
# Begin Group "iSource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\iScreen\Acs_core.cpp
# End Source File
# Begin Source File

SOURCE=.\iScreen\Acs_ext.cpp
# End Source File
# Begin Source File

SOURCE=.\iScreen\acs_quant.cpp
# End Source File
# Begin Source File

SOURCE=.\iScreen\hbm.cpp
# End Source File
# Begin Source File

SOURCE=.\iScreen\Hfont.cpp
# End Source File
# Begin Source File

SOURCE=".\iScreen\I-World.cpp"
# End Source File
# Begin Source File

SOURCE=.\iScreen\iScript.cpp
# End Source File
# Begin Source File

SOURCE=.\iScreen\iText.cpp
# End Source File
# Begin Source File

SOURCE=.\iScreen\savegame.cpp
# End Source File
# End Group
# Begin Group "iHeaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\iScreen\ACI_EVNT.H
# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\ACI_IDS.H
# End Source File
# Begin Source File

SOURCE=.\iScreen\ACI_SCR.H
# End Source File
# Begin Source File

SOURCE=.\iScreen\controls.h
# End Source File
# Begin Source File

SOURCE=.\iScreen\doggy.h
# End Source File
# Begin Source File

SOURCE=.\iScreen\hbm.h
# End Source File
# Begin Source File

SOURCE=.\iScreen\HFONT.H
# End Source File
# Begin Source File

SOURCE=".\iScreen\I-World.h"
# End Source File
# Begin Source File

SOURCE=".\iScreen\I-WorldEvent.h"
# End Source File
# Begin Source File

SOURCE=.\iScreen\iText.h
# End Source File
# Begin Source File

SOURCE=.\iScreen\iTextID.h
# End Source File
# Begin Source File

SOURCE=.\iScreen\savegame.h
# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\SCR_DEFS.H
# End Source File
# End Group
# Begin Group "iScripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\ACI_STR.INC
# End Source File
# Begin Source File

SOURCE=.\iScreen\Scripts\acs_scr.set
# End Source File
# Begin Source File

SOURCE=.\iScreen\Scripts\Arcane.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__ARCAN="iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\scr_defs.h"	"Runtime\AI_Arcane_Prm.h"	"Runtime\ArcaneDefine.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\Scripts\Arcane.scr
InputName=Arcane

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__ARCAN="iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\scr_defs.h"	"Runtime\AI_Arcane_Prm.h"	"Runtime\ArcaneDefine.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\Scripts\Arcane.scr
InputName=Arcane

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__ARCAN="iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\scr_defs.h"	"Runtime\AI_Arcane_Prm.h"	"Runtime\ArcaneDefine.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\Scripts\Arcane.scr
InputName=Arcane

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\COMMON.INC
# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\doggy.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__DOGGY="iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\scr_defs.h"	"iScreen\doggy.h"	"iScreen\Scripts\aci_str.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\SCRIPTS\doggy.scr
InputName=doggy

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__DOGGY="iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\scr_defs.h"	"iScreen\doggy.h"	"iScreen\Scripts\aci_str.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\SCRIPTS\doggy.scr
InputName=doggy

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__DOGGY="iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\scr_defs.h"	"iScreen\doggy.h"	"iScreen\Scripts\aci_str.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\SCRIPTS\doggy.scr
InputName=doggy

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\iScreen\Scripts\I-World.inc"
# End Source File
# Begin Source File

SOURCE=.\iScreen\Scripts\iscreen.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__ISCRE="iScreen\aci_evnt.h"	"iScreen\controls.h"	"iScreen\I-WorldEvent.h"	"iScreen\Scripts\scr_defs.h"	"iScreen\Scripts\aci_ids.h"	"iScreen\Scripts\common.inc"	"iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\vkeys.inc"	"iScreen\Scripts\aci_str.inc"	"iScreen\Scripts\I-World.inc"	"iScreen\Scripts\iwAssemblyObj.inc"	"iScreen\Scripts\iwNameScr.inc"	"iScreen\Scripts\iwElectionaryScr.inc"	"iScreen\Scripts\options.inc"	"iScreen\Scripts\mainmenu.inc"	"Sound\snd_ids.h"	"iScreen\Scripts\player_cfg.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\Scripts\iscreen.scr
InputName=iscreen

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__ISCRE="iScreen\aci_evnt.h"	"iScreen\controls.h"	"iScreen\I-WorldEvent.h"	"iScreen\Scripts\scr_defs.h"	"iScreen\Scripts\aci_ids.h"	"iScreen\Scripts\common.inc"	"iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\vkeys.inc"	"iScreen\Scripts\aci_str.inc"	"iScreen\Scripts\I-World.inc"	"iScreen\Scripts\iwAssemblyObj.inc"	"iScreen\Scripts\iwNameScr.inc"	"iScreen\Scripts\iwElectionaryScr.inc"	"iScreen\Scripts\options.inc"	"iScreen\Scripts\mainmenu.inc"	"Sound\snd_ids.h"	"iScreen\Scripts\player_cfg.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\Scripts\iscreen.scr
InputName=iscreen

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__ISCRE="iScreen\aci_evnt.h"	"iScreen\controls.h"	"iScreen\I-WorldEvent.h"	"iScreen\Scripts\scr_defs.h"	"iScreen\Scripts\aci_ids.h"	"iScreen\Scripts\common.inc"	"iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\vkeys.inc"	"iScreen\Scripts\aci_str.inc"	"iScreen\Scripts\I-World.inc"	"iScreen\Scripts\iwAssemblyObj.inc"	"iScreen\Scripts\iwNameScr.inc"	"iScreen\Scripts\iwElectionaryScr.inc"	"iScreen\Scripts\options.inc"	"iScreen\Scripts\mainmenu.inc"	"Sound\snd_ids.h"	"iScreen\Scripts\player_cfg.inc"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\Scripts\iscreen.scr
InputName=iscreen

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\iText.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__ITEXT="iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\aci_str.inc"	"iScreen\iTextID.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\SCRIPTS\iText.scr
InputName=iText

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__ITEXT="iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\aci_str.inc"	"iScreen\iTextID.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\SCRIPTS\iText.scr
InputName=iText

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__ITEXT="iScreen\Scripts\acs_scr.set"	"iScreen\Scripts\aci_str.inc"	"iScreen\iTextID.h"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\SCRIPTS\iText.scr
InputName=iText

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iScreen\Scripts\iwAssemblyObj.inc
# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\iwElectionaryScr.inc
# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\iwNameScr.inc
# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\MAINMENU.INC
# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\OPTIONS.INC
# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\player_cfg.inc
# End Source File
# Begin Source File

SOURCE=.\iScreen\Scripts\sprites.scr

!IF  "$(CFG)" == "MechoSoma - Win32 Release"

USERDEP__SPRIT="iScreen\Scripts\scr_defs.h"	"iScreen\Scripts\acs_scr.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\Scripts\sprites.scr
InputName=sprites

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Debug"

USERDEP__SPRIT="iScreen\Scripts\scr_defs.h"	"iScreen\Scripts\acs_scr.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\Scripts\sprites.scr
InputName=sprites

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "MechoSoma - Win32 Final"

USERDEP__SPRIT="iScreen\Scripts\scr_defs.h"	"iScreen\Scripts\acs_scr.set"	
# Begin Custom Build
ProjDir=.
InputPath=.\iScreen\Scripts\sprites.scr
InputName=sprites

"Resource\iScreen\$(InputName).scb" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	parser $(InputName).scr acs_scr.set -ifiScreen\Scripts\ -ofResource\iScreen\  -q 
	cd $(ProjDir) 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iScreen\SCRIPTS\VKEYS.INC
# End Source File
# End Group
# Begin Source File

SOURCE=.\Resource\Iscreen\iscreen.ini
# End Source File
# End Group
# Begin Group "ini"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource\bots.ini
# End Source File
# Begin Source File

SOURCE=.\Resource\Iscreen\camera.ini
# End Source File
# Begin Source File

SOURCE=.\game.ini
# End Source File
# Begin Source File

SOURCE=.\RESOURCE\ISCREEN\local.ini
# End Source File
# Begin Source File

SOURCE=.\Resource\Options.ini
# End Source File
# End Group
# Begin Source File

SOURCE=.\MechoSoma.rc
# End Source File
# Begin Source File

SOURCE=.\Moonshine.ico
# End Source File
# End Target
# End Project
