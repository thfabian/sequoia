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

include(SequoiaIncludeGuard)
sequoia_include_guard()

include(SequoiaCheckCXXFlag)

#.rst:
# sequoia_check_and_set_cxx_flag
# ------------------------------
#
# Test if the C++ compiler flag is supported and if so, add it to the ``CMAKE_CXX_FLAGS``.
#
# .. code-block:: cmake
#
#   sequoia_check_and_set_cxx_flag(FLAG NAME)
#
# ``FLAG``
#   Compiler flag to check (e.g -Wall).
# ``NAME``
#   Name of the check (e.g HAVE_GCC_WALL).
#
macro(sequoia_check_and_set_cxx_flag FLAG NAME)
  sequoia_check_cxx_flag("${FLAG}" ${NAME})
  if(${NAME})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
  endif()
endmacro()
