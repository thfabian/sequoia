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
# FindBackward
# ------------
#
# This script locates backward (https://github.com/bombela/backward-cpp). This script makes use 
# of the standard find_package arguments of ``VERSION``, ``REQUIRED`` and ``QUIET``. BACKWARD_FOUND
# will report if Backward was found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# Defines the following variables:
#
#   BACKWARD_FOUND               - System has the Backward header
#   BACKWARD_INCLUDE_DIRS        - The location of Backward header
#   BACKWARD_HAS_BFD             - System has libbfd from the GNU/binutils
#   BACKWARD_BFD_INCLUDE_DIRS    - Include directories of libbfd
#   BACKWARD_BFD_LIBRARIES       - Location of libbfd
#   BACKWARD_HAS_DW              - System has libdw from the elfutils
#   BACKWARD_DW_INCLUDE_DIRS     - Include directories of libdw
#   BACKWARD_DW_LIBRARIES        - Location of libdw
#
# Hints
# ^^^^^
#
# Set ``BACKWARD_ROOT`` to a directory that contains a Backward installation, or directly set 
# ``BACKWARD_INCLUDE_DIRS``.

include(FindPackageHandleStandardArgs)

#
# Parse BACKWARD_ROOT
#
set(BACKWARD_ROOT_ENV $ENV{BACKWARD_ROOT})
if(BACKWARD_ROOT_ENV)
  set(BACKWARD_ROOT ${BACKWARD_ROOT_ENV} CACHE PATH "Backward install path.")
endif()

if(NOT(BACKWARD_ROOT))
  find_path(BACKWARD_ROOT NAMES backward/backward.hpp HINTS ${BACKWARD_INCLUDE_DIRS})
else()
  get_filename_component(_BACKWARD_ROOT_ABSOLUTE ${BACKWARD_ROOT} ABSOLUTE)
  set(BACKWARD_ROOT ${_BACKWARD_ROOT_ABSOLUTE} CACHE PATH "Backward install path.")
endif()

#
# Look for backward/backward.hpp
#
if(NOT(BACKWARD_INCLUDE_DIRS))
  find_path(BACKWARD_INCLUDE_DIRS NAMES backward/backward.hpp HINTS ${BACKWARD_ROOT})
endif()

#
# Check for libbfd from the GNU/binutils 
#
if(NOT(WIN32))
  find_path(LIBBFD_INCLUDE_DIR NAMES "bfd.h")
  find_library(LIBBFD_LIBRARY bfd)
  find_path(LIBDL_INCLUDE_DIR NAMES "dlfcn.h")
  find_library(LIBDL_LIBRARY dl)
  find_package_handle_standard_args(
    libbfd 
    DEFAULT_MSG 
      LIBBFD_INCLUDE_DIR 
      LIBBFD_LIBRARY
      LIBDL_INCLUDE_DIR 
      LIBDL_LIBRARY
  )
  mark_as_advanced(LIBBFD_INCLUDE_DIR LIBBFD_LIBRARY LIBDL_INCLUDE_DIR LIBDL_LIBRARY)
  set(BACKWARD_HAS_BFD ${LIBBFD_FOUND})
  if(BACKWARD_HAS_BFD)
    set(BACKWARD_BFD_INCLUDE_DIRS ${LIBBFD_INCLUDE_DIR} ${LIBDL_INCLUDE_DIR})
    set(BACKWARD_BFD_LIBRARIES ${LIBBFD_LIBRARY} ${LIBDL_LIBRARY})
    mark_as_advanced(BACKWARD_BFD_INCLUDE_DIRS BACKWARD_BFD_LIBRARIES)
  endif()
else()
  set(BACKWARD_HAS_BFD FALSE)
endif()

#
# Check for libdw from the elfutils
#
if(NOT(WIN32))
  find_path(LIBDW_INCLUDE_DIRS NAMES "elfutils/libdw.h" "elfutils/libdwfl.h")
  find_library(LIBDW_LIBRARIES dw)
  find_package_handle_standard_args(
    libdw 
    DEFAULT_MSG	
      LIBDW_LIBRARIES 
      LIBDW_INCLUDE_DIRS
  )
  mark_as_advanced(LIBDW_INCLUDE_DIRS LIBDW_LIBRARIES)
  set(BACKWARD_HAS_DW ${LIBDW_FOUND})
  if(BACKWARD_HAS_DW)
    set(BACKWARD_DW_INCLUDE_DIRS ${LIBDW_INCLUDE_DIRS})
    set(BACKWARD_DW_LIBRARIES ${LIBDW_LIBRARIES})
    mark_as_advanced(BACKWARD_DW_INCLUDE_DIRS BACKWARD_DW_LIBRARIES)
  endif()
else()
  set(BACKWARD_HAS_DW FALSE)
endif()

#
# Handle result
#
find_package_handle_standard_args(
  Backward
  FAIL_MESSAGE "Could NOT find Backward. (Try setting BACKWARD_ROOT in the env)"
  REQUIRED_VARS  
    BACKWARD_ROOT
    BACKWARD_INCLUDE_DIRS
  VERSION_VAR BACKWARD_VERSION
)
mark_as_advanced(BACKWARD_INCLUDE_DIRS)

if(NOT(BACKWARD_FOUND) AND Backward_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find BACKWARD. (Try setting BACKWARD_ROOT in the env)")
endif()


