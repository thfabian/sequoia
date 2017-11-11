//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_ENGINE_CORE_OPTIONS_H
#define SEQUOIA_ENGINE_CORE_OPTIONS_H

#include "sequoia-engine/Core/Export.h"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

namespace sequoia {

namespace core {

/// @brief Meta data of an option (e.g corresponding command line option)
struct OptionMetaData {
  std::string CommandLine;        ///< Command line option (e.g "foo" will create --foo)
  std::string CommandLineShort;   ///< Short command option (e.g "f" will create "-f")
  bool CommandLineHasValue;       ///< Command line options takes a value (e.g --foo=VALUE)
  std::string CommandLineMetaVar; ///< Name of the meta variable in the help string if the option
                                  ///  takes a value (e.g <VALUE> in --foo=VALUE)
  std::string DocString;          ///< Documentation string of the option

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Generic options class which stores a `key` : `value` pair
///
/// The Options class stores everything as a string and provides implicit conversions between
/// strings and numerical types. Options can be parsed from command-line using `CommandLine` and
/// setting the appropriate `OptionMetaData` or read from an XML file.
///
/// @code{.cpp}
///   Options opt;
///
///   opt.setInt("foo", 2);
///   assert(opt.getInt("foo"), 2);
///   assert(opt.getString("foo"), "2");
///
///   opt.setString("bar", "2");
///   assert(opt.getInt("foo") == opt.getInt("bar"));
/// @endcode
///
/// @ingroup core
class SEQUOIA_API Options {
public:
  /// @brief Initialize empty options
  ///
  /// Various classes provide default options. See for example: `GameOptions`.
  Options();

  /// @brief Set option `name` to `value`
  ///
  /// @param name   Name of the option
  /// @param value  Value of the option
  /// @{
  void setBool(const std::string& name, bool value) noexcept { setImpl(name, value, false); }
  void setInt(const std::string& name, int value) noexcept { setImpl(name, value, false); }
  void setFloat(const std::string& name, float value) noexcept { setImpl(name, value, false); }
  void setString(const std::string& name, std::string value) noexcept {
    setImpl(name, std::move(value), false);
  }

  void setBool(const std::string& name, bool value, const OptionMetaData& metaData) noexcept {
    setBool(name, value);
    setMetaData(name, metaData);
  }

  void setInt(const std::string& name, int value, const OptionMetaData& metaData) noexcept {
    setInt(name, value);
    setMetaData(name, metaData);
  }

  void setFloat(const std::string& name, float value, const OptionMetaData& metaData) noexcept {
    setFloat(name, value);
    setMetaData(name, metaData);
  }

  void setString(const std::string& name, std::string value,
                 const OptionMetaData& metaData) noexcept {
    setString(name, std::move(value));
    setMetaData(name, metaData);
  }
  /// @}

  /// @brief Set option `name` to `value` if the option does **not** yet exists, otherwise nothing
  /// will be done
  ///
  /// @param name   Name of the option
  /// @param value  Value of the option
  /// @{
  void setDefaultBool(const std::string& name, bool value) noexcept { setImpl(name, value, true); }
  void setDefaultInt(const std::string& name, int value) noexcept { setImpl(name, value, true); }
  void setDefaultFloat(const std::string& name, float value) noexcept {
    setImpl(name, value, true);
  }
  void setDefaultString(const std::string& name, std::string value) noexcept {
    setImpl(name, std::move(value), true);
  }

  void setDefaultBool(const std::string& name, bool value,
                      const OptionMetaData& metaData) noexcept {
    setDefaultBool(name, value);
    setMetaData(name, metaData);
  }

  void setDefaultInt(const std::string& name, int value, const OptionMetaData& metaData) noexcept {
    setDefaultInt(name, value);
    setMetaData(name, metaData);
  }

  void setDefaultFloat(const std::string& name, float value,
                       const OptionMetaData& metaData) noexcept {
    setDefaultFloat(name, value);
    setMetaData(name, metaData);
  }

  void setDefaultString(const std::string& name, std::string value,
                        const OptionMetaData& metaData) noexcept {
    setDefaultString(name, std::move(value));
    setMetaData(name, metaData);
  }
  /// @}

  /// @brief Get a **copy** of the option `name` as type `T` (performs conversion if necessary)
  /// @param name   Name of the option
  /// @throws Exception   Option `name` does not exist or conversion failed
  /// @{
  template <class T>
  T get(const std::string& name) const {
    return getImpl(name, static_cast<T*>(nullptr));
  }
  bool getBool(const std::string& name) const { return get<bool>(name); }
  int getInt(const std::string& name) const { return get<int>(name); }
  float getFloat(const std::string& name) const { return get<float>(name); }
  std::string getString(const std::string& name) const { return get<std::string>(name); }
  /// @}

  /// @brief Get a **copy** of the option `name` as an integer statically casted to `Enum`
  /// @param name   Name of the option
  /// @throws Exception   Option `name` does not exist or conversion failed
  template <class Enum>
  Enum getEnum(const std::string& name) const {
    return static_cast<Enum>(get<int>(name));
  }

  /// @brief Get the meta data map of the options
  const std::unordered_map<std::string, OptionMetaData>& getOptionsMetaData() const {
    return optionsMetaData_;
  }

  /// @brief Add a meta data to the option `name`
  void setMetaData(const std::string& name, const OptionMetaData& metaData) {
    optionsMetaData_[name] = metaData;
  }

  /// @brief Add a meta data to the option `name`
  /// @throws Exception   meta data of option `name` does not exist
  OptionMetaData& getMetaData(const std::string& name);

  /// @brief Write options the a XML config `file`
  /// Note that the meta-data is *not* serialized
  void write(const std::string& file) const;

  /// @brief Read options from a XML config `file`
  /// @returns `true` on success, `false` on error
  bool read(const std::string& file);

  /// @brief Convert to string
  std::string toString() const;

private:
  void setImpl(const std::string& name, bool value, bool isDefault) noexcept;
  void setImpl(const std::string& name, int value, bool isDefault) noexcept;
  void setImpl(const std::string& name, float value, bool isDefault) noexcept;
  void setImpl(const std::string& name, std::string value, bool isDefault) noexcept;

  bool getImpl(const std::string& name, const bool*) const;
  int getImpl(const std::string& name, const int*) const;
  float getImpl(const std::string& name, const float*) const;
  std::string getImpl(const std::string& name, const std::string*) const;

  const std::string& getValueImpl(const std::string& name) const;

private:
  std::unordered_map<std::string, std::string> options_;
  std::unordered_map<std::string, OptionMetaData> optionsMetaData_;
};

/// @brief Set the options for the core library
/// @ingroup core
extern SEQUOIA_API void setDefaultOptions(const std::shared_ptr<Options>& options);

} // namespace core

using Options = core::Options;
using OptionMetaData = core::OptionMetaData;

} // namespace sequoia

#endif
