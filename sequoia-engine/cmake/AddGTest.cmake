##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

find_package(GTest REQUIRED)

include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
sequoia_export_package_variable(
  GTEST 
  ${GTEST_FOUND} 
  "GTest: Found" 
  ${GTEST_LIBRARIES}
)
