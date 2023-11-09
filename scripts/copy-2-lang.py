
#!/usr/bin/python3

from sys import argv
from os import unlink, makedirs
from os.path import exists, join, dirname
from shutil import copyfile

land_deps = [
    # "./Resource/intro.mpg",
    "./Resource/iScreen/iText.scb",
    "./Resource/iScreen/iscreen.scb",
    "./Resource/iScreen/doggy.scb",
    "./Resource/iScreen/INTRO/splash.jpg",
    "./Resource/iScreen/Intro_Movie/1.jpg",
    "./Resource/iScreen/Intro_Movie/2.jpg",
    "./Resource/iScreen/Intro_Movie/3.jpg",
    "./Resource/iScreen/Intro_Movie/Sound/intro_2_2.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_3_1.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_3_2.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_4_3.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_2_3.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_1_2.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_2_1.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_5_2.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_4_2.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_6_1.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_7_3.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_5_3.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_1_1.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_7_1.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_4_1.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_1_3.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_7_2.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_5_1.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_6_2.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_6_3.mp+",
    "./Resource/iScreen/Intro_Movie/Sound/intro_7_4.mp+",
    "./Resource/iScreen/ARCANE/START/go1.tga",
    "./Resource/iScreen/ARCANE/START/go2.tga",
    "./Resource/iScreen/ARCANE/START/go.tga",
    "./Resource/mch_tracks.scb",
    "./Resource/M3D/s.3ds",
    "./Resource/M3D/a.3ds",
    "./Resource/M3D/r.3ds",
    "./Resource/M3D/t_red.3ds",
    "./Resource/M3D/r_red.3ds",
    "./Resource/M3D/s_red.3ds",
    "./Resource/M3D/a_red.3ds",
    "./Resource/M3D/t.3ds",
    "./Resource/M3d.scb",
]

src = argv[1]
dst = argv[2]


for file in land_deps:
    src_file = join(src, file)
    if not exists(src_file):
        src_file = src_file[:-3] + src_file[len(src_file)-3:].upper()

    if not exists(src_file):
        raise Exception("File " + src_file + " does not exists")
    
    dst_file = join(dst, file)
    if not exists(dirname(dst_file)):
        makedirs(dirname(dst_file))
    copyfile(src_file, dst_file)
    unlink(src_file)