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

#ifndef SEQUOIA_MATH_MATH_H
#define SEQUOIA_MATH_MATH_H

#include "sequoia/Math/Constants.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <string>

namespace sequoia {

namespace math {

using namespace glm;

class Radian;
class Degree;

/// @brief Wrapper class which indicates a given angle value is in Radians
///
/// Radian values are interchangeable with Degree values, and conversions will be done automatically
/// between them.
///
/// @ingroup math
class Radian {
  float rad_;

public:
  /// @brief Construct with radian `r`
  explicit Radian(float r = 0.0f) : rad_(r) {}

  /// @brief Construct from Degree `d`
  Radian(const Degree& d);

  /// @brief Copy construct
  Radian(const Radian&) = default;

  /// @name Assignment
  /// @{
  Radian& operator=(const float& r) noexcept {
    rad_ = r;
    return *this;
  }
  Radian& operator=(const Radian& r) noexcept = default;
  Radian& operator=(const Degree& d) noexcept;
  /// @}

  /// @name Addition
  /// @{
  Radian operator+(const Radian& r) const noexcept { return Radian(rad_ + r.rad_); }
  Radian operator+(const Degree& d) const noexcept;
  /// @}

  /// @name Substract
  /// @{
  Radian operator-(const Radian& r) const noexcept { return Radian(rad_ - r.rad_); }
  Radian operator-(const Degree& d) const noexcept;
  /// @}

  /// @name Increment
  /// @{
  Radian& operator+=(const Radian& r) noexcept {
    rad_ += r.rad_;
    return *this;
  }
  Radian& operator+=(const Degree& d) noexcept;
  /// @}

  /// @name Decrement
  /// @{
  Radian& operator-=(const Radian& r) noexcept {
    rad_ -= r.rad_;
    return *this;
  }
  Radian& operator-=(const Degree& d) noexcept;
  /// @}

  /// @name Comparison
  /// @{
  bool operator<(const Radian& r) const noexcept { return rad_ < r.rad_; }
  bool operator<=(const Radian& r) const noexcept { return rad_ <= r.rad_; }
  bool operator==(const Radian& r) const noexcept { return rad_ == r.rad_; }
  bool operator!=(const Radian& r) const noexcept { return rad_ != r.rad_; }
  bool operator>=(const Radian& r) const noexcept { return rad_ >= r.rad_; }
  bool operator>(const Radian& r) const noexcept { return rad_ > r.rad_; }
  /// @}

  float inDegrees() const noexcept;
  float inRadians() const noexcept { return rad_; }

  /// @brief Construct from `Degree`
  static inline Radian fromDegree(float deg) noexcept {
    return Radian(deg * math::constantsf::deg2rad);
  }
  static inline Radian fromDegree(const Degree& deg) noexcept;

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Wrapper class which indicates a given angle value is in Degrees
///
/// Degree values are interchangeable with Radian values, and conversions will be done automatically
/// between them.
///
/// @ingroup math
class Degree {
  float deg_;

public:
  /// @brief Construct with degree `d`
  explicit Degree(float d = 0.0f) : deg_(d) {}

  /// @brief Construct from Radian `r`
  Degree(const Radian& r) : deg_(r.inDegrees()) {}

  /// @brief Copy-construct
  Degree(const Degree& d) = default;

  /// @name Assignment
  /// @{
  Degree& operator=(const float& r) noexcept {
    deg_ = r;
    return *this;
  }
  Degree& operator=(const Degree& d) noexcept = default;
  Degree& operator=(const Radian& r) noexcept {
    deg_ = r.inDegrees();
    return *this;
  }
  /// @}

  /// @name Addition
  /// @{
  Degree operator+(const Degree& d) const noexcept { return Degree(deg_ + d.deg_); }
  Degree operator+(const Radian& r) const noexcept { return Degree(deg_ + r.inDegrees()); }
  /// @}

  /// @name Substract
  /// @{
  Degree operator-(const Degree& d) const noexcept { return Degree(deg_ - d.deg_); }
  Degree operator-(const Radian& r) const noexcept { return Degree(deg_ - r.inDegrees()); }
  /// @}

  /// @name Increment
  /// @{
  Degree& operator+=(const Degree& d) noexcept {
    deg_ += d.deg_;
    return *this;
  }
  Degree& operator+=(const Radian& r) noexcept {
    deg_ += r.inDegrees();
    return *this;
  }
  /// @}

  /// @name Decrement
  /// @{
  Degree& operator-=(const Degree& d) noexcept {
    deg_ -= d.deg_;
    return *this;
  }
  Degree& operator-=(const Radian& r) noexcept {
    deg_ -= r.inDegrees();
    return *this;
  }
  /// @}

  /// @name Comparison
  /// @{
  bool operator<(const Degree& d) const noexcept { return deg_ < d.deg_; }
  bool operator<=(const Degree& d) const noexcept { return deg_ <= d.deg_; }
  bool operator==(const Degree& d) const noexcept { return deg_ == d.deg_; }
  bool operator!=(const Degree& d) const noexcept { return deg_ != d.deg_; }
  bool operator>=(const Degree& d) const noexcept { return deg_ >= d.deg_; }
  bool operator>(const Degree& d) const noexcept { return deg_ > d.deg_; }
  /// @}

  float inDegrees() const noexcept { return deg_; }
  float inRadians() const noexcept { return deg_ * math::constantsf::deg2rad; }

  /// @brief Construct from `Radian`
  static inline Degree fromRadian(float rad) noexcept {
    return Degree(rad * math::constantsf::rad2deg);
  }
  static inline Degree fromRadian(const Radian& rad) noexcept {
    return Degree::fromRadian(rad.inRadians());
  }

  /// @brief Convert to string
  std::string toString() const;
};

inline Radian Radian::fromDegree(const Degree& deg) noexcept {
  return Radian::fromDegree(deg.inDegrees());
}

inline float Radian::inDegrees() const noexcept { return rad_ * math::constantsf::rad2deg; }

inline Radian::Radian(const Degree& d) : rad_(d.inRadians()) {}

inline Radian& Radian::operator=(const Degree& d) noexcept {
  rad_ = d.inRadians();
  return *this;
}

inline Radian Radian::operator+(const Degree& d) const noexcept {
  return Radian(rad_ + d.inRadians());
}
inline Radian& Radian::operator+=(const Degree& d) noexcept {
  rad_ += d.inRadians();
  return *this;
}

inline Radian Radian::operator-(const Degree& d) const noexcept {
  return Radian(rad_ - d.inRadians());
}
inline Radian& Radian::operator-=(const Degree& d) noexcept {
  rad_ -= d.inRadians();
  return *this;
}

} // namespace math

} // namspace sequoia

#endif
