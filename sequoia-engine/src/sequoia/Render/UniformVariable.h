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

#include "sequoia/Core/Export.h"
#include "sequoia/Math/Math.h"
#include "sequoia/Render/Exception.h"
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <iosfwd>

namespace sequoia {

namespace render {

/// @brief Type of uniform variables
/// @ingroup render
enum class UniformType {
  Invalid,
#define UNIFORM_VARIABLE_TYPE(Type, Name) Name,
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
};

/// @brief Stream UniformType
/// @ingroup render
SEQUOIA_API extern std::ostream& operator<<(std::ostream& os, UniformType type);

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

#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  template <>                                                                                      \
  struct UniformTypeToType<UniformType::Name> {                                                    \
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
  static_assert(value != UniformType::Invalid, "invalid type");
};

#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  template <>                                                                                      \
  struct TypeToUniformType<Type> {                                                                 \
    static constexpr UniformType value = UniformType::Name;                                        \
  };
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE

} // namespace internal

/// @brief Uniform variable
/// @ingroup render
class SEQUOIA_API UniformVariable {
public:
  struct InvalidData {
    bool operator==(const InvalidData& other) const noexcept { return true; }
    bool operator!=(const InvalidData& other) const noexcept { return false; }
  };

  using DataType = boost::variant<
#define UNIFORM_VARIABLE_TYPE(Type, Name) Type,
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
      InvalidData>;

  /// @brief Construct with `name` and `data`
  /// @param data   Data of the uniform variable
  template <class T>
  UniformVariable(const T& data) {
    this->set(data);
  }

  UniformVariable() : data_(InvalidData{}), type_(UniformType::Invalid) {}
  UniformVariable(const UniformVariable&) = default;
  UniformVariable(UniformVariable&&) = default;

  UniformVariable& operator=(const UniformVariable&) = default;
  UniformVariable& operator=(UniformVariable&&) = default;

#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  UniformVariable& operator=(const Type& value) {                                                  \
    this->set<Type>(value);                                                                        \
    return *this;                                                                                  \
  }
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE

  /// @brief Get the data of the variable as type `T`
  /// @throws core::Exception  type of the uniform variable is not of type `T`
  template <class T>
  inline const T& get() const {
    if(!isOfType<T>()) {
      UniformType type = internal::TypeToUniformType<T>::value;
      SEQUOIA_THROW(RenderSystemException, "invalid type '%s' of uniform variable, expected '%s'",
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
