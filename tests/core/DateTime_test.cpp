#include "../catch.hpp"
#include <Xli/DateTime.h>

using namespace Xli;

TEST_CASE("ConvertsToAndFromTimestamp")
{
    DateTime now = DateTime::Now();
    DateTime same = DateTime(now.ToTimestamp());
    REQUIRE(now.Year == same.Year);
    REQUIRE(now.Month == same.Month);
    REQUIRE(now.Day == same.Day);
    REQUIRE(now.Hour == same.Hour);
    REQUIRE(now.Minute == same.Minute);
    REQUIRE(now.Second == same.Second);
    REQUIRE(now.Millisecond == same.Millisecond);
}

