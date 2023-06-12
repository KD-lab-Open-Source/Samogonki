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

### FFmpeg
1. Clone FFmpeg n4.3.6
```
git clone --depth 1 --branch n4.3.6 https://git.ffmpeg.org/ffmpeg.git ffmpeg
```

2. Configurate
```
./configure
	--prefix=build
	--disable-programs
	--disable-doc
	--disable-avdevice
	--disable-avfilter
	--disable-network
	--disable-pixelutils
	--disable-everything
	--enable-decoder=pcm_s16le,mpc7
	--enable-demuxer=wav,mpc
	--enable-protocol=file
```

3. Build and install
```
make
make install
```

4. Copy
```
mkdir 3rdparty/FFmpeg-n4.3.6
cp -r 3rdparty/ffmpeg/build/include 3rdparty/FFmpeg-n4.3.6/
cp -r 3rdparty/ffmpeg/build/lib 3rdparty/FFmpeg-n4.3.6/
```

### macOS
Download [libjpeg-turbo](https://sourceforge.net/projects/libjpeg-turbo/files/2.1.0/libjpeg-turbo-ios-2.1.0.dmg/download)
and unpack it to `3rdparty/libjpeg-turbo` folder.

## How to run

Copy *moonshine-runners* binary into original folder of the game and run it.
In case of unexpected problems try to unpack *resource.pak* and set *resources=0* in *game.ini*.

## Suppport

Feel free to open bug report on github or ask directly in game telegram channel
