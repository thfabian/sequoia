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

find_package(Threads REQUIRED)

if(CMAKE_USE_PTHREADS_INIT)
  set(threadlib "pthread")
elseif(CMAKE_USE_WIN32_THREADS_INIT)
  set(threadlib "WIN32 threads")
elseif(CMAKE_USE_SPROC_INIT)
  set(threadlib "sproc")
elseif(CMAKE_HP_PTHREADS_INIT)
  set(threadlib "hp pthreads")
endif()

sequoia_export_package(
  PACKAGE Threads
  FOUND ON 
  VERSION_STR "${threadlib}"
  LIBRARIES ${CMAKE_THREAD_LIBS_INIT}
)
