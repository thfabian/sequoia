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

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Export.h"
#include "sequoia/Core/Platform.h"
#include <boost/variant.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace sequoia {

namespace core {

namespace internal {

struct InvalidData {
  inline bool operator==(const InvalidData& other) const noexcept { return true; }
  inline bool operator!=(const InvalidData& other) const noexcept { return false; }
};

enum class OptionsType : std::uint32_t {
  Invalid = 0,
#define OPTIONS_TYPE(Type, Name) Name,
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE
};

using OptionsData = boost::variant<
#define OPTIONS_TYPE(Type, Name) Type,
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE
    InvalidData>;

// OptionsType -> Type
template <OptionsType Enum>
struct InvalidOptionsTypeToType {
  using type = void;
};

template <OptionsType Enum>
struct OptionsTypeToType {
  using type = typename InvalidOptionsTypeToType<Enum>::type;
  static_assert(!std::is_same<type, void>::value, "invalid OptionsType");
};

#define OPTIONS_TYPE(Type, Name)                                                                   \
  template <>                                                                                      \
  struct OptionsTypeToType<OptionsType::Name> {                                                    \
    using type = Type;                                                                             \
  };
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE

// Type -> OptionsType
template <class T>
struct InvalidTypeToOptionsType {
  static constexpr OptionsType value = OptionsType::Invalid;
};

template <class T>
struct TypeToOptionsType {
  static constexpr OptionsType value = InvalidTypeToOptionsType<T>::value;
  static_assert(value != OptionsType::Invalid, "invalid type");
};

#define OPTIONS_TYPE(Type, Name)                                                                   \
  template <>                                                                                      \
  struct TypeToOptionsType<Type> {                                                                 \
    static constexpr OptionsType value = OptionsType::Name;                                        \
  };
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE

/// @brief Performs conversion of `typeTo` to `typeFrom`
OptionsData convert(OptionsType typeTo, OptionsType typeFrom, const OptionsData& dataFrom);

} // namespace internal

/// @brief Option representation
/// @ingroup core
class SEQUOIA_API Option {
public:
  /// @brief Default constructor
  Option() : type_(internal::OptionsType::Invalid), data_(internal::InvalidData{}) {}
  Option(const Option&) = default;
  Option(Option&&) = default;

  Option& operator=(const Option&) = default;
  Option& operator=(Option&&) = default;

#define OPTIONS_TYPE(Type, Name)                                                                   \
  Option& operator=(const Type& value) {                                                           \
    this->set<Type>(value);                                                                        \
    return *this;                                                                                  \
  }                                                                                                \
                                                                                                   \
  Option(const Type& data) : type_(internal::TypeToOptionsType<Type>::value), data_(data) {}
#include "sequoia/Core/OptionsType.inc"
#undef OPTIONS_TYPE

  Option(const char* data) : type_(internal::OptionsType::String), data_(std::string(data)) {}
  Option& operator=(const char* value) {
    this->set<std::string>(std::string(value));
    return *this;
  }

  /// @brief Check the options type
  template <class T>
  bool isOfType() const noexcept {
    return internal::TypeToOptionsType<T>::value == type_;
  }

  /// @brief Get a **copy** of the option as type `T` (performs conversion if necessary)
  /// @throws Exception   Conversion failed (can only happen for string conversions e.g `"foo"` to
  ///                     `int`)
  template <class T>
  inline T get() const {
    return boost::get<T>(internal::convert(internal::TypeToOptionsType<T>::value, type_, data_));
  }

  /// @brief Set data of the variable (this copies the `data`)
  template <class T>
  inline void set(const T& data) noexcept {
    type_ = internal::TypeToOptionsType<T>::value;
    data_ = data;
  }
  inline void set(const char* data) noexcept { this->set(std::string(data)); }

  /// @brief Check if option is invalid
  bool isInvalid() const { return type_ == internal::OptionsType::Invalid; }

  /// @brief Convert to string
  std::string toString() const;

private:
  internal::OptionsType type_; ///< Type of the option
  internal::OptionsData data_; ///< Data of the option
};

/// @brief Meta data of an option (e.g corresponding command line option)
struct OptionMetaData {
  std::string CommandLine;        ///< Command line option (e.g "foo" will create --foo)
  std::string CommandLineShort;   ///< Short option (e.g "f" will create "-f"
  std::string CommandLineMetaVar; ///< Meta variable used in the doc string of the option
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
  /// or "Window.Width") to allow serializing it to an *.ini file.
  template <class T>
  void set(const std::string& name, T&& value) noexcept {
    SEQUOIA_ASSERT_MSG(
        std::count(name.begin(), name.end(), '.') == 1,
        "Option is not of the form \"X.Y\" i.e contains more (or less) than one dot");
    options_[name] = Option{value};
  }

  /// @brief Get a **copy** of the option `name` as type `T` (performs conversion if necessary)
  /// @throws Exception   Option `name` does not exist or conversion failed
  template <class T>
  T get(const std::string& name) const {
    return this->getAsOption(name).get<T>();
  }

  /// @brief Get a string representation of the option `name`
  /// @throws Exception   Option `name` does not exist
  std::string getAsString(const std::string& name) const { return this->get<std::string>(name); }

  /// @brief Get a **reference** to the option of `name`
  /// @throws Exception   Option `name` does not exist
  const Option& getAsOption(const std::string& name) const;
  Option& getAsOption(const std::string& name);

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
  void write(const platform::Path& file) const;

  /// @brief Read options from `file`
  void read(const platform::Path& file);

  /// @brief Convert to string
  std::string toString() const;

private:
  std::unordered_map<std::string, Option> options_;
  std::unordered_map<std::string, OptionMetaData> optionsMetaData_;
};

} // namespace core

using Options2 = core::Options2;

} // namespace sequoia

#endif
