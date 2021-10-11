@echo off
prep /om /ft /sf ?new_clip@CameraDispatcher@@AAEHAAUCameraCoords@@@Z "MechoSoma.exe"
if errorlevel 1 goto done
profile mechosoma /profile %1
if errorlevel 1 goto done
prep /m mechosoma
if errorlevel 1 goto done
plist /STC mechosoma > profile0.lst
plist /ST  mechosoma > profile1.lst
plist /SC  mechosoma > profile2.lst
:done
