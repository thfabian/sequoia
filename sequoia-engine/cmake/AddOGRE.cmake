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

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  list(APPEND ogre_libraries ${OGRE_LIBRARY_DBG})
else()
  list(APPEND ogre_libraries ${OGRE_LIBRARY_REL})
endif()

# Components
foreach(component Terrain Paging Property)
  if(OGRE_${component}_FOUND)
    # Todo.. handle Debug/Release
    if(OGRE_${component}_LIBRARY_REL)
      list(APPEND ogre_libraries ${OGRE_${component}_LIBRARY_REL})
    endif()
  endif()
endforeach()

# Plugins
foreach(plugin RenderSystem_GL)
  if(OGRE_${plugin}_FOUND)
    if(OGRE_${plugin}_LIBRARY_REL)
        # Todo.. handle Debug/Release
      list(APPEND ogre_libraries ${OGRE_${plugin}_LIBRARY_REL})
    endif()
    list(APPEND OGRE_PLUGINS ${plugin})
  endif()
endforeach()

sequoia_export_package_variable(
  OGRE 
  ${OGRE_FOUND} 
  "Ogre: ${OGRE_VERSION}" 
  ${ogre_libraries}
)
	
