# Moonshine Runners Community edition

[![Linux](https://github.com/KD-lab-Open-Source/Samogonki/actions/workflows/linux.yaml/badge.svg)](https://github.com/KD-lab-Open-Source/Samogonki/actions/workflows/linux.yaml)
[![Windows](https://github.com/KD-lab-Open-Source/Samogonki/actions/workflows/windows.yaml/badge.svg)](https://github.com/KD-lab-Open-Source/Samogonki/actions/workflows/windows.yaml)
[![macOS](https://github.com/KD-lab-Open-Source/Samogonki/actions/workflows/macos.yaml/badge.svg)](https://github.com/KD-lab-Open-Source/Samogonki/actions/workflows/macos.yaml)

Original game: Moonshine Runners / Spanking Runners / Самогонки

Developer: LLC "KD VISION" (Kaliningrad) / (с) ООО "КД ВИЖЕН" (Калининград)

All source code except '3rdparty' folder are licensed under **GPLv3**. 
'3rdparty' libraries are licensed under original license of certain library.
The game is compatible with original release from 1C.

Community in telegram: https://t.me/SamogonkiGame

## Where to play

* Windows/Linux/MacOs [Steam](https://store.steampowered.com/app/2599800/_/)
* Web [GamePix](https://www.gamepix.com/play/moonshine-runners-samogonki)

## Building

Look into buildscripts:
* [Linux](https://github.com/KD-lab-Open-Source/Samogonki/blob/cmake/.github/workflows/linux.yaml)
* [Windows](https://github.com/KD-lab-Open-Source/Samogonki/blob/cmake/.github/workflows/windows.yaml)
* [macOS](https://github.com/KD-lab-Open-Source/Samogonki/blob/cmake/.github/workflows/macos.yaml)

### SDL 2
1. Clone
```
git clone --depth 1 --branch release-2.26.5 https://github.com/libsdl-org/SDL.git SDL
```

2. Configurate
```
cmake -B cmake-build-debug -DSDL_SHARED=0 -DSDL_STATIC=1 -DSDL_TEST=0 -DCMAKE_INSTALL_PREFIX=target-debug
```

3. Build
```
cmake --build cmake-build-debug --target install
```

4. Copy
```
mkdir 3rdparty/SDL-2.26.5
cp -r 3rdparty/SDL/target-debug/include 3rdparty/SDL-2.26.5/
cp -r 3rdparty/SDL/target-debug/lib 3rdparty/SDL-2.26.5/
```

## How to run

Copy *moonshine-runners* binary into original folder of the game and run it.
In case of unexpected problems try to unpack *resource.pak* and set *resources=0* in *game.ini*.

## Crash reports

Any crash (access violation/segfault, abort, unhandled C++ exception, fatal
signal) shows a message box with instructions and writes a report to
**XErrH.log** next to the executable (or to the user data directory when the
game folder is read-only).

* **Windows**: unhandled exceptions are caught with `SetUnhandledExceptionFilter`;
  the report contains registers, a stack trace and, when a matching PDB is next
  to the executable, function names with `file(line)`. A minidump
  **crash-*.dmp** is written next to the executable as well.
* **Linux/macOS**: fatal signals (`SIGSEGV`, `SIGBUS`, `SIGFPE`, `SIGILL`,
  `SIGABRT`) and `std::terminate` are reported with a `backtrace()`. The dialog
  is shown through SDL. There is no minidump; use the printed addresses with
  `addr2line` (Linux) or `atos` (macOS) on a build with debug info.

Please attach the log (and the dump, if present) to a GitHub issue:
https://github.com/KD-lab-Open-Source/Samogonki/issues

### Symbolized stack traces (PDB)

The stack trace contains function names and `file(line)` only when a matching
PDB is available next to the executable. MinGW builds carry DWARF debug info,
which is converted to a PDB with cv2pdb:

```
.\scripts\make-pdb.ps1 -Exe build-mingw\MechoSoma\moonshine-runners.exe
```

The script uses `cv2pdb64.exe` from `PATH`/`.tools` or downloads the pinned
release. Build with `-DCMAKE_BUILD_TYPE=RelWithDebInfo` (the Windows CI does
this) and keep `moonshine-runners.pdb` next to `moonshine-runners.exe` when
shipping the build with symbols. PDB files are optional: without them the
report still lists modules and offsets.

### Crash report self-test

Set `MOONSHINE_CRASH_TEST` to `av`, `abort`, `exception` or `stack` to trigger a
test crash at startup, and `MOONSHINE_CRASH_NO_DIALOG=1` to suppress the dialog
(useful for automated checks).

## Suppport

Feel free to open bug report on github or ask directly in game telegram channel
