//===-- sequoia/Core/GlobalConfiguration.h ------------------------------------------*- C++ -*-===//
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

#ifndef SEQUOIA_CORE_GLOBALCONFIGURATION_H
#define SEQUOIA_CORE_GLOBALCONFIGURATION_H

#include "sequoia/Core/Singleton.h"
#include "sequoia/Core/String.h"
#include <boost/property_tree/ptree_fwd.hpp>
#include <memory>

namespace sequoia {

namespace core {

/// @brief Expose configuration options
/// 
/// @ingroup core
class SEQUOIA_EXPORT GlobalConfiguration : public Singleton<GlobalConfiguration> {
public:
  /// @brief Available parsers
  enum ParserKind { XML, JSON, INI, INFO };

  /// @brief Initialize an empty configuration
  GlobalConfiguration();

  /// @brief Get the underlying property tree
  std::unique_ptr<boost::property_tree::ptree>& ptree() noexcept { return ptree_; }
  const std::unique_ptr<boost::property_tree::ptree>& ptree() const noexcept { return ptree_; }

  /// @brief Save the configration to disk
  ///
  /// @param filename   Filename to use
  /// @param parser     Parser to use
  ///
  /// @throw IOException  Failed to write to disk
  void save(std::string filename, ParserKind parser = ParserKind::XML) const;

  /// @brief Load configration from disk (the `ParserKind` will be deduced from the file extension)
  ///
  /// @param filename   Filename to use
  /// @param parser     Parser to use
  ///
  /// @throw IOException      Failed to open file
  /// @throw ParserException  Failed to load configuration
  void load(std::string filename);
  
private:
  std::unique_ptr<boost::property_tree::ptree> ptree_;
};

} // namespace core

} // namespace sequoia

#endif
