##===------------------------------------------------------------------------------*- CMake -*-===##
##
##                                      S E Q U O I A
##
## This file is distributed under the MIT License (MIT). 
## See LICENSE.txt for details.
##
##===------------------------------------------------------------------------------------------===##
unset(projects)

#
# Boost
#
list(APPEND projects boost)
set(_v 62)
set(boost_version 1.${_v}.0)
set(boost_url "http://sourceforge.net/projects/boost/files/boost/1.${_v}.0/boost_1_${_v}_0.tar.gz/download")
set(boost_md5 "6f4571e7c5a66ccc3323da6c24be8f05")
set(boost_components filesystem)

#
# GTest
#
list(APPEND projects gtest)
set(gtest_version "1.7.0")
set(gtest_url "https://github.com/google/googletest/archive/release-${gtest_version}.tar.gz")
set(gtest_md5 "4ff6353b2560df0afecfbda3b2763847")

#
# SFML
#
list(APPEND projects sfml)
set(sfml_version "2.4.1")
set(sfml_url "https://github.com/SFML/SFML/archive/${sfml_version}.tar.gz")
set(sfml_md5 "abe4eea023275dc82eb4d47914095425")
set(sfml_components system window graphics main)

#
# TBB
#
list(APPEND projects tbb)
set(tbb_url "https://github.com/thfabian/tbb/archive/tbb44u4.tar.gz")
set(tbb_md5 "bfbeb805c91fcd32f858f8d9c26ad0e8")
