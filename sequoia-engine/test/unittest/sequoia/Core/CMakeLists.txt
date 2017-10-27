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

sequoia_engine_add_unittest(
  NAME SequoiaCoreTest
  SOURCES TestAlignedADT.cpp
          TestArrayRef.cpp
          TestColor.cpp
          TestDoubleBuffered.cpp
          TestFile.cpp
          TestFuture.cpp
          TestHash.cpp
          TestImage.cpp
          TestMain.cpp
          TestMemory.cpp
          TestMutex.cpp
          TestOptions.cpp
          TestPlatform.cpp
          TestSingletonManager.cpp
          TestSTLExtra.cpp
          TestStringRef.cpp
          TestVersion.cpp
)

