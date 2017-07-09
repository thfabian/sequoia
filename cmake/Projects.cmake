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
set(zlib_md5 "0095d2d2d1f3442ce1318336637b695f")

#
# backward
#
list(APPEND projects backward)
set(backward_version "1.3")
set(backward_url "https://github.com/bombela/backward-cpp/archive/v${backward_version}.tar.gz")
set(backward_md5 "b89051fb35fd2eacf5ab9c7bced5faf6")

#
# Boost
#
list(APPEND projects boost)
set(boost_min_version 1.58.0)
set(_v 60)
set(boost_download_version 1.${_v}.0)
set(boost_url "http://sourceforge.net/projects/boost/files/boost/1.${_v}.0/boost_1_${_v}_0.tar.gz/download")
set(boost_md5 "28f58b9a33469388302110562bdf6188")
set(boost_components thread date_time system atomic chrono filesystem program_options)

#
# FreeImage
#
list(APPEND projects freeimage)
set(freeimage_url "https://github.com/thfabian/FreeImage/archive/v3.15.3-2.tar.gz")
set(freeimage_md5 "7330ba9c9f1722adf1d3838f1749c879")

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
list(APPEND projects glfw)
set(glfw_version "3.2.1")
set(glfw_url "https://github.com/glfw/glfw/archive/${glfw_version}.tar.gz")
set(glfw_md5 "91b8250b6edcc26c9f5205555070a504")

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
# TBB
#
list(APPEND projects tbb)
set(tbb_url "https://github.com/thfabian/tbb/archive/tbb44u4.tar.gz")
set(tbb_md5 "bfbeb805c91fcd32f858f8d9c26ad0e8")

