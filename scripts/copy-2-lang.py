#!/usr/bin/python3

from sys import argv
from os import makedirs
from os.path import exists, join, dirname, isfile
from shutil import copyfile, rmtree
from re import sub, IGNORECASE
from glob import glob

lang_deps = [
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

    "./Resource/m3d_font_add.scb",
]

fonts_dir = ["./Resource/iScreen/FONTS/", "./Resource/M3D/Font/Local"]

src = argv[1]
dst = argv[2]

if exists(dst):
    rmtree(dst)

for file in lang_deps:
    src_file = join(src, file)
    if not exists(src_file):
        src_file = src_file[:-3] + src_file[len(src_file)-3:].upper()

    if not exists(src_file):
        src_file = sub("iscreen", "iSCREEN", src_file, flags=IGNORECASE)

    if not exists(src_file):
        src_file = src_file[:-3] + src_file[len(src_file)-3:].lower()

    if not exists(src_file):
        raise Exception("File " + src_file + " does not exists")
    
    dst_file = join(dst, file)
    if not exists(dirname(dst_file)):
        makedirs(dirname(dst_file))
    copyfile(src_file, dst_file)

for font_dir in fonts_dir:
    src_fonts_dir = join(src, font_dir)
    if not exists(src_fonts_dir):
        src_fonts_dir = sub("iscreen", "iSCREEN", src_fonts_dir, flags=IGNORECASE)
    if not exists(src_fonts_dir):
        raise Exception("Local fonts dir not found", src_fonts_dir)
    for next in glob("**/*", root_dir=src_fonts_dir, recursive=True):
        src_file = join(src_fonts_dir, next)
        if isfile(src_file):
            dst_file = join(dst, font_dir, next) 
            if not exists(dirname(dst_file)):
                makedirs(dirname(dst_file))
            copyfile(src_file, dst_file)
