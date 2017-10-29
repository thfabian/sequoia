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

# sequoia_extract_cmake_package_args
# ----------------------------------
#
# Extract the CMake arguments of the <PACKAGE> and append it to <CMAKE_ARGS>.
#
#    PACKAGE:STRING=<>      - Name of the package
#    CMAKE_ARGS:STRING=<>   - Append the CMake arguments of <PACKGE> to this variable.
#
macro(sequoia_extract_cmake_package_args PACKAGE CMAKE_ARGS)
  string(TOUPPER ${PACKAGE} package_upper)
  if(DEFINED SEQUOIA_${package_upper}_CMAKE_ARGS)
    list(APPEND ${CMAKE_ARGS} ${SEQUOIA_${package_upper}_CMAKE_ARGS})
  endif()
endmacro()