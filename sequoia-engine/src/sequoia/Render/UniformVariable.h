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

#ifndef SEQUOIA_RENDER_UNIFORMVARIABLE_H
#define SEQUOIA_RENDER_UNIFORMVARIABLE_H

#include "sequoia/Core/Any.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Export.h"
#include <iosfwd>

namespace sequoia {

namespace render {

/// @brief Type of uniform variables
/// @ingroup render
enum class UniformType { Invalid, Int, Float };

/// @brief Stream UniformType
/// @ingroup render
std::ostream& operator<<(std::ostream& os, UniformType type);

namespace internal {

template <UniformType Enum>
struct InvalidUniformTypeToType {
  using type = void;
};

template <UniformType Enum>
struct UniformTypeToType {
  using type = typename InvalidUniformTypeToType<Enum>::type;
  static_assert(!std::is_same<type, void>::value, "invalid UniformType");
};

#define UNIFORM_VARIABLE_TYPE(Type, Enum, Name)                                                    \
  template <>                                                                                      \
  struct UniformTypeToType<Enum> {                                                                 \
    using type = Type;                                                                             \
  };
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE

template <class T>
struct InvalidTypeToUniformType {
  static constexpr UniformType value = UniformType::Invalid;
};

template <class T>
struct TypeToUniformType {
  static constexpr UniformType value = InvalidTypeToUniformType<void>::value;
  static_assert(value == UniformType::Invalid, "invalid type");
};

#define UNIFORM_VARIABLE_TYPE(Type, Enum, Name)                                                    \
  template <>                                                                                      \
  struct TypeToUniformType<Type> {                                                                 \
    static constexpr UniformType value = Enum;                                                     \
  };
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE

} // namespace internal

/// @brief Uniform variable
/// @ingroup render
class SEQUOIA_API UniformVariable {

  /// Data of the variable
  core::any data_;

  /// Type of the variable
  UniformType type_;

  /// Name of the variable
  std::string name_;

public:
  /// @brief Get the data of the variable as type `T`
  /// @throws core::Exception  type of the uniform variable is not of type `T`
  template <class T>
  const T& get() const {
    if(!isOfType<T>())
      SEQUOIA_THROW(core::Exception, "invalid type '%s' of uniform variable '%s', expected '%s' ",
                    internal::TypeToUniformType<T>::value, name_, type_);
    return *core::any_cast<T>(&data_);
  }

  /// @brief Set data of the variable (this copies the `data`)
  template <class T>
  void set(const T& data) {
    type_ = internal::TypeToUniformType<T>::value;
    data_ = core::any(data);
  }

  /// @brief Check if uniform variable is of type `T`
  template <class T>
  bool isOfType() const noexcept {
    return internal::TypeToUniformType<T>::value == type_;
  }

  /// @brief Set the name of the variable
  void setName(const std::string& name) { name_ = name; }

  /// @brief Get the name of the variable
  const std::string& getName() const noexcept { return name_; }

  /// @brief Get the type of the variable
  UniformType getType() const noexcept { return type_; }

  /// @brief Convert to string
  std::string toString() const;
};

} // namespace render

} // namespace sequoia

#endif
