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

sequoia_add_library(
  NAME SequoiaRender
  SOURCES Camera.h
          Camera.cpp
          Export.h
          Exception.h
          HardwareBuffer.h
          HardwareBufferVertex.h
          HardwareBufferIndex.h
          RenderFwd.h
          RenderSystem.h
          RenderSystemImpl.h
          RenderSystem.cpp
          RenderTarget.h
          RenderTarget.cpp
          RenderData.h
          RenderData.cpp
          RenderWindow.h
          ViewFrustum.h
          Viewport.h
          Viewport.cpp
          
          GL/GL.h          
          GL/GLRenderSystem.h
          GL/GLRenderSystem.cpp
          GL/GLRenderWindow.h
          GL/GLRenderWindow.cpp
          GL/GLCamera.h
          GL/GLCamera.cpp

  DEPENDS SequoiaCore
          ${SEQUOIA_EXTERNAL_LIBRARIES}
)
