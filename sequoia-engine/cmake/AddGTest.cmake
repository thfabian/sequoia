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

find_package(GTest REQUIRED)
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})

# Copy DLL's to bin directories on windows
if(WIN32)
  foreach(build_type Release Debug RelWithDebInfo)
    foreach(gtest_lib ${GTEST_LIBRARIES})
    
      # GTEST DLLs are in ${GTEST_ROOT}/bin
      if(NOT(DEFINED GTEST_ROOT))
        get_filename_component(dir_tmp "${gtest_lib}" DIRECTORY)
        get_filename_component(gtest_root "${dir_tmp}" DIRECTORY)
        set(GTEST_ROOT ${gtest_root} CACHE STRING "Root directory of GTest libraries")
      endif()
      
      set(lib_dir "${GTEST_ROOT}/bin")
      get_filename_component(lib_name "${gtest_lib}" NAME_WE)
      
      file(COPY "${lib_dir}/${lib_name}.dll" 
           DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/${build_type}")
    endforeach()
  endforeach()
endif()

sequoia_export_package_variable(
  gtest 
  ${GTEST_FOUND} 
  "GTest: Found" 
  ${GTEST_LIBRARIES}
)
