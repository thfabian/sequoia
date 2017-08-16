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

#.rst:
# FindLibDL
# ---------
#
# .. code-block:: cmake
#
#   find_package(LibDL [REQUIRED] [QUIET])
#
# This module locates the native dynamic linking loader (libdl.so_) includes and library. 
# ``LIBDL_FOUND`` will  report if ``libdl.so`` has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
# ``LIBDL_FOUND``
#  System has the libDL headers.
# ``LIBDL_INCLUDE_DIRS``
#  The location of the libDL headers.
# ``LIBDL_LIBRARIES``
#  The location of the libDL library (``libdl.so``).
#
# Hints
# ^^^^^
#
# Set ``LIBDL_NO_HEADERS`` to avoid searching for the headers (i.e ``dlfcn.h``).
#
# .. _libdl.so: https://linux.die.net/man/3/dlopen
#

include(FindPackageHandleStandardArgs)

if(LIBDL_LIBRARY)
  set(LibDL_FIND_QUIETLY TRUE)
endif(LIBDL_LIBRARY)

find_path(LIBDL_INCLUDE_DIRS dlfcn.h)

set(LIBDL_NAMES dl libdl ltdl libltdl)
find_library(LIBDL_LIBRARIES NAMES ${LIBDL_NAMES})

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
  LibDL 
  DEFAULT_MSG
  LIBDL_LIBRARIES
)

if(NOT(DEFINED LIBDL_NO_HEADERS) AND NOT(LIBDL_INCLUDE_DIRS) AND LIBDL_FOUND)
  message(FATAL_ERROR "Could NOT find headers for libdl.so: ${LIBDL_LIBRARIES}")
endif()

if(NOT(LIBDL_FOUND) AND LibDL_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find libdl.so")
endif()

mark_as_advanced(
 LIBDL_LIBRARIES 
 LIBDL_INCLUDE_DIRS
)

