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

set(sequoia_engine_cmake_args ${Sequoia_CMAKE_ARGS})

if(SEQUOIA_ASSERTS)
  list(APPEND sequoia_engine_cmake_args "-DSEQUOIA_ASSERTS:BOOL=ON")
else()
  if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    list(APPEND sequoia_engine_cmake_args "-DSEQUOIA_ASSERTS:BOOL=OFF")
  else()
    list(APPEND sequoia_engine_cmake_args "-DSEQUOIA_ASSERTS:BOOL=ON")
  endif()
endif()

set(sequoia_engine_source "${CMAKE_CURRENT_SOURCE_DIR}/sequoia-engine")
set(sequoia_engine_build "${CMAKE_CURRENT_BINARY_DIR}/sequoia-engine")

unset(sequoia_engine_deps)
sequoia_add_optional_deps(sequoia_engine_deps "${projects}")

ExternalProject_Add(
  sequoia-engine
  SOURCE_DIR ${sequoia_engine_source}
  BINARY_DIR ${sequoia_engine_build}
  INSTALL_COMMAND ""
  CMAKE_CACHE_ARGS
    ${sequoia_engine_cmake_args}
  DEPENDS
    ${sequoia_engine_deps}
)

# Create bash or bat script for re-invoking cmake (for convenience)
set(script_args)
foreach(arg ${sequoia_engine_cmake_args})
  string(REGEX MATCH "^(.*)=+(.*)$" dummy ${arg})
  if(NOT(CMAKE_MATCH_1) AND NOT(CMAKE_MATCH_2))
    set(script_args "${script_args} ${arg}")
  else()
    # Value of the option has to be quoted for Windows
    set(script_args "${script_args} ${CMAKE_MATCH_1}=\"${CMAKE_MATCH_2}\"")
  endif()
endforeach()
set(script_args "-G \"${CMAKE_GENERATOR}\" ${script_args}")
  
if(WIN32)
  file(WRITE ${sequoia_engine_build}/rerun-cmake.bat 
       "\"${CMAKE_COMMAND}\" \"${CMAKE_SOURCE_DIR}/sequoia-engine\" ${script_args}\n")
else()
  file(WRITE ${sequoia_engine_build}/rerun-cmake.sh 
       "#!bin/bash\n${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}/sequoia-engine ${script_args}\n")
endif()

