#include "../catch.hpp"
#include <Xli/Unicode.h>

using Xli::Unicode;
using Xli::Utf16;

TEST_CASE("ToUpper")
{
    REQUIRE(0 == Unicode::ToUpper(0));
    REQUIRE('.' == Unicode::ToUpper('.'));
    REQUIRE('A' == Unicode::ToUpper('a'));
    REQUIRE('A' == Unicode::ToUpper('A'));
    REQUIRE(0xc5 == Unicode::ToUpper(0xe5)); //æ
}

TEST_CASE("ToLower")
{
    REQUIRE(0 == Unicode::ToLower(0));
    REQUIRE('.' == Unicode::ToLower('.'));
    REQUIRE('a' == Unicode::ToLower('a'));
    REQUIRE('a' == Unicode::ToLower('A'));
    REQUIRE(0xe5 == Unicode::ToLower(0xc5)); //æ
}
