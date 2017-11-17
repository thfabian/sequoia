##===------------------------------------------------------------------------------*- CMake -*-===##
##                          _ 
##                         | |
##                       __| | __ ___      ___ ___ 
##                      / _` |/ _` \ \ /\ / / '_  |
##                     | (_| | (_| |\ V  V /| | | |
##                      \__,_|\__,_| \_/\_/ |_| |_| - Compiler Toolchain
##
##
##  This file is distributed under the MIT License (MIT). 
##  See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

include(SequoiaIncludeGuard)
sequoia_include_guard()

include(SequoiaCheckAndSetCXXFlag)

#.rst:
# sequoia_enable_clang_asan
# -------------------------
#
# Enable Clang's `address sanitizer`_.
#
# .. code-block:: cmake
#
#   sequoia_enable_clang_asan()
#
# .. _address sanitizer: https://clang.llvm.org/docs/AddressSanitizer.html
#
macro(sequoia_enable_clang_asan)
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    sequoia_check_and_set_cxx_flag("-fno-omit-frame-pointer" HAVE_GCC_NO_OMIT_FRAME_POINTER)
    sequoia_check_and_set_cxx_flag("-fno-optimize-sibling-calls" HAVE_GCC_NO_INLINING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  endif()
endmacro()
