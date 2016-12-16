##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

set(_source "${CMAKE_CURRENT_SOURCE_DIR}/sequoia-engine")
set(_build "${CMAKE_CURRENT_BINARY_DIR}/sequoia-engine")

unset(_deps)
sequoia_add_optional_deps(_deps "${projects}")

ExternalProject_Add(
  sequoia-engine
  SOURCE_DIR ${_source}
  BINARY_DIR ${_build}
  CMAKE_CACHE_ARGS
    ${Sequoia_DEFAULT_ARGS}
    ${Sequoia_THIRDPARTYLIBS_ARGS}
  DEPENDS
    ${_deps}
)
