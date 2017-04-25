##===------------------------------------------------------------------------------*- CMake -*-===##
##                         _____                        _       
##                        / ____|                      (_)      
##                       | (___   ___  __ _ _   _  ___  _  __ _ 
##                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
##                        ____) |  __/ (_| | |_| | (_) | | (_| |
##                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
##                                       | |                    
##                                       |_| 
##
## This file is distributed under the MIT License (MIT).
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

sequoia_configure_file(Config.h.cmake)

sequoia_add_library(
  NAME SequoiaCore
  SOURCES ArrayRef.h
          Any.h
          Assert.h
          #Assert.cpp          
          NString.h
          NString.cpp
          Singleton.h
          SingletonManager.h
          SingletonManager.cpp
  DEPENDS ${SEQUOIA_BOOST_LIBRARIES}
          ${SEQUOIA_THREADS_LIBRARIES}
)
