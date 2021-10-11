@echo off
set WorkFolder=d:\public\perimeter\

del %WorkFolder%\Game.exe
del \garbage\*.* /S /Q

net use p: /delete
net use p: \\RADIO\G /PERSISTENT:NO
set ssdir=\\RADIO\G\Base.mvss
set ssuser=Builder
P:\Base.mvss\win32\SS.EXE Workfold $/PERIMETER .
P:\Base.mvss\win32\SS.EXE GET *.* -G -R -W -I- -O-

start /wait BuildConsole perimeter.dsw /Prj=Game /cfg="Win32 Release" /Log /NoLogo /ShowAgent /ShowTime /Clean
start /wait BuildConsole perimeter.dsw /Prj=Scripts /cfg="Win32 Release" /NoLogo /ShowAgent
start /wait BuildConsole perimeter.dsw /Prj=Game /cfg="Win32 Release" /NoLogo /ShowAgent

copy Scripts\*.* %WorkFolder%Scripts\ /y
copy Resource\*.btdb %WorkFolder%Resource\ /y

copy *.exe %WorkFolder% /y
copy *.dll %WorkFolder% /y
copy *.pdb %WorkFolder% /y

del *.cpp /S /Q
del *.h /S /Q

set WorkFolder= 
