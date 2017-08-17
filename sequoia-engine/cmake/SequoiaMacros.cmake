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

sequoia_include_guard()

include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)
include(CMakeParseArguments)

## sequoia_check_cxx_flag
## ----------------------
##
## Test if the compiler flag exists.
##
##    FLAG:STRING=<>   - Compiler flag to check (e.g -O3)
##    NAME:STRING=<>   - Name of the check (e.g HAVE_GCC_O3)
##
macro(sequoia_check_cxx_flag FLAG NAME)
  check_cxx_compiler_flag("${FLAG}" ${NAME})
endmacro()

## sequoia_check_and_set_cxx_flag
## ------------------------------
##
## Test if the compiler flag exists and add it to the ``CMAKE_CXX_FLAGS``.
##
##    FLAG:STRING=<>   - Compiler flag to check (e.g -O3)
##    NAME:STRING=<>   - Name of the check (e.g HAVE_GCC_O3)
##
macro(sequoia_check_and_set_cxx_flag FLAG NAME)
  sequoia_check_cxx_flag("${FLAG}" ${NAME})
  if(${NAME})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
  endif()
endmacro()

## sequoia_set_cxx_standard
## ------------------------
##
## Set the minimum standard of C++.
##
##    MIN_CXX_STANDARD:STRING=<>   - Minimum C++ standard which needs to be supported, one of 
##                                   ["c++14" "c++17"].
##
macro(sequoia_set_cxx_standard MIN_CXX_STANDARD)
    set(supported_standards "c++14" "c++17")    
    
    if(NOT("${MIN_CXX_STANDARD}" IN_LIST supported_standards))
      message(FATAL_ERROR 
              "Unknown C++ standard (${MIN_CXX_STANDARD}), supported values: ${supported_standards}")
    endif()
    
    # Try the latest possible standards first (-std=c++1z or /std:c++latest)    
    if(WIN32 AND NOT MINGW)
      #set(std_cxx_latest "c++latest")
      # TODO: We need to use boost >= 1.63 for this
    else()
      set(std_cxx_latest "c++17")      
    endif()
    
    set(cxx_stanard_flag)
    foreach(std_cxx ${std_cxx_latest} ${MIN_CXX_STANDARD})
      
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
      check_cxx_compiler_flag("${cxx_flag}" HAVE_${STD_CXX}_STANDARD_SUPPORT)
    
      if(HAVE_${STD_CXX}_STANDARD_SUPPORT)
        set(cxx_stanard_flag ${cxx_flag})
        break()
      endif()
    endforeach()
    
    if(NOT(cxx_stanard_flag))
      message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no ${MIN_CXX_STANDARD} support.")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxx_stanard_flag}")
endmacro()

## sequoia_check_and_set_arch_flag
## -------------------------------
##
## Test for the most recent architecture (this mimics -march=native of gcc for MSVC).
##
macro(sequoia_check_and_set_arch_flag)
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

## sequoia_set_cxx_flags
## ---------------------
##
## Set C++ flags.
##
macro(sequoia_set_cxx_flags)

  # Remove -DNDEBUG flag if ASSERTS are ON
  if(SEQUOIA_ASSERTS)
    if(WIN32)
      set(ndebug_flag "/DNDEBUG" "/D NDEBUG")
    else()
      set(ndebug_flag "-DNDEBUG")
    endif()
    foreach(flag ${ndebug_flag})
      string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
      string(REPLACE "${flag}" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    endforeach()
  else()
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
    sequoia_check_and_set_arch_flag()

    # Optimization
    if(SEQUOIA_OPTIMIZE)
      # TODO: figure out the LTO flags etc...
    endif()
    
    # C4244 - conversion from 'type1' to 'type2', possible loss of data
    sequoia_check_and_set_cxx_flag("/wd4244" HAVE_MSVC_WD4244)
    
    # C4661 - 'function' : no prototype provided; assumed no parameters
    sequoia_check_and_set_cxx_flag("/wd4661" HAVE_MSVC_WD4661)
    
    # C4267 - 'var' : conversion from 'size_t' to 'type', possible loss of data
    sequoia_check_and_set_cxx_flag("/wd4267" HAVE_MSVC_WD4267)
    
  #
  # GCC/Clang
  #    
  else()
    # Architecture
    sequoia_check_and_set_cxx_flag("-march=native" HAVE_GCC_MARCH_NATIVE)
    
    # Optimization
    if(SEQUOIA_OPTIMIZE)
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
        set(SEQUOIA_USE_LTO ON CACHE INTERNAL "")
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
    sequoia_check_and_set_cxx_flag("-Wsign-promo" HAVE_GCC_WSIGN_PROMO)
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

    if(SEQUOIA_USE_CCACHE)
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

## sequoia_check_compiler
## ----------------------
##
## Check whether the current compiler is supported, and provides friendly hints to the user.
##
macro(sequoia_check_compiler)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "3.4")
          message(WARNING "
      ### You appear to be using Clang ${CMAKE_CXX_COMPILER_VERSION}, which is known
      ### to be unable to compile Sequoia. Consider switching to
      ### Clang >= 3.5. If it is already installed on your
      ### system, you can tell CMake about it with
      ###
      ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/clang
          ")
      endif()
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
      if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "6.1.0")
          message(WARNING "
      ### You appear to be using Apple's Clang ${CMAKE_CXX_COMPILER_VERSION}, which is
      ### shipped with Xcode < 6.3. Unfortunately, only Apple's Clang
      ### >= 6.1.0 (shipped with Xcode >= 6.3) is supported by Sequoia.
      ### You should consider updating to Xcode >= 6.3 (requires Yosemite)
      ### or using a non-Apple Clang >= 3.4, which can be installed via
      ### Homebrew with
      ###
      ###     brew install llvm --with-clang
      ###
      ### You can then tell CMake to use that non-system Clang with
      ###
      ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/clang
          ")
      endif()
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
      if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "17")
          message(WARNING "
      ### You appear to be using Intel's ICC ${CMAKE_CXX_COMPILER_VERSION}, which is known
      ### to be unable to compile Sequoia. Consider switching to
      ### ICC >= 17.0. If it is already installed on your
      ### system, you can tell CMake about it with
      ###
      ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/icpc
          ")
      endif()
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "5.0")
          message(WARNING "
      ### You appear to be using GCC ${CMAKE_CXX_COMPILER_VERSION}, which is known to be
      ### unable to compile Sequoia. Only GCC >= 5.0 is supported.
      ### Consider using a more recent GCC or switching to Clang.
      ### If a more recent compiler is already installed on your
      ### system, you can tell CMake to use it with
      ###
      ###     cmake .. -DCMAKE_CXX_COMPILER=/path/to/g++
          ")
      endif()
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
      if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "19.0")
          message(WARNING "
      ### You appear to be using Visual Studio ${CMAKE_CXX_COMPILER_VERSION}, 
      ### which is known to be unable to compile Sequoia. 
      ### Only Visual Studio 14 2015 is currently supported.
          ")
      endif()
  else()
      message(WARNING "
      ### You appear to be using a compiler that is not yet tested with Sequoia.
      ")
  endif()
endmacro()

## sequoia_add_target_clang_format
## -------------------------------
##
## Setup a target for clang-format (if it was found):
##    check-format      Runs clang format and exits with a non-zero exit code if any files need to 
##                      be reformatted
##    format            Runs clang format and updates files in place
##
## If clang-format cannot be found, try setting CLANG_FORMAT_BIN to the clang-format binary.
## The targets are only available on Unix like systems.
##
##    SOURCE_DIR:STRING=<>      - Run clang-format recursivly on all files in SOURCE_DIR with "*h"
##                                and ".cpp" extensions. 
##
macro(sequoia_add_target_clang_format SOURCE_DIR) 
  
  find_program(CLANG_FORMAT_BIN 
    NAMES clang-format-3.8 clang-format-3.7 clang-format-3.6 clang-format
    PATHS ${CLANG_FORMAT_PATH} $ENV{CLANG_FORMAT_PATH} /usr/local/bin /usr/bin 
          NO_DEFAULT_PATH
  )

  if(NOT(SEQUOIA_ON_WIN32) AND NOT("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND"))
    if("$ENV{CMAKE_EXPORT_COMPILE_COMMANDS}" STREQUAL "1" OR CLANG_TIDY_FOUND)
      # Generate a Clang compile_commands.json "compilation database" file for use
      # with various development tools, such as Vim's YouCompleteMe plugin.
      # See http://clang.llvm.org/docs/JSONCompilationDatabase.html
      set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
    endif()

    # Runs clang format and updates files in place.
    add_custom_target(format 
                      COMMAND ${CMAKE_SOURCE_DIR}/cmake/scripts/run-clang-format.sh 
                      ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CLANG_FORMAT_BIN} 
                      1
                      `find ${SOURCE_DIR} -name \*.h -print -o -name \*.cpp -print`)

    # Runs clang format and exits with a non-zero exit code if any files need to be reformatted
    add_custom_target(check-format 
                      COMMAND ${CMAKE_SOURCE_DIR}/cmake/scripts/run-clang-format.sh 
                      ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CLANG_FORMAT_BIN} 
                      0
                      `find ${SOURCE_DIR} -name \*.h -print -o -name \*.cpp -print`)
  endif()
endmacro()
