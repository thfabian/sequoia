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

#ifndef SEQUOIA_CORE_OPTIONS2_H
#define SEQUOIA_CORE_OPTIONS2_H

#include "sequoia-engine/Core/Export.h"
#include <cstdint>
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

  std::string toString() const;
};

/// @brief Options of the Sequoia Engine
/// @ingroup core
class SEQUOIA_API Options2 {
public:
  /// @brief Set option `name` to `value`
  ///
  /// Note that the name of the option should always be of the form "X.Y" (e.g "Window.Fullscreen"
  /// or "Window.Width") to allow serializing it to an `*.ini` file.
  ///
  /// @param name   Name of the option
  /// @param value  Value of the option
  /// @{
  void set(const std::string& name, bool value) noexcept { setImpl(name, value); }
  void set(const std::string& name, int value) noexcept { setImpl(name, value); }
  void set(const std::string& name, float value) noexcept { setImpl(name, value); }
  void set(const std::string& name, double value) noexcept {
    setImpl(name, static_cast<float>(value));
  }
  void set(const std::string& name, const char* value) noexcept {
    setImpl(name, std::string(value));
  }
  void set(const std::string& name, std::string value) noexcept { setImpl(name, std::move(value)); }
  /// @}

  /// @brief Get a **copy** of the option `name` as type `T` (performs conversion if necessary)
  /// @param name   Name of the option
  /// @throws Exception   Option `name` does not exist or conversion failed
  template <class T>
  T get(const std::string& name) const {
    return getImpl(name, static_cast<T*>(nullptr));
  }

  /// @brief Get a **copy** of the option `name` as a string
  /// @param name   Name of the option
  /// @throws Exception   Option `name` does not exist
  std::string getAsString(const std::string& name) const {
    return getImpl(name, static_cast<std::string*>(nullptr));
  }

  /// @brief Get the meta data map of the options
  const std::unordered_map<std::string, OptionMetaData>& getOptionsMetaData() const {
    return optionsMetaData_;
  }

  /// @brief Add a meta data to the option `name`
  void setMetaData(const std::string& name, const OptionMetaData& metaData) {
    optionsMetaData_[name] = metaData;
  }

  /// @brief Get a copy of the options
  Options2 getCopy() const { return Options2{*this}; }

  /// @brief Write options the a config `file`
  /// Note that the meta-data is *not* serialized
  void write(const std::string& file) const;

  /// @brief Read options from `file`
  void read(const std::string& file);

  /// @brief Convert to string
  std::string toString() const;

private:
  void setImpl(const std::string& name, bool value) noexcept;
  void setImpl(const std::string& name, int value) noexcept;
  void setImpl(const std::string& name, float value) noexcept;
  void setImpl(const std::string& name, std::string value) noexcept;

  bool getImpl(const std::string& name, const bool*) const;
  int getImpl(const std::string& name, const int*) const;
  float getImpl(const std::string& name, const float*) const;
  std::string getImpl(const std::string& name, const std::string*) const;

  const std::string& getValueImpl(const std::string& name) const;

private:
  std::unordered_map<std::string, std::string> options_;
  std::unordered_map<std::string, OptionMetaData> optionsMetaData_;
};

} // namespace core

using Options2 = core::Options2;

} // namespace sequoia

#endif
