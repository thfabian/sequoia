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

unset(projects)

#
# Blaze
#
list(APPEND projects blaze)
set(blaze_version "3.1")
set(blaze_url "https://bitbucket.org/blaze-lib/blaze/downloads/blaze-${blaze_version}.tar.gz")
set(blaze_md5 "2938e015f0d274e8d62ee5c4c0c1e9f3")

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
# GLFW
#
list(APPEND projects glfw)
set(glfw_version "3.2.1")
set(glfw_url "https://github.com/glfw/glfw/archive/${glfw_version}.tar.gz")
set(glfw_md5 "91b8250b6edcc26c9f5205555070a504")

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

