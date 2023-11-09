#!/usr/bin/python3

from glob import glob
from os import unlink
from subprocess import check_call;

for file in glob("**/*.pak", recursive=True):
    print("Extracting", file)
    check_call(["unzip", "-q", file, "-d", "./"])
    unlink(file)