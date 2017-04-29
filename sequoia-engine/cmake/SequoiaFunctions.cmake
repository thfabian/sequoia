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

sequoia_include_guard()

include(CMakeParseArguments)

## sequoia_report_result
## ---------------------
##
## Report a list of strings as STATUS.
##
##    FILE:STRING=<>             - Header of the current report.
##    ARGN:STRING=<>             - List of strings to report.
##
function(sequoia_report_result HEADER)
  string(LENGTH ${HEADER} header_length)
  set(full_header "----------------------------------------------------------------")
  math(EXPR right_header_length "43 - ${header_length}")
  string(SUBSTRING ${full_header} "0" "${right_header_length}" right_header)
  
  message(STATUS "------------------- ${HEADER} ${right_header}")
  foreach(arg ${ARGN})
    message(STATUS "${arg}")
  endforeach()
  message(STATUS "${full_header}")
endfunction()

## sequoia_run_cppcheck
## --------------------
##
## Run Cppcheck on all source files. A new target will be created `Cppcheck-${TARGET}` and a 
## dependency will be added to the `cppcheck` target.
##
##    TARGET:STRING=<>           - CMake target to retrieve the includes.
##    SOURCES:STRING=<>          - List of source files.
##
function(sequoia_run_cppcheck TARGET)
  if(NOT TARGET ${TARGET})
    message(FATAL_ERROR "${TARGET} is not a valid CMake target")
  endif()

  set(includes "$<TARGET_PROPERTY:${TARGET},INCLUDE_DIRECTORIES>")
  get_directory_property(definitions COMPILE_DEFINITIONS)

  add_custom_target(
    Cppcheck-${TARGET}
    COMMAND
      ${CPPCHECK_EXECUTABLE}
          "$<$<BOOL:${includes}>:-I$<JOIN:${includes},\t-I>>"
          "$<$<BOOL:${definitions}>:-D$<JOIN:${definitions},\t-D>>"
          --check-config
          --enable=warning,performance,portability,information,missingInclude
          --std=c++11
          --verbose
          --suppress=missingIncludeSystem
          ${ARGN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  add_dependencies(Cppcheck-${TARGET} ${TARGET})
  
  if(NOT TARGET cppcheck)
    add_custom_target(cppcheck COMMENT "Running Cppcheck")
  endif()
  add_dependencies(cppcheck Cppcheck-${TARGET})
endfunction()

## sequoia_add_library
## -------------------
##
## Create a static/shared library depending on the varialble BUILD_SHARED_LIBS.
## All header files (".h") in the directory of the source files are treated as dependencies of the
## library.
##    
##    NAME:STRING=<>             - Name of the library.
##    SOURCES:STRING=<>          - List of source files.
##    DEPENDS:STRING=<>          - List of external libraries and/or CMake targets treated as
##                                 dependencies of the library.
##
function(sequoia_add_library)
  #
  # Parse arguments
  #
  set(options)
  set(one_value_args NAME)
  set(multi_value_args SOURCES DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  
  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  
  #
  # Add library
  #
  add_library(${ARG_NAME} ${ARG_SOURCES})
  target_link_libraries(${ARG_NAME} ${ARG_DEPENDS})

  if(SEQUOIA_HAS_CPPCHECK)
    sequoia_run_cppcheck(${ARG_NAME} ${ARG_SOURCES})
  endif()
  
  if(SEQUOIA_COMPILER_MSVC)
    set_property(TARGET ${ARG_NAME} PROPERTY ARCHIVE_OUTPUT_DIRECTORY 
                 ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
    set_property(TARGET ${ARG_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY 
                 ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
    set_property(TARGET ${ARG_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY 
                 ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
  else()
    set_property(TARGET ${ARG_NAME} PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set_property(TARGET ${ARG_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set_property(TARGET ${ARG_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  endif()
endfunction()

## sequoia_add_executable
## ----------------------
##
## Create an executable.
## All header files (".h") in the directory of the source files are treated as dependencies of the
## library.
##    
##    NAME:STRING=<>             - Name of the test.
##    SOURCES:STRING=<>          - List of source files.
##    WIN32_APPLICATION          - Build an executable with a WinMain entry point on windows.
##    DEPENDS:STRING=<>          - List of external libraries and/or CMake targets to link against.
##
function(sequoia_add_executable)
  #
  # Parse arguments
  #
  set(options WIN32_APPLICATION)
  set(one_value_args NAME)
  set(multi_value_args SOURCES DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  
  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  
  #
  # Add executable
  #
  if(ARG_WIN32_APPLICATION)
    add_executable(${ARG_NAME} WIN32 ${ARG_SOURCES})
  else()
    add_executable(${ARG_NAME} ${ARG_SOURCES})
  endif()
  
  target_link_libraries(${ARG_NAME} ${ARG_DEPENDS})
  if(SEQUOIA_COMPILER_MSVC)
    set_property(TARGET ${ARG_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY 
                 ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
  else()
    set_property(TARGET ${ARG_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  endif()
endfunction()

## sequoia_add_test
## ----------------
##
## Create an executable and register it in CTest.
## All header files (".h") in the directory of the source files are treated as dependencies of the
## library.
##    
##    NAME:STRING=<>             - Name of the test.
##    SOURCES:STRING=<>          - List of source files.
##    DEPENDS:STRING=<>          - List of external libraries and/or CMake targets to link against.
##
function(sequoia_add_test)
  #
  # Parse arguments
  #
  set(options)
  set(one_value_args NAME)
  set(multi_value_args SOURCES DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  
  #
  # Add executable
  #
  sequoia_add_executable(${ARGN})
  
  #
  # Add test
  #
  add_test(NAME CTest-${ARG_NAME} COMMAND $<TARGET_FILE:${ARG_NAME}> --gtest_color=yes)

endfunction()

## sequoia_configure_file
## ----------------------
##
## Configure file and store the output in the same direcotry hirarchy as the input while 
## substituting ${CMAKE_SOURCE_DIR} for ${CMAKE_BINARY_DIR}.
##
## If the input file has a ".cmake" extension, it will be removed in the output.
##
##    FILE:STRING=<>             - File to configure.
##
function(sequoia_configure_file FILE)
  get_filename_component(absolute_path ${FILE} ABSOLUTE)
  file(RELATIVE_PATH relative_path ${CMAKE_SOURCE_DIR} ${absolute_path})

  get_filename_component(output_dir ${relative_path} DIRECTORY)
  get_filename_component(output_name_cmake ${relative_path} NAME)
  string(REGEX REPLACE ".cmake" "" output_file_name ${output_name_cmake})
  
  set(input_file ${FILE})
  set(output_file "${CMAKE_BINARY_DIR}/${output_dir}/${output_file_name}")
  
  configure_file(${input_file} ${output_file})
  
  # TODO: Problem here as ${install_dir} still has the "src"
  set(install_dir "${CMAKE_INSTALL_PREFIX}/${output_dir}")
  #install(FILES ${output_file} DESTINATION ${install_dir})
endfunction(sequoia_configure_file)

