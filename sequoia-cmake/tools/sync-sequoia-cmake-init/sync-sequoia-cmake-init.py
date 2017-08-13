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
## Sync all `SequoiaCMakeInit.cmake` scripts in all sequoia projects.
##
##===------------------------------------------------------------------------------------------===##

import os
import filecmp
import shutil

def update_file(file_dst, file_src, project):
    if not os.path.exists(file_dst) or not filecmp.cmp(file_dst, file_src):
        print("-- Updating SequoiaCMakeInit.cmake of",project)
        shutil.copy(file_src, file_dst)

def main():
    sequoia_dir = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', '..', '..')
    sequoia_cmake_init_script = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 
                                             '..', 'cmake', 'SequoiaCMakeInit.cmake')

    # Update the main project
    update_file(os.path.join(sequoia_dir, 'cmake', 'SequoiaCMakeInit.cmake'), 
                sequoia_cmake_init_script, 'sequoia')

    # Update sub-projects
    for project in ['sequoia-docs', 'sequoia-engine']:
        if not os.path.exists(os.path.join(sequoia_dir, project)):
            print("WARNING: %s not checked out - cannot update 'SequoiaCMakeInit.cmake'" % project)

        update_file(os.path.join(sequoia_dir, project, 'cmake', 'SequoiaCMakeInit.cmake'), 
                    sequoia_cmake_init_script, project)

if __name__ == '__main__':
    main()
