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

## sequoia_add_objects
## -------------------
##
## Create an "object" library of the given sources. The objects can be referred to with  
## $<TARGET_OBJECTS:NAME>.
##
## Use `sequoia_link_objects` to create the library.
##
##    NAME:STRING=<>             - Name of the object library.
##    SOURCES:STRING=<>          - List of source files.
##    DEPENDS:STRING=<>          - List of CMake targets this object library depends on.
##
function(sequoia_add_objects)
  # Parse arguments
  set(options)
  set(one_value_args NAME)
  set(multi_value_args SOURCES DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  
  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  
  # Treat .json files as header files
  foreach(source ${ARG_SOURCES})
    get_filename_component(ext ${source} EXT)
    if("${ext}" STREQUAL ".json")
      set_source_files_properties(${source} PROPERTIES HEADER_FILE_ONLY 1)
    endif()
  endforeach()
  
  # Add object library
  add_library(${ARG_NAME} OBJECT ${ARG_SOURCES})
  if(ARG_DEPENDS)
    add_dependencies(${ARG_NAME} ${ARG_DEPENDS})
  endif()
  
  # Add shared library flags
  if(BUILD_SHARED_LIBS)
    target_compile_definitions(${ARG_NAME} PRIVATE -DSEQUOIA_SHARED_LIBRARIES)
  endif()  
  
  # Use folders in Visual Studio  
  if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    foreach(src ${ARG_SOURCES})
      get_filename_component(dir ${src} DIRECTORY)
      # Keep directories (e.g GL/Foo.h goes into Sources/GL/Foo.h)
      if(dir)
        set(group "Sources\\${dir}")
        source_group("${group}" FILES ${src})
      else()
        source_group("Sources" FILES ${src})
      endif()
    endforeach()
  endif()
  
  if(SEQUOIA_HAS_CPPCHECK)
   sequoia_run_cppcheck(${ARG_NAME} ${ARG_SOURCES})
  endif()
endfunction()

## sequoia_link_objects
## --------------------
##
## Link the given objects into a single static or dynamic library (depending on BUILD_SHARED_LIBS)
##
##    NAME:STRING=<>             - Name of the library.
##    OBJECTS:STRING=<>          - List of CMake targets whose objects files will be used. Note this
##                                 effectivly links $<TARGET_OBJECTS:Target> into a library.
##    DEPENDS:STRING=<>          - List of external dependencies (i.e libraries)
##
function(sequoia_link_objects)
  # Parse arguments
  set(options)
  set(one_value_args NAME)
  set(multi_value_args SOURCES DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  
  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  
  set(objects)
  foreach(target ${ARG_SOURCES})
    list(APPEND objects $<TARGET_OBJECTS:${target}>)
  endforeach()
  
  add_library(${ARG_NAME} ${objects})
  target_link_libraries(${ARG_NAME} ${ARG_DEPENDS})
  
  if(WIN32 AND NOT(MSVC_IDE))
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
  # Parse arguments
  set(options WIN32_APPLICATION)
  set(one_value_args NAME)
  set(multi_value_args SOURCES DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  
  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  
  # Add executable
  if(ARG_WIN32_APPLICATION)
    add_executable(${ARG_NAME} WIN32 ${ARG_SOURCES})
  else()
    add_executable(${ARG_NAME} ${ARG_SOURCES})
  endif()
  
  # Use folders in Visual Studio  
  if(MSVC)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    foreach(src ${ARG_SOURCES})
      get_filename_component(dir ${src} DIRECTORY)
      if(dir)
        set(group "Sources\\${dir}")
        source_group("${group}" FILES ${src})
      else()
        source_group("Sources" FILES ${src})
      endif()
    endforeach()
  endif()
  
  target_link_libraries(${ARG_NAME} ${ARG_DEPENDS})
  if(WIN32 AND NOT(MSVC_IDE))
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
##    
##    NAME:STRING=<>             - Name of the test.
##    SOURCES:STRING=<>          - List of source files.
##    DEPENDS:STRING=<>          - List of external libraries and/or CMake targets to link against.
##
function(sequoia_add_test)
  # Parse arguments
  set(options)
  set(one_value_args NAME)
  set(multi_value_args SOURCES DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
  
  # Add executable
  sequoia_add_executable(${ARGN})
  
  # Register the test with CTest
  add_test(NAME CTest-${ARG_NAME} COMMAND $<TARGET_FILE:${ARG_NAME}> --gtest_color=yes)
endfunction()

## sequoia_generate_options
## ------------------------
##
## Generate an the option header and include file. The specification of the options is provided via
## JSON files and parsed using the sequoia-opt-gen.py script.
##
##    INPUT_HEADER:STRING=<>     - Template of the header file to generate.
##    OUTPUT_HEADER:STRING=<>    - Generated header file.
##    INPUT_INCLUDE:STRING=<>    - Template of the include file to generate.
##    OUTPUT_INCLUDE:STRING=<>   - Generated include file.
##    OPTIONS:STRING=<>          - List input JSON files containing the option specification.
##
function(sequoia_generate_options)
  # Parse arguments
  set(options)
  set(one_value_args INPUT_HEADER INPUT_INCLUDE OUTPUT_HEADER OUTPUT_INCLUDE)
  set(multi_value_args OPTIONS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT("${ARG_UNPARSED_ARGUMENTS}" STREQUAL ""))
    message(FATAL_ERROR "invalid argument ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  set(script ${CMAKE_SOURCE_DIR}/tools/opt-gen/opt-gen.py)
  set(script_path "\"${script}\"")
  set(script_args --input-header="${ARG_INPUT_HEADER}"
                  --input-include="${ARG_INPUT_INCLUDE}"
                  --output-header="${ARG_OUTPUT_HEADER}" 
                  --output-include="${ARG_OUTPUT_INCLUDE}")
  foreach(opt ${ARG_OPTIONS})
    list(APPEND script_args "\"${opt}\"")
  endforeach()

  string(REPLACE "${CMAKE_BINARY_DIR}/" "" header "${ARG_OUTPUT_HEADER}")
  
  # Run opt-gen.py
  add_custom_command(
    OUTPUT ${ARG_OUTPUT_HEADER} ${ARG_OUTPUT_INCLUDE}
    COMMAND ${PYTHON_EXECUTABLE} ${script_path} ${script_args}
    DEPENDS ${ARG_INPUT_HEADER} ${ARG_INPUT_INCLUDE} ${ARG_OPTIONS} ${script}
    COMMENT "Generating options header ${header}"
  )

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
endfunction()

