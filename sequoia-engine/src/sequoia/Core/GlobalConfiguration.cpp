//===-- sequoia/Core/GlobalConfiguration.cpp ----------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Global configuration class.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Exception.h"
#include "sequoia/Core/GlobalConfiguration.h"
#include <boost/property_tree/ptree.hpp>

namespace sequoia {

namespace core {

template <>
GlobalConfiguration* Singleton<GlobalConfiguration>::instance_ = nullptr;

GlobalConfiguration::GlobalConfiguration()
    : ptree_(std::make_unique<boost::property_tree::ptree>()) {}

} // namespace core

} // namespace sequoia
