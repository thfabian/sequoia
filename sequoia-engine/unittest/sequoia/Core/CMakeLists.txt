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

sequoia_add_test(
  NAME SequoiaCoreTest
  SOURCES TestArrayRef.cpp
          TestException.cpp
          TestGlobalConfiguration.cpp
          TestMain.cpp
          TestSmallVector.cpp
          TestSingletonManager.cpp
          TestString.cpp
          TestStringRef.cpp
          TestVersion.cpp
  DEPENDS SequoiaUnittest
          SequoiaCore
          ${SEQUOIA_BOOST_LIBRARIES}
          ${SEQUOIA_OGRE_LIBRARIES}
          ${SEQUOIA_GTEST_LIBRARIES}
          ${SEQUOIA_THREADS_LIBRARIES}
)

