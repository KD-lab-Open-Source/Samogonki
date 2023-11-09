#!/usr/bin/python3

from sys import argv
from glob import glob
from subprocess import check_output
from os.path import join, isfile

def scan_dir(dir):
    files = {}
    for file in glob("**/*", recursive=True, root_dir=dir):
        path = join(dir, file)
        if isfile(path):
            md5 = check_output(["md5sum", path]).decode("utf-8").replace("\n", "").split(" ")[0].strip()
            files[file.lower()] = md5
    return files

missed_files = []
not_mached_files = []
src = scan_dir(argv[1])
dst = scan_dir(argv[2])

for next in src:
    if not next in dst:
        missed_files.append(next)
    elif src[next] != dst[next]:
        not_mached_files.append(next + " " + src[next] + " " + dst[next])

for next in dst:
    if not next in src:
        missed_files.append(next)

print("Missed files:")
for next in missed_files:
    print(next)

print("Not matched:")
for next in not_mached_files:
    print(next)

