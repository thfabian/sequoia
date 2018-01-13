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

include(CMakeParseArguments)
include(SequoiaComputeOptionalDependency)
include(SequoiaMakePackageInfo)
include(SequoiaExportPackage)

# sequoia_find_package
# --------------------
#
# Try to find the package <PACKAGE>. If the package cannot be found via find_package, the 
# file "External_<PACKAGE>" will be included which should define a target <PACKAGE> (in all 
# lower case) which is used to built the package.
#
# The option USE_SYSTEM_<PACKAGE> indicates if the <PACKAGE> (all uppercase) is built or 
# supplied by the system. Note that USE_SYSTEM_<PACKAGE> does not honor the user setting if 
# the package cannot be found (i.e it will build it regardlessly).
#
# The variable SEQUOIA_<PACKAGE>_FOUND will be set to indiacte if the package has been found (or is 
# going to be built).
#
#    PACKAGE:STRING=<>        - Name of the package (has to be the same name as used in 
#                               find_package).
#    PACKAGE_ARGS:LIST=<>     - Arguments passed to find_package.
#    FORWARD_VARS:LIST=<>     - List of variables which will be exported if defined. 
#    REQUIRED_VARS:LIST=<>    - Variables which need to be TRUE to consider the package as 
#                               found. By default we check that <PACKAGE>_FOUND is TRUE.
#    VERSION_VAR:STRING=<>    - Name of the variable which is defined by the find_package command
#                               to provide the version. By default we use <PACKAGE>_VERSION (or a 
#                               variation thereof).
#    BUILD_VERSION:STRING=<>  - Version of the package which is built (if required)
#    NO_BUILD:BOOL            - Don't build the package if not found.
#    DEPENDS:LIST=<>          - Dependencies of this package.
#
function(sequoia_find_package)
  set(options NO_BUILD)
  set(one_value_args PACKAGE BUILD_VERSION VERSION_VAR)
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

  set(version "${ARG_BUILD_VERSION}")

  # Do we use the system package or build it from source? 
  set(doc "Should we use the system ${ARG_PACKAGE}?")
  set(default_use_system ON)
  if(SEQUOIA_NO_SYSTEM_LIBS)
    set(default_use_system OFF)
  endif()

  option(USE_SYSTEM_${package_upper} ${doc} ${default_use_system})

  set(no_build OFF)
  if(DEFINED ARG_NO_BUILD AND ARG_NO_BUILD)
    set(no_build ON)
  endif()

  set(use_system FALSE)

  if(NOT(no_build) AND NOT(USE_SYSTEM_${package_upper}))
    set(USE_SYSTEM_${package_upper} OFF CACHE BOOL ${doc} FORCE)
    if(NOT(DEFINED ARG_NO_BUILD AND ARG_NO_BUILD))
      include(${external_file})
      set(found ON)
    else()
      set(found OFF)
    endif()
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
      message(STATUS "Package ${ARG_PACKAGE} not found due to missing: ${missing_required_vars}")    
    endif()
    
    if(required_vars_ok AND (${ARG_PACKAGE}_FOUND OR ${package_upper}_FOUND)) 
      set(use_system TRUE)
      set(USE_SYSTEM_${package_upper} ON CACHE BOOL ${doc} FORCE)

      # Forward arguments
      foreach(var ${ARG_FORWARD_VARS})
        if(DEFINED ${var})
          sequoia_export_package(PACKAGE ${ARG_PACKAGE} CMAKE_ARGS "-D${var}:PATH=${${var}}")
        endif()
      endforeach()

      # Try to detect the version we just found
      if(DEFINED ARG_VERSION_VAR)
        # Try the user variable
        set(version "${${ARG_VERSION_VAR}}")
      elseif(DEFINED ${ARG_PACKAGE}_VERSION_MAJOR AND
             DEFINED ${ARG_PACKAGE}_VERSION_MINOR AND
             DEFINED ${ARG_PACKAGE}_VERSION_PATCH)
        # SemVer (X.Y.Z)
        set(version 
            "${${ARG_PACKAGE}_VERSION_MAJOR}.${${ARG_PACKAGE}_VERSION_MINOR}.${${ARG_PACKAGE}_VERSION_PATCH}")
      elseif(DEFINED ${ARG_PACKAGE}_MAJOR_VERSION AND 
             DEFINED ${ARG_PACKAGE}_MINOR_VERSION AND 
             DEFINED ${ARG_PACKAGE}_SUBMINOR_VERSION)
        # Boost SemVer
        set(version 
            "${${ARG_PACKAGE}_MAJOR_VERSION}.${${ARG_PACKAGE}_MINOR_VERSION}.${${ARG_PACKAGE}_SUBMINOR_VERSION}")
      elseif(DEFINED ${ARG_PACKAGE}_VERSION)
        # Standard <PACKAGE>_VERSION
        set(version "${${ARG_PACKAGE}_VERSION}")
      elseif(DEFINED ${package_upper}_VERSION)
        # <PACKAGE>_VERSION with <PACKAGE> all uppercase
        set(version "${${package_upper}_VERSION}")
      else()
        # give up!
        set(version "unknown")
      endif()

      set(found ON)
    else()
      set(USE_SYSTEM_${package_upper} OFF CACHE BOOL ${doc} FORCE)
      if(NOT(DEFINED ARG_NO_BUILD AND ARG_NO_BUILD))
        include(${external_file})
        set(found ON)
      else()
        set(found OFF)
      endif()
    endif()
  endif()

  # Set the dependencies if we build
  if(NOT(use_system) AND ARG_DEPENDS)
    foreach(dep ${ARG_DEPENDS})
      sequoia_compute_optional_dependency(${dep} package_dependencies)
    endforeach()

    if(package_dependencies)
      add_dependencies(${target} ${package_dependencies})
    endif()
  endif()

  if("${version}" STREQUAL "")
    set(version "-")
  endif()

  set(SEQUOIA_${package_upper}_FOUND ${found} CACHE BOOL "Was ${package} found" FORCE)
  sequoia_make_package_info(${ARG_PACKAGE} ${version} ${use_system} ${found})
endfunction()
