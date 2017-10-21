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

#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Export.h"
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

// Conversion
OptionsData convertTo(OptionsType typeTo, OptionsType typeFrom, const OptionsData& dataFrom);

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

  /// @brief Check the options type
  template <class T>
  bool isOfType() const noexcept {
    return internal::TypeToOptionsType<T>::value == type_;
  }

  /// @brief Get a **copy** of the option as type `T` (performs conversion if necessary)
  /// @throws Exception   Conversion failed
  template <class T>
  inline T get() const {
    return boost::get<T>(internal::convertTo(internal::TypeToOptionsType<T>::value, type_, data_));
  }

  /// @brief Set data of the variable (this copies the `data`)
  template <class T>
  inline void set(const T& data) noexcept {
    type_ = internal::TypeToOptionsType<T>::value;
    data_ = data;
  }

  /// @brief Convert to string
  std::string toString() const;

private:
  internal::OptionsType type_; ///< Type of the option
  internal::OptionsData data_; ///< Data of the option
};

/// @brief Options of the Sequoia Engine
/// @ingroup core
class SEQUOIA_API Options2 {
public:
  /// @brief Set option `name` to `value`
  template <class T>
  void set(const std::string& name, T&& value) noexcept {
    set(name, Option{value});
  }

  /// @brief Set option `name` to `value`
  void set(const std::string& name, const Option& value) noexcept { options_[name] = value; }

  /// @brief Get a **copy** of the option `name` as type `T` (performs conversion if necessary)
  /// @throws Exception   Option `name` does not exist or conversion failed
  template <class T>
  T get(const std::string& name) {
    auto it = options_.find(name);
    if(it == options_.end())
      SEQUOIA_THROW(core::Exception, "option '{}' does not exists", name);
    return it->second.get<T>();
  }

  /// @brief Convert to string
  std::string toString() const;

private:
  std::unordered_map<std::string, Option> options_;
};

} // namespace core

using Options2 = core::Options2;

} // namespace sequoia

#endif
