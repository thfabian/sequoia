##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

sequoia_add_test(
  NAME SequoiaCoreTest
  SOURCES TestException.cpp
          TestString.cpp
          TestMain.cpp
          TestVersion.cpp
  DEPENDS SequoiaCore 
          ${SEQUOIA_BOOST_LIBRARIES}
          ${SEQUOIA_SFML_LIBRARIES}
          ${SEQUOIA_GTEST_LIBRARIES}
          ${SEQUOIA_THREADS_LIBRARIES}
)

