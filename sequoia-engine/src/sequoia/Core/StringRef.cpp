//===-- sequoia/Core/StringRef.cpp --------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Represent a constant reference to a string, i.e. a character array and a length, which need not
/// be null terminated.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/EditDistance.h"
#include "sequoia/Core/SmallVector.h"
#include "sequoia/Core/StringRef.h"
#include <bitset>
#include <climits>
#include <iostream>

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

/// compare_lower - Compare strings, ignoring case.
int StringRef::compare_lower(StringRef RHS) const {
  if(int Res = ascii_strncasecmp(Data, RHS.Data, std::min(Length, RHS.Length)))
    return Res;
  if(Length == RHS.Length)
    return 0;
  return Length < RHS.Length ? -1 : 1;
}

/// Check if this string starts with the given \p Prefix, ignoring case.
bool StringRef::startswith_lower(StringRef Prefix) const {
  return Length >= Prefix.Length && ascii_strncasecmp(Data, Prefix.Data, Prefix.Length) == 0;
}

/// Check if this string ends with the given \p Suffix, ignoring case.
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

// Compute the edit distance between the two given strings.
unsigned StringRef::edit_distance(sequoia::core::StringRef Other, bool AllowReplacements,
                                  unsigned MaxEditDistance) const {
  return sequoia::core::ComputeEditDistance(makeArrayRef(data(), size()),
                                            makeArrayRef(Other.data(), Other.size()),
                                            AllowReplacements, MaxEditDistance);
}

//===----------------------------------------------------------------------===//
// String Operations
//===----------------------------------------------------------------------===//

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

//===----------------------------------------------------------------------===//
// String Searching
//===----------------------------------------------------------------------===//

/// find - Search for the first string \arg Str in the string.
///
/// \return - The index of the first occurrence of \arg Str, or npos if not
/// found.
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

/// rfind - Search for the last string \arg Str in the string.
///
/// \return - The index of the last occurrence of \arg Str, or npos if not
/// found.
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

/// find_first_of - Find the first character in the string that is in \arg
/// Chars, or npos if not found.
///
/// Note: O(size() + Chars.size())
StringRef::size_type StringRef::find_first_of(StringRef Chars, size_t From) const {
  std::bitset<1 << CHAR_BIT> CharBits;
  for(size_type i = 0; i != Chars.size(); ++i)
    CharBits.set((unsigned char)Chars[i]);

  for(size_type i = std::min(From, Length), e = Length; i != e; ++i)
    if(CharBits.test((unsigned char)Data[i]))
      return i;
  return npos;
}

/// find_first_not_of - Find the first character in the string that is not
/// \arg C or npos if not found.
StringRef::size_type StringRef::find_first_not_of(char C, size_t From) const {
  for(size_type i = std::min(From, Length), e = Length; i != e; ++i)
    if(Data[i] != C)
      return i;
  return npos;
}

/// find_first_not_of - Find the first character in the string that is not
/// in the string \arg Chars, or npos if not found.
///
/// Note: O(size() + Chars.size())
StringRef::size_type StringRef::find_first_not_of(StringRef Chars, size_t From) const {
  std::bitset<1 << CHAR_BIT> CharBits;
  for(size_type i = 0; i != Chars.size(); ++i)
    CharBits.set((unsigned char)Chars[i]);

  for(size_type i = std::min(From, Length), e = Length; i != e; ++i)
    if(!CharBits.test((unsigned char)Data[i]))
      return i;
  return npos;
}

/// find_last_of - Find the last character in the string that is in \arg C,
/// or npos if not found.
///
/// Note: O(size() + Chars.size())
StringRef::size_type StringRef::find_last_of(StringRef Chars, size_t From) const {
  std::bitset<1 << CHAR_BIT> CharBits;
  for(size_type i = 0; i != Chars.size(); ++i)
    CharBits.set((unsigned char)Chars[i]);

  for(size_type i = std::min(From, Length) - 1, e = -1; i != e; --i)
    if(CharBits.test((unsigned char)Data[i]))
      return i;
  return npos;
}

/// find_last_not_of - Find the last character in the string that is not
/// \arg C, or npos if not found.
StringRef::size_type StringRef::find_last_not_of(char C, size_t From) const {
  for(size_type i = std::min(From, Length) - 1, e = -1; i != e; --i)
    if(Data[i] != C)
      return i;
  return npos;
}

/// find_last_not_of - Find the last character in the string that is not in
/// \arg Chars, or npos if not found.
///
/// Note: O(size() + Chars.size())
StringRef::size_type StringRef::find_last_not_of(StringRef Chars, size_t From) const {
  std::bitset<1 << CHAR_BIT> CharBits;
  for(size_type i = 0, e = Chars.size(); i != e; ++i)
    CharBits.set((unsigned char)Chars[i]);

  for(size_type i = std::min(From, Length) - 1, e = -1; i != e; --i)
    if(!CharBits.test((unsigned char)Data[i]))
      return i;
  return npos;
}

void StringRef::split(SmallVectorImpl<StringRef>& A, StringRef Separator, int MaxSplit,
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

void StringRef::split(SmallVectorImpl<StringRef>& A, char Separator, int MaxSplit,
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

//===----------------------------------------------------------------------===//
// Helpful Algorithms
//===----------------------------------------------------------------------===//

/// count - Return the number of non-overlapped occurrences of \arg Str in
/// the string.
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

static unsigned GetAutoSenseRadix(StringRef& Str) {
  if(Str.empty())
    return 10;

  if(Str.startswith("0x") || Str.startswith("0X")) {
    Str = Str.substr(2);
    return 16;
  }

  if(Str.startswith("0b") || Str.startswith("0B")) {
    Str = Str.substr(2);
    return 2;
  }

  if(Str.startswith("0o")) {
    Str = Str.substr(2);
    return 8;
  }

  if(Str[0] == '0' && Str.size() > 1 && ascii_isdigit(Str[1])) {
    Str = Str.substr(1);
    return 8;
  }

  return 10;
}

bool consumeUnsignedInteger(StringRef& Str, unsigned Radix, unsigned long long& Result) {
  // Autosense radix if not specified.
  if(Radix == 0)
    Radix = GetAutoSenseRadix(Str);

  // Empty strings (after the radix autosense) are invalid.
  if(Str.empty())
    return true;

  // Parse all the bytes of the string given this radix.  Watch for overflow.
  StringRef Str2 = Str;
  Result = 0;
  while(!Str2.empty()) {
    unsigned CharVal;
    if(Str2[0] >= '0' && Str2[0] <= '9')
      CharVal = Str2[0] - '0';
    else if(Str2[0] >= 'a' && Str2[0] <= 'z')
      CharVal = Str2[0] - 'a' + 10;
    else if(Str2[0] >= 'A' && Str2[0] <= 'Z')
      CharVal = Str2[0] - 'A' + 10;
    else
      break;

    // If the parsed value is larger than the integer radix, we cannot
    // consume any more characters.
    if(CharVal >= Radix)
      break;

    // Add in this character.
    unsigned long long PrevResult = Result;
    Result = Result * Radix + CharVal;

    // Check for overflow by shifting back and seeing if bits were lost.
    if(Result / Radix < PrevResult)
      return true;

    Str2 = Str2.substr(1);
  }

  // We consider the operation a failure if no characters were consumed
  // successfully.
  if(Str.size() == Str2.size())
    return true;

  Str = Str2;
  return false;
}

bool consumeSignedInteger(StringRef& Str, unsigned Radix, long long& Result) {
  unsigned long long ULLVal;

  // Handle positive strings first.
  if(Str.empty() || Str.front() != '-') {
    if(consumeUnsignedInteger(Str, Radix, ULLVal) ||
       // Check for value so large it overflows a signed value.
       (long long)ULLVal < 0)
      return true;
    Result = ULLVal;
    return false;
  }

  // Get the positive part of the value.
  StringRef Str2 = Str.drop_front(1);
  if(consumeUnsignedInteger(Str2, Radix, ULLVal) ||
     // Reject values so large they'd overflow as negative signed, but allow
     // "-0".  This negates the unsigned so that the negative isn't undefined
     // on signed overflow.
     (long long)-ULLVal > 0)
    return true;

  Str = Str2;
  Result = -ULLVal;
  return false;
}

/// GetAsUnsignedInteger - Workhorse method that converts a integer character
/// sequence of radix up to 36 to an unsigned long long value.
bool getAsUnsignedInteger(StringRef Str, unsigned Radix, unsigned long long& Result) {
  if(consumeUnsignedInteger(Str, Radix, Result))
    return true;

  // For getAsUnsignedInteger, we require the whole string to be consumed or
  // else we consider it a failure.
  return !Str.empty();
}

bool getAsSignedInteger(StringRef Str, unsigned Radix, long long& Result) {
  if(consumeSignedInteger(Str, Radix, Result))
    return true;

  // For getAsSignedInteger, we require the whole string to be consumed or else
  // we consider it a failure.
  return !Str.empty();
}

std::ostream& operator<<(std::ostream& stream, const StringRef& S) {
  return (stream <<  S.str());
}

} // namespace core

} // namespace sequoia
