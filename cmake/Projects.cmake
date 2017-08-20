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

unset(projects)

#
# zlib
#
list(APPEND projects zlib)
set(zlib_url "https://github.com/thfabian/zlib/archive/v1.2.11.tar.gz")
set(zlib_md5 "c380c38d7a493b3b8823408574989f91")

#
# backward
#
list(APPEND projects backward)
set(backward_version "1.4")
set(backward_url "https://github.com/thfabian/backward-cpp/archive/v${backward_version}.tar.gz")
set(backward_md5 "ecb23d60195d81edeedfe3206fa72c76")

#
# benchmark
#
list(APPEND projects benchmark)
set(benchmark_version "1.2.0")
set(benchmark_url "https://github.com/google/benchmark/archive/v${benchmark_version}.tar.gz")
set(benchmark_md5 "48d0b090cd7a84af2c4a28c8dc963c74")

#
# Boost
#
list(APPEND projects boost)
set(boost_min_version 1.58.0)
set(_v 63)
set(boost_download_version 1.${_v}.0)
set(boost_url "http://sourceforge.net/projects/boost/files/boost/1.${_v}.0/boost_1_${_v}_0.tar.gz/download")
set(boost_md5 "7b493c08bc9557bbde7e29091f28b605")
set(boost_components thread date_time system atomic chrono filesystem program_options)

#
# protobuf
#
# list(APPEND projects protobuf)
# set(protobuf_version "3.3.0")
# set(protobuf_url "https://github.com/google/protobuf/archive/v${protobuf_version}.tar.gz")
# set(protobuf_md5 "10644296984c7cbdd1e8436e88b069d2")

#
# glbinding
#
list(APPEND projects glbinding)
set(glbinding_version "2.1.1")
set(glbinding_url "https://github.com/cginternals/glbinding/archive/v${glbinding_version}.tar.gz")
set(glbinding_md5 "93dab7f4d64eff5385e3d1915e765695")

#
# GLFW
#
list(APPEND projects glfw3)
set(glfw3_version "3.2.1")
set(glfw3_url "https://github.com/glfw/glfw/archive/${glfw3_version}.tar.gz")
set(glfw3_md5 "91b8250b6edcc26c9f5205555070a504")

#
# gli
#
list(APPEND projects gli)
set(gli_version "0.8")
set(gli_download_version "0.8.2.0")
set(gli_url "https://github.com/g-truc/gli/archive/${gli_download_version}.tar.gz")
set(gli_md5 "163bf765271cb036b2408ce2171b9e37")

#
# glm
#
list(APPEND projects glm)
set(glm_version "0.9")
set(glm_download_version "0.9.8.4")
set(glm_url "https://github.com/g-truc/glm/archive/${glm_download_version}.tar.gz")
set(glm_md5 "26ca8792038da90f67301fa0a3d5b693")

#
# GTest
#
list(APPEND projects gtest)
set(gtest_version "1.7.0")
set(gtest_url "https://github.com/google/googletest/archive/release-${gtest_version}.tar.gz")
set(gtest_md5 "4ff6353b2560df0afecfbda3b2763847")

#
# OpenCV
#
list(APPEND projects opencv)
set(opencv_version "3.2.0")
set(opencv_url "https://github.com/opencv/opencv/archive/${opencv_version}.tar.gz")
set(opencv_md5 "a43b65488124ba33dde195fea9041b70")

#
# TBB
#
list(APPEND projects tbb)
set(tbb_url "https://github.com/thfabian/tbb/archive/tbb44u4.tar.gz")
set(tbb_md5 "bfbeb805c91fcd32f858f8d9c26ad0e8")

#
# tinyobjloader
#
list(APPEND projects tinyobjloader)
set(tinyobjloader_version "1.0.6")
set(tinyobjloader_url "https://github.com/syoyo/tinyobjloader/archive/v${tinyobjloader_version}.tar.gz")
set(tinyobjloader_md5 "ef3198fdbd98171d43d3cc703015be33")