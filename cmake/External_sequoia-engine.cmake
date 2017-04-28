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

set(_source "${CMAKE_CURRENT_SOURCE_DIR}/sequoia-engine")
set(_build "${CMAKE_CURRENT_BINARY_DIR}/sequoia-engine")

unset(_deps)
sequoia_add_optional_deps(_deps "${projects}")

if(SEQUOIA_ASSERTS)
  list(APPEND cmake_args "-DSEQUOIA_ASSERTS:BOOL=ON")
else()
  if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    list(APPEND cmake_args "-DSEQUOIA_ASSERTS:BOOL=OFF")
  else()
    list(APPEND cmake_args "-DSEQUOIA_ASSERTS:BOOL=ON")
  endif()
endif()
  
ExternalProject_Add(
  sequoia-engine
  SOURCE_DIR ${_source}
  BINARY_DIR ${_build}
  CMAKE_CACHE_ARGS
    ${Sequoia_DEFAULT_ARGS}
    ${Sequoia_THIRDPARTYLIBS_ARGS}
    ${cmake_args}
  DEPENDS
    ${_deps}
)

# Create bash script for re-invoking cmake
set(script_args)
foreach(arg ${Sequoia_DEFAULT_ARGS} ${Sequoia_THIRDPARTYLIBS_ARGS} ${cmake_args})
  set(script_args "${script_args} ${arg}")
endforeach()
file(WRITE ${_build}/rerun-cmake.sh 
     "#!bin/bash\n${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}/sequoia-engine ${script_args}\n")

