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

#ifndef SEQUOIA_ENGINE_CORE_PREPROCESSORUTIL_H
#define SEQUOIA_ENGINE_CORE_PREPROCESSORUTIL_H

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#define SEQUOIA_PP_SEQ_2_FILLER_0(X, Y) ((X, Y)) SEQUOIA_PP_SEQ_2_FILLER_1
#define SEQUOIA_PP_SEQ_2_FILLER_1(X, Y) ((X, Y)) SEQUOIA_PP_SEQ_2_FILLER_0
#define SEQUOIA_PP_SEQ_2_FILLER_0_END
#define SEQUOIA_PP_SEQ_2_FILLER_1_END

#define SEQUOIA_PP_SEQ_3_FILLER_0(X, Y, Z) ((X, Y, Z)) SEQUOIA_PP_SEQ_3_FILLER_1
#define SEQUOIA_PP_SEQ_3_FILLER_1(X, Y, Z) ((X, Y, Z)) SEQUOIA_PP_SEQ_3_FILLER_0
#define SEQUOIA_PP_SEQ_3_FILLER_0_END
#define SEQUOIA_PP_SEQ_3_FILLER_1_END

#define SEQUOIA_PP_SEQ_4_FILLER_0(X, Y, Z, W) ((X, Y, Z, W)) SEQUOIA_PP_SEQ_4_FILLER_1
#define SEQUOIA_PP_SEQ_4_FILLER_1(X, Y, Z, W) ((X, Y, Z, W)) SEQUOIA_PP_SEQ_4_FILLER_0
#define SEQUOIA_PP_SEQ_4_FILLER_0_END
#define SEQUOIA_PP_SEQ_4_FILLER_1_END

/// @brief The `SEQUOIA_PP_SEQ_X_FOR_EACH` macro repeats a macro for each element in a seq of tuples
/// of size X where X is one of 2, 3 or 4
///
/// This uses a clever trick to force the double parentheses arround a sequence. Given the sequence
/// `(int, x)(float, y)`, this produces `((int, x))((float, y))` and works the following:
///
///   1. Start with: `BOOST_PP_CAT(SEQUOIA_PP_SEQ_2_FILLER_0 (int,x)(float,y), _END)`
///   2. Expand `SEQUOIA_PP_SEQ_2_FILLER_0(A,B)` to `((A,B))SEQUOIA_PP_SEQ_2_FILLER_1`
///      yielding `BOOST_PP_CAT(((int,x))SEQUOIA_PP_SEQ_2_FILLER_1(float,y), _END)`
///   3. Expand `SEQUOIA_PP_SEQ_2_FILLER_1(A,B)` to `((A,B))SEQUOIA_PP_SEQ_2_FILLER_0`
///      yielding `BOOST_PP_CAT(((int,x))((float,y))SEQUOIA_PP_SEQ_2_FILLER_0, _END)`
///   4. Resolve `BOOST_PP_CAT` to get `((int,x))((float,y))SEQUOIA_PP_SEQ_2_FILLER_0_END`
///   5. Finally, expand `SEQUOIA_PP_SEQ_2_FILLER_0_END` to nothing yielding `((int,x))((float,y))`
///
/// @ingroup core
/// @{
#define SEQUOIA_PP_SEQ_2_FOR_EACH(Macro, Data, Sequence)                                           \
  BOOST_PP_SEQ_FOR_EACH(Macro, Data, BOOST_PP_CAT(SEQUOIA_PP_SEQ_2_FILLER_0 Sequence, _END))

#define SEQUOIA_PP_SEQ_3_FOR_EACH(Macro, Data, Sequence)                                           \
  BOOST_PP_SEQ_FOR_EACH(Macro, Data, BOOST_PP_CAT(SEQUOIA_PP_SEQ_3_FILLER_0 Sequence, _END))

#define SEQUOIA_PP_SEQ_4_FOR_EACH(Macro, Data, Sequence)                                           \
  BOOST_PP_SEQ_FOR_EACH(Macro, Data, BOOST_PP_CAT(SEQUOIA_PP_SEQ_4_FILLER_0 Sequence, _END))
/// @}

#endif
