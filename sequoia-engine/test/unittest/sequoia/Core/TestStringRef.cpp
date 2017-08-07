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
#include <gtest/gtest.h>

using namespace sequoia::core;

namespace sequoia {

std::ostream& operator<<(std::ostream& OS, const std::pair<StringRef, StringRef>& P);

std::ostream& operator<<(std::ostream& OS, const std::pair<StringRef, StringRef>& P) {
  OS << "(" << P.first << ", " << P.second << ")";
  return OS;
}

} // namespace sequoia

// Check that we can't accidentally assign a temporary std::string to a StringRef. (Unfortunately
// we can't make use of the same thing with constructors.)
//
// Disable this check under MSVC; even MSVC 2015 isn't consistent between std::is_assignable and
// actually writing such an assignment.
#if !defined(SEQUOIA_COMPILER_MSVC)
static_assert(!std::is_assignable<StringRef, std::string>::value,
              "Assigning from prvalue std::string");
static_assert(!std::is_assignable<StringRef, std::string&&>::value,
              "Assigning from xvalue std::string");
static_assert(std::is_assignable<StringRef, std::string&>::value,
              "Assigning from lvalue std::string");
static_assert(std::is_assignable<StringRef, const char*>::value, "Assigning from prvalue C string");
static_assert(std::is_assignable<StringRef, const char*&&>::value,
              "Assigning from xvalue C string");
static_assert(std::is_assignable<StringRef, const char*&>::value, "Assigning from lvalue C string");
#endif

namespace {

TEST(StringRefTest, Construction) {
  EXPECT_EQ("", StringRef());
  EXPECT_EQ("hello", StringRef("hello"));
  EXPECT_EQ("hello", StringRef("hello world", 5));
  EXPECT_EQ("hello", StringRef(std::string("hello")));
}

TEST(StringRefTest, EmptyInitializerList) {
  StringRef S = {};
  EXPECT_TRUE(S.empty());

  S = {};
  EXPECT_TRUE(S.empty());
}

TEST(StringRefTest, Iteration) {
  StringRef S("hello");
  const char* p = "hello";
  for(const char *it = S.begin(), *ie = S.end(); it != ie; ++it, ++p)
    EXPECT_EQ(*it, *p);
}

TEST(StringRefTest, StringOps) {
  const char* p = "hello";
  EXPECT_EQ(p, StringRef(p, 0).data());
  EXPECT_TRUE(StringRef().empty());
  EXPECT_EQ((size_t)5, StringRef("hello").size());
  EXPECT_EQ(-1, StringRef("aab").compare("aad"));
  EXPECT_EQ(0, StringRef("aab").compare("aab"));
  EXPECT_EQ(1, StringRef("aab").compare("aaa"));
  EXPECT_EQ(-1, StringRef("aab").compare("aabb"));
  EXPECT_EQ(1, StringRef("aab").compare("aa"));
  EXPECT_EQ(1, StringRef("\xFF").compare("\1"));

  EXPECT_EQ(-1, StringRef("AaB").compare_lower("aAd"));
  EXPECT_EQ(0, StringRef("AaB").compare_lower("aab"));
  EXPECT_EQ(1, StringRef("AaB").compare_lower("AAA"));
  EXPECT_EQ(-1, StringRef("AaB").compare_lower("aaBb"));
  EXPECT_EQ(-1, StringRef("AaB").compare_lower("bb"));
  EXPECT_EQ(1, StringRef("aaBb").compare_lower("AaB"));
  EXPECT_EQ(1, StringRef("bb").compare_lower("AaB"));
  EXPECT_EQ(1, StringRef("AaB").compare_lower("aA"));
  EXPECT_EQ(1, StringRef("\xFF").compare_lower("\1"));

  EXPECT_EQ(-1, StringRef("aab").compare_numeric("aad"));
  EXPECT_EQ(0, StringRef("aab").compare_numeric("aab"));
  EXPECT_EQ(1, StringRef("aab").compare_numeric("aaa"));
  EXPECT_EQ(-1, StringRef("aab").compare_numeric("aabb"));
  EXPECT_EQ(1, StringRef("aab").compare_numeric("aa"));
  EXPECT_EQ(-1, StringRef("1").compare_numeric("10"));
  EXPECT_EQ(0, StringRef("10").compare_numeric("10"));
  EXPECT_EQ(0, StringRef("10a").compare_numeric("10a"));
  EXPECT_EQ(1, StringRef("2").compare_numeric("1"));
  EXPECT_EQ(1, StringRef("\xFF").compare_numeric("\1"));
  EXPECT_EQ(1, StringRef("V16").compare_numeric("V1_q0"));
  EXPECT_EQ(-1, StringRef("V1_q0").compare_numeric("V16"));
  EXPECT_EQ(-1, StringRef("V8_q0").compare_numeric("V16"));
  EXPECT_EQ(1, StringRef("V16").compare_numeric("V8_q0"));
  EXPECT_EQ(-1, StringRef("V1_q0").compare_numeric("V8_q0"));
  EXPECT_EQ(1, StringRef("V8_q0").compare_numeric("V1_q0"));
}

TEST(StringRefTest, Operators) {
  EXPECT_EQ("", StringRef());
  EXPECT_TRUE(StringRef("aab") < StringRef("aad"));
  EXPECT_FALSE(StringRef("aab") < StringRef("aab"));
  EXPECT_TRUE(StringRef("aab") <= StringRef("aab"));
  EXPECT_FALSE(StringRef("aab") <= StringRef("aaa"));
  EXPECT_TRUE(StringRef("aad") > StringRef("aab"));
  EXPECT_FALSE(StringRef("aab") > StringRef("aab"));
  EXPECT_TRUE(StringRef("aab") >= StringRef("aab"));
  EXPECT_FALSE(StringRef("aaa") >= StringRef("aab"));
  EXPECT_EQ(StringRef("aab"), StringRef("aab"));
  EXPECT_FALSE(StringRef("aab") == StringRef("aac"));
  EXPECT_FALSE(StringRef("aab") != StringRef("aab"));
  EXPECT_TRUE(StringRef("aab") != StringRef("aac"));
  EXPECT_EQ('a', StringRef("aab")[1]);
}

TEST(StringRefTest, Substr) {
  StringRef Str("hello");
  EXPECT_EQ("lo", Str.substr(3));
  EXPECT_EQ("", Str.substr(100));
  EXPECT_EQ("hello", Str.substr(0, 100));
  EXPECT_EQ("o", Str.substr(4, 10));
}

TEST(StringRefTest, Slice) {
  StringRef Str("hello");
  EXPECT_EQ("l", Str.slice(2, 3));
  EXPECT_EQ("ell", Str.slice(1, 4));
  EXPECT_EQ("llo", Str.slice(2, 100));
  EXPECT_EQ("", Str.slice(2, 1));
  EXPECT_EQ("", Str.slice(10, 20));
}

TEST(StringRefTest, Split) {
  StringRef Str("hello");
  EXPECT_EQ(std::make_pair(StringRef("hello"), StringRef("")), Str.split('X'));
  EXPECT_EQ(std::make_pair(StringRef("h"), StringRef("llo")), Str.split('e'));
  EXPECT_EQ(std::make_pair(StringRef(""), StringRef("ello")), Str.split('h'));
  EXPECT_EQ(std::make_pair(StringRef("he"), StringRef("lo")), Str.split('l'));
  EXPECT_EQ(std::make_pair(StringRef("hell"), StringRef("")), Str.split('o'));

  EXPECT_EQ(std::make_pair(StringRef("hello"), StringRef("")), Str.rsplit('X'));
  EXPECT_EQ(std::make_pair(StringRef("h"), StringRef("llo")), Str.rsplit('e'));
  EXPECT_EQ(std::make_pair(StringRef(""), StringRef("ello")), Str.rsplit('h'));
  EXPECT_EQ(std::make_pair(StringRef("hel"), StringRef("o")), Str.rsplit('l'));
  EXPECT_EQ(std::make_pair(StringRef("hell"), StringRef("")), Str.rsplit('o'));
}

TEST(StringRefTest, Split2) {
  std::vector<StringRef> parts;
  std::vector<StringRef> expected;

  expected.push_back("ab");
  expected.push_back("c");
  StringRef(",ab,,c,").split(parts, ",", -1, false);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("");
  expected.push_back("ab");
  expected.push_back("");
  expected.push_back("c");
  expected.push_back("");
  StringRef(",ab,,c,").split(parts, ",", -1, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("");
  StringRef("").split(parts, ",", -1, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  StringRef("").split(parts, ",", -1, false);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  StringRef(",").split(parts, ",", -1, false);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("");
  expected.push_back("");
  StringRef(",").split(parts, ",", -1, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a");
  expected.push_back("b");
  StringRef("a,b").split(parts, ",", -1, true);
  EXPECT_TRUE(parts == expected);

  // Test MaxSplit
  expected.clear();
  parts.clear();
  expected.push_back("a,,b,c");
  StringRef("a,,b,c").split(parts, ",", 0, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a,,b,c");
  StringRef("a,,b,c").split(parts, ",", 0, false);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a");
  expected.push_back(",b,c");
  StringRef("a,,b,c").split(parts, ",", 1, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a");
  expected.push_back(",b,c");
  StringRef("a,,b,c").split(parts, ",", 1, false);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a");
  expected.push_back("");
  expected.push_back("b,c");
  StringRef("a,,b,c").split(parts, ",", 2, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a");
  expected.push_back("b,c");
  StringRef("a,,b,c").split(parts, ",", 2, false);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a");
  expected.push_back("");
  expected.push_back("b");
  expected.push_back("c");
  StringRef("a,,b,c").split(parts, ",", 3, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a");
  expected.push_back("b");
  expected.push_back("c");
  StringRef("a,,b,c").split(parts, ",", 3, false);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a");
  expected.push_back("b");
  expected.push_back("c");
  StringRef("a,,b,c").split(parts, ',', 3, false);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("");
  StringRef().split(parts, ",", 0, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back(StringRef());
  StringRef("").split(parts, ",", 0, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  StringRef("").split(parts, ",", 0, false);
  EXPECT_TRUE(parts == expected);
  StringRef().split(parts, ",", 0, false);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a");
  expected.push_back("");
  expected.push_back("b");
  expected.push_back("c,d");
  StringRef("a,,b,c,d").split(parts, ",", 3, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("");
  StringRef().split(parts, ',', 0, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back(StringRef());
  StringRef("").split(parts, ',', 0, true);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  StringRef("").split(parts, ',', 0, false);
  EXPECT_TRUE(parts == expected);
  StringRef().split(parts, ',', 0, false);
  EXPECT_TRUE(parts == expected);

  expected.clear();
  parts.clear();
  expected.push_back("a");
  expected.push_back("");
  expected.push_back("b");
  expected.push_back("c,d");
  StringRef("a,,b,c,d").split(parts, ',', 3, true);
  EXPECT_TRUE(parts == expected);
}

TEST(StringRefTest, Trim) {
  StringRef Str0("hello");
  StringRef Str1(" hello ");
  StringRef Str2("  hello  ");

  EXPECT_EQ(StringRef("hello"), Str0.rtrim());
  EXPECT_EQ(StringRef(" hello"), Str1.rtrim());
  EXPECT_EQ(StringRef("  hello"), Str2.rtrim());
  EXPECT_EQ(StringRef("hello"), Str0.ltrim());
  EXPECT_EQ(StringRef("hello "), Str1.ltrim());
  EXPECT_EQ(StringRef("hello  "), Str2.ltrim());
  EXPECT_EQ(StringRef("hello"), Str0.trim());
  EXPECT_EQ(StringRef("hello"), Str1.trim());
  EXPECT_EQ(StringRef("hello"), Str2.trim());

  EXPECT_EQ(StringRef("ello"), Str0.trim("hhhhhhhhhhh"));

  EXPECT_EQ(StringRef(""), StringRef("").trim());
  EXPECT_EQ(StringRef(""), StringRef(" ").trim());
  EXPECT_EQ(StringRef("\0", 1), StringRef(" \0 ", 3).trim());
  EXPECT_EQ(StringRef("\0\0", 2), StringRef("\0\0", 2).trim());
  EXPECT_EQ(StringRef("x"), StringRef("\0\0x\0\0", 5).trim('\0'));
}

TEST(StringRefTest, StartsWith) {
  StringRef Str("hello");
  EXPECT_TRUE(Str.startswith(""));
  EXPECT_TRUE(Str.startswith("he"));
  EXPECT_FALSE(Str.startswith("helloworld"));
  EXPECT_FALSE(Str.startswith("hi"));
}

TEST(StringRefTest, StartsWithLower) {
  StringRef Str("heLLo");
  EXPECT_TRUE(Str.startswith_lower(""));
  EXPECT_TRUE(Str.startswith_lower("he"));
  EXPECT_TRUE(Str.startswith_lower("hell"));
  EXPECT_TRUE(Str.startswith_lower("HELlo"));
  EXPECT_FALSE(Str.startswith_lower("helloworld"));
  EXPECT_FALSE(Str.startswith_lower("hi"));
}

TEST(StringRefTest, ConsumeFront) {
  StringRef Str("hello");
  EXPECT_TRUE(Str.consume_front(""));
  EXPECT_EQ("hello", Str);
  EXPECT_TRUE(Str.consume_front("he"));
  EXPECT_EQ("llo", Str);
  EXPECT_FALSE(Str.consume_front("lloworld"));
  EXPECT_EQ("llo", Str);
  EXPECT_FALSE(Str.consume_front("lol"));
  EXPECT_EQ("llo", Str);
  EXPECT_TRUE(Str.consume_front("llo"));
  EXPECT_EQ("", Str);
  EXPECT_FALSE(Str.consume_front("o"));
  EXPECT_TRUE(Str.consume_front(""));
}

TEST(StringRefTest, EndsWith) {
  StringRef Str("hello");
  EXPECT_TRUE(Str.endswith(""));
  EXPECT_TRUE(Str.endswith("lo"));
  EXPECT_FALSE(Str.endswith("helloworld"));
  EXPECT_FALSE(Str.endswith("worldhello"));
  EXPECT_FALSE(Str.endswith("so"));
}

TEST(StringRefTest, EndsWithLower) {
  StringRef Str("heLLo");
  EXPECT_TRUE(Str.endswith_lower(""));
  EXPECT_TRUE(Str.endswith_lower("lo"));
  EXPECT_TRUE(Str.endswith_lower("LO"));
  EXPECT_TRUE(Str.endswith_lower("ELlo"));
  EXPECT_FALSE(Str.endswith_lower("helloworld"));
  EXPECT_FALSE(Str.endswith_lower("hi"));
}

TEST(StringRefTest, ConsumeBack) {
  StringRef Str("hello");
  EXPECT_TRUE(Str.consume_back(""));
  EXPECT_EQ("hello", Str);
  EXPECT_TRUE(Str.consume_back("lo"));
  EXPECT_EQ("hel", Str);
  EXPECT_FALSE(Str.consume_back("helhel"));
  EXPECT_EQ("hel", Str);
  EXPECT_FALSE(Str.consume_back("hle"));
  EXPECT_EQ("hel", Str);
  EXPECT_TRUE(Str.consume_back("hel"));
  EXPECT_EQ("", Str);
  EXPECT_FALSE(Str.consume_back("h"));
  EXPECT_TRUE(Str.consume_back(""));
}

TEST(StringRefTest, Find) {
  StringRef Str("helloHELLO");
  StringRef LongStr("hellx xello hell ello world foo bar hello HELLO");

  struct {
    StringRef Str;
    char C;
    std::size_t From;
    std::size_t Pos;
    std::size_t LowerPos;
  } CharExpectations[] = {
      {Str, 'h', 0U, 0U, 0U},
      {Str, 'e', 0U, 1U, 1U},
      {Str, 'l', 0U, 2U, 2U},
      {Str, 'l', 3U, 3U, 3U},
      {Str, 'o', 0U, 4U, 4U},
      {Str, 'L', 0U, 7U, 2U},
      {Str, 'z', 0U, StringRef::npos, StringRef::npos},
  };

  struct {
    StringRef Str;
    StringRef S;
    std::size_t From;
    std::size_t Pos;
    std::size_t LowerPos;
  } StrExpectations[] = {{Str, "helloword", 0, StringRef::npos, StringRef::npos},
                         {Str, "hello", 0, 0U, 0U},
                         {Str, "ello", 0, 1U, 1U},
                         {Str, "zz", 0, StringRef::npos, StringRef::npos},
                         {Str, "ll", 2U, 2U, 2U},
                         {Str, "ll", 3U, StringRef::npos, 7U},
                         {Str, "LL", 2U, 7U, 2U},
                         {Str, "LL", 3U, 7U, 7U},
                         {Str, "", 0U, 0U, 0U},
                         {LongStr, "hello", 0U, 36U, 36U},
                         {LongStr, "foo", 0U, 28U, 28U},
                         {LongStr, "hell", 2U, 12U, 12U},
                         {LongStr, "HELL", 2U, 42U, 12U},
                         {LongStr, "", 0U, 0U, 0U}};

  for(auto& E : CharExpectations) {
    EXPECT_EQ(E.Pos, E.Str.find(E.C, E.From));
    EXPECT_EQ(E.LowerPos, E.Str.find_lower(E.C, E.From));
    EXPECT_EQ(E.LowerPos, E.Str.find_lower(toupper(E.C), E.From));
  }

  for(auto& E : StrExpectations) {
    EXPECT_EQ(E.Pos, E.Str.find(E.S, E.From));
    EXPECT_EQ(E.LowerPos, E.Str.find_lower(E.S, E.From));
    EXPECT_EQ(E.LowerPos, E.Str.find_lower(E.S.upper(), E.From));
  }

  EXPECT_EQ(3U, Str.rfind('l'));
  EXPECT_EQ(StringRef::npos, Str.rfind('z'));
  EXPECT_EQ(StringRef::npos, Str.rfind("helloworld"));
  EXPECT_EQ(0U, Str.rfind("hello"));
  EXPECT_EQ(1U, Str.rfind("ello"));
  EXPECT_EQ(StringRef::npos, Str.rfind("zz"));

  EXPECT_EQ(8U, Str.rfind_lower('l'));
  EXPECT_EQ(8U, Str.rfind_lower('L'));
  EXPECT_EQ(StringRef::npos, Str.rfind_lower('z'));
  EXPECT_EQ(StringRef::npos, Str.rfind_lower("HELLOWORLD"));
  EXPECT_EQ(5U, Str.rfind("HELLO"));
  EXPECT_EQ(6U, Str.rfind("ELLO"));
  EXPECT_EQ(StringRef::npos, Str.rfind("ZZ"));

  EXPECT_EQ(2U, Str.find_first_of('l'));
  EXPECT_EQ(1U, Str.find_first_of("el"));
  EXPECT_EQ(StringRef::npos, Str.find_first_of("xyz"));

  Str = "hello";
  EXPECT_EQ(1U, Str.find_first_not_of('h'));
  EXPECT_EQ(4U, Str.find_first_not_of("hel"));
  EXPECT_EQ(StringRef::npos, Str.find_first_not_of("hello"));

  EXPECT_EQ(3U, Str.find_last_not_of('o'));
  EXPECT_EQ(1U, Str.find_last_not_of("lo"));
  EXPECT_EQ(StringRef::npos, Str.find_last_not_of("helo"));
}

TEST(StringRefTest, Count) {
  StringRef Str("hello");
  EXPECT_EQ(2U, Str.count('l'));
  EXPECT_EQ(1U, Str.count('o'));
  EXPECT_EQ(0U, Str.count('z'));
  EXPECT_EQ(0U, Str.count("helloworld"));
  EXPECT_EQ(1U, Str.count("hello"));
  EXPECT_EQ(1U, Str.count("ello"));
  EXPECT_EQ(0U, Str.count("zz"));
}

TEST(StringRefTest, Drop) {
  StringRef S("StringRefTest::Drop");

  StringRef Dropped = S.drop_front(5);
  EXPECT_EQ(Dropped, "gRefTest::Drop");

  Dropped = S.drop_back(5);
  EXPECT_EQ(Dropped, "StringRefTest:");

  Dropped = S.drop_front(0);
  EXPECT_EQ(Dropped, S);

  Dropped = S.drop_back(0);
  EXPECT_EQ(Dropped, S);

  Dropped = S.drop_front(S.size());
  EXPECT_TRUE(Dropped.empty());

  Dropped = S.drop_back(S.size());
  EXPECT_TRUE(Dropped.empty());
}

TEST(StringRefTest, Take) {
  StringRef S("StringRef::Take");

  StringRef Taken = S.take_front(5);
  EXPECT_EQ(Taken, "Strin");

  Taken = S.take_back(5);
  EXPECT_EQ(Taken, ":Take");

  Taken = S.take_front(S.size());
  EXPECT_EQ(Taken, S);

  Taken = S.take_back(S.size());
  EXPECT_EQ(Taken, S);

  Taken = S.take_front(0);
  EXPECT_TRUE(Taken.empty());

  Taken = S.take_back(0);
  EXPECT_TRUE(Taken.empty());
}

TEST(StringRefTest, FindIf) {
  StringRef Punct("Test.String");
  StringRef NoPunct("ABCDEFG");
  StringRef Empty;

  auto IsPunct = [](char c) { return ::ispunct(c); };
  auto IsAlpha = [](char c) { return ::isalpha(c); };
  EXPECT_EQ(4U, Punct.find_if(IsPunct));
  EXPECT_EQ(StringRef::npos, NoPunct.find_if(IsPunct));
  EXPECT_EQ(StringRef::npos, Empty.find_if(IsPunct));

  EXPECT_EQ(4U, Punct.find_if_not(IsAlpha));
  EXPECT_EQ(StringRef::npos, NoPunct.find_if_not(IsAlpha));
  EXPECT_EQ(StringRef::npos, Empty.find_if_not(IsAlpha));
}

TEST(StringRefTest, TakeWhileUntil) {
  StringRef S("String With 1 Number");

  StringRef Taken = S.take_while([](char c) { return ::isdigit(c); });
  EXPECT_EQ("", Taken);

  Taken = S.take_until([](char c) { return ::isdigit(c); });
  EXPECT_EQ("String With ", Taken);

  Taken = S.take_while([](char c) { return true; });
  EXPECT_EQ(S, Taken);

  Taken = S.take_until([](char c) { return true; });
  EXPECT_EQ("", Taken);

  S = "";
  Taken = S.take_while([](char c) { return true; });
  EXPECT_EQ("", Taken);
}

TEST(StringRefTest, DropWhileUntil) {
  StringRef S("String With 1 Number");

  StringRef Taken = S.drop_while([](char c) { return ::isdigit(c); });
  EXPECT_EQ(S, Taken);

  Taken = S.drop_until([](char c) { return ::isdigit(c); });
  EXPECT_EQ("1 Number", Taken);

  Taken = S.drop_while([](char c) { return true; });
  EXPECT_EQ("", Taken);

  Taken = S.drop_until([](char c) { return true; });
  EXPECT_EQ(S, Taken);

  StringRef EmptyString = "";
  Taken = EmptyString.drop_while([](char c) { return true; });
  EXPECT_EQ("", Taken);
}

} // anonymous namespace
