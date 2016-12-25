##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

sequoia_add_library(
  NAME SequoiaDriver
  SOURCES Driver.cpp
          CommandLine.cpp
          Win32Console.cpp
  DEPENDS SequoiaGame
          SequoiaCore
          ${SEQUOIA_BOOST_LIBRARIES}
          ${SEQUOIA_OGRE_LIBRARIES}
          ${SEQUOIA_THREADS_LIBRARIES}
)

sequoia_add_executable(
  NAME Sequoia
  WIN32_APPLICATION
  SOURCES Main.cpp
  DEPENDS SequoiaDriver
          SequoiaGame
          SequoiaCore
          ${SEQUOIA_BOOST_LIBRARIES}
          ${SEQUOIA_OGRE_LIBRARIES}
          ${SEQUOIA_THREADS_LIBRARIES}
)

