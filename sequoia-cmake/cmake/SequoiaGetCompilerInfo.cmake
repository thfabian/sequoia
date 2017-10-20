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

#.rst:
# sequoia_get_compiler_info
# -------------------------
#
# Get the identification string of the compiler.
#
# .. code-block:: cmake
#
#   sequoia_get_compiler()
#
# The functions defines the following variable:
#
# ``SEQUOIA_COMPILER_STRING``
#   String of the currently used compiler.
# 
# and conditionally one of the following:
#
# ``SEQUOIA_COMPILER_MSVC``
#   Set to 1 if the compiler in use is MSVC
# ``SEQUOIA_COMPILER_GNU``
#   Set to 1 if the compiler in use is GNU
# ``SEQUOIA_COMPILER_CLANG``
#   Set to 1 if the compiler in use is Clang
#
macro(sequoia_get_compiler_info)
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
    set(SEQUOIA_COMPILER_MSVC 1 CACHE INTERNAL "Compiler is MSVC" FORCE)
    set(SEQUOIA_COMPILER_STRING "Visual Studio (${CMAKE_CXX_COMPILER_VERSION})" 
        CACHE INTERNAL "Compiler-id string" FORCE)
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    set(SEQUOIA_COMPILER_GNU 1 CACHE INTERNAL "Compiler is GNU gcc" FORCE)
    set(SEQUOIA_COMPILER_STRING "GNU gcc (${CMAKE_CXX_COMPILER_VERSION})" 
        CACHE INTERNAL "Compiler-id string" FORCE)
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(SEQUOIA_COMPILER_CLANG 1 CACHE INTERNAL "Compiler is LLVM Clang" FORCE)
    set(SEQUOIA_COMPILER_STRING "LLVM Clang (${CMAKE_CXX_COMPILER_VERSION})" 
        CACHE INTERNAL "Compiler-id string" FORCE)
  endif()
endmacro()
