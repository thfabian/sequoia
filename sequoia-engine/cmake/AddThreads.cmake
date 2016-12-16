##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                   S E R I A L B O X
##
## This file is distributed under terms of BSD license. 
## See LICENSE.txt for more information.
##
##===------------------------------------------------------------------------------------------===##

if(NOT(SEQUOIA_THREADS_FOUND))
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
  
  sequoia_export_package_variable(THREADS ON "Threads: ${threadlib}" ${CMAKE_THREAD_LIBS_INIT})
endif()

