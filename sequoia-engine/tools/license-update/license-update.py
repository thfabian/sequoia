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
## This script updates all the license files.
##
##===------------------------------------------------------------------------------------------===##

license_template = """{0}==={1}-*- {2} -*-==={0}
{0}                         _____                        _       
{0}                        / ____|                      (_)      
{0}                       | (___   ___  __ _ _   _  ___  _  __ _ 
{0}                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
{0}                        ____) |  __/ (_| | |_| | (_) | | (_| |
{0}                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
{0}                                       | |                    
{0}                                       |_| 
{0}
{0} This file is distributed under the MIT License (MIT).
{0} See LICENSE.txt for details.
{0}
{0}===------------------------------------------------------------------------------------------==={0}"""

from argparse import ArgumentParser
from sys import exit, stderr, argv
from os import fsencode, listdir, path, walk

def update_license(file, comment, lang):
    print("Updating " + file + " ...")
    
    new_data = None
    with open(file, 'r') as f:
        read_data = f.read()
        
        first_line = '{0}==={1}-*- {2} -*-==={0}'.format(comment, (82 - len(lang)) * '-', lang)
        last_line = '{0}==={1}==={0}'.format(comment, 90 * '-')
        
        first_idx = read_data.find(first_line)
        last_idx = read_data.find(last_line) + len(last_line)
        if first_idx == -1 or last_idx == -1:
            return
        
        replacement_str = read_data[first_idx:last_idx]
        new_data = read_data.replace(replacement_str, 
                                     license_template.format(comment, (82 - len(lang)) * '-', lang))
                                     
    with open(file, 'w') as f:
        f.write(new_data)
        
def main():
    parser = ArgumentParser("license-update.py", 
                            description="Update the license of all files.")
    parser.add_argument("dirs", help="directories to traverse", metavar='dir', nargs='+')
    parser.add_argument("-v", "--verbose", dest="verbose", action='store_true',
                        help="enable verbose logging")
    args = parser.parse_args()

    for dir in args.dirs:
        for root, sub_folders, files in walk(dir):
            for filename in files:
                if filename.endswith(".py"):
                    lang = 'Python'
                    comment = '##'
                elif filename.endswith(".cpp") or filename.endswith(".h") or \
                     filename.endswith(".inc"):
                    lang = 'C++'
                    comment = '//' 
                elif filename.endswith(".cmake") or filename == "CMakeLists.txt":
                    lang = 'CMake'  
                    comment = '##'      
                else:
                    continue
                
                file = path.join(root, filename)
                update_license(file, comment, lang)
    
if __name__ == '__main__':
    main()
