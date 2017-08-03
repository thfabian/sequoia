#!/bin/bash 
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

# @brief Install opencv
#
# @param $1   Install directory
# @param $2   opencv version triple (X.Y.Z)
install_opencv() {
  pushd $(pwd)
  local start_time=$(date +%s)

  if [[ $# -lt 2 ]]; then
    fatal_error "argument mistmatch: ${FUNCNAME[0]} <install_prefix> <version>"
  fi

  local install_dir=$1
  shift
  local opencv_version=$1
  shift
  
  local opencv_install_dir=$install_dir/opencv-$opencv_version

  abort_and_cleanup() {
    rm -rf $opencv_install_dir && mkdir -p $opencv_install_dir 
    fatal_error "$1"
  }

  NOTICE "${FUNCNAME[0]}: Installing opencv $opencv_version into \"$opencv_install_dir\""
  mkdir -p ${opencv_install_dir}
  if [[ ! -z "$(ls -A ${opencv_install_dir})" ]]; then
    NOTICE "${FUNCNAME[0]}: Package already installed. Skipping."
  else
    local opencv_url=$(printf "https://github.com/opencv/opencv/archive/%s.tar.gz"                 \
                      ${opencv_version})

    NOTICE "${FUNCNAME[0]}: Downloading opencv $opencv_url ..."
    { wget --no-check-certificate -O - ${opencv_url} |                                             \
      tar --strip-components=1 -xz -C ${opencv_install_dir}; } ||                                  \
      abort_and_cleanup "Failed to download opencv from: $opencv_url"
    NOTICE "${FUNCNAME[0]}: Successfully downloaded $opencv_url"

    cd ${opencv_install_dir}
    NOTICE "${FUNCNAME[0]}: Starting to build opencv ..."
    mkdir build && cd build
    cmake ..  -DCMAKE_BUILD_TYPE=Release                                                           \
              -DCMAKE_INSTALL_PREFIX:PATH=${opencv_install_dir}                                    \
              -DBUILD_DOCS:BOOL=OFF                                                                \
              -DBUILD_EXAMPLES:BOOL=OFF                                                            \
              -DBUILD_PACKAGE:BOOL=OFF                                                             \
              -DBUILD_PERF_TESTS:BOOL=OFF                                                          \
              -DBUILD_TBB:BOOL=OFF                                                                 \
              -DBUILD_TESTS:BOOL=OFF                                                               \
              -DBUILD_WITH_DEBUG_INFO:BOOL=OFF                                                     \
                                                                                                   \
              -DBUILD_opencv_core:BOOL=ON                                                          \
              -DBUILD_opencv_highgui:BOOL=ON                                                       \
              -DBUILD_opencv_imgcodecs:BOOL=ON                                                     \
              -DBUILD_opencv_imgproc:BOOL=ON                                                       \
                                                                                                   \
              -DBUILD_opencv_apps:BOOL=OFF                                                         \
              -DBUILD_opencv_calib3d:BOOL=OFF                                                      \
              -DBUILD_opencv_cudaarithm:BOOL=OFF                                                   \
              -DBUILD_opencv_cudabgsegm:BOOL=OFF                                                   \
              -DBUILD_opencv_cudacodec:BOOL=OFF                                                    \
              -DBUILD_opencv_cudafeatures2d:BOOL=OFF                                               \
              -DBUILD_opencv_cudafilters:BOOL=OFF                                                  \
              -DBUILD_opencv_cudaimgproc:BOOL=OFF                                                  \
              -DBUILD_opencv_cudalegacy:BOOL=OFF                                                   \
              -DBUILD_opencv_cudaobjdetect:BOOL=OFF                                                \
              -DBUILD_opencv_cudaoptflow:BOOL=OFF                                                  \
              -DBUILD_opencv_cudastereo:BOOL=OFF                                                   \
              -DBUILD_opencv_cudawarping:BOOL=OFF                                                  \
              -DBUILD_opencv_cudev:BOOL=OFF                                                        \
              -DBUILD_opencv_features2d:BOOL=OFF                                                   \
              -DBUILD_opencv_flann:BOOL=OFF                                                        \
              -DBUILD_opencv_ml:BOOL=OFF                                                           \
              -DBUILD_opencv_objdetect:BOOL=OFF                                                    \
              -DBUILD_opencv_photo:BOOL=OFF                                                        \
              -DBUILD_opencv_python2:BOOL=OFF                                                      \
              -DBUILD_opencv_shape:BOOL=OFF                                                        \
              -DBUILD_opencv_stitching:BOOL=OFF                                                    \
              -DBUILD_opencv_superres:BOOL=OFF                                                     \
              -DBUILD_opencv_ts:BOOL=OFF                                                           \
              -DBUILD_opencv_video:BOOL=OFF                                                        \
              -DBUILD_opencv_videoio:BOOL=OFF                                                      \
              -DBUILD_opencv_videostab:BOOL=OFF                                                    \
              -DBUILD_opencv_world:BOOL=OFF                                                        \
              -DBUILD_opencv_python3:BOOL=OFF                                                      \
              -DBUILD_opencv_java:BOOL=OFF                                                         \
                                                                                                   \
              -DWITH_PNG:BOOL=ON                                                                   \
              -DWITH_TIFF:BOOL=ON                                                                  \
              -DWITH_JPEG:BOOL=ON                                                                  \
                                                                                                   \
              -DWITH_1394:BOOL=OFF                                                                 \
              -DWITH_ARAVIS:BOOL=OFF                                                               \
              -DWITH_CLP:BOOL=OFF                                                                  \
              -DWITH_CUBLAS:BOOL=OFF                                                               \
              -DWITH_CUDA:BOOL=OFF                                                                 \
              -DWITH_CUFFT:BOOL=OFF                                                                \
              -DWITH_EIGEN:BOOL=OFF                                                                \
              -DWITH_FFMPEG:BOOL=OFF                                                               \
              -DWITH_GDAL:BOOL=OFF                                                                 \
              -DWITH_GDCM:BOOL=OFF                                                                 \
              -DWITH_GIGEAPI:BOOL=OFF                                                              \
              -DWITH_GPHOTO2:BOOL=OFF                                                              \
              -DWITH_GSTREAMER:BOOL=OFF                                                            \
              -DWITH_GSTREAMER_0_10:BOOL=OFF                                                       \
              -DWITH_IPP:BOOL=OFF                                                                  \
              -DWITH_IPP_A:BOOL=OFF                                                                \
              -DWITH_LAPACK:BOOL=OFF                                                               \
              -DWITH_LIBV4L:BOOL=OFF                                                               \
              -DWITH_MATLAB:BOOL=OFF                                                               \
              -DWITH_NVCUVID:BOOL=OFF                                                              \
              -DWITH_OPENCL:BOOL=OFF                                                               \
              -DWITH_OPENCLAMDBLAS:BOOL=OFF                                                        \
              -DWITH_OPENCLAMDFFT:BOOL=OFF                                                         \
              -DWITH_OPENCL_SVM:BOOL=OFF                                                           \
              -DWITH_OPENEXR:BOOL=OFF                                                              \
              -DWITH_OPENGL:BOOL=OFF                                                               \
              -DWITH_OPENMP:BOOL=OFF                                                               \
              -DWITH_OPENNI:BOOL=OFF                                                               \
              -DWITH_OPENNI2:BOOL=OFF                                                              \
              -DWITH_OPENVX:BOOL=OFF                                                               \
              -DWITH_PTHREADS_PF:BOOL=OFF                                                          \
              -DWITH_PVAPI:BOOL=OFF                                                                \
              -DWITH_QT:BOOL=OFF                                                                   \
              -DWITH_TBB:BOOL=OFF                                                                  \
              -DWITH_UNICAP:BOOL=OFF                                                               \
              -DWITH_V4L:BOOL=OFF                                                                  \
              -DWITH_VA:BOOL=OFF                                                                   \
              -DWITH_VA_INTEL:BOOL=OFF                                                             \
              -DWITH_VTK:BOOL=OFF                                                                  \
              -DWITH_WEBP:BOOL=OFF                                                                 \
              -DWITH_XIMEA:BOOL=OFF                                                                \
              -DWITH_XINE:BOOL=OFF                                                                 \
          || abort_and_cleanup "Failed to configure opencv"
    make -j2 install || abort_and_cleanup "Failed to build opencv"
  fi
  
  local elapsed_time=$(expr $(date +%s) - $start_time)
  NOTICE $(printf "${FUNCNAME[0]}: Successfully installed opencv %s (took %dm %ds)\n"              \
           $opencv_version $(($elapsed_time%3600/60)) $(($elapsed_time%60)))

  export OpenCV_DIR="${opencv_install_dir}"
  popd
}
