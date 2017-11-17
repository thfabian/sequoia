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
include(CMakeParseArguments)

#.rst:
# sequoia_enable_clang_msan
# -------------------------
#
# Enable Clang's `memory sanitizer`_.
#
# .. code-block:: cmake
#
#   sequoia_enable_clang_msan([BLACKLIST_SRC])
#
# ``BLACKLIST_SRC``
#   List of directories to blacklist (e.g ``/usr/lib/*`` to ignore all system installed libraries).
#
# .. _memory sanitizer: https://clang.llvm.org/docs/MemorySanitizer.html
#
macro(sequoia_enable_clang_msan)
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(options)
    set(one_value_args NAME)
    set(multi_value_args BLACKLIST_SRC)
    cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    sequoia_check_and_set_cxx_flag("-fno-omit-frame-pointer" HAVE_GCC_NO_OMIT_FRAME_POINTER)
    sequoia_check_and_set_cxx_flag("-fno-optimize-sibling-calls" HAVE_GCC_NO_INLINING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")

    if(ARG_BLACKLIST_SRC)
      set(black_list_file "${CMAKE_CURRENT_BINARY_DIR}/SequoiaEnableClangMSan-Blacklist.txt")
      file(WRITE ${black_list_file} "# ${PROJECT_NAME} blacklist\n")
      foreach(src ${ARG_BLACKLIST_SRC})
        file(APPEND ${black_list_file} "src:${src}\n")
      endforeach()
      sequoia_check_and_set_cxx_flag("-fsanitize-blacklist=${black_list_file}" 
                                     HAVE_CLANG_SANITIE_BLACKLIST)
    endif()
  endif()
endmacro()
