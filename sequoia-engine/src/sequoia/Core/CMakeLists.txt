##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

sequoia_configure_file(Config.h.cmake)

sequoia_add_library(
  NAME SequoiaCore
  SOURCES Assert.cpp
          Version.cpp
          Exception.cpp
          ErrorHandler.cpp
          SingletonManager.cpp
          SmallVector.cpp
          String.cpp
          StringRef.cpp
          GlobalConfiguration.cpp
  DEPENDS ${SEQUOIA_BOOST_LIBRARIES}
          ${SEQUOIA_OGRE_LIBRARIES}
          ${SEQUOIA_THREADS_LIBRARIES}
)
