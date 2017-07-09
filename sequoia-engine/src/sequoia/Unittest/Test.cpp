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

#include "sequoia/Unittest/Test.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include <cmath>
#include <sstream>

namespace sequoia {

namespace unittest {

namespace internal {

namespace {

static const char* Dimensions[] = {"X", "Y", "Z", "W"};

} // anonymous namespace

std::pair<bool, std::string> compareHelper(float expected, float actual, float absErr) {
  if(std::isnan(expected))
    return std::make_pair(false, "  abs(expected - actual) < absErr    FAILED\n"
                                 "\n"
                                 "  expected is nan\n");
  if(std::isnan(actual))
    return std::make_pair(false, "  abs(expected - actual) < absErr    FAILED\n"
                                 "\n"
                                 "  actual is nan\n");

  std::stringstream expected_ss;
  expected_ss << std::setprecision(std::numeric_limits<float>::digits10 + 2) << expected;

  std::stringstream actual_ss;
  actual_ss << std::setprecision(std::numeric_limits<float>::digits10 + 2) << actual;

  if(std::abs(expected - actual) > absErr)
    return std::make_pair(false, core::format("  abs(expected - actual) < absErr    FAILED\n"
                                              "\n"
                                              "  expected = %s\n"
                                              "  actual = %s\n"
                                              "  abs(expected - actual) = %f\n"
                                              "  absErr = %f\n",
                                              expected_ss.str(), actual_ss.str(),
                                              std::abs(expected - actual), absErr));

  return std::make_pair(true, std::string());
}

std::pair<bool, std::string> compareHelper(const float* expected, const float* actual,
                                           std::size_t size, float absErr, const char* expectedStr,
                                           const char* actualStr) {
  SEQUOIA_ASSERT(size <= 4);

  bool hasError = false;
  std::stringstream ss;

  for(std::size_t i = 0; i < size; ++i) {
    auto test = compareHelper(expected[i], actual[i], absErr);

    auto vecToStr = [&size](const float* ptr) {
      return core::RangeToString()(std::vector<float>(ptr, ptr + size));
    };

    if(!test.first) {
      if(!hasError) {
        ss << "ERROR:  abs(" << expectedStr << " - " << actualStr << ") < absErr\n"
           << "\n  " << expectedStr << " = " << vecToStr(expected) << "\n  " << actualStr << " = "
           << vecToStr(actual) << "\n";
        hasError = true;
      }
      ss << "\n  " << Dimensions[i] << ":\n  " << core::indent(test.second);
    }
  }

  return std::make_pair(!hasError, ss.str());
}

} // namespace internal

} // namespace unittest

} // namespace sequoia
