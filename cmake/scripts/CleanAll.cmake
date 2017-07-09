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

set(cmake_generated ${CMAKE_BINARY_DIR}/CMakeCache.txt
                    ${CMAKE_BINARY_DIR}/CTestTestfile.cmake
                    ${CMAKE_BINARY_DIR}/cmake_install.cmake
                    ${CMAKE_BINARY_DIR}/Makefile
                    ${CMAKE_BINARY_DIR}/CMakeFiles
                    ${CMAKE_BINARY_DIR}/prefix
                    ${CMAKE_BINARY_DIR}/sequoia-engine
                    ${CMAKE_BINARY_DIR}/sequoia-engine-prefix
                    ${CMAKE_BINARY_DIR}/thirdparty
)

foreach(file ${cmake_generated})
  if(EXISTS ${file})
     file(REMOVE_RECURSE ${file})
  endif()
endforeach(file)

