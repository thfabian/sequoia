##===------------------------------------------------------------------------------*- CMake -*-===##
##                          _ 
##                         | |
##                       __| | __ ___      ___ ___ 
##                      / _` |/ _` \ \ /\ / / '_  |
##                     | (_| | (_| |\ V  V /| | | |
##                      \__,_|\__,_| \_/\_/ |_| |_| - Compiler Toolchain
##
##
##  This file is distributed under the MIT License (MIT). 
##  See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

include(SequoiaIncludeGuard)
sequoia_include_guard()

#.rst:
# sequoia_enable_rpath
# --------------------
#
# Enable full RPATH_ support. Note this macro should be called before any libraries or executables 
# are added.
#
# .. code-block:: cmake
#
#   sequoia_enable_rpath(LIB_PREFIX)
#
# ``LIB_PREFIX``
#   Install prefix for libraries (e.g ``lib``).
#
# .. _RPATH: https://cmake.org/Wiki/CMake_RPATH_handling
#
macro(sequoia_enable_rpath LIB_PREFIX)
  # Use, i.e. don't skip the full RPATH for the build tree
  set(CMAKE_SKIP_BUILD_RPATH FALSE)

  # If this is not enabled, some libraries are not found in the build directory
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 

  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_PREFIX}")
  set(CMAKE_MACOSX_RPATH "${CMAKE_INSTALL_RPATH}")

  # Add the automatically determined parts of the RPATH which point to directories outside the 
  # build tree to the install RPATH
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

  # The RPATH to be used when installing, but only if it's not a system directory
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES 
      "${CMAKE_INSTALL_PREFIX}/${LIB_PREFIX}" is_system_dir)
  if("${is_system_dir}" STREQUAL "-1")
     set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_PREFIX}")
  endif()
endmacro()
