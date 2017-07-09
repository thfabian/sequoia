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

include(CheckCXXCompilerFlag)
include(CMakeParseArguments)

## sequoia_report_result
## ---------------------
##
## Report a list of strings as STATUS.
##
##    FILE:STRING=<>             - Header of the current report.
##    ARGN:STRING=<>             - List of strings to report.
##
macro(sequoia_report_result HEADER)
  string(LENGTH ${HEADER} header_length)
  set(full_header "----------------------------------------------------------------")
  math(EXPR right_header_length "43 - ${header_length}")
  string(SUBSTRING ${full_header} "0" "${right_header_length}" right_header)
  
  message(STATUS "------------------- ${HEADER} ${right_header}")
  foreach(arg ${ARGN})
    message(STATUS "${arg}")
  endforeach()
  message(STATUS "${full_header}")
endmacro()

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

## sequoia_set_assert_level
## ------------------------
##
## If SEQUOIA_ASSERTS is ON, the macro NDBEUG will be undefined independcent of the current 
## CMAKE_BUILD_TYPE.
##
macro(sequoia_set_assert_level)
  if(SEQUOIA_ASSERTS)
    if(WIN32)
      set(ndebug_flag "/UNDEBUG")
    else()
      set(ndebug_flag "-UNDEBUG")
    endif()
    list(APPEND Sequoia_DEFAULT_ARGS "-DCMAKE_CXX_FLAGS:STRING=${ndebug_flag}" 
                                     "-DCMAKE_C_FLAGS:STRING=${ndebug_flag}")
  endif()
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

## sequoia_add_target_clean_all
## ----------------------------
##
## Provide a ``clean-all`` target which clears the CMake Cache.
##
macro(sequoia_add_target_clean_all)
  add_custom_target(clean-all
          COMMAND ${CMAKE_MAKE_PROGRAM} clean
          COMMAND ${CMAKE_COMMAND} -P 
                  "${CMAKE_SOURCE_DIR}/cmake/scripts/CleanAll.cmake")
endmacro()

## sequoia_gen_package_info_str
## ----------------------------
##
## Append a package info string to ``SEQUOIA_PACKAGE_INFO`` for ``PACKAGE``.
##
##    PACKAGE_NAME:STRING=<>      - Name of the package
##    USE_SYSTEM:BOOL=<>          - Do we use the system version of the package?
##
macro(sequoia_gen_package_info_str PACKAGE_NAME USE_SYSTEM)
  string(LENGTH ${PACKAGE_NAME} package_name_length)
  math(EXPR indent_length "20 - ${package_name_length}")

  set(full_indent "                    ") 
  string(SUBSTRING ${full_indent} "0" "${indent_length}" indent)

  if(${USE_SYSTEM})
    set(SEQUOIA_PACKAGE_INFO "${PACKAGE_NAME}${indent}: found;${SEQUOIA_PACKAGE_INFO}")
  else()
    set(SEQUOIA_PACKAGE_INFO "${SEQUOIA_PACKAGE_INFO};${PACKAGE_NAME}${indent}: building")
  endif()
endmacro()

## sequoia_build_package
## ---------------------
##
## Build the package using the file ``External_${PACKAGE}``. This is equivalent to 
## ``include(External_${PACKAGE})`` but also set the appropriate report variables. 
##
##    PACKAGE:STRING=<>           - Name of the package
##
macro(sequoia_build_package PACKAGE)
  set(external_file External_${PACKAGE})
  sequoia_gen_package_info_str(${PACKAGE} FALSE)
  include(${external_file})
endmacro()

## sequoia_find_package
## --------------------
##
## Try to find the package ``PACKAGE``. If the package cannot be found via ``find_package``, the 
## file ``External_${PACKAGE}`` will be included which should define a target ``PACKAGE`` (not 
## necessarily the same case) which is used to built the package.
##
## The option ``USE_SYSTEM_${PACKAGE}`` indicates if the ${PACKAGE} (all uppercase) is built or 
## the system version is going to be used. Note that ``USE_SYSTEM_${PACKAGE}`` does not honor the 
## user setting if the package cannot be found.
##
##    PACKAGE:STRING=<>           - Name of the package (has to be the same name as used in 
##                                  ``find_package``).
##    PACKAGE_ARGS:LIST=<>        - Arguments passed to ``find_package``
##    FORWARD_VARS:LIST=<>        - List of arguments which are appended to the 
##                                  ``Sequoia_THIRDPARTYLIBS_ARGS``
macro(sequoia_find_package)
  set(options)
  set(one_value_args PACKAGE)
  set(multi_value_args PACKAGE_ARGS FORWARD_VARS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  string(TOUPPER ${ARG_PACKAGE} package_upper)

  # Define the external file to include if we cannot find the package
  set(external_file External_${ARG_PACKAGE})

  # Do we use the system package or build it from source? 
  set(doc "Should we use the system ${ARG_PACKAGE}?")
  set(default_use_system ON)
  if(NO_SYSTEM_LIBS)
    set(default_use_system OFF)
  endif()

  option(USE_SYSTEM_${package_upper} ${doc} ${default_use_system})

  set(use_system FALSE)
  if(NOT(USE_SYSTEM_${package_upper}))
    set(USE_SYSTEM_${package_upper} OFF CACHE BOOL ${doc} FORCE)
    include(${external_file})
  else()
    # Has the system the package?
    find_package(${ARG_PACKAGE} ${ARG_PACKAGE_ARGS})

    if(${ARG_PACKAGE}_FOUND)
      # YES.. use it!
      set(use_system TRUE)

      # Forward arguments
      foreach(var ${ARG_FORWARD_VARS})
        if(DEFINED ${var})
          set(Sequoia_THIRDPARTYLIBS_ARGS "${Sequoia_THIRDPARTYLIBS_ARGS};-D${var}:PATH=${${var}}")
        endif()
      endforeach()

    else()
      # NO.. build it!
      set(USE_SYSTEM_${package_upper} OFF CACHE BOOL ${doc} FORCE)
      include(${external_file})
    endif()
  endif()

  sequoia_gen_package_info_str(${ARG_PACKAGE} ${use_system})
endmacro()

