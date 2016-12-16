##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

#
# Version
#
if(NOT DEFINED SEQUOIA_VERSION_MAJOR)
  set(SEQUOIA_VERSION_MAJOR 0 CACHE INTERNAL "Major version of Sequoia" FORCE)
endif()

if(NOT DEFINED SEQUOIA_VERSION_MINOR)
  set(SEQUOIA_VERSION_MINOR 0 CACHE INTERNAL "Minor version of Sequoia" FORCE)
endif()

if(NOT DEFINED SEQUOIA_VERSION_PATCH)
  set(SEQUOIA_VERSION_PATCH 1 CACHE INTERNAL "Patch version of Sequoia" FORCE)
endif()

if(NOT DEFINED SEQUOIA_VERSION_SUFFIX)
  set(SEQUOIA_VERSION_SUFFIX "dev" CACHE INTERNAL "Suffix of the version" FORCE)
endif()

if(NOT DEFINED SEQUOIA_VERSION_STRING)
  set(SEQUOIA_VERSION_STRING 
      "${SEQUOIA_VERSION_MAJOR}.${SEQUOIA_VERSION_MINOR}.${SEQUOIA_VERSION_PATCH}-${SEQUOIA_VERSION_SUFFIX}"
      CACHE INTERNAL "Version string of Sequoia" FORCE)
endif()

#
# Platform
#
if(WIN32)
  set(SEQUOIA_ON_WIN32 1 CACHE INTERNAL "Platform is Win32-ish" FORCE)
  set(SEQUOIA_PLATFORM_STRING "Windows" CACHE INTERNAL "Platform-id string" FORCE)
elseif(APPLE)
  set(SEQUOIA_ON_UNIX 1 CACHE INTERNAL "Platform is Unix-ish" FORCE)
  set(SEQUOIA_PLATFORM_STRING "Darwin" CACHE INTERNAL "Platform-id string" FORCE)
elseif(UNIX)
  set(SEQUOIA_ON_UNIX 1 CACHE INTERNAL "Platform is Unix-ish" FORCE)
  set(SEQUOIA_PLATFORM_STRING "Unix" CACHE INTERNAL "Platform-id string" FORCE)
endif()

#
# Compiler
#
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


