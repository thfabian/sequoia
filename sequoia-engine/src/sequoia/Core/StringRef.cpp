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

#include "sequoia/Core/StringRef.h"
#include "sequoia/Core/Compiler.h"
#include <bitset>
#include <climits>
#include <iostream>

#ifdef SEQUOIA_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4146 4267)
#endif

namespace sequoia {

namespace core {

const size_t StringRef::npos;

static char ascii_tolower(char x) {
  if(x >= 'A' && x <= 'Z')
    return x - 'A' + 'a';
  return x;
}

static char ascii_toupper(char x) {
  if(x >= 'a' && x <= 'z')
    return x - 'a' + 'A';
  return x;
}

static bool ascii_isdigit(char x) { return x >= '0' && x <= '9'; }

// strncasecmp() is not available on non-POSIX systems, so define an
// alternative function here.
static int ascii_strncasecmp(const char* LHS, const char* RHS, size_t Length) {
  for(size_t I = 0; I < Length; ++I) {
    unsigned char LHC = ascii_tolower(LHS[I]);
    unsigned char RHC = ascii_tolower(RHS[I]);
    if(LHC != RHC)
      return LHC < RHC ? -1 : 1;
  }
  return 0;
}

int StringRef::compare_lower(StringRef RHS) const {
  if(int Res = ascii_strncasecmp(Data, RHS.Data, std::min(Length, RHS.Length)))
    return Res;
  if(Length == RHS.Length)
    return 0;
  return Length < RHS.Length ? -1 : 1;
}

bool StringRef::startswith_lower(StringRef Prefix) const {
  return Length >= Prefix.Length && ascii_strncasecmp(Data, Prefix.Data, Prefix.Length) == 0;
}

bool StringRef::endswith_lower(StringRef Suffix) const {
  return Length >= Suffix.Length &&
         ascii_strncasecmp(end() - Suffix.Length, Suffix.Data, Suffix.Length) == 0;
}

size_t StringRef::find_lower(char C, size_t From) const {
  char L = ascii_tolower(C);
  return find_if([L](char D) { return ascii_tolower(D) == L; }, From);
}

/// compare_numeric - Compare strings, handle embedded numbers.
int StringRef::compare_numeric(StringRef RHS) const {
  for(size_t I = 0, E = std::min(Length, RHS.Length); I != E; ++I) {
    // Check for sequences of digits.
    if(ascii_isdigit(Data[I]) && ascii_isdigit(RHS.Data[I])) {
      // The longer sequence of numbers is considered larger.
      // This doesn't really handle prefixed zeros well.
      size_t J;
      for(J = I + 1; J != E + 1; ++J) {
        bool ld = J < Length && ascii_isdigit(Data[J]);
        bool rd = J < RHS.Length && ascii_isdigit(RHS.Data[J]);
        if(ld != rd)
          return rd ? -1 : 1;
        if(!rd)
          break;
      }
      // The two number sequences have the same length (J-I), just memcmp them.
      if(int Res = compareMemory(Data + I, RHS.Data + I, J - I))
        return Res < 0 ? -1 : 1;
      // Identical number sequences, continue search after the numbers.
      I = J - 1;
      continue;
    }
    if(Data[I] != RHS.Data[I])
      return (unsigned char)Data[I] < (unsigned char)RHS.Data[I] ? -1 : 1;
  }
  if(Length == RHS.Length)
    return 0;
  return Length < RHS.Length ? -1 : 1;
}

std::string StringRef::lower() const {
  std::string Result(size(), char());
  for(size_type i = 0, e = size(); i != e; ++i) {
    Result[i] = ascii_tolower(Data[i]);
  }
  return Result;
}

std::string StringRef::upper() const {
  std::string Result(size(), char());
  for(size_type i = 0, e = size(); i != e; ++i) {
    Result[i] = ascii_toupper(Data[i]);
  }
  return Result;
}

size_t StringRef::find(StringRef Str, size_t From) const {
  if(From > Length)
    return npos;

  const char* Start = Data + From;
  size_t Size = Length - From;

  const char* Needle = Str.data();
  size_t N = Str.size();
  if(N == 0)
    return From;
  if(Size < N)
    return npos;
  if(N == 1) {
    const char* Ptr = (const char*)::memchr(Start, Needle[0], Size);
    return Ptr == nullptr ? npos : Ptr - Data;
  }

  const char* Stop = Start + (Size - N + 1);

  // For short haystacks or unsupported needles fall back to the naive algorithm
  if(Size < 16 || N > 255) {
    do {
      if(std::memcmp(Start, Needle, N) == 0)
        return Start - Data;
      ++Start;
    } while(Start < Stop);
    return npos;
  }

  // Build the bad char heuristic table, with uint8_t to reduce cache thrashing.
  uint8_t BadCharSkip[256];
  std::memset(BadCharSkip, N, 256);
  for(unsigned i = 0; i != N - 1; ++i)
    BadCharSkip[(uint8_t)Str[i]] = N - 1 - i;

  do {
    uint8_t Last = Start[N - 1];
    if(SEQUOIA_BUILTIN_UNLIKELY(Last == (uint8_t)Needle[N - 1]))
      if(std::memcmp(Start, Needle, N - 1) == 0)
        return Start - Data;

    // Otherwise skip the appropriate number of bytes.
    Start += BadCharSkip[Last];
  } while(Start < Stop);

  return npos;
}

size_t StringRef::find_lower(StringRef Str, size_t From) const {
  StringRef This = substr(From);
  while(This.size() >= Str.size()) {
    if(This.startswith_lower(Str))
      return From;
    This = This.drop_front();
    ++From;
  }
  return npos;
}

size_t StringRef::rfind_lower(char C, size_t From) const {
  From = std::min(From, Length);
  size_t i = From;
  while(i != 0) {
    --i;
    if(ascii_tolower(Data[i]) == ascii_tolower(C))
      return i;
  }
  return npos;
}

size_t StringRef::rfind(StringRef Str) const {
  size_t N = Str.size();
  if(N > Length)
    return npos;
  for(size_t i = Length - N + 1, e = 0; i != e;) {
    --i;
    if(substr(i, N).equals(Str))
      return i;
  }
  return npos;
}

size_t StringRef::rfind_lower(StringRef Str) const {
  size_t N = Str.size();
  if(N > Length)
    return npos;
  for(size_t i = Length - N + 1, e = 0; i != e;) {
    --i;
    if(substr(i, N).equals_lower(Str))
      return i;
  }
  return npos;
}

StringRef::size_type StringRef::find_first_of(StringRef Chars, size_t From) const {
  std::bitset<1 << CHAR_BIT> CharBits;
  for(size_type i = 0; i != Chars.size(); ++i)
    CharBits.set((unsigned char)Chars[i]);

  for(size_type i = std::min(From, Length), e = Length; i != e; ++i)
    if(CharBits.test((unsigned char)Data[i]))
      return i;
  return npos;
}

StringRef::size_type StringRef::find_first_not_of(char C, size_t From) const {
  for(size_type i = std::min(From, Length), e = Length; i != e; ++i)
    if(Data[i] != C)
      return i;
  return npos;
}

StringRef::size_type StringRef::find_first_not_of(StringRef Chars, size_t From) const {
  std::bitset<1 << CHAR_BIT> CharBits;
  for(size_type i = 0; i != Chars.size(); ++i)
    CharBits.set((unsigned char)Chars[i]);

  for(size_type i = std::min(From, Length), e = Length; i != e; ++i)
    if(!CharBits.test((unsigned char)Data[i]))
      return i;
  return npos;
}

StringRef::size_type StringRef::find_last_of(StringRef Chars, size_t From) const {
  std::bitset<1 << CHAR_BIT> CharBits;
  for(size_type i = 0; i != Chars.size(); ++i)
    CharBits.set((unsigned char)Chars[i]);

  for(size_type i = std::min(From, Length) - 1, e = -1; i != e; --i)
    if(CharBits.test((unsigned char)Data[i]))
      return i;
  return npos;
}

StringRef::size_type StringRef::find_last_not_of(char C, size_t From) const {
  for(size_type i = std::min(From, Length) - 1, e = -1; i != e; --i)
    if(Data[i] != C)
      return i;
  return npos;
}

StringRef::size_type StringRef::find_last_not_of(StringRef Chars, size_t From) const {
  std::bitset<1 << CHAR_BIT> CharBits;
  for(size_type i = 0, e = Chars.size(); i != e; ++i)
    CharBits.set((unsigned char)Chars[i]);

  for(size_type i = std::min(From, Length) - 1, e = -1; i != e; --i)
    if(!CharBits.test((unsigned char)Data[i]))
      return i;
  return npos;
}

void StringRef::split(std::vector<StringRef>& A, StringRef Separator, int MaxSplit,
                      bool KeepEmpty) const {
  StringRef S = *this;

  // Count down from MaxSplit. When MaxSplit is -1, this will just split
  // "forever". This doesn't support splitting more than 2^31 times
  // intentionally; if we ever want that we can make MaxSplit a 64-bit integer
  // but that seems unlikely to be useful.
  while(MaxSplit-- != 0) {
    size_t Idx = S.find(Separator);
    if(Idx == npos)
      break;

    // Push this split.
    if(KeepEmpty || Idx > 0)
      A.push_back(S.slice(0, Idx));

    // Jump forward.
    S = S.slice(Idx + Separator.size(), npos);
  }

  // Push the tail.
  if(KeepEmpty || !S.empty())
    A.push_back(S);
}

void StringRef::split(std::vector<StringRef>& A, char Separator, int MaxSplit,
                      bool KeepEmpty) const {
  StringRef S = *this;

  // Count down from MaxSplit. When MaxSplit is -1, this will just split
  // "forever". This doesn't support splitting more than 2^31 times
  // intentionally; if we ever want that we can make MaxSplit a 64-bit integer
  // but that seems unlikely to be useful.
  while(MaxSplit-- != 0) {
    size_t Idx = S.find(Separator);
    if(Idx == npos)
      break;

    // Push this split.
    if(KeepEmpty || Idx > 0)
      A.push_back(S.slice(0, Idx));

    // Jump forward.
    S = S.slice(Idx + 1, npos);
  }

  // Push the tail.
  if(KeepEmpty || !S.empty())
    A.push_back(S);
}

size_t StringRef::count(StringRef Str) const {
  size_t Count = 0;
  size_t N = Str.size();
  if(N > Length)
    return 0;
  for(size_t i = 0, e = Length - N + 1; i != e; ++i)
    if(substr(i, N).equals(Str))
      ++Count;
  return Count;
}

std::ostream& operator<<(std::ostream& stream, const StringRef& S) { return (stream << S.str()); }

} // namespace core

} // namespace sequoia

#ifdef SEQUOIA_COMPILER_MSVC
#pragma warning(pop)
#endif
