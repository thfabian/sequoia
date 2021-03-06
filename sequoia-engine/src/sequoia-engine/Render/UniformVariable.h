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

#ifndef SEQUOIA_ENGINE_RENDER_UNIFORMVARIABLE_H
#define SEQUOIA_ENGINE_RENDER_UNIFORMVARIABLE_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Math/Math.h"
#include "sequoia-engine/Render/Exception.h"
#include <array>
#include <boost/variant.hpp>
#include <iosfwd>

namespace sequoia {

namespace render {

/// @brief Type of uniform variables
/// @ingroup render
enum class UniformType : std::uint32_t {
  Invalid = 0,
#define UNIFORM_VARIABLE_TYPE(Type, Name) Name, VectorOf##Name,
#include "sequoia-engine/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
  Struct
};

/// @brief Stream UniformType
/// @ingroup render
SEQUOIA_API extern std::ostream& operator<<(std::ostream& os, UniformType type);

namespace internal {

template <class T>
struct IsUniformType {
  static constexpr bool value = false;
};

#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  template <>                                                                                      \
  struct IsUniformType<Type> {                                                                     \
    static constexpr bool value = true;                                                            \
  };
#include "sequoia-engine/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE

struct InvalidData {
  inline bool operator==(const InvalidData& other) const noexcept { return true; }
  inline bool operator!=(const InvalidData& other) const noexcept { return false; }
};

template <UniformType Enum>
struct InvalidUniformTypeToType {
  using type = void;
};

template <UniformType Enum>
struct UniformTypeToType {
  using type = typename InvalidUniformTypeToType<Enum>::type;
  static_assert(!std::is_same<type, void>::value, "invalid UniformType");
};

#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  template <>                                                                                      \
  struct UniformTypeToType<UniformType::Name> {                                                    \
    using type = Type;                                                                             \
  };                                                                                               \
  template <>                                                                                      \
  struct UniformTypeToType<UniformType::VectorOf##Name> {                                          \
    using type = std::vector<Type>;                                                                \
  };
#include "sequoia-engine/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE

template <class T>
struct InvalidTypeToUniformType {
  static constexpr UniformType value = UniformType::Invalid;
};

template <class T>
struct TypeToUniformType {
  static constexpr UniformType value = InvalidTypeToUniformType<T>::value;
  static_assert(value != UniformType::Invalid, "invalid type");
};

#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  template <>                                                                                      \
  struct TypeToUniformType<Type> {                                                                 \
    static constexpr UniformType value = UniformType::Name;                                        \
  };                                                                                               \
  template <>                                                                                      \
  struct TypeToUniformType<std::vector<Type>> {                                                    \
    static constexpr UniformType value = UniformType::VectorOf##Name;                              \
  };
#include "sequoia-engine/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE

} // namespace internal

/// @brief Uniform variable
/// @ingroup render
class SEQUOIA_API UniformVariable {
public:
  using DataType = boost::variant<
#define UNIFORM_VARIABLE_TYPE(Type, Name) Type, std::vector<Type>,
#include "sequoia-engine/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
      internal::InvalidData>;

  /// @brief Construct with `name` and `data`
  /// @param data   Data of the uniform variable
  template <class T>
  UniformVariable(const T& data) {
    this->set(data);
  }

  template <class T, std::size_t N>
  UniformVariable(const T (&data)[N]) {
    this->set(toVec(data));
  }

  template <class T, std::size_t N>
  UniformVariable(const std::array<T, N>& data) {
    this->set(toVec(data));
  }

  UniformVariable() : data_(internal::InvalidData{}), type_(UniformType::Invalid) {}
  UniformVariable(const UniformVariable&) = default;
  UniformVariable(UniformVariable&&) = default;

  UniformVariable& operator=(const UniformVariable&) = default;
  UniformVariable& operator=(UniformVariable&&) = default;

#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  UniformVariable& operator=(const Type& value) {                                                  \
    this->set<Type>(value);                                                                        \
    return *this;                                                                                  \
  }                                                                                                \
  UniformVariable& operator=(const std::vector<Type>& value) {                                     \
    this->set<std::vector<Type>>(value);                                                           \
    return *this;                                                                                  \
  }
#include "sequoia-engine/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE

  template <class T, std::size_t N>
  UniformVariable& operator=(const T (&value)[N]) {
    this->set(toVec(value));
    return *this;
  }

  template <class T, std::size_t N>
  UniformVariable& operator=(const std::array<T, N>& value) {
    this->set(toVec(value));
    return *this;
  }

  /// @brief Get a *reference* to the data of the variable as type `T`
  /// @throws core::RenderSystemException  type of the uniform variable is not of type `T`
  template <class T>
  inline const T& get() const {
    if(!isOfType<T>()) {
      UniformType type = internal::TypeToUniformType<T>::value;
      SEQUOIA_THROW(RenderSystemException, "invalid type '{}' of uniform variable, expected '{}'",
                    type, type_);
    }
    return boost::get<T>(data_);
  }

  /// @brief Set data of the variable (this copies the `data`)
  template <class T>
  inline void set(const T& data) noexcept {
    type_ = internal::TypeToUniformType<T>::value;
    data_ = data;
  }

  /// @brief Check if uniform variable is of type `T`
  template <class T>
  inline bool isOfType() const noexcept {
    return internal::TypeToUniformType<T>::value == type_;
  }

  /// @brief Get the type of the variable
  inline UniformType getType() const noexcept { return type_; }

  /// @brief Is the variable empty
  bool empty() const noexcept { return type_ == UniformType::Invalid; }

  /// @name Comparison
  /// @{
  bool operator==(const UniformVariable& other) const noexcept;
  bool operator!=(const UniformVariable& other) const noexcept { return !(*this == other); }
  /// @}

  /// @brief Convert to string
  std::string toString() const;

private:
  template <class T, std::size_t N>
  inline std::vector<T> toVec(const T (&value)[N]) {
    return std::vector<T>(value, value + N);
  }

  template <class T, std::size_t N>
  inline std::vector<T> toVec(const std::array<T, N>& value) {
    return std::vector<T>(value.begin(), value.end());
  }

private:
  /// Data of the variable
  DataType data_;

  /// Type of the variable
  UniformType type_;
};

/// @brief Stream UniformVariable
/// @ingroup render
SEQUOIA_API extern std::ostream& operator<<(std::ostream& os, const UniformVariable& var);

} // namespace render

using UniformVariable = render::UniformVariable;

} // namespace sequoia

#endif
