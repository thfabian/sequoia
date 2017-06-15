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

#ifndef SEQUOIA_MATH_AXISALIGNEDBOX_H
#define SEQUOIA_MATH_AXISALIGNEDBOX_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Core/Export.h"
#include "sequoia/Math/Math.h"
#include <iosfwd>
#include <string>

namespace sequoia {

namespace math {

/// @brief A 3D box aligned with the x/y/z axes
///
/// This class represents a simple box which is aligned with the axes. Internally it only stores 2
/// points as the extremeties of the box, one which is the minima of all 3 axes, and the other which
/// is the maxima of all 3 axes. This class is typically used for an axis-aligned bounding box
/// (AABB) for collision and visibility determination.
///
/// @ingroup math
class SEQUOIA_API AxisAlignedBox {
public:
  /// @brief Type of extent
  enum ExtentKind { EK_Null, EK_Finite, EK_Infinite };

  /// @brief Enumeration of corners
  /// @verbatim
  ///
  ///         1-------2
  ///        /|      /|
  ///       / |     / |
  ///      5-------4  |
  ///      |  0----|--3
  ///      | /     | /
  ///      |/      |/
  ///      6-------7
  ///
  /// minimum_ = 0
  /// maxumum_ = 4
  ///
  /// @endverbatim
  enum CornerKind {
    CK_FarLeftBottom = 0,
    CK_FarLeftTop = 1,
    CK_FarRightTop = 2,
    CK_FarRightBottom = 3,
    CK_NearRightBottom = 7,
    CK_NearLeftBottom = 6,
    CK_NearLeftTop = 5,
    CK_NearRightTop = 4
  };

  /// @brief Empty box
  inline AxisAlignedBox() : minimum_(0), maximum_(0), extent_(EK_Null) {}

  /// @brief Initialize with `minimum` and `maximum`
  inline AxisAlignedBox(const vec3& minimum, const vec3& maximum)
      : minimum_(minimum), maximum_(maximum), extent_(EK_Finite) {}

  /// @brief Get the minimum corner of the box
  inline const vec3& getMinimum() const { return minimum_; }
  inline vec3& getMinimum() { return minimum_; }

  /// @brief Get the maximum corner of the box
  inline const vec3& getMaximum() const { return maximum_; }
  inline vec3& getMaximum() { return maximum_; }

  /// @brief Sets the minimum corner of the box
  inline void setMinimum(const vec3& vec) {
    extent_ = EK_Finite;
    minimum_ = vec;
  }

  /// @brief  Sets the maximum corner of the box
  inline void setMaximum(const vec3& vec) {
    extent_ = EK_Finite;
    maximum_ = vec;
  }

  /// @brief Sets both minimum and maximum extents.
  inline void setExtents(const vec3& min, const vec3& max) {
    SEQUOIA_ASSERT_MSG((min.x <= max.x && min.y <= max.y && min.z <= max.z),
                       "minimum corner of the box must be less than or equal to maximum corner");
    extent_ = EK_Finite;
    minimum_ = min;
    maximum_ = max;
  }

  /// @brief Get the position of one of the corners
  inline vec3 getCorner(CornerKind corner) const {
    switch(corner) {
    case CK_FarLeftBottom:
      return minimum_;
    case CK_FarLeftTop:
      return vec3(minimum_.x, maximum_.y, minimum_.z);
    case CK_FarRightTop:
      return vec3(maximum_.x, maximum_.y, minimum_.z);
    case CK_FarRightBottom:
      return vec3(maximum_.x, minimum_.y, minimum_.z);
    case CK_NearRightBottom:
      return vec3(maximum_.x, minimum_.y, maximum_.z);
    case CK_NearLeftBottom:
      return vec3(minimum_.x, minimum_.y, maximum_.z);
    case CK_NearLeftTop:
      return vec3(minimum_.x, maximum_.y, maximum_.z);
    case CK_NearRightTop:
      return maximum_;
    default:
      sequoia_unreachable("invalid corner");
    }
  }

  /// @brief Merges the `other` into the current box
  ///
  /// The result is the box which encompasses both.
  void merge(const AxisAlignedBox& other) {
    // Do nothing if rhs null, or this is infinite
    if((other.extent_ == EK_Null) || (extent_ == EK_Infinite)) {
      return;
    }
    // Otherwise if rhs is infinite, make this infinite, too
    else if(other.extent_ == EK_Infinite) {
      extent_ = EK_Infinite;
    }
    // Otherwise if current null, just take rhs
    else if(extent_ == EK_Null) {
      setExtents(other.minimum_, other.maximum_);
    }
    // Otherwise merge
    else {
      setExtents(math::min(minimum_, other.minimum_), math::max(maximum_, other.maximum_));
    }
  }

  /// @brief Extends the box to encompass the specified point (if needed)
  inline void merge(const vec3& point) {
    switch(extent_) {
    case EK_Null:
      setExtents(point, point);
      break;
    case EK_Finite:
      setExtents(math::min(minimum_, point), math::max(maximum_, point));
      break;
    case EK_Infinite:
      break;
    }
  }

  /// @brief Sets the box to a `null` value i.e. not a box
  inline void setNull() { extent_ = EK_Null; }

  /// @brief Returns true if the box is null i.e. empty
  inline bool isNull(void) const { return (extent_ == EK_Null); }

  /// @brief Returns true if the box is finite
  inline bool isFinite(void) const { return (extent_ == EK_Finite); }

  /// @brief  Sets the box to `infinite`
  inline void setInfinite() { extent_ = EK_Infinite; }

  /// @brief Returns true if the box is infinite
  inline bool isInfinite(void) const { return (extent_ == EK_Infinite); }

  /// @brief Returns whether or not this box intersects `other`
  inline bool intersects(const AxisAlignedBox& other) const {

    // Early-fail for nulls
    if(isNull() || other.isNull())
      return false;

    // Early-success for infinites
    if(isInfinite() || other.isInfinite())
      return true;

    // Use up to 6 separating planes
    if(maximum_.x < other.minimum_.x)
      return false;
    if(maximum_.y < other.minimum_.y)
      return false;
    if(maximum_.z < other.minimum_.z)
      return false;

    if(minimum_.x > other.maximum_.x)
      return false;
    if(minimum_.y > other.maximum_.y)
      return false;
    if(minimum_.z > other.maximum_.z)
      return false;

    // otherwise, must be intersecting
    return true;
  }

  /// @brief Comparison
  /// @{
  bool operator==(const AxisAlignedBox& other) const noexcept {
    return minimum_ == other.minimum_ && maximum_ == other.maximum_ && extent_ == other.extent_;
  }
  bool operator!=(const AxisAlignedBox& other) const noexcept { return !(*this == other); }
  /// @}

  /// @brief Clear the box
  void clear() {
    setExtents(vec3(0), vec3(0));
    extent_ = EK_Null;
  }

  /// @brief Convert to string
  std::string toString() const;

private:
  vec3 minimum_;
  vec3 maximum_;
  ExtentKind extent_;
};

} // namespace math

} // namespace sequoia

#endif
