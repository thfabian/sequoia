##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

include(CheckCXXCompilerFlag)

## sequoia_check_in_source_build
## -----------------------------
##
## Prevent in source builds.
##
macro(sequoia_check_in_source_build)
  if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE)
    message(FATAL_ERROR "In-source builds are not allowed.
Please create a build directory and run cmake from there, passing the path
to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'.
Please delete them.")
  endif()
endmacro()

## sequoia_check_in_source_build
## -----------------------------
##
## Convenience macro for adding dependencies optionally if not using system copies. This function 
## takes the external project target name, looks for a variable of the form USE_SYSTEM_TARGETNAME, 
## if this does not exist or is set to false the supplied taget name will be added to dep_var.
##
macro(sequoia_add_optional_deps DEP_VAR)
  foreach(_dependency ${ARGN})
    string(TOUPPER "${_dependency}" _uDependency)
    if(NOT USE_SYSTEM_${_uDependency})
      list(APPEND ${DEP_VAR} ${_dependency})
    endif()
  endforeach()
endmacro()

## sequoia_check_in_source_build
## -----------------------------
##
## This script defines a download_dir variable, if it's not already defined.
##
## Cache the downloads in download_dir if it's defined. Otherwise, use the user's typical 
## Downloads directory, if it already exists. Otherwise, use a Downloads subdir in the build tree.
##
macro(sequoia_set_default_download_dir)
  if(NOT DEFINED download_dir)
    if(NOT "$ENV{HOME}" STREQUAL "" AND EXISTS "$ENV{HOME}/Downloads")
      set(download_dir "$ENV{HOME}/Downloads")
    elseif(NOT "$ENV{USERPROFILE}" STREQUAL "" AND EXISTS "$ENV{USERPROFILE}/Downloads")
      set(download_dir "$ENV{USERPROFILE}/Downloads")
    elseif(NOT "${CMAKE_CURRENT_BINARY_DIR}" STREQUAL "")
      set(download_dir "${CMAKE_CURRENT_BINARY_DIR}/Downloads")
    else()
      message(FATAL_ERROR "unexpectedly empty CMAKE_CURRENT_BINARY_DIR")
    endif()
    string(REPLACE "\\" "/" download_dir "${download_dir}")
  endif()

  file(MAKE_DIRECTORY "${download_dir}")
  if(NOT EXISTS "${download_dir}")
    message(FATAL_ERROR "could not find or make Downloads directory")
  endif()
endmacro()

## sequoia_setup_sequioa_engine_builds
## -----------------------------------
##
## Setup build directories for the sequoia-engine in Release, RelWithDebInfo and Debug mode using
## the current CMakeCache. 
##
##  LIST_PATH:PATH=<>       - CMakeLists.txt path
##  ARGN                    - Arguments passed to CMake
##
macro(sequoia_setup_sequioa_engine_builds)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Release)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)

  set(CMAKE_ARGS "${Sequoia_DEFAULT_ARGS}" "${Sequoia_THIRDPARTYLIBS_ARGS}")
  set(LIST_PATH ${CMAKE_SOURCE_DIR}/sequoia-engine)


  list(APPEND CMAKE_DEBUG_ARGS -DSEQUOIA_ASSERTS=ON)
  list(APPEND CMAKE_RELWITHDEBINFO_ARGS -DSEQUOIA_ASSERTS=ON)
  list(APPEND CMAKE_RELEASE_ARGS -DSEQUOIA_ASSERTS=OFF)
  foreach(arg ${CMAKE_ARGS})
    # Remove -DCMAKE_INSTALL_PREFIX
    string(REGEX REPLACE "-DCMAKE_INSTALL_PREFIX:PATH=.*" "" arg_in ${arg})
    
    if(NOT(arg_in))
      continue()
    endif()

    # Replace CMAKE_BUILD_TYPE
    string(REGEX REPLACE "-DCMAKE_BUILD_TYPE:STRING=.*" 
                         "-DCMAKE_BUILD_TYPE:STRING=Debug" arg_debug ${arg_in})
    list(APPEND CMAKE_DEBUG_ARGS ${arg_debug})

    string(REGEX REPLACE "-DCMAKE_BUILD_TYPE:STRING=.*" 
                         "-DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo" arg_relwithdebinfo ${arg_in})
    list(APPEND CMAKE_RELWITHDEBINFO_ARGS ${arg_relwithdebinfo})

    string(REGEX REPLACE "-DCMAKE_BUILD_TYPE:STRING=.*" 
                         "-DCMAKE_BUILD_TYPE:STRING=Release" arg_release ${arg_in})
    list(APPEND CMAKE_RELEASE_ARGS ${arg_release})
  endforeach()

  unset(_deps)
  sequoia_add_optional_deps(_deps "${projects}")

  # Release
  add_custom_target(setup-release-build ALL 
                    COMMAND ${CMAKE_COMMAND} ${LIST_PATH} ${CMAKE_RELEASE_ARGS} -G "${CMAKE_GENERATOR}"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Release
                    COMMENT "Setting up Release build of sequoia-engine"
                    DEPENDS ${_deps})
  set(filename ${CMAKE_CURRENT_BINARY_DIR}/Release/run-cmake.sh)
  file(WRITE "${filename}" "#!/bin/bash\n${CMAKE_COMMAND} ${LIST_PATH} -G \"${CMAKE_GENERATOR}\"")
  foreach(arg ${CMAKE_RELEASE_ARGS})
    file(APPEND "${filename}" " ${arg}")
  endforeach()
  file(APPEND "${filename}" "\n")

  # RelWithDebInfo
  add_custom_target(setup-relwithdebinfo-build ALL 
                    COMMAND ${CMAKE_COMMAND} ${LIST_PATH} ${CMAKE_RELWITHDEBINFO_ARGS} -G "${CMAKE_GENERATOR}"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo
                    COMMENT "Setting up RelWithDebInfo build of sequoia-engine"
                    DEPENDS ${_deps})
  set(filename ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/run-cmake.sh)
  file(WRITE "${filename}" "#!/bin/bash\n${CMAKE_COMMAND} ${LIST_PATH} -G \"${CMAKE_GENERATOR}\"")
  foreach(arg ${CMAKE_RELWITHDEBINFO_ARGS})
    file(APPEND "${filename}" " ${arg}")
  endforeach() 
  file(APPEND "${filename}" "\n") 

  # Debug
  add_custom_target(setup-debug-build ALL 
                    COMMAND ${CMAKE_COMMAND} ${LIST_PATH} ${CMAKE_DEBUG_ARGS} -G "${CMAKE_GENERATOR}"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug
                    COMMENT "Setting up Debug build of sequoia-engine"
                    DEPENDS ${_deps})
  set(filename ${CMAKE_CURRENT_BINARY_DIR}/Debug/run-cmake.sh)
  file(WRITE "${filename}" "#!/bin/bash\n${CMAKE_COMMAND} ${LIST_PATH} -G \"${CMAKE_GENERATOR}\"")
  foreach(arg ${CMAKE_DEBUG_ARGS})
    file(APPEND "${filename}" " ${arg}")
  endforeach()  
  file(APPEND "${filename}" "\n")
  
endmacro()

