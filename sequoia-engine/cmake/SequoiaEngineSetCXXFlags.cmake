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

include(SequoiaCheckAndSetCXXFlag)
include(CheckCXXSourceRuns)

# sequoia_engine_set_msvc_arch_flag
# ---------------------------------
#
# Test for the most recent architecture (this mimics -march=native of gcc for MSVC).
#
macro(sequoia_engine_set_msvc_arch_flag)
  if(SEQUOIA_COMPILER_MSVC) 
    set(CMAKE_REQUIRED_FLAGS)
    
    # Check AVX
    set(CMAKE_REQUIRED_FLAGS "/arch:AVX")  
    check_cxx_source_runs("
        #include <immintrin.h>
        int main() {
          __m256 a, b, c;
          const float src[8] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
          float dst[8];
          a = _mm256_loadu_ps(src);
          b = _mm256_loadu_ps(src);
          c = _mm256_add_ps(a, b);
          _mm256_storeu_ps(dst, c);
          for(int i = 0; i < 8; i++)
            if((src[i] + src[i]) != dst[i])
              return -1;
          return 0;
        }"
        HAVE_MSVC_AVX_EXTENSION)
  
    # Check AVX2
    set(CMAKE_REQUIRED_FLAGS "/arch:AVX2")
    check_cxx_source_runs("
        #include <immintrin.h>
        int main() {
          __m256i a, b, c;
          const int src[8] = {1, 2, 3, 4, 5, 6, 7, 8};
          int dst[8];
          a =  _mm256_loadu_si256((__m256i*)src);
          b =  _mm256_loadu_si256((__m256i*)src);
          c = _mm256_add_epi32( a, b );
          _mm256_storeu_si256((__m256i*)dst, c);
          for(int i = 0; i < 8; i++)
            if((src[i] + src[i]) != dst[i])
              return -1;
          return 0;
        }"
        HAVE_MSVC_AVX2_EXTENSION)
  
    # Set Flags
    if(HAVE_MSVC_AVX2_EXTENSION)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
    elseif(HAVE_MSVC_AVX_EXTENSION)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX")
    endif()
  endif()
endmacro()

# sequoia_engine_set_cxx_flags
# ----------------------------
#
# Set C++ flags.
#
macro(sequoia_engine_set_cxx_flags)
  if(NOT(SEQUOIA_ENGINE_ASSERTS))
    add_definitions(-DNDEBUG)
    add_definitions(-DSEQUOIA_DISABLE_ASSERTS)
  endif()

  #
  # MSVC
  #
  if(SEQUOIA_COMPILER_MSVC)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DNOMINMAX)
    add_definitions(-DUNICODE)
    
    sequoia_check_and_set_cxx_flag("/EHsc" HAVE_MSVC_EHSC)
    sequoia_engine_set_msvc_arch_flag()

    # Optimization
    if(SEQUOIA_ENGINE_OPTIMIZE)
      # TODO: figure out the LTO flags etc...
    endif()
    
    # C4244 - conversion from 'type1' to 'type2', possible loss of data
    sequoia_check_and_set_cxx_flag("/wd4244" HAVE_MSVC_WD4244)
    
    # C4661 - 'function' : no prototype provided; assumed no parameters
    sequoia_check_and_set_cxx_flag("/wd4661" HAVE_MSVC_WD4661)
    
    # C4267 - 'var' : conversion from 'size_t' to 'type', possible loss of data
    sequoia_check_and_set_cxx_flag("/wd4267" HAVE_MSVC_WD4267)
    
  #
  # GCC/Clang/Intel
  #    
  else()
    # Architecture
    sequoia_check_and_set_cxx_flag("-march=native" HAVE_GCC_MARCH_NATIVE)
    
    # Optimization
    if(SEQUOIA_ENGINE_OPTIMIZE)
      sequoia_check_and_set_cxx_flag("-ftree-vectorize" HAVE_GCC_TREE_VECTORIZE)
      sequoia_check_and_set_cxx_flag("-ffast-math" HAVE_GCC_FAST_MATH)
      sequoia_check_and_set_cxx_flag("-fomit-frame-pointer" HAVE_GCC_OMIT_FRAME_POINTER)

      # Enable link time optimization (LTO)
      set(use_lto OFF)
      if(SEQUOIA_COMPILER_GNU)
        sequoia_check_cxx_flag("-flto" HAVE_GCC_LTO)
        if(HAVE_GCC_LTO)
          # If we are using GCC we *need* to use the gcc-ar and gcc-ranlib for LTO to work properly.
          # https://gcc.gnu.org/ml/gcc-help/2016-01/msg00061.html
          get_filename_component(gcc_install_dir ${CMAKE_CXX_COMPILER} DIRECTORY)
          find_program(GCC_AR gcc-ar HINTS ${gcc_install_dir})
          find_program(GCC_RANLIB gcc-ranlib HINTS ${gcc_install_dir})
           
          if(GCC_AR AND GCC_RANLIB)
            set(CMAKE_AR ${GCC_AR})
            set(CMAKE_RANLIB ${GCC_RANLIB})
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
            set(use_lto ON)
          endif()

        endif()
      elseif(SEQUOIA_COMPILER_CLANG)
        sequoia_check_and_set_cxx_flag("-flto" HAVE_GCC_LTO)
        set(use_lto ${HAVE_GCC_LTO})
      endif()

      if(use_lto)  
        set(SEQUOIA_ENGINE_USE_LTO ON CACHE INTERNAL "")
      endif()
    endif()

    # Pthread
    sequoia_check_and_set_cxx_flag("-pthread" HAVE_GCC_PTHREAD)

    # Warnings
    sequoia_check_and_set_cxx_flag("-Wall" HAVE_GCC_WALL)
    sequoia_check_and_set_cxx_flag("-W" HAVE_GCC_W)
    sequoia_check_and_set_cxx_flag("-Wformat" HAVE_GCC_WFORMAT)
    sequoia_check_and_set_cxx_flag("-Wmissing-declarations" HAVE_GCC_WMISSING_DECLARATIONS)
    sequoia_check_and_set_cxx_flag("-Wmissing-prototypes" HAVE_GCC_WMISSING_PROTOTYPES)
    sequoia_check_and_set_cxx_flag("-Wstrict-prototypes" HAVE_GCC_WSTRICT_PROTOTYPES)
    sequoia_check_and_set_cxx_flag("-Wundef" HAVE_GCC_WUNDEF)
    sequoia_check_and_set_cxx_flag("-Winit-self" HAVE_GCC_WINI_SELF)
    sequoia_check_and_set_cxx_flag("-Wpointer-arith" HAVE_GCC_WPOINTER_ARITH)
    sequoia_check_and_set_cxx_flag("-Wshadow" HAVE_GCC_WSHADOW)
    sequoia_check_and_set_cxx_flag("-Werror=return-type" HAVE_GCC_WERROR_RETURN_TYPE)
    sequoia_check_and_set_cxx_flag("-Werror=non-virtual-dtor" HAVE_GCC_WERROR_NON_VIRTUAL_DTOR)
    sequoia_check_and_set_cxx_flag("-Werror=address" HAVE_GCC_WERROR_ADDRESS)
    sequoia_check_and_set_cxx_flag("-Werror=sequence-point" HAVE_GCC_WERROR_SEQUENCE_POINT)

    sequoia_check_and_set_cxx_flag("-Wno-sign-promo" HAVE_GCC_WNO_SIGN_PROMO)
    sequoia_check_and_set_cxx_flag("-Wno-sign-compare" HAVE_GCC_WNO_SIGN_COMPARE)
    sequoia_check_and_set_cxx_flag("-Wno-unused-parameter" HAVE_GCC_WNO_UNUSDED_PARAMETER)
    
    if(BUILD_SHARED_LIBS)
      sequoia_check_and_set_cxx_flag("-fPIC" HAVE_GCC_PIC)
    endif()

    if(SEQUOIA_ENGINE_USE_CCACHE)
      if(SEQUOIA_COMPILER_CLANG)
        sequoia_check_and_set_cxx_flag("-Qunused-arguments" HAVE_CLANG_UNUSED_ARGUMENTS)
        sequoia_check_and_set_cxx_flag("-fcolor-diagnostics" HAVE_CLANG_COLOR_DIAGNOSTICS)
        sequoia_check_and_set_cxx_flag("-Wno-undefined-var-template" 
                                       HAVE_CLANG_WNO_UNDEFINED_VAR_TEMPLATE)
      endif()

      if(SEQUOIA_COMPILER_GNU)
        sequoia_check_and_set_cxx_flag("-fdiagnostics-color=always" HAVE_GCC_COLOR_DIAGNOSTICS)
      endif()
    endif()
  endif()
endmacro()
