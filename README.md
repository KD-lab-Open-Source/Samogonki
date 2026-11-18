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

### SDL 3
CMake downloads SDL 3 when it configures the game and builds it as a static
library, so there is nothing to install. To build offline, point CMake at an
unpacked SDL 3.4.20 source tree:
```
cmake -B build -DFETCHCONTENT_SOURCE_DIR_SDL3=/path/to/SDL3-3.4.20
```

### Windows
The Windows build needs Visual C++ (Visual Studio 2022 or newer with the C++
workload). CMake also downloads zlib there (`FETCHCONTENT_SOURCE_DIR_ZLIB`
for an offline build), and the runtime is linked in, so the game needs no
Visual C++ redistributable:
```
cmake -B build -A x64
cmake --build build --config RelWithDebInfo
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
PDB is available next to the executable. The linker writes it for a
`RelWithDebInfo` build (the Windows CI builds that), as
`build/MechoSoma/RelWithDebInfo/moonshine-runners.pdb`; keep it next to
`moonshine-runners.exe` when shipping the build with symbols. PDB files are
optional: without them the report still lists modules and offsets.

### Crash report self-test

Set `MOONSHINE_CRASH_TEST` to `av`, `abort`, `exception` or `stack` to trigger a
test crash at startup, and `MOONSHINE_CRASH_NO_DIALOG=1` to suppress the dialog
(useful for automated checks).

## Suppport

Feel free to open bug report on github or ask directly in game telegram channel
