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

include(SequoiaFindPackage)

# sequoia_build_package
# ---------------------
#
# Build the package <PACKAGE> by including the file "External_<PACKAGE>" which should define a 
# target <PACKAGE> (in all lower case) which is used to built the package.
#
#    PACKAGE:STRING=<>        - Name of the package
#    BUILD_VERSION:STRING=<>  - Version of the package which is built (if required)
#    DEPENDS:LIST=<>          - Dependencies of this package.
#
macro(sequoia_build_package)
  set(options)
  set(one_value_args PACKAGE BUILD_VERSION)
  set(multi_value_args DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  string(TOUPPER ${ARG_PACKAGE} package_upper)
  option(USE_SYSTEM_${package_upper} OFF)

  sequoia_find_package(
    PACKAGE ${ARG_PACKAGE}
    BUILD_VERSION ${ARG_BUILD_VERSION}
    DEPENDS ${ARG_DEPENDS}
  )
endmacro()
