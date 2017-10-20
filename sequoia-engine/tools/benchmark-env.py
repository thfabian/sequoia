#!/usr/bin/python3
# -*- coding: utf-8 -*-
##===-----------------------------------------------------------------------------*- Python -*-===##
##                         _____                        _
##                        / ____|                      (_)
##                       | (___   ___  __ _ _   _  ___  _  __ _
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
##                                       | |
##                                       |_|
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##
##
## Setup the benchmark environment (e.g set cpu-frequency to performance mode).
##
##===------------------------------------------------------------------------------------------===##

import subprocess
from argparse import ArgumentParser
from sys import platform


def which(program):
    """ Check if program exists an return path to it. """
    import os
    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    fpath, fname = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file

    return None


def main():
    parser = ArgumentParser("benchmark-env.py",
                            description="Setup the system for benchmarking.")
    args = parser.parse_args()
    if platform == "linux" or platform == "linux2":
        # Set the cpu-frequency to 'performance'
        cpupower = which("cpupower")
        if cpupower:
            subprocess.call("sudo %s frequency-set --governor performance" % cpupower, shell=True)
        else:
            print("WARNING: cpupower not available - cannot set cpu-frequency")


if __name__ == '__main__':
    main()
