##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

sequoia_add_library(
  NAME SequoiaGame
  SOURCES Export.h
          Game.h
          Game.cpp
  DEPENDS SequoiaRender
          SequoiaCore
          ${SEQUOIA_EXTERNAL_LIBRARIES}   
)

