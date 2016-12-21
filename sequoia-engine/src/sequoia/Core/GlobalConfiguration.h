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

#include "sequoia/Core/String.h"
#include <OGRE/OgreSingleton.h>
#include <boost/property_tree/ptree_fwd.hpp>
#include <iosfwd>
#include <memory>
#include <vector>

namespace sequoia {

namespace core {

/// @brief Expose configuration options
///
/// @ingroup core
class SEQUOIA_EXPORT GlobalConfiguration : public Ogre::Singleton<GlobalConfiguration> {
public:
#ifdef SEQUOIA_ON_WIN32
  using StringType = std::wstring;
#else
  using StringType = std::string;
#endif
  using TreeType = boost::property_tree::basic_ptree<std::string, StringType>;

  /// @brief Available parsers
  enum ParserKind { XML, JSON, INI, INFO };

  /// @brief Initialize an empty configuration
  GlobalConfiguration();

  /// @brief Get the underlying property tree
  std::shared_ptr<TreeType>& ptree() noexcept { return ptree_; }
  const std::shared_ptr<TreeType>& ptree() const noexcept { return ptree_; }

  /// @brief Set the value of the node at the given path to the supplied value, translated to the
  /// tree's data type (`std::string` (Unix) or `std::wstring` (Win32)).
  ///
  /// If the node doesn't exist, it is created, including all its missing parents.
  /// @{
  void put(const std::string& path, const bool& value);
  void put(const std::string& path, const int& value);
  void put(const std::string& path, const float& value);
  void put(const std::string& path, const StringType& value);
  /// @}

  /// @brief Return the translated value (from `StringType` to the requested type) if possible, and
  /// the default value if the node doesn't exist or conversion fails.
  /// @{
  bool getBoolean(const std::string& path, bool defaultValue) const;
  int getInteger(const std::string& path, int defaultValue) const;
  float getFloat(const std::string& path, float defaultValue) const;
  StringType getString(const std::string& path, StringType defaultValue) const;
  /// @}

  /// @brief Save the configration to disk
  ///
  /// Nodes added via `addSkipNode` will be skipped when saving the configuration to disk.
  ///
  /// @param filename   Filename to use
  /// @param parser     Parser to use
  ///
  /// @throw IOException  Failed to write to disk
  void save(String filename, ParserKind parser = ParserKind::XML) const;

  /// @brief Load configration from disk (the `ParserKind` will be deduced from the file extension)
  ///
  /// @param filename   Filename to use
  /// @param parser     Parser to use
  ///
  /// @throw IOException      Failed to open file
  /// @throw ParserException  Failed to load configuration
  void load(String filename);

  /// @brief Convert to string
  std::string toString(ParserKind parser = ParserKind::XML) const;

  /// @brief Convert to stream
  SEQUOIA_EXPORT friend std::ostream& operator<<(std::ostream& stream,
                                                 const GlobalConfiguration& config);

  /// @brief Before saving the configuration to disk, all direct children of the root with name
  /// `node` will be removed.
  void addSkipNode(const std::string& node);

private:
  std::shared_ptr<TreeType> ptree_;
  std::vector<std::string> skipNodes_;
};

} // namespace core

using GlobalConfiguration = core::GlobalConfiguration;

} // namespace sequoia

#endif
