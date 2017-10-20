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

include(SequoiaIncludeGuard)
sequoia_include_guard()

include(SequoiaCheckAndSetCXXFlag)

#.rst:
# sequoia_set_cxx_standard
# ------------------------
#
# Set the minimum standard of C++.
#
# .. code-block:: cmake
#
#   sequoia_set_cxx_standard(MIN_CXX_STANDARD)
#
# ``MIN_CXX_STANDARD``
#   Minimum C++ standard which needs to be supported, one of 
#   [``c++11``, ``c++14``, ``c++1z``, ``c++17``, ``c++2a``].
#
macro(sequoia_set_cxx_standard MIN_CXX_STANDARD)
  set(supported_standards "c++11" "c++14" "c++1z" "c++17" "c++2a")    
  
  if(NOT("${MIN_CXX_STANDARD}" IN_LIST supported_standards))
    message(FATAL_ERROR 
            "Unknown C++ standard (${MIN_CXX_STANDARD}), supported values: ${supported_standards}")
  endif()
  
  set(std_cxx ${MIN_CXX_STANDARD})
    
  if(WIN32 AND NOT MINGW)
    set(cxx_flag "/std:${std_cxx}")
  elseif(CYGWIN)
    string(REPLACE "c" "gnu" ${std_cxx} gnu_standard)
    set(cxx_flag "-std=${gnu_standard}")
  else()
    set(cxx_flag "-std=${std_cxx}")
  endif()
    
  string(TOUPPER ${std_cxx} STD_CXX)
  string(REPLACE "+" "X" STD_CXX ${STD_CXX})
  sequoia_check_and_set_cxx_flag("${cxx_flag}" HAVE_${STD_CXX}_STANDARD_SUPPORT)
  
  if(NOT(HAVE_${STD_CXX}_STANDARD_SUPPORT))
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no ${MIN_CXX_STANDARD} support.")
  endif()
endmacro()
