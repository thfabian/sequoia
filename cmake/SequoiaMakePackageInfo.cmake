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

# sequoia_make_package_info
# -------------------------
#
# Append a package info string to the global variable SEQUOIA_PACKAGE_INFO for PACKAGE.
#
#    PACKAGE_NAME:STRING=<>      - Name of the package
#    PACKAGE_VERSION:STRING=<>   - Version of the package
#    USE_SYSTEM:BOOL=<>          - Do we use the system version of the package?
#
macro(sequoia_make_package_info PACKAGE_NAME PACKAGE_VERSION USE_SYSTEM)
  string(LENGTH ${PACKAGE_NAME} package_name_length)
  math(EXPR indent_length "20 - ${package_name_length}")

  set(full_indent "                    ") 
  string(SUBSTRING ${full_indent} "0" "${indent_length}" indent)

  set(version_str "")
  if(NOT(PACKAGE_VERSION STREQUAL ""))
    set(version_str "(ver ${PACKAGE_VERSION})")
  endif()

  if(${USE_SYSTEM})
    set(SEQUOIA_PACKAGE_INFO 
      "${PACKAGE_NAME}${indent}: found    ${version_str};${SEQUOIA_PACKAGE_INFO}")
  else()
    set(SEQUOIA_PACKAGE_INFO 
      "${SEQUOIA_PACKAGE_INFO};${PACKAGE_NAME}${indent}: building ${version_str}")
  endif()
endmacro()