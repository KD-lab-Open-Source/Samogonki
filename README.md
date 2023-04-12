# Moonshine Runners Community edition

[![Linux](https://github.com/KD-lab-Open-Source/Samogonki/workflows/Linux/badge.svg)](https://github.com/KD-lab-Open-Source/Samogonki/actions/workflows/linux.yaml)
[![Windows](https://github.com/KD-lab-Open-Source/Samogonki/workflows/Windows/badge.svg)](https://github.com/KD-lab-Open-Source/Samogonki/actions/workflows/windows.yaml)

Original game: Moonshine Runners / Spanking Runners / Самогонки

Developer: LLC "KD VISION" (Kaliningrad) / (с) ООО "КД ВИЖЕН" (Калининград)

All source code except '3rdparty' folder are licensed under **GPLv3**. 
'3rdparty' libraries are licensed under original license of certain library.
The game is compatible with original release from 1C.

Community in telegram: https://t.me/SamogonkiGame

## Building

Look into buildscripts:
* [Linux](https://github.com/KD-lab-Open-Source/Samogonki/blob/cmake/.github/workflows/linux.yaml)
* [Windows](https://github.com/KD-lab-Open-Source/Samogonki/blob/cmake/.github/workflows/windows.yaml)

### macOS
Download [libjpeg-turbo](https://sourceforge.net/projects/libjpeg-turbo/files/2.1.0/libjpeg-turbo-ios-2.1.0.dmg/download)
and unpack it to `3rdparty/libjpeg-turbo` folder.

## How to run

Copy *moonshine-runners* binary into original folder of the game and run it.
In case of unexpected problems try to unpack *resource.pak* and set *resources=0* in *game.ini*.

## Suppport

Feel free to open bug report on github or ask directly in game telegram channel
