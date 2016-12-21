##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

sequoia_add_library(
  NAME SequoiaUnittest
  SOURCES Environment.cpp
  DEPENDS ${SEQUOIA_BOOST_LIBRARIES}
          ${SEQUOIA_GTEST_LIBRARIES}
          ${SEQUOIA_THREADS_LIBRARIES}
)
