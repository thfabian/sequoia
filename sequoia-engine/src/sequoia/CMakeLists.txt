##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

add_subdirectory(Core)
#add_subdirectory(Driver)

if(SEQUOIA_TESTING)
  add_subdirectory(Unittest)
endif()

