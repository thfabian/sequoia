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

# Platform specific falgs
if(UNIX)
  # Add GTK+ support for showing images
  set(platform_arg -DWITH_GTK:BOOL=ON -DWITH_GTK_2_X:BOOL=ON)
elseif(WIN32)
  # We always build with dynamic runtime (/MD)
  set(platform_arg -DBUILD_WITH_STATIC_CRT:BOOL=OFF -DWITH_WIN32UI:BOOL=ON)
endif()

ExternalProject_Add(
  opencv
  DOWNLOAD_DIR ${download_dir}
  URL ${opencv_url}
  URL_MD5 ${opencv_md5}
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/opencv"
  INSTALL_DIR "${Sequoia_INSTALL_PREFIX}/opencv"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>

    ${platform_arg}
    -DBUILD_DOCS:BOOL=OFF 
    -DBUILD_EXAMPLES:BOOL=OFF 
    -DBUILD_PACKAGE:BOOL=OFF 
    -DBUILD_PERF_TESTS:BOOL=OFF 
    -DBUILD_SHARED_LIBS:BOOL=OFF 
    -DBUILD_TBB:BOOL=OFF 
    -DBUILD_TESTS:BOOL=OFF 
    -DBUILD_WITH_DEBUG_INFO:BOOL=OFF

    # Modules
    -DBUILD_opencv_core:BOOL=ON
    -DBUILD_opencv_highgui:BOOL=ON
    -DBUILD_opencv_imgcodecs:BOOL=ON
    -DBUILD_opencv_imgproc:BOOL=ON
    
    -DBUILD_opencv_apps:BOOL=OFF
    -DBUILD_opencv_calib3d:BOOL=OFF
    -DBUILD_opencv_cudaarithm:BOOL=OFF
    -DBUILD_opencv_cudabgsegm:BOOL=OFF
    -DBUILD_opencv_cudacodec:BOOL=OFF
    -DBUILD_opencv_cudafeatures2d:BOOL=OFF
    -DBUILD_opencv_cudafilters:BOOL=OFF
    -DBUILD_opencv_cudaimgproc:BOOL=OFF
    -DBUILD_opencv_cudalegacy:BOOL=OFF
    -DBUILD_opencv_cudaobjdetect:BOOL=OFF
    -DBUILD_opencv_cudaoptflow:BOOL=OFF
    -DBUILD_opencv_cudastereo:BOOL=OFF
    -DBUILD_opencv_cudawarping:BOOL=OFF
    -DBUILD_opencv_cudev:BOOL=OFF
    -DBUILD_opencv_features2d:BOOL=OFF
    -DBUILD_opencv_flann:BOOL=OFF
    -DBUILD_opencv_ml:BOOL=OFF
    -DBUILD_opencv_objdetect:BOOL=OFF
    -DBUILD_opencv_photo:BOOL=OFF
    -DBUILD_opencv_python2:BOOL=OFF
    -DBUILD_opencv_shape:BOOL=OFF
    -DBUILD_opencv_stitching:BOOL=OFF
    -DBUILD_opencv_superres:BOOL=OFF
    -DBUILD_opencv_ts:BOOL=OFF
    -DBUILD_opencv_video:BOOL=OFF
    -DBUILD_opencv_videoio:BOOL=OFF
    -DBUILD_opencv_videostab:BOOL=OFF
    -DBUILD_opencv_world:BOOL=OFF

    # External library support
    -DWITH_PNG:BOOL=ON
    -DWITH_TIFF:BOOL=ON
    -DWITH_JPEG:BOOL=ON
    
    -DWITH_1394:BOOL=OFF
    -DWITH_ARAVIS:BOOL=OFF
    -DWITH_CLP:BOOL=OFF
    -DWITH_CUBLAS:BOOL=OFF
    -DWITH_CUDA:BOOL=OFF
    -DWITH_CUFFT:BOOL=OFF
    -DWITH_EIGEN:BOOL=OFF                     
    -DWITH_FFMPEG:BOOL=OFF                     
    -DWITH_GDAL:BOOL=OFF                       
    -DWITH_GDCM:BOOL=OFF   
    -DWITH_GIGEAPI:BOOL=OFF
    -DWITH_GPHOTO2:BOOL=OFF
    -DWITH_GSTREAMER:BOOL=OFF
    -DWITH_GSTREAMER_0_10:BOOL=OFF
    -DWITH_IPP:BOOL=OFF
    -DWITH_IPP_A:BOOL=OFF
    -DWITH_LAPACK:BOOL=OFF
    -DWITH_LIBV4L:BOOL=OFF
    -DWITH_MATLAB:BOOL=OFF
    -DWITH_NVCUVID:BOOL=OFF
    -DWITH_OPENCL:BOOL=OFF
    -DWITH_OPENCLAMDBLAS:BOOL=OFF
    -DWITH_OPENCLAMDFFT:BOOL=OFF
    -DWITH_OPENCL_SVM:BOOL=OFF
    -DWITH_OPENEXR:BOOL=OFF
    -DWITH_OPENGL:BOOL=OFF
    -DWITH_OPENMP:BOOL=OFF
    -DWITH_OPENNI:BOOL=OFF                                                                                                                               
    -DWITH_OPENNI2:BOOL=OFF
    -DWITH_OPENVX:BOOL=OFF
    -DWITH_PTHREADS_PF:BOOL=OFF
    -DWITH_PVAPI:BOOL=OFF
    -DWITH_QT:BOOL=OFF
    -DWITH_TBB:BOOL=OFF
    -DWITH_UNICAP:BOOL=OFF                                                                                                                                            
    -DWITH_V4L:BOOL=OFF                                                                                                                               
    -DWITH_VA:BOOL=OFF                                                                                                                                                 
    -DWITH_VA_INTEL:BOOL=OFF                                                                         
    -DWITH_VTK:BOOL=OFF                                                                                                                                                 
    -DWITH_WEBP:BOOL=OFF                                                                                                     
    -DWITH_XIMEA:BOOL=OFF                                                                                                                                                 
    -DWITH_XINE:BOOL=OFF                 
)

ExternalProject_Get_Property(opencv install_dir)
set(OpenCV_DIR "${install_dir}" CACHE INTERNAL "")

if(WIN32)
  list(APPEND Sequoia_THIRDPARTY_CMAKE_ARGS "-DOpenCV_DIR:PATH=${OpenCV_DIR}")
else()
  list(APPEND Sequoia_THIRDPARTY_CMAKE_ARGS "-DOpenCV_DIR:PATH=${OpenCV_DIR}/share/OpenCV")
endif()

