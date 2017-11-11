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

include(ExternalProject)
include(SequoiaBuildPackage)
include(SequoiaExportPackage)
include(SequoiaFindPackage)
include(SequoiaExtractCMakePackageArgs)

#
# zlib
#
set(zlib_version "1.2.11")
set(zlib_url "https://github.com/thfabian/zlib/archive/v${zlib_version}.tar.gz")
set(zlib_md5 "c380c38d7a493b3b8823408574989f91")

sequoia_find_package(
  PACKAGE ZLIB 
  FORWARD_VARS ZLIB_ROOT
  BUILD_VERSION ${zlib_version}
)

#
# assimp
#
set(assimp_version "4.0.1")
set(assimp_url "https://github.com/assimp/assimp/archive/v${assimp_version}.tar.gz")
set(assimp_md5 "23a6301c728a413aafbfa1cca19ba91f")

sequoia_find_package(
  PACKAGE assimp
  PACKAGE_ARGS ${assimp_version} NO_MODULE QUIET
  FORWARD_VARS assimp_DIR
  BUILD_VERSION ${assimp_version}
  DEPENDS zlib
)

#
# backward
#
set(backward_version "1.4.2")
set(backward_url "https://github.com/thfabian/backward-cpp/archive/v${backward_version}.tar.gz")
set(backward_md5 "5fabf457255d57f1c01da96ce583b657")

sequoia_build_package(
  PACKAGE backward
  BUILD_VERSION ${backward_version}
)

#
# Google Benchmark
#
set(benchmark_version "1.2.0")
set(benchmark_url "https://github.com/google/benchmark/archive/v${benchmark_version}.tar.gz")
set(benchmark_md5 "48d0b090cd7a84af2c4a28c8dc963c74")

sequoia_build_package(
  PACKAGE benchmark
  BUILD_VERSION ${benchmark_version}
)

#
# Boost
#
set(boost_min_version 1.58.0)
set(_v 63)
set(boost_download_version 1.${_v}.0)
set(boost_url "http://sourceforge.net/projects/boost/files/boost/1.${_v}.0/boost_1_${_v}_0.tar.gz/download")
set(boost_md5 "7b493c08bc9557bbde7e29091f28b605")
set(boost_components thread date_time system atomic chrono filesystem program_options)

sequoia_find_package(
  PACKAGE Boost
  PACKAGE_ARGS ${boost_min_version} COMPONENTS ${boost_components}
  FORWARD_VARS BOOST_ROOT Boost_LIBRARY_DIRS Boost_INCLUDE_DIRS
  BUILD_VERSION ${boost_download_version}
)

#
# cereal
#
set(cereal_version "1.2.2")
set(cereal_url "https://github.com/USCiLab/cereal/archive/v${cereal_version}.tar.gz")
set(cereal_md5 "4c56c7b9499dba79404250ef9a040481")

sequoia_build_package(
  PACKAGE cereal
  BUILD_VERSION ${cereal_version}
)

#
# fmt
#
set(fmt_version "4.0.0")
set(fmt_url "https://github.com/fmtlib/fmt/archive/${fmt_version}.tar.gz")
set(fmt_md5 "c9be9a37bc85493d1116b0af59a25eba")

sequoia_build_package(
  PACKAGE fmt
  BUILD_VERSION ${fmt_version}
)

#
# glbinding
#
set(glbinding_version "2.1.1")
set(glbinding_url "https://github.com/cginternals/glbinding/archive/v${glbinding_version}.tar.gz")
set(glbinding_md5 "a7d03b323c269defda9f22c47c731585")

sequoia_find_package(
 PACKAGE glbinding 
 PACKAGE_ARGS ${glbinding_version}
 FORWARD_VARS GLBINDING_ROOT
 BUILD_VERSION ${glbinding_version}
)

#
# glfw3
#
set(glfw3_version "3.2.1")
set(glfw3_url "https://github.com/glfw/glfw/archive/${glfw3_version}.tar.gz")
set(glfw3_md5 "91b8250b6edcc26c9f5205555070a504")

sequoia_build_package(
  PACKAGE glfw3
  BUILD_VERSION ${glfw3_version}
)

#
# gli
#
set(gli_version "0.8")
set(gli_download_version "0.8.2.0")
set(gli_url "https://github.com/g-truc/gli/archive/${gli_download_version}.tar.gz")
set(gli_md5 "163bf765271cb036b2408ce2171b9e37")

sequoia_build_package(
  PACKAGE gli
  BUILD_VERSION ${gli_version}
)

#
# glm
#
set(glm_version "0.9")
set(glm_download_version "0.9.8.4")
set(glm_url "https://github.com/g-truc/glm/archive/${glm_download_version}.tar.gz")
set(glm_md5 "26ca8792038da90f67301fa0a3d5b693")

sequoia_build_package(
  PACKAGE glm
  BUILD_VERSION ${glm_version}
)

#
# OpenCV
#
set(opencv_version "3.3.1")
set(opencv_url "https://github.com/opencv/opencv/archive/${opencv_version}.tar.gz")
set(opencv_md5 "b1ed9aea030bb5bd9df28524d97de84c")

sequoia_find_package(
  PACKAGE OpenCV
  PACKAGE_ARGS ${opencv_version} NO_MODULE QUIET
  REQUIRED_VARS OPENCV_CORE_FOUND OPENCV_HIGHGUI_FOUND OPENCV_IMGCODECS_FOUND OPENCV_IMGPROC_FOUND
  FORWARD_VARS OpenCV_DIR
  BUILD_VERSION ${opencv_version}
  DEPENDS zlib
)

#
# spdlog
#
set(spdlog_version "0.14.0")
set(spdlog_url "https://github.com/gabime/spdlog/archive/v0.14.0.tar.gz")
set(spdlog_md5 "e8c83f739e0172b4687d625bb1da1aac")

sequoia_build_package(
  PACKAGE spdlog
  BUILD_VERSION ${spdlog_version}
)

#
# TBB
#
set(tbb_url "https://github.com/thfabian/tbb/archive/tbb44u4.tar.gz")
set(tbb_md5 "df4c96df8352c13bcbc8b642fff257b3")

sequoia_find_package(
  PACKAGE TBB 
  FORWARD_VARS TBB_ROOT_DIR
)

#
# GTest
#
set(gtest_version "1.7.0")
set(gtest_url "https://github.com/google/googletest/archive/release-${gtest_version}.tar.gz")
set(gtest_md5 "4ff6353b2560df0afecfbda3b2763847")

sequoia_build_package(
  PACKAGE gtest
  BUILD_VERSION ${gtest_version}
)

sequoia_report_result("Package summary" ${SEQUOIA_PACKAGE_INFO})
