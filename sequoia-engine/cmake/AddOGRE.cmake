##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##

find_package(OGRE REQUIRED)

include_directories(SYSTEM ${OGRE_INCLUDE_DIRS})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  list(APPEND ogre_libraries ${OGRE_LIBRARY_DBG})
else()
  list(APPEND ogre_libraries ${OGRE_LIBRARY_REL})
endif()

if(WIN32)
  foreach(build_type Release Debug RelWithDebInfo)
    file(COPY "${OGRE_BINARY_REL}" 
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/${build_type}")
  endforeach()
endif()

set(ogre_components Overlay Terrain Paging Property)
set(ogre_plugins RenderSystem_GL RenderSystem_Direct3D11)

#
# Components
#
foreach(component ${ogre_components})
  if(OGRE_${component}_FOUND)
    if(OGRE_${component}_LIBRARY_REL)
      list(APPEND ogre_libraries ${OGRE_${component}_LIBRARY_REL})
    endif()

    # Additional include directories for ${component}
    if(OGRE_${component}_INCLUDE_DIRS)
      include_directories(SYSTEM ${OGRE_${component}_INCLUDE_DIRS})
    endif()
    
    # Copy DLL
    if(WIN32)
      foreach(build_type Release Debug RelWithDebInfo)
        file(COPY "${OGRE_${component}_BINARY_REL}" 
             DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/${build_type}")
      endforeach()
    endif()
  endif()
endforeach()

#
# Plugins
#
foreach(plugin ${ogre_plugins})
  if(OGRE_${plugin}_FOUND)
  
    if(OGRE_${plugin}_LIBRARY_REL)
      list(APPEND ogre_libraries ${OGRE_${plugin}_LIBRARY_REL})
    endif()
    list(APPEND OGRE_PLUGINS ${plugin})
    
    # Copy DLL
    if(WIN32)
      foreach(build_type Release Debug RelWithDebInfo)
        file(COPY "${OGRE_${plugin}_REL}" 
             DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/${build_type}")
      endforeach()
    endif()
  endif()
endforeach()

sequoia_export_package_variable(
  OGRE 
  ${OGRE_FOUND} 
  "Ogre: ${OGRE_VERSION}" 
  ${ogre_libraries}
)
	
