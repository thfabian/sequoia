##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _       
##                        / ____|                      (_)      
##                       | (___   ___  __ _ _   _  ___  _  __ _ 
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
##                                       | |                    
##                                       |_| 
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

#.rst:
# Findlibpng
# ----------
#
# This script locates libpng. This script makes use of the standard find_package arguments of
# ``REQUIRED`` and ``QUIET``. LIBPNG_FOUND will report if an acceptable version of libpng was found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project:
#
# ``LIBPNG_INCLUDE_DIRS``
#   where to find png.h, etc.
# ``LIBPNG_LIBRARIES``
#   the libraries to link against to use PNG.
# ``LIBPNG_DEFINITIONS``
#   You should add_definitons(${PNG_DEFINITIONS}) before compiling code
#   that includes png library files.
# ``LIBPNG_FOUND``
#   If false, do not try to use PNG.
# ``LIBPNG_VERSION_STRING``
#   the version of the PNG library found
#
# Hints
# ^^^^^
#
# Set ``LIBPNG_ROOT`` to a directory that contains a libpng installation.

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

#
# Parse LIBPNG_ROOT
#
set(LIBPNG_ROOT_ENV $ENV{LIBPNG_ROOT})
if(LIBPNG_ROOT_ENV)
  set(LIBPNG_ROOT ${LIBPNG_ROOT_ENV} CACHE PATH "libpng install path.")
endif()

if(NOT(LIBPNG_ROOT))
  find_path(LIBPNG_ROOT NAMES png.h HINTS ${LIBPNG_INCLUDE_DIRS})
else()
  get_filename_component(_LIBPNG_ROOT_ABSOLUTE ${LIBPNG_ROOT} ABSOLUTE)
  set(LIBPNG_ROOT ${_LIBPNG_ROOT_ABSOLUTE} CACHE PATH "libpng install path.")
endif()

if(LIBPNG_FIND_QUIETLY)
  set(_FIND_ZLIB_ARG QUIET)
endif()

if(NOT(ZLIB_FOUND))
  find_package(ZLIB ${_FIND_ZLIB_ARG})
endif()

if(ZLIB_FOUND)
  #===-------------------------------------------------------------------------------------------===
  #   Find png headers
  #====------------------------------------------------------------------------------------------===
  if(NOT(LIBPNG_INTERNAL_INCLUDE_DIR))
    find_path(LIBPNG_INTERNAL_INCLUDE_DIR png.h HINTS ${LIBPNG_ROOT})
    mark_as_advanced(LIBPNG_INTERNAL_INCLUDE_DIR)
  endif()

  #===-------------------------------------------------------------------------------------------===
  #   Read version from png.h
  #====------------------------------------------------------------------------------------------===
  if(LIBPNG_INTERNAL_INCLUDE_DIR AND NOT(LIBPNG_VERSION_STRING))
    # Get version  
    file(STRINGS "${LIBPNG_INTERNAL_INCLUDE_DIR}/png.h" png_version_str REGEX 
         "^#define[ \t]+PNG_LIBPNG_VER_STRING[ \t]+\".+\"")

    string(REGEX REPLACE 
           "^#define[ \t]+PNG_LIBPNG_VER_STRING[ \t]+\"([^\"]+)\".*" 
           "\\1" 
           LIBPNG_VERSION_STRING 
           "${png_version_str}")
    unset(png_version_str)
  endif()

  #===-------------------------------------------------------------------------------------------===
  #   Find libpng library
  #====------------------------------------------------------------------------------------------===
  if(NOT(LIBPNG_LIBRARIES))
    list(APPEND LIBPNG_NAMES png libpng)
    unset(LIBPNG_NAMES_DEBUG)
    
    set(_PNG_VERSION_SUFFIXES 17 16 15 14 12)
    if(LIBPNG_FIND_VERSION MATCHES "^([0-9]+)\\.([0-9]+)(\\..*)?$")
      set(_PNG_VERSION_SUFFIX_MIN "${CMAKE_MATCH_1}${CMAKE_MATCH_2}")
      if(LIBPNG_FIND_VERSION_EXACT)
        set(_PNG_VERSION_SUFFIXES ${_PNG_VERSION_SUFFIX_MIN})
      else()
        string(REGEX REPLACE
            "${_PNG_VERSION_SUFFIX_MIN}.*" "${_PNG_VERSION_SUFFIX_MIN}"
            _PNG_VERSION_SUFFIXES "${_PNG_VERSION_SUFFIXES}")
      endif()
      unset(_PNG_VERSION_SUFFIX_MIN)
    endif()
    foreach(v IN LISTS _PNG_VERSION_SUFFIXES)
      list(APPEND LIBPNG_NAMES png${v} libpng${v})
      list(APPEND LIBPNG_NAMES_DEBUG png${v}d libpng${v}d)
    endforeach()
    unset(_PNG_VERSION_SUFFIXES)

    # For compatiblity with versions prior to this multi-config search, honor
    # any PNG_LIBRARY that is already specified and skip the search.
    find_library(LIBPNG_LIBRARY_RELEASE NAMES ${LIBPNG_NAMES} HINTS ${LIBPNG_ROOT})
    find_library(LIBPNG_LIBRARY_DEBUG NAMES ${LIBPNG_NAMES_DEBUG} HINTS  ${LIBPNG_ROOT})

    select_library_configurations(LIBPNG)
    mark_as_advanced(LIBPNG_LIBRARY LIBPNG_LIBRARY_RELEASE LIBPNG_LIBRARY_DEBUG)

    unset(LIBPNG_NAMES)
    unset(LIBPNG_NAMES_DEBUG)
    unset(LIBPNG_FOUND)
  endif()

  if(LIBPNG_LIBRARY AND LIBPNG_INTERNAL_INCLUDE_DIR)

    # png.h includes zlib.h. Sigh.
    set(LIBPNG_INCLUDE_DIRS ${LIBPNG_INTERNAL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})
    set(LIBPNG_LIBRARIES ${LIBPNG_LIBRARY} ${ZLIB_LIBRARY})

    if(CYGWIN)
      if(BUILD_SHARED_LIBS)
        # No need to define PNG_USE_DLL here, because it's default for Cygwin.
      else()
        set(PNG_DEFINITIONS -DPNG_STATIC)
      endif()
    endif()
  
  endif()
endif()

find_package_handle_standard_args(
  LIBPNG
  FAIL_MESSAGE "Could NOT find libpng. (Try setting LIBPNG_ROOT in the env)"
  REQUIRED_VARS 
    LIBPNG_LIBRARY
    LIBPNG_INCLUDE_DIRS
    ZLIB_FOUND 
  VERSION_VAR LIBPNG_VERSION_STRING
)

if(NOT(LIBPNG_FOUND) AND LIBPNG_FIND_REQUIRED EQUAL 1)
  message(FATAL_ERROR "Could NOT find libpng. (Try setting LIBPNG_ROOT in the env)")
endif()

