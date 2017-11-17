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
# sequoia_cmake_init
# ------------------
#
# Add the necessary paths to ``CMAKE_MODULE_PATH`` to use the functions, macros and modules of the 
# ``sequoia-cmake`` project. To find the ``sequoia-cmake`` module the  following directories are 
# searched:
#
#  1. CMake variable ``SEQUOIA_CMAKE_DIR``
#  2. Environment variable ``SEQUOIA_CMAKE_DIR``
#  3. ``${CMAKE_CURRENT_LIST_DIR}/../sequoia-cmake``
#  4. ``${CMAKE_CURRENT_LIST_DIR}/sequoia-cmake``
#
# where ``CMAKE_CURRENT_LIST_DIR`` is the directory of the listfile currently being processed. This
# macro also sets the ``SEQUOIA_CMAKE_DIR`` to the top-level director of the sequoia-cmake project.
#
# .. code-block:: cmake
#
#  sequoia_cmake_init()
#
# .. note:: This file is placed in each sub-projects ``cmake/`` directory and needs to be kept in 
#           sync. Use the python script `sync-sequoia-cmake-init.py` (in 
#           ``<sequoia-cmake>/tools/sync-sequoia-cmake-init``) to assert all projects have an 
#           up-to-date version.
#
macro(sequoia_cmake_init)
  set(sequoia_cmake_init_dir)
  if(DEFINED SEQUOIA_CMAKE_DIR)
    set(sequoia_cmake_init_dir ${SEQUOIA_CMAKE_DIR})
  elseif(NOT "$ENV{SEQUOIA_CMAKE_DIR}" STREQUAL "")
    set(sequoia_cmake_init_dir $ENV{SEQUOIA_CMAKE_DIR})
  elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../sequoia-cmake")
    set(sequoia_cmake_init_dir ${CMAKE_CURRENT_LIST_DIR}/../sequoia-cmake)
  elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/sequoia-cmake")
    set(sequoia_cmake_init_dir ${CMAKE_CURRENT_LIST_DIR}/sequoia-cmake)
  else()
    message(FATAL_ERROR "sequoia-cmake project not found!")
  endif()

  list(APPEND CMAKE_MODULE_PATH "${sequoia_cmake_init_dir}/cmake")

  if(NOT DEFINED SEQUOIA_CMAKE_DIR)
    get_filename_component(sequoia_cmake_dir ${sequoia_cmake_init_dir} ABSOLUTE)
    set(SEQUOIA_CMAKE_DIR ${sequoia_cmake_dir} CACHE PATH "Path to sequoia-cmake project.")
  endif()
endmacro()
