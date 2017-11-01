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

find_package(OpenCV NO_MODULE REQUIRED core highgui imgcodecs imgproc)

# Select the configuration which matches the current BUILD_TYPE
get_property(configurations TARGET opencv_highgui PROPERTY IMPORTED_CONFIGURATIONS)
string(TOUPPER ${CMAKE_BUILD_TYPE} sequoia_engine_configuration)

if(${sequoia_engine_configuration} IN_LIST configurations)
  set(config ${sequoia_engine_configuration})
else()
  list(GET configurations 0 config) # Just take the first one
endif()

# Iterate all OpenCV targets and glob the location of the library and the dependencies
foreach(target ${OpenCV_LIBS})
  get_property(${target}_INCLUDE_DIRS TARGET ${target} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
  get_property(${target}_EXTERNAL_LIBRARIES TARGET  ${target} 
               PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES_${config})
  get_property(${target}_LIBRARY TARGET  ${target} 
               PROPERTY IMPORTED_LOCATION_${config})

  list(APPEND opencv_includes ${${target}_INCLUDE_DIRS})
  list(APPEND opencv_libraries ${${target}_LIBRARY} ${${target}_EXTERNAL_LIBRARIES})
endforeach()

# Due to dependencies between the targets we need to resolve all the targets
foreach(lib ${opencv_libraries})
  if(TARGET ${lib})
    get_property(location TARGET ${lib} PROPERTY IMPORTED_LOCATION_${config})
    list(APPEND opencv_libraries_resolved ${location})
  else()
    list(APPEND opencv_libraries_resolved ${lib})
  endif()
endforeach()

sequoia_export_package(
  NAME OpenCV
  FOUND ${OPENCV_FOUND} 
  VERSION ${OpenCV_VERSION} 
  LIBRARIES ${opencv_libraries_resolved}
  INCLUDE_DIRS ${opencv_includes}
)
