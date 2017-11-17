##===------------------------------------------------------------------------------*- CMake -*-===##
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

set(SEQUOIA_EDITOR_VERSION_MAJOR 0 CACHE INTERNAL "Major version of the sequoia-editor" FORCE)
set(SEQUOIA_EDITOR_VERSION_MINOR 0 CACHE INTERNAL "Minor version of the sequoia-editor" FORCE)
set(SEQUOIA_EDITOR_VERSION_PATCH 1 CACHE INTERNAL "Patch version of the sequoia-editor" FORCE)
set(SEQUOIA_EDITOR_VERSION_SUFFIX "dev" CACHE INTERNAL "Suffix of the sequoia-editor version" FORCE)

set(SEQUOIA_EDITOR_VERSION 
  "${SEQUOIA_EDITOR_VERSION_MAJOR}.${SEQUOIA_EDITOR_VERSION_MINOR}.${SEQUOIA_EDITOR_VERSION_PATCH}"
  CACHE INTERNAL "Version of sequoia-editor" FORCE)
