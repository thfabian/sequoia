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

# sequoia_set_download_dir
# ------------------------
#
# Set typical downloads directory, if it already exists. Otherwise, use a Downloads subdir in the 
# build tree.
#
#    OUT_VAR:STRING=<>      - Directory to download packages
#
macro(sequoia_set_download_dir OUT_VAR)
  if(NOT "$ENV{HOME}" STREQUAL "" AND EXISTS "$ENV{HOME}/Downloads")
    set(download_dir "$ENV{HOME}/Downloads")
  elseif(NOT "$ENV{USERPROFILE}" STREQUAL "" AND EXISTS "$ENV{USERPROFILE}/Downloads")
    set(download_dir "$ENV{USERPROFILE}/Downloads")
  elseif(NOT "${CMAKE_CURRENT_BINARY_DIR}" STREQUAL "")
    set(download_dir "${CMAKE_CURRENT_BINARY_DIR}/Downloads")
  else()
    message(FATAL_ERROR "unexpectedly empty CMAKE_CURRENT_BINARY_DIR")
  endif()
  string(REPLACE "\\" "/" download_dir "${download_dir}")

  file(MAKE_DIRECTORY "${download_dir}")
  if(NOT EXISTS "${download_dir}")
    message(FATAL_ERROR "Could not find or make download directory")
  endif()

  set(${OUT_VAR} "${download_dir}")
endmacro()