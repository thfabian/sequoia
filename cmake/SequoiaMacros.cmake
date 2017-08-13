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

## sequoia_add_optional_deps
## -------------------------
##
## Convenience macro for adding dependencies optionally if not using system copies. This function 
## takes the external project target name, looks for a variable of the form USE_SYSTEM_TARGETNAME, 
## if this does not exist or is set to false the supplied taget name will be added to dep_var.
##
##    DEP_VAR:STRING=<>      - Output variable containing the resolved dependencies
##    ARGN                   - Dependencies to append
##
macro(sequoia_add_optional_deps DEP_VAR)
  foreach(dep ${ARGN})
    string(TOUPPER "${dep}" dependency)
    if(NOT USE_SYSTEM_${dependency})
      list(APPEND ${DEP_VAR} ${dep})
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
## file ``External_<PACKAGE>`` will be included which should define a target ``PACKAGE`` (in all 
## lower case) which is used to built the package.
##
## The option ``USE_SYSTEM_<PACKAGE>`` indicates if the <PACKAGE> (all uppercase) is built or 
## supplied by the system. Note that ``USE_SYSTEM_<PACKAGE>`` does not honor the user setting if 
## the package cannot be found.
##
##    PACKAGE:STRING=<>           - Name of the package (has to be the same name as used in 
##                                  ``find_package``).
##    PACKAGE_ARGS:LIST=<>        - Arguments passed to ``find_package``.
##    FORWARD_VARS:LIST=<>        - List of variables which are appended (if found) to the 
##                                  ``Sequoia_THIRDPARTYLIBS_ARGS``.
##    REQUIRED_VARS:LIST=<>       - Variables which need to be TRUE to consider the package as 
##                                  found. By default we check that ``<PACKAGE>_FOUND`` is TRUE.
##    DEPENDS:LIST=<>             - Dependencies of this package.
##
macro(sequoia_find_package)
  set(options)
  set(one_value_args PACKAGE)
  set(multi_value_args PACKAGE_ARGS FORWARD_VARS REQUIRED_VARS DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  string(TOUPPER ${ARG_PACKAGE} package_upper)

  # Define the external file to include if we cannot find the package
  set(external_file External_${ARG_PACKAGE})

  # Define the name of the target *if* we built it (targets are always lower-case for us)
  string(TOLOWER ${ARG_PACKAGE} target)

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
    # Check if the system has the package
    find_package(${ARG_PACKAGE} ${ARG_PACKAGE_ARGS})

    # Check if all the required variables are set
    set(required_vars_ok TRUE)
    set(missing_required_vars)
    foreach(arg ${ARG_REQUIRED_VARS})
      if(NOT(${arg}))
        set(required_vars_ok FALSE)
        set(missing_required_vars "${missing_required_vars} ${arg}")
      endif()
    endforeach()

    if(missing_required_vars)
      message(STATUS "Package ${ARG_PACKAGE} not found due to missing:${missing_required_vars}")    
    endif()
    
    if(required_vars_ok AND (${ARG_PACKAGE}_FOUND OR ${ARG_PACKAGE}_DIR)) 
      set(use_system TRUE)

      # Forward arguments
      foreach(var ${ARG_FORWARD_VARS})
        if(DEFINED ${var})
          set(Sequoia_THIRDPARTYLIBS_ARGS "${Sequoia_THIRDPARTYLIBS_ARGS};-D${var}:PATH=${${var}}")
        endif()
      endforeach()

    else()
      set(USE_SYSTEM_${package_upper} OFF CACHE BOOL ${doc} FORCE)
      include(${external_file})
    endif()
  endif()

  # Set the dependencies if we build
  if(NOT(use_system) AND ARG_DEPENDS)
    set(deps)
    sequoia_add_optional_deps(deps ${ARG_DEPENDS})
    if(deps)
      add_dependencies(${target} ${deps})
    endif()
  endif()

  sequoia_gen_package_info_str(${ARG_PACKAGE} ${use_system})
endmacro()
